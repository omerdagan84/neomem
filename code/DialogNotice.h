
// CDialogNotice
// message dialog
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "EditEx.h"


class CDialogNotice : public CDialog
{
// Construction
public:
	CDialogNotice(CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
	CString m_strMessage;

// Dialog Data
	//{{AFX_DATA(CDialogNotice)
	enum { IDD = IDD_NOTICE };
	CButton	m_btnDownload;
	CEditEx	m_txt;
	CButton	m_btnClose;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogNotice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogNotice)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnClose();
	afx_msg void OnBtnDownload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


