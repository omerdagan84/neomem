
// CPageClassInheritance
// Inheritance page in the Class Wizard
// (not used)
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "SheetWizard.h"
#include "TreeCtrlEx.h"

#include "NeoDoc.h"
#include "BObject.h"


class CPageClassInheritance : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageClassInheritance)

// Construction
public:
	CPageClassInheritance();
	~CPageClassInheritance();

// Dialog Data
	//{{AFX_DATA(CPageClassInheritance)
	enum { IDD = IDD_CLASS_INHERITANCE };
	CTreeCtrlEx	m_tvw;
	CStatic	m_lblTitle;
	CStatic	m_lblInstructions;
	CButton	m_chkInherit;
	//}}AFX_DATA

	CNeoDoc* m_pDoc;
	CSheetWizard* m_psh;
	BObject* m_pobj;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageClassInheritance)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageClassInheritance)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClkTvw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkInherit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
