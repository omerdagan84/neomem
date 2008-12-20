
// CSheetWizard
// This class encapsulates the Class Wizard. 
// Derived from our class CPropertySheetEx2.
//--------------------------------------------------------------------------------



#pragma once


#include "PropertySheetEx2.h"

#include "BObject.h"


class CSheetWizard : public CPropertySheetEx2
{
	DECLARE_DYNAMIC(CSheetWizard)

// Construction
public:
	CSheetWizard();
	CSheetWizard(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheetWizard(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheetWizard();

// enums
public:
	enum eAddEditMode {modeAddOrEdit = 1, modeAddOnly = 2, modeEditOnly = 3};
//	enum eWizardMode {modeAddClass = 1, modeEditClass = 2};
//	enum eWizardMode {modeClass = 1, modeProperty = 2};

// Operations
public:
	void SetClassWizardTitle();
	void SetEditClass(BObject* pobjClass);
//	int DoModalParameters(eWizardMode nMode, BObject* pobj = 0);
	int DoModalParameters(int nAddEditMode, BObject* pobjClass = 0);

// Attributes
public:
	CNeoDoc* m_pDoc;
	BOOL m_bAdd; // True if user picked Add option in first page

	BObject* m_pobj; // Pointer to the object we're editing (add or edit)
	BObject* m_pobjAdd; // New object used in Add mode
	BObject* m_pobjEdit; // New object used in Edit mode (copy of original item user picked)
	BObject* m_pobjEditOriginal; // Item user picked to Edit in first page, if any
//	BOOL m_bAddFolder; // True if user chose to add a folder in page 60
//	CString m_strFolderName; // Name of folder to add
//	BObject* m_pobjFolderLocation; // Location of folder to add
//	BOOL m_bAutosort; // True if user chose autosort in page 50

	int m_nAddEditMode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetWizard)
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
protected:
	//{{AFX_MSG(CSheetWizard)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
