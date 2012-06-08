
// DialogSaveOptions.h : header file

//. rename to DialogFileSecurity


#pragma once

#include "StringEx.h"



class CDialogSaveOptions : public CDialog
{
// Construction
public:
	CDialogSaveOptions(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDialogSaveOptions)
	enum { IDD = IDD_FILE_SECURITY };
	CStatic	m_lblPassword2;
	CEdit	m_txtPassword2;
	CComboBox	m_cboEncryption;
	CEdit	m_txtPassword;
	CStatic	m_lblPassword;
	CStringEx	m_strPassword;
	int		m_nEncryptionType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSaveOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL Validate();
	// Generated message map functions
	//{{AFX_MSG(CDialogSaveOptions)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnHelp();
	afx_msg void OnCloseupCboEncryption();
	virtual void OnOK();
	afx_msg void OnSelchangeCboEncryption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



