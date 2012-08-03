
// CViewHomeInner
// Scrollable inner portion of the Home View.
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "FontEx.h"
#include "StaticEx.h"

#include "NeoDoc.h"


class CViewHomeInner : public CScrollView
{
	DECLARE_DYNCREATE(CViewHomeInner)

//protected: // Note: Made these public so could create in CViewHome!
public:
	CViewHomeInner();           // protected constructor used by dynamic creation
	virtual ~CViewHomeInner();


// Operations
public:
	void RecalculateLayout(int cx = 0, int cy = 0);

// Public Attributes
public:
	CNeoDoc* m_pDoc;
	CRect m_rMargins;
	CStaticEx m_lblDescription;
	CStaticEx m_lblTip;
	CStaticEx m_lblTip2;
	CStaticEx m_lblPrevTip;
	CStaticEx m_lblDot;
	CStaticEx m_lblNextTip;


// Implementation
private:
	void GetFileDescription();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewHomeInner)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
private:
	//{{AFX_MSG(CViewHomeInner)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLblPrev();
	afx_msg void OnLblNext();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
};

