
// CListCtrlEx



#include "precompiled.h"

#include "ListCtrlEx.h"
#include "DialogEditLink.h"
#include "DialogSelectProperty.h"
#include "EditInPlace.h"
#include "FrameChild.h"
#include "NeoMem.h"

#include "BDataColumns.h"
#include "BDataDate.h"
#include "BDataLink.h"
#include "BDataNumber.h"
#include "BObject.h"
#include "ConstantsDatabase.h"
#include "BDoc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// Macros
//---------------------------------------------------------------------------

#define NM_MAX_TEXT 256 // for buffers

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_CHAR()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillFocus)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnNotifyDeleteItem)
	ON_NOTIFY_REFLECT(LVN_DELETEALLITEMS, OnNotifyDeleteAllItems)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, OnNotifyItemChanging)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetDispInfo)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEACTIVATE()
	ON_COMMAND(ID_COLUMN_SORT_ASCENDING, OnColumnSortAscending)
	ON_COMMAND(ID_COLUMN_SORT_DESCENDING, OnColumnSortDescending)
	ON_COMMAND(ID_COLUMN_EDIT_PROPERTY, OnColumnEditProperty)
	ON_COMMAND(ID_COLUMN_INSERT, OnColumnInsert)
	ON_COMMAND(ID_COLUMN_DELETE, OnColumnDelete)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RCLICK, 0, OnColumnRClick)
	ON_UPDATE_COMMAND_UI(ID_COLUMN_SORT_CLEAR, OnUpdateColumnSortClear)
	// don't ask me why this works, it just does.
	// this is for changing the width of a header, etc.
//	ON_NOTIFY_EX(HDN_ITEMCHANGED, 0, OnHeaderItemChanged)
	ON_NOTIFY_EX(HDN_ITEMCHANGEDW, 0, OnHeaderItemChanged)
	ON_NOTIFY_EX(HDN_ITEMCHANGEDA, 0, OnHeaderItemChanged)
	ON_NOTIFY_EX(HDN_ENDDRAG, 0, OnHeaderEndDrag)
END_MESSAGE_MAP()



// Construction/Destruction
//---------------------------------------------------------------------------

CListCtrlEx::CListCtrlEx() :
	m_pDoc(0),

	// Flags for mode of listview
	m_nMode(modeNormalListView),
	m_nFocusState(fsNone),
	m_bExpandLastColumn(FALSE),
	m_bExpandAllColumns(FALSE),
	m_bDisableFirstColumn(FALSE),
	m_bDisabledItems(FALSE),

	// Selected cell
	m_nRow(-1),
	m_nCol(-1),

	// Dummy row
	m_bDummyRow(FALSE),
	m_nNewIconIndex(0),
	
	// Columns
	m_bAutoColumns(FALSE),
	m_pdatColumns(NULL),
//	m_bColumnInfoChanged(FALSE),
//	m_bColumnInfoCopied(FALSE),
//	m_bColumnInfoSaved(FALSE),
	m_bColumnsChanged(FALSE),
	m_bSaveChangesAutomatically(FALSE),
	m_nTargetColumn(-1),

	// Sort
	m_lngSortPropertyID(0),
	m_iSortDirection(1),

	// Drag Drop
//	m_nDragOver(0),
	m_bMouseDown(FALSE),

//	GetColors()
	m_clrDisabledItems(0x00f08080) // light blue
{
}


CListCtrlEx::~CListCtrlEx()
{
	// If we still have a pointer to the column bdata, and it's a new one and wasn't saved, delete it
//	if (m_pdatColumns && (m_bColumnInfoCopied && !m_bColumnInfoSaved))
//		delete m_pdatColumns;
}


BOOL CListCtrlEx::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Set default styles for ListView control
	cs.style |= 
			LVS_REPORT  |
			LVS_SHAREIMAGELISTS |
			LVS_EDITLABELS |
			LVS_SHOWSELALWAYS |
			LVS_SINGLESEL | // bug: had left this out and so was able to select more than one item in Index viewetc!
			0;
	return CListCtrl::PreCreateWindow(cs);
}


int CListCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;	
	
	// Make sure listview and header are in ansi mode, not unicode!
//	ListView_SetUnicodeFormat(m_hWnd, FALSE);
//	CHeaderCtrl* ph = GetHeaderCtrl();
//	Header_SetUnicodeFormat(ph->m_hWnd, FALSE);

	// Get header control
//	m_phdr = GetHeaderCtrl();
//	ASSERT_VALID(m_phdr);

	return 0;
}





// Methods
//---------------------------------------------------------------------------




// ListView is requesting information to display an item
void CListCtrlEx::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LVITEM* pLVITEM = &(pDispInfo->item);

	if (pLVITEM->mask & LVIF_TEXT)
	{
		// Get object associated with specified item
		BObject* pobj = (BObject*) pLVITEM->lParam;
//		if ((pobj != 0) && (pobj != (BObject*) keyDummyRow))
		if (pobj)
		{
			ASSERT_VALID(pobj);
			// Get property associated with the specified column
			ULONG lngPropertyID = GetColumnPropertyID(pLVITEM->iSubItem);

			// pLVITEM->pszText is a pointer to a null-terminated string containing the item text. 
			// ie LPSTR = char*
			// but CStrings convert to LPCTSTR, ie const char*
			// but apparently the char* is because this member is used for both read and write.
			// so okay to just cast from const char* to char*, as we do below. 
			// see also http://social.msdn.microsoft.com/Forums/en-US/vcgeneral/thread/9b138faa-56aa-4179-bd93-92f8ebc9e3da/

			// Get object string
			// Note const_cast to remove const from text
			// Special case for Property View - need to display the value of the property for the
			// document's current object. The Value column has a dummy property of propValue assigned to it.
			if (lngPropertyID == propValue)
				pLVITEM->pszText = const_cast <TCHAR*> (m_pDoc->GetCurrentObject()->GetPropertyString(pobj->GetObjectID()));
			else
				pLVITEM->pszText = const_cast <TCHAR*> (pobj->GetPropertyString(lngPropertyID));
		}
	}
	*pResult = 0;
}




// Set state of all selected items
// eg pass LVIS_CUT, 0 to clear cut state, LVIS_CUT, LVIS_CUT to set it
void CListCtrlEx::SetSelectedItemsState(UINT nStateMask, UINT nState)
{
	int i = 0;
	LVITEM lvi;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = GetNextSelectedItem(pos);
		lvi.stateMask = nStateMask;
		lvi.state = nState;
		SetItemState(nItem, &lvi);
	}
}



// Find the item with the specified data
// Returns index of the item if successful or -1 if not2
//, make inline?
int CListCtrlEx::FindItemData(LPARAM lParam)
{
	LVFINDINFO lvfi;
	lvfi.flags = LVFI_PARAM;
	lvfi.lParam = lParam;
	return FindItem(&lvfi);
}


// When the control gets the focus, make sure the focus rectangle is visible on an item
// Note: This window gets the focus when the user is dragging something over it
// In that case, we don't want to draw the highlight because the drag drop code handles
// highlighting whatever item the cursor is over
// But how do we know that's the case?
// This might be why the default behavior is to just draw a focus rectangle around the first cell
// when the focus is first gained.
// Maybe on lose focus, we should set cell to -1, -1
// on set focus, set cell to 0, 0 and mode to focusrect
// then in click or arrows set mode to focushighlight
// but then we'll lose the current position if user is tabbing between views
void CListCtrlEx::OnSetFocus(CWnd* pOldWnd) 
{
	xTRACE("CListCtrlEx::OnSetFocus(pOldWnd=0x%p) - highlight current cell\n", pOldWnd);

	CListCtrl::OnSetFocus(pOldWnd);
	
	if (m_nMode & modeSelectCell)
	{
		// Select the first cell in the listview if none is currently selected
		// Also make sure the currently selected cell is still valid
		BOOL bNoneSelected = ((m_nRow == -1) || (m_nCol == -1));
		BOOL bInvalidCell = ((m_nRow >= GetItemCount()) || (m_nCol >= GetColumnCount()));
		if (bNoneSelected || bInvalidCell)
		{
			m_nRow = 0;
			m_nCol = (m_bDisableFirstColumn) ? 1 : 0; // If user can't edit first column, set to second column
		}

		// Set focus mode to highlight the current cell and redraw the current cell
		//, shouldn't SelectCell set focus state to fsHighlight?
		m_nFocusState = fsHighlight;
		InvalidateCurrentCell();
//		SelectCell(m_nRow, m_nCol);

		// Notify parent window of selected item
		GetParent()->SendMessage(CM_ITEM_SELECTED);
	}
	else if (m_nMode & modeNormalListView)
	{
		// If no item is currently selected, put focus rectangle on first one
		// Don't do it if we're in full row select mode though
		POSITION pos = GetFirstSelectedItemPosition();
		BOOL bFullRowSelectMode = (GetExtendedStyle() & LVS_EX_FULLROWSELECT);
		if ((pos != 0) && (GetItemCount() > 0) && !bFullRowSelectMode)
		{
			xTRACE("  modeNormal set focus to first item\n");
			SetItemState(0, LVIS_FOCUSED, LVIS_FOCUSED);
		}
	}
}


// We have lost focus, so update the drawing of the current cell if necessary
void CListCtrlEx::OnKillFocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	xTRACE("CListCtrlEx::OnKillFocus - if in cell select mode, draw current cell in inactive colors\n");
	if (m_nMode & modeSelectCell)
	{
		xTRACE("   erase highlight\n");
		// Set focus mode to none and redraw the current cell
//		m_nFocusState = fsNone;
		m_nFocusState = fsInactive;
		InvalidateCurrentCell();
		UpdateWindow(); // sends wm_paint
	}
	*pResult = 0;
}


// Select the specified item (select and set focus to it).
// Handles select cell mode and normal mode.
// Pass -1 to select all items.
void CListCtrlEx::SelectItem(int nItem)
{
	xTRACE("CListCtrlEx::SelectItem %d\n", nItem);
	if (m_nMode & modeSelectCell)
	{
		// If we're in cell select mode, select a cell using our own SelectCell method,
		// first making sure that the current column is valid.
		if (m_nCol < 0 || m_nCol >= GetColumnCount())
			m_nCol = 0;

		//. handle -1 for select all items
//		if (nItem == -1)
//		{
//
//		}
//		else
//		{
			// for now, just make it 0 if received -1
			if (nItem < 0 || nItem >= GetItemCount())
				nItem = 0;
			SelectCell(nItem, m_nCol);
//		}
	}
	else // Normal mode
	{
		// If in normal mode, select the item using the listview control's own method.
		UINT nMask = LVIS_SELECTED | LVIS_FOCUSED;
		UINT nState = 0;

		// Clear the currently selected items
		// If nItem is -1, then the state change is applied to all items
		SetItemState(-1, nState, nMask);

		// Select and set focus to the specified item(s)
		nState = nMask;
		SetItemState(nItem, nState, nMask);
		RedrawItems(nItem, nItem); // make sure item's new state gets drawn

		// Make sure it's visible
		if (nItem != -1)
		{
			// Note: GetCountPerPage may return -1 if listview is not visible yet
			// In that case, DON'T call EnsureVisible because it gets messed up
			int nPerPage = GetCountPerPage(); 
			if (nPerPage != -1)
			{
				EnsureVisible(nItem, TRUE);
			}
		}
	}
}



// Clear selected items
//. what about select cell mode?
void CListCtrlEx::ClearSelections()
{
	UINT nMask = LVIS_SELECTED | LVIS_FOCUSED;
	UINT nState = 0;
	SetItemState(-1, nState, nMask);
}


// Select an item based on the data passed (select and set focus to it).
// Returns the item's index.
// Note: This works for both normal and select cell modes.
int CListCtrlEx::SelectItemData(LPARAM lParam)
{
	int nItem = FindItemData(lParam);
	SelectItem(nItem);
	return nItem;
}


// Get the data associated with the currently selected item.
// Returns Null if no selected item.
// This handles select cell mode.
LPARAM CListCtrlEx::GetSelectedItemData()
{
	int nItem = GetSelectedItem();
	if (nItem == -1)
		return NULL;
	return GetItemData(nItem);
}



// Get the index of the first selected item, or returns -1 if none selected
// This handles select cell mode
int CListCtrlEx::GetSelectedItem()
{
	if (m_nMode & modeSelectCell)
	{
		if ((m_nRow != -1) && (m_nCol != -1))
			return m_nRow;
		else
			return -1;
	}
	else
	{
		POSITION pos = GetFirstSelectedItemPosition();
		if (pos)
		{
			int nItem = GetNextSelectedItem(pos);
			return nItem;
		}
		return -1;
	}
}




// Start edit of a sub item label (a cell).
// We define a helper function to set up the edit control. The helper function takes only the row 
// and column index of the subitem. EditSubItem() ensures that the row as well as the column 
// is visible before it creates the edit control. It then creates the edit control of the right size and 
// with the proper text justification. The edit control created is of the class CEditInPlace which 
// we will define later. 
// nItem: The row index of the item to edit.
// nCol: The column of the sub item (this is the column INDEX, not the ORDER!).
// Returns temporary pointer to the new edit control.
// Note: The returned pointer should not be saved.
// From codeguru.com
// See OnEndLabelEdit for reselection code
CEdit* CListCtrlEx::EditSubItem(int nItem, int nCol, LPCTSTR pszEditText /* = 0 */)
{
	xTRACE("CListCtrlEx::EditSubItem row %d col %d text %s\n", nItem, nCol, pszEditText);

	// Make sure that the item is visible
	if (!EnsureVisible(nItem, TRUE)) 
		return NULL;

	// Make sure that nCol is valid
	int nColumns = GetColumnCount();
	if (nCol >= nColumns || GetColumnWidth(nCol) < 5)
		return NULL;

	// Don't let user edit first column if flag is set
	// Bug: = instead of == again!!
	if ((m_bDisableFirstColumn) && (nCol == 0))
		return NULL;

	//, Check for LVS_EDITLABELS
//	if (!GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS) 
//		return NULL;

	// Refresh the display so that any highlighted cell color gets erased
	// Note: This will automatically disable the current cell when we set the focus to the edit control,
	// because the listview will receive the killfocus message, at which point it will disable the 
	// current cell and redraw it. So we don't need to do anything here.
//	m_nFocusState = fsNone;
//	InvalidateCurrentCell();
//	UpdateWindow();

	// Now scroll if we need to expose the column
	EnsureCellVisible(nItem, nCol);

	// Get position for edit control
	CRect r;
	GetCellRect(nItem, nCol, r);

	// Adjust for margins
	r.left += 4;
	r.right = r.left + GetColumnWidth(nCol) - 3;

	// Make sure it fits in this window
	CRect rClient;
	GetClientRect(&rClient);
	if (r.right > rClient.right) 
		r.right = rClient.right;

	// If on first column, shift to make room for icon
//,	// If cell has icon, shift to make room for it
	if (nCol == 0)
		r.left += 15;

	// Get column alignment and edit control style
	LVCOLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn(nCol, &lvcol);
	DWORD dwStyle;
	if ((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if ((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else dwStyle = ES_CENTER;
	dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;

	// Get starting text for edit control.
	// If user didn't pass any starting text, then use contents of cell.
	CString strEditText;
	if (pszEditText == 0)
		// Note: GetItemText has a bug in it so had to make my own version GetItemTextOP
		strEditText = GetItemTextOP(nItem, nCol);
	else
		strEditText = pszEditText;

	// Create CEditInPlace edit control, with text obtained from the cell (or what user passed to this routine).
	CEditInPlace *pEdit = new CEditInPlace (this, nItem, nCol, strEditText); // last term is CString (not a reference either)
	pEdit->Create(dwStyle, r, this, IDC_TXT_EDIT);
//	pEdit->SetFocus(); // added this - didn't help bomb problem
	// try this - no help
//	pEdit->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS);
//	pEdit->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return pEdit;
}



// Edit the currently selected cell in the listview.
// Returns a pointer to the edit control, or 0 if not able to edit property value.
CEdit* CListCtrlEx::EditCurrentCell(LPCTSTR pszEditText /* = 0 */)
{
	xTRACE("CListCtrlEx::EditCurrentCell %s\n", pszEditText);

	// Check if user is allowed to edit this cell
	//. check if read only flag is set also
//	BObject* pobjCurrent = (BObject*) m_lvw.GetItemData(m_lvw.m_nRow);
//	if ((DWORD) pobjCurrent == CListCtrlEx::keyDummyRow)
//	ULONG lngPropertyID = m_lvw.GetColumnPropertyID();
	ULONG lngPropertyID = GetColumnPropertyID();
	if (lngPropertyID)
	{
		BObject* pobjPropertyDef = 0;
		if (lngPropertyID == propValue)
			pobjPropertyDef = (BObject*) GetItemData(m_nRow);
		else
			pobjPropertyDef = m_pDoc->GetObject(lngPropertyID);
		ASSERT_VALID(pobjPropertyDef);
		BOOL bDisplayLinkHierarchy = pobjPropertyDef->GetPropertyLong(propDisplayLinkHierarchy);
		BObject* pobjAdditionalProp = pobjPropertyDef->GetPropertyLink(propAdditionalDisplayProperty);
		if (bDisplayLinkHierarchy || (pobjAdditionalProp != 0))
		{
			AfxMessageBox("Because this property displays a link hierarchy and/or an additional\n"
				"display property, you must edit this value using Edit Value in Dialog (F4).", MB_ICONINFORMATION);
			return 0;
		}
	}

	return EditSubItem(m_nRow, m_nCol, pszEditText);
}



// The CEditInPlace class is designed to destroy the edit control and delete the object when it 
// loses focus. Clicking on the scrollbars of the ListView control does not take away the focus 
// from the edit control. We therefore add handlers for the scrollbar messages which force focus 
// away from the edit control by setting the focus to the list view control itself. 
//. also erase focus rect because it gets messed up on scroll
void CListCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetFocus() != this) SetFocus();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetFocus() != this) SetFocus();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}



// Determine the row index and column index for a point.
// pt: point to be tested (in listctrl client coordinates)
// pnCol: pointer to integer that will hold the column index
// Returns the row index or -1 if point is not over a row (also sets nCol = -1 if not over a valid cell)
// Adapted from CodeGuru code
// Note: The row returned accounts for the TopIndex and all that.
// Note: We use this code because it's easier than SubItemHitTest.
int CListCtrlEx::HitTestEx(CPoint &pt, int* pnCol) const
{
  
	// Get the top and bottom row visible
	int nTopRow = GetTopIndex();
	int nBottomRow = nTopRow + GetCountPerPage();
	if (nBottomRow > GetItemCount())
		nBottomRow = GetItemCount();
	
	// Get header control and number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumns = pHeader->GetItemCount();

	// Loop through the visible rows
	for (int nRow = nTopRow; nRow <= nBottomRow; nRow++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect r;
		GetItemRect(nRow, &r, LVIR_BOUNDS);
		if (r.PtInRect(pt))
		{
			// It's in the row, so now find the column it's in, searching in column order.
			for (int nColOrder = 0; nColOrder < nColumns; nColOrder++)
			{
				int nColIndex = pHeader->OrderToIndex(nColOrder);
				int nColWidth = GetColumnWidth(nColIndex);
				if (pt.x >= r.left && pt.x <= (r.left + nColWidth))
				{
					// Found the cell - return the row and column
					if (pnCol)
						*pnCol = nColIndex;
					return nRow;
				}
				r.left += nColWidth;
			}
		}
	}

	// Not over a valid cell, return -1
	*pnCol = -1;
	return -1;
}




// This is where we'll highlight the currently selected cell and draw the focus rectangle!
// NM_CUSTOMDRAW handler.
void CListCtrlEx::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Returning this means to do the default drawing
	*pResult = CDRF_DODEFAULT;

	// Exit if we're in normal mode
	if (m_nMode == modeNormalListView)
		return;

	// Let control draw itself normally if mode is set to not draw focus or highlight
	// BUG: did it again! used = instead of ==!
	if ((m_nMode & modeSelectCell) && (m_nFocusState == fsNone))
		return;
	
	// For this notification, the structure given (NMHDR* pNMHDR) is actually a
	// NMLVCUSTOMDRAW structure that tells you what's going on with the custom
	// draw action. So, we'll need to cast to the larger structure. 
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW) pNMHDR;
	
	switch (lplvcd->nmcd.dwDrawStage)
	{

	case CDDS_PREPAINT:
		// Ask for item notifications, and a notification when the control is all painted
		*pResult = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		break;
		
	case CDDS_ITEMPREPAINT:
		// Ask for subitem notifications
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		//, could also color alternate rows here
//			int iRow = lplvcd->nmcd.dwItemSpec;
//			if (iRow & 1)
//			{
//				lplvcd->clrTextBk = RGB(255, 0, 0);
//				lplvcd->clrText = RGB(255, 255, 0);
//				*pResult = CDRF_NEWFONT;
//			}
		break;

	// This gets sent before each subitem (cell) is drawn
	// lplvcd->nmcd.dwItemSpec = row
	// lplvcd->iSubItem = column
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		{

			// Default is to always specify standard colors
			// Note: You have to change the colors for all cells, since otherwise once you change
			// the colors, all the remaining cells get that color also
			COLORREF clrBack = Library::clrWindow;
			COLORREF clrFore = Library::clrWindowText;
			*pResult = CDRF_NEWFONT;

			// Check if item is disabled, if so color entire row disabled
			// Note: Tried using cut state, itemdata 0, and stateimagemask before finally
			// using overlaymask!
			if (m_bDisabledItems)
			{
				int nItem = lplvcd->nmcd.dwItemSpec;
				if (GetItemState(nItem, LVIS_OVERLAYMASK) == stateDisabled)
				{
					clrFore = m_clrDisabledItems;
				}
			}

			//, could check if item is high priority and change color
//			if ()
//			{
//			}

			// Check if subitem being drawn is a hyperlink
			//. this will be too slow - will need to cache information somewhere
			ULONG lngPropertyID = GetColumnPropertyID(lplvcd->iSubItem);
			BObject* pobjPropertyDef = 0;
			// Check for special case in properties view
			//, bad oo code - this control shouldn't know this - just get from data associated with cell
			if (lngPropertyID == propValue)
				// get property associated with row
				pobjPropertyDef = (BObject*) GetItemData(lplvcd->nmcd.dwItemSpec);
			else
				// Normal contents view
				pobjPropertyDef = m_pDoc->GetObject(lngPropertyID);

			// Note: Had to put this if statement here because if property view wasn't entirely visible,
			// getitemdata was returning 0!
			xTRACE("  custom draw propid %d\n", lngPropertyID);
			if (pobjPropertyDef)
			{
				ASSERT_VALID(pobjPropertyDef);
				BObject* pobjPropType = pobjPropertyDef->GetPropertyLink(propPropertyType);
				if (pobjPropType)
				{
					ASSERT_VALID(pobjPropType);
					ULONG lngPropertyTypeID = pobjPropType->GetObjectID();
					xTRACE("     custom draw proptypeid %d\n", lngPropertyTypeID);
/*					if (lngPropertyTypeID == proptypeHyperlink)
					{
						//, make underline also
						clrFore = g_clrHyperlinkText;
					}
*/
					switch (lngPropertyTypeID) 
					{
						case proptypeHyperlink:
						case proptypeEmail:
						case proptypeWebsite:
						case proptypeFile:
						case proptypeFolder:
							clrFore = Library::clrHyperlinkText;
					}
				}
			}

			// Change colors if we're on selected cell
			if (m_nMode & modeSelectCell)
			{
				// Check to see if it's about to draw the currently selected cell.
				// If it is, change the colors.
				// Note: lplvcd->nmcd.dwItemSpec is the current row index.
				// Warning: cast from signed to unsigned: m_nRow is -1 sometimes.
				// dwItemSpec is an unsigned long. 
				// But it's okay because nRow will just become some very large number, 
				// and won't match dwItemSpec. 
				if (lplvcd->nmcd.dwItemSpec == (ULONG) m_nRow)
				{
					if (lplvcd->iSubItem == m_nCol)
					{
						xTRACE("CListCtrlEx::OnCustomDraw, on the selected col/row (%d/%d) - set to inactive or highlight colors\n", m_nCol, m_nRow);
						if (m_nFocusState == fsInactive)
						{
							clrBack = Library::clrInactive;
							clrFore = Library::clrInactiveText;
						}
						else // if (m_nFocusState == fsHighlight)
						{
							clrBack = Library::clrHighlight;
							clrFore = Library::clrHighlightText;
						}
					}
				} // == row
			} // modeSelectCell
			
			lplvcd->clrTextBk = clrBack;
			lplvcd->clrText = clrFore;
		
			break;
		}

	// After all painting is done, we want to draw the focus rectangle around the current item, if any
	case CDDS_POSTPAINT:
		{
			// Draw focus rectangle only if focus state is highlight or focusrect.
			// Also don't draw it if the current row is behind the header
			// BUG: = instead of == again
//			if (m_nMode == modeSelectCell)
			if (m_nMode & modeSelectCell)
			{
				int nTopRow = GetTopIndex();
				BOOL bDrawFocus = ((m_nFocusState == fsHighlight) || (m_nFocusState == fsFocus));
				BOOL bValidCell = ((m_nRow != -1) && (m_nCol != -1));
				BOOL bBehindHeader = (m_nRow < nTopRow);
				if (bDrawFocus && bValidCell & !bBehindHeader)
				{
					xTRACE("   draw focus rectangle around current cell (row %d, col %d)\n", m_nRow, m_nCol);
					CRect r;
					GetCellRect(m_nRow, m_nCol, r); // LVIR_BOUNDS, r);
					xTRACE("      left, top, right, bottom (%d, %d, %d, %d)\n", r.left, r.top, r.right, r.bottom);
					CClientDC dc(this);
					dc.DrawFocusRect(&r);
				}
			}
			break;
		}

//	default:
//		// We're not interested
//		*pResult = CDRF_DODEFAULT;
	}

}




// Get bounding rectangle of a cell in the listview.
// Necessary because GetSubItemRect handles first column oddly (returns entire row).
// Returns True if successful.
BOOL CListCtrlEx::GetCellRect(int nRow, int nColIndex, CRect &r, int nArea /* = LVIR_BOUNDS */)
{
	// If no row or col specified, set rect to zero and return
	if ((nRow == -1) || (nColIndex == -1))
	{
		r.SetRectEmpty();
		return FALSE;
	}

	// If not in column 0, just use regular routine
	if (nColIndex != 0)
		return GetSubItemRect(nRow, nColIndex, nArea, r);
	
	// If there's just one column in this listview, we can just use the GetItemRect routine to get the 
	// entire row area.
	if (GetColumnCount() == 1)
		return GetItemRect(nRow, r, nArea);

	// Get the next cell's position to get the right edge
	CRect rNextCell;
	int nOrder = IndexToOrder(0);
	nColIndex = OrderToIndex(nOrder + 1);
	if (!GetSubItemRect(nRow, nColIndex, nArea, rNextCell))
		return FALSE;

	// If column 0 is first column, then just get the rectangle for the entire row.
	if (nOrder == 0)
	{
		// Get rectangle for entire row.
		if (!GetItemRect(nRow, r, nArea))
			return FALSE;
	}
	else
	{
		// If column 0 has been moved, then we need to get the right edge of the previous cell (lame).
		// (You can't just get subitem rect for col 0 because it returns the whole row)
		nColIndex = OrderToIndex(nOrder - 1);
		if (!GetSubItemRect(nRow, nColIndex, nArea, r))
			return FALSE;
		r.left = r.right + 2;
	}

	// Change right edge to be the left edge of the next cell.
	r.right = rNextCell.left;
	
	return TRUE;
}




// Handle repositioning of current cell with arrow keys etc
void CListCtrlEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	xTRACE("CListCtrlEx::OnKeyDown\n");

	// Let control handle keydown if not in cell select mode
	if (!(m_nMode & modeSelectCell))
	{
		CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

    BOOL bControl = ::GetKeyState(VK_CONTROL) < 0;
    BOOL bShift = ::GetKeyState(VK_SHIFT) < 0;
	int nCountPerPage = GetCountPerPage();

	// Handle column order
	int nColOrder = IndexToOrder(m_nCol);

    switch (nChar)
    {
		case VK_LEFT:
				if (nColOrder > 0)
					SelectCellOrder(m_nRow, nColOrder - 1);
				else if (m_nRow > 0)
					SelectCellOrder(m_nRow - 1, GetColumnCount() - 1);
			return;

		case VK_RIGHT:
			if (nColOrder < GetColumnCount() - 1)
				SelectCellOrder(m_nRow, nColOrder + 1);
			else if (m_nRow < GetItemCount() - 1)
				SelectCellOrder(m_nRow + 1, 0);
			return;

		case VK_RETURN:
			if (nColOrder < GetColumnCount() - 1)
				SelectCellOrder(m_nRow, nColOrder + 1);
			else if (m_nRow < GetItemCount() - 1)
				SelectCellOrder(m_nRow + 1, 0);
			return;

		case VK_TAB:
			if (!bShift)
			{
				if (nColOrder < GetColumnCount() - 1)
					SelectCellOrder(m_nRow, nColOrder + 1);
				else if (m_nRow < GetItemCount() - 1)
					SelectCellOrder(m_nRow + 1, 0);
			}
			else
			{
				if (nColOrder > 0)
					SelectCellOrder(m_nRow, nColOrder - 1);
				else if (m_nRow > 0)
					SelectCellOrder(m_nRow - 1, GetColumnCount() - 1);
			}
			return;

		case VK_UP:
			if (m_nRow > 0)
				SelectCell(m_nRow - 1, m_nCol);
			return;

		case VK_DOWN:
			if (m_nRow < GetItemCount() - 1)
				SelectCell(m_nRow + 1, m_nCol);
			return;

		case VK_HOME:
			if (bControl)
				SelectCellOrder(0, 0);
			else
				SelectCellOrder(m_nRow, 0);
			return;

		case VK_END:
			if (bControl)
				SelectCellOrder(GetItemCount() - 1, GetColumnCount() - 1);
			else
				SelectCellOrder(m_nRow, GetColumnCount() - 1);
			return;

		case VK_PAGE_UP:
			if (m_nRow > nCountPerPage - 1)
				SelectCell(m_nRow - nCountPerPage, m_nCol);
			else
				SelectCell(0, m_nCol);
			return;

		case VK_PAGE_DOWN:
			if (m_nRow < GetItemCount() - nCountPerPage)
				SelectCell(m_nRow + nCountPerPage, m_nCol);
			else
				SelectCell(GetItemCount() - 1, m_nCol);
			return;

		//. Tell parent about this - treat like a double click on an item
//		case VK_RETURN:
//			CWnd* pwnd = GetParent();
//			pwnd->SendMessage(
//			return;

		// If user starts typing, start editing the current cell
//		default:
//			EditCurrentCell();
//			return;
    }   

	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}




void CListCtrlEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	xTRACE("CListCtrlEx::OnChar\n");

	BOOL bControl = ::GetKeyState(VK_CONTROL) < 0;
	BOOL bInvalidChar = ((nChar == VK_ESCAPE) | (nChar == VK_TAB) | (nChar == VK_RETURN));

	// If in cell select mode and not a control character or invalid character,
	// then start editing the current cell.
	if ((m_nMode & modeSelectCell) && !bControl && !bInvalidChar)
	{
		// Start editing the current cell, and pass it this character to start with
		CEdit* pwndEdit = EditCurrentCell();
		if (pwndEdit) // will be zero if no columns in view!
		{
			ASSERT_VALID(pwndEdit);
			TCHAR pszChar[2];
			pszChar[0] = nChar;
			pszChar[1] = 0;
			pwndEdit->SetWindowText(pszChar);
			// Position caret after the character we just sent it
			pwndEdit->SetSel(1, 1);
		}
	}
	else
	{
		// Call base class handler
		CListCtrl::OnChar(nChar, nRepCnt, nFlags);
	}
}




// Select a cell - invalidate current cell, set new position, ensure visible, invalidate new position.
// Also handles validating the new row and column.
// Note: nRow and nCol may be -1 which indicates no cell should be selected.
// Added bEnsureVisible to fix bug when you clicked on cell with mouse - listview would scroll up weirdly.
// Also sends a CM_ITEM_SELECTED message to parent window.
void CListCtrlEx::SelectCell(int nRow, int nCol, BOOL bEnsureVisible /* = TRUE */)
{
	xTRACE("CListCtrlEx::SelectCell row %d col %d\n", nRow, nCol);

	// Erase the current cell
	InvalidateCurrentCell();

	// Set to a valid cell if of out of range
	BOOL bInvalidCell = ((nRow >= GetItemCount()) || (nCol >= GetColumnCount()));
	if (bInvalidCell)
	{
		xTRACE("  invalid cell - setting to 0, 0\n");
		nRow = 0;
		nCol = 0;
	}

	// If not allowed to edit first column, make sure we're not trying to select a cell there
	if ((m_bDisableFirstColumn) && (nCol == 0))
		nCol = 1;

	// Set the new row and column, and make sure it's visible if specified
	m_nCol = nCol;
	m_nRow = nRow;
	if (bEnsureVisible)
		EnsureCellVisible(nRow, nCol);

	xTRACE("   changed to row %d, col %d\n", nRow, nCol);

	// Save target column for commands and such. 
	m_nTargetColumn = nCol;

	// Make sure something will be drawn!
	if (m_nFocusState == fsNone)
		m_nFocusState = fsInactive;

	// Now draw the new cell
	InvalidateCurrentCell();

	// Tell parent window about item being selected or deselected
	GetParent()->SendMessage(CM_ITEM_SELECTED);
}


// Same as SelectCell but you pass the order of the column to select, not the index
void CListCtrlEx::SelectCellOrder(int nRow, int nColOrder, BOOL bEnsureVisible /* = TRUE */)
{
	SelectCell(nRow, OrderToIndex(nColOrder), bEnsureVisible);
}


// Make sure the specified cell is visible by scrolling the listview if necessary.
// This was causing a bug in the release version only, which was slowing down the program tremendously.
BOOL CListCtrlEx::EnsureCellVisible(int nRow, int nCol)
{
	xTRACE("CListCtrlEx::EnsureCellVisible row %d col %d\n", nRow, nCol);

	// Exit if no row or column specified
	if ((nRow == -1) || (nCol == -1)) return FALSE;

	// Scroll so the row is visible, if necessary
	EnsureVisible(nRow, TRUE);

	// Now scroll so the column is visible, if necessary
	// bug: didn't update this to account for column order
	int nColOrder = IndexToOrder(nCol);
	int nOffset = 0;
	for (int i = 0; i < nColOrder; i++)
	{
		int nColIndex = OrderToIndex(i);
		nOffset += GetColumnWidth(nColIndex);
	}

	// Get right offset
	int nColIndex = OrderToIndex(nColOrder);
	int nOffsetRight = nOffset + GetColumnWidth(nColIndex);

	CRect r;
	GetItemRect(nRow, &r, LVIR_BOUNDS);

	CRect rClient;
	GetClientRect(&rClient);
	if (nOffset + r.left < 0 || nOffsetRight + r.left > rClient.right)
	{
		CSize sz;
		sz.cx = nOffset + r.left;
		sz.cy = 0;
		Scroll(sz);
	}

	return TRUE;
}



// Invalidate the current cell's area, forcing it to be redrawn
void CListCtrlEx::InvalidateCurrentCell()
{
	xTRACE("CListCtrlEx InvalidateCurrentCell (row %d, col %d)\n", m_nRow, m_nCol);
	if ((m_nRow != -1) && (m_nCol != -1))
	{
		CRect r;
		GetCellRect(m_nRow, m_nCol, r); // LVIR_BOUNDS, r);
		InvalidateRect(&r, TRUE);
//		UpdateWindow();
	}
}




// If item or items are deleted, move the selected cell elsewhere
//, check that this doesn't get called for each item when clearing the listview
void CListCtrlEx::OnNotifyDeleteItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// bug:: did = instead of == again
	if (m_nMode & modeSelectCell)
	{
		// Reselect the cell - will reposition if on invalid cell
		SelectCell(m_nRow, m_nCol);
	}
	*pResult = 0;
}


void CListCtrlEx::OnNotifyDeleteAllItems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// bug:: did = instead of == again
	if (m_nMode & modeSelectCell)
	{
		m_nFocusState = fsNone;
		InvalidateCurrentCell();
		m_nRow = -1;
		m_nCol = -1;
	}
	*pResult = 0;
}





/*
// See individual CView classes for double click handling.
// Also see BObject::UIEditValue, pobj->UIEditValue(lngPropertyID);
void CListCtrlEx::OnNotifyDblClk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	AfxMessageBox("clce dblclick");	
	// get selected item
	// check mouse position - if over icon or label, select the item
	// if over a cell, edit value in dialog or do nothing

	*pResult = 0;
}
*/




void CListCtrlEx::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
	
	// Recalculate column widths on listview resize
	RecalculateLayout();
}


// Recalculate the listview by resizing columns if flags are set.
void CListCtrlEx::RecalculateLayout()
{
	CRect r;
	GetClientRect(r);

	// Get last column and expand it to fill client width
	if (m_bExpandLastColumn)
	{		
		int nCols = GetColumnCount();
		if (nCols > 0)
		{
			int nCol = nCols - 1;

			// Get widths of all previous columns
			int nWidths = 0;
			int nWidth = 0;
			for (int i = 0; i < nCols - 1; i++)
			{
				nWidth = GetColumnWidth(i);
				nWidths += nWidth;
			}

			// Fill client width if possible
			nWidth = r.right - nWidths;
			if (nWidth > 0)
				SetColumnWidth(nCol, nWidth);
		}
	}

	//. Keep column widths proportional
	if (m_bExpandAllColumns)
	{
		// GetClientRect(r) gets same as cx and cy
	}
}





// Delete the currently selected item and any children.
BOOL CListCtrlEx::DeleteSelectedItem()
{
	int nItem = GetSelectedItem();

	// Walk through remaining items, deleting them if their indent level is greater than current items.
	int nIndent = GetItemIndent(nItem);
	for (int i = GetItemCount() - 1; i > nItem; i--)
	{
		if (GetItemIndent(i) > nIndent)
		{
			DeleteItem(i);
		}
	}

	return DeleteItem(nItem);
}




// LVN_ITEMCHANGING handler. 
// Notifies user that an item in the listview is changing in some way. 
// Return True to prevent the change, or False to allow it.
// Set target column here also.
// Note: In disabled items mode, we can prevent user from checking items by returning True here.
void CListCtrlEx::OnNotifyItemChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pnmlv = (NM_LISTVIEW*)pNMHDR;

	UINT nChangedBits = pnmlv->uChanged;

	// Allow/disallow checking/unchecking items.
	if (m_bDisabledItems)
	{
		// Check if the state image is changing - that means the user is checking or unchecking the checkbox.
		// If changed bits include any in the state image mask, then it is being changed (check/uncheck image).
		BOOL bCheckmarkChanging = (nChangedBits & LVIS_STATEIMAGEMASK);
		if (bCheckmarkChanging)
		{
			// If item is disabled, don't allow change (return True).
			// Bug: For some reason, uOldState doesn't include the overlaymask bits, so need to get them
			// using GetItemState instead.
			if (GetItemState(pnmlv->iItem, LVIS_OVERLAYMASK) == stateDisabled)
				*pResult = TRUE;
			else
				// Otherwise allow the change
				*pResult = FALSE;
			// bug: forgot to say return here, so was allowing any changes
			return;
		}
	}
	*pResult = 0;
}





// Clear any selected items
void CListCtrlEx::SelectNone()
{
	SetSelectedItemsState(LVIS_SELECTED | LVIS_FOCUSED, 0);
}


// Copy the contents of the listview to the clipboard.
// Copies column headers and accounts for column order also.
BOOL CListCtrlEx::CopyToClipboard(BOOL bIncludeHeaders)
{
	xTRACE("CListCtrlEx::CopyToClipboard\n");

	// Walk through rows and columns, adding text to string separated by tabs
	// Copy the text to the clipboard when done

	CWaitCursor wc;

	int nRows = GetItemCount();
	int nCols = GetColumnCount();

	//, bad to use cstring this way (slow and memory fragmenting)
	CString str;
	CString strTab = "\t";
	CString strLF = "\n";
	CString strLine;

	// Get column header names
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	CString strColumn;
	LPTSTR szBuffer = strColumn.GetBuffer(200);
	lvc.pszText = szBuffer;
	lvc.cchTextMax = 200;
	strLine.Empty();
	for (int nCol = 0; nCol < nCols; nCol++)
	{
		int nIndex = OrderToIndex(nCol);
		GetColumn(nIndex, &lvc);
		strLine += CString(szBuffer);
		strLine += (nCol == nCols - 1) ? strLF : strTab;
	}
	strColumn.ReleaseBuffer();
	str = strLine;

	// Get cell contents
	for (int nRow = 0; nRow < nRows; nRow++)
	{
		strLine.Empty();
		for (int nCol = 0; nCol < nCols; nCol++)
		{
			int nIndex = OrderToIndex(nCol);
			strLine += GetItemText(nRow, nIndex);
			strLine += (nCol == nCols - 1) ? strLF : strTab;
		}
		str += strLine;
	}

	// Copy text to clipboard
	theApp.CopyToClipboard(str);

	return TRUE;
}


// Get the number of columns in the listview
int CListCtrlEx::GetColumnCount()
{
	return GetHeaderCtrl()->GetItemCount();
}


// Returns the index of the column at the specified column position
int CListCtrlEx::OrderToIndex(int nColPosition)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	return pHeader->OrderToIndex(nColPosition);
}



// Convert the column index to the order it is shown in the listview
int CListCtrlEx::IndexToOrder(int nColIndex)
{
	LVCOLUMN lvc;
	lvc.mask = LVCF_ORDER;
	GetColumn(nColIndex, &lvc);
	return lvc.iOrder;
}



// Move the source item either before or after the target item.
int CListCtrlEx::MoveItemToSibling(int nSource, int nTarget, BOOL bAfter)
{
	xTRACE("CListCtrlEx::MoveItemToSibling\n");

	// Copy the source item to LVITEM structure
	LVITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_INDENT | LVIF_PARAM | LVIF_STATE;
	lvi.iItem = nSource;
	lvi.iSubItem = 0;
//	lvi.pszText = LPSTR_TEXTCALLBACK;
	GetItem(&lvi);

	// Get position to insert new item at
	int nNewPosition = bAfter ? nTarget + 1 : nTarget;
	lvi.iItem = nNewPosition;
	lvi.mask = lvi.mask | LVIF_TEXT;
	lvi.pszText = LPSTR_TEXTCALLBACK;

	// Insert the copy
	int nIndex = InsertItem(&lvi);
	ASSERT(nIndex != -1);

	// Delete the original source item, getting its changed position
	if (nTarget < nSource)
		nSource++;
	VERIFY(DeleteItem(nSource));

	// Now refind the source item (that is, the new copy of it)
	nNewPosition = FindItemData(lvi.lParam);
	ASSERT(nNewPosition != -1);

	return nNewPosition;
}





// Delete the item with the specified item data, if found in list.
// Returns True if delete item successfully.
BOOL CListCtrlEx::DeleteItemData(LPARAM lParam)
{
	int nItem = FindItemData(lParam);
	if (nItem != -1)
		return DeleteItem(nItem);
	return FALSE;
}



// Get object id of selected bobject, or 0 if none
ULONG CListCtrlEx::GetSelectedObjectID()
{
	BObject* pobjSelected = (BObject*) GetSelectedItemData();
	if (pobjSelected)
	{
		ASSERT_VALID(pobjSelected);
		return pobjSelected->GetObjectID();
	}
	return 0;
}




// Fills the array with item data of selected items.
// Returns the number of selected items.
// This handles select cell mode.
int CListCtrlEx::GetSelectedItemsArray(CObArray& aObjects)
{
	ASSERT_VALID(&aObjects);
	aObjects.RemoveAll();

	// If in cell select mode, make sure we get the current cell
	//. also need to handle if more than one item selected, as in drag drop
	if (m_nMode == modeSelectCell)
	{
		if ((m_nCol != -1) && (m_nRow != -1))
		{
			DWORD dwData = GetItemData(m_nRow);
			// Make sure we don't include the dummy new row!
			if (dwData != keyDummyRow)
				aObjects.Add((CObject*) dwData);
		}
		return aObjects.GetSize();
	}

	// If in normal mode, use listview's normally selected items
	int nItems = GetSelectedCount();
	int i = 0;
	if (nItems)
	{
		aObjects.SetSize(nItems);
		POSITION pos = GetFirstSelectedItemPosition();
		while (pos)
		{
			int nItem = GetNextSelectedItem(pos);
			DWORD dwData = GetItemData(nItem);
			// Make sure we don't include the dummy new row!
			if (dwData != keyDummyRow)
			{
				aObjects.SetAt(i, (CObject*) dwData);
				i++;
			}
		}
	}
	// Resize array in case we removed the dummy new row
	if (i != nItems)
	{
		nItems = i;
		aObjects.SetSize(nItems);
	}
	ASSERT_VALID(&aObjects);
	return nItems;
}



// Get selected items into array.
// Note: won't get disabled items.
// This proceeds in listview order, so the order the items are in is the order the bdatalink 
// will link to the objects. 
// Returns number of items in bdatalink object.
// Merge GetSelectedItemsArray into this - needs to handle cell select mode!
int CListCtrlEx::GetSelectedItems(BDataLink *pdatLink)
{
	xTRACE("CListCtrlEx::GetSelectedItems\n");

	ASSERT_VALID(pdatLink);
	if (m_nMode == modeCheckboxes)
	{
		// Walk through listview items, adding checked ones to bdatalink.
		pdatLink->SetMultiple();
		pdatLink->RemoveAll(); // start with clear array
		int nItems = GetItemCount();
		for (int i = 0; i < nItems; i++)
		{
			if (GetCheck(i))
			{
				// Don't include disabled items
				if (GetItemState(i, LVIS_OVERLAYMASK) != stateDisabled)					
				{
					BObject* pobj = (BObject*) GetItemData(i);
					if (pobj)
					{
						ASSERT_VALID(pobj);
						pdatLink->AddLink(pobj);
					}
				}
			}
		}
	}
	else
	{
		// Get single selected item
		BObject* pobj = (BObject*) GetSelectedItemData();
		// If nothing is selected pobj will be zero, which is alright.
		if (pobj)
		{
			ASSERT_VALID(pobj);
		}
		pdatLink->SetLink(pobj);
	}	
	return pdatLink->GetLinkCount();
}




// Set selected items
// Note: If pdatLink is 0, will just select first item in list
BOOL CListCtrlEx::SetSelectedItems(BDataLink *pdatLink)
{
	if (pdatLink)
	{
		ASSERT_VALID(pdatLink);
		if (m_nMode == modeCheckboxes)
		{
			ASSERT(pdatLink->IsMultiple());
			int nItems = pdatLink->GetLinkCount();
			for (int i = 0; i < nItems; i++)
			{
				BObject* pobj = pdatLink->GetLinkAt(i);
				int nItem = FindItemData((LPARAM) pobj);
				if (nItem != -1)
					SetCheck(nItem, TRUE);
			}
		}
		else // single select?
		{
			ASSERT(pdatLink->IsSingle());
			BObject* pobj = pdatLink->GetLink(); // might be 0
			if (pobj)
				SelectItemData((DWORD) pobj);
			else
				SelectItem(0); // select first item
		}
	}
	// If pdatlinks is 0, just select first item
	else if (GetItemCount() > 0)
		SelectItem(0);
	return TRUE;
}




// Turn checkboxes mode on or off.
// Also checks off selected item or removes checkboxes as appropriate.
void CListCtrlEx::SetCheckboxes(BOOL bCheckboxes)
{
	if (bCheckboxes)
	{
		// Set listview mode
		m_nMode = modeCheckboxes;
		SetExtendedStyle(GetExtendedStyle() | LVS_EX_CHECKBOXES);
		// Turn off single selection mode
		LONG lngStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		::SetWindowLong(m_hWnd, GWL_STYLE, lngStyle & ~LVS_SINGLESEL);
		// Check off the selected item, if any
		int nItem = GetSelectedItem();
		if (nItem != -1)
			SetCheck(nItem, TRUE);
	}
	else
	{
		// Clear all checkboxes
		int nItems = GetItemCount();
		for (int i = 0; i < nItems; i++)
			SetCheck(i, FALSE);
		// Set listview mode
		m_nMode = modeNormalListView;
		SetExtendedStyle(GetExtendedStyle() & ~LVS_EX_CHECKBOXES);
		// Turn on single selection mode
		LONG lngStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		::SetWindowLong(m_hWnd, GWL_STYLE, lngStyle | LVS_SINGLESEL);
		// Make sure we have at least one item selected
		if ((nItems > 0) && (GetSelectedItem() == -1))
			SelectItem(0);
	}
}


// Turn full row select on or off
void CListCtrlEx::SetFullRowSelect(BOOL bOn)
{
	if (bOn)
	{
		m_nMode = modeFullRowSelect;
		SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	}
	else
	{
		m_nMode = modeNormalListView;
		SetExtendedStyle(GetExtendedStyle() & ~LVS_EX_FULLROWSELECT);
	}
}






// Add the child objects of the specified bobject to the listview, recursively if specified.
void CListCtrlEx::AddObjects(BObject* pobjStart, ULONG lngExcludeFlags, 
								BOOL bIncludeStart /* = FALSE */, BOOL bRecurse /* = FALSE */, 
								int nIndent /* = 0 */, BOOL bClearFirst /* = FALSE */, BOOL bHierarchy /* = FALSE */)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pDoc);
	ASSERT_VALID(pobjStart);

	// Clear list if specified
	if (bClearFirst)
		DeleteAllItems();

	// Set up structure here for speed
	LVITEM lvi;
	if (bHierarchy)
		lvi.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT | LVIF_INDENT;
	else
		lvi.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT; // ignore indent
	lvi.iSubItem = 0;
	lvi.pszText = LPSTR_TEXTCALLBACK;
	lvi.iIndent = nIndent;
	lvi.stateMask = 0;
	lvi.state = 0;
	lvi.cchTextMax = 0;

	// Add the start object if specified
	if (bIncludeStart)
	{
		if (!(pobjStart->GetFlag(lngExcludeFlags)))
		{
			// Get index of icon associated with this object
			int nImage = pobjStart->GetIconIndex();
		
			// Get the new item number
			int nItem = GetItemCount(); // always add to end of the list because of recursion

			// Add this BObject to the list and store pointer to object in item data
			lvi.iItem = nItem;
			lvi.iImage = nImage; 
			lvi.lParam = (LPARAM) pobjStart;
			int nIndex = InsertItem(&lvi);
			ASSERT(nIndex != -1);

			// Check for disabled item
			if (pobjStart->GetFlag(flagDisabled))
				SetItemState(nItem, CListCtrlEx::stateDisabled, LVIS_OVERLAYMASK);

			// Increase the indent level for the children
			nIndent++;
		}
	}

	// Walk through child objects (if any) and add them to the list also
	BObjects* paChildren = pobjStart->GetChildren();
	if (paChildren)
	{
		ASSERT_VALID(paChildren);

		// If we're adding items in a hierarchy, we'll need to sort the children before we add them.
		// Sort the children alphabetically (otherwise they show up in combo unsorted).
		// We must copy the children array to do this.
		// Note: We only need to sort them if autosort is on for the parent bobject.
		if (bHierarchy)
		{
			paChildren = paChildren->CreateCopy();
			ASSERT_VALID(paChildren);
			if (pobjStart->IsSorted())
				paChildren->Sort(propName);
		}

		ASSERT_VALID(paChildren);
		int nItems = paChildren->GetSize();
		for (int i = 0; i < nItems; i++)
		{
			BObject* pobj = DYNAMIC_DOWNCAST(BObject, paChildren->GetAt(i));
			ASSERT_VALID(pobj);

			// bug: used && instead of &
			if (!(pobj->GetFlag(lngExcludeFlags)))
			{
				// Always add to the end of the list because of recursion
				int nItem = ListView_GetItemCount(m_hWnd);

				// Add this BObject to the list 
				// bug: If dialog's listview is set to be sorted, insert item will fail and return -1
				// if you try to pass LPSTR_TEXTCALLBACK. Fortunately the help said not to do
				// this, but it took a while to find it.
				lvi.iItem = nItem;
				lvi.iImage = pobj->GetIconIndex(); 
				lvi.lParam = (LPARAM) pobj;
				lvi.iIndent = nIndent;
				// For the most speed, send the message directly by using the insert item macro
				int nIndex = ListView_InsertItem(m_hWnd, &lvi);
				ASSERT(nIndex != -1);

				// Check for disabled item
				if (pobj->GetFlag(flagDisabled))
					SetItemState(nItem, CListCtrlEx::stateDisabled, LVIS_OVERLAYMASK);

				if (bRecurse && pobj->HasChildren())
					AddObjects(pobj, lngExcludeFlags, FALSE, bRecurse, nIndent + 1, FALSE, bHierarchy);
			}
		}
		
		// Now delete the copy of the children array
		if (bHierarchy)
			delete paChildren;

	}
}







// Add an object to the list.
// Returns index of new object, or -1 if failed.
// Note: Don't handle removing and adding dummy row here, because in some cases
// need to add several objects in a row.
//, handle parent, recursion, etc
int CListCtrlEx::AddObject(BObject *pobj, BObject *pobjParent /* = NULL */)
{
	int nIndex = -1;
	int nItems = GetItemCount();
	int nImage = pobj->GetIconIndex();

	if (pobjParent)
	{
		// Add object indented properly under parent object
		int nParent = FindItemData((LPARAM) pobjParent);
		if (nParent != -1)
		{
			int nIndent = GetItemIndent(nParent);
			
			LVITEM lvi;
			lvi.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT | LVIF_INDENT;
			lvi.iItem = nParent + 1; // add under parent
			lvi.iSubItem = 0;
			lvi.pszText = LPSTR_TEXTCALLBACK;
			lvi.iIndent = nIndent + 1; // add under parent item
			lvi.stateMask = 0;
			lvi.state = 0;
			lvi.cchTextMax = 0;
			lvi.iImage = nImage;
			lvi.lParam = (LPARAM) pobj;
			nIndex = InsertItem(&lvi);
			ASSERT(nIndex != -1);
		}
	}
	else
	{
		// Just add object to top level
		nIndex = InsertItem(nItems, LPSTR_TEXTCALLBACK, nImage);
		ASSERT(nIndex != -1);
		SetItemData(nIndex, (DWORD) pobj);
	}

	// Check for disabled item
	if (pobj->GetFlag(flagDisabled))
		SetItemState(nIndex, CListCtrlEx::stateDisabled, LVIS_OVERLAYMASK);

	return nIndex;
}



// Add dummy new row, if flag is set.
// The dummy row has a special icon associated with it and its item data is keyDummyRow.
void CListCtrlEx::AddDummyRow()
{
	if (m_bDummyRow)
	{
		int nItems = GetItemCount();
		int nIndex = InsertItem(nItems, "", m_nNewIconIndex);
		ASSERT (nIndex != -1);
		SetItemData(nIndex, keyDummyRow);
	}
}


// Remove dummy row, if flag is set and row exists
void CListCtrlEx::RemoveDummyRow()
{
	if (m_bDummyRow)
	{
		// Verify that there is at least one item and that the last row is indeed a dummy row
		int nItem = GetItemCount() - 1;
		if (nItem >=0)
		{
			DWORD dwData = GetItemData(nItem);
			if (dwData == keyDummyRow)
				DeleteItem(nItem);
		}
	}
}



// Get the PropertyID associated with the specified column, or 0 if not found.
// If iCol is -1 (default) then will use current cell column.
// Note: We store the PropertyID in each column's lParam data.
ULONG CListCtrlEx::GetColumnPropertyID(int iCol /* = -1 */) 
{
	if (iCol == -1) 
		iCol = m_nCol; // use current cell column if none specified
	// If no cell selected return 0
	if (iCol == -1)
		return 0;
	HDITEM hdi;
	hdi.mask = HDI_LPARAM;
	if (GetHeaderCtrl()->GetItem(iCol, &hdi))
		return hdi.lParam; // PropertyID
	return 0;
}





/*
// DON'T DELETE THIS - good for debugging
BOOL CListCtrlEx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* pnm = (NMHDR*) lParam;
	UINT nIDFrom = pnm->idFrom;
	UINT nCode = pnm->code;
	CString str = theApp.GetNotifyCodeString(nCode);
	xTRACE("CListCtrlEx OnNotify id %d, code %d %s\n", nIDFrom, nCode, (LPCTSTR) str);
	return CListCtrl::OnNotify(wParam, lParam, pResult);
}
*/





// HDN_ENDDRAG handler. Use this to handle reordering columns.
BOOL CListCtrlEx::OnHeaderEndDrag(UINT id, NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMHEADER* phdr = (NMHEADER*) pNMHDR;
	HDITEM* pitem = phdr->pitem;

	if (pitem->mask == HDI_ORDER)
	{
		// Save column order here? why not?
		// i don't think we need to because the control handles the order transparently - 
		// eg it always returns the index, not the order
//		int iNewOrder = pitem->iOrder;

		// Set flag
		m_bColumnsChanged = TRUE;

		// Notify parent window
		//. note: since this hasn't returned yet, the order hasn't been set for the listview yet.
		// therefore, this will save the existing order, not the new order!
		// so let's skip this for now - not as important as keeping column id's in synch.
//		NotifyParentOfChanges();
	}
	
	// Return FALSE to let control automatically place item, TRUE to prevent it.
	*pResult = FALSE;

	// Your function must return TRUE if the notification message has been completely handled or 
	// FALSE if other objects in the command routing should have a chance to handle the message.
	// Note: If you return TRUE here, the listview won't get the message and so it
	// won't know to reposition its columns also.
	return FALSE;
}





// Notify parent of changes to column arrangement so it can save them, if necessary.
void CListCtrlEx::NotifyParentOfChanges()
{
	GetParent()->SendMessage(CM_COLUMNS_CHANGED);
}



// Delete the column associated with the specified property def, if there.
// See also RemoveColumn.
void CListCtrlEx::DeleteColumnProperty(BObject *pobjPropertyDef)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjPropertyDef); 
	CHeaderCtrl* phdr = GetHeaderCtrl();
	ASSERT_VALID(phdr);

	BOOL bChanged = FALSE;

	// Bug: Since the property def has been deleted, m_lngObjectID is no longer valid!!!!!!!
	// Also, at this point, the bobject has been removed from document index, so can't do GetObject on it!!!!
	ULONG lngPropertyID = pobjPropertyDef->GetObjectID();

	// Now search through columns and remove if there
	HDITEM hdi;
	hdi.mask = HDI_LPARAM;
	int nCols = phdr->GetItemCount();
	for (int i=0; i < nCols; i++)
	{
		phdr->GetItem(i, &hdi);
		if (hdi.lParam == (long) lngPropertyID)
		{
			// Delete column
			VERIFY(DeleteColumn(i));
			// Update indexes so can walk through other columns
			i--;
			nCols--;
			// set flag
			bChanged = TRUE;
		}
	}

	// Set flag and alert parent if any changes made so will save them to current object
	if (bChanged)
	{
		// refresh listview to redraw header?
//		RedrawWindow();

		// Set flag
//		m_pdatColumns->m_bModified = TRUE;
		m_bColumnsChanged = TRUE;

		// Notify parent window
		NotifyParentOfChanges();
	}
}



// Copy the contents of the current cell to the clipboard
BOOL CListCtrlEx::CopyCurrentCellToClipboard()
{
	if (m_nRow != -1 && m_nCol != -1)
	{
		CString str = GetItemTextOP(m_nRow, m_nCol);
		theApp.CopyToClipboard(str);
		return TRUE;
	}
	return FALSE;
}


BOOL CListCtrlEx::CutCurrentCellToClipboard()
{
	if (CopyCurrentCellToClipboard())
	{
		// Send LVN_ENDLABELEDIT notification to parent of ListView ctrl
		//, make a method for notifications
		NMLVDISPINFO nmdi;
		nmdi.hdr.hwndFrom = m_hWnd;
		nmdi.hdr.idFrom = -1; // GetDlgCtrlID(); // this should be edit control id
		nmdi.hdr.code = LVN_ENDLABELEDIT;

		nmdi.item.mask = LVIF_TEXT;
		nmdi.item.iItem = m_nRow;
		nmdi.item.iSubItem = m_nCol;
		nmdi.item.pszText = "";
		nmdi.item.cchTextMax = 1;

		GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM) &nmdi); 

		return TRUE;
	}
	return FALSE;
}


BOOL CListCtrlEx::PasteClipboardToCurrentCell()
{
	// If on a valid cell, get clipboard text and start editing contents
	if (m_nCol != -1 && m_nRow != -1)
	{
		CString str;
		if (theApp.GetTextFromClipboard(str))
		{
			EditCurrentCell(str);
			return TRUE;
		}
	}
	return FALSE;
}



// Handle update of cut copy paste commands.
void CListCtrlEx::UpdateEdit(CCmdUI *pCmdUI)
{
	BOOL bValidCell = (m_nRow != -1 && m_nCol != -1);

	switch (pCmdUI->m_nID)
	{
	case ID_EDIT_COPY:
	case ID_EDIT_CUT:
		{
			// Enable cut and copy if there is one or more items selected in the listview
//			POSITION pos = m_lvw.GetFirstSelectedItemPosition();  // returns NULL if no items selected
//			pCmdUI->Enable((int)pos);
			pCmdUI->Enable(bValidCell);
			break;
		}
	case ID_EDIT_PASTE_PLAIN:
	case ID_EDIT_PASTE_FORMATTED:
		{
			// Enable paste if there is NeoMem clipboard format data on the clipboard
			// data object represents the destination side
			COleDataObject codo;
			codo.AttachClipboard();
//			pCmdUI->Enable(codo.IsDataAvailable(theApp.m_cfObjects));
			pCmdUI->Enable(bValidCell && codo.IsDataAvailable(CF_TEXT));
			break;
		}
	}
}


// Added this to fix what seems to be a bug in MFC's GetItemText, which does not retrieve the
// entire contents of the cell. Seems to stop at 256 chars each time. 
// Note: OP stood for OutlinePad - can change it
// nItem is grid item number, so before calling need to convert column to item number. 
CString CListCtrlEx::GetItemTextOP(int nItem, int nSubItem)
{
	ASSERT(::IsWindow(m_hWnd));
	
	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.iSubItem = nSubItem;
	
	CString str;
	
	int nLen = 128;
	int nChars = 0;
	do
	{
		nLen *= 2;
		lvi.cchTextMax = nLen;
		lvi.pszText = str.GetBufferSetLength(nLen);
		nChars  = (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
		str.ReleaseBuffer(); // bug: didn't have this for some reason
		// Bug in MFC or control here - nChars returns 0 even though didn't retrieve all characters.
		// So now we check if nChars is 0 AND the string is not blank, which means we should continue getting chars.
		// Note: IsEmpty and GetLength don't work because we're using GetBuffer.
	// If filled buffer then go back for another round
	} while ((nChars == nLen) || (nChars == 0 && (_tcslen(lvi.pszText) != 0)));

	return str;	
}



// Mouse handling
//-------------------------------------------------------------------------------------------------------------------------

void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint ptClient) 
{
	xTRACE("CListCtrlEx::OnLButtonDown\n");

	// If in select cell mode, swallow the left button event - don't let control get it (otherwise it will
	// select the item also its own way, which we don't want).
	if (m_nMode & modeSelectCell)
	{
		xTRACE("  olbd: swallow event since we're in select cell mode\n");
		// If user has already selected multiple cells, then this will just set the focus cell.

		// Find column and row clicked on
		int nCol;
		int nRow = HitTestEx(ptClient, &nCol);

		// Select the cell
		SelectCell(nRow, nCol);

		// Set flag and save start point for drag operation start (if on first column)
		if (nCol == 0)
		{
			m_bMouseDown = TRUE;
			m_ptDragStart = ptClient; // save start point also
		}

		// Let the BData object do any additional actions for the click (eg open website).
		// Note: pdat might be zero, if no value for the property.
		BData* pdat = GetCellBData(nRow, nCol);
		if (pdat)
		{
			ASSERT_VALID(pdat);
			pdat->UIOnClick();
		}

	}
	else
	{
		xTRACE("  olbd: let control have event\n");
		// Let control have it
		CListCtrl::OnLButtonDown(nFlags, ptClient);
	}

}


void CListCtrlEx::OnMouseMove(UINT nFlags, CPoint ptClient) 
{
	// If mouse button is down, then assume user is starting a drag operation
	if ((m_bMouseDown) && (nFlags & MK_LBUTTON))
	{
		//, see if moved far enough
//		ptClient - m_ptDragStart
//		this->DragAcceptFiles
		xTRACE("start drag\n");
		
		// Send notification to parent of ListView ctrl
		NMLVDISPINFO nmdi;
		nmdi.hdr.hwndFrom = m_hWnd;
		nmdi.hdr.idFrom = GetDlgCtrlID(); // this should be edit control id
		nmdi.hdr.code = LVN_BEGINDRAG;
//		nmdi.item.mask = LVIF_TEXT;
//		nmdi.item.iItem = m_nRow;
//		nmdi.item.iSubItem = m_nCol;
//		nmdi.item.pszText = "";
//		nmdi.item.cchTextMax = 1;
		GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM) &nmdi);

		m_bMouseDown = FALSE; //, bad name for this flag
	}

	// See if mouse is over a hyperlink.
	// We let each bdata handle this how they want.
	// For hyperlink, changes cursor to a hand
	//, if too slow, cache propertytypeid and do switch statement here
	int nCol;
	int nRow = HitTestEx(ptClient, &nCol);
	if ((nCol != -1) && (nRow != -1))
	{
		BData* pdat = GetCellBData(nRow, nCol);
		if (pdat)
		{
			ASSERT_VALID(pdat);
			pdat->UIOnMouseMove();
		}
	}

	// Call base class
	CListCtrl::OnMouseMove(nFlags, ptClient);
}



void CListCtrlEx::OnLButtonUp(UINT nFlags, CPoint point) 
{	
	xTRACE("CListCtrlEx::OnLButtonUp - call base class\n");

	// Clear drag start flag
	m_bMouseDown = FALSE;

	// Call base class
	CListCtrl::OnLButtonUp(nFlags, point);
}



void CListCtrlEx::OnRButtonDown(UINT nFlags, CPoint ptClient) 
{
	if (m_nMode & modeSelectCell)
	{
		// Find column and row clicked on
		int nCol;
		int nRow = HitTestEx(ptClient, &nCol);

		// Select the cell
		SelectCell(nRow, nCol);
	}
	else
		CListCtrl::OnRButtonDown(nFlags, ptClient);
}




// Get the indent level of the specified item.
int CListCtrlEx::GetItemIndent(int nItem)
{
	// bug: didn't do memset and GetItem returned garbage
	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.mask = LVIF_INDENT;
	lvi.iItem = nItem;
	GetItem(&lvi);
	return lvi.iIndent;
}


// The framework calls this member function when the cursor is in an inactive window and 
// the user presses a mouse button. 
// The default implementation passes this message to the parent window before any processing 
// occurs. If the parent window returns TRUE, processing is halted.
// We handle this so that if user is renaming an item and clicks in the edit ctrl the focus does not
// pass to the listctrl!
int CListCtrlEx::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	xTRACE("CListCtrlEx::OnMouseActivate\n");	
	
	// If editing a cell, return noactivate, otherwise call the base class as usual.
	// Bug: This bombed when switching focus from another app - pwnd was 0.
	CWnd* pwnd = GetFocus();
	BOOL bFocusInEdit = ((pwnd != 0) && (pwnd->GetDlgCtrlID() == IDC_TXT_EDIT));

	// If user clicks somewhere else in the listview, should call base class. 
	// Need to determine if click occurred in the edit window.
	CPoint p(::GetMessagePos()); // get position of message source in screen coords
	CWnd* pwndClick = WindowFromPoint(p);
	ASSERT_VALID(pwndClick);
	BOOL bClickedInEdit = (pwndClick->GetDlgCtrlID() == IDC_TXT_EDIT);

	// If focus is not in edit control, OR user did NOT click in edit control, call the base class.
	// This will terminate the edit control if it has focus.
	if (!bFocusInEdit || !bClickedInEdit)
		return CListCtrl::OnMouseActivate(pDesktopWnd, nHitTest, message);
	else
		return MA_NOACTIVATE;
}


// Simulate a keypress
// Note: Sending keydown and keyup messages doesn't work if this control doesn't have focus
// (at least I think that's why it doesn't work)
BOOL CListCtrlEx::PressKey(UINT nVK)
{
	int nItem = GetSelectedItem();
	int nItems = GetItemCount();
	
	if (nItems == 0)
		return FALSE;

	switch (nVK)
	{
		case VK_UP:
			if (nItem == -1)
				SelectItem(0);
			else if (nItem > 0)
				SelectItem(nItem - 1);
			break;
		case VK_DOWN:
			if (nItem == -1)
				SelectItem(nItems - 1);
			else if (nItem < nItems - 1)
				SelectItem(nItem + 1);
			break;
		case VK_HOME:
			SelectItem(0);
			break;
		case VK_END:
			SelectItem(nItems - 1);
			break;
		default:
			break;
	}
	return TRUE;
}



// Get the bdata object associated with the specified row and column.
// Don't pass any parameters to get current cell's bdata.
// May return NULL if no bdata object. 
// None of the callers seem to modify the bdata object.
BData* CListCtrlEx::GetCellBData(int nRow /* = -1 */, int nCol /* = -1 */)
{
	BData* pdat = 0;

	// Get current cell if none specified (might still wind up being -1's though if no cell selected)
	// Don't do this because if you do, you could be moving the mouse around on empty
	// cells and it would show a hand, if the currently selected cell is a hyperlink property!
	// Bug: by fixing that bug, caused another one (was not retrieving bdata on right click of cell
	// because that code passed nothing to this routine, expecting the defaults to do this...)
	// Fixed by changing the mouse move code to check for -1's. 
	if (nRow == -1)
		nRow = m_nRow;
	if (nCol == -1)
		nCol = m_nCol;

	if ((nRow != -1) && (nCol != -1))
	{
		// If we're in Contents view, pobj will be the bobject we're interested in.
		// If we're in Properties view, pobj will be the PropertyDef bobject we want to look up.
		BObject* pobj = (BObject*) GetItemData(nRow);
		if (pobj)
		{
			ASSERT_VALID(pobj);
			// Get property associated with the specified column
			ULONG lngPropertyID = GetColumnPropertyID(nCol);
			// Get object string
			// Special case for Property View - need to display the value of the property for the
			// document's current object. The Value column has a dummy property of propValue assigned to it.
			if (lngPropertyID == propValue)
				pdat = m_pDoc->GetCurrentObject()->GetPropertyData(pobj->GetObjectID());
			else
				pdat = pobj->GetPropertyData(lngPropertyID);
		}
	}

	return pdat;
}



// Get width of number of columns specified, or all columns if nCols=0.
int CListCtrlEx::GetColumnWidths(int nCols /* = 0 */)
{
	int nWidths = 0;
	if (nCols == 0)
		nCols = GetColumnCount();
	if (nCols > 0)
	{
		int nWidth = 0;
		for (int i = 0; i < nCols; i++)
		{
			nWidth = GetColumnWidth(i);
			nWidths += nWidth;
		}
	}
	return nWidths;
}



// Get text associated with specified column (property name)
CString CListCtrlEx::GetColumnText(int nColOrder)
{
	CString str;
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	LPTSTR pszBuffer = str.GetBuffer(NM_MAX_TEXT);
	lvc.pszText = pszBuffer;
	lvc.cchTextMax = NM_MAX_TEXT;
	int nIndex = OrderToIndex(nColOrder);
	GetColumn(nIndex, &lvc);
	str.ReleaseBuffer();
	return str;
}




// Print routines
//-----------------------------------------------------------------------------------------------------------------


// Initialize printing
// pDC refers to printer device context.
//. handle word wrapping in cells (eg for long descriptions)
void CListCtrlEx::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CPrintInfoMore& rpim = theApp.m_printinfo;

	// Get grid info
	int nCols = GetColumnCount();
	int nColWidthsTotal = GetColumnWidths();
	m_nPrintLinesTotal = GetItemCount(); // will need to adjust this for word wrapping
	if (m_bDummyRow) m_nPrintLinesTotal--;
	int nRows = m_nPrintLinesTotal; // nRows is the number of grid rows - each row may have more than one line

	// Get screen font information (for this control)
	TEXTMETRIC tm;
	CDC* pScreenDC = GetDC();
	CFont* poldfont = pScreenDC->SelectObject(&theApp.m_fontControls);
	pScreenDC->GetTextMetrics(&tm);
	pScreenDC->SelectObject(poldfont);
	int nScreenCharWidth = tm.tmAveCharWidth; // avg character width
	int nScreenChars = nColWidthsTotal / nScreenCharWidth; // number of chars that should fit on screen
	int nScreenPixelsPerInchX = pScreenDC->GetDeviceCaps(LOGPIXELSX); 

	// Get printer font, adjusting size if necessary to fit all chars
	rpim.InitFonts(pDC);
	int nPrintChars = rpim.m_nPageWidthWithinMargins / rpim.m_nPrintCharWidth;
	float fScale = (float) rpim.m_nPageWidthWithinMargins / (float) nColWidthsTotal; // factor to convert from screen pixels to page pixels
	if (nScreenChars > nPrintChars)
	{
		float fFactor = (float) nPrintChars / (float) nScreenChars;
		rpim.InitFonts(pDC, fFactor);
		nPrintChars = rpim.m_nPageWidthWithinMargins / rpim.m_nPrintCharWidth;
	}

	// Horizontal space between cells 
	UINT nCellSpacing = rpim.m_nPixelsPerInchX / 20; // pixels (1/20th of an inch)

	// Get arrays of column positions and widths
	m_anColPos.SetSize(nCols);
	m_anColWidth.SetSize(nCols);
	m_anRowLines.SetSize(nRows);
	UINT nPos = 0;
	for (int i = 0; i < nCols; i++)
	{
		int nCol = OrderToIndex(i);
		UINT nWidth = GetColumnWidth(nCol);
		UINT nPosScaled = (int) (nPos * fScale);
		UINT nWidthScaled = (int) (nWidth * fScale);
		nWidthScaled -= nCellSpacing;
		m_anColPos.SetAt(i, nPosScaled);
		m_anColWidth.SetAt(i, nWidthScaled);
		// Don't let cells go beyond the right margin
		if (nPosScaled + nWidthScaled > rpim.m_nPageWidthWithinMargins)
		{
			nWidthScaled = rpim.m_nPageWidthWithinMargins - nPosScaled;
			nWidthScaled -= nCellSpacing;
			m_anColWidth.SetAt(i, nWidthScaled);
		}
		nPos+=nWidth;
	}

/*
	//. Now walk through rows and handle word wrapping
	// call DrawText to determine text height
	//. what about tabs within the text? allowed?
	// DT_CALCRECT Determines the width and height of the rectangle. If there are multiple lines 
	// of text, DrawText uses the width of the rectangle pointed to by the lprc parameter and 
	// extends the base of the rectangle to bound the last line of text. DrawText returns the 
	// height of the formatted text, but does not draw the text. 
	// DT_EXPANDTABS Expands tab characters. The default number of characters per tab is eight. 
	// DT_NOPREFIX Turns off processing of prefix characters.  
	// DT_WORDBREAK Breaks words. Lines are automatically broken between words if a word 
	// extends past the edge of the rectangle specified by the lprc parameter. A carriage return-line feed 
	// sequence also breaks the line. 
	UINT uFormat = DT_CALCRECT + DT_NOPREFIX + DT_WORDBREAK;
	LONG nSingleLineHeight = rpim.m_nPrintLineHeight;	 //, giving 24 or 20 - should be 34?
	LONG nSingleLineHeight = 34; // what it should be
	ASSERT(nSingleLineHeight > 0);
	for (int j = 0; j < nRows; j++)
	{
		LONG nMaxHeight = 0;
		for (i = 0; i < nCols; i++)
		{
			CRect r;
			int nCol = OrderToIndex(i);
			CString str = GetItemTextOP(j, nCol);
			UINT nCellWidth = m_anColWidth.GetAt(nCol);
			r.SetRect(0, 0, nCellWidth, 0);
			LONG nLineHeight = pDC->DrawText(str, &r, uFormat);
//			LONG nLineHeight = r.bottom;
			if (nLineHeight > nMaxHeight)
				nMaxHeight = nLineHeight;
		}
		// See how many lines this row will take up and add it to the total
		int nLines = nMaxHeight / nSingleLineHeight;
		m_anRowLines.SetAt(j, nLines);
		m_nPrintLinesTotal += (nLines - 1);
	}
*/

	// Calculate number of pages
	int nAvailHeight = rpim.m_nPageHeightWithinMargins - 
					(rpim.m_nHeaderLineHeight * (rpim.m_nHeaderLines + rpim.m_nFooterLines));
	m_nPrintLinesPerPage = nAvailHeight / rpim.m_nPrintLineHeight;
	m_nPrintLinesPerPage--; // subtract one line to leave room for column headers on each page
	UINT nPages = max (1, (m_nPrintLinesTotal + (m_nPrintLinesPerPage - 1)) / m_nPrintLinesPerPage);
	pInfo->SetMaxPage(nPages);

}


// Print a page of information
void CListCtrlEx::PrintPage(CDC *pDC, CPrintInfo *pInfo)
{
	if (m_nPrintLinesTotal != 0)
	{
		CPrintInfoMore& rpim = theApp.m_printinfo;
		
		pDC->SetBkMode(TRANSPARENT); // otherwise text will erase parts of line
		
		int nLeft = pInfo->m_rectDraw.left;
		int nCurrentPage = pInfo->m_nCurPage;
		int nCols = GetColumnCount();
		//. handle word wrap
		int nRowStart = (nCurrentPage - 1) * m_nPrintLinesPerPage;
		int nRowEnd = min (m_nPrintLinesTotal - 1, nRowStart + m_nPrintLinesPerPage - 1);
		int nRows = nRowEnd - nRowStart + 1;
		int nLineHeight = rpim.m_nPrintLineHeight;
		CString str;
		CFont* poldfont = 0;

		for (int j = 0; j <= nRows; j++) // 0 is column header row, 1..nRows are cell rows
		{
			int nRow = nRowStart + j - 1;
			BOOL bHeaderRow = (j == 0);

			// Select font
			if (bHeaderRow) // use bold for header row
				poldfont = pDC->SelectObject(&rpim.m_fontPrintBold);
			else
				poldfont = pDC->SelectObject(&rpim.m_fontPrint);

			// Walk through columns and print cells
			int y = rpim.m_nOffsetToTop + (rpim.m_nHeaderLines * rpim.m_nHeaderLineHeight) + (j * nLineHeight);
			for (int nCol = 0; nCol < nCols; nCol++)
			{
				if (bHeaderRow)
					str = GetColumnText(nCol); // this takes col order so it's okay
				else
				{
					int nColOrder = OrderToIndex(nCol);
					str = GetItemTextOP(nRow, nColOrder);
				}
				CRect r;
				int x = rpim.m_nOffsetToLeft + m_anColPos.GetAt(nCol);
				int nWidth = m_anColWidth.GetAt(nCol);
//				int nHeight = nLineHeight * m_anRowLines.GetAt(j); //. for wordwrap
				int nHeight = nLineHeight * 1;
				r.SetRect(x, y, x + nWidth, y + nHeight);
//				pDC->TextOut(m_nOffsetToLeft, y, str);
//				pDC->ExtTextOut(x, y, ETO_CLIPPED, &r, str, NULL);
				UINT uFormat = DT_LEFT + DT_NOPREFIX + DT_WORDBREAK; //. DT_RIGHT center
				pDC->DrawText(str, &r, uFormat);
			}
			pDC->SelectObject(poldfont);

			// Draw line under row
			//, make optional
			y += nLineHeight;
			CPen* poldpen = pDC->SelectObject(&rpim.m_penLine);
			pDC->MoveTo(rpim.m_nOffsetToLeft, y);
			pDC->LineTo(rpim.m_nOffsetToLeft + rpim.m_nPageWidthWithinMargins, y);
			pDC->SelectObject(poldpen);

		}
	}
}




// Column Handlers
//-----------------------------------------------------------------------------------------------------------------





// HDN_ITEMCHANGED handler
// Use this to handle resizing column widths
BOOL CListCtrlEx::OnHeaderItemChanged(UINT id, NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMHEADER* phdr = (NMHEADER*) pNMHDR;
	HDITEM* pitem = phdr->pitem;
	int nItem = phdr->iItem;
	if (pitem->mask == HDI_WIDTH)
	{
		// Save new width
		if (m_bSaveChangesAutomatically && m_pdatColumns != NULL)
		{
			int nNewWidth = pitem->cxy;
			m_pdatColumns->SetColumnWidth(nItem, nNewWidth);
		}

		// Set flag
		m_bColumnsChanged = TRUE;

		// Notify parent window
		NotifyParentOfChanges();	
	}
	*pResult = 0;

	// Your function must return TRUE if the notification message has been completely handled or 
	// FALSE if other objects in the command routing should have a chance to handle the message.
	// Note: If you don't return FALSE here, the listview won't get the message and so it
	// won't know to resize its columns also.
	return FALSE;
}


// Sort by the column clicked on, if it's allowed
// Note: This only handles left click on a column header. 
void CListCtrlEx::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nCol = pNMListView->iSubItem;
	SortByColumn(nCol);
	*pResult = 0;
}




// Insert a new column in the listview. Will ask the user which property to insert.
// Pass the position to insert the new column at, otherwise will insert at the current
// location or at the end of the columns.
// Returns the index of the new column, or -1 if none added.
int CListCtrlEx::InsertColumnAsk(OBJID idProperty /*=0*/, int nCol /*=-1*/, BObject* pobjParent /*=NULL*/) {

	ASSERT_VALID(m_pDoc);

	int nNewCol = -1;

	// Get default class (eg if we're on a folder, it can have a default class associated with it)
	BObject* pobjDefaultClass = NULL;
	if (pobjParent)
		pobjDefaultClass = pobjParent->GetPropertyLink(propDefaultClass);
	if (pobjDefaultClass == NULL)
		pobjDefaultClass = m_pDoc->GetObject(classPaper);

	// Let user pick a property if not specified
	if (idProperty == 0) {
		CDialogSelectProperty dlg;
		dlg.m_pobjDefaultClass = pobjDefaultClass;
		if (dlg.DoModal() == IDCANCEL)
			return -1;
		idProperty = dlg.m_lngSelectedID;
	}

	// Check if property is already in header
	int i = FindColumn(idProperty);
	if (i == -1) {

		// Add to end of columns if none specified
		if (nCol == -1)
			nCol = GetColumnCount();
		else
			nCol = OrderToIndex(nCol); // convert from column order to position

		// Insert new column in bdatacolumns object
		if (m_bSaveChangesAutomatically && m_pdatColumns != NULL)
			m_pdatColumns->InsertColumn(idProperty, m_pDoc, 0, nCol);

		// Set flag so we know to save data
		m_bColumnsChanged = TRUE;

		// Add column to header/listview control
		BObject* pobjPropDef = m_pDoc->GetObject(idProperty);
		// 1.1d make sure we actually have an object!
		if (pobjPropDef)  {

			LPCTSTR pszColumnName = pobjPropDef->GetPropertyString(propName);
			int nColAlignment = pobjPropDef->GetPropertyDefAlignment();
			int nColWidth = pobjPropDef->GetPropertyDefWidth();
			int nItem = InsertColumn(nCol, pszColumnName, nColAlignment, nColWidth);

			// Set column item data
			CHeaderCtrl* phdr = GetHeaderCtrl();
			ASSERT_VALID(phdr);
			HDITEM hdi;
			hdi.mask = HDI_LPARAM;
			hdi.lParam = idProperty;
			phdr->SetItem(nItem, &hdi);

			// Notify parent window
			NotifyParentOfChanges();

			// Ask user if they want to add this property to the default class also
//			BObject* pobjCurrent = m_pDoc->GetCurrentObject();
//			BObject* pobjDefaultClass = pobjCurrent->GetPropertyLink(propDefaultClass);
//			ASSERT_VALID(pobjDefaultClass);
			// maybe this should be automatic?
			// only prob is if they add a folder and don't set up the class,
			// they just add properties, then later they set up the class
			// might wind up adding all these props to the paper class, for instance
//			LPCTSTR szClassName = pobjDefaultClass->GetPropertyString(propName);
//			CString strMsg;
//			strMsg.Format("Do you want to add this property to the %s definition class also?", szClassName);
//			if (AfxMessageBox(strMsg, MB_ICONQUESTION + MB_YESNO) == IDYES) {
//				pobjDefaultClass->GetPropertyData(propColumn
//			}

			// Return the new column index
			nNewCol = nCol;
		}
	}
	else { // already there
		AfxMessageBox(_T("That property is already in the view."), MB_ICONINFORMATION);
	}

	return nNewCol;
}








// Save the current column order info to the specified BDataColumns object.
BOOL CListCtrlEx::SaveColumnOrder(BDataColumns* pdatColumns)
{
	ASSERT_VALID(pdatColumns);

	// Create temp array to store column order info
	int nColumns = GetColumnCount();
	LPINT anOrder = (LPINT) malloc(nColumns*sizeof(int));
	ASSERT(anOrder);
//,	CUIntArray anOrder;
//	anOrder.SetSize(nColumns);

	// Get info to temp array
	GetColumnOrderArray(anOrder, nColumns);

	// Save temp array to bdata object
	pdatColumns->SetColumnOrder(anOrder, nColumns);

	// Delete temp array
//	delete anOrder; // bug: forgot to delete this memory block
	free((void*) anOrder); // bug: used delete instead of free

	return TRUE;
}


// Save the current column info array to the specified bobject, 
// either the bobject itself or its class, depending on its class.
// Folders always get their own unique column array, other objects don't.
BOOL CListCtrlEx::SaveColumnInfo(BObject *pobj)
{
	xTRACE("CListCtrlEx::SaveColumnInfo\n");

	ASSERT_VALID(m_pDoc);
	ASSERT_VALID(pobj);
	CHeaderCtrl* phdr = GetHeaderCtrl();
	ASSERT_VALID(phdr);

	if (m_bColumnsChanged)
	{
		// Create bdata object, then walk through columns and add them to bdata.
		BDataColumns* pdatColumns = new BDataColumns;
		int nCols = phdr->GetItemCount();
		HDITEM hdi;
		hdi.mask = HDI_LPARAM | HDI_WIDTH;
		for (int i = 0; i < nCols; i++)
		{
			phdr->GetItem(i, &hdi);
			ULONG lngPropertyID = hdi.lParam;
			int nWidth = hdi.cxy;
			pdatColumns->InsertColumn(lngPropertyID, m_pDoc, nWidth);
		}

		// Now save the column order
		SaveColumnOrder(pdatColumns);

		// Save pdatColumns to class or object.
		// BUG:: Used = instead of == again!
		if (pobj->GetClassID() == classFolder)
		{
			xTRACE("  save to object\n");
			// Save column info to the object
			pobj->SetPropertyData(propColumnInfoArray, pdatColumns, TRUE, FALSE);
		}
		else
		{
			xTRACE("  save to class\n");
			// Save column info to the object's class
			ULONG lngClassID = pobj->GetClassID();
			BObject* pobjClass = m_pDoc->GetObject(lngClassID);
			pobjClass->SetPropertyData(propObjectColumnInfoArray, pdatColumns, TRUE, FALSE);
		}
		delete pdatColumns;
		
		// Clear flag
		m_bColumnsChanged = FALSE;
	}

	return TRUE;
}


// Returns zero-based index of column that displays the specified property, or -1 if couldn't be found.
int CListCtrlEx::FindColumn(ULONG lngPropertyID) {

	CHeaderCtrl* phdr = GetHeaderCtrl();
	ASSERT_VALID(phdr);
	// Walk through columns to find property
	HDITEM hdi;
	hdi.mask = HDI_LPARAM;
	int nCols = phdr->GetItemCount();
	for (int i = 0; i < nCols; i++) {
		phdr->GetItem(i, &hdi);
		if (lngPropertyID == (ULONG) hdi.lParam)
			return i;
	}
	return -1;
}




// Remove the specified column from the listview.
// Don't let user remove Name column because it screws up icon handling.
// Default column of -1 means to remove the column with the current cell.
BOOL CListCtrlEx::RemoveColumn(int nCol /* = -1 */) {

	if (nCol == -1) nCol = m_nCol; // Use current column if none specified
	if (nCol == -1) return FALSE; // Exit if no column
	if (nCol >= GetColumnCount()) return FALSE; // Exit if invalid column

	ASSERT_VALID(m_pDoc);

	// Don't let user remove name property.
	ULONG lngPropID = GetColumnPropertyID(nCol);
	ASSERT(lngPropID);
	if (lngPropID == propName) {
		AfxMessageBox(_T("The Name property cannot be removed from the view."), MB_ICONINFORMATION);
		return FALSE;
	}

	// Verify removing column with user. 
	// This is kind of annoying but it's easy to click on the remove column button if you're curious and
	// blam - the name column is gone!
	CString strMsg;
	BObject* pobjProp = m_pDoc->GetObject(lngPropID);
	ASSERT_VALID(pobjProp);
	strMsg.Format("Are you sure you want to remove the column \"%s\" from this view?\nNote: This will not delete the property from your file, nor will it remove any property values stored in this column.", pobjProp->GetPropertyString(propName));
	if (AfxMessageBox(strMsg, MB_ICONQUESTION | MB_YESNO) == IDNO)
		return FALSE;

	// Delete property from array
	if (m_bSaveChangesAutomatically && m_pdatColumns != NULL) {
		m_pdatColumns->RemoveColumn(nCol);
	}

	// Set flag
	m_bColumnsChanged = TRUE;

	// Remove column from header
	DeleteColumn(nCol);

	// Notify parent window
	NotifyParentOfChanges();	

	return TRUE;
}




// Create columns from specified BDataColumns object.
// Returns number of columns added.
int CListCtrlEx::InitializeColumns(BDataColumns* pdatColumns, BOOL bExpandLastColumn /* = FALSE */, BOOL bSaveChangesAutomatically /* = FALSE */)
{
	xTRACE("CListCtrlEx::InitializeColumns\n");

	ASSERT_VALID(m_pDoc);
	ASSERT_VALID(pdatColumns);
	CHeaderCtrl* phdr = GetHeaderCtrl();
	ASSERT_VALID(phdr);

	// If user wants changes saved automatically then remember the pdatColumns
	m_bSaveChangesAutomatically = bSaveChangesAutomatically;
	if (bSaveChangesAutomatically)
		m_pdatColumns = pdatColumns;
	else
		m_pdatColumns = NULL;

	HDITEM hdi;
	hdi.mask = HDI_LPARAM;

	// Delete all columns
	while (DeleteColumn(0));

	// Walk through columns array and add columns
	ASSERT_VALID(pdatColumns);
	int nColumns = pdatColumns->GetColumnCount();
	for (int i = 0; i < nColumns; i++)
	{
		ColumnInfo& rci = pdatColumns->GetColumnInfo(i);
		BObject* pobjPropDef = pdatColumns->GetPropertyDef(i, m_pDoc);

		// Get alignment for column based on propertydef's property type (eg number is right aligned)
		int nAlignment = pobjPropDef->GetPropertyDefAlignment();

		// Add the column
		//, use macro for speed
		InsertColumn(i, pobjPropDef->GetPropertyString(propName), nAlignment, rci.GetColWidth());

		//, header control DOES have itemdata, just not exposed through listview!
		hdi.lParam = rci.GetPropertyID();
		phdr->SetItem(i, &hdi);
	}

	// Set order of columns
	LPINT paOrder = pdatColumns->GetColumnOrder();
	if (paOrder)
		VERIFY(SetColumnOrderArray(nColumns, paOrder));
	
	// Resize so that columns get expanded correctly
	m_bExpandLastColumn = bExpandLastColumn;
	RecalculateLayout();

	// Reset flag
	m_bColumnsChanged = FALSE;

	return nColumns;
}




// Update the specified column with the information from the specified property def.
// Call this if a property def changes, for instance.
// Returns True if successful
BOOL CListCtrlEx::UpdateColumn(int nCol, BObject* pobjPropertyDef)
{
	ASSERT_VALID(pobjPropertyDef);

	CHeaderCtrl* phdr = GetHeaderCtrl();
	ASSERT_VALID(phdr);

	// Get propertydef properties
	LPCTSTR szText = pobjPropertyDef->GetPropertyString(propName);
	ULONG lngPropertyID = pobjPropertyDef->GetObjectID();
	int nAlignment = pobjPropertyDef->GetPropertyDefAlignment();
	
	// Edit the column
	// must use setcolumn for text and setitem for lparam
	// SetItem would appear to handle both but text just turns out blank if you use it
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_FMT;
	// note cast - const char* to char* - this is okay because pszText has
	// two functions - read and write, and for this it's just reading the string. 
	lvc.pszText = (LPTSTR) szText; 
	lvc.fmt = nAlignment;
	SetColumn(nCol, &lvc);

	HDITEM hdi;
	hdi.mask = HDI_LPARAM;
	hdi.lParam = lngPropertyID;
	phdr->SetItem(nCol, &hdi);

	return TRUE;
}




// CSortObject
// Helper class used with sorting contents
class CSortObject
{
public:
	ULONG m_lngPropertyID;
	ULONG m_lngPropertyTypeID;
	int m_iSortDirection;
};



// Compare two BObjects for sorting purposes (callback function).
// Return value should be -1, 0, or 1.
static int CALLBACK CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// Get objects to compare
	BObject* pobj1 = (BObject*) lParam1;
	BObject* pobj2 = (BObject*) lParam2;

	// Get sort object and information it contains
	CSortObject* pso = (CSortObject*) lParamSort;
	ULONG lngPropertyID = pso->m_lngPropertyID;
	ULONG lngPropertyTypeID = pso->m_lngPropertyTypeID;
	int iDir = pso->m_iSortDirection;

	// Note: Could let the bdata classes handle comparisons, but that wouldn't work for 
	// some pseudo props that use pdoc's temp bdata vars, and it would be too slow.
	int iResult = 0;
	if (pobj1 && pobj2)
	{
/*
		//, Check for high priority items first
		// If object 1 is high priority and object 2 is not, put object 1 first
		// If object 2 is high priority and object 1 is not, put object 2 first
		BOOL bHigh1 = (pobj1->GetFlag(flagHighPriority));
		BOOL bHigh2 = (pobj2->GetFlag(flagHighPriority));
		BOOL bDifferent = (bHigh1 ^ bHigh2);
		if (bDifferent) return iDir * (bHigh1 ? -1 : 1);

		//, Check for folders - folders should come before other objects
		// If object 1 is a folder and object 2 is not, put object 1 first
		// If object 2 is a folder and object 1 is not, put object 2 first
		BOOL bFolder1 = (pobj1->GetClassID() == classFolder);
		BOOL bFolder2 = (pobj2->GetClassID() == classFolder);
		bDifferent = (bFolder1 ^ bFolder2);
		if (bDifferent) return iDir * (bFolder1 ? -1 : 1);
*/
		switch (lngPropertyTypeID)
		{
		case proptypeLong:
			{
				ULONG lng1 = pobj1->GetPropertyLong(lngPropertyID);
				ULONG lng2 = pobj2->GetPropertyLong(lngPropertyID);
				if (lng1 == lng2)
					iResult = 0;
				else
					iResult = lng1 < lng2 ? -1 : 1;
				break;
			}
		// All three of these property types use BDataNumber for storage
		case proptypeNumber:
		case proptypeCurrency:
		case proptypeCalculated:
			{
				//, will eventually need to compare using units also
				BDataNumber* pdat1 = DYNAMIC_DOWNCAST(BDataNumber, pobj1->GetPropertyData(lngPropertyID));
				if (pdat1 == 0) return iDir;
				BDataNumber* pdat2 = DYNAMIC_DOWNCAST(BDataNumber, pobj2->GetPropertyData(lngPropertyID));
				if (pdat2 == 0) return -iDir;

				if (pdat1 < pdat2)
					iResult = -1;
				else if (pdat1 > pdat2)
					iResult = 1;
				else
					iResult = 0;
				break;
				delete pdat1;
				delete pdat2;
			}
		case proptypeDate:
			{
				//, will eventually need to compare using units also
				BDataDate* pdat1 = DYNAMIC_DOWNCAST(BDataDate, pobj1->GetPropertyData(lngPropertyID));
				if (pdat1 == 0) return iDir;
				BDataDate* pdat2 = DYNAMIC_DOWNCAST(BDataDate, pobj2->GetPropertyData(lngPropertyID));
				if (pdat2 == 0) return -iDir;

				if (pdat1 == pdat2)
					iResult = 0;
				else
					iResult = pdat1 < pdat2 ? -1 : 1;

				BOOL bString1 = pdat1->IsStringDate();
				BOOL bString2 = pdat2->IsStringDate();

				if (bString1) iResult = iDir;
				else if (bString2) iResult = -iDir;

				delete pdat1;
				delete pdat2;
			}
		default: // (treat as strings)
			{
				LPCTSTR psz1 = pobj1->GetPropertyString(lngPropertyID);
				LPCTSTR psz2 = pobj2->GetPropertyString(lngPropertyID);
				iResult = lstrcmpi(psz1, psz2);
				break;
			}
		}
	}

	// Change direction if descending sort (iDir is -1 if descending, or 1 if ascending)
	iResult *= iDir;

	return iResult;
}




// Sort the listview by the specified property.
// If no property specified will use the current sort property or propName and current direction
// (just does a resort).
// This will remove the dummy new row, if any, before sorting, then re-add it when done.
// To toggle existing sort direction pass iDir = 0, or 1 for ascending or -1 for descending.
// To clear sort memory call ClearSortMemory
//. and show an up or down arrow too
void CListCtrlEx::SortByProperty(ULONG lngPropertyID /* = 0 */, int iDir /* = 0 */)
{
	ASSERT_VALID(m_pDoc);

	CWaitCursor wc;

	// If no property specified, use the current sort property if any, or name if none.
	if (lngPropertyID == 0)
	{
		if (m_lngSortPropertyID)
			lngPropertyID = m_lngSortPropertyID;
		else
			lngPropertyID = propName;
		// Also use the current sort direction
		iDir = m_iSortDirection;
	}

	// Remember current row data and column (if any)
	DWORD dwCurrentData = GetSelectedItemData();
	int nCurrentCol = m_nCol;

	// Remove dummy new row, if any
	RemoveDummyRow();

	// Get property type
	ULONG lngPropertyTypeID = proptypeString; // default is string
	BObject* pobjPropDef = m_pDoc->GetObject(lngPropertyID);
	if (pobjPropDef)
	{
		ASSERT_VALID(pobjPropDef);
		BObject* pobjPropType = pobjPropDef->GetPropertyLink(propPropertyType);
		if (pobjPropType)
		{
			ASSERT_VALID(pobjPropType);
			lngPropertyTypeID = pobjPropType->GetObjectID();
		}
	}

	// User wants to toggle the sort direction for this column
	if (iDir == 0)
	{
		// See if this column has already been sorted, if so, toggle the direction,
		// otherwise do an ascending sort
		if (lngPropertyID == m_lngSortPropertyID)
			iDir = -m_iSortDirection;
		else
			iDir = 1;
	}
	ASSERT (iDir == 1 || iDir == -1);

	// Create an object that includes sort props and directions, and proptype too
	CSortObject so;
	so.m_lngPropertyID = lngPropertyID;
	so.m_lngPropertyTypeID = lngPropertyTypeID;
	so.m_iSortDirection = iDir;

	// Sort items using our comparison function
	CListCtrl::SortItems(CompareItems, (DWORD) &so);

	// Re-add dummy new row, if necessary
	AddDummyRow();

	// Reselect current row
	if (dwCurrentData != 0)
	{
		int nRow = FindItemData(dwCurrentData);
		SelectCell(nRow, nCurrentCol);
	}

	// Show up or down icon
	//. image list won't cut it - will need to do owner draw to get it to appear in the center like outlook
	//. use CustomDraw message?
//	CHeaderCtrl* pHeader = m_lvw.GetHeaderCtrl();
//	ASSERT_VALID(pHeader);
//	HDITEM hdi;
//	pHeader->GetItem(iCol, &hdi);
//	hdi.mask = HDI_IMAGE | HDI_FORMAT;
//	hdi.iImage = 0;
//	hdi.fmt = HDF_IMAGE | HDF_STRING;
//	pHeader->SetItem(iCol, &hdi);

	// Remember the current sort property and direction
	m_lngSortPropertyID = lngPropertyID;
	m_iSortDirection = iDir;
}


// Sort the listview by the specified column. 
// Calls SortByProperty.
//void CListCtrlEx::SortByColumn(int nCol, BDoc* pDoc, int iDir /* = 0 */)
void CListCtrlEx::SortByColumn(int nCol, int iDir /* = 0 */)
{
	ULONG lngPropertyID = GetColumnPropertyID(nCol);
	SortByProperty(lngPropertyID, iDir);
}



// This clears the sort variables
void CListCtrlEx::ClearSortMemory()
{
	m_lngSortPropertyID = 0;
	m_iSortDirection = 0;
}





void CListCtrlEx::OnColumnRClick(NMHDR * pNMHDR, LRESULT * pResult) {

	NMHEADER* phdr = (NMHEADER*) pNMHDR;
	*pResult = 0;

	// Exit if not auto columns
	if (!m_bAutoColumns) return;

	CPoint ptScreen(::GetMessagePos()); // Get position of message source
	
	// Get column that was right clicked on and save to module level variable.
	// Note: The help doesn't explain this but this does account for column reordering.
	// So if you moved the description column to the end, and rclicked on it there,
	// the index retrieved would be the same.
	CPoint ptClient = ptScreen;
	GetHeaderCtrl()->ScreenToClient(&ptClient);
	HDHITTESTINFO hti;
	hti.pt = ptClient;
	m_nTargetColumn = GetHeaderCtrl()->SendMessage(HDM_HITTEST, 0, (LPARAM) (LPHDHITTESTINFO) &hti);

	// Show popup menu
	BCMenu menu;
	menu.LoadMenu(IDR_POPUP_CONTENTS_HEADER);
	menu.LoadToolbar(IDR_MAINFRAME);
	menu.LoadToolbar(IDR_TOOLBAR_CONTENTS);
	// load any other toolbars here for more images
	BCMenu* pPopup = (BCMenu*) menu.GetSubMenu(0);
	if (pPopup) {
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
	}

}


// Sort column contents ascending
void CListCtrlEx::OnColumnSortAscending() 
{
	SortByColumn(m_nTargetColumn, 1);
}


// Sort column contents descending
void CListCtrlEx::OnColumnSortDescending() 
{
	SortByColumn(m_nTargetColumn, -1);
}


// Edit the property associated with the specified column
void CListCtrlEx::OnColumnEditProperty() 
{
	ULONG lngPropertyID = GetColumnPropertyID(m_nTargetColumn);
	BObject* pobjPropertyDef = m_pDoc->GetObject(lngPropertyID);
	m_pDoc->UIEditPropertyDef(pobjPropertyDef);
}



// Insert a new column.
// m_nTargetColumn has been set. 
void CListCtrlEx::OnColumnInsert() {

	xTRACE("CListCtrlEx OnColumnInsert\n");
	BObject* pobjCurrent = m_pDoc->GetCurrentObject();
	// First we want to disable all the currently visible properties
	DisableVisibleProperties();
	// Don't let user insert something before first column
	// (otherwise the control gets screwy and the icon sticks with the new col 0 and the Name column has no icon)
	if (IndexToOrder(m_nTargetColumn) == 0)
		m_nTargetColumn = OrderToIndex(1);
	OBJID idProperty = 0; // pass 0 to make it ask user 
	int nCol = InsertColumnAsk(idProperty, m_nTargetColumn, pobjCurrent); 
	EnableAllProperties();
}


// Delete a column
void CListCtrlEx::OnColumnDelete() 
{
	if (m_nTargetColumn != -1)
	{
		RemoveColumn(m_nTargetColumn);
	}
}




// Enable all properties in the Properties system folder by clearing the flagDisabled flag.
void CListCtrlEx::EnableAllProperties() {

	ASSERT_VALID(m_pDoc);

	// Clear all properties' flags
	BObject* pobjProperties = m_pDoc->GetObject(folderProperties);
	ASSERT_VALID(pobjProperties);
	pobjProperties->SetFlag(flagDisabled, FALSE, TRUE);
}



// Disable the currently visible properties by setting their flagDisabled flag.
void CListCtrlEx::DisableVisibleProperties() {

	ASSERT_VALID(m_pDoc);

	// Clear all properties' flags
	EnableAllProperties();

	// Disable the currently visible properties
	int nColumns = GetColumnCount();
	for (int i = 0; i < nColumns; i++) {
		ULONG lngPropertyID = GetColumnPropertyID(i);
		BObject* pobjProp = m_pDoc->GetObject(lngPropertyID);
		ASSERT_VALID(pobjProp);
		pobjProp->SetFlag(flagDisabled, TRUE, FALSE);
	}	
}






// Clear any existing sort by reloading items
void CListCtrlEx::OnColumnSortClear() 
{	
	// Remember current row data and column (if any)
	DWORD dwCurrentData = GetSelectedItemData();
	int nCurrentCol = m_nCol;

	// Add the children of the start object to the list, with no recursion
	DeleteAllItems();
	BObject* pobjStart = m_pDoc->GetCurrentObject();
	AddObjects(pobjStart, theApp.m_lngExcludeFlags, FALSE, FALSE, 0, TRUE);

	// Add dummy new row
	AddDummyRow();
	
	// Clear sort
	ClearSortMemory();

	// Reselect item
	if (dwCurrentData != 0)
	{
		int nRow = FindItemData(dwCurrentData);
		SelectCell(nRow, nCurrentCol);
	}
}


void CListCtrlEx::OnUpdateColumnSortClear(CCmdUI* pCmdUI) 
{
	// Should only be enabled if autosort is OFF for parent
	// (because in that case, user can move items up and down, and will need to be able to
	// turn sort off in order to do that).
	// Also only enable if contents are sorted
	BObject* pobjStart = m_pDoc->GetCurrentObject();
	BOOL bAutosort = !(pobjStart->GetFlag(flagNoAutosort));
	BOOL bEnable = (bAutosort == FALSE) && (m_lngSortPropertyID != 0);
	pCmdUI->Enable(bEnable);
}




// Save column info when columns have changed.
// This is the CM_COLUMNS_CHANGED handler, sent by the control.
LRESULT CListCtrlEx::OnLvwColumnsChanged(WPARAM wParam, LPARAM lParam)
{
	xTRACE("CListCtrlEx CM_COLUMNS_CHANGED handler - save column info\n");
	BObject* pobj = m_pDoc->GetCurrentObject();
	ASSERT_VALID(pobj);
	SaveColumnInfo(pobj); // this saves the BDataColumns object to the current pobj
	return 0;
}



// Follow the link in the current cell.
// If on name property, open that object.
void CListCtrlEx::FollowLink(CView* pViewToRestore /* = NULL */)
{
	if (m_nCol == 0 && m_nRow != -1) 
	{
		if (m_nRow < GetItemCount())
		{
			BObject* pobj = (BObject*) GetItemData(m_nRow);
			if (pobj)
			{
				ASSERT_VALID(pobj);
				// Select item in treeview
				// This will broadcast all the update messages necessary (save, select, load)
				m_pDoc->SetCurrentObject(pobj);
				// now restore focus to right side
				if (pViewToRestore)
				{
					ASSERT_VALID(pViewToRestore);
					pViewToRestore->SetFocus();
				}
			}
		}
	}
}





// Select first item if nothing is currently selected,
// and there are actually items in the list. 
BOOL CListCtrlEx::SelectAnything()
{
	if (this->GetSelectedItem() == -1)
	{
		if (this->GetItemCount() > 0)
		{
			this->SelectItem(0);
			return TRUE;
		}
	}
	return FALSE;
}



