
// CViewTabs
// This view contains a tabstrip which can contain other views. 
// This is used for both the left and the right sides of the main window.
// Date: February 2000
//--------------------------------------------------------------------------------------------------------------------



#pragma once


#include "SplitterWndEx.h"
#include "TabCtrlEx.h"
#include "ViewEx.h"

#include "BDoc.h"
class BDataViews;



class CViewTabs : public CViewEx
{
	DECLARE_DYNCREATE(CViewTabs)

// Construction
protected:
	CViewTabs();           // protected constructor used by dynamic creation
	virtual ~CViewTabs();

// Operations
public:
	void ActivateFirstView();
	BOOL AddView(ULONG lngViewID);
	BOOL AdjustViewHeights();
	void DeleteCopiedViewInfo();
	int FindView(ULONG lngViewID);
	CViewEx* GetCurrentView();
	CView* GetNextView(CView* pCurrentView);
	CView* GetPreviousView(CView* pCurrentView);
	CViewEx* GetView(ULONG lngViewID);
	CSplitterWndEx* InitializeSplitter(int nTab, int nViews);
	CViewEx* LoadView(ULONG lngViewID);
	BOOL MoveViewUp(ULONG lngViewID);
	void PrepareToModifyViews();
	void RecalcLayout(BOOL bRepaint);
	BOOL RemoveView(ULONG lngViewID);
	void ResetViews();
	void SelectTab(int nTab);
	BOOL SelectView(ULONG lngViewID);
	void SetMode(ULONG lngMode);
	void ShowTab(int nTab);
	void UpdateTabs();	
	void ViewsModified();

	void OnSplitterResized();
	void OnSplitterPaneResized();
	BOOL IsViewVisible(ULONG lngViewID);
	BOOL LoadViews(BObject* pobj);

// Public Attributes
public:
	CObArray m_aCurrentViews; // array of current view objects
	CTabCtrlEx m_tbc;
//	BDoc* m_pDoc;


// Attributes
private:
	ULONG m_lngMode;	// Mode of view (navigation or contents) //, mode? not subclass? 
	BOOL m_bDrag;
	int m_nTabRClick; 
//	CView* m_pCurrentView;
//	CWnd* m_pCurrentChildWnd; // splitter or view window
	ULONG m_lngCurrentViewID; 
	CObArray m_Splitters; // array of pointers to splitter windows (one for each tab)
	CMapWordToPtr m_Map; // Maps ViewID to CView pointer
	int m_nCurrentTab; // keep track of current tab being displayed

	// View arrangement
	BDataViews* m_pdatViews; // BData object storing tab/view information
	BOOL m_bViewsCopied; // flag indicating tab/view arrays have been copied
	BOOL m_bViewsChanged; // flag indicating user changed tab/view arrangement somehow
	BOOL m_bViewsSaved; // flag indicating if m_pdatViews was saved to an object
	BYTE m_bytViewHeight; //, for v1, store the height of the first pane as % of total
	BOOL m_bNewSplitter; // flag to get around splitter sizing problem

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewTabs)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CViewTabs)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnViewEdit();
	afx_msg void OnTabHide();
	//}}AFX_MSG
	afx_msg void OnCmdViewID(UINT nCommandID);
	afx_msg void OnUpdateViewID(CCmdUI* pCmdUI);
	afx_msg void OnTabMerge();
	// Note: All WM_NOTIFY handlers have the following structure:
	// afx_msg void memberFxn( NMHDR * pNotifyStruct, LRESULT * result );
	afx_msg void OnTabClick(NMHDR* pNotify, LRESULT* result);
	afx_msg void OnTabRightClick(NMHDR* pNotify, LRESULT* result);
//	afx_msg void OnTabSelChange(NMHDR* pNotify, LRESULT* result);
	DECLARE_MESSAGE_MAP()
};

