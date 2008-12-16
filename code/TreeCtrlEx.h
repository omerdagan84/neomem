
// CTreeCtrlEx
// This class extends the CTreeCtrl class.
// Date: February 2000
//-----------------------------------------------------------------------------------------------------------------


#pragma once


class BObject;
class BObjects;


class CTreeCtrlEx : public CTreeCtrl
{
// Construction
public:
	CTreeCtrlEx();
	virtual ~CTreeCtrlEx();

// Operations
public:
	HTREEITEM AddObject(BObject* pobj, BObject* pobjParent);
	void AddObjects(BObject* pobjParent, ULONG lngExcludeFlags, 
								BOOL bIncludeStart = FALSE, BOOL bRecurse = FALSE, BOOL bUseExpandFlags = FALSE);
	void AddChildrenToTree(BObjects* paChildren, HTREEITEM htiParent, ULONG lngExcludeFlags, 
								BOOL bSortChildren, BOOL bUseExpandFlags);
	HTREEITEM CopyItem(HTREEITEM htiItem, HTREEITEM htiNewParent);
	HTREEITEM CopyItemToSibling(HTREEITEM htiSource, HTREEITEM htiTarget, BOOL bAfter);
	void DeleteChildren(HTREEITEM htiParent);
	BOOL DeleteItemData(DWORD dwData);
	BOOL ExpandChildren(HTREEITEM hti, UINT nCode, BOOL bRecurse);
	HTREEITEM FindItemData(DWORD dwData, HTREEITEM htiStartItem = 0); 
//	virtual HMENU GetRDragMenu();
//	int GetSelectedItemsArray(CObArray& aObjects);
	DWORD GetSelectedItemData();
	BOOL MoveItem(HTREEITEM htiItem, HTREEITEM htiNewParent);
	HTREEITEM MoveItemToSibling(HTREEITEM htiSource, HTREEITEM htiTarget, BOOL bAfter);
	HTREEITEM SelectItemData(DWORD dwData);
	void SortByProperty(HTREEITEM htiParent, ULONG lngPropertyID);

// Attributes
protected:
//	HTREEITEM m_htiPopup;	// Stores node in tree right clicked on
//	BOOL m_blnDragging; // Dragging flag
//	BOOL m_blnDraggingRight; // Dragging right button flag
//	CImageList* m_pDragImageList; // Pointer to image list created by drag operations
//	HTREEITEM m_htiDragItem; // Item being dragged
//	HTREEITEM m_htiDropItem; // Item being dropped onto

// Implementation

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlEx)
	public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
protected:
//	virtual void OnItemCopied(HTREEITEM hItem, HTREEITEM hNewItem);


// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlEx)
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


