
// CPageClassContents
// Contents page in the Class Wizard
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "ListCtrlEx.h"
#include "Resource.h"
#include "XComboBoxEx.h"
class CSheetWizard;

#include "BDataColumns.h"
class BObject;
class CNeoDoc;


class CPageClassContents : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageClassContents)

// Construction
public:
	CPageClassContents();
	~CPageClassContents();

public:
	void LoadData();
private:
	void UpdateControls();

// Dialog Data
	//{{AFX_DATA(CPageClassContents)
	enum { IDD = IDD_CLASS_CONTENTS };
	XComboBoxEx	m_cboClass;
	CStatic	m_lblSorted;
	CStatic	m_lblDefaultClass;
	CStatic	m_lblTitle;
	CStatic	m_lblInstructions;
	CButton m_optChildrenYes;
	CButton m_optChildrenNo;
	CButton m_optSortYes;
	CButton m_optSortNo;
	//}}AFX_DATA

private:
	CNeoDoc* m_pDoc;
	CSheetWizard* m_psh;
	BObject* m_pobj;
//	BDataColumns* m_pdatColumns;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageClassContents)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageClassContents)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptChildrenYes();
	afx_msg void OnOptChildrenNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

