
// CDialogImport
// This dialog is used to select data source for import/export.
// Date: July 2001
//-----------------------------------------------------------------------------------------------------------------



#pragma once


class CDialogImport : public CDialog
{
// Construction
public:
	CDialogImport(CWnd* pParent = NULL);   // standard constructor

// Attributes

// Dialog Data
private:
	//{{AFX_DATA(CDialogImport)
	enum { IDD = IDD_IMPORT };
	CButton	m_btnSelectFile;
	CEdit	m_txtFile;
	CListBox	m_lst;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogImport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	CString m_strFilename;
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogImport)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnSelectFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



