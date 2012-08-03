
// CPageClassProperties
// Properties page in the Class Wizard
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "ListCtrlEx.h"
#include "resource.h"
class CSheetWizard;

#include "NeoDoc.h"
class BObject;
class BDataColumns;


class CPageClassProperties : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageClassProperties)

// Construction
public:
	CPageClassProperties();
	~CPageClassProperties();

public:
	void LoadData();

// Dialog Data
private:
	//{{AFX_DATA(CPageClassProperties)
	enum { IDD = IDD_CLASS_PROPERTIES };
	CListCtrlEx	m_lvw;
	CStatic	m_lblTitle;
	CButton	m_btnEdit;
	CButton	m_btnDelete;
	CButton	m_btnAdd;
	CStatic	m_lblInstructions;
	//}}AFX_DATA

private:
	CNeoDoc* m_pDoc;
	CSheetWizard* m_psh;
	BObject* m_pobj;
	BObject* m_pobjPropertiesFolder;
	BOOL m_bInitialized;
//	BDataColumns* m_pdatColumns;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageClassProperties)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageClassProperties)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnEdit();
	afx_msg void OnBtnDelete();
	afx_msg void OnDblClkLvw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

