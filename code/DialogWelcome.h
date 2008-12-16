
// CDialogWelcome
// Welcome dialog displays message to user etc.
//-----------------------------------------------------------------------------------------------------------------



#pragma once

#include "RichEditCtrlEx.h"


class CDialogWelcome : public CDialog
{
// Construction
public:
	CDialogWelcome(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogWelcome)
	enum { IDD = IDD_WELCOME };
	CStatic	m_picHeader;
	CButton	m_btnNewFile;
	CButton	m_btnExistingFile;
	CButton	m_btnExample;
	CRichEditCtrlEx	m_rtf;
	CButton	m_chkShowDialog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogWelcome)
	public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogWelcome)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnExample();
	afx_msg void OnBtnExistingFile();
	afx_msg void OnBtnNewFile();
	afx_msg void OnPaint();
	afx_msg void OnBtnClose();
	//}}AFX_MSG
//	afx_msg LRESULT OnDDEExecute(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnBtnHelp();
	DECLARE_MESSAGE_MAP()
};

