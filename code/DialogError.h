
// CDialogError
// This dialog reports an error to the user.
// Based on code from CodeGuru.com.
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "Error.h"


class CDialogError : public CDialog
{
// Construction
public:
	// standard constructor
	CDialogError(CWnd* pParent = NULL);

// Public Attributes
public:
	CError* m_pe; // pointer to our error object
	BOOL m_bEnableSendButton;

// Operations
public:
	void UpdateDescription();


// Dialog Data
private:
  //{{AFX_DATA(CDialogError)
	enum { IDD = IDD_ERROR };
	CEdit	m_txtDescription;
	CButton	m_btnExit;
	CButton	m_btnContinue;
	CStatic	m_lblTitle;
	CButton m_btnSend;
	CEdit m_txtErrorMessage;
	//}}AFX_DATA

// Overrides
// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogError)
	protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation 
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogError)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSend();
	afx_msg void OnBtnCopy();
	afx_msg void OnBtnContinue();
	afx_msg void OnBtnExit();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};





