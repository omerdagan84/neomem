
// CViewSearch


#include "precompiled.h"

#include "ViewSearch.h"
#include "Constants.h"
#include "FrameChild.h"
#include "Hint.h"
#include "NeoMem.h"
#include "ViewHeader.h"
#include "ViewProperties.h"
#include "ViewRtf.h"
#include "ViewTabs.h"

#include "ConstantsDatabase.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CViewSearch, CViewEx)

BEGIN_MESSAGE_MAP(CViewSearch, CViewEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LVW_RESULTS, OnNotifyItemChanged)
	//{{AFX_MSG_MAP(CViewSearch)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_KEY_RETURN, OnKeyReturn)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_FIND_NEXT, OnCmdEditFindNext)
	ON_COMMAND(ID_KEY_TAB, OnKeyTab)
	ON_COMMAND(ID_KEY_SHIFT_TAB, OnKeyShiftTab)
	ON_COMMAND(ID_OBJ_OPEN, OnObjOpen) //ok
	ON_COMMAND(ID_OBJ_EDIT_IN_PLACE, OnObjEditInPlace) //ok
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_OBJ_MOVE_DOWN, OnUpdateObjMoveDown)
	ON_UPDATE_COMMAND_UI(ID_OBJ_MOVE_UP, OnUpdateObjMoveUp)
	ON_COMMAND(ID_NAVIGATE_FOLLOW_LINK, OnNavigateFollowLink)
	ON_COMMAND(ID_OBJ_MOVE_TO, OnObjMoveTo)
	ON_UPDATE_COMMAND_UI(ID_OBJ_MOVE_TO, OnUpdateObjMoveTo)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_GO, OnBtnGo)
	ON_BN_CLICKED(IDC_BTN_NEXT_MATCH, OnCmdEditFindNext) // use f3 handler
	ON_NOTIFY(NM_CLICK, IDC_LVW_RESULTS, OnNotifyClick)
	ON_NOTIFY(NM_DBLCLK, IDC_LVW_RESULTS, OnNotifyDblClick)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LVW_RESULTS, OnBeginLabelEdit)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LVW_RESULTS, OnEndLabelEdit)
END_MESSAGE_MAP()



// Construction/Destruction
//-------------------------------------------------------------------------------------------

CViewSearch::CViewSearch()
{
	xTRACE("CViewSearch Constructor\n");
	
	m_lngPropertyID = 0;
	m_bMatchCase = FALSE;
	m_bWholeWord = FALSE;

	m_nSearchInLabelWidth = 0;
//	m_nSearchForLabelWidth = 0;
	m_nButtonWidth = 0;
//	m_nButtonHeight = 0;

//	m_pobjPopup = 0;
//	m_nPopupItem = -1;

	// Initialize tab order (ie for tabbing between controls - must do manually)
	m_to.Add(IDC_TXT_SEARCH);
	m_to.Add(IDC_CBO_SEARCH_IN);
	m_to.Add(IDC_LVW_RESULTS);
}

CViewSearch::~CViewSearch()
{
}


int CViewSearch::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// Call base class
	if (CViewEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetFont(&(theApp.m_fontControls)); // set default font for this window and all children (right?)

	// Create list control
	CRect r;
	r.SetRectEmpty();
	if (m_lvw.Create(
					WS_CHILD | 
					WS_VISIBLE | 
					LVS_REPORT |			// Report view mode
//					LVS_SHOWSELALWAYS |		// Always show selection
					LVS_SINGLESEL |			// Allows only one item to be selected (default is multiple)
					LVS_EDITLABELS |		// Allow editing
					LVS_SHAREIMAGELISTS |	// Share image lists
					0, r, this, IDC_LVW_RESULTS))
	{
		// Tell CViewEx about child window
		SetChildView(&m_lvw);

		m_lvw.m_bAutoColumns = TRUE;

		// Initialize listview
		m_lvw.m_pDoc = m_pDoc;
		m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
		m_lvw.SetFullRowSelect(TRUE);

		// Initialize columns
		//, Keep columns proportional
		m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsSearchView), TRUE, TRUE);
	}
	else return -1;


	// Create toolbar
//	if (m_tbr.Create(0, CRect(0,0,0,0), this, IDR_TOOLBAR_SEARCH))
	if (m_tbr.Create(TBSTYLE_WRAPABLE, CRect(0,0,0,0), this, IDR_TOOLBAR_SEARCH))
	{
		// Load toobar from resource file
		// Also adds 1 pixel to the button height to give us more room for controls!
		if (!m_tbr.LoadToolBar(MAKEINTRESOURCE(IDR_TOOLBAR_SEARCH), 1))
			return -1;

		// Let toolbar have more than one row
		// Note: Can't do modify style to wrappable after loading toolbar because it will revert
		// to default size, so I put it in Create call above.
//		m_tbr.ModifyStyle(0, TBSTYLE_WRAPABLE);

		// Get width of buttons
		DWORD dwSize = m_tbr.GetButtonSize();
		m_nButtonWidth = LOWORD(dwSize);
		m_nButtonHeight = HIWORD(dwSize);

		// Add "Search In" label
		CString strLabel = " Search In";
		m_lblSearchIn.m_clrBackground = g_clr3dFace;
		if (m_lblSearchIn.Create(strLabel, r, &m_tbr, 0, FALSE, DT_LEFT | DT_VCENTER))
//		if (m_lblSearchIn.Create(strLabel, r, &m_tbr, 0, FALSE, DT_CENTER | DT_VCENTER))
		{
			m_nSearchInLabelWidth = m_lblSearchIn.m_rTextSize.right;
		}
		else return -1;

		// Add combo
		// Note: The height of this rectangle determines the height of the dropdown of the combobox
		r.SetRect(0, 0, 100, 200);
		if (m_cboSearchIn.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, r, &m_tbr, IDC_CBO_SEARCH_IN))
		{
			// Initialize comboboxex
			m_cboSearchIn.SetFont(&(theApp.m_fontControls));
			m_cboSearchIn.SetImageList(m_pDoc->GetImageList());

			//, make methods to do all this stuff..

			// Get propertydef icon
			BObject* pobj = m_pDoc->GetObject(classProperty);
			ASSERT_VALID(pobj);
			int nImage = pobj->GetIconIndex(); 

			// Add "All Properties"
			COMBOBOXEXITEM cbi;
			cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
			cbi.pszText = "All Properties";
			cbi.iItem = 0; 
			cbi.iImage = nImage;
			cbi.iSelectedImage = nImage;
			cbi.iIndent = 0;
			cbi.lParam = 0; // all properties
			int nIndex = m_cboSearchIn.InsertItem(&cbi);
			ASSERT (nIndex != -1);

			// Add "Text View" 
			// Must do this manually because propRtf is a hidden property.
			cbi.pszText = "Text View";
			cbi.iItem = 1;
//			cbi.lParam = propRtfText;
			cbi.lParam = (long) m_pDoc->GetObject(propRtfText);
			nIndex = m_cboSearchIn.InsertItem(&cbi);
			ASSERT(nIndex != -1);

			// Now add all individual properties in alpha order
			BObject* pobjProperties = m_pDoc->GetObject(folderProperties);
			m_cboSearchIn.AddObjects(pobjProperties, theApp.m_lngExcludeFlags);

			// Remove "Class" and "Icon" properties because we don't search through those currently.
//			pobj = m_pDoc->GetObject(propClassName);
//			nIndex = m_cboSearchIn.FindItemData((DWORD) pobj);
//			m_cboSearchIn.DeleteItem(nIndex);

//			pobj = m_pDoc->GetObject(propIcon);
//			nIndex = m_cboSearchIn.FindItemData((DWORD) pobj);
//			m_cboSearchIn.DeleteItem(nIndex);

			// Select first item
			m_cboSearchIn.SetCurSel(0);
		}
		else return -1;

		// Add "Search For" label
//		strLabel = "Search For";
//		if (m_lblSearchFor.Create(strLabel, WS_CHILD | WS_VISIBLE | SS_SIMPLE, r, &m_tbr))
//		{
//			m_lblSearchFor.SetFont(&(theApp.m_fontControls));
//			CDC* pdc = GetDC();
//			CSize sz = pdc->GetTextExtent(strLabel);
//			m_nSearchForLabelWidth = sz.cx;
//		}
//		else return -1;

		// Add search textbox
		CRect r;
//		UINT nIndex = m_tbr.CommandToIndex(IDC_TXT_SEARCH);
//		VERIFY(m_tbr.GetItemRect(nIndex, &r));
		VERIFY(m_tbr.GetItemRectFromID(IDC_TXT_SEARCH, r));
		if (m_txtSearchFor.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, r, &m_tbr, IDC_TXT_SEARCH))
		{
			m_txtSearchFor.m_bHighlightOnEntry = TRUE; // highlight text on entry
			m_txtSearchFor.m_bWantReturn = FALSE;
			m_txtSearchFor.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
			m_txtSearchFor.SetFont(&theApp.m_fontControls);			
			m_txtSearchFor.SetWindowText("Enter search text here");
		}
		else return -1;

		// Must call this to initialize the toolbar position correctly
		m_tbr.Position();

		// Calculate position of all controls on toolbar
		RecalculateLayout();
		
		// Tell the view that there is a toolbar attached to it
		SetToolBar(&m_tbr);

		// Get width of separators
//		CRect r;
//		m_tbr.GetItemRect(3, r);
//		int nSeparatorWidth = r.right - r.left + 1;

		// Make the dummy button a separator
		// not necessary
//		TBBUTTONINFO tbi;
//		tbi.cbSize = sizeof(TBBUTTONINFO);
//		tbi.dwMask = TBIF_STYLE;
//		tbi.fsStyle = TBSTYLE_SEP;
//		m_tbr.SetButtonInfo(ID_TXT_SEARCH, &tbi);

//,		m_tbr.MapAccelerator
//.		m_tbr.EnableButton();
	
	}


/*
	// Create combobox to enter search text
	// Note: The height of this rectangle determines the height of the dropdown of the combobox
	r.SetRect(0, 0, 100, 200);
	if (!m_cboSearch.Create(WS_CHILD | WS_VISIBLE | 
											WS_TABSTOP |
											WS_VSCROLL |
											CBS_DROPDOWN | // edit
											CBS_AUTOHSCROLL |
											0, r, this, IDC_CBO_SEARCH))
		return -1;

	// Add previous search terms to combo
	// (These are serialized in the document)
	CStringList& lstSearches = m_pDoc->m_lstSearches;
	POSITION pos = lstSearches.GetTailPosition();
	while (pos)
	{
		CString str = lstSearches.GetPrev(pos);
		m_cboSearch.AddString(str);
	}

	// Create properties combo
	// Note: The height of this rectangle determines the height of the dropdown of the combobox
	r.SetRect(0, 0, 100, 200);
	if (!m_cboSearchIn.Create(WS_CHILD | WS_VISIBLE | 
											WS_TABSTOP |
											WS_VSCROLL |
											CBS_DROPDOWNLIST | // static
											CBS_AUTOHSCROLL |
											0, r, this, IDC_CBO_PROPERTIES))
		return -1;

	// Create checkboxes
	if (!m_chkMatchCase.Create(_T("Match Case"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
						r, this, IDC_CHK_MATCH_CASE))
		return -1;
	if (!m_chkWholeWord.Create(_T("Whole Word"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
						r, this, IDC_CHK_WHOLE_WORD))
		return -1;

  // set focus
//	GotoDlgCtrl(GetDlgItem(IDC_CBO_SEARCH));

*/

	// Load custom accelerators for this view
	LoadViewAccelerators(IDR_VIEW_SEARCH);

	return 0;
}



// Diagnostics
//-------------------------------------------------------------------------------------------

#ifdef _DEBUG
void CViewSearch::AssertValid() const
{
	CView::AssertValid();
}

void CViewSearch::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG




// Drawing
//-------------------------------------------------------------------------------------------

void CViewSearch::OnDraw(CDC* pDC)
{
	// Call base class
	CViewEx::OnDraw(pDC);
}


void CViewSearch::OnSize(UINT nType, int cx, int cy) 
{
	// Resize toolbar buttons and controls
//	if ((cx != 0) && (cy != 0))
		RecalculateLayout(cx, cy);

	// Now we can call the base class, which needs the height of the toolbar 
	CViewEx::OnSize(nType, cx, cy);
}



// Arrange toolbar buttons and controls
void CViewSearch::RecalculateLayout(int cx /* = 0 */, int cy /* = 0 */)
{
	CRect r;

	// Get client width and height
	if (cx == 0)
	{
		GetClientRect(r);
		cx = r.right;
		cy = r.bottom;
	}
//	if (cx == 0) return;

	// Initialize button structure
	TBBUTTONINFO tbi;
	tbi.cbSize = sizeof(TBBUTTONINFO);
	tbi.dwMask = TBIF_SIZE;

	// Get width of toolbar
	int nToolbarWidth = cx - 4; // 4 pixels for borders

	// Position label "search for"
//	tbi.cx = m_nSearchForLabelWidth; // obtained in oncreate
//	m_tbr.SetButtonInfo(IDC_LBL_SEARCH_FOR, &tbi);
//	m_tbr.GetItemRect(2, r);
//	r.bottom -= 1;
//	m_lblSearchFor.MoveWindow(r);

	// Position textbox for search term
	int nTextWidth = nToolbarWidth - 2 * m_nButtonWidth; // 2 buttons. button width from oncreate.
	tbi.cx = nTextWidth; // width in pixels
	m_tbr.SetButtonInfo(IDC_TXT_SEARCH, &tbi);
//	m_tbr.GetItemRect(2, r);
//	m_tbr.GetItemRect(m_tbr.CommandToIndex(IDC_TXT_SEARCH), r);
	m_tbr.GetItemRectFromID(IDC_TXT_SEARCH, r);
	r.DeflateRect(0, 1, 0, 1);
//	r.bottom -= 1;
	m_txtSearchFor.MoveWindow(r);

	// Position label "search in"
	tbi.cx = m_nSearchInLabelWidth; // obtained in oncreate
	m_tbr.SetButtonInfo(IDC_LBL_SEARCH_IN, &tbi);
//	m_tbr.GetItemRect(0, r);
	m_tbr.GetItemRectFromID(IDC_LBL_SEARCH_IN, r);
	r.bottom -= 1;
	m_lblSearchIn.MoveWindow(r);

	// Position properties combo
	tbi.cx = nToolbarWidth - m_nSearchInLabelWidth;
	m_tbr.SetButtonInfo(IDC_CBO_SEARCH_IN, &tbi);
//	m_tbr.GetItemRect(1, r);
	m_tbr.GetItemRectFromID(IDC_CBO_SEARCH_IN, r);
	r.bottom -= 2; //.
	m_cboSearchIn.MoveWindow(r);

	// Now resize the toolbar so that it will position buttons on the two rows correctly
	r.SetRect(0, 0, nToolbarWidth, cy);
	m_tbr.SetRows(2, TRUE, r);

}




// Message Handlers
//-------------------------------------------------------------------------------------------

void CViewSearch::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	TRACE("    CViewSearch::OnUpdate %s\n", theApp.GetHintName(lHint));

	switch (lHint)
	{

//	case hintMove:
	case hintDelete:
		{
			CHint* ph = (CHint*) pHint;
			int nObjects = ph->m_paObjects->GetSize();
			for (int i = 0; i < nObjects; i++)
			{
				BObject* pobj = (BObject*) ph->m_paObjects->GetAt(i);
				// Note: Object has already been deleted, so don't validate it!
//				ASSERT_VALID(pobj);
				// remove from the list
				m_lvw.DeleteItemData((LPARAM) pobj);
				// also from the property list
				m_cboSearchIn.DeleteItemData((LPARAM) pobj);
			}
			break;
		}

	case hintPropertyChange:
		{
			// Find item and refresh it
			CHint* ph = (CHint*) pHint;
			ASSERT_VALID(ph);
			BObject* pobj = ph->m_pobjObject;
			ASSERT_VALID(pobj);
			ULONG lngPropertyID = ph->m_lngPropertyID;
			// check the main list
			int nItem = m_lvw.FindItemData((LPARAM) pobj);
			if (nItem != -1)
			{
				// Update icon if that's what's changing
				if (lngPropertyID == propIconID)
				{
					int nImage = pobj->GetIconIndex();
					m_lvw.SetItem(nItem, 0, LVIF_IMAGE, 0, nImage, 0, 0, 0);
				}
				m_lvw.Update(nItem);
			}
			// and also the property list
			nItem = m_cboSearchIn.FindItemData((LPARAM) pobj);
			if (nItem != -1)
			{
				// Update icon if that's what's changing
				if (lngPropertyID == propIconID)
				{
					int nImage = pobj->GetIconIndex();
					m_cboSearchIn.SetItemIcon(nItem, nImage);
				}
				// otherwise refresh the cbo list
				m_cboSearchIn.RedrawWindow();
			}
			// Bug: forgot to put break;!!!!
			break;
		}

	case hintRefresh:
		{
			// Redisplay all objects
			m_lvw.RedrawWindow();
			break;
		}

	case hintRefreshAllIcons:
		{
			// Walk through all items, reobtaining their icons
			int nItems = m_lvw.GetItemCount();
			for (int i = 0; i < nItems; i++)
			{
				BObject* pobj = (BObject*) m_lvw.GetItemData(i);
				if (pobj)
				{
					ASSERT_VALID(pobj);
					int nIcon = pobj->GetIconIndex();
					m_lvw.SetItem(i, 0, LVIF_IMAGE, 0, nIcon, 0, 0, 0);
				}					
			}
			break;
		}

	}
}



void CViewSearch::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	xTRACE("CViewSearch OnActivateView %d\n", bActivate);
	CViewEx::OnActivateView(bActivate, pActivateView, pDeactiveView);
	// If this view is being activated, set the focus to the child control
	if (bActivate)
	{
		RecalculateLayout();
//		m_cboSearch.SetFocus();
		m_txtSearchFor.SetFocus();
		// select the text in it
//		m_txtSearchFor.SetSel(0, -1, TRUE);
//		m_cboSearchIn.SetFocus();
	}
}



void CViewSearch::OnNotifyDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Switch to data view
	OnObjOpen();
}




void CViewSearch::OnSelectSearch()
{
	OnBtnGo();
}


// Handler for return key accelerator ID_KEY_RETURN
void CViewSearch::OnKeyReturn() 
{
	xTRACE("CViewSearch OnKeyReturn\n");
	// If in listview window, it's as if user double clicked on item
//	if (::GetFocus() == m_lvw.m_hWnd)
//		OnObjOpen();
	// Otherwise, just do the search in the cbo box
//	else
	// If enter was pressed in textbox then do the search
	if (::GetFocus() == m_txtSearchFor.m_hWnd)
		OnBtnGo();
}





// Popup Menu
//------------------------------------------------------------------------------------------------------

//, is there a way we could handle context menus more generally?
// ie create some object that will create the menu and handle the item selected??
// i don't like having to do this in each view
// let CViewEx handle it?
// ie it could call virtual method to get selected objects into an array,
// then depending on what was selected, bring up a different menu
// or could call a virtual method to get the popup menu
void CViewSearch::OnContextMenu(CWnd* pWnd, CPoint ptScreen) 
{
	// Set flag if user pressed context key (x and y will be -1)
	BOOL bContextKey = ((ptScreen.x == -1) && (ptScreen.y == -1));

	// Exit if right click was in header control
	if (!bContextKey)
	{
		CRect r;
		m_lvw.GetHeaderCtrl()->GetClientRect(&r);
		m_lvw.GetHeaderCtrl()->ClientToScreen(&r);
		if (r.PtInRect(ptScreen)) return;
	}

	// If pressed context key, get position from current cell
	if (bContextKey)
	{
		CRect r;
		m_lvw.GetCellRect(m_lvw.m_nRow, m_lvw.m_nCol, r);
		m_lvw.ClientToScreen(&r);
		ptScreen.x = r.left + 6;
		ptScreen.y = r.top + 6;
	}

	// Get selected item
	int nItem = m_lvw.GetSelectedItem();
	if (nItem == -1) // nothing selected
	{
		// Show popup menu for background
//		AfxMessageBox("menu for background");
	}
	else
	{
		// Get bobject address
		BObject* pobj = (BObject*) m_lvw.GetItemData(nItem);
		
		// Save item so that menu handlers will know which object was clicked on
//		m_pobjPopup = pobj;
//		m_nPopupItem = nItem;
		m_pDoc->SetTargetObject(pobj);

		// Display pop up menu
//		CMenu menu;
//		if (menu.LoadMenu(IDR_POPUP_SEARCH))
//		{
//			CMenu* pPopup = menu.GetSubMenu(0);
//			ASSERT(pPopup != NULL);
			BCMenu* pPopup = InitPopup(IDR_POPUP_SEARCH, ID_OBJ_OPEN);
			if (pPopup)
			{
//				pPopup->SetDefaultItem(ID_OBJ_OPEN);
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
			}
//		}
	}
}



void CViewSearch::OnObjOpen()  // ok
{
	if (m_lvw.GetSelectedCount() == 0)
		return;

	// Switch to data view
	ShowView(viewData, TRUE, FALSE);
}



void CViewSearch::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	xTRACE("CViewSearch OnBeginLabelEdit\n");
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	// Clear accelerators so edit control will receive Returns!
	LoadViewAccelerators(0);
}


void CViewSearch::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	xTRACE("CViewSearch::OnEndLabelEdit\n");

	//, do data validation, return FALSE if you don't like it
 
	// Reload accelerators so we will get the Return command!
	LoadViewAccelerators(IDR_VIEW_SEARCH);
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LVITEM* pLVITEM = &(pDispInfo->item);
	if (pLVITEM->pszText)
	{
		// bug: for some reason lparam is not always filled out correctly!
//		BObject* pobj = (BObject*) pLVITEM->lParam;
//		ASSERT_VALID(pobj);
		int nItem = pLVITEM->iItem;
		BObject* pobj = (BObject*) m_lvw.GetItemData(nItem);
		ASSERT_VALID(pobj);
		
		pobj->SetPropertyText(propName, pLVITEM->pszText);

		*pResult = TRUE;
	}
	else
	{
		*pResult = 0;
	}
}






// Search
//--------------------------------------------------------------------------------------------------


// Do Search
void CViewSearch::OnBtnGo()
{
	CWaitCursor wc;

	// Send hintSave message so any current changes get saved
	BObject* pobjCurrent = m_pDoc->GetCurrentObject();
	ASSERT_VALID(pobjCurrent);
	m_pDoc->UpdateAllViewsEx(this, hintSave, pobjCurrent);

	// Clear any sort memory from listview
	m_lvw.ClearSortMemory();

	// Get search text
//	m_cboSearch.GetWindowText(m_strFindText);
	m_txtSearchFor.GetWindowText(m_strFindText);
	if (m_strFindText.IsEmpty()) return;

	//, Add search text to list and remove any extra
//	CStringList& lstSearches = m_pDoc->m_lstSearches;
//	lstSearches.AddTail(m_strFindText);
//	while (lstSearches.GetCount() > m_nMaxSearches)
//		lstSearches.RemoveHead();
	// Also add to combobox
//	m_cboSearch.InsertString(0, m_strFindText);
//	if (m_cboSearch.GetCount() > m_nMaxSearches)
//		m_cboSearch.DeleteString(m_nMaxSearches); // delete the last string

	// Get property to search in (0 = all properties)
	int nIndex = m_cboSearchIn.GetCurSel();
	m_lngPropertyID = 0;
	BObject* pobjProp = (BObject*) m_cboSearchIn.GetItemData(nIndex);
	if (pobjProp)
	{
		ASSERT_VALID(pobjProp);
		m_lngPropertyID = pobjProp->GetObjectID();
	}

	// Do search, starting at the root object so that reference folders, etc. get included in search.
	BObjects aResults;
	ULONG lngExcludeFlags = theApp.m_lngSearchExcludeFlags; 
	BObject* pobjStart = m_pDoc->m_pobjRoot;
	ASSERT_VALID(pobjStart);
	int nItems = m_pDoc->SearchForText(pobjStart, m_lngPropertyID, m_strFindText, aResults, lngExcludeFlags, m_bMatchCase, m_bWholeWord);

	// Set results label
//	CString strResults;
//	if (nItems == 1)
//		strResults.Format(_T("%d item found."), nItems);
//	else
//		strResults.Format(_T("%d items found."), nItems);
//	m_lblResults.SetWindowText(strResults);

	// Walk through collection and add items to listview
	m_lvw.DeleteAllItems(); // Clear listview first
	for (int i = 0; i < nItems; i++)
	{
		BObject* pobj = (BObject*) aResults.GetAt(i);
		ASSERT_VALID(pobj);
		int nImage = pobj->GetIconIndex();
		int nIndex = m_lvw.InsertItem(i, LPSTR_TEXTCALLBACK, nImage);
		// Make sure item was added correctly
		ASSERT (nIndex != -1);
		// Store object address in item data
		m_lvw.SetItemData(nIndex, (DWORD) pobj);
	}

	if (nItems > 0)
	{
		// Make sure text view is shown		
		// Select first item in listview
		// this should trigger notify event (OnNotifyItemChanged)
		m_lvw.SelectItem(0);
	}

	// Give message if no matches
	if (nItems == 0)
	{
		CString str;
		str.Format("No matches for '%s' found.", m_strFindText);
		AfxMessageBox(str, MB_ICONINFORMATION);
	}

	// Set focus to list
	m_lvw.SetFocus();

}


// f3 will advance to next match
// if no more, will advance to next item in found items list
// this way, can type in search text, hit enter, see the selection on the right,
// and hit f3 continually to find all occurrences of the text in the entire document

//, actually, would be nice if the focus were in the rtf control, so user could edit as necessary
// so the rtf has the focus, user hits f3 through the text, makes changes, etc.

// By default, a rich edit control shows and hides the selection highlight when it gains 
// and loses the focus. You can show or hide the selection highlight at any time by using 
// the HideSelection member function. For example, an application might provide a Search 
// dialog box to find text in a rich edit control. The application might select matching text without 
// closing the dialog box, in which case it must use HideSelection to highlight the selection.



// This handles the user moving through list with keyboard and different item gaining focus.
// Want to highlight the search text on the right in the appropriate view (rtf or properties).
void CViewSearch::OnNotifyItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	xTRACE("CViewSearch OnNotifyItemChanged\n");

	*pResult = 0;
	NMLISTVIEW* plv = (NMLISTVIEW*) pNMHDR;
	if (plv->uNewState & LVIS_FOCUSED)
	{
		// New item has gained focus
		BObject* pobj = (BObject*) m_lvw.GetItemData(plv->iItem);
		if (pobj)
		{
			ASSERT_VALID(pobj);

			// Select object in document so other views will load it
			m_pDoc->SetCurrentObject(pobj, this);

			// If we're supposed to look in rtf, then search in rtf view first
			// Note: if text found in Home object rtf view won't be available!
			if (m_lngPropertyID == propRtfText || m_lngPropertyID == 0)
			{
//				CView* pView = ShowView(viewText, FALSE); // show rtf view but don't activate it
//				CView* pView = ShowView(viewText, FALSE, TRUE); // show rtf view but don't activate it
				CView* pView = ShowView(viewText, FALSE, FALSE); // show rtf view but don't activate it
				CViewRtf* pViewRtf = DYNAMIC_DOWNCAST(CViewRtf, pView);
				if (pViewRtf)
				{
					ASSERT_VALID(pViewRtf);

					// Goto start of text
					pViewRtf->GotoStart();

					// Highlight next occurrence of text
					long nPos = pViewRtf->FindNext(m_strFindText, m_bMatchCase, m_bWholeWord);
					
					// Exit if found since we don't need to do anything else
					if (nPos != -1) return;

					// Exit if we were only supposed to look in rtf text and it wasn't found.
					if (m_lngPropertyID == propRtfText)
						return;
				}
			}

			// If we've reached this point, we need to look in the properties for the selected object.
//			CView* pView = ShowView(viewProperties, FALSE); // show properties view but don't activate it
//			CView* pView = ShowView(viewProperties, FALSE, TRUE); // show properties view but don't activate it
			CView* pView = ShowView(viewProperties, FALSE, FALSE); // show properties view but don't activate it
//			if (pView == 0)
//			{
//				if (IDYES == AfxMessageBox("The Properties View is not visible for this object - search results cannot be displayed without the Properties View. "
//										"Do you want to add the Properties View to objects of this class?", MB_ICONQUESTION + MB_YESNO))
//				{
//					BObject* pobjView = m_pDoc->GetObject(viewProperties);
//					m_pDoc->UpdateAllViewsEx(NULL, hintAddView, pobjView);
//					// try it again...
//					pView = ShowView(viewProperties, FALSE); // show properties view but don't activate it
//				}
//			}
			CViewProperties* pViewProps = DYNAMIC_DOWNCAST(CViewProperties, pView);
			if (pViewProps)
			{
				// Highlight next occurrence of text
				ULONG lngFoundPropID = pViewProps->FindNext(m_strFindText, 
																m_bMatchCase, m_bWholeWord, m_lngPropertyID);
			}
		}
	}
}



// Next Match Button/F3 handler
void CViewSearch::OnCmdEditFindNext() 
{
	// Search in rtf view first
	if (m_lngPropertyID == 0 || m_lngPropertyID == propRtfText)
	{
//		CView* pView = ShowView(viewText, TRUE);
//		CView* pView = ShowView(viewText, FALSE);
//		CView* pView = ShowView(viewText, FALSE, TRUE);
		CView* pView = ShowView(viewText, FALSE, FALSE);
		CViewRtf* pViewRtf = DYNAMIC_DOWNCAST(CViewRtf, pView);
		if (pViewRtf) // Home Folder does not include viewrtf so this will be zero!
		{
			ASSERT_VALID(pViewRtf);
			long nPos = pViewRtf->FindNext(m_strFindText, m_bMatchCase, m_bWholeWord);

			// If found text, just exit.
			if (nPos != -1) return;

			// If text was not found, and we're only supposed to search in rtf, then go to next object and exit.
			if (nPos == -1 && m_lngPropertyID == propRtfText)
			{
				SelectNextObject();
				return;
			}

		}
		// At this point, text was not found in rtf view, but we still need to look in the other properties...
	}

	// If we've reached this point, we need to look in the properties for the selected object.
//	CView* pView = ShowView(viewProperties, FALSE); // show properties view but don't activate it
//	CView* pView = ShowView(viewProperties, FALSE, TRUE); // show properties view but don't activate it
	CView* pView = ShowView(viewProperties, FALSE, FALSE); // show properties view but don't activate it
	CViewProperties* pViewProps = DYNAMIC_DOWNCAST(CViewProperties, pView);
	if (pViewProps)
	{
		// Highlight next occurrence of text
		ULONG lngFoundPropID = pViewProps->FindNext(m_strFindText, m_bMatchCase, m_bWholeWord, 
																		m_lngPropertyID);
		// If not found in properties, go on to the next object.
		if (lngFoundPropID == 0)
			SelectNextObject();
	}

	// Make sure we keep this window active so that F3 will stay here...

}



// User clicked on another item, so show it on the right side and highlight the found text
void CViewSearch::OnNotifyClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
}




// Goto next control
void CViewSearch::OnKeyTab() 
{
	HWND hWnd = ::GetFocus();
	int nID = ::GetDlgCtrlID(hWnd);
	int nNewID = m_to.GetNextID(nID);
	CWnd* pWnd = GetDescendantWindow(nNewID);
	if (pWnd)
		pWnd->SetFocus();
}


// Goto previous control
void CViewSearch::OnKeyShiftTab() 
{
	HWND hWnd = ::GetFocus();
	int nID = ::GetDlgCtrlID(hWnd);
	int nNewID = m_to.GetPrevID(nID);
	CWnd* pWnd = GetDescendantWindow(nNewID);
	if (pWnd)
		pWnd->SetFocus();
}



// Select the next object in the listview, restarting at beginning if at the end. 
void CViewSearch::SelectNextObject()
{
	// Advance to next item in results
	int nItem = m_lvw.GetSelectedItem() + 1;

	// If at end of list, start over at beginning
	if (nItem >= m_lvw.GetItemCount())
	{
		nItem = 0;
		AfxMessageBox("Reached end of search results - starting over at beginning.", MB_ICONINFORMATION);
	}

	// Select the new item
	// Note: Selecting a new item will trigger the OnNotifyItemChanged event.
	m_lvw.SelectItem(nItem);
}





void CViewSearch::OnObjEditInPlace()  // ok
{
	xTRACE("CViewSearch::OnObjEditInPlace\n");
//	int nItem = m_lvw.GetSelectedItem();
	BObject* pobjTarget = m_pDoc->GetTargetObject();
	int nItem = m_lvw.FindItemData((LPARAM) pobjTarget);
	if (nItem != -1)
	{
		m_lvw.SetFocus();
		m_lvw.EditLabel(nItem);
	}
}





// Simulate a keypress
BOOL CViewSearch::PressKey(UINT nVK)
{
	return m_lvw.PressKey(nVK);
}


// Set target on gaining focus
void CViewSearch::OnSetFocus(CWnd* pOldWnd) 
{
	xTRACE("CViewSearch::OnSetFocus - set target object\n");
	CViewEx::OnSetFocus(pOldWnd);
	
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();
	m_pDoc->SetTargetObject(pobj);
}



void CViewSearch::OnUpdateObjMoveDown(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE); // not valid in this view
}

void CViewSearch::OnUpdateObjMoveUp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE); // not valid in this view	
}




void CViewSearch::OnNavigateFollowLink() 
{
	OnObjOpen();
}



void CViewSearch::OnObjMoveTo() 
{
	m_pDoc->UIMoveObjectTo();
}

void CViewSearch::OnUpdateObjMoveTo(CCmdUI* pCmdUI) 
{
}
