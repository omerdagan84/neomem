// DialogPassword.h : header file


#pragma once



class CDialogPassword : public CDialog
{
// Construction
public:
	CDialogPassword(CWnd* pParent = NULL);   // standard constructor

// Public Attributes
public:
	CString m_strValidPassword;
	CString m_strFilename;
	CString	m_strPassword;

// Dialog Data
private:
	//{{AFX_DATA(CDialogPassword)
	enum { IDD = IDD_PASSWORD };
	CEdit	m_txtFilename;
	CStatic	m_lblFilename;
	CEdit	m_txtPassword;
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
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogPassword)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


