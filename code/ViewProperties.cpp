
// CViewProperties


#include "precompiled.h"

#include "NeoMem.h"
#include "ViewProperties.h"
#include "Constants.h"
#include "DialogSelectProperty.h"
#include "Hint.h"

#include "BObject.h"
#include "ConstantsDatabase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CViewProperties, CViewEx)

BEGIN_MESSAGE_MAP(CViewProperties, CViewEx)
	//{{AFX_MSG_MAP(CViewProperties)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_OBJ_EDIT_IN_PLACE, OnObjEditInPlace) //ok
	ON_COMMAND(ID_OBJ_EDIT_IN_DIALOG, OnObjEditInDialog) //ok
//	ON_COMMAND(ID_EDIT_DELETE, OnCmdEditDelete)
	ON_COMMAND(ID_EDIT_COPY_GRID, OnCmdEditCopyGrid)
	ON_COMMAND(ID_EDIT_PASTE_PLAIN, OnCmdEditPastePlain)
	ON_COMMAND(ID_EDIT_PASTE_FORMATTED, OnCmdEditPasteFormatted)
	ON_COMMAND(ID_EDIT_CUT, OnCmdEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnCmdEditCopy)
	ON_COMMAND(ID_EDIT_UNDO, OnCmdEditUndo)
	ON_COMMAND(ID_EDIT_CLEAR, OnCmdEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_PLAIN, OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_FORMATTED, OnUpdateEdit)
	ON_COMMAND(ID_ADD_PROPERTY, OnCmdAddProperty)
	ON_COMMAND(ID_DELETE_PROPERTY, OnCmdDeleteProperty)
	//}}AFX_MSG_MAP
//	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LVW_PROPERTIES, OnBeginLabelEdit)
//	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LVW_PROPERTIES, OnColumnClick)
//	ON_NOTIFY(NM_CLICK, IDC_LVW_PROPERTIES, OnClick)
//	ON_NOTIFY(LVN_DELETEITEM, IDC_LVW_PROPERTIES, OnDeleteItem)
//	ON_NOTIFY(HDN_ITEMCLICK, IDC_LVW_PROPERTIES, OnHeaderItemClick)
	ON_NOTIFY(NM_DBLCLK, IDC_LVW_PROPERTIES, OnDblClk)
	ON_NOTIFY(NM_RCLICK, IDC_LVW_PROPERTIES, OnNotifyRClick)
//	ON_NOTIFY(LVN_GETDISPINFO, IDC_LVW_PROPERTIES, OnGetDispInfo)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LVW_PROPERTIES, OnEndLabelEdit)
	ON_COMMAND_RANGE(ID_POPUP_BDATA_START, ID_POPUP_BDATA_STOP, OnPopupBDataCommand)
END_MESSAGE_MAP()




// Construction/Destruction
//----------------------------------------------------------------------------------------------

CViewProperties::CViewProperties()
{
	m_pobjCurrent = NULL;
}

CViewProperties::~CViewProperties()
{
}


int CViewProperties::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CViewEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create listview
	if (m_lvw.Create(WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, IDC_LVW_PROPERTIES))
	{
		// Tell CViewEx about the child view
		SetChildView(&m_lvw);

		// Initialize listview
		m_lvw.m_pDoc = m_pDoc;
		m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
		m_lvw.m_nMode = CListCtrlEx::modeSelectCell;
		m_lvw.m_bDisableFirstColumn = TRUE;
		m_lvw.m_bExpandLastColumn = TRUE; // fill width with columns
		m_lvw.ModifyStyle(LVS_EDITLABELS, 0); // don't allow editing of first column labels (property name)
		m_lvw.SetExtendedStyle(m_lvw.GetExtendedStyle() | LVS_EX_GRIDLINES); // turn on gridlines

		// Initialize columns
//		BDataColumns& rdat = m_pDoc->m_datPropertyViewColumns;
//		if (rdat.GetColumnCount() == 0)
//		{
//			rdat.InsertColumn(propName, m_pDoc, 140);
//			rdat.InsertColumn(propValue, m_pDoc, 240); // pseudo property
//		}
//		m_lvw.InitializeColumns(&rdat, TRUE);
//		m_lvw.InitializeColumns(m_pDoc->m_pdatColumnsPropertyView);
		m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsPropertyView), TRUE, TRUE);
	}
	else return -1;


	// Create toolbar
	if (m_tbr.Create(WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, IDR_TOOLBAR_PROPERTIES))
	{
		// Must call this to initialize the toolbar position correctly
		//. or call from SetToolBar!
		m_tbr.Position();

		// Load toolbar bitmap and buttons from resource file
		m_tbr.LoadToolBar(MAKEINTRESOURCE(IDR_TOOLBAR_PROPERTIES));

		// Tell CViewEx about the toolbar
		SetToolBar(&m_tbr);
	}
	else return -1;

	// Load custom accelerators for this view
	LoadViewAccelerators(IDR_VIEW_PROPERTIES);

	return 0;
}




// Diagnostics
//----------------------------------------------------------------------------------------------

#ifdef _DEBUG
void CViewProperties::AssertValid() const
{
	CViewEx::AssertValid();
}

void CViewProperties::Dump(CDumpContext& dc) const
{
	CViewEx::Dump(dc);
}
#endif //_DEBUG




// Message Handlers
//----------------------------------------------------------------------------------------------


void CViewProperties::OnInitialUpdate() 
{
	// swallow this so doesn't get invalidate
//	CViewEx::OnInitialUpdate();	
}


void CViewProperties::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	TRACE("    CViewProperties::OnUpdate %s\n", theApp.GetHintName(lHint));
	switch (lHint)
	{

	case hintLoad:
		{
			BObject* pobj = (BObject*) pHint;
			ASSERT_VALID(pobj);

//			this->LockWindowUpdate();
//			m_lvw.SetRedraw(FALSE);

			// Clear the listview first
			m_lvw.DeleteAllItems();

			// Get the property defs associated with the object
			CObArray aPropertyDefs;
			int nPropertyDefs = pobj->GetPropertyDefs(aPropertyDefs, FALSE, FALSE);

			// Get propertydef class and get icon from that
			BObject* pobjClassPropertyDef = m_pDoc->GetObject(classProperty);
			ASSERT_VALID(pobjClassPropertyDef);
			int nImage = pobjClassPropertyDef->GetIconIndex();

			// Walk through property defs and add them to the list
			for (int i = 0; i < nPropertyDefs; i++)
			{
				BObject* pobjPropertyDef = (BObject*) aPropertyDefs.GetAt(i);
				ASSERT_VALID(pobjPropertyDef);

				// Don't add property def if it should be excluded based on admin settings
				if (!(pobjPropertyDef->GetFlag(theApp.m_lngExcludeFlags)))
				{
					// Add the propertydef to the list
					int nIndex = m_lvw.InsertItem(i, LPSTR_TEXTCALLBACK, nImage);
					// Make sure item was added correctly
					ASSERT (nIndex != -1);
					// Store property def object address in item data
					m_lvw.SetItemData(nIndex, (DWORD) pobjPropertyDef);
				}
			}

			// Remember the current object so can access through getdispinfo etc
			m_pobjCurrent = pobj;

//			m_lvw.SetRedraw(TRUE);
//			this->UnlockWindowUpdate();

			break;
		}

	case hintPropertyChange:
		{
			CHint* ph = (CHint*) pHint;
			
			// Exit if this hint refers to a different object
			// actually, might affect some props anyway, eg. links to property name, class inherited props, etc.
			// prop name, etc., so just update all items
//			if (m_pobjCurrent != ph->m_pobjObject)
//				break;
			// Find property in listview
//			BObject* pobjProp = m_pDoc->GetObject(ph->m_lngPropertyID);
//			int nItem = m_lvw.FindItemData((LPARAM) pobjProp);
//			if (nItem != -1)
//				m_lvw.Update(nItem);

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
				// If the object is a property def, this will remove it from the listview.
				m_lvw.DeleteItemData((LPARAM) pobj);
			}

			// Redisplay all property values, in case one referenced a deleted object, etc.
			m_lvw.RedrawWindow();
			break;
		}

	case hintRefresh:
		{
			// Redisplay all objects
			m_lvw.RedrawWindow();
			break;
		}

	}
}






void CViewProperties::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	xTRACE("CViewProperties OnActivateView %d\n", bActivate);
	CViewEx::OnActivateView(bActivate, pActivateView, pDeactiveView);

	// If this view is being activated, set the focus to the child control
	// Note: CListCtrlEx's focus handler sets focus to first item if none selected
	if (bActivate) m_lvw.SetFocus();
}







// This is the F2 handler
// Edit the currently selected cell
// CListCtrlEx handles selecting a cell, moving around the grid, etc.
// This view must handle F2 though because it's a command, and won't get routed to the control.
void CViewProperties::OnObjEditInPlace() 
{
	// This will create edit box on current cell and send OnEndLabelEdit message when done
	m_lvw.EditCurrentCell();
}



// Use this to handle when user finishes editing the cell
//, do data validation, return FALSE if you don't like it
void CViewProperties::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Update CDocument and other views
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LVITEM* pLVITEM = &(pDispInfo->item);
	if (pLVITEM->pszText)
	{
		BObject* pobjProperty = (BObject*) m_lvw.GetItemData(pLVITEM->iItem);
		ASSERT_VALID(pobjProperty);
		ULONG lngPropertyID = pobjProperty->GetObjectID();

		// Change property
		m_pobjCurrent->SetPropertyText(lngPropertyID, pLVITEM->pszText);

		*pResult = TRUE;
	}
	else
	{
		*pResult = 0;
	}
}





// This is the F4 handler
// Edit the property value with a dialog
void CViewProperties::OnObjEditInDialog() 
{
	// Exit if on first column or no cell selected
	if (m_lvw.m_nCol == 0) return;
	if ((m_lvw.m_nRow < 0) || (m_lvw.m_nCol < 0)) return;

	// Get propertydef for the selected row
	BObject* pobjPropertyDef = (BObject*) m_lvw.GetItemData(m_lvw.m_nRow);
	if (pobjPropertyDef)
	{
		ASSERT_VALID(pobjPropertyDef);

		// Edit the property value in an appropriate dialog
		ULONG lngPropertyID = pobjPropertyDef->GetObjectID();
		m_pobjCurrent->EditValue(lngPropertyID);
	}
}


// Double click is same as F4
void CViewProperties::OnDblClk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnObjEditInDialog();
	*pResult = 0;
}



// Bring up popup menu
void CViewProperties::OnNotifyRClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	NMHEADER* phdr = (NMHEADER*) pNMHDR;

/*
	CPoint ptScreen(::GetMessagePos()); // Get position of message source
	
	// Get row that was right clicked on and save to module level variable
//	CPoint ptClient = ptScreen;
//	m_lvw.ScreenToClient(&ptClient);
//	HDHITTESTINFO hti;
//	hti.pt = ptClient;
//	m_nColumnRClick = m_lvw.GetHeaderCtrl()->SendMessage(HDM_HITTEST, 0, (LPARAM) (LPHDHITTESTINFO) &hti);
//	int nCol;
//	m_nRowRClick = m_lvw.HitTestEx(ptClient, &nCol);

	// Show popup menu
	CMenu menu;
	if (menu.LoadMenu(IDR_POPUP_EDIT_VALUE))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		if (pPopup)
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
	}
*/
	*pResult = 0;
}



void CViewProperties::OnContextMenu(CWnd* pWnd, CPoint ptScreen) 
{
//	CPoint ptScreen(::GetMessagePos()); // Get position of message source
	
	// Get row that was right clicked on and save to module level variable
	//, not necessary, since rclick will select the row first
//	CPoint ptClient = ptScreen;
//	m_lvw.ScreenToClient(&ptClient);
//	HDHITTESTINFO hti;
//	hti.pt = ptClient;
//	m_nColumnRClick = m_lvw.GetHeaderCtrl()->SendMessage(HDM_HITTEST, 0, (LPARAM) (LPHDHITTESTINFO) &hti);
//	int nCol;
//	m_nRowRClick = m_lvw.HitTestEx(ptClient, &nCol);

	// Set flag if user pressed context key (x and y will be -1)
	BOOL bContextKey = ((ptScreen.x == -1) && (ptScreen.y == -1));
	if (bContextKey)
	{
		CRect r;
		m_lvw.GetCellRect(m_lvw.m_nRow, m_lvw.m_nCol, r);
		m_lvw.ClientToScreen(&r);
		ptScreen.x = r.left + 6;
		ptScreen.y = r.top + 6;
	}

	// Show popup menu
//	CMenu menu;
//	if (menu.LoadMenu(IDR_POPUP_EDIT_VALUE))
//	{
//		CMenu* pPopup = menu.GetSubMenu(0);
		BCMenu* pPopup = InitPopup(IDR_POPUP_EDIT_VALUE);
		if (pPopup)
		{
			// Add bdata menu items, if any
			BData* pdat = m_lvw.GetCellBData();
			if (pdat)
			{
				ASSERT_VALID(pdat);
				int nPos = pPopup->GetMenuItemCount() - 1; // insert before "Cancel"
				pdat->AddMenuItems(pPopup, nPos);
			}
			// Enable/disable undo menu
//			pPopup->EnableMenuItem(ID_EDIT_UNDO, MF_BYCOMMAND | (m_objUndo.IsValid() ? MF_ENABLED : MF_GRAYED));
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
		}
//	}
	
//	*pResult = 0;

	// Call base class
//	CViewEx::OnContextMenu(pWnd, point);
}



/*
// Delete the selected property value
void CViewProperties::OnCmdEditDelete() 
{
	ULONG lngPropertyID = GetSelectedPropertyID();
	if (lngPropertyID)
	{
		ASSERT_VALID(m_pobjCurrent);
		ASSERT_VALID(m_pDoc);
//		m_objUndo.SaveDeleteProperty(m_pobjCurrent, lngPropertyID); // save bdata for undo
		// Try to delete property value - if failed, clear the undo object we just created
//		if (!m_pobjCurrent->DeleteProperty(lngPropertyID)) // this will update views and set doc modified flag
//			m_objUndo.Clear();
		m_pobjCurrent->DeleteProperty(lngPropertyID); // this will update views and set doc modified flag
	}
}
*/


// Get the PropertyID associated with the selected row, or 0 if none
ULONG CViewProperties::GetSelectedPropertyID()
{
	if (m_lvw.m_nRow < 0)
		return 0;

	// Get propertydef for the selected row
	BObject* pobjPropertyDef = (BObject*) m_lvw.GetItemData(m_lvw.m_nRow);
	if (pobjPropertyDef)
	{
		ASSERT_VALID(pobjPropertyDef);
		// Return the propertyid
		ULONG lngPropertyID = pobjPropertyDef->GetObjectID();
		return lngPropertyID;
	}
	return 0;
}



// Copy the grid to the clipboard
void CViewProperties::OnCmdEditCopyGrid() 
{
	m_lvw.CopyToClipboard(TRUE);	
}



// Select the specified property value.
// This is used by the search tab to highlight a value.
void CViewProperties::SelectPropertyValue(ULONG lngPropertyID)
{
	// Find property in listview
	BObject* pobjProp = m_pDoc->GetObject(lngPropertyID);
	int nItem = m_lvw.FindItemData((LPARAM) pobjProp);
	if (nItem != -1)
	{
		// Select the cell
		m_lvw.SelectCell(nItem, 1);
	}
}


// Find the next occurrence of the specified text in this view and highlight the
// property value containing it.
// If property id is not zero will search only in that property.
// Returns the property that was highlighted, or zero if text was not found and no property was highlighted.
ULONG CViewProperties::FindNext(CString &strFindText, BOOL bMatchCase, BOOL bWholeWord, 
														ULONG lngPropertyID /* = 0 */)
{
	ULONG lngHighlightPropID = 0;

	// If propertyid is zero, we want to search through all property values in this view
	if (lngPropertyID == 0)
	{
		// Start at row after current highlighted row, or 0 if none.
		// Note: if already on last row, will wind up returning 0 because won't go through loop.
		int nStartRow = m_lvw.m_nRow + 1; 
		int nRows = m_lvw.GetItemCount();
		// Walk through rows, searching for text
		for (int i = nStartRow; i < nRows; i++)
		{
			// Search through property value for specified text
//			CString strPropertyValue = m_lvw.GetItemText(i, 1);
			//. this bombed sometimes with access violation - i think it's been fixed though. 
			CString strPropertyValue = m_lvw.GetItemTextOP(i, 1);
			if (!bMatchCase)
				strPropertyValue.MakeLower(); //, lame!
			int nPos = strPropertyValue.Find(strFindText, 0);
			if (nPos != -1)
			{
				// Found text in this property, so highlight this value and quit
				m_lvw.SelectCell(i, 1);
				BObject* pobjPropDef = (BObject*) m_lvw.GetSelectedItemData();
				ASSERT_VALID(pobjPropDef);
				lngHighlightPropID = pobjPropDef->GetObjectID();
				return lngHighlightPropID;
			}
		}

	}
	else
	{
		// Since we're just searching for one specific property, highlight it and quit.
		// If we've already highlighted this property for this object though, we need to return zero
		// so search code will be able to go on to next object in results!
		SelectPropertyValue(lngPropertyID);
//		lngHighlightPropID = lngPropertyID;
		lngHighlightPropID = 0;
	}

	return lngHighlightPropID;
}





void CViewProperties::OnUpdateEdit(CCmdUI* pCmdUI) 
{
	m_lvw.UpdateEdit(pCmdUI);	
}

void CViewProperties::OnCmdEditPasteFormatted() 
{
	m_lvw.PasteClipboardToCurrentCell();
}

void CViewProperties::OnCmdEditCut() 
{
	m_lvw.CutCurrentCellToClipboard();
}

void CViewProperties::OnCmdEditCopy() 
{
	m_lvw.CopyCurrentCellToClipboard();
}

void CViewProperties::OnCmdEditPastePlain() 
{
	m_lvw.PasteClipboardToCurrentCell();
}



// Pass the specified BDataCommand on to the right clicked BData value
void CViewProperties::OnPopupBDataCommand(UINT nBDataCommandID)
{
	BData* pdat = m_lvw.GetCellBData();
	if (pdat)
	{
		ASSERT_VALID(pdat);
		pdat->HandleCommand(nBDataCommandID);
	}	
}


void CViewProperties::OnCmdEditUndo() 
{
//	if (m_objUndo.IsValid())
//	{
//		m_objUndo.Restore();
//	}
}

void CViewProperties::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(m_objUndo.IsValid());
	pCmdUI->Enable(FALSE);
}


// Delete the currently selected property value
void CViewProperties::OnCmdEditClear() 
{
	ULONG lngPropertyID = GetSelectedPropertyID();
	if (lngPropertyID)
	{
		ASSERT_VALID(m_pobjCurrent);
		ASSERT_VALID(m_pDoc);
//		m_objUndo.SaveDeleteProperty(m_pobjCurrent, lngPropertyID); // save bdata for undo
		// Try to delete property value - if failed, clear the undo object we just created
//		if (!m_pobjCurrent->DeleteProperty(lngPropertyID)) // this will update views and set doc modified flag
//			m_objUndo.Clear();
//		m_pobjCurrent->DeleteProperty(lngPropertyID); // this will update views and set doc modified flag
		// this will ask user, update views and set doc modified flag
		m_pobjCurrent->DeleteProperty(lngPropertyID, TRUE, TRUE, TRUE); 
	}
}



// Let user add a new property to this object and/or the classdef
// Add a new property to the current bobject
// to do this we need to add it to the object's classdef
// ask the user before doing this
// then redisplay all properties and let them fill in the value

// so we need to disable all properties that are already visible
// then bring up the add property dialog
// then add the selected property to the classdef
// then redisplay this view (sent hintLoad to this view only?)
void CViewProperties::OnCmdAddProperty() 
{
	ASSERT_VALID(m_pDoc);
	ASSERT_VALID(m_pobjCurrent);

	// Get Properties folder object
	BObject* pobjProperties = m_pDoc->GetObject(folderProperties);
	ASSERT_VALID(pobjProperties);

	// Enable all properties
	pobjProperties->SetFlag(flagDisabled, FALSE, TRUE);

	// Get the properties associated with the current object
	CObArray aProperties;
	int nProperties = m_pobjCurrent->GetPropertyDefs(aProperties, FALSE, FALSE);

	// Disable the properties
	for (int i = 0; i < nProperties; i++)
	{
		BObject* pobjProp = (BObject*) aProperties.GetAt(i);
		ASSERT_VALID(pobjProp);
		pobjProp->SetFlag(flagDisabled, TRUE, FALSE);
	}

	BObject* pobjClass = m_pobjCurrent->GetClassObject();
	ASSERT_VALID(pobjClass);

	// Bring up Add Property dialog to let user select a property to add
	CDialogSelectProperty dlg;
	dlg.m_pobjDefaultClass = pobjClass;
	if (dlg.DoModal() == IDOK)
	{
		// Get the selected property
		ULONG lngPropertyID = dlg.m_lngSelectedID;
		BObject* pobjProperty = m_pDoc->GetObject(lngPropertyID);
		ASSERT_VALID(pobjProperty);

		// Add the selected property to the object's classdef
		// Can't just edit the bdata directly, because it might be the parent classdef's!!!
		// So we need to make a copy, then write the copy back to the class!!
		// Actually that's not true since propObjectProperties doesn't get handled like that!!
//		BDataLink* pdat = STATIC_DOWNCAST(BDataLink, pobjClass->GetPropertyData(propObjectProperties));
//		BDataLink* pdatCopy = STATIC_DOWNCAST(BDataLink, pdat->CreateCopy());
//		pdatCopy->AddLink(pobjProperty);
//		if (!pobjClass->SetPropertyData(propObjectProperties, pdatCopy))
//		{
//			// Standard - if bobject didn't save bdata then delete it
//			delete pdatCopy;
//		}
		//, duplicate code in 3 places!!
		BDataLink* pdatLinks = STATIC_DOWNCAST(BDataLink, pobjClass->GetPropertyData(propObjectProperties));
		if (pdatLinks)
		{
			ASSERT_VALID(pdatLinks);
			pdatLinks->AddLinkID(lngPropertyID, m_pDoc);
		}
		else
		{
			// No bdata yet - create a new one and store it with the class def
			pdatLinks = new BDataLink;
			pdatLinks->SetMultiple();
			pdatLinks->AddLinkID(lngPropertyID, m_pDoc);
			pobjClass->SetPropertyData(propObjectProperties, pdatLinks);
		}

		// Redisplay this view using CViewEx's single view update method!
		UpdateView(NULL, hintLoad, m_pobjCurrent);

		// Select the value of the property just added
		SelectPropertyValue(lngPropertyID);
	}	

	// Enable the properties again
	pobjProperties->SetFlag(flagDisabled, FALSE, TRUE);

	// Make sure focus is back on listview (sometimes wouldn't be)
	m_lvw.SetFocus();
}



// Delete the specified property value.
// If the property is not included in the object's classdef, then it will disappear from the list of properties.
void CViewProperties::OnCmdDeleteProperty() 
{
	ASSERT_VALID(m_pobjCurrent);

	// Delete the selected property value
	BObject* pobjPropDef = (BObject*) m_lvw.GetSelectedItemData();
	ASSERT_VALID(pobjPropDef);
	ULONG lngPropertyID = pobjPropDef->GetObjectID();
//	m_pobjCurrent->DeleteProperty(lngPropertyID);
	m_pobjCurrent->DeleteProperty(lngPropertyID, TRUE, TRUE, TRUE); // ask user

	// Redisplay this view using CViewEx's single view update method
	UpdateView(NULL, hintLoad, m_pobjCurrent);
}





// Printing
//---------------------------------------------------------------------------

BOOL CViewProperties::DoPrint(BOOL bPreview)
{
	if (bPreview)
		CViewEx::OnFilePrintPreview(); // in cviewex
	else
		CViewEx::OnFilePrint(); // in file viewprnt.cpp
	return TRUE;
}


BOOL CViewProperties::OnPreparePrinting(CPrintInfo* pInfo)
{
	xTRACE("CViewProperties OnPreparePrinting\n");
	return DoPreparePrinting(pInfo); // default preparation - brings up dialog
}


// At this point the printer has been selected so we can calculate the max number of pages. 
// Consider printing a spreadsheet. When overriding OnPreparePrinting, you must calculate 
// how many sheets of paper will be required to print the entire spreadsheet and then 
// use that value when calling the SetMaxPage member function of CPrintInfo. 
void CViewProperties::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Initialize fonts margins etc
	CViewEx::OnBeginPrinting(pDC, pInfo);

	theApp.m_printinfo.m_strViewName = "Properties View";
	theApp.m_printinfo.m_strObjectName = m_pDoc->GetCurrentObject()->GetPropertyText(propName);

	// Get grid information etc
	m_lvw.OnBeginPrinting(pDC, pInfo);
}


void CViewProperties::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CPrintInfoMore& rpim = theApp.m_printinfo;

	rpim.PrintHeader(pDC, pInfo);
	m_lvw.PrintPage(pDC, pInfo);
	rpim.PrintFooter(pDC, pInfo);
	rpim.DrawMargins(pDC, pInfo);
}


// Free any fonts and other resources allocated in OnBeginPrinting.
void CViewProperties::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	xTRACE("CViewContents OnEndPrinting\n");
	// call base class to delete gdi objects
	CViewEx::OnEndPrinting(pDC, pInfo);
}

