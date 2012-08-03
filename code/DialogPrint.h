
// CDialogPrint
//-----------------------------------------------------------------------------------------------------------------



#pragma once

#include "ListCtrlEx.h"
class CNeoDoc;


class CDialogPrint : public CDialog
{
// Construction
public:
	CDialogPrint(CWnd* pParent = NULL);   // standard constructor

// Operations
public:
	int DoModalParameters(ULONG lngCurrentViewID);

// Public Attributes
public:
	CNeoDoc* m_pDoc;
	ULONG m_lngSelectedViewID;
	BOOL	m_bPrintHeaderFooter;

// Dialog Data
private:
	//{{AFX_DATA(CDialogPrint)
	enum { IDD = IDD_PRINT };
	CButton	m_chkHeaderFooter;
	CListCtrlEx	m_lvw;
	CButton	m_chkLimit;
	CButton	m_btnHelp;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPrint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogPrint)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

