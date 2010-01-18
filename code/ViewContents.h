
// CViewContents
// This view displays the contents of the selected object in a modified ListView control.
// Date: November 1999
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "ListCtrlEx.h"
#include "ToolBarCtrlEx.h"
#include "Undo.h"
#include "ViewEx.h"
#include "FontEx.h"

#include "BObject.h"
#include "NeoDoc.h"



class CViewContents : public CViewEx {

	DECLARE_DYNCREATE(CViewContents)

public:

	// Construction
	CViewContents();
	virtual ~CViewContents();

	// Operations
	virtual BOOL DoPrint(BOOL bPreview);


	// Implementation
private:
	BObject* DragGetDropTarget(CPoint ptClient); // get drop target for given point
//	void DisableVisibleProperties(); // set flagDisabled for properties that are already visible in this view
//	void EnableAllProperties(); // clear flagDisabled for all properties
	BObject* GetSelectedObject(); // get currently selected object
	void SetTargetObject();

	// Attributes
//private: 
public: // so can test drive its functions!
	CListCtrlEx m_lvw; // list view control
private: 
	CToolBarCtrlEx m_tbr; // toolbar control
//	CUndo m_objUndo; // undo object
//	int m_nTargetColumn; // selected column for header popup menu
	int m_nDragOver; // item being dragged over



	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewContents)
	public:
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void OnInitialUpdate();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL
#ifdef _DEBUG
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
#endif


	// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Message map functions
protected:
	//{{AFX_MSG(CViewContents)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNavigateFollowLink(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjMoveDown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjMoveUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewGridlines(CCmdUI* pCmdUI);
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCmdEditCopy();
	afx_msg void OnCmdEditCut();
	afx_msg void OnCmdEditPasteFormatted();
	afx_msg void OnCmdViewGridlines();
	afx_msg void OnObjAdd(); //ok
	afx_msg void OnObjEditInPlace(); //ok
	afx_msg void OnObjEditInDialog(); //ok
	afx_msg void OnCmdEditSelectAll();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCmdEditCopyGrid();
	afx_msg void OnCmdEditClear();
	afx_msg void OnCmdEditPastePlain();
	afx_msg void OnCmdEditUndo();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnNavigateFollowLink();
	afx_msg void OnObjAddFolder();
	afx_msg void OnObjMoveTo();
	afx_msg void OnUpdateObjMoveTo(CCmdUI* pCmdUI);
	//}}AFX_MSG
//	afx_msg void OnUpdateColumnInsert(CCmdUI* pCmdUI);
	// test
//	afx_msg void OnHeaderItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnHeaderEndDrag(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnHeaderRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotifyDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotifyKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotifyReturn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupBDataCommand(UINT nCommandID);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg LRESULT OnLvwColumnsChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLvwItemSelected(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};




