
// CDialogEditString
// This dialog lets the user edit a string (more room than CDialogEditName).
//-----------------------------------------------------------------------------------------------------------------




#pragma once


#include "EditEx.h"


class CDialogEditString : public CDialog
{
// Construction
public:
	CDialogEditString(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDialogEditString)
	enum { IDD = IDD_EDIT_STRING };
	CStatic	m_lblInstructions;
	CEditEx	m_txtValue;
	CString	m_strValue;
	//}}AFX_DATA

	CString m_strCaption;
	CString m_strInstructions;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEditString)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogEditString)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

