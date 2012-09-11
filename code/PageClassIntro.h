
// CPageClassIntro
// Intro page in the Class Wizard
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "SheetWizard.h"
#include "ListCtrlEx.h"

#include "BDoc.h"
#include "BObject.h"
#include "BDataColumns.h"


class CPageClassIntro : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageClassIntro)

// Construction
public:
	CPageClassIntro();
	~CPageClassIntro();

// Dialog Data
private:
	//{{AFX_DATA(CPageClassIntro)
	enum { IDD = IDD_CLASS_INTRO };
	CButton	m_btnDelete;
	CListCtrlEx m_lvw;
	CButton m_optAdd;
	CButton m_optEdit;
	//}}AFX_DATA
 
private:
	BDoc* m_pDoc;
	CSheetWizard* m_psh;
	BObject* m_pobj;
//	BDataColumns* m_pdatColumns;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageClassIntro)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageClassIntro)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptEdit();
	afx_msg void OnOptAdd();
	afx_msg void OnBtnDelete();
	afx_msg void OnSetFocusLvwClass(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotifyDblClkLvw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangedLvw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

