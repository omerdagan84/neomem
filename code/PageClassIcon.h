
// CPageClassIcon
// Icon page in the Class Wizard
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "SheetWizard.h"
#include "ListCtrlEx.h"
#include "resource.h"

#include "BDataColumns.h"
class BObject;
class BDoc;


class CPageClassIcon : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageClassIcon)

// Construction
public:
	CPageClassIcon();
	~CPageClassIcon();

// Dialog Data
private:
	//{{AFX_DATA(CPageClassIcon)
	enum { IDD = IDD_CLASS_ICON };
	CListCtrlEx	m_lvw;
	CStatic	m_lblTitle;
	CButton	m_btnRename;
	CButton	m_btnDelete;
	CButton	m_btnImport;
	CButton	m_btnDefault;
	CStatic	m_lblInstructions;
	//}}AFX_DATA

private:
	BDoc* m_pDoc;
	CSheetWizard* m_psh;
	BObject* m_pobj;
	BObject* m_pobjIconFolder;
	ULONG m_lngDefaultIconID;
//	BDataColumns* m_pdatColumns;
	BObject* m_pobjStart;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageClassIcon)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageClassIcon)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnImport();
	afx_msg void OnBtnDefault();
	afx_msg void OnDblClkLvw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnRename();
	afx_msg void OnBtnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

