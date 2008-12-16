
// CViewEx
// This is the base class for all Views that display information about the file. 
// It extends the CView class by allowing toolbars to dock to it, and provides 
// an Update method. 
// Date: February 2000
//-----------------------------------------------------------------------------------------------------------------




#pragma once

#include "FontEx.h"
#include "BCMenu.h" // bcmenu

#include "NeoDoc.h"
#include "BObjects.h"



class CViewEx : public CView
{
	DECLARE_DYNCREATE(CViewEx)

// Construction
protected:
	CViewEx();           // protected constructor used by dynamic creation
	virtual ~CViewEx();

// Operations
public:
	BCMenu* InitPopup(UINT nPopupID, UINT nDefaultID = NULL);
	BOOL IsActiveView();
	CView* GetSiblingView(ULONG lngViewID); // get another view 
	ULONG GetViewID();
	HACCEL LoadViewAccelerators(int nAccelID); // get accelerators specific to this view
	void SetChildView(CWnd* pView); // contain a child view within this view
	void SetTitleBarName(LPCTSTR pszName);
	void SetToolBar(CToolBarCtrl* pWnd); // dock a toolbar to the top of the view
	void SetViewID(ULONG lngViewID);
	void ShowTitleBar(BOOL bShow);
	CView* ShowView(ULONG lngViewID, BOOL bSetActive, BOOL bAskAddIfNotFound);
	void UpdateView(CView* pSender, LPARAM lHint, CObject* pHint);

	virtual BOOL DoPrint(BOOL bPreview); // handle print/print preview
	virtual BOOL PressKey(UINT nVK); // handle a key press

// Attributes
protected:
	BCMenu m_popmenu;
	CNeoDoc* m_pDoc; // pointer to document associated with this view

	// Drag Drop
	BObjects m_aDragObjects; // array of objects being dragged - derived class should fill this and call DragStart.
	COleDropTarget m_DropTarget; // needed for drag drop
	CWnd* m_pwndDropTarget; // window to receive scroll events during drag drop
	BOOL m_bInterestedInData; // true if drop source data is interesting to this view
	DWORD m_dwDragKeyState; // initial key state as received by DragEnter
	void DragRegister(CWnd* pwndDropTarget); // derived view should call this to register the window for ole dragdrop
	void DragStart(); // 
	virtual BObject* DragGetDropTarget(CPoint ptClient); // derived view should override this to get drop target under a point

	// Title bar
	BOOL m_bShowTitleBar; // true if this view should show its mini title bar
	int m_nTitleBarHeight; // height of mini title bar
	CString m_strTitleBar; // name to display in mini title bar

private:
	CWnd* m_pviewChild; // child view window
	CToolBarCtrl* m_ptbr; // child toolbar window, if any
	HACCEL m_hAccel; // handle to custom accelerators for this view
	ULONG m_lngViewID; // ViewID of this view, eg viewRtf


// Implementation
private: 
	void OnPrinterChanged(const CDC& dcPrinter);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragScroll(DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual void OnDragLeave();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewEx)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateNeedSel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedClip(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPastePlain();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnViewHide();
	afx_msg void OnViewMoveUp();
	afx_msg void OnDevModeChange(LPSTR lpDeviceName);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	//}}AFX_MSG
	afx_msg BOOL OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};




