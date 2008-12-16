
// CPageClassName
// Name page in the Class Wizard
//-----------------------------------------------------------------------------------------------------------------


#pragma once

#include "SheetWizard.h"
#include "TreeCtrlEx.h"

#include "NeoDoc.h"
#include "BObject.h"



class CPageClassName : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageClassName)

// Construction
public:
	CPageClassName();
	~CPageClassName(); 
 
// Dialog Data
	//{{AFX_DATA(CPageClassName)
	enum { IDD = IDD_CLASS_NAME };
	CStatic	m_lblTitle;
	CEdit	m_txtName;
	CEdit	m_txtDescription;
	CStatic	m_lblInstructions;
	CString	m_strDescription;
	CString	m_strName;
	//}}AFX_DATA

	CNeoDoc* m_pDoc;
	CSheetWizard* m_psh;
	BObject* m_pobj;

	CTreeCtrlEx m_tvw;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageClassName)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageClassName)
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

