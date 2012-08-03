
// CDialogEx

#pragma once


//#include "NeoDoc.h"
class CNeoDoc;


class CDialogEx : public CDialog
{
// Construction
public:
//	CDialogEx(CWnd* pParent = NULL);   // standard constructor
	CDialogEx(UINT nTemplateID, CWnd* pParent = NULL);   // standard constructor

// Attributes
protected:
	CNeoDoc* m_pDoc;

// Dialog Data
	//{{AFX_DATA(CDialogEx)
//	enum { IDD = IDD_EDIT_NUMBER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
	//{{AFX_MSG(CDialogEx)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




