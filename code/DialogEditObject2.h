
// CDialogEditObject2
// Lets user choose class and name for new object.
// Date: February 2000
//-----------------------------------------------------------------------------------------------------------------



#pragma once



#include "Resource.h"
#include "ListCtrlEx.h"
#include "XComboBoxEx.h"

#include "BDataColumns.h"
class BObject;
class CNeoDoc;


class CDialogEditObject2 : public CDialog
{

// Construction
public:
	CDialogEditObject2(CWnd* pParent = NULL);   // standard constructor
	~CDialogEditObject2();

// Operations
public:
	int DoModalAddObject(CString& strName, BObject* pobjParent, BObject* pobjClass, BObject* pobjDefaultClass);
	int DoModalEditObject(CString& strName, BObject* pobjParent, BObject* pobjClass, BObject* pobjDefaultClass);

// Public Attributes
public:
	ULONG m_lngClassID; // new object class id
	BObject* m_pobjParent; // parent of new object - used for display, and to get default class
	BObject* m_pobjClass; 
	BObject* m_pobjDefaultClass; 
	CString m_strName;


// Implementation
private:
	CNeoDoc* m_pDoc;
	BOOL m_bAddMode; // Add mode or edit mode
	BOOL m_bLocationComboFilled;
	BOOL m_bNameChanged;

private:
	void FillDefaultClassCombo();
	void OnClassChanged();

// Dialog Data
private:
	BObject* AddNewClass();
	//{{AFX_DATA(CDialogEditObject2)
	enum { IDD = IDD_EDIT_OBJECT2 };
	CButton	m_btnNewClass2;
	CButton	m_btnNewClass;
	CStatic	m_lblClass;
	CStatic	m_lblLocation;
	XComboBoxEx	m_cboLocation;
	CStatic	m_lblInstructions;
	CEdit	m_txtName;
	CEdit	m_txtDescription;
	CListCtrlEx	m_lvw;
	CStatic	m_lblDefaultClass;
	XComboBoxEx	m_cboDefaultClass;
	CButton	m_btnHelp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEditObject2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogEditObject2)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditName();
	afx_msg void OnNotifyItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnHelp();
	afx_msg void OnDropdownCboLocation();
	afx_msg void OnBtnNewClass();
	afx_msg void OnBtnNewClass2();
	afx_msg void OnSelchangeCboLocation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



