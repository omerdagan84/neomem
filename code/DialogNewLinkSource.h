
// CDialogNewLinkSource
// This dialog lets the user create a new Link Source folder.
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "resource.h"
#include "XComboBoxEx.h"
class CNeoDoc;


class CDialogNewLinkSource : public CDialog
{
// Construction
public:
	CDialogNewLinkSource(CWnd* pParent = NULL);   // standard constructor

// Public Attributes
public:
	CNeoDoc* m_pDoc;

	ULONG m_lngClassID; // [out]
	CString m_strFolderName; // [out]

// Dialog Data
	//{{AFX_DATA(CDialogNewLinkSource)
	enum { IDD = IDD_NEW_LINK_SOURCE };
	CButton	m_btnHelp;
	CEdit	m_txtFolderName;
	CStatic	m_lblFolder;
	CStatic	m_lblClass;
	CButton	m_btnNewClass;
	XComboBoxEx m_cboClass;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogNewLinkSource)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogNewLinkSource)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnNewClass();
	afx_msg void OnSelChangeCboClass();
	afx_msg void OnBtnHelp();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



