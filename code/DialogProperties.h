
// CDialogProperties
//-----------------------------------------------------------------------------------------------------------------


#pragma once

#include "ListCtrlEx.h"


class CDialogProperties : public CDialog
{
// Construction
public:
	CDialogProperties(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
private:
	//{{AFX_DATA(CDialogProperties)
	enum { IDD = IDD_PROPERTIES };
	CListCtrlEx	m_lvw;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogProperties)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

