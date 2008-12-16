
// CStatusBarEx
// Custom status bar window.
//-----------------------------------------------------------------------------------------------------------------
// Contains clock (CClock)
// Progress bar control

// See SetStatusBarText





#pragma once

#include "Clock.h"


class CStatusBarEx : public CStatusBar
{
// Construction
public:
	CStatusBarEx();
	virtual ~CStatusBarEx();

// Operations
public:
	void Resize();

// Attributes
public:
	CProgressCtrl m_wndProgress;
private:
	CClock m_clock;
//	CEdit m_txt;
	int m_nClockPane;
	ULONG m_lngProgressLeft;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusBarEx)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CStatusBarEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
//	afx_msg void OnUpdateDateIndicator(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()
};



