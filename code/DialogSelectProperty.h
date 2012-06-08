
// CDialogSelectProperty
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "ListCtrlEx.h"
#include "XComboBoxEx.h"

#include "NeoDoc.h"
#include "BObject.h"


class CDialogSelectProperty : public CDialog
{
// Construction
public:
	CDialogSelectProperty(CWnd* pParent = NULL);   // standard constructor


// Operations
public:
	void AddProperties();

// Attributes
public:
	ULONG m_lngSelectedID;
	BObject* m_pobjDefaultClass;
	BOOL m_bShowAll;
	UINT m_nMode; // 0 for default or IDD_PROPERTY_WIZARD
private:
	CNeoDoc* m_pDoc;
	BObject* m_pobjClasses;
	BObject* m_pobjProperties;


// Dialog Data
	//{{AFX_DATA(CDialogSelectProperty)
	enum { IDD = IDD_SELECT_PROPERTY };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CListCtrlEx	m_lvw;
	CStatic	m_lblInstructions;
	XComboBoxEx	m_cboClass;
	CButton	m_btnHelp;
	CButton	m_btnEdit;
	CButton	m_btnDelete;
	CButton	m_btnAdd;
	CButton m_optShowAll;
	CButton m_optShowSelected;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSelectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogSelectProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnHelp();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnEdit();
	afx_msg void OnBtnDelete();
	afx_msg void OnSelChangeCboClass();
	afx_msg void OnOptShowAll();
	afx_msg void OnOptShowSelected();
	virtual void OnOK();
	afx_msg void OnBtnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


