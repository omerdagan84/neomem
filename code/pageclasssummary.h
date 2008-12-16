
// CPageClassSummary
// Summary page in the Class Wizard
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "SheetWizard.h"
//#include "RichEditCtrlEx.h"

#include "NeoDoc.h"
#include "BObject.h"



class CPageClassSummary : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageClassSummary)

// Construction
public:
	CPageClassSummary();
	~CPageClassSummary();

// Dialog Data
	//{{AFX_DATA(CPageClassSummary)
	enum { IDD = IDD_CLASS_SUMMARY };
	CStatic	m_lblTitle;
	CStatic	m_lblInstructions;
	//}}AFX_DATA

	CNeoDoc* m_pDoc;
	CSheetWizard* m_psh;
	BObject* m_pobj;
//	CRichEditCtrlEx m_rtf;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageClassSummary)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageClassSummary)
	virtual BOOL OnInitDialog();
//	afx_msg void OnSetFocusRtf(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

