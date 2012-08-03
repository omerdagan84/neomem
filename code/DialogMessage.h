
// CDialogMessage
// A generic message dialog
// Date: March 2001
//-----------------------------------------------------------------------------------------------------------------


#pragma once

#include "RichEditCtrlEx.h"


class CDialogMessage : public CDialog
{
// Construction
public:
	CDialogMessage(CWnd* pParent = NULL);   // standard constructor

// Operations
public:
	BOOL DoModalParameters(CString strMessage);


// Dialog Data
private:
	//{{AFX_DATA(CDialogMessage)
	enum { IDD = IDD_MESSAGE };
//	CRichEditCtrlEx	m_rtf;
	CRichEditCtrl	m_rtf;
	CStatic	m_picHeader;
	CButton	m_chkOption;
	CButton	m_btnClose;
	CButton	m_btnA;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogMessage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogMessage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



