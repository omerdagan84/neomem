
// CViewHeader
// This view contains the tab view on the right side of the screen, and also displays 
// an optional band at the top with the name of the currently selected object.
// Date: November 1999
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "ViewTabs.h"


class CViewHeader : public CViewEx
{
	DECLARE_DYNCREATE(CViewHeader)

// Construction
protected:
	CViewHeader();           // protected constructor used by dynamic creation
	virtual ~CViewHeader();

// Operations

// Attributes
public:
	CViewTabs* m_pviewTabs; // tabs view contained by this view
private:
	int m_nHeight; // height of this header view in pixels

// Implementation
protected:
	void RecalcLayout(BOOL bRepaint);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewHeader)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewHeader)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};




