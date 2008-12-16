
// CViewTree
// This view displays all the objects in the file in a hierarchical tree. 
// Date: November 1999
//-----------------------------------------------------------------------------------------------------------------

//, different data view modes:
// alphabetical - walk through all objects recursively, add them to treeview sorted
// categorized - walk through all objects recursively, get class, see if class has been added yet,
//    if not, add it. if it has, get it, and add the object as a child of the class, sorted.
// when does this get done?
// when the tab is clicked on
// you could do it at the same time that the treeview is filled
// ie on loadfile
// but that would just slow things down even more
// better to load it on tab click




#pragma once


#include "TreeCtrlEx.h"
#include "ViewEx.h"

#include "BObject.h"
#include "BObjects.h"
#include "NeoDoc.h"



class CViewTree : public CViewEx
{
	DECLARE_DYNCREATE(CViewTree)

// Construction
protected: 
	CViewTree();
	virtual ~CViewTree();

// Operations
public:
	void Navigate(UINT nCommandID); // called from cframechild on alt-up etc
	void RefreshIcons(HTREEITEM hti);
	void ResortChildren(HTREEITEM hti);

// Attributes
private:
	CTreeCtrlEx m_tvw;	// Tree control extended //, public so child frame can send keystrokes to it
	BOOL m_bTempSelection; // if true prevents updateviews from being sent in selchanged
	HTREEITEM m_htiSaveDuringRename; // used by OnObjEditInPlace

	// Drag Drop support
	HTREEITEM m_htiDragOver; // Item being dragged over
	BObject* m_pobjDragObject;
	virtual BObject* DragGetDropTarget(CPoint ptClient);


// Overrides
public:
	virtual BOOL PressKey(UINT nVK);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewTree)
	public:
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDropEx(COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point);
	virtual void OnDragLeave();
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL
/*
#ifdef _DEBUG
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
#endif
*/

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	
// Generated message map functions
protected:
	//{{AFX_MSG(CViewTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnObjEditInPlace(); //ok
	afx_msg void OnObjSortChildren(); //ok
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateEdit(CCmdUI* pCmdUI);
	afx_msg void OnCmdEditCopy();
	afx_msg void OnCmdEditCut();
	afx_msg void OnCmdEditPastePlain();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnObjMoveTo();
	afx_msg void OnUpdateObjMoveTo(CCmdUI* pCmdUI);
	afx_msg void OnCmdFileExport();
	//}}AFX_MSG
//	afx_msg void OnFilePrintPreview(); 
	afx_msg void OnNotifyKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};


