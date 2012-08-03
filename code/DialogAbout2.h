
// CDialogAbout2
// uses rtf control
//-----------------------------------------------------------------------------------------------------------------




#pragma once


#include "RichEditCtrlEx.h"


class CDialogAbout2 : public CDialog
{
// Construction
public:
	CDialogAbout2(CWnd* pParent = NULL);   // standard constructor

// Operations
public:
	void LoadTab();

// Attributes
protected:
	CRichEditCtrlEx m_rtf;
//	CRichEditCtrl m_rtf;
//	CWnd m_wnd;
	CString m_strAbout;
	CString m_strCredits;
	HACCEL m_hAccel;

// Dialog Data
private: 
	//{{AFX_DATA(CDialogAbout2)
	enum { IDD = IDD_ABOUT2 };
	CTabCtrl	m_tbc;
	CButton	m_btnClose;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogAbout2)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogAbout2)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnClose();
	afx_msg void OnBtnCredits();
	afx_msg void OnSelchangeTbs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTabAbout();
	afx_msg void OnTabCredits();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

