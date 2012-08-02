
// CFrameChild
// This class encapsulates all code associated with the main child frame, 
// which contains both the left and right panes.
// Date: November 1999
//-----------------------------------------------------------------------------------------------------------------

//, rename to CFrameDoc


#pragma once


#include "SplitterWndEx.h"
#include "ViewEx.h"

class CViewTabs;
class CViewHeader;


class CFrameChild : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameChild)

// Construction
public:
	CFrameChild();
	virtual ~CFrameChild();

// Operations
public:
	CViewEx* GetCurrentView();
	CViewEx* GetCurrentView(BOOL bLeftSide);
	ULONG GetSplitterPos();
	CView* GetView(ULONG lngViewID);
	BOOL IsViewVisible(ULONG lngViewID);
	void SetSplitterPos(ULONG lngSplitterPos);
	CView* ShowView(ULONG lngViewID, BOOL bSetActive);

	CViewHeader* GetViewHeader() { return m_pviewHeader; };

// Attributes
protected:
	CViewHeader* m_pviewHeader;		// pointer to right CViewHeader control
	CSplitterWndEx m_wndSplitter;
	CViewTabs* m_pviewNavigation;	// pointer to left CViewTabs control

	BOOL DoFilePrint(BOOL bPrintPreview, BOOL bAskForView);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameChild)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle); // advanced overridable

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CFrameChild)
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnCmdFileExport();
	afx_msg void OnCmdViewNext();
	afx_msg void OnCmdViewPrevious();
	afx_msg void OnObjEditInPlace(); //ok
	afx_msg void OnCmdFileImport();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFilePrintFromPreview();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
	//}}AFX_MSG
	
	afx_msg void OnCmdView(UINT nCommandID);
	afx_msg void OnUpdateView(CCmdUI* pCmdUI);
	afx_msg void OnCmdNavigate(UINT nCommandID);
//.	afx_msg void OnCmdAddObject();

public:
	DECLARE_MESSAGE_MAP()
};


