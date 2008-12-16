
// CDialogException
// test dialog
//-----------------------------------------------------------------------------------------------------------------



#pragma once

#include "resource.h"


class CDialogException : public CDialog
{
// Construction
public:
	CDialogException(CWnd* pParent = NULL);   // standard constructor



// Dialog Data
	//{{AFX_DATA(CDialogException)
	enum { IDD = IDD_EXCEPTION };
	CListBox	m_lst;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogException)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogException)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

