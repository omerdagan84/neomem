
// CViewHome
// This view is the first view the user sees, and contains the tip of the day.
//-----------------------------------------------------------------------------------------------------------------

#pragma once

#include "StaticEx.h"
#include "ViewEx.h"
#include "ViewHomeInner.h"


class CViewHome : public CViewEx
{
	DECLARE_DYNCREATE(CViewHome)

// Construction
protected:
	CViewHome();           // protected constructor used by dynamic creation
	virtual ~CViewHome();

// Operations
public:
	void DrawLines();

// Attributes
protected:
	int m_nFooterHeight;
	int m_nHeaderHeight;

	// Controls
	CStaticEx m_lblHeader;
	CStaticEx m_lblFooter;
//	CStaticEx m_lblFooter2;
	CStaticEx m_lblWebsite;
	CStaticEx m_lblDot;
	CStaticEx m_lblEmail;

	CViewHomeInner* m_pview;
//	CRichEditCtrlEx m_rtf;

//	CString m_strFileDescription;
//	CString m_strTip;
//	CString m_strFileSize;
//	CString m_strObjects;


// Implementation
protected:
	void GetFileDescription();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewHome)
	public:
	protected:
	virtual void OnDraw(CDC* pDC);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CViewHome)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLblWebsite();
	afx_msg void OnLblEmail();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




