
// CDialogViews
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "Resource.h"
#include "ListCtrlEx.h"

#include "BDataViews.h"
#include "NeoDoc.h"


class CDialogViews : public CDialog
{
// Construction
public:
	CDialogViews(CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
	CNeoDoc* m_pDoc;
	BDataViews* m_pdatViews;
	int m_nTab;

// Dialog Data
	//{{AFX_DATA(CDialogViews)
	enum { IDD = IDD_VIEWS };
	CListCtrlEx	m_lvw;
	CStatic	m_lblInstructions;
	CButton	m_btnUp;
	CButton	m_btnHelp;
	CButton	m_btnDown;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogViews)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogViews)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnHelp();
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

