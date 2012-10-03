
// CDialogEditString
// This dialog lets the user edit a string (more room than CDialogEditName).
//-----------------------------------------------------------------------------------------------------------------


#pragma once

#include "resource.h"

#include "EditEx.h"


class CDialogEditString : public CDialog
{
// Construction
public:
	CDialogEditString(CWnd* pParent = NULL);   // standard constructor

// Public Attributes
public:
	CString	m_strValue;
	CString m_strCaption;
	CString m_strInstructions;

// Dialog Data
private:
	//{{AFX_DATA(CDialogEditString)
	enum { IDD = IDD_EDIT_STRING };
	CStatic	m_lblInstructions;
	CEditEx	m_txtValue;
	//}}AFX_DATA


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

