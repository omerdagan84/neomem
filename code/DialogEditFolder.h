
// CDialogEditFolder
// Lets user choose name and contents for a folder.
// Date: February 2000
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "Resource.h"
#include "ListCtrlEx.h"
#include "XComboBoxEx.h"

#include "BDataColumns.h"
class BObject;
class CNeoDoc;



class CDialogEditFolder : public CDialog
{

// Construction
public:
	CDialogEditFolder(CWnd* pParent = NULL);   // standard constructor
	~CDialogEditFolder();

// Operations
public:
	BObject* AddNewClass();
	int DoModalAddFolder(CString& strName, CString& strDescription, BObject* pobjParent, BObject* pobjDefaultClass);
	int DoModalEditFolder(CString& strName, CString& strDescription, BObject* pobjParent, BObject* pobjDefaultClass);

// Attributes
public:
//	ULONG m_lngClassID; // new object class id
//	BObject* m_pobjClass; 
	BObject* m_pobjParent; // parent of new object - used for display, and to get default class
	BObject* m_pobjDefaultClass;  // ie contents of folder
	CNeoDoc* m_pDoc;
	BOOL m_bAddMode; // Add mode or edit mode
	BOOL m_bLocationComboFilled;
//	BOOL m_bNameChanged;
//	BOOL m_bDescriptionChanged;


// Implementation
private:
//	void FillDefaultClassCombo();
//	void OnClassChanged();

// Dialog Data
public:
	//{{AFX_DATA(CDialogEditFolder)
	enum { IDD = IDD_EDIT_FOLDER };
	CString m_strName;
	CString	m_strDescription;
	CStatic	m_lblDefaultClass;
	CStatic	m_lblDescription;
//	CStatic	m_lblClass;
	CStatic	m_lblLocation;
	CStatic	m_lblInstructions;
	CEdit	m_txtName;
	CEdit	m_txtDescription;
	CListCtrlEx	m_lvw;
	XComboBoxEx	m_cboLocation;
//	XComboBoxEx	m_cboDefaultClass;
	CButton	m_btnNewClass;
	CButton	m_btnHelp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEditFolder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogEditFolder)
	virtual BOOL OnInitDialog();
//	afx_msg void OnChangeEditName();
//	afx_msg void OnNotifyItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDropdownCboLocation();
//	afx_msg void OnSelchangeCboLocation();
	afx_msg void OnBtnNewClass();
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



