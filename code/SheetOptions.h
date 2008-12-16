
// CSheetOptions
// This class encapsulates the Options dialog. 
// Derived from our class CPropertySheetEx2.
//-----------------------------------------------------------------------------------------------------------------

#pragma once

#include "PropertySheetEx2.h"


class CSheetOptions : public CPropertySheetEx2
{
	DECLARE_DYNAMIC(CSheetOptions)

// Construction
public:
	CSheetOptions(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheetOptions(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheetOptions();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetOptions)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
protected:
	//{{AFX_MSG(CSheetOptions)
	afx_msg void OnTabDisplay();
	afx_msg void OnTabFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


