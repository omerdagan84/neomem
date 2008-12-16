// DialogMessage2.h : header file


#pragma once



class CDialogMessage2 : public CDialog
{
// Construction
public:
	CDialogMessage2(CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
	CString m_strCaption;

// Dialog Data
	//{{AFX_DATA(CDialogMessage2)
	enum { IDD = IDD_MESSAGE2 };
	CButton	m_btnClose;
	CStatic	m_lblMessage;
	CButton	m_chkOption;
	BOOL	m_bShowMessage;
	CString	m_strMessage;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogMessage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogMessage2)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnClose();
	afx_msg void OnChkOption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


