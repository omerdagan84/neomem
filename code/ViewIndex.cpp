
// CViewIndex



#include "precompiled.h"

#include "NeoMem.h"
#include "ViewIndex.h"
#include "Constants.h"
#include "Hint.h"

#include "ConstantsDatabase.h"
#include "BObject.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




//----------------------------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CViewIndex, CViewEx)

BEGIN_MESSAGE_MAP(CViewIndex, CViewEx)
	//{{AFX_MSG_MAP(CViewIndex)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_OBJ_EDIT_IN_PLACE, OnObjEditInPlace) //ok
	ON_COMMAND(ID_OBJ_OPEN, OnObjOpen) //ok
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_OBJ_MOVE_DOWN, OnUpdateObjMoveDown)
	ON_UPDATE_COMMAND_UI(ID_OBJ_MOVE_UP, OnUpdateObjMoveUp)
	ON_COMMAND(ID_NAVIGATE_FOLLOW_LINK, OnNavigateFollowLink)
	ON_COMMAND(ID_OBJ_MOVE_TO, OnObjMoveTo)
	ON_UPDATE_COMMAND_UI(ID_OBJ_MOVE_TO, OnUpdateObjMoveTo)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LVW_INDEX, OnNotifyClick)
	ON_NOTIFY(NM_DBLCLK, IDC_LVW_INDEX, OnNotifyDblClick)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LVW_INDEX, OnNotifyItemChanged)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LVW_INDEX, OnEndLabelEdit)
	ON_CBN_SELCHANGE(IDC_CBO_CLASS, OnSelchangeClass)
//	ON_NOTIFY(CBN_SELCHANGE, IDC_CBO_CLASS, OnNotifyClassChanged)
END_MESSAGE_MAP()




// Construction/Destruction
//----------------------------------------------------------------------------------------------

CViewIndex::CViewIndex()
{
	m_bLoaded = FALSE;
//	m_nPopupItem = -1;
//	m_pobjPopup = 0;
	m_lngClassID = 0; // all classes, by default
}

CViewIndex::~CViewIndex()
{
}


int CViewIndex::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CViewEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create listview
	if (m_lvw.Create(WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, IDC_LVW_INDEX))
	{
		// Tell CViewEx about child window
		SetChildView(&m_lvw);

		m_lvw.m_bAutoColumns = TRUE;

		// Initialize listview
		m_lvw.m_pDoc = m_pDoc;
		m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
		m_lvw.SetFullRowSelect(TRUE);
//		m_lvw.SetFullRowSelect(FALSE);
		
		// Initialize columns
		//, Keep columns proportional
		m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsIndexView), TRUE, TRUE);
	}
	else return -1;


	// Create toolbar
	CRect r;
	if (m_tbr.Create(0, CRect(0,0,0,0), this, IDR_TOOLBAR_INDEX))
	{
		// Load toobar from resource file
		if (!m_tbr.LoadToolBar(MAKEINTRESOURCE(IDR_TOOLBAR_INDEX)))
			return -1;

		// Add combo
		// Note: The height of this rectangle determines the height of the dropdown of the combobox
		r.SetRect(0, 0, 100, 200);
		if (m_cboClass.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, r, &m_tbr, IDC_CBO_CLASS))
		{
			// Initialize comboboxex
			m_cboClass.SetFont(&(theApp.m_fontControls));
			m_cboClass.SetImageList(m_pDoc->GetImageList());

			//, make methods to do all this stuff..

			// Get classdef icon
			BObject* pobj = m_pDoc->GetObject(classClass);
			ASSERT_VALID(pobj);
			int nImage = pobj->GetIconIndex(); 

			// Add "All Classes"
			COMBOBOXEXITEM cbi;
			cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
			cbi.pszText = "All Classes";
			cbi.iItem = 0; 
			cbi.iImage = nImage;
			cbi.iSelectedImage = nImage;
			cbi.iIndent = 0;
			cbi.lParam = 0; // all classes
			int nIndex = m_cboClass.InsertItem(&cbi);
			ASSERT (nIndex != -1);

			// Now add all individual classes in alpha order
			BObject* pobjClasses = m_pDoc->GetObject(folderClasses);
			m_cboClass.AddObjects(pobjClasses, theApp.m_lngExcludeFlags, FALSE, TRUE);
//			m_cboClass.AddObjects(pobjClasses, 0, FALSE, TRUE);

			// Select first item
			m_cboClass.SetCurSel(0);
		}
		else return -1;


		// Add label to toolbar
//		CRect r;
//		m_tbr.GetClientRect(r);
//		if (!m_lbl.Create("Click on column heading to sort", WS_CHILD | WS_VISIBLE, r, &m_tbr))
//			return -1;
//		m_lbl.SetFont(&theApp.m_fontControls);
		
		// Must call this to initialize the toolbar position correctly
		m_tbr.Position();

		// Calculate position of all controls on toolbar
		RecalculateLayout();
		
		// Tell the view that there is a toolbar attached to it
		SetToolBar(&m_tbr);	
	}


	// Load custom accelerators for this view
	LoadViewAccelerators(IDR_VIEW_INDEX);

	return 0;
}




// CViewIndex diagnostics
//----------------------------------------------------------------------------------------------

#ifdef _DEBUG
void CViewIndex::AssertValid() const
{
	CViewEx::AssertValid();
}

void CViewIndex::Dump(CDumpContext& dc) const
{
	CViewEx::Dump(dc);
}
#endif //_DEBUG




// Message Handlers
//----------------------------------------------------------------------------------------------


void CViewIndex::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{	
	TRACE("    CViewIndex::OnUpdate %s\n", theApp.GetHintName(lHint));

	switch (lHint)
	{

	case hintLoad:
		{
			// Since hintLoad gets sent on initial view creation AND on selection of new object,
			// need flag to check if the view has already been loaded.
			if (m_bLoaded == FALSE)
			{
				ReloadItems();
			}

			break;
		}

	case hintSelect:
		{
			BObject* pobj = (BObject*) pHint;
			ASSERT_VALID(pobj);
			int nItem = m_lvw.SelectItemData((LPARAM) pobj);
//			m_lvw.EnsureVisible(nItem, TRUE);
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
			int nItem = m_lvw.FindItemData((LPARAM) pobj);
			if (nItem != -1)
			{
				// Update icon if that's what's changing
				if (lngPropertyID == propIconID)
				{
					int nImage = pobj->GetIconIndex();
					m_lvw.SetItem(nItem, 0, LVIF_IMAGE, 0, nImage, 0, 0, 0);
				}
				// update any other properties
				m_lvw.Update(nItem);
			}
			// that update was inadequate - eg change class name and that didn't get 
			// updated in the list...
			m_lvw.RedrawWindow();

			// also update class combo
			ULONG lngClassID = pobj->GetClassID();
			if (lngClassID == classClass)
			{
				int nItem = m_cboClass.FindItemData((LPARAM) pobj);
				if (nItem != -1)
				{
					// Update icon if that's what's changing
					if (lngPropertyID == propIconID)
					{
						int nImage = pobj->GetIconIndex();
						m_cboClass.SetItemIcon(nItem, nImage);// .SetItem(nItem, 0, LVIF_IMAGE, 0, nImage, 0, 0, 0);
					}
					// other props (ie name) are obtained on the fly..
					m_cboClass.RedrawWindow();
				}
			}
			break;
		}

	case hintRefresh:
		{
			// Redisplay all objects
			m_lvw.RedrawWindow();
			break;
		}

	case hintDelete:
		{
			CHint* ph = (CHint*) pHint;
			ASSERT_VALID(ph);
			ASSERT_VALID(ph->m_paObjects);
			int nObjects = ph->m_paObjects->GetSize();
			for (int i = 0; i < nObjects; i++)
			{
				BObject* pobj = (BObject*) ph->m_paObjects->GetAt(i);
				// Note: Object has already been deleted, so don't validate it!
//				ASSERT_VALID(pobj);
				// delete from list
				m_lvw.DeleteItemData((LPARAM) pobj);
				// and from class combo
				m_cboClass.DeleteItemData((LPARAM) pobj);
			}
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

	case hintAdd:
		{
			// Add the specified object if not already in list
			BObject* pobj = (BObject*) pHint;
			ASSERT_VALID(pobj);
			int nItem = m_lvw.FindItemData((LPARAM) pobj);
			if (nItem == -1)
			{
				if (!(pobj->GetFlag(theApp.m_lngExcludeFlags)))
				{
					// Get index of icon associated with this object
					int nImage = pobj->GetIconIndex();
					
					// Add this BObject to the end of the list and store pointer to object in item data
					int nItems = m_lvw.GetItemCount();
					int nIndex = m_lvw.InsertItem(nItems, LPSTR_TEXTCALLBACK, nImage);
					ASSERT (nIndex != -1);
					m_lvw.SetItemData(nIndex, (DWORD) pobj);
				}
			}
			// also update class combo
			ULONG lngClassID = pobj->GetClassID();
			if (lngClassID == classClass)
			{
				int nItem = m_cboClass.FindItemData((LPARAM) pobj);
				if (nItem == -1) // make sure it's not already there (not likely, but..)
				{
					m_cboClass.AddObject(pobj);
//					m_cboClass.RedrawWindow();
				}
			}

			break;
		}
	}

}


// Add the child objects of the specified bobject to the listview
void CViewIndex::AddChildrenToList(BObject* pobjStart)
{
	// Walk through child objects and add them to the tree
	BObjects* paChildren = pobjStart->m_paChildren;
	if (paChildren)
	{
		ASSERT_VALID(paChildren);
		int nItems = paChildren->GetSize();
		for (int i = 0; i < nItems; i++)
		{
			BObject* pobj = (BObject*) paChildren->GetAt(i);
			ASSERT_VALID(pobj);

			// Only add this object if it's not a system object
//			if (!(pobj->GetFlag(flagSystem)))
			if (!(pobj->GetFlag(theApp.m_lngSearchExcludeFlags)))
			{
				// filter by class
				if ((m_lngClassID==0) || (pobj->GetClassID() == m_lngClassID))
				{
					// Get index of icon associated with this object
					int nImage = pobj->GetIconIndex();
					
					// Add this BObject to the list and store pointer to object in item data
					int nIndex = m_lvw.InsertItem(0, LPSTR_TEXTCALLBACK, nImage);
					ASSERT (nIndex != -1);
					m_lvw.SetItemData(nIndex, (DWORD) pobj);
				}
			}

			// If this object has children, then call this routine recursively
			if (pobj->m_paChildren)
			{
				// Call this routine recursively
				AddChildrenToList(pobj);
			}
		}
	}
}





void CViewIndex::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	xTRACE("CViewIndex::OnActivateView(bActivate=%d)\n", bActivate);
	CViewEx::OnActivateView(bActivate, pActivateView, pDeactiveView);

	// If this view is being activated, set the focus to the child control
	// Note: CListCtrlEx's focus handler sets focus to first item if none selected
	if (bActivate) 
	{
		RecalculateLayout();
		m_lvw.SetFocus();
	}
}



// User clicked on another item, so select it in the document
//, this isn't getting called
void CViewIndex::OnNotifyClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();
	if (pobj)
	{
		ASSERT_VALID(pobj);
		m_pDoc->SetCurrentObject(pobj, this);
	}
	*pResult = 0;
}




// This handles the user moving through list with keyboard and different item gaining focus
void CViewIndex::OnNotifyItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* plv = (NMLISTVIEW*) pNMHDR;
	if (plv->uNewState & LVIS_FOCUSED)
	{
		// New item has gained focus
		BObject* pobj = (BObject*) m_lvw.GetItemData(plv->iItem);
		if (pobj)
		{
			ASSERT_VALID(pobj);
			m_pDoc->SetCurrentObject(pobj, this);
		}
	}
	*pResult = 0;
}





/*
void CViewIndex::OnObjEditInDialog() 
{
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();
	if (pobj)
	{
		ASSERT_VALID(pobj);
		pobj->EditValue(propName);
	}		
}
*/



void CViewIndex::OnContextMenu(CWnd* pWnd, CPoint ptScreen) 
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
//		if (menu.LoadMenu(IDR_POPUP_INDEX))
//		{
//			CMenu* pPopup = menu.GetSubMenu(0);
			BCMenu* pPopup = InitPopup(IDR_POPUP_INDEX, ID_OBJ_OPEN);
			if (pPopup)
			{
//				pPopup->SetDefaultItem(ID_OBJ_OPEN);
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
			}
//		}
	}	
}




//void CViewIndex::OnCmdEditDelete() 
//{
//}

/*
void CViewIndex::OnCmdEditProperties() 
{
	BObject* pobjCurrent = m_pDoc->GetCurrentObject();
	if (pobjCurrent)
	{
		ASSERT_VALID(pobjCurrent);
		pobjCurrent->DisplayProperties();
	}
}
*/




void CViewIndex::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	xTRACE("CViewIndex::OnEndLabelEdit\n");
	//, do data validation, return FALSE if you don't like it
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

		// This set doc modified and tells views
		pobj->SetPropertyText(propName, pLVITEM->pszText);

		*pResult = TRUE;
	}
	else
	{
		*pResult = 0;
	}
}



void CViewIndex::OnObjEditInPlace() 
{
	xTRACE("CViewIndex::OnObjEditInPlace\n");
	int nItem = m_lvw.GetSelectedItem();
	if (nItem != -1)
	{
		m_lvw.EditSubItem(nItem, 0);
	}

//	ASSERT(m_htiPopup);

	// Note: This mimics Explorer behavior - you can rename an item without changing the current selection.
	// The flag tells OnSelChanged to ignore the new selection, and code in OnEndLabelEdit will set
	// the selection back to the original selection.
//	m_bTempSelection = TRUE;
//	HTREEITEM htiSave = m_tvw.GetSelectedItem();
//	m_htiSaveDuringRename = m_tvw.GetSelectedItem();
//	if (m_tvw.SelectItem(m_htiPopup))
//	{
//		m_bTempSelection = TRUE;
//		m_tvw.EditLabel(m_htiPopup);
//	}
//	m_tvw.SelectItem(htiSave);
//	m_bTempSelection = FALSE;

	// See OnEndLabelEdit for reselection code
}

/*
void CViewIndex::OnPopupEditInDialog() 
{
	ASSERT(m_nPopupItem);

	// Edit value in dialog
	BObject* pobj = (BObject*) m_lvw.GetItemData(m_nPopupItem);
	ASSERT_VALID (pobj);
	m_pDoc->UIRenameObject(pobj);
}
*/





void CViewIndex::OnObjOpen() 
{
	if (m_lvw.GetSelectedCount() == 0)
		return;

	// Switch to data view
	ShowView(viewData, TRUE, FALSE);
}


/*
void CViewIndex::OnObjProperties() 
{
	if (m_pobjPopup)
	{
		ASSERT_VALID(m_pobjPopup);
		m_pobjPopup->DisplayProperties();
	}
}
*/


void CViewIndex::OnNotifyDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Switch to data view
	OnObjOpen();
}



// Simulate a keypress
BOOL CViewIndex::PressKey(UINT nVK)
{
	// maybe listview has some code that checks if it has the focus or something, cause this don't work!
//	m_lvw.SendMessage(WM_KEYDOWN, (WPARAM) nVK, 0x01480001);
//	m_lvw.SendMessage(WM_KEYUP, (WPARAM) nVK, 0xc1480001);
	return m_lvw.PressKey(nVK);
}



// Set target on gaining focus
void CViewIndex::OnSetFocus(CWnd* pOldWnd) 
{
	xTRACE("CViewIndex::OnSetFocus - set target object\n");
	CViewEx::OnSetFocus(pOldWnd);
	
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();
	m_pDoc->SetTargetObject(pobj);
}


void CViewIndex::OnUpdateObjMoveDown(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE); // not valid in this view
}

void CViewIndex::OnUpdateObjMoveUp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE); // not valid in this view
}



void CViewIndex::OnNavigateFollowLink() 
{
	OnObjOpen();
}

void CViewIndex::OnObjMoveTo() 
{
	m_pDoc->UIMoveObjectTo();
}

void CViewIndex::OnUpdateObjMoveTo(CCmdUI* pCmdUI) 
{
}



// Arrange toolbar buttons and controls
void CViewIndex::RecalculateLayout(int cx /* = 0 */, int cy /* = 0 */)
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

	// Position combo
//	tbi.cx = nToolbarWidth - m_nClassLabelWidth;
	tbi.cx = nToolbarWidth;
	m_tbr.SetButtonInfo(IDC_CBO_CLASS, &tbi);
//	m_tbr.GetItemRect(1, r);
	m_tbr.GetItemRectFromID(IDC_CBO_CLASS, r);
	r.bottom -= 2; //.
	m_cboClass.MoveWindow(r);

	// Now resize the toolbar so that it will position buttons on the two rows correctly
	r.SetRect(0, 0, nToolbarWidth, cy);
//	m_tbr.SetRows(2, TRUE, r);
	m_tbr.SetRows(1, TRUE, r);

}


void CViewIndex::OnSize(UINT nType, int cx, int cy) 
{
	// Resize toolbar buttons and controls
//	if ((cx != 0) && (cy != 0))
		RecalculateLayout(cx, cy);

	// Now we can call the base class, which needs the height of the toolbar 
	CViewEx::OnSize(nType, cx, cy);
}



BOOL CViewIndex::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CViewEx::OnNotify(wParam, lParam, pResult);
}



// User selected a new location - change the class accordingly (based on parent's default class).
void CViewIndex::OnSelchangeClass() 
{
/*	BObject* pobjParent = (BObject*) m_cboLocation.GetSelectedItemData();
	if (pobjParent)
	{
		BObject* pobjClass = pobjParent->GetPropertyLink(propDefaultClass);
		ASSERT_VALID(pobjClass);
		// This will trigger listview selchange event
		m_lvw.SelectItemData((LPARAM) pobjClass);
	}
*/
//	AfxMessageBox("bugh");

	// get currently selected class (0 for all)
	BObject* pobjClass = (BObject*) m_cboClass.GetSelectedItemData();
	if (pobjClass)
	{
		ASSERT_VALID(pobjClass);
		m_lngClassID = pobjClass->GetObjectID();
	}
	else
		m_lngClassID = 0; // all classes
	ReloadItems();
}




void CViewIndex::ReloadItems()
{
	// Clear the listview first
	m_lvw.DeleteAllItems();

	// Add root item
	BObject* pobjRoot = m_pDoc->GetRootMain();
	ASSERT_VALID(pobjRoot);
//	if (!(pobjRoot->GetFlag(flagSystem)))
	if (!(pobjRoot->GetFlag(theApp.m_lngSearchExcludeFlags)))
	{
		// filter by class
		if ((m_lngClassID==0) || (pobjRoot->GetClassID() == m_lngClassID))
		{
			int nImage = pobjRoot->GetIconIndex();
			int nIndex = m_lvw.InsertItem(0, LPSTR_TEXTCALLBACK, nImage);
			ASSERT (nIndex != -1);
			m_lvw.SetItemData(nIndex, (DWORD) pobjRoot);
		}
	}

	// Walk through all objects recursively and add them
	AddChildrenToList(pobjRoot);

	//, use above code until listviewex handles textcallback!!
//		m_lvw.AddObjects(pobjRoot, theApp.m_lngExcludeFlags, TRUE, TRUE);

	// Sort items
//	m_lvw.SortByProperty(propName);
	m_lvw.SortByProperty(propName, 1);

	// Set flag
	m_bLoaded = TRUE;

	// Now select the current object
	BObject* pobjCurrent = m_pDoc->GetCurrentObject();
	int nItem = m_lvw.SelectItemData((LPARAM) pobjCurrent);
//	m_lvw.EnsureVisible(nItem, TRUE);

}



// filter contents of listview based on current class selection
// ie mark items as invisible or visible. 
// apparently not possible. sorry. 
void CViewIndex::FilterItems()
{
//	m_lvw.SetItemState();...
}
