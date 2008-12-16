
// DialogUpgradeFile.h : header file

#pragma once


#include "RichEditCtrlEx.h"


class CDialogUpgradeFile : public CDialog
{
// Construction
public:
	CDialogUpgradeFile(CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
	CString m_strFile;

// Dialog Data
	//{{AFX_DATA(CDialogUpgradeFile)
	enum { IDD = IDD_UPGRADE_FILE };
		// NOTE: the ClassWizard will add data members here
	CRichEditCtrlEx	m_rtf;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogUpgradeFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogUpgradeFile)
	afx_msg void OnBtnHelp();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



