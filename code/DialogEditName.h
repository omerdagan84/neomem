
// CDialogEditName
// This dialog lets the user edit a name (a short string) 
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "Resource.h"



class CDialogEditName : public CDialog {

// Construction
public:
	CDialogEditName(CWnd* pParent = NULL);   // standard constructor


// Public Attributes
public:
	CString m_strCaption;
	CString m_strInstructions;
	CString	m_strName;
	BOOL m_bPassword;

// Dialog Data
private: 
	//{{AFX_DATA(CDialogEditName)
	enum { IDD = IDD_EDIT_NAME };
	CStatic	m_lblInstructions;
	CEdit	m_txtName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEditName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogEditName)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

