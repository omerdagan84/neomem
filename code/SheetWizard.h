
// CSheetWizard
// This class encapsulates the Class Wizard. 
// Derived from our class CPropertySheetEx2.
//--------------------------------------------------------------------------------

/*
	called by 

	OnCmdEditClasses - called by ID_EDIT_CLASS_WIZ command
	UIEditClass - called by edit link dialog. 
	UIAddNewClass - called by various dialogs. 

*/

//. rename to CSheetClassWizard


#pragma once


#include "BObject.h"
#include "PropertySheetEx2.h"


class CSheetWizard : public CPropertySheetEx2 {

	DECLARE_DYNAMIC(CSheetWizard)

// Construction
public:
	CSheetWizard();
	CSheetWizard(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheetWizard(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheetWizard();

// Enums
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


// Public Attributes
public: 
	BDoc* m_pdoc;
	BOOL m_bAddMode; // True if user picked Add option in first page

	BObject* m_pobj; // Pointer to the object we're editing (add or edit)
	BObject* m_pobjAdd; // New object used in Add mode
//x	BObject& m_objAdd; // New object used in Add mode
	int m_nAddEditMode;

// Private Attributes
private:
	BObject* m_pobjEdit; // New object used in Edit mode (copy of original item user picked)
//x	BObject& m_objEdit; // New object used in Edit mode (copy of original item user picked)
	BObject* m_pobjEditOriginal; // Item user picked to Edit in first page, if any
//	BOOL m_bAddFolder; // True if user chose to add a folder in page 60
//	CString m_strFolderName; // Name of folder to add
//	BObject* m_pobjFolderLocation; // Location of folder to add
//	BOOL m_bAutosort; // True if user chose autosort in page 50


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetWizard)
	//}}AFX_VIRTUAL

// Message Map
	protected:
	//{{AFX_MSG(CSheetWizard)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

