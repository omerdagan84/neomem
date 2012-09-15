
// CTreeCtrlEx


#include "precompiled.h"

#include "NeoMem.h"
#include "TreeCtrlEx.h"

#include "BDoc.h"
#include "ConstantsDatabase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





BEGIN_MESSAGE_MAP(CTreeCtrlEx, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeCtrlEx)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, OnGetDispInfo)
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CTreeCtrlEx::CTreeCtrlEx()
{
//	m_blnDragging = 0;
//	m_blnDraggingRight = 0;
//	m_pDragImageList = 0;
//	m_htiDragItem = 0;
//	m_htiDropItem = 0;
}

CTreeCtrlEx::~CTreeCtrlEx()
{
}




// Add objects to tree recursively, starting with the start object
void CTreeCtrlEx::AddObjects(BObject* pobjStart, ULONG lngExcludeFlags, 
								BOOL bIncludeStart /* = FALSE */, BOOL bRecurse /* = FALSE */, 
								BOOL bUseExpandFlags /* = FALSE */)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjStart);

	// Clear treeview first
	DeleteAllItems();

	HTREEITEM htiStart = 0;

	if (bIncludeStart)
	{
		// User's choice should override the exclude flags
		htiStart = AddObject(pobjStart, 0);
	}
		
	// Add children recursively, if any
	if (pobjStart->GetChildren())
	{
		BOOL bSortChildren = !(pobjStart->GetFlag(flagNoAutosort));
		ASSERT_VALID(pobjStart->GetChildren());
		AddChildrenToTree(pobjStart->GetChildren(), htiStart, lngExcludeFlags, bSortChildren, bUseExpandFlags);
	}

	if (bIncludeStart)
	{
		// Expand root item
		Expand(htiStart, TVE_EXPAND);
	}
	else
	{
		// Expand top items
		HTREEITEM hti = GetFirstVisibleItem();
		while (hti)
		{
			Expand(hti, TVE_EXPAND);
			hti = GetNextSiblingItem(hti);
		}
	}

}



// Add an object to the tree.
// Returns the hti of the new item.
HTREEITEM CTreeCtrlEx::AddObject(BObject *pobj, BObject *pobjParent)
{
	// Get parent item, if any
	HTREEITEM htiParent = FindItemData((DWORD) pobjParent, 0);
	if (htiParent == 0)
		htiParent = TVI_ROOT;

	int nImage = pobj->GetIconIndex();

	// Add this bobject to the tree
	HTREEITEM hti = InsertItem(
				TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE,	// Mask
//				pobj->GetPropertyString(propName),
				LPSTR_TEXTCALLBACK,		// Use callback to provide text (saves memory)
				nImage,					// Image
				nImage,					// Selected Image
				0,						// State
				0,						// State Mask
				(LPARAM) pobj,			// Item Data
				htiParent,				// Parent
//				TVI_SORT				// Insert sorted
				TVI_LAST				// Insert After
				);
	ASSERT (hti);

	// If we're adding this object to the root node, make sure to expand it (because the
	// root node doesn't have +/- next to it, and by default it is not expanded apparently).
	if (GetRootItem() == htiParent)
		Expand(htiParent, TVE_EXPAND);

	// In order to make sure the + sign gets displayed next to the parent, we need
	// to force a refresh...
	SetItemData(htiParent, (DWORD) pobjParent);

	return hti;
}


// Add collection of bobjects and their children, if any, to the treeview.
//, In the future, could navigate through properties other than the Children collection.
void CTreeCtrlEx::AddChildrenToTree(BObjects* paChildren, HTREEITEM htiParent, 
													ULONG lngExcludeFlags, 
													BOOL bSortChildren, 
													BOOL bUseExpandFlags) 
{	
	if (paChildren)
	{
		// Set up the insert structure outside loop for speed
		TVINSERTSTRUCT tvis;
//		tvis.hInsertAfter = bSortChildren ? TVI_SORT : TVI_LAST;
		tvis.hInsertAfter = TVI_LAST; // any sorting will be done after all children are added
		TVITEM& tvi = tvis.item; // tvi is a reference to the TVITEM embedded in the TVINSERTSTRUCT structure
		tvi.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		tvi.pszText = LPSTR_TEXTCALLBACK;

		// Walk through child objects and add them recursively to the tree
		int nItems = paChildren->GetSize();
		for (int i = 0; i < nItems; i++)
		{
			BObject* pobj = DYNAMIC_DOWNCAST(BObject, paChildren->GetAt(i));
			ASSERT_VALID(pobj);
		
			if (!(pobj->GetFlag(lngExcludeFlags)))
			{
				// Get the index of the icon in the image list
				int nImage = pobj->GetIconIndex();

				// Add this bobject to the tree
//				HTREEITEM hti = InsertItem(
//							TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE,	// Mask
//							pobj->GetPropertyString(propName),
//							nImage,					// Image
//							nImage,					// Selected Image
//							0,						// State
//							0,						// State Mask
//							(LPARAM) pobj,			// Item Data
//							htiParent,				// Parent
//							TVI_LAST				// Insert After
//							);
//				ASSERT (hti);

				// Use TVINSERTSTRUCT for speed
				tvis.hParent = htiParent;
				// tvi is a reference to the TVITEM embedded in the TVINSERTSTRUCT structure
				tvi.iImage = nImage;
				tvi.iSelectedImage = nImage;
				tvi.lParam = (LPARAM) pobj;			
//				hti = m_tvw.InsertItem(&tvis);
//				hti = (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
				// Send the message directly for even more speed
//				HTREEITEM hti = (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM) &tvis);
				HTREEITEM hti = TreeView_InsertItem(m_hWnd, &tvis);
				ASSERT (hti);

				// If this object has children, then call this routine recursively.
				// BUG:: Had this code outside of check for valid flags, as a result,
				// hti was invalid sometimes, and recurse call bombed. Makes sense for listview
				// but not treeview!
				if (pobj->GetChildren())
				{
					// Call this routine recursively
					BOOL bSortGrandchildren = !(pobj->GetFlag(flagNoAutosort));
					AddChildrenToTree(pobj->GetChildren(), hti, lngExcludeFlags, bSortGrandchildren, bUseExpandFlags);

					// Expand item if specified and flag is set
					if (bUseExpandFlags && (pobj->GetFlag(flagExpanded)))
						Expand(hti, TVE_EXPAND);
				}
			}
		}

		// Now sort the children of the item using our callback function
//		// which will put folders before any other objects.
		if (bSortChildren)
			SortByProperty(htiParent, propName);
//			SortByProperty(htiParent, propName, 1);

	}
}




// Gets the DWORD data associated with the currently selected item
//, Should return NULL if no item selected
DWORD CTreeCtrlEx::GetSelectedItemData()
{
	HTREEITEM hti = GetSelectedItem();
	if (hti)
		return GetItemData(hti);
	return NULL;
}




// Select an item based on its data
HTREEITEM CTreeCtrlEx::SelectItemData(DWORD dwData)
{
	HTREEITEM hti = FindItemData(dwData, 0);
	SelectItem(hti);
//?	Select(hti, TVGN_CARET);
	return hti;
}







// Find node in tree with the specified data
// Pass hItem = 0 to search from root node down
HTREEITEM CTreeCtrlEx::FindItemData(DWORD dwData, HTREEITEM htiStartItem /* = 0 */)
{
	// from codeguru.com
	// This is not an efficient way to do it.  Use a CMap instead. 
	// CMap<HTREEITEM, HTREEITEM&, CMyData*, CMyData*> myMap; 
	// And look the help file on how to use it. 
	//. if we have several hundred items, might need to use the map

	if (GetCount() == 0)
		return 0;

	// Start at root item if hItem is 0
	if (htiStartItem == 0)
		htiStartItem = CTreeCtrl::GetRootItem();

	// Exit if found item
	if (CTreeCtrl::GetItemData(htiStartItem) == dwData)
		return htiStartItem;

	HTREEITEM htiItem = NULL;
	HTREEITEM htiChild = CTreeCtrl::GetChildItem(htiStartItem);
	if (htiChild)
		htiItem = FindItemData(dwData, htiChild);

	if (htiItem == NULL)
	{
		//, not very efficient - creates another call for each sibling item -
		// just loop through
		HTREEITEM htiSibling = CTreeCtrl::GetNextSiblingItem(htiStartItem);
		if (htiSibling)
			htiItem = FindItemData(dwData, htiSibling);
	}

	return htiItem;
}




// Delete all the children of the specified tree item
void CTreeCtrlEx::DeleteChildren(HTREEITEM htiParent)
{
	HTREEITEM htiChild = GetChildItem(htiParent);
	while (htiChild)
	{
		HTREEITEM htiNext = GetNextSiblingItem(htiChild);
		DeleteItem(htiChild);
		htiChild = htiNext;
	}
}




// Move the specified item to a new parent item
BOOL CTreeCtrlEx::MoveItem(HTREEITEM htiItem, HTREEITEM htiNewParent)
{
	// First we want to copy the item to the new location, then delete the old item
	HTREEITEM htiCopy = CopyItem(htiItem, htiNewParent);
	if (htiCopy)
	{
		// Now select the copy of the item (otherwise two objects might wind up selected)
		SelectItem(htiCopy); // this triggers OnSelChanged event
		DeleteItem(htiItem);
		return TRUE;
	}
	return FALSE;
}



// Creates a copy of the specified item at a new parent location.
// Returns the handle of the new item, or 0 if failed.
HTREEITEM CTreeCtrlEx::CopyItem(HTREEITEM htiItem, HTREEITEM htiNewParent)
{
	HTREEITEM htiCopy = 0;
	TVITEM tvi;
//	tvi.mask = -1; // get all info
//	tvi.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_STATE;
	tvi.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_STATE;
	tvi.hItem = htiItem;
	tvi.stateMask = -1; // get all state info
	if (GetItem(&tvi))
	{
		htiCopy = InsertItem("", htiNewParent);
		if (htiCopy)
		{
			// Specify that text of copy is to be obtained through callback
			tvi.hItem = htiCopy;
			tvi.mask |= TVIF_TEXT;
			tvi.pszText = LPSTR_TEXTCALLBACK;
			SetItem(&tvi);

			// Recursively copy child items
			HTREEITEM htiChild = GetChildItem(htiItem);
			while (htiChild)
			{
				HTREEITEM htiChildCopy = CopyItem(htiChild, htiCopy);
				htiChild = GetNextSiblingItem(htiChild);
			}

		}
	}

	return htiCopy;
}


// Move an item to a location before or after the target item
// Returns the new HTREEITEM for the source, or 0 if failed
HTREEITEM CTreeCtrlEx::MoveItemToSibling(HTREEITEM htiSource, HTREEITEM htiTarget, BOOL bAfter)
{
	// First we want to copy the item to the new location, then delete the old item
	HTREEITEM htiSourceNew = CopyItemToSibling(htiSource, htiTarget, bAfter);
	if (htiSourceNew)
	{
		// If the currently selected item is about to be deleted, select the copy of it to 
		// prevent extra updates.
		if (GetSelectedItem() == htiSource)
			SelectItem(htiSourceNew);
		DeleteItem(htiSource);
		return htiSourceNew;
	}
	return 0;
}



// Copy item to a location before or after the target item
HTREEITEM CTreeCtrlEx::CopyItemToSibling(HTREEITEM htiSource, HTREEITEM htiTarget, BOOL bAfter)
{
	HTREEITEM htiCopy = 0;

	// Figure out where to copy item to
	HTREEITEM htiNewParent = GetParentItem(htiTarget);
	ASSERT(htiNewParent);

	// Get source item info
	TVITEM tvi;
	tvi.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_STATE;
	tvi.hItem = htiSource;
	tvi.stateMask = -1; // get all state info
	if (GetItem(&tvi))
	{
		// Copy item to new parent after the specified target
		HTREEITEM htiAfter = htiTarget; // default is to move after the target
		if (!bAfter)
		{
			// If we're moving before the target, and the target is the first item in the list, 
			// move the source to the beginning
			if (htiTarget == GetChildItem(htiNewParent))
				htiAfter = TVI_FIRST;
			else
				// If we're moving before the target, and the target is NOT the first item in the list,
				// then move the source after the item before the target
				htiAfter = GetPrevSiblingItem(htiTarget);
		}
		htiCopy = InsertItem("", htiNewParent, htiAfter);
		if (htiCopy)
		{
			// Specify that text of copy is to be obtained through callback
			tvi.hItem = htiCopy;
			tvi.mask |= TVIF_TEXT;
			tvi.pszText = LPSTR_TEXTCALLBACK;
			SetItem(&tvi);

			// Recursively copy child items
			HTREEITEM htiChild = GetChildItem(htiSource);
			while (htiChild)
			{
				HTREEITEM htiChildCopy = CopyItem(htiChild, htiCopy);
				htiChild = GetNextSiblingItem(htiChild);
			}
		}
	}

	return htiCopy;
}





// Expand or collapse the children of the specified item, recursively if specified.
// nCode is TVE_COLLAPSE or TVE_EXPAND.
BOOL CTreeCtrlEx::ExpandChildren(HTREEITEM hti, UINT nCode, BOOL bRecurse)
{
	HTREEITEM htiChild = GetChildItem(hti);
	while (htiChild)
	{
		HTREEITEM htiNext = GetNextSiblingItem(htiChild);
		Expand(htiChild, nCode);
		if (bRecurse)
		{
			ExpandChildren(htiChild, nCode, bRecurse);
		}
		htiChild = htiNext;
	}
	return TRUE;
}





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
// Note: This gets called once for each type of info, ie. once for text and once for children info,
// so used an else if structure below.
void CTreeCtrlEx::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	TVITEM* pTVITEM = &(pTVDispInfo->item);
	
	UINT nMask = pTVITEM->mask;
	HTREEITEM hti = pTVITEM->hItem;

	BObject* pobj = (BObject*) GetItemData(hti);
	if (pobj)
	{
		ASSERT_VALID(pobj);

		// Depending on what information is being requested, return different info
		if (nMask & TVIF_TEXT)
			// Note const_cast removing const modifier from LPCTSTR - 
			// this struct is used for sending and receiving strings, so 
			// pszText is nonconst. But for sending data can treat it as const.
			// Had been using GetBuffer which slowed the program down ENORMOUSLY, 
			// as it wound up copying the strings.
			pTVITEM->pszText = const_cast <LPSTR> (pobj->GetPropertyString(propName));

		else if (nMask & TVIF_CHILDREN)
			pTVITEM->cChildren = pobj->HasChildren();
	}

	*pResult = 0;
}




// Delete the item with the associated item data.
// Note: If you delete the currently selected item the treeview will automatically
// select another item, which triggers the selchange event.
BOOL CTreeCtrlEx::DeleteItemData(DWORD dwData)
{
	HTREEITEM hti = FindItemData(dwData, 0);
	if (hti)
	{
		return DeleteItem(hti);
	}
	return FALSE;
}






// Compare two BObjects for sorting purposes (callback function).
// Return value should be -1, 0, or 1.
// Called by SortChildrenCB
// Note: ListCtrl has similar function, but this is streamlined to handle names only.
// This code needs to be fast
static int CALLBACK CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	BObject* pobj1 = (BObject*) lParam1;
	BObject* pobj2 = (BObject*) lParam2;

	if (pobj1 && pobj2)
	{
/*
		// Check for high priority items first
		// If object 1 is high priority and object 2 is not, put object 1 first
		// If object 2 is high priority and object 1 is not, put object 2 first
		BOOL bHigh1 = (pobj1->GetFlag(flagHighPriority));
		BOOL bHigh2 = (pobj2->GetFlag(flagHighPriority));
		BOOL bDifferent = (bHigh1 ^ bHigh2);
		if (bDifferent) return (bHigh1 ? -1 : 1);

		// Check for folders - folders should come before other objects
		// If object 1 is a folder and object 2 is not, put object 1 first
		// If object 2 is a folder and object 1 is not, put object 2 first
		BOOL bFolder1 = (pobj1->GetClassID() == classFolder);
		BOOL bFolder2 = (pobj2->GetClassID() == classFolder);
		bDifferent = (bFolder1 ^ bFolder2);
		if (bDifferent) return (bFolder1 ? -1 : 1);
*/
		// Compare strings
		LPCTSTR psz1 = pobj1->GetPropertyString(propName);
		LPCTSTR psz2 = pobj2->GetPropertyString(propName);
		return lstrcmpi(psz1, psz2);
	}
	return 0;
}



// Sort the specified item's children by the specified property.
// Uses our callback function CompareItems.
//, eventually have custom sort ability for each object that gets saved with it - eg sort by descending date, etc.
void CTreeCtrlEx::SortByProperty(HTREEITEM htiParent, ULONG lngPropertyID)
{
	ASSERT(htiParent);
	ASSERT(lngPropertyID == propName); //, for now

//	CWaitCursor wc;

	// Remember current row data and column (if any)
//	DWORD dwCurrentData = GetSelectedItemData();
//	int nCurrentCol = m_nCol;

	TVSORTCB tvs;
	tvs.hParent = htiParent;
	tvs.lpfnCompare = CompareItems;
	tvs.lParam = 0;
	CTreeCtrl::SortChildrenCB(&tvs);

	// Reselect current row
//	if (dwCurrentData != 0)
//	{
//		int nRow = FindItemData(dwCurrentData);
//		SelectCell(nRow, nCurrentCol);
//	}
}




// The framework calls this member function when the cursor is in an inactive window and 
// the user presses a mouse button. 
// The default implementation passes this message to the parent window before any processing 
// occurs. If the parent window returns TRUE, processing is halted.
// We handle this so that if user is renaming an item and clicks in the edit ctrl the focus does not
// pass to the tree ctrl.
int CTreeCtrlEx::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	xTRACE("CTreeCtrlEx::OnMouseActivate\n");	
	
	// This code was taken from CListCtrlEx, which uses a custom edit ctrl with ID of IDC_TXT_EDIT.
	// For treectrl, normal edit ctrl ID is 0..
	int nEditID = 0; // IDC_TXT_EDIT;

	// If renaming an item, return noactivate, otherwise call the base class as usual.
	CWnd* pwnd = GetFocus();
	BOOL bFocusInEdit = ((pwnd != 0) && (pwnd->GetDlgCtrlID() == nEditID));

	// If user clicks somewhere else in the listview, should call base class. 
	// Need to determine if click occurred in the edit window.
	CPoint p(::GetMessagePos()); // get position of message source in screen coords
	CWnd* pwndClick = WindowFromPoint(p);
	ASSERT_VALID(pwndClick);
	BOOL bClickedInEdit = (pwndClick->GetDlgCtrlID() == nEditID);

	// If focus is not in edit control, OR user did NOT click in edit control, call the base class.
	// This will terminate the edit control if it has focus.
	if (!bFocusInEdit || !bClickedInEdit)
		return CTreeCtrl::OnMouseActivate(pDesktopWnd, nHitTest, message);
	else
		return MA_NOACTIVATE;
}

