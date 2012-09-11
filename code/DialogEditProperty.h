
// CDialogEditProperty
// This dialog lets the user create or edit a property definition object. 
//--------------------------------------------------------------------------------------------------------------------


#pragma once


#include "XComboBoxEx.h"
#include "resource.h"


class BObject;
class BDoc;


class CDialogEditProperty : public CDialog
{
// Construction
public:
	CDialogEditProperty(CWnd* pParent = NULL);   // standard constructor

// Operations
public:
	BOOL DoModalAdd();
	int DoModalEdit(LPCTSTR strName, LPCTSTR strDescription);

// Public Attributes
public:

	enum eMode {modeNone = 0, modeAddProperty = 1, modeEditProperty = 2};
	eMode m_nMode;

	BObject* m_pobjPropertyDef; // need this in edit mode!
	BObject* m_pobjPropertyType;
	BObject* m_pobjLinkSource;
	BObject* m_pobjAdditionalProperty;
	BObject* m_pobjUnits;
	BOOL m_bLimitLinks;
	BOOL m_bDisplayHierarchy;
	BOOL m_bAdditionalProperty;
	BOOL m_bSystemProperty;
	CString	m_strName;
	CString	m_strDescription;
	

// Implementation	
private:
	BDoc* m_pDoc;
private:
	void FillUnitsCombo();
	void FillPropertyCombo();
	void FillLinkSourceCombo();

// Dialog Data
private:
	//{{AFX_DATA(CDialogEditProperty)
	enum { IDD = IDD_EDIT_PROPERTY };
	CEdit	m_txtTypeDescription;
	XComboBoxEx m_cboType;
	XComboBoxEx m_cboLinkSource;
	XComboBoxEx m_cboAdditionalProperty;
	XComboBoxEx m_cboUnits;
	CButton	m_chkAdditionalProperty;
	CButton	m_chkLimitLinks;
	CButton	m_chkDisplayHierarchy;
	CStatic	m_lblAdditionalProperty;
	CStatic	m_lblUnits;
	CStatic	m_lblLinkSource;
	CStatic	m_lblInstructions;
	CEdit	m_txtDescription;
	CEdit	m_txtName;
	CButton m_btnNewLinkSource;
	CButton	m_btnHelp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEditProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogEditProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeCboType();
	afx_msg void OnSelChangeCboLinkSource();
	afx_msg void OnBtnNewLinkSource();
	afx_msg void OnBtnHelp();
	afx_msg void OnChkAdditionalProperty();
	afx_msg void OnChkHierarchy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




