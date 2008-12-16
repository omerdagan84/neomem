
// CDialogEditPersonName
// This dialog lets the user edit a BDataPersonName object (first, last name, etc.)
//-----------------------------------------------------------------------------------------------------------------



#pragma once

#include "resource.h"


class CDialogEditPersonName : public CDialog
{
// Construction
public:
	CDialogEditPersonName(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDialogEditPersonName)
	enum { IDD = IDD_EDIT_PERSON_NAME };
	CComboBox	m_cboTitle;
	CComboBox	m_cboSuffix;
	CButton	m_btnHelp;
	CString	m_strFirst;
	CString	m_strLast;
	CString	m_strMiddle;
	CString	m_strNickname;
	CString	m_strSuffix;
	CString	m_strTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEditPersonName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogEditPersonName)
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

