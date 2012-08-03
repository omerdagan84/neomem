
// CDialogEditDate
// Dialog for editing dates.
// Note: Requires Comctl32.dll 4.70 (for calendar control)
//-----------------------------------------------------------------------------------------------------------------



#pragma once

#include "resource.h"


class CDialogEditDate : public CDialog
{
// Construction
public:
	CDialogEditDate(CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
	BOOL m_bUseTime;
	COleDateTime m_dtDate;
	COleDateTime m_dtTime;

// Dialog Data
private: 
	//{{AFX_DATA(CDialogEditDate)
	enum { IDD = IDD_EDIT_DATE };
	CButton	m_btnHelp;
	CDateTimeCtrl	m_wndTime;
	CMonthCalCtrl	m_wndDate;
	CButton	m_chkTime;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEditDate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogEditDate)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkTime();
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
