
// CViewTree



#include "precompiled.h"

#include "ViewTree.h"
#include "Constants.h"
#include "DialogEditLink.h"
#include "DialogEditName.h"
#include "DialogEditObject.h"
#include "Hint.h"
#include "FrameChild.h"
#include "NeoMem.h"
#include "RichEditCtrlEx.h"

#include "ConstantsDatabase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// Macros
//---------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CViewTree, CViewEx)

BEGIN_MESSAGE_MAP(CViewTree, CViewEx)
	//{{AFX_MSG_MAP(CViewTree)
	ON_WM_CREATE()
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TVW_DATA, OnEndLabelEdit)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TVW_DATA, OnSelChanged)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TVW_DATA, OnBeginLabelEdit)
	ON_NOTIFY(NM_RCLICK, IDC_TVW_DATA, OnRClick)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TVW_DATA, OnItemExpanding)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TVW_DATA, OnItemExpanded)
	ON_COMMAND(ID_OBJ_EDIT_IN_PLACE, OnObjEditInPlace) //ok
	ON_COMMAND(ID_OBJ_SORT_CHILDREN, OnObjSortChildren)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEdit)
	ON_COMMAND(ID_EDIT_COPY, OnCmdEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnCmdEditCut)
	ON_COMMAND(ID_EDIT_PASTE_PLAIN, OnCmdEditPastePlain)
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_PLAIN, OnUpdateEdit)
	ON_COMMAND(ID_OBJ_MOVE_TO, OnObjMoveTo)
	ON_UPDATE_COMMAND_UI(ID_OBJ_MOVE_TO, OnUpdateObjMoveTo)
	ON_COMMAND(ID_POPUP_EXPORT, OnCmdFileExport)
	//}}AFX_MSG_MAP
//	ON_NOTIFY(TVN_BEGINDRAG, IDC_TVW_DATA, CViewEx::OnBeginDrag)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TVW_DATA, OnBeginDrag)
	ON_NOTIFY(TVN_KEYDOWN, IDC_TVW_DATA, OnNotifyKeyDown)
//	ON_NOTIFY(TVN_BEGINRDRAG, IDC_TVW_DATA, OnBeginRDrag)
//	ON_COMMAND(ID_FILE_PRINT, CViewEx::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CViewEx::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CViewEx::OnFilePrintPreview)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()




// Construction/Destruction
//---------------------------------------------------------------------------

CViewTree::CViewTree()
{
	m_bTempSelection = FALSE;
	m_htiDragOver = 0;
	m_htiSaveDuringRename = 0;
//	m_htiPopup = 0;
//	m_pobjPopup = NULL;
//	m_pDoc = NULL;
	m_pobjDragObject = NULL;
}

CViewTree::~CViewTree()
{
}


// Create the child controls.
int CViewTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CViewEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//. put these as defaults in Ex class?
	if (m_tvw.Create(
				WS_CHILD | 
				WS_VISIBLE |
//				WS_TABSTOP | 
				TVS_NOTOOLTIPS | // Turn OFF tooltips!!
				TVS_EDITLABELS |	// Allows the user to edit the labels of tree view items. 
				TVS_HASBUTTONS |	// Displays plus (+) and minus (-) buttons next to parent items. 
//				TVS_LINESATROOT	|	// draw lines from root
//				TVS_SINGLEEXPAND |	// single click expand
//				TVS_INFOTIP |		// Version 4.71. The tree view control will send the TVN_GETINFOTIP notification to obtain tooltip information. 
				TVS_SHOWSELALWAYS |	// Causes a selected item to remain selected when the tree view control loses focus. 
				TVS_HASLINES |		// Uses lines to show the hierarchy of items.
//				TVS_FULLROWSELECT |	// Select using entire row (can't be used with HASLINES)
				0,
				CRect(0,0,0,0), 
				this, 
				IDC_TVW_DATA
				))
	{
		// Tell CViewEx about child window
		SetChildView(&m_tvw);
	}
	else return -1;

//	m_tvw.SetFont(&theApp.m_fontControls);

	// Register this window as an OLE drop target
	//. could also specify borders in init
	CViewEx::DragRegister(&m_tvw); // pass scroll window

	// Get document
//	m_pDoc = GetDocument();

	// Set image list
	CImageList* pImageList = m_pDoc->GetImageList();
	ASSERT (pImageList);
	m_tvw.SetImageList(pImageList, TVSIL_NORMAL);

	// Set number of pixels to indent
//	m_tvw.SetIndent(5);	

	// Load custom accelerators for this view
	LoadViewAccelerators(IDR_VIEW_DATA);

	return 0;
}



// Diagnostics
//---------------------------------------------------------------------------

#ifdef _DEBUG
void CViewTree::AssertValid() const
{
	CViewEx::AssertValid();
}

void CViewTree::Dump(CDumpContext& dc) const
{
	CViewEx::Dump(dc);
}

#endif //_DEBUG




// Printing
//---------------------------------------------------------------------------

/*
void CViewTree::OnFilePrintPreview()
{
	// call child frame handler, which will pass it on to the rtf view handler
	CFrameChild* pFrame = (CFrameChild*) GetParentFrame();
	pFrame->SendMessage(WM_COMMAND, ID_FILE_PRINT_PREVIEW);
}

BOOL CViewTree::OnPreparePrinting(CPrintInfo* pInfo)
{
	xTRACE("CViewTree OnPreparePrinting\n");
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CViewTree::OnBeginPrinting(CDC*, CPrintInfo*)
{
	xTRACE("CViewTree OnBeginPrinting\n");
	// TODO: add extra initialization before printing
}

void CViewTree::OnEndPrinting(CDC*, CPrintInfo*)
{
	xTRACE("CViewTree OnEndPrinting\n");
	// TODO: add cleanup after printing
}
*/



// Message Handlers
//---------------------------------------------------------------------------


void CViewTree::OnInitialUpdate()
{
	// The default implementation of this function calls the OnUpdate member 
	// function with no hint information (that is, using the default values 
	// of 0 for the lHint parameter and NULL for the pHint parameter). 
//	CViewEx::OnInitialUpdate();
}



// Document has changed - update the treeview to reflect those changes
void CViewTree::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	TRACE("    CViewTree::OnUpdate %s\n", theApp.GetHintName(lHint));
//	xTRACE("CViewTree::OnUpdate(lHint=%d, pHint=0x%pL)\n", lHint, pHint);
//	CWaitCursor cw;

	// To use lHint, define special hint values, typically a bitmask or an enumerated type, 
	// and have the document pass one of these values. To use pHint, derive a hint class from 
	// CObject and have the document pass a pointer to a hint object; when overriding OnUpdate, 
	// use the CObject::IsKindOf member function to determine the run-time type of the hint object.
	// If lHint is 0 and pHint is NULL, the document has sent a generic update notification. 
	// If a view receives a generic update notification, or if it cannot decode the hints, 
	// it should invalidate its entire client area.

	//, Need this for release version?
	if (m_pDoc == 0) return;

	switch (lHint)
	{
		case hintLoadTree:
			{
				// Reload the entire treeview here, starting with the specified object.
				// pHint contains a pointer to the starting bobject.
				// Walk through bobjects in document, adding children recursively.
				// or could add children dynamically later as items are expanded.
				BObject* pobjStart = (BObject*) pHint;
				ASSERT_VALID(pobjStart);

				// Default is to show the root node.
				// If we're loading the main root, see if we're admin or not.
				BOOL bIncludeStart = TRUE;
				if (pobjStart->GetObjectID() == rootMain)
				{
					bIncludeStart = (theApp.m_bAdmin);
				}

				// Add objects
				ULONG lngExcludeFlags = theApp.m_lngExcludeFlags;
				m_tvw.AddObjects(pobjStart, lngExcludeFlags, bIncludeStart, TRUE, TRUE);

				// Select current item
				xTRACE("  select current item in tree\n");
				// this should generate the selchanged event
				// why would this not generate the selchange event if it wasn't on the doc root object?
				HTREEITEM htiCurrent = m_tvw.FindItemData((DWORD) m_pDoc->GetCurrentObject(), 0);
				m_tvw.Select(htiCurrent, TVGN_CARET);
//,				BObject* pobjCurrent = m_pDoc->GetCurrentObject();
//				m_tvw.SelectItemData((DWORD)pobjCurrent);

				break;
			}
			
		case hintSelect:
			{
				// Select the specified object in the treeview.
				// This will happen when the user double clicks on an object in the contents, 
				// for instance.

				// Note: If we receive this event and this view is not the current view on the left side,
				// ignore it, as the Search view or Index view is active and we don't want to wind
				// up expanding all the items in the Data view.
				// On selecting the data view tab though, we'll need to select the current object
				// if the tree's selection is different.
//				if (IsWindowVisible())
				if (IsWindowEnabled())
				{
					//, you also might have the case that in the future we dynamically load
					// items into the tree, so the bobject in question might not even be
					// loaded, in which case you'd need to add it into the tree at the
					// appropriate place
					BObject* pobjStart = (BObject*) pHint;
					ASSERT_VALID(pobjStart);

					HTREEITEM htiSelect = m_tvw.FindItemData((DWORD) pobjStart, 0);
					if (htiSelect)
					{
						// Make item visible and select it.
						// This generates the selchanged event, which calls doc.SelectCurrentObject,
						// which broadcasts hintSave, hintSelect, and hintLoad
						m_tvw.Select(htiSelect, TVGN_CARET);
	//					m_tvw.SelectItem(htiSelect);
					}
				}
				break;
			}

/*
		case hintReloadChildren:
			{
				// Find the specified item in the tree
				BObject* pobjStart = (BObject*) pHint;
				ASSERT_VALID(pobjStart);

				HTREEITEM htiParent = m_tvw.FindItemData((DWORD) pobjStart, 0);
				if (htiParent)
				{
					// Delete all the children of the selected item
					m_tvw.DeleteChildren(htiParent);

					// Now reload the child objects, if any, in the tree control
					if (pobjStart->m_paChildren)
						AddChildrenToTree(pobjStart->m_paChildren, htiParent, (pobjStart->GetFlag(flagNoAutosort)));
				}
			}
*/

		case hintAdd:
			{
				// Add a node to the treeview at the specified location (parent)				
				BObject* pobjNew = (BObject*) pHint;
				ASSERT_VALID(pobjNew);
				BObject* pobjParent = pobjNew->GetParent();
				ASSERT_VALID(pobjParent);

				// Only add the item if the parent is in the view (this could happen if you're in user mode
				// and you add a new property, for instance).
				HTREEITEM htiParent = m_tvw.FindItemData((DWORD) pobjParent, 0);
				if (htiParent)
				{
					m_tvw.AddObject(pobjNew, pobjParent);
					// Now sort the siblings alphabetically if autosort is on for parent
					if (pobjParent->IsSorted())
						m_tvw.SortByProperty(htiParent, propName);
				}
/*					
				// Resolved: Prob - if we're adding to the currently selected item, it doesn't get the + until
				// you move off of it. why is that?
				// Needed to refresh the parent - this seems to do the trick
				m_tvw.SetItemData(htiParent, (DWORD) pobjParent);
				// Note: we only want to select the new item here if it was added from the treeview
				// Therefore, the selection was moved to the tree view add code
//				m_tvw.SelectItem(htiNew);
*/									
				break;
			}

		case hintDelete:
			{
				// Delete items from the treeview
				CHint* pobjHint = (CHint*) pHint;
				BObjects* paObjects = pobjHint->m_paObjects;
				ASSERT_VALID(paObjects);
				int nItems = paObjects->GetSize();
				for (int i = 0; i < nItems; i++)
				{
					// Note: If you delete the currently selected item the treeview will automatically
					// select another item, which triggers the selchange event, which 
					// sends hintSave, hintSelect, and hintLoad
					// since the current object has been deleted, hintSave will fail
					BObject* pobj = (BObject*) paObjects->GetAt(i);
					// Note: pobj might be an invalid bobject because it's already been deleted
					// by the time this gets called, so don't do assert on it
					m_tvw.DeleteItemData((DWORD) pobj);
				}
				break;
			}

		case hintPropertyChange:
			{
				// The only properties we need to worry about in this view are Name, IconID, ClassID, ClassName

				CHint* pobjHint = (CHint*) pHint;
				ASSERT_VALID(pobjHint);

				// Check if the object is in this view
				BObject* pobj = pobjHint->m_pobjObject;
				ASSERT_VALID(pobj);

				HTREEITEM hti = m_tvw.FindItemData((DWORD) pobj, 0);
				if (hti)
				{
					// Handle different property changes differently
					ULONG lngPropertyID = pobjHint->m_idProperty;
					if (lngPropertyID == propIconID || lngPropertyID == propClassID || lngPropertyID == propClassName)
					{
						// Get the new index of the icon in the image list
						int nImage = pobj->GetIconIndex();
						m_tvw.SetItemImage(hti, nImage, nImage);
					}
					if (lngPropertyID == propName)
					{
						// Since the text is handled by a callback, just reset the item to refresh it
						m_tvw.SetItem(hti, TVIF_TEXT, LPSTR_TEXTCALLBACK, 0, 0, 0, 0, 0);

						// Resort siblings if autosort is on for this item's parent
						if (pobj->IsParentSorted())
						{
							HTREEITEM htiParent = m_tvw.GetParentItem(hti);
							if (htiParent) 
								m_tvw.SortChildren(htiParent);
						}
					}
				}
				// Bug: Forgot to put break here - bad
				break;
			}

		case hintMoveObject:
			{
				// Move object in the treeview to a new parent object

				BObject* pobj = (BObject*) pHint;
				ASSERT_VALID(pobj);

				BObject* pobjNewParent = pobj->GetParent();
				ASSERT_VALID(pobjNewParent);

				// Note: htiParentNew might be 0 if the new parent object is not in this view
				HTREEITEM htiNewParent = m_tvw.FindItemData((DWORD) pobjNewParent, 0);

				// Note: htiObject might be 0 if the object is not in this view
				HTREEITEM htiObject = m_tvw.FindItemData((DWORD) pobj, 0);
				if (htiObject)
				{
					// If the new parent is in this view, move the item to it, otherwise delete the item
					if (htiNewParent)
						m_tvw.MoveItem(htiObject, htiNewParent);
					else
						m_tvw.DeleteItem(htiObject);
				}

				// Now resort the new parent if autosort is on
				if (pobjNewParent->IsSorted())
					m_tvw.SortChildren(htiNewParent);

				// Make sure the destination is expanded
//				m_tvw.Expand(htiNewParent, TVE_EXPAND);
				// Copy to the new location
//				HTREEITEM htiNew = m_tvw.CopyBranch(m_htiDragItem, m_htiDropItem, TVI_LAST);
				// Delete the old item and select the new item
//				m_tvw.DeleteItem(m_htiDragItem);
//				m_tvw.SelectItem(htiNew);

				break;
			}

		case hintReposition:
			{
				// Move item up or down
				// Move pobjObject before or after pobjTarget, depending on bAfter flag.
				CHint* ph = (CHint*) pHint;
				BObject* pobjSource = ph->m_pobjObject;
				BObject* pobjTarget = ph->m_pobjTarget;
				BOOL bAfter = ph->m_bAfter;
				ASSERT_VALID(pobjSource);
				ASSERT_VALID(pobjTarget);

				// First make sure the target object is visible in this view
				HTREEITEM htiTarget = m_tvw.FindItemData((DWORD) pobjTarget, 0);
				if (htiTarget)
				{
					HTREEITEM htiSource = m_tvw.FindItemData((DWORD) pobjSource, 0);
					ASSERT(htiSource);
					ASSERT(htiTarget);

					// Need to know if the currently selected object is the source being repositioned,
					// as it will be deleted and we'll need to reselect it if so.
					BOOL bSourceIsCurrent = (m_pDoc->GetCurrentObject() == pobjSource);

					//. only prob is when you delete the source, since it's selected,
					// the treeview selects its parent, causing contents to be refreshed
					// to prevent this you could select the copy before deleting the source
					m_tvw.LockWindowUpdate();
					HTREEITEM htiSourceNew = m_tvw.MoveItemToSibling(htiSource, htiTarget, bAfter);
					m_tvw.UnlockWindowUpdate();

					// Reselect the source if it was the current object on entry
					if (bSourceIsCurrent)
						m_tvw.SelectItem(htiSourceNew);
				}

				break;
			}

		case hintRefresh:
			{
				// Redraw the entire window to redisplay names
				m_tvw.RedrawWindow();
				break;
			}

		case hintResortChildren:
			{
				BObject* pobjParent = (BObject*) pHint;
				ASSERT_VALID(pobjParent);
				HTREEITEM htiParent = m_tvw.FindItemData((DWORD) pobjParent, 0);
				if (htiParent)
					m_tvw.SortByProperty(htiParent, propName);
				break;
			}

		case hintResortAllChildren:
			{
				// walk through all items in tree and resort children if autosort is on
				HTREEITEM hti = m_tvw.GetFirstVisibleItem();	
				ResortChildren(hti); // recurses downwards through treeview
				break;
			}

		case hintRefreshAllIcons:
			{
				// Walk through all items, reobtaining their icons
				HTREEITEM hti = m_tvw.GetFirstVisibleItem();	
				RefreshIcons(hti); // recurses downwards through treeview
				// Now refresh window to make sure the new icons get displayed
				m_tvw.RedrawWindow();
				break;
			}

//		default:
			// Call base class (invalidates entire client area)
//			CViewEx::OnUpdate(pSender, lHint, pHint);
	//		break;
	}
	
}


// Starting with the specified item, recurse downwards through the treeview and
// walk through all siblings.
void CViewTree::RefreshIcons(HTREEITEM hti)
{
	while (hti)
	{
		BObject* pobj = (BObject*) m_tvw.GetItemData(hti);
		if (pobj)
		{
			ASSERT_VALID(pobj);
			int nIcon = pobj->GetIconIndex();
			m_tvw.SetItemImage(hti, nIcon, nIcon);
		}
		// Update all children of this item recursively
		HTREEITEM htiChild = m_tvw.GetChildItem(hti);
		if (htiChild)
		{
			RefreshIcons(htiChild);
		}
		hti = m_tvw.GetNextSiblingItem(hti);
	}
}


// Resort children of given item recursively
//, shouldn't use sortchildren because it doesn't use our callback
//, better to put this in treeview class also - pass bRecurse param
void CViewTree::ResortChildren(HTREEITEM hti)
{
	while (hti)
	{
		BObject* pobj = (BObject*) m_tvw.GetItemData(hti);
		if (pobj)
		{
			ASSERT_VALID(pobj);
			if (pobj->IsParentSorted())
				m_tvw.SortChildren(hti);
		}
		// Update all children of this item recursively
		HTREEITEM htiChild = m_tvw.GetChildItem(hti);
		if (htiChild)
		{
			ResortChildren(htiChild);
		}
		hti = m_tvw.GetNextSiblingItem(hti);
	}
}



// Selection in the treeview changed, so tell Document and other Views
void CViewTree::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	xTRACE("CViewTree::OnSelChanged!\n");
	xTRACE("  item old: hti %d lparam %d\n", pNMTreeView->itemOld.hItem, pNMTreeView->itemOld.lParam);

	// Get newly selected item
	TVITEM itemNew = pNMTreeView->itemNew;
	BObject* pobj = (BObject*) itemNew.lParam;
	// 1.1d adding if(pobj), since this assertion might be failing sometimes (see bug 3982)
	// Of course, this is not the actual bug - that remains to be found. 
	if (pobj)
	{
		ASSERT_VALID (pobj);

		// First check if object is the same as current object - if so then ignore this
	//	if (pobj == m_pDoc->GetCurrentObject())
	//		return;

		// If this was triggered by SetCurrentObject then exit (don't need to call it again!)
		if (m_pDoc->m_bSettingCurrentObject) return;

		// If in process of renaming item, don't update current item
		if (m_bTempSelection) return;

		// Tell document that selection has changed (will broadcast UpdateAllViewsEx also)
		m_pDoc->SetCurrentObject(pobj, this);
	}
}





// Rename the selected item
void CViewTree::OnObjEditInPlace() 
{
	xTRACE("CViewTree:OnObjEditInPlace()\n");
//	ASSERT(m_htiPopup);

	// Note: This mimics Explorer behavior - you can rename an item without changing the current selection.
	// The flag tells OnSelChanged to ignore the new selection, and code in OnEndLabelEdit will set
	// the selection back to the original selection.
	BObject* pobjTarget = m_pDoc->GetTargetObject();
	HTREEITEM htiTarget = m_tvw.FindItemData((DWORD) pobjTarget);
	if (htiTarget) // just in case it's zero
	{
		m_htiSaveDuringRename = m_tvw.GetSelectedItem();
		m_bTempSelection = TRUE;
		if (m_tvw.SelectItem(htiTarget))
		{
			// need to subclass this to get it to swallow mouse clicks?
			m_tvw.EditLabel(htiTarget);
		}
	}

	// See OnEndLabelEdit for reselection code
}


// Allow or disallow label editing here
// When label editing begins, a tree control sends a TVN_BEGINLABELEDIT notification 
// message. By processing this notification, you can allow editing of some labels 
// and prevent editing of others. Returning 0 allows editing, and returning nonzero prevents it. 
void CViewTree::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	xTRACE("CViewTree::OnBeginLabelEdit\n");
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	//. check here if the object text is read-only or not
	*pResult = 0;
}

	
// User renamed an item in the treeview
// Need to update the document, setmodifiedflag, updateallviewsEx
//. If the pszText member is non-NULL, return TRUE to set the item's label to the 
// edited text. Return FALSE to reject the edited text and revert to the original label. 
void CViewTree::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	xTRACE("CViewTree::OnEndLabelEdit\n");
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	TVITEM* pTVITEM = &(pTVDispInfo->item);
	
	if (pTVITEM->pszText != NULL)
	{
		// Save the new name
		HTREEITEM hti = pTVITEM->hItem;
		BObject* pobj = (BObject*) m_tvw.GetItemData(hti);
		ASSERT_VALID (pobj);
		pobj->SetPropertyText(propName, pTVITEM->pszText);

		// Resort siblings if autosort is on for this item's parent
		if (pobj->IsParentSorted())
		{
			HTREEITEM htiParent = m_tvw.GetParentItem(hti);
			if (htiParent) 
				m_tvw.SortByProperty(htiParent, propName);
		}

		*pResult = TRUE;
	}
	else
	{
		*pResult = 0;
	}

	// Reselect previously selected item if this was initiated through popup menu.
	// See OnObjEditInPlace.
	if (m_bTempSelection)
	{
		m_bTempSelection = FALSE;
		if (m_htiSaveDuringRename)
			VERIFY(m_tvw.SelectItem(m_htiSaveDuringRename));
	}

}


/*
// A tree view control allocates memory for storing each item; the text of the item 
// labels takes up a significant portion of this memory. If your application maintains 
// a copy of the strings in the tree view control, you can decrease the memory requirements 
// of the control by specifying the LPSTR_TEXTCALLBACK value in the pszText member of 
// TVITEM instead of passing actual strings to the tree view. Using LPSTR_TEXTCALLBACK 
// causes the tree view control to retrieve the text of an item's label from the parent 
// window whenever the item needs to be redrawn. To retrieve the text, the tree view 
// control sends a TVN_GETDISPINFO notification message, which includes the address of 
// a NMTVDISPINFO structure. The parent window must fill the appropriate members of 
// the included structure. 
// Note: Seems that this gets called once for each type of info, ie. once for text and once for children info
// Therefore, used an else if structure below.
void CViewTree::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{

	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	TVITEM* pTVITEM = &(pTVDispInfo->item);
	
	UINT intMask = pTVITEM->mask;
	HTREEITEM hti = pTVITEM->hItem;

	BObject* pobj = (BObject*) m_tvw.GetItemData(hti);
	ASSERT_VALID(pobj);

	// Depending on what information is being requested, return different info
	if (intMask & TVIF_TEXT)
		// had been using GetBuffer which slowed the program down ENORMOUSLY, as it wound up copying the strings
		pTVITEM->pszText = (TCHAR*) pobj->GetPropertyText(propName); //, note cast to tchar from const

	else if (intMask & TVIF_CHILDREN)
		pTVITEM->cChildren = pobj->HasChildren();

	*pResult = 0;
}
*/


// A tree control sends a TVN_ITEMEXPANDING notification message when a parent item's 
// list of child items is about to be expanded or collapsed. 
// The notification gives you 
// the opportunity to prevent the change or to set any attributes of the parent item that 
// depend on the state of the list of child items. After changing the state of the list, 
// the tree control sends a TVN_ITEMEXPANDED notification message.
//, this is where you could dynamically add child items to the tree
//, also can prevent item from expanding here
void CViewTree::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;
}



// Save expand/collapse state to BObject
void CViewTree::OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM* ptvi = &pNMTreeView->itemNew;
	BObject* pobj = (BObject*) ptvi->lParam;
	if (pNMTreeView->action == TVE_COLLAPSE)
	{
		HTREEITEM hti = ptvi->hItem;

		// Set flag for this and all child objects
		pobj->SetFlag(flagExpanded, FALSE, TRUE);
		
		// Tell tree to collapse all children also
		m_tvw.ExpandChildren(hti, TVE_COLLAPSE, TRUE);
	}
	else
	{
		pobj->SetFlag(flagExpanded, TRUE);
	}
	*pResult = 0;
}


void 
CViewTree::OnSize(UINT nType, int cx, int cy) 
{
	CViewEx::OnSize(nType, cx, cy);
}





// Popup Menu handlers
//---------------------------------------------------------------------------

// OnRClick
// Handle right click with context menu handler
//, why did I have this? it was causing duplicate context menus to popup
void CViewTree::OnRClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	xTRACE("CViewTree OnRClick\n");
//	CPoint point(::GetMessagePos()); // screen coords
//	OnContextMenu(this, point);
	*pResult = 0;
}



// Bring up context menu for tree
void CViewTree::OnContextMenu(CWnd* pWnd, CPoint ptScreen)
{
	xTRACE("CViewTree OnContextMenu %d  %d\n", ptScreen.x, ptScreen.y);

	HTREEITEM hti = 0;

	// If user pressed context menu key ("Application Key") x and y will be -1; just use current item
	if (ptScreen.x == -1 && ptScreen.y == -1)
	{
		hti = m_tvw.GetSelectedItem();
		CRect r;
		if (m_tvw.GetItemRect(hti, &r, TRUE))
		{
			ClientToScreen(&r);
			ptScreen = r.CenterPoint();
		}
	}
	else
	{
		// Convert screen coordinates to client coordinates
		CPoint ptClient = ptScreen;
		CWnd::ScreenToClient(&ptClient);

		// See if user clicked on a node in the tree
		// Note: Flags will contain info on where exactly the user clicked (on icon, on label, etc)
		UINT nFlags;
		hti = m_tvw.HitTest(ptClient, &nFlags);
	}

	if (hti)
	{
		// Get bobject
		BObject* pobj = (BObject*) m_tvw.GetItemData(hti);

		// Set as target object
		m_pDoc->SetTargetObject(pobj);

		// Save original selection and move to temp selection
		m_bTempSelection = TRUE;
		HTREEITEM htiSave = m_tvw.GetSelectedItem();
		m_tvw.SelectItem(hti);
		m_bTempSelection = FALSE;

		// Display pop up menu
//		CMenu menu;
//		if (menu.LoadMenu(IDR_POPUP_TREE))
//		{
//			CMenu* pPopup = menu.GetSubMenu(0);
//			ASSERT(pPopup);
			BCMenu* pPopup = InitPopup(IDR_POPUP_TREE);
			if (pPopup)
			{			
				//, add dynamic menu items (class specific actions)

				// Check high priority item if flag is set for the object
//				BOOL bHighPriority = m_pobjPopup->GetFlag(flagHighPriority);
//				UINT nCheck = MF_BYCOMMAND | bHighPriority ? MF_CHECKED : MF_UNCHECKED;
//				pPopup->CheckMenuItem(ID_OBJ_PRIORITY_HIGH, nCheck);

//				pPopup->SetDefaultItem(ID_OBJ_OPEN);
//				BOOL bNoModifyClass = m_pobjPopup->GetFlag(flagNoModifyClass);

				BOOL bAutosort = pobj->IsSorted();
				pPopup->CheckMenuItem(ID_OBJ_AUTOSORT, bAutosort ? MF_CHECKED : MF_UNCHECKED);
//				pPopup->EnableMenuItem(ID_OBJ_SORT_CHILDREN, bAutosort ? MF_DISABLED : MF_ENABLED); // enable if autosort is off
				pPopup->RemoveMenu(ID_OBJ_SORT_CHILDREN, MF_BYCOMMAND); 
//				pPopup->RemoveMenu(ID_OBJ_AUTOSORT, MF_BYCOMMAND); //, for now just hide this item
//				pPopup->EnableMenuItem(ID_OBJ_CHANGE_OBJECT_CLASS, bNoModifyClass ? MF_GRAYED : MF_ENABLED);
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
			}
//		}

		// Restore original selection
		m_bTempSelection = TRUE;
		m_tvw.SelectItem(htiSave);
		m_bTempSelection = FALSE;
	}

}



// Sort the children of the selected object
// Note: The object that was right clicked on is stored in the mpobjPopup variable.
// For now this just sorts the view and leaves the actual child array alone.
void CViewTree::OnObjSortChildren()
{
	BObject* pobjTarget = m_pDoc->GetTargetObject();
	HTREEITEM htiTarget = m_tvw.FindItemData((DWORD) pobjTarget);
	m_tvw.SortByProperty(htiTarget, propName);

/*
	ASSERT_VALID(m_pobjPopup);
	BObjects* pChildren = m_pobjPopup->m_pChildren;
	if (pChildren)
	{
		ASSERT_VALID(pChildren);
		
		// Sort the objects in the pChildren collection
		// Note: Rather than just sort the tree view, we want to sort the actual child collection,
		// then refresh the views
		pChildren->Sort(propName);

		// Tell all views (including this one) to reload the children of the selected object
		GetDocument()->UpdateAllViewsEx(NULL, hintReloadChildren, m_pobjPopup);
	}
*/
}



//, Toggle show children for the selected item
// Note: The default is to show children 
//void CViewTree::OnPopupShowChildren()
//{
//}




// Clipboard Operations
//----------------------------------------------------------------------------------------------------

void CViewTree::OnCmdEditCopy() 
{
}

void CViewTree::OnCmdEditCut() 
{
}

void CViewTree::OnCmdEditPastePlain() 
{
}


//. should this be here or in frame?
void CViewTree::OnUpdateEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
/*	switch (pCmdUI->m_nID)
	{
	case ID_EDIT_COPY:
	case ID_EDIT_CUT:
		{
			// Enable cut and copy if there is one or more items selected in the listview
//			POSITION pos = m_lvwContents.GetFirstSelectedItemPosition();  // returns NULL if no items selected
//			pCmdUI->Enable((int)pos);
			break;
		}
	case ID_EDIT_PASTE_PLAIN:
		{
			// Enable paste if there is NeoMem clipboard format data on the clipboard
			// data object represents the destination side
			COleDataObject odo;
			odo.AttachClipboard();
			pCmdUI->Enable(odo.IsDataAvailable(theApp.m_cfObjects));
			break;
		}
	}
*/
}



// Drag Drop Operations
//----------------------------------------------------------------------------------------------------

// TVN_BEGINDRAG Handler
// Notifies a tree view control's parent window that a drag-and-drop operation involving 
// the left mouse button is being initiated. 
//DEL void CViewTree::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
//DEL {
//DEL 	// note - this will call DragGetSelectedObjects() to get the array of objects
//DEL 	TRACE("CViewTree OnBeginDrag\n");
//DEL 	CViewEx::OnBeginDrag(pNMHDR, pResult);
//DEL }


// TVN_BEGINDRAG Handler
// Notifies a tree view control's parent window that a drag-and-drop operation involving 
// the left mouse button is being initiated. 
void CViewTree::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	xTRACE("CViewTree::OnBeginDrag\n");

	// Get drag object and save to array
	NMTREEVIEW* ptv = (NMTREEVIEW*) pNMHDR;
	BObject* pobj = (BObject*) ptv->itemNew.lParam;
	ASSERT_VALID(pobj);
	m_aDragObjects.RemoveAll();
	m_aDragObjects.Add(pobj);

	// Start drag operation by wrapping objects into an hGlobal.
	CViewEx::DragStart();
}



//DEL DROPEFFECT CViewTree::OnDragScroll(DWORD dwKeyState, CPoint point)
//DEL {
//DEL 	// note - this will send scroll messages to target window if near border
//DEL 	TRACE("CViewTree OnDragScroll\n");
//DEL 	return CViewEx::OnDragScroll(dwKeyState, point);
//DEL }


//DEL DROPEFFECT CViewTree::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
//DEL {
//DEL 	// this remembers key state and determines if we're interested in the data being dragged
//DEL 	TRACE("CViewTree OnDragEnter\n");
//DEL 	return CViewEx::OnDragEnter(pDataObject, dwKeyState, point);
//DEL }


// Called by the framework during a drag operation when the mouse is moved over the 
// drop target window. The default implementation is to do nothing and return DROPEFFECT_NONE.
// Override this function to give the user visual feedback during the drag operation. Since 
// this function is called continuously, any code contained within it should be optimized as much as possible. 
// If this function returns DROPEFFECT_NONE, then OnDrop will NOT get called on mouseup
DROPEFFECT CViewTree::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	xTRACE("CViewTree::OnDragOver\n");

	// Call base class
	CViewEx::OnDragOver(pDataObject, dwKeyState, point);

	// Ignore if not interested in the data
	if (!m_bInterestedInData) return DROPEFFECT_NONE;

	// See if cursor is over an item
	UINT nFlags = 0;
	DROPEFFECT de;
	HTREEITEM hti = m_tvw.HitTest(point, &nFlags);
	if (hti)
		// Note: We check the validity of the target in OnDrop, not here.
		// If control key is pressed, user wants to copy the data
		de = (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
	else
		// Not over an item
		de = DROPEFFECT_NONE;

	// Highlight the item if changed from last time
	if (hti != m_htiDragOver)
		m_tvw.SelectDropTarget(hti);

	// Save which item cursor is over, if any
	m_htiDragOver = hti;

	return de;
}


// Called by the framework when the user releases a data object over a valid drop target. 
// The default implementation is to do nothing and return a dummy value ( -1 ) to indicate 
// that the framework should call the OnDrop handler.
// Override this function to implement the effect of an right mouse-button drag and drop. 
// Right mouse-button drag and drop typically displays a menu of choices when the right 
// mouse-button is released.
// Your override of OnDropEx should query for the right mouse-button. You can call GetKeyState 
// or store the right mouse-button state from your OnDragEnter handler. 
// If the right mouse-button is down, your override should display a popup menu which 
// offers the drop effects support by the drop source. 
// Examine dropList to determine the drop effects supported by the drop source. Enable 
// only these actions on the popup menu. 
// Use SetMenuDefaultItem to set the default action based on dropDefault. 
// Finally, take the action indicated by the user selection from the popup menu. 
// If the right mouse-button is not down, your override should process this as a standard 
// drop request. Use the drop effect specified in dropDefault. Alternately, your override can 
// return the dummy value ( -1 ) to indicate that OnDrop will handle this drop operation. 
// Use pDataObject to examine the COleDataObject for Clipboard data format and data dropped 
// at the specified point.
DROPEFFECT CViewTree::OnDropEx(COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point)
{
	// Call base class
	return CViewEx::OnDropEx(pDataObject, dropDefault, dropList, point);
/*
	if (m_dwDragKeyState & MK_RBUTTON)
	{
		// Bring up menu
		CMenu menu;	
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, ID_MOVEHERE, _T("&Move Here"));
		menu.AppendMenu(MF_STRING, ID_COPYHERE, _T("&Copy Here"));
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING, ID_CANCEL, _T("Cancel"));
		menu.SetDefaultItem(ID_MOVEHERE);
		return menu.Detach();
	}
	else return -1;
*/
}


// Called by the framework when the user releases a data object over a valid drop target. 
// The default implementation does nothing and returns FALSE.
// Override this function to implement the effect of an OLE drop into the client area of the view. 
// The data object can be examined via pDataObject for Clipboard data formats and data 
// dropped at the specified point.
//DEL BOOL CViewTree::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
//DEL {
//DEL 	TRACE("CViewTree OnDrop\n");
//DEL 	// Call base class
//DEL 	return CViewEx::OnDrop(pDataObject, dropEffect, point);
//DEL }


// Called by the framework during a drag operation when the mouse is moved out of the 
// valid drop area for that window.
// Override this function if the current view needs to clean up any actions taken during 
// OnDragEnter or OnDragOver calls, such as removing any visual user feedback while the 
// object was dragged and dropped.
void CViewTree::OnDragLeave() 
{
	xTRACE("CViewTree::OnDragLeave\n");

	// Call base class
	CViewEx::OnDragLeave();

	// Clear the drop target
	m_tvw.SelectDropTarget(NULL);
	m_htiDragOver = 0;
}


/*
// Virtual function called by CViewEx to get objects to drag, if any
//int CViewTree::DragGetSelectedObjects(NMHDR* pNMHDR, BObjects& aObjects)
int CViewTree::DragGetSelectedObjects(BObjects& aObjects)
{
	// Gather up selected objects into array, and start drag drop operation
	// Note: For treeview, we can only drag one item - 
	// get it from the pNMHDR structure passed to this routine.
//	NMTREEVIEW* ptv = (NMTREEVIEW*) pNMHDR;
//	BObject* pobj = (BObject*) ptv->itemNew.lParam;

	//, this is kinda circuituous
	// this may not work correctly
	// should save object in drag start event, then use it here
//	BObject* pobj = (BObject*) m_tvw.GetSelectedItemData();
	BObject* pobj = m_pobjDragObject;

	// Add the drag object to the array
	if (pobj)
	{
		ASSERT_VALID(pobj);
		aObjects.Add(pobj);
	}

	return aObjects.GetSize();
}
*/


// Virtual function called by CViewEx to get object under specified cursor point, if any
BObject* CViewTree::DragGetDropTarget(CPoint ptClient)
{
	// See which item is the target
	UINT nFlags = 0;
	HTREEITEM htiTarget = m_tvw.HitTest(ptClient, &nFlags);
	if (!htiTarget) return NULL;

	// Get pointer to the target BObject
	BObject* pobjTarget = (BObject*) m_tvw.GetItemData(htiTarget);
	if (!pobjTarget) return NULL;
	ASSERT_VALID(pobjTarget);

	return pobjTarget;
}



//---------------------------------------------------------------------------------------------------



// Called by the framework when a view is activated or deactivated. The default implementation 
// of this function sets the focus to the view being activated. Override this function if you want 
// to perform special processing when a view is activated or deactivated. 
// ** For example, if you 
// want to provide special visual cues that distinguish the active view from the inactive views, 
// you would examine the bActivate parameter and update the view’s appearance accordingly.
//. (could highlight the view border or something)
void CViewTree::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	xTRACE("CViewTree OnActivateView %d\n", bActivate);
	CViewEx::OnActivateView(bActivate, pActivateView, pDeactiveView);

	// If this view is being activated, set the focus to the child control
	if (bActivate) m_tvw.SetFocus();
}



void CViewTree::Navigate(UINT nCommandID)
{
//	switch (nCommandID)
//	{
//	case ID_NAVIGATE_UP:
//		m_tvw.GetItem
//	}
}


// Trap Delete key here.
// We have to do this because if we just use an accelerator, the command gets
// triggered even if you're editing a label in the treeview and hit delete!
void CViewTree::OnNotifyKeyDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVKEYDOWN* pkd = (NMTVKEYDOWN*) pNMHDR;
	xTRACE("CViewTree OnNotifyKeyDown %d\n", pkd->wVKey);
	if (pkd->wVKey == VK_DELETE)
	{
		// If user is not editing a label, translate the delete to an ID_EDIT_DELETE command
		// and send it up to the child frame
		if (!m_tvw.GetEditControl())
		{
			CFrameChild* pFrame = (CFrameChild*) GetParentFrame();
			pFrame->SendMessage(WM_COMMAND, ID_OBJ_DELETE);
		}
	}
/*
#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F
*/
}




// Simulate a keypress.
BOOL CViewTree::PressKey(UINT nVK)
{
	m_tvw.SendMessage(WM_KEYDOWN, (WPARAM) nVK, 0);
	m_tvw.SendMessage(WM_KEYUP, (WPARAM) nVK, 0);
	return TRUE;
}



// Set target object on gaining focus
void CViewTree::OnSetFocus(CWnd* pOldWnd) 
{
	xTRACE("CViewTree::OnSetFocus - set target object\n");
	CViewEx::OnSetFocus(pOldWnd);
	
	BObject* pobj = (BObject*) m_tvw.GetSelectedItemData();
	m_pDoc->SetTargetObject(pobj);	
}





void CViewTree::OnObjMoveTo() 
{
	m_pDoc->UIMoveObjectTo();
}

void CViewTree::OnUpdateObjMoveTo(CCmdUI* pCmdUI) 
{
}

// export the selected item to a file
void CViewTree::OnCmdFileExport() 
{
	BObject* pobj = m_pDoc->GetTargetObject();
	if (pobj)
		theApp.Export(pobj);
}



/*
// 1.1d added this for debugging
// DON'T DELETE THIS CODE good for debugging
#ifdef _DEBUG
BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* pnm = (NMHDR*) lParam;
	UINT nIDFrom = pnm->idFrom;
	UINT nCode = pnm->code;
	CString str = theApp.GetNotifyCodeString(nCode);
	TRACE("CViewTree OnNotify id %d, code %d %s\n", nIDFrom, nCode, (LPCTSTR) str);
	return CViewEx::OnNotify(wParam, lParam, pResult);
}
#endif
*/





