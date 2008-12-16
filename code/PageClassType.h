
// CPageClassType
// Type page in the Class Wizard
//-----------------------------------------------------------------------------------------------------------------



#pragma once



#include "SheetWizard.h"

#include "NeoDoc.h"
#include "BObject.h"



class CPageClassType : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageClassType)

// Construction
public:
	CPageClassType();
	~CPageClassType();

// Dialog Data
	//{{AFX_DATA(CPageClassType)
	enum { IDD = IDD_CLASS_TYPE };
	CComboBox	m_cboNameType;
	CStatic	m_lblInstructions;
	CStatic	m_lblTitle;
	//}}AFX_DATA

	CNeoDoc* m_pDoc;
	CSheetWizard* m_psh;
	BObject* m_pobj;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageClassType)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageClassType)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

