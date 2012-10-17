
// CViewContents



#include "precompiled.h"

#include "ViewContents.h"

#include "NeoMem.h"
#include "Constants.h"

#include "DialogEditLink.h"
#include "FrameChild.h"
#include "Hint.h"
#include "ListCtrlEx.h"


//#include "Brooklyn.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//#define PRINTMARGIN 0


// Macros
//---------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CViewContents, CViewEx)

BEGIN_MESSAGE_MAP(CViewContents, CViewEx)
	//{{AFX_MSG_MAP(CViewContents)
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATE_FOLLOW_LINK, OnUpdateNavigateFollowLink)
	ON_UPDATE_COMMAND_UI(ID_OBJ_MOVE_DOWN, OnUpdateObjMoveDown)
	ON_UPDATE_COMMAND_UI(ID_OBJ_MOVE_UP, OnUpdateObjMoveUp)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_PLAIN, OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRIDLINES, OnUpdateViewGridlines)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LVW_CONTENTS, OnBeginLabelEdit)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LVW_CONTENTS, OnEndLabelEdit)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_COPY, OnCmdEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnCmdEditCut)
	ON_COMMAND(ID_EDIT_PASTE_PLAIN, OnCmdEditPastePlain)
	ON_COMMAND(ID_EDIT_PASTE_FORMATTED, OnCmdEditPasteFormatted)
	ON_COMMAND(ID_VIEW_GRIDLINES, OnCmdViewGridlines)
	ON_COMMAND(ID_OBJ_ADD, OnObjAdd) //ok
	ON_COMMAND(ID_OBJ_EDIT_IN_PLACE, OnObjEditInPlace) //ok
	ON_COMMAND(ID_OBJ_EDIT_IN_DIALOG, OnObjEditInDialog) //ok
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnCmdEditSelectAll)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY_GRID, OnCmdEditCopyGrid)
	ON_COMMAND(ID_EDIT_CLEAR, OnCmdEditClear)
	ON_COMMAND(ID_EDIT_UNDO, OnCmdEditUndo)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_NAVIGATE_FOLLOW_LINK, OnNavigateFollowLink)
	ON_COMMAND(ID_OBJ_ADD_FOLDER, OnObjAddFolder)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_PLAIN, OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_FORMATTED, OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEdit)
	ON_COMMAND(ID_OBJ_MOVE_TO, OnObjMoveTo)
	ON_UPDATE_COMMAND_UI(ID_OBJ_MOVE_TO, OnUpdateObjMoveTo)
	//}}AFX_MSG_MAP
	// test
//	ON_NOTIFY(HDN_ITEMCHANGED, 0, OnHeaderItemChanged)
//	ON_NOTIFY(HDN_ENDDRAG, 0, OnHeaderEndDrag)
	// Standard printing commands
//	ON_COMMAND(ID_FILE_PRINT, CViewEx::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CViewEx::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CViewEx::OnFilePrintPreview)
	ON_NOTIFY(NM_RETURN, IDC_LVW_CONTENTS, OnNotifyReturn)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LVW_CONTENTS, OnBeginDrag)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LVW_CONTENTS, OnNotifyKeyDown)
//	ON_NOTIFY(NM_RCLICK, IDC_LVW_CONTENTS, OnNotifyRClick)
	ON_NOTIFY(NM_DBLCLK, IDC_LVW_CONTENTS, OnNotifyDblClk)
	ON_COMMAND_RANGE(ID_POPUP_BDATA_START, ID_POPUP_BDATA_STOP, OnPopupBDataCommand)
//	ON_UPDATE_COMMAND_UI(ID_COLUMN_INSERT, OnUpdateColumnInsert)

	// Note: Header Controls normally have ID of 0, but you could set it to match the list control i guess
//	ON_NOTIFY(NM_RCLICK, 0, OnHeaderRClick) // rclick from header control!
//	ON_MESSAGE(CM_COLUMNS_CHANGED, OnLvwColumnsChanged)
	ON_MESSAGE(CM_ITEM_SELECTED, OnLvwItemSelected)
END_MESSAGE_MAP()




// Construction/Destruction
//---------------------------------------------------------------------------

CViewContents::CViewContents() {
//	m_nTargetColumn = -1;
	m_nDragOver = 0;
}

CViewContents::~CViewContents() {
}



// Create child windows
int CViewContents::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CViewEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create listview control as child view
	//, i think these get overridden anyway in clistctrlex creation, right?
	if (m_lvw.Create(
					WS_CHILD |
					WS_VISIBLE |
					LVS_REPORT |			// Report view mode
//					LVS_SHOWSELALWAYS |		// Always show selection
//					LVS_SINGLESEL |			// Allows only one item to be selected (default is multiple)
					LVS_EDITLABELS |		// Allow editing
					LVS_SHAREIMAGELISTS |	// Share image lists
					0,
					CRect(0,0,0,0),
					this,
					IDC_LVW_CONTENTS
					))
	{
		// Store doc pointer
		m_lvw.m_pDoc = m_pDoc;

		// Set our custom listview mode
		m_lvw.m_nMode = CListCtrlEx::modeSelectCell;

		m_lvw.m_bAutoColumns = TRUE;

		// We'll be using the dummy new row
		m_lvw.m_bDummyRow = TRUE;
		m_lvw.m_nNewIconIndex = m_pDoc->GetIconIndex(iconNew);

		// Set extended style (or could call CreateEx I guess)
		//, is this overridden too?
		DWORD dwGridStyle = app.m_bDisplayGridlines ? LVS_EX_GRIDLINES : 0;
		m_lvw.SetExtendedStyle(
						dwGridStyle | 
//						LVS_EX_GRIDLINES |
//						LVS_EX_FULLROWSELECT |
						LVS_EX_HEADERDRAGDROP |
//						LVS_EX_CHECKBOXES |
//						LVS_EX_FLATSB |
//						LVS_EX_SUBITEMIMAGES |
//						LVS_EX_TRACKSELECT |
//						LVS_EX_ONECLICKACTIVATE |
//						LVS_EX_TWOCLICKACTIVATE |
//						LVS_EX_UNDERLINEHOT |
						0);

		// Associate image list with listview
		m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);

		// Set header attributes
		m_lvw.GetHeaderCtrl()->SetImageList(app.GetAppImageList());

		// Tell CViewEx about the view
		SetChildView(&m_lvw);

	}
	else return -1;

	
	// Create toolbar
	if (m_tbr.Create(0, CRect(0,0,0,0), this, IDR_TOOLBAR_CONTENTS))
	{
		// this is lame - every button has same width and expects text
//		m_tbr.ModifyStyle(0, TBSTYLE_LIST);

		// Must call this to initialize the toolbar position correctly
		m_tbr.Position();

		// Load toolbar bitmaps and buttons from resource file
		m_tbr.LoadToolBar(MAKEINTRESOURCE(IDR_TOOLBAR_CONTENTS));

		// Tell the view that there is a toolbar attached to it
		SetToolBar(&m_tbr);

		// Set button text
		// this is lame
//		TBBUTTONINFO tbbi;
//		tbbi.cbSize = sizeof(TBBUTTONINFO);
//		tbbi.dwMask = TBIF_TEXT;
//		tbbi.idCommand = 0;
//		tbbi.iImage = 0;
//		tbbi.fsState = 0;
//		tbbi.fsStyle = 0;
//		tbbi.cx = 0;
//		tbbi.lParam = 0;
//		tbbi.pszText = "Add Column...";
//		tbbi.cchText = 0;
//		m_tbr.SetButtonInfo(ID_COLUMN_INSERT, &tbbi);

	}
	else return -1;

	// Load custom accelerators for this view
	LoadViewAccelerators(IDR_VIEW_CONTENTS);

	// Register this view window as a drop target with OLE DLL's
	CViewEx::DragRegister(&m_lvw); // specify window to receive scroll messages

	return 0;
}




// Diagnostics
//---------------------------------------------------------------------------

#ifdef _DEBUG
void CViewContents::AssertValid() const {
	CViewEx::AssertValid();
}

void CViewContents::Dump(CDumpContext& dc) const {
	CViewEx::Dump(dc);
}

#endif //_DEBUG





// Routines
//---------------------------------------------------------------------------


// Called by the framework after the view is first attached to the document, 
// but before the view is initially displayed. The default implementation of this 
// function calls the OnUpdate member function with no hint information (that is, 
// using the default values of 0 for the lHint parameter and NULL for the pHint 
// parameter). Override this function to perform any one-time initialization that 
// requires information about the document. For example, if your application has 
// fixed-sized documents, you can use this function to initialize a view’s scrolling 
// limits based on the document size. If your application supports variable-sized 
// documents, use OnUpdate to update the scrolling limits every time the document changes.
void CViewContents::OnInitialUpdate() {

	//trace("CViewContents::OnInitialUpdate\n");

	// Store pointer to document
	// bad to do this here because it gets called AFTER OnUpdate!!
//	m_pDoc = (BDoc*) GetDocument();

	// Call base class
	// just calls OnUpdate with 0 0
//	CViewEx::OnInitialUpdate();
}




// The default implementation of OnUpdate invalidates the view's entire client area. 
// You can override it to invalidate only those regions of the client area that map 
// to the modified portions of the document.

// The UpdateAllViews member function of class CDocument and the OnUpdate member function 
// of class CView let you pass information describing what parts of the document were 
// modified. This "hint" mechanism lets you limit the area that the view must redraw. 
// OnUpdate takes two "hint" arguments. The first, lHint, of type LPARAM, lets you pass 
// any data you like, while the second, pHint, of type CObject*, lets you pass a pointer 
// to any object derived from CObject. 

// When a view becomes invalid, Windows sends it a WM_PAINT message. The view's OnPaint 
// handler function responds to the message by creating a device-context object of class 
// CPaintDC and calls your view's OnDraw member function. You do not normally have to 
// write an overriding OnPaint handler function.


// Update the view
void CViewContents::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) {

	//trace("    CViewContents::OnUpdate %s\n", app.GetHintName(lHint));
//	TRACE("CViewContents::OnUpdate(lHint=%d, pHint=0x%pL)\n", lHint, pHint);
//	CWaitCursor cw;

	switch (lHint) {
//		case hintReload:
//		case hintSelect:
		case hintLoad: {

				// Load the view for an object - set up columns and fill listview with contents
				BObject* pobjStart = DYNAMIC_DOWNCAST(BObject, pHint);
				ASSERT_VALID(pobjStart);

				// Column Info:
				// all folders have unique column layout
				// so if we're on a folder, should write to the object
				// eg all tape objects have the same column layout
				// in that case, we would get from the tape classdef
				// and write to the tape classdef

				// * so in writing the value, we need to check the class of the object
				// if it's a folder, write directly to the object
				// if it's not, get the object's classdef and write the changes there

				// create copy of the column information only on change
				// actually only need to copy it if it was inherited from a base class
				// ie if we got the data directly from the object's class, then we wouldn't need
				// to copy it, because writing to the data is okay

				// Note: hintLoad gets called twice on program startup.

				// If we still have a pointer to a BDataColumns object and it was never saved,
				// we need to delete it, as it's just a copy of one from the document.
				// The destructor does this same code.
//				if ((m_pdatCopy != NULL) && (m_bColumnsSaved == FALSE))
//				{
//					delete m_pdatCopy;
//					m_pdatCopy = NULL;
//				}

				// Get column information from object or from object's classdef
//x				BDataColumns& cols = pobjStart->GetColumns();
				BDataColumns* pdatColumns = DYNAMIC_DOWNCAST(BDataColumns, pobjStart->GetPropertyData(propColumnInfoArray));

				// Invoke copy constructor to create copy of the array
				// We need to copy the data because it might be coming from the object's classdef,
				// in which case we don't want the listview to modify it directly. Instead, it will modify
				// this copy, which we'll then save to the object on hintSave if necessary.
				// hintSave will delete it if it doesn't save it.
//				m_pdatCopy = new BDataColumns(pdatColumns);

				// Set flag so we know we're responsible for deleting the bdata object (since it's a copy and
				// not owned by any bobject).
//				m_bColumnsSaved = FALSE;

				// Initialize listview columns - see ListCtrlEx.cpp
//				m_lvw.InitializeColumns(m_pdatCopy);
				m_lvw.InitializeColumns(pdatColumns);
				delete pdatColumns;
//x				m_lvw.InitializeColumns(&cols);
//x				delete &cols;

				// Add the children of the start object to the list, with no recursion
				m_lvw.AddObjects(pobjStart, app.m_lngExcludeFlags, FALSE, FALSE, 0, TRUE);

				// Add dummy new row
				m_lvw.AddDummyRow();
				
				// Sort items
				BOOL bAutosort = !(pobjStart->GetFlag(flagNoAutosort));
				if (bAutosort)
					m_lvw.SortByProperty(propName, 1);
				else
					m_lvw.ClearSortMemory();

				break;
			}

		case hintSave: {

				// Bug: hintLoad was being called twice, while hintSave was called once, causing
				// one of the bdata copies to be a memory leak!
				// Note: m_pdatCopy might be zero, which is the case when we're closing a document
				// and we broadcast hintSave just to make sure everything is saved.
//				if (m_pdatCopy)
//				{
//					ASSERT_VALID(m_pdatCopy);
					// Save column arrangement if changed
//					if (m_pdatCopy->m_bModified)
//					if (m_lvw.m_bColumnsChanged)
//					{
						BObject* pobj = DYNAMIC_DOWNCAST(BObject, pHint);
						ASSERT_VALID(pobj);
						m_lvw.SaveColumnInfo(pobj); // this saves the BDataColumns object to the pobj
						// Set flag so we know that we don't own the BDataColumns object anymore, 
						// and are not responsible for deleting it in hintLoad or the destructor.
//						m_bColumnsSaved = TRUE;
//					}
//				}
				break;
			}

		case hintAdd: {

				// Add an item to the listview
				BObject* pobjNew = DYNAMIC_DOWNCAST(BObject, pHint);
				ASSERT_VALID(pobjNew);
				BObject* pobjParent = pobjNew->GetParent();
				ASSERT_VALID(pobjParent);

				// Only need to handle this if the parent is the document's currently selected item
				// (otherwise the new object is not visible in contents)
				if (pobjParent == m_pDoc->GetCurrentObject()) {

					// Remove dummy new row
					m_lvw.RemoveDummyRow();

					// Add this BObject to the list
					m_lvw.AddObject(pobjNew);

					// Add dummy new row
					m_lvw.AddDummyRow();
				}
				break;
			}

		case hintDelete: {

				// Delete items from the listview and/or the column bdata (might be deleting a propertydef!)
				CHint* pobjHint = DYNAMIC_DOWNCAST(CHint, pHint);
				BObjects* paObjects = pobjHint->paObjects;
				ASSERT_VALID(paObjects);
				int nItems = paObjects->GetSize();
				for (int i = 0; i < nItems; i++) {
					BObject* pobj = DYNAMIC_DOWNCAST(BObject, paObjects->GetAt(i));
					// Note: pobj may be invalid by this point because it's already been deleted,
					// but we still need to remove it from the list, so DON'T ASSERT_VALID
//					ASSERT_VALID(pobj);
					int nItem = m_lvw.FindItemData((LPARAM) pobj);
					if (nItem != -1) {
						m_lvw.DeleteItem(nItem);
					}
					// Now check column bdata
					//. if this has already been saved to the object, then the bdata should have been updated,
					// but the listview control will still have the column.
					// i guess we just want to remove the column if it's there - save will take care of the rest
					// let listview handle all this
					m_lvw.DeleteColumnProperty(pobj);
				}
				// Bug: If deleting object from index view, and the next object selected happens to be that
				// object's parent, then the object gets deleted after being adding to the contents view,
				// but getdispinfo gets called on the deleted object, causing a bomb.
				// And a bad bomb too, because in release mode you're pointing at invalid data.
				break;
			}
		
		case hintPropertyChange: {

				//... need to update any dependent properties also
				// eg if classid changes, then classname changes also
				// parent -> location, etc.
				// need generic way of handling this
				// eg if this were displaying a calculated property, like total = rate * hours
				// then when hours is updated, need to update total also
				// that code you might want to put in obj.SetProperty
				// because there you could check if the property has any dependent properties
				// and then send property change hints for all those properties as well

				CHint* pobjHint = DYNAMIC_DOWNCAST(CHint, pHint);
				ASSERT_VALID(pobjHint);
//				CHint* pobjHint = CHint::ToHint(pHint);
				
				BObject* pobj = pobjHint->pobjObject;
				ASSERT_VALID(pobj);
				OBJID idProperty = pobjHint->idProperty;

				if ((pobj == m_pDoc->GetCurrentObject()) && (idProperty == propColumnInfoArray)) {
//					AfxMessageBox ("hi!");

					// Get column information from object or from object's classdef
//x					BDataColumns& cols = pobj->GetColumns();
					BDataColumns* pdat = DYNAMIC_DOWNCAST(BDataColumns, pobj->GetPropertyData(propColumnInfoArray));

					// Initialize listview columns - see ListCtrlEx.cpp
					m_lvw.InitializeColumns(pdat);
					delete pdat;

				}
				else {

					// Look for object in list
					int nItem = m_lvw.FindItemData((LPARAM) pobj);
					if (nItem != -1) {

						// Get new icon if icon or class changed
						if (idProperty == propIconID || idProperty == propClassID 
								|| idProperty == propClassName) {
							// Get the new index of the icon in the image list
							int nImage = pobj->GetIconIndex();
							m_lvw.SetItem(nItem, 0, LVIF_IMAGE, 0, nImage, 0, 0, 0);
						}

						// Look for property in columns
						int nCol = m_lvw.FindColumn(pobjHint->idProperty);
						if (nCol != -1) {
							// Update the item's row
							m_lvw.Update(nItem);
						}
					}

					// If the object that is changing is a property, see if it's in the columns.
					// Note: When you edit a property, several properties get changed so this might get
					// called several times.
					if (pobj->GetClassID() == classProperty) {
						// Only concerned with name and property type right now
						if ((idProperty == propName) || (idProperty == propPropertyType)) {
							OBJID idObject = pobj->GetObjectID();
							int nCol = m_lvw.FindColumn(idObject);
							if (nCol != -1) {
								// Found it, so update the column info now
								m_lvw.UpdateColumn(nCol, pobj);
							}
						}
					}
				}

				break;
			}


		case hintMoveObject: {

				// Move object into or out of the listview
				// 1. if object is in this view, check the new parent. if new parent != current object, remove item
				// 2. if object is not in this view, check the new parent. if new parent == current object, add item

				BObject* pobj = DYNAMIC_DOWNCAST(BObject, pHint);
				ASSERT_VALID(pobj);

				// If the new parent for the objects is the current object, then objects are being moved here.
				// Otherwise, they are being moved to another folder.
				//. have similar code in 3 hint handlers here...
				BOOL bMoveHere = (pobj->GetParent() == m_pDoc->GetCurrentObject());

				// Remove dummy new row
				m_lvw.RemoveDummyRow();

				// Search for object in the listview control
				int nItem = m_lvw.FindItemData((LPARAM) pobj);
					
				// If object is in this view, and it is being moved elsewhere, remove it
				if (nItem != -1 && !bMoveHere)
					m_lvw.DeleteItem(nItem);

				// If object is not in this view, and is being moved here, add it
				if (nItem == -1 && bMoveHere)
					m_lvw.AddObject(pobj);

				// Re-add dummy new row
				m_lvw.AddDummyRow();

				break;
			}

		case hintReposition: {

				// Move item up or down
				// Move pobjObject before or after pobjTarget, depending on bAfter flag.
				CHint* ph = DYNAMIC_DOWNCAST(CHint, pHint);
				BObject* pobjSource = ph->pobjObject;
				BObject* pobjTarget = ph->pobjTarget;
				BOOL bAfter = ph->bAfter;
				ASSERT_VALID(pobjSource);
				ASSERT_VALID(pobjTarget);
				
				// Need to know if the currently selected object is the source being repositioned,
				// as it will be deleted and we'll need to reselect it if so.
				BOOL bSourceIsCurrent = (GetSelectedObject() == pobjSource);

//				m_lvw.LockWindowUpdate();
//				m_lvw.UnlockWindowUpdate();
				int nSource = m_lvw.FindItemData((LPARAM) pobjSource);
				int nTarget = m_lvw.FindItemData((LPARAM) pobjTarget);
				
				// bug:: instead of != -1 had && -1 for target
				if ((nSource != -1) && (nTarget != -1)) {
					// Move the source before or after the target
					int nSourceNew = m_lvw.MoveItemToSibling(nSource, nTarget, bAfter);

					// Reselect the source if it was the current selection on entry
					if (bSourceIsCurrent)
						m_lvw.SelectItem(nSourceNew);
				}

				break;
			}

		case hintRefresh: {
				// Redisplay all objects
				m_lvw.RedrawWindow();
				break;
			}

		case hintResortChildren: {
				BObject* pobjParent = DYNAMIC_DOWNCAST(BObject, pHint);
				ASSERT_VALID(pobjParent);
				// Resort this view if the parent object is the currently displayed object
				if (pobjParent == m_pDoc->GetCurrentObject()) {
//					m_lvw.SortByProperty(); //, flips sort order right?
					m_lvw.SortByProperty(propName, 1);
					m_lvw.ClearSortMemory();
				}
				break;
			}

		case hintRefreshAllIcons: {

				// Walk through all items, reobtaining their icons
				int nItems = m_lvw.GetItemCount();
				for (int i = 0; i < nItems; i++) {
					BObject* pobj = (BObject*) m_lvw.GetItemData(i);
					if (pobj) {
						ASSERT_VALID(pobj);
						int nIcon = pobj->GetIconIndex();
						m_lvw.SetItem(i, 0, LVIF_IMAGE, 0, nIcon, 0, 0, 0);
					}					
				}
				break;
			}
/*
		case hintReloadChildren: {
				// Reload the children of the specified object
				//. this is called by obsolete sort code...
				BObject* pobjStart = (BObject*) pHint;
				ASSERT_VALID(pobjStart);
				// Note: we can ignore this if the specified object is not the current object,
				// because this view only displays the children of the current object,
				// according to the document.
				BObject* pobjCurrent = m_pDoc->GetCurrentObject();
				ASSERT_VALID(pobjCurrent);
				if (pobjCurrent == pobjStart)
				{
					// Add children to listview
					AddChildrenToList(pobjStart);
				}
			}
*/


//		default:
			// Call base class (invalidates entire client area)
//			CViewEx::OnUpdate(pSender, lHint, pHint);
//			break;
	}
}









// Popup Menu handlers
//---------------------------------------------------------------------------


// Show context menu
// Three possibilities: No items selected, One item selected, Multiple items selected.
// should really only do no items selected if clicked outside of a valid cell!
//, if drag select multiple items, then right click drag, then show multiple menu.
// Note: See OnHeaderRClick for header popup menu.
void CViewContents::OnContextMenu(CWnd* pWnd, CPoint ptScreen) {

	//trace("CViewContents::OnContextMenu(ptscreen.x,y=%d,%d)\n", ptScreen.x, ptScreen.y);

	// Set flag if user pressed context key (x and y will be -1)
	BOOL bContextKey = ((ptScreen.x == -1) && (ptScreen.y == -1));

	// Check if user right clicked in the header control
	if (!bContextKey) {

		// Exit if right click was in header control
		//, move onheaderrclick code here, then move it down below
		CRect r;
		m_lvw.GetHeaderCtrl()->GetClientRect(&r);
		m_lvw.GetHeaderCtrl()->ClientToScreen(&r);
		if (r.PtInRect(ptScreen)) return;

		// Exit if right click was not in listview window
		m_lvw.GetWindowRect(r);
		if (!r.PtInRect(ptScreen)) return;
	}

	// Get selected item(s)
//	int nItems = m_lvw.GetSelectedItemsArray(m_aObjects);
	int nItem = m_lvw.GetSelectedItem();
	BObject* pobj = (BObject*) m_lvw.GetItemData(nItem);

	// See if on new row
	BOOL bOnNewRow = (pobj == (BObject*) CListCtrlEx::keyDummyRow);

	// See if rclicked on first column
	// Bug: wasn't converting to the right window - was using this view's screentoclient, not m_lvw's
	UINT nFlags;
	CPoint ptClient = ptScreen;
	m_lvw.ScreenToClient(&ptClient);
	m_lvw.HitTest(ptClient, &nFlags);
//	BOOL bOnIcon = (nFlags & LVHT_ONITEMICON);
	BOOL bOnFirstItem = (m_lvw.m_nCol == 0);

	// If no cell selected, user must have clicked on background
	if (m_lvw.m_nRow == -1 || m_lvw.m_nCol == -1) {

		// Show popup menu for background

		// If user pressed context menu key use center of window
		if (bContextKey) {
			CRect r;
			m_lvw.GetClientRect(&r);
			m_lvw.ClientToScreen(&r);
			ptScreen = r.CenterPoint();
		}

		// Use the current object as popup target (mimics Window Explorer behavior)
		m_pDoc->SetTargetObject(m_pDoc->GetCurrentObject());

//		CMenu menu;
//		if (menu.LoadMenu(IDR_POPUP_CONTENTS_EMPTY)) {
//			CMenu* pPopup = menu.GetSubMenu(0);
//			if (pPopup) {
////				BOOL bNoModifyClass = m_pobjPopup->GetFlag(flagNoModifyClass);
////				pPopup->EnableMenuItem(ID_OBJ_CHANGE_OBJECT_CLASS, bNoModifyClass ? MF_GRAYED : MF_ENABLED);
//				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
//			}
//		}
		BCMenu* pPopup = InitPopup(IDR_POPUP_CONTENTS_EMPTY);
		if (pPopup) {
//			BOOL bNoModifyClass = m_pobjPopup->GetFlag(flagNoModifyClass);
//			pPopup->EnableMenuItem(ID_OBJ_CHANGE_OBJECT_CLASS, bNoModifyClass ? MF_GRAYED : MF_ENABLED);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
		}

	}


	// Bring up object menu if user rclicked on the first column (and not on new row)
//	else if (bOnFirstItem)
	else if (bOnFirstItem && !bOnNewRow) {
		// If user pressed context menu key use center of item
		if (bContextKey) {
			CRect r;
			if (m_lvw.GetItemRect(nItem, &r, LVIR_ICON)) {
				m_lvw.ClientToScreen(&r);
				ptScreen = r.CenterPoint();
			}
		}

		// Get bobject address
//		BObject* pobj = (BObject*) m_lvw.GetItemData(nItem);
//		if (pobj == (BObject*) CListCtrlEx::keyDummyRow) return; // exit if right clicked on new row
		ASSERT_VALID(pobj);
		
		// Save item so that menu handlers will know which object was clicked on
		m_pDoc->SetTargetObject(pobj);

		// Display pop up menu
//		CMenu menu;
//		if (menu.LoadMenu(IDR_POPUP_CONTENTS)) {
//			CMenu* pPopup = menu.GetSubMenu(0);
			BCMenu* pPopup = InitPopup(IDR_POPUP_CONTENTS, ID_OBJ_OPEN);
			if (pPopup) {
//				pPopup->SetDefaultItem(ID_OBJ_OPEN); // default is same as double clicking
//				pPopup->SetDefaultItem(ID_OBJ_EDIT_IN_DIALOG); // default is same as double clicking

				// Check high priority item if flag is set for the object
//				BOOL bHighPriority = m_pobjPopup->GetFlag(flagHighPriority);
//				UINT nCheck = MF_BYCOMMAND | bHighPriority ? MF_CHECKED : MF_UNCHECKED;
//				pPopup->CheckMenuItem(ID_OBJ_PRIORITY_HIGH, nCheck);

//				pPopup->Attach
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
			}
//		}

	}

	// Bring up value menu if user rclicked on a cell value
	else {
		// If user pressed context menu key use cell position
		if (bContextKey) {
			CRect r;
			m_lvw.GetCellRect(m_lvw.m_nRow, m_lvw.m_nCol, r);
			m_lvw.ClientToScreen(&r);
			ptScreen.x = r.left + 6;
			ptScreen.y = r.top + 6;
		}

		// Set target object
//		BObject* pobj = (BObject*) m_lvw.GetItemData(nItem);
//		if (pobj == (BObject*) CListCtrlEx::keyDummyRow) return; // exit if right clicked on new row
//		ASSERT_VALID(pobj);		
		m_pDoc->SetTargetObject(pobj);

//		CMenu menu;
//		if (menu.LoadMenu(IDR_POPUP_EDIT_VALUE)) {
//			CMenu* pPopup = menu.GetSubMenu(0);
			BCMenu* pPopup = InitPopup(IDR_POPUP_EDIT_VALUE, ID_OBJ_EDIT_IN_DIALOG);
			if (pPopup) {
				// Add bdata menu items, if any
				BData* pdat = m_lvw.GetCellBData();
				if (pdat) {
					ASSERT_VALID(pdat);
					int nPos = pPopup->GetMenuItemCount() - 1; // insert before "Cancel"
					pdat->UIAddMenuItems(pPopup, nPos);
				}
				
				// Enable/disable undo menu
//				pPopup->EnableMenuItem(ID_EDIT_UNDO, MF_BYCOMMAND | (m_objUndo.IsValid() ? MF_ENABLED : MF_GRAYED));

				// Set default item
//				pPopup->SetDefaultItem(ID_OBJ_EDIT_IN_DIALOG); // default is same as double clicking
				
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
			}
//		}

	}

//, multiple selection not implemented yet
/*
	else if (nItems > 1) {
		// If user pressed context menu key use center of window
		if (bContextKey) {
			CRect r;
			m_lvw.GetClientRect(&r);
			ClientToScreen(&r);
			ptScreen = r.CenterPoint();
		}

		// Get array of selected bobjects into BDataLink object
//		m_lvw.GetSelectedItems(m_datPopupLink);
		m_pobjPopup = 0;
		m_nPopupItem = 0;
		
		// Display pop up menu
//		CMenu menu;
//		if (menu.LoadMenu(IDR_POPUP_CONTENTS_MULTI)) {
//			CMenu* pPopup = menu.GetSubMenu(0);
			BCMenu* pPopup = InitPopup(IDR_POPUP_CONTENTS_MULTI);
			if (pPopup) {
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
			}
//		}
	}
*/

}



// Pass the specified BDataCommand onto the right clicked BData value
void CViewContents::OnPopupBDataCommand(UINT nCommandID) {
	BData* pdat = m_lvw.GetCellBData();
	if (pdat) {
		ASSERT_VALID(pdat);
		pdat->UIHandleCommand(nCommandID);
	}	
}




void CViewContents::OnCmdViewGridlines() {
	// Toggle gridlines on/off
	DWORD dwStyle = m_lvw.GetExtendedStyle();
	dwStyle ^= LVS_EX_GRIDLINES;
	m_lvw.SetExtendedStyle(dwStyle);
	// Update app variable
	app.m_bDisplayGridlines = (dwStyle & LVS_EX_GRIDLINES) ? TRUE : FALSE;
}


void CViewContents::OnUpdateViewGridlines(CCmdUI* pCmdUI) {
	// Indent button if gridlines are on
	DWORD dwStyle = m_lvw.GetExtendedStyle();
	int nCheck = (dwStyle & LVS_EX_GRIDLINES) ? 1 : 0;
	pCmdUI->SetCheck(nCheck);
}



// Edit the currently selected cell.
// This is the F2 handler.
// CListCtrlEx handles selecting a cell, moving around the grid, etc.
void CViewContents::OnObjEditInPlace() {
	// This will create an edit box on current cell and send OnEndLabelEdit message when done.
	m_lvw.EditCurrentCell();
}


void CViewContents::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) {
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	//. Check if item is read-only (eg location)
	// To prevent the user from editing the label, return TRUE.
/*
	CEdit* pEdit = m_lvw.GetEditControl();
	if (pEdit)
	{
		UINT nMax = pEdit->GetLimitText();
		//trace("nmax=%d\n", nMax);
		pEdit->SetLimitText(0); // sets to UINT_MAX 
	}
*/
	*pResult = 0;
}


// User finished editing the text of a cell.
// Do data validation, return FALSE if you don't like it.
//, move this to clistctrlex also?
void CViewContents::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) {

	//trace("CViewContents::OnEndLabelEdit\n");
	
	// Update CDocument and other views
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LVITEM* pLVITEM = &(pDispInfo->item);
	if (pLVITEM->pszText) {

		// Get object edited from iItem
		int nItem = pLVITEM->iItem;
		BObject* pobj = (BObject*) m_lvw.GetItemData(nItem);

		// Check if we're on the new row (pobj will be magic value of 0)
		if ((DWORD) pobj == CListCtrlEx::keyDummyRow) {

			// We're on the new row, so we'll want to add a new object, modify the dummy row, tell all views,
			// then re-add a new dummy row

			// Add new object of default class
			BObject* pobjParent = m_pDoc->GetCurrentObject();
			OBJID idClass = pobjParent->GetPropertyLink(propDefaultClass);
			BObject* pobjClass = m_pDoc->GetObject(idClass);
			CString strName;
			pobjClass->GetClassDefNewName(strName);

			BObject& objNew = BObject::New(*m_pDoc, idClass, strName, pobjParent->id);

			// Set the property that the user was editing
			ULONG lngPropertyID = m_lvw.GetColumnPropertyID(pLVITEM->iSubItem);
			objNew.SetPropertyString(lngPropertyID, pLVITEM->pszText, FALSE, FALSE);

			// Modify the dummy item to become the new object
			int nImage = objNew.GetIconIndex();
			m_lvw.SetItem(nItem, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, LPSTR_TEXTCALLBACK, nImage, 0, 0, (LPARAM) &objNew);

			// Now re-add the dummy row
			m_lvw.AddDummyRow();

			//, Now select the dummy row
			// depends on which key they ended the edit with - if enter, should select dummy row,
			// if tab, should select next column

		}
		else {

			// We're on a regular object row
			ASSERT_VALID(pobj);

			// Get property edited from iSubItem
			ULONG lngPropertyID = m_lvw.GetColumnPropertyID(pLVITEM->iSubItem);

			// This will set document modified flag and update views
			// Note: We need also to tell this view also because if classname or icon changes,
			// then OnUpdate code will modify the icon also.
			pobj->SetPropertyString(lngPropertyID, pLVITEM->pszText);
		}


		// Text is okay, we accept it
		*pResult = TRUE;
	}
	else {
		*pResult = 0;
	}
}




// Clipboard Operations
//----------------------------------------------------------------------------------------------------

// Copy the selected items to the clipboard
void CViewContents::OnCmdEditCopy() {

	//trace("CViewContents::OnCmdEditCopy\n");
	
	// Exit if on new row (nothing there)
	BObject* pobj = GetSelectedObject();
	if (pobj == (BObject*) CListCtrlEx::keyDummyRow)
		return;

	m_lvw.CopyCurrentCellToClipboard();

/*
	// Add selected items to a BObjects collection object
	// all we need is an array of the object addresses, which is what bobjects is
	BObjects aObjects;
	if (!m_lvw.GetSelectedItemsArray(aObjects))
		return; // none selected

	// Save the bobjects array to a global memory object
	HGLOBAL hGlobal = aObjects.SaveToGlobal();
	if (!hGlobal) return;

	CString strItems = aObjects.GetText();
	//trace("  Copy the following objects to the clipboard: %s\n", (LPCTSTR) strItems);

	// Put the data on the clipboard
	//.. memory leak - when is this supposed to get deleted?
	// maybe the operating system takes care of it?
	COleDataSource* pds = new COleDataSource;
	pds->CacheGlobalData(app.m_cfObjects, hGlobal);
	pds->SetClipboard();
*/
}


void CViewContents::OnCmdEditCut() 
{
	// Exit if on new row (nothing there)
	BObject* pobj = GetSelectedObject();
	if (pobj == (BObject*) CListCtrlEx::keyDummyRow)
		return;

	m_lvw.CutCurrentCellToClipboard();

	// Call copy and then 'delete' the items from the document (mark the items as cut)
//	OnCmdEditCopy();
	//. rather than delete the items, mark them as cut,
	// then on paste, we just move them to their new location.
	//. this will work as long as we stay within the document
	// set their state to cut, and add them to an application cut items list?
//	m_lvw.SetSelectedItemsState(LVIS_CUT, LVIS_CUT);
	//. when/how do we clear the cut state?
}


// Paste contents of clipboard (array of bobject pointers)
//. paste will need to know if source data was cut or copied to clipboard - 
// if cut, then paste will need to move the items to the new location,
// if copied, then paste will need to create duplicates of the items and put them in the new location
void CViewContents::OnCmdEditPasteFormatted() 
{
	//trace("CViewContents::OnPasteFormatted\n");

	m_lvw.PasteClipboardToCurrentCell();
	
/*	if (odo.IsDataAvailable(app.m_cfObjects))
	{
		HGLOBAL hGlobal = odo.GetGlobalData(app.m_cfObjects);

		// Recreate bobjects array from global memory object
		//. note: this won't work if coming from another instance (right?) as we won't have
		// access to that instances data (pobjects)?
		BObjects aObjects;
		aObjects.LoadFromGlobal(hGlobal);

		// we need to know where to paste the items
		// if contents has the focus, then just add as child of the currently selected item (get from document)
		// this mimics explorer behavior, even when a file or folder is selected
		BObject* pobjParent = m_pDoc->GetCurrentObject();

		CString strItems = aObjects.GetText();
		//trace("  Paste the following items as children of %s: %s\n", (LPCTSTR) pobjParent->GetText(), (LPCTSTR) strItems);

	}
*/	
}



void CViewContents::OnUpdateEdit(CCmdUI* pCmdUI) 
{
	m_lvw.UpdateEdit(pCmdUI);
}



// This is the Delete key handler.
// Clear the current property value or delete the object.
void CViewContents::OnCmdEditClear() 
{
	// Get target object
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();
	if (pobj)
	{
		ASSERT_VALID(pobj);

		// Get current column
		int nCol = m_lvw.m_nCol;
		if ((nCol >=0) && (nCol < m_lvw.GetColumnCount()))
		{
			// Get target property
			ULONG lngPropertyID = m_lvw.GetColumnPropertyID(nCol);

			// If on name property, delete the object
			if (lngPropertyID == propName)
			{
				m_pDoc->UIDeleteObject(pobj);
			}
			else
			// Otherwise just clear the cell contents
			{
				// Clear the cell by deleting the property value at the cell.
//				// Save copy of bdata to undo object.
//				m_objUndo.SaveDeleteProperty(pobj, lngPropertyID);
//				// Try to delete property value - if failed, clear the undo object we just created
//				if (!pobj->DeleteProperty(lngPropertyID))
//					m_objUndo.Clear();
//				pobj->DeleteProperty(lngPropertyID);
				pobj->DeleteProperty(lngPropertyID, TRUE, TRUE, TRUE); // ask user
			}
		}
	}
	// Set focus back to listview
	m_lvw.SetFocus();
}



void CViewContents::OnCmdEditUndo() 
{
	// Check that our undo bdata object is still valid
	// If valid, restore it
//	if (m_objUndo.IsValid())
//	{
//		m_objUndo.Restore();
//	}
}

void CViewContents::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// Enable undo if we have an undo bdata object AND it's valid 
	// AND undo object is still valid AND undo property is still valid
//	pCmdUI->Enable(m_objUndo.IsValid());
	pCmdUI->Enable(FALSE);
}





// Drag Drop Operations
//----------------------------------------------------------------------------------------------------


// LVN_BEGINDRAG Handler
// Notifies a list view control's parent window that a drag-and-drop operation involving 
// the left mouse button is being initiated. 
void CViewContents::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//trace("CViewContents::OnBeginDrag\n");

	// Get drag object(s) into array
	m_lvw.GetSelectedItemsArray(m_aDragObjects);

	// Start drag operation by wrapping objects into an hGlobal.
	CViewEx::DragStart();
}


BObject* CViewContents::DragGetDropTarget(CPoint ptClient)
{
	//trace("CViewContents::DragGetDropTarget\n");
	
	// All we need is the top left coord of the child window within the view
	CRect rView;
	CRect rChild;
	GetWindowRect(&rView);
	m_lvw.GetWindowRect(&rChild);
	CPoint ptOffset(- rChild.left + rView.left, - rChild.top + rView.top);
	ptClient.Offset(ptOffset);

	// Do hit test 
	int nCol;
	int nItem = m_lvw.HitTestEx(ptClient, &nCol);

	BObject* pobj = 0;
	if (nItem != -1)
		pobj = (BObject*) m_lvw.GetItemData(nItem);
	else
		// If no item is under cursor, then assume user wants to drop item(s) into this object!
		pobj = m_pDoc->GetCurrentObject();

	return pobj;
}



DROPEFFECT CViewContents::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	//trace("CViewContents::OnDragOver\n");

	// Call base class
	CViewEx::OnDragOver(pDataObject, dwKeyState, point);

	// Ignore if not interested in the data
	if (!m_bInterestedInData) return DROPEFFECT_NONE;

	// See if cursor is over an item
	// Note: point should be in client coords - ie may need to translate into target window space.
	// Ie Ole gives you coords relative to the window you register with, which is the view.
	// But what we need here is the coordinates relative to the child window, the listview control,
	// so need to offset the point.
	// All we need is the top left coord of the child window within the view.
	UINT nFlags = 0;
	DROPEFFECT de;
	CRect rChild;
	m_lvw.GetWindowRect(&rChild);
	CRect rView;
	GetWindowRect(&rView);
	CPoint ptOffset(- rChild.left + rView.left, - rChild.top + rView.top);
	point.Offset(ptOffset);

	// Get item the cursor is over. Will return -1 if not over anything
//	int nItem = m_lvw.HitTest(point, &nFlags);
	int nCol;
	int nItem = m_lvw.HitTestEx(point, &nCol);

	// If cursor is over dummy row, treat as if it's over nothing
	if (nItem != -1)
	{
		DWORD dwData = m_lvw.GetItemData(nItem);
		if (dwData == CListCtrlEx::keyDummyRow)
			nItem = -1;
	}

	// If control key is pressed, user wants to copy the data.
	// Even if cursor is not over an item, we can still drop it here
	// Note: We check the validity of the target in OnDrop, not here.
	de = (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;

	// Highlight the item if changed from last time
	if (nItem != m_nDragOver)
	{
		// If no item is selected, clear all selected items
		if (nItem == -1)
			m_lvw.ClearSelections();
		else
//			m_lvw.SelectDropTarget(nItem);
			m_lvw.SelectItem(nItem);
	}

	// Save which item cursor is over, if any
	m_nDragOver = nItem;

	return de;
}



void CViewContents::OnDragLeave()
{
	//trace("CViewContents::OnDragLeave\n");

	// Clear the drop target
//	m_lvw.SelectDropTarget(NULL);
	m_lvw.ClearSelections();
	m_nDragOver = 0;

	CViewEx::OnDragLeave();
}


//---------------------------------------------------------------------------------------------------





void CViewContents::OnNotifyKeyDown(NMHDR* pNMHDR, LRESULT* pResult)
{
/*
	NMLVKEYDOWN* pkd = (NMLVKEYDOWN*) pNMHDR;
	//trace("CViewContents OnNotifyKeyDown %d\n", pkd->wVKey);
	if (pkd->wVKey == VK_DELETE)
	{
		// If user is not editing a label, translate the delete to an ID_EDIT_CLEAR command
		// and send it up to the child frame.
		if (!m_lvw.GetEditControl())
		{
			CFrameChild* pFrame = (CFrameChild*) GetParentFrame();
			pFrame->SendMessage(WM_COMMAND, ID_EDIT_CLEAR);
		}
	}
*/
}









// this doesn't appear to be called ever
// i think cause we set focus to the control, so that's where all the keypresses go
void CViewContents::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	AfxMessageBox("keydown");
	CViewEx::OnKeyDown(nChar, nRepCnt, nFlags);
}



// Select all listview items
void CViewContents::OnCmdEditSelectAll() 
{
	//.
	m_lvw.SelectItem(-1);	
}



// User double clicked on an item, so edit the value.
void CViewContents::OnNotifyDblClk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Check mouse position - if over icon, select the item.
	CPoint pt(::GetMessagePos()); // get position of message source (screen coords)
	m_lvw.ScreenToClient(&pt); // convert to listview client coords

	UINT uFlags;
	int iItem = m_lvw.HitTest(pt, &uFlags);
//	BOOL bOnIcon = uFlags & LVHT_ONITEMICON;
//	BOOL bOnItem = uFlags & LVHT_ONITEM;
	BOOL bOnFirstItem = (m_lvw.m_nCol == 0);
	iItem = m_lvw.m_nRow;
//	if ((iItem != -1) && bOnIcon) // check if double clicked on item icon
	if ((iItem != -1) && bOnFirstItem) // check if double clicked on item icon
	{
		BObject* pobj = (BObject*) m_lvw.GetItemData(iItem);
		if (pobj)
//		if ((pobj != NULL) && (pobj != (BObject*) CListCtrlEx::keyDummyRow))
		{
			ASSERT_VALID(pobj);
			// Select item in treeview
			// This will broadcast all the update messages necessary (save, select, load)
      
			// bug: was passing this view to SetCurrentObject, but that meant that this view wasn't
			// receiving the update message, so if you double clicked on a folder, the contents would not
			// be refreshed even though you were now on a new object! fixed 2003-04-04.
//			m_pDoc->SetCurrentObject(pobj, this);
			m_pDoc->SetCurrentObject(pobj, NULL);
			return; 
		}
		else
		{
			// Double clicked on first cell of new row
			return;
		}
	}

	// If double clicked on a cell, bring up edit dialog as if they hit F4.
	if (m_lvw.m_nCol != -1 && m_lvw.m_nRow != -1)
	{
		OnObjEditInDialog();
	}
	*pResult = 0;
}





void CViewContents::OnNotifyReturn(NMHDR* pNMHDR, LRESULT* pResult)
{
/*
	// If user hit enter on the item in column 0, select it
	if (m_lvw.m_nCol == 0 && m_lvw.m_nRow != -1) 
	{
		if (m_lvw.m_nRow < m_lvw.GetItemCount())
		{
			BObject* pobj = (BObject*) m_lvw.GetItemData(m_lvw.m_nRow);
			if (pobj)
			{
				ASSERT_VALID(pobj);
				// Select item in treeview
				// This will broadcast all the update messages necessary (save, select, load)
				m_pDoc->SetCurrentObject(pobj, this);
			}
		}
	}
*/
	*pResult = 0;
}



void CViewContents::OnSize(UINT nType, int cx, int cy) 
{
	CViewEx::OnSize(nType, cx, cy);
}




// Copy the contents of the grid to the clipboard
void CViewContents::OnCmdEditCopyGrid() 
{
	m_lvw.CopyToClipboard(TRUE);
}




// Get the currently selected bobject, if any, or 0 if none.
BObject* CViewContents::GetSelectedObject()
{
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();	
	if (pobj)
	{
		ASSERT_VALID(pobj);
	}
	return pobj;
}





//---------------------------------------------------------------------------------------------------



// Don't delete this code! good for debugging!
#ifdef _DEBUG
BOOL CViewContents::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// unpack contents and print them
	NMHDR* pnm = (NMHDR*) lParam;
	UINT nIDFrom = pnm->idFrom;
	UINT nCode = pnm->code;
	CString str = app.GetNotifyCodeString(nCode);
	//trace("CViewContents OnNotify id %d, code %d %s\n", nIDFrom, nCode, (LPCTSTR) str);

	// as you were
	return CViewEx::OnNotify(wParam, lParam, pResult);
}
#endif



//---------------------------------------------------------------------------------------------------


void CViewContents::OnUpdateObjMoveUp(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	// Disable if contents are sorted!
	if (m_lvw.m_lngSortPropertyID == 0)
	{
		BObject* pobj = GetSelectedObject();
		if (pobj)
			bEnable = pobj->IsMoveUpDownValid(TRUE);
	}
	pCmdUI->Enable(bEnable);
}


void CViewContents::OnUpdateObjMoveDown(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	// Disable if contents are sorted!
	if (m_lvw.m_lngSortPropertyID == 0)
	{
		BObject* pobj = GetSelectedObject();
		if (pobj)
			bEnable = pobj->IsMoveUpDownValid(FALSE);
	}
	pCmdUI->Enable(bEnable);
}





void CViewContents::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	//trace("CViewContents::OnActivateView(bActivate=%d) - if true set focus to m_lvw\n", bActivate);

	CViewEx::OnActivateView(bActivate, pActivateView, pDeactiveView);

	// If this view is being activated, set the focus to the child control.
	// Note: CListCtrlEx's focus handler sets focus to first item if none selected.
	if (bActivate) 
	{
		m_lvw.SetFocus();
	}
}



void CViewContents::OnCmdEditPastePlain() 
{
	//trace("CViewContents::OnPastePlain\n");
	m_lvw.PasteClipboardToCurrentCell();
}



/*
// Disable the currently visible properties by setting their flagDisabled flag.
void CViewContents::DisableVisibleProperties()
{
	ASSERT_VALID(m_pDoc);

	// Clear all properties' flags
	EnableAllProperties();

	// Disable the currently visible properties
	int nColumns = m_lvw.GetColumnCount();
	for (int i = 0; i < nColumns; i++)
	{
		ULONG lngPropertyID = m_lvw.GetColumnPropertyID(i);
		BObject* pobjProp = m_pDoc->GetObject(lngPropertyID);
		pobjProp->SetFlag(flagDisabled, TRUE, FALSE);
	}	
}


// Enable all properties in the Properties system folder by clearing the flagDisabled flag.
void CViewContents::EnableAllProperties()
{
	ASSERT_VALID(m_pDoc);

	// Clear all properties' flags
	BObject* pobjProperties = m_pDoc->GetObject(folderProperties);
	pobjProperties->SetFlag(flagDisabled, FALSE, TRUE);
}
*/




// New item selected - notify document of the command target, and save current column. 
LRESULT CViewContents::OnLvwItemSelected(WPARAM wParam, LPARAM lParam)
{
	//trace("CViewContents::OnLvwItemSelected - set target object\n");
	// Only do this if this view is the active view - otherwise leads to problems moving items in the
	// treeview, etc. 
	if (this->IsActiveView())
		SetTargetObject();
	return 0;
}




// Edit the currently selected property value with a dialog.
// This is the F4 handler.
void CViewContents::OnObjEditInDialog() 
{
	//trace("CViewContents::OnObjEditInDialog\n");

	// Get command target
	BObject* pobj = m_pDoc->GetTargetObject();

	// Exit if no target object
//	if (pobj == 0) return;

	// Check if the command target is the new row
	if (pobj == (BObject*) CListCtrlEx::keyDummyRow)
	{
		// We're on the new row, so we'll want to add a new object, modify the dummy row, tell all views,
		// then re-add a new dummy row
		
		// Add new object of default class
		BObject* pobjParent = m_pDoc->GetCurrentObject();
		OBJID idClass = pobjParent->GetPropertyLink(propDefaultClass);
		BObject* pobjClass = m_pDoc->GetObject(idClass);
		CString strName;
		pobjClass->GetClassDefNewName(strName);
		BObject& obj = BObject::New(*m_pDoc, idClass, strName, pobjParent->id);

		// Add new object to listview
		//. lvw should handle removing and adding the dummy row
		m_lvw.RemoveDummyRow();
		m_lvw.AddObject(pobj);
		m_lvw.AddDummyRow();

	}
	ASSERT_VALID(pobj);

	// Get property to edit
	ULONG lngPropertyID = m_lvw.GetColumnPropertyID();

	// Edit the property value in an appropriate dialog.
//x	pobj->UIEditValue(lngPropertyID);
//x	app.ui.EditValue(pobj, lngPropertyID);
	pobj->UIEditValue(lngPropertyID, app.ui);

}



// Set command targets on gaining focus
void CViewContents::OnSetFocus(CWnd* pOldWnd) 
{
	//trace("CViewContents::OnSetFocus - set target object\n");
	CViewEx::OnSetFocus(pOldWnd);
	m_lvw.SetFocus(); // this will ensure that some cell is selected. // 1.1 added this so ctrl+enter would work right
	SetTargetObject();
}


void CViewContents::SetTargetObject()
{
	// Save selected object as command target
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();
//	if (pobj == (BObject*) CListCtrlEx::keyDummyRow) // catch this!
//		m_pDoc->SetTargetObject(0);
//	else
//		m_pDoc->SetTargetObject(pobj);
	m_pDoc->SetTargetObject(pobj);

	// Save current column for column command handlers
//	m_nTargetColumn = m_lvw.m_nCol; 
}















// Column Handlers
//---------------------------------------------------------------------------------------------------

// these functions have been moved to clistctrlex

/*
// Bring up column popup
// moved to clistctrlex::OnColumnRClick
void CViewContents::OnHeaderRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHEADER* phdr = (NMHEADER*) pNMHDR;

	CPoint ptScreen(::GetMessagePos()); // Get position of message source
	
	// Get column that was right clicked on and save to module level variable
	// Note: The help doesn't explain this but this does account for column reordering.
	// So if you moved the description column to the end, and rclicked on it there,
	// the index retrieved would be the same.
	CPoint ptClient = ptScreen;
	m_lvw.GetHeaderCtrl()->ScreenToClient(&ptClient);
	HDHITTESTINFO hti;
	hti.pt = ptClient;
	m_nTargetColumn = m_lvw.GetHeaderCtrl()->SendMessage(HDM_HITTEST, 0, (LPARAM) (LPHDHITTESTINFO) &hti);

	// Show popup menu
	CMenu menu;
	if (menu.LoadMenu(IDR_POPUP_CONTENTS_HEADER))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		if (pPopup)
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
	}
	*pResult = 0;
}



// Sort column contents ascending
void CViewContents::OnColumnSortAscending() 
{
	m_lvw.SortByColumn(m_nTargetColumn, 1);
}


// Sort column contents descending
void CViewContents::OnColumnSortDescending() 
{
	m_lvw.SortByColumn(m_nTargetColumn, -1);
}


// Edit the property associated with the specified column
void CViewContents::OnColumnEditProperty() 
{
	ULONG lngPropertyID = m_lvw.GetColumnPropertyID(m_nTargetColumn);
	BObject* pobjPropertyDef = m_pDoc->GetObjectNull(lngPropertyID);
	m_pDoc->UIEditPropertyDef(pobjPropertyDef);
}


// Insert a new column
void CViewContents::OnColumnInsert() 
{
	//trace("CViewContents OnColumnInsert\n");
	BObject* pobjCurrent = m_pDoc->GetCurrentObject();
	// First we want to disable all the currently visible properties
	DisableVisibleProperties();
	// Don't let user insert something before first column!
	// (otherwise the control gets screwy and the icon sticks with the new col 0 and the Name column has no icon!)
//	if (m_nTargetColumn == 0) 
//		m_nTargetColumn = 1;
	if (m_lvw.IndexToOrder(m_nTargetColumn) == 0)
		m_nTargetColumn = m_lvw.OrderToIndex(1);
	int nCol = m_lvw.InsertColumnAsk(pobjCurrent, m_nTargetColumn);
	EnableAllProperties();
}


// Delete a column
void CViewContents::OnColumnDelete() 
{
	if (m_nTargetColumn != -1)
	{
		m_lvw.RemoveColumn(m_nTargetColumn);
	}
}


// Clear any existing sort by reloading items
void CViewContents::OnColumnSortClear() 
{	
	// Remember current row data and column (if any)
	DWORD dwCurrentData = m_lvw.GetSelectedItemData();
	int nCurrentCol = m_lvw.m_nCol;

	// Add the children of the start object to the list, with no recursion
	m_lvw.DeleteAllItems();
	BObject* pobjStart = m_pDoc->GetCurrentObject();
	m_lvw.AddObjects(pobjStart, app.m_lngExcludeFlags, FALSE, FALSE, 0, TRUE);

	// Add dummy new row
	m_lvw.AddDummyRow();
	
	// Clear sort
	m_lvw.ClearSortMemory();

	// Reselect item
	if (dwCurrentData != 0)
	{
		int nRow = m_lvw.FindItemData(dwCurrentData);
		m_lvw.SelectCell(nRow, nCurrentCol);
	}
}


void CViewContents::OnUpdateColumnSortClear(CCmdUI* pCmdUI) 
{
	// Should only be enabled if autosort is OFF for parent
	// (because in that case, user can move items up and down, and will need to be able to
	// turn sort off in order to do that).
	// Also only enable if contents are sorted
	BObject* pobjStart = m_pDoc->GetCurrentObject();
	BOOL bAutosort = !(pobjStart->GetFlag(flagNoAutosort));
	BOOL bEnable = (bAutosort == FALSE) && (m_lvw.m_lngSortPropertyID != 0);
	pCmdUI->Enable(bEnable);
}


// Save column info when columns have changed
// This is the CM_COLUMNS_CHANGED handler, sent by clistctrlex
LRESULT CViewContents::OnLvwColumnsChanged(WPARAM wParam, LPARAM lParam)
{
	//trace("CViewContents CM_COLUMNS_CHANGED handler - save column info\n");
	BObject* pobj = m_pDoc->GetCurrentObject();
	ASSERT_VALID(pobj);
	m_lvw.SaveColumnInfo(pobj); // this saves the BDataColumns object to the current pobj
	// Set flag so we know we're no longer responsible for deleting the BDataColumns object.
//	m_bColumnsSaved = TRUE;
	return 0;
}

*/







// Add a new object to the contents view.
// Note: we ignore the command target for this command, as no matter what object is selected
// in the contents view, we want to add the new object as a child of the doc's current object.
void CViewContents::OnObjAdd() 
{
	HOBJECT hobjParent = m_pDoc->GetCurrentObject();
	ASSERT_VALID(hobjParent);
	// Bring up add new dialog
	// Add object but don't select it as the current object in the data view
	HOBJECT hobjNew = m_pDoc->UIAddNewObject(hobjParent, FALSE);
	if (hobjNew)
	{
		// Select the new object in this view
		ASSERT_VALID(hobjNew);
		int nNewItem = m_lvw.FindItemData((LPARAM) hobjNew);
		if (nNewItem != -1)
			m_lvw.SelectItem(nNewItem);
	}
}


// Add a new folder to this view
void CViewContents::OnObjAddFolder() 
{
	BObject* pobjParent = m_pDoc->GetCurrentObject();
	ASSERT_VALID(pobjParent);
	BObject* pobjNew = m_pDoc->UIAddNewFolder(pobjParent, FALSE);
	if (pobjNew)
	{
		// Select the new object in this view
		ASSERT_VALID(pobjNew);
		int nNewItem = m_lvw.FindItemData((LPARAM) pobjNew);
		if (nNewItem != -1)
			m_lvw.SelectItem(nNewItem);
	}
}



void CViewContents::OnNavigateFollowLink() 
{
	// If user is on name property, open that object
	// If on a link property, follow that link
	// If user hit enter on the item in column 0, select it
	m_lvw.FollowLink(this);
	// at this point the contents view might not even be visible since we're on a different object
	// so where do you put the focus?
//	m_lvw.SetFocus(); //dont work
}


void CViewContents::OnUpdateNavigateFollowLink(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(m_lvw.CanFollowLink());	
}



void CViewContents::OnObjMoveTo() 
{
	m_pDoc->UIMoveObjectTo();
}

void CViewContents::OnUpdateObjMoveTo(CCmdUI* pCmdUI) 
{
}


/*
void CViewContents::OnUpdateColumnInsert(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
*/




BOOL CViewContents::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// Add our list view control to message routing.
	// Should return True if the message is handled; otherwise False.
	// Note: It is not necessary to implement an ON_UPDATE_COMMAND_UI handler to enable
	// or disable the menu item.  
	if (m_lvw.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// If the object(s) in the extended command route don't handle
	// the command, then let the base class OnCmdMsg handle it.
	return CViewEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}






// Printing
//---------------------------------------------------------------------------

BOOL CViewContents::DoPrint(BOOL bPreview) {
	if (bPreview)
		CViewEx::OnFilePrintPreview(); // in cviewex
	else
		CViewEx::OnFilePrint(); // in file viewprnt.cpp
	return TRUE;
}



BOOL CViewContents::OnPreparePrinting(CPrintInfo* pInfo) {

	//trace("CViewContents::OnPreparePrinting\n");
	
	// If your application knows how many pages the document contains when OnPreparePrinting
	// is called, it should inform MFC by calling CPrintInfo::SetMaxPage before calling
	// DoPreparePrinting. MFC displays the maximum page number in the To box of the Print dialog.
	// If you don't set the maximum page number in OnPreparePrinting, you should set it in
	// OnBeginPrinting if possible. (better there because at this point you don't know what
	// printer the user has chosen)
//	pInfo->SetMaxPage(nMaxPage);

	// A nonzero return from OnPreparePrinting begins the printing process, and a 0 return
	// cancels the print job before it begins. 
	return DoPreparePrinting(pInfo); // default preparation - brings up dialog
}


// At this point the printer has been selected so we can calculate the max number of pages. 
// Consider printing a spreadsheet. When overriding OnPreparePrinting, you must calculate 
// how many sheets of paper will be required to print the entire spreadsheet and then 
// use that value when calling the SetMaxPage member function of CPrintInfo. 
void CViewContents::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {

	// Initialize fonts margins etc
	CViewEx::OnBeginPrinting(pDC, pInfo);

	app.m_printinfo.m_strViewName = _T("Contents View");
	app.m_printinfo.m_strObjectName = m_pDoc->GetCurrentObject()->GetPropertyString(propName);

	// Get grid information etc
	m_lvw.OnBeginPrinting(pDC, pInfo);
}


/*
// As a rule, you need to override OnPrepareDC only if you use OnDraw to draw to both the screen 
// and the printed page. If you do all your printing from OnPrint, there's no need to override OnPrepareDC.
// When overriding OnPrepareDC, you must translate m_nCurPage into the range of rows 
// and columns that will appear on that particular sheet and then adjust the viewport origin accordingly.
// After each call to OnPrepareDC, the framework checks a member of the CPrintInfo structure 
// called m_bContinuePrinting. Its default value is TRUE. As long as it remains so, the framework 
// continues the print loop. If it is set to FALSE, the framework stops. To perform print-time pagination, 
// override OnPrepareDC to check whether the end of the document has been reached, and 
// set m_bContinuePrinting to FALSE when it has. 
// The default implementation of OnPrepareDC sets m_bContinuePrinting to FALSE if the current page 
// is greater than 1. This means that if the length of the document wasn’t specified, the framework 
// assumes the document is one page long. One consequence of this is that you must be careful if you 
// call the base class version of OnPrepareDC. Do not assume that m_bContinuePrinting will be TRUE 
// after calling the base class version. 
void CViewContents::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)  {
	// TODO: Add your specialized code here and/or call the base class	
	CViewEx::OnPrepareDC(pDC, pInfo);
}
*/


// This is where we draw the actual text, since we're not using OnDraw to draw to both
// the screen and the printer. 
// Note: The default implementation of OnPrint just calls OnDraw.
// To determine which page OnPrint has been called to print, check CPrintInfo::m_nCurPage.
// If you need to discriminate between real printing and print preview, check the m_bPreview
// data member of pInfo. 
// If you need a GDI object for several consecutive pages, Windows requires that you select it 
// into the device context each time OnPrint is called.
// TheOnPrint member function is the appropriate place to print headers or footers because it 
// is called for each page, and because it is called only for printing, not for screen display. You 
// can define a separate function to print a header or footer, and pass it the printer device 
// context from OnPrint. 
void CViewContents::OnPrint(CDC* pDC, CPrintInfo* pInfo) {

	CPrintInfoMore& rpim = app.m_printinfo;

	rpim.PrintHeader(pDC, pInfo);
	m_lvw.PrintPage(pDC, pInfo);
	rpim.PrintFooter(pDC, pInfo);
	rpim.DrawMargins(pDC, pInfo);
}


/*
// When the user closes the print preview window, the framework calls OnEndPrintPreview
// to notify the application that print preview is about to end. The default implementation
// calls OnEndPrinting, reactivates the original view, and destroys the print preview window. 
void CViewContents::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) {
	// TODO: Add your specialized code here and/or call the base class	
	CViewEx::OnEndPrintPreview(pDC, pInfo, point, pView);
}
*/

// Free any fonts and other resources allocated in OnBeginPrinting.
void CViewContents::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {
	//trace("CViewContents::OnEndPrinting\n");
	// call base class to delete gdi objects
	CViewEx::OnEndPrinting(pDC, pInfo);
}

