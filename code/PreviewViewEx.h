
// CPreviewViewEx
// Extension of MFC private CPreviewView class
//-----------------------------------------------------------------------------------------------------------------



#pragma once


class CPreviewViewEx : public CPreviewView
{
	DECLARE_DYNCREATE(CPreviewViewEx)

// Construction
protected:
	CPreviewViewEx();           // protected constructor used by dynamic creation
	virtual ~CPreviewViewEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewViewEx)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnActivateView(BOOL bActivate, CView*, CView*);
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPreviewViewEx)
	afx_msg void OnPreviewSetup();
	afx_msg void OnPreviewPrint();
	afx_msg void OnPreviewHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


