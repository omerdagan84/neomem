// DialogPassword.h : header file


#pragma once



class CDialogPassword : public CDialog
{
// Construction
public:
	CDialogPassword(CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
	CString m_strValidPassword;
	CString m_strFilename;

// Dialog Data
	//{{AFX_DATA(CDialogPassword)
	enum { IDD = IDD_PASSWORD };
	CEdit	m_txtFilename;
	CStatic	m_lblFilename;
	CEdit	m_txtPassword;
	CString	m_strPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPassword)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL Validate();

	// Generated message map functions
	//{{AFX_MSG(CDialogPassword)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


