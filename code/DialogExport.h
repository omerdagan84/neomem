

#pragma once

//import "bobject.h"
//#import "FileFormat.h"

class BObject;
class CFileFormat;

class CDialogExport : public CDialog
{
// Construction
public:
	CDialogExport(CWnd* pParent = NULL);   // standard constructor

// Public Attributes
public:
	BObject* m_pobj;
	CFileFormat* m_ff;
	CString m_strFormat;
	CString	m_strItem;
	CString	m_strFilename;

// Dialog Data
private:
	//{{AFX_DATA(CDialogExport)
	enum { IDD = IDD_EXPORT };
	CStatic	m_lblItem;
	CButton	m_chkSystem;
	CButton	m_chkRecurse;
	CEdit	m_txtFilename;
	CComboBox	m_cboFormat;
	CButton	m_btnChooseFile;
	BOOL	m_bRecurse;
	BOOL	m_bSystem;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogExport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogExport)
	virtual void OnOK();
	afx_msg void OnBtnChooseFile();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


