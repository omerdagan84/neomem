
// CDialogEditLink
// This dialog lets the user edit a Link property - they can choose 
// object(s) to link to. 
// Also lets them add and edit objects in place, to a degree. 
// Date: February 2000
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "ViewTabs.h"
#include "ListCtrlEx.h"
#include "Resource.h"

class CNeoDoc;
class BObject;
class BDataLink;


class CDialogEditLink : public CDialog {


// Construction
public:
	CDialogEditLink(CWnd* pParent = NULL);   // standard constructor

// Enums
public:
	enum eButtonFlags {
		flagShowNone = 0, flagShowAdd = 1, flagShowImport = 2, 
		flagShowDefault = 4, flagShowEdit = 8, flagShowDelete = 16, 
		flagShowClose = 32,	flagShowAddChild = 64
	};

// Operations
public:
	void AddObject(BOOL bAddAsChild);
	void AddObjects();
	int DoModalEdit(CString strCaption, CString strInstructions, ULONG lngStartID, ULONG lngExcludeFlags = 0, BOOL bIncludeStart = FALSE);
	int DoModalLink(CString strCaption, CString strInstructions, BOOL bMultiSelectVisible, BOOL bMultiSelectEnabled, 
			BOOL bMultiSelectOn, ULONG lngStartID, BDataLink* pdatLink, ULONG lngExcludeFlags = 0, BOOL bIncludeStart = FALSE);
	int DoModalLinkSimple(CString strCaption, CString strInstructions, ULONG lngStartID, ULONG lngSelectedID = 0, 
			ULONG lngDefaultID = 0, ULONG lngExcludeFlags = 0, BOOL bIncludeStart = FALSE);
	int DoModalSelect(CString strCaption, CString strInstructions, ULONG lngStartID, ULONG lngSelectedID = 0, 
			ULONG lngDefaultID = 0, ULONG lngExcludeFlags = 0, BOOL bIncludeStart = FALSE);
	void ShowFilterCheckbox(BOOL bFilterOn, ULONG lngFilterExcludeFlags);
	void UpdateControls();

// Public Attributes
public: 

	// determines which mode will be shown - IDD_EDIT_LINK, IDD_MOVE_TO, etc
	// UINT m_nMode; 

	// Fill these in before calling DoModal, will contain results on exit
	CString m_strCaption; // [in]
	CString m_strInstructions; // [in]
	ULONG m_lngButtonFlags; // [in] - use eButtonFlags to specify which buttons to show

	ULONG m_lngStartID; // [in]
	ULONG m_lngSelectedID; // [in] [out]
	ULONG m_lngDefaultID; // [in]
	ULONG m_lngExcludeFlags; // [in]
	BOOL m_bIncludeStart; // [in]

	BObject* m_pobjStart; // [out]
	BObject* m_pobjSelected; // [out]
	BDataLink* m_pdatLink; // [in] [out]

	BOOL m_bMultiSelectVisible; // [in]
	BOOL m_bMultiSelectEnabled; // [in]
	BOOL m_bMultiSelectOn; // [in]

	BOOL m_bFilterVisible; // if true will show filter checkbox
	BOOL m_bFilterOn;
	ULONG m_lngFilterExcludeFlags;

	BOOL m_bHierarchy; // if true will show items in hierarchy, otherwise in flat list

	ULONG m_lngDefaultClassID;
	UINT m_nHelpID; // determines which help topic will be brought up for the dialog, since it has so many different modes.

private:
	CNeoDoc* m_pDoc;
	CBitmap m_bmpUp;
	CBitmap m_bmpDown;

// Dialog Data
private: 
	//{{AFX_DATA(CDialogEditLink)
	enum { IDD = IDD_EDIT_LINK };
	CButton	m_chkFilter;
	CButton	m_btnAddChild;
	CButton	m_chkHierarchy;
	CButton	m_btnHelp;
	CButton	m_btnDown;
	CButton	m_btnArrange;
	CButton	m_btnUp;
	CButton	m_chkMultiple;
	CListCtrlEx	m_lvw;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnEdit;
	CButton	m_btnDelete;
	CButton	m_btnUseDefault;
	CButton	m_btnAdd;
	CStatic	m_lblInstructions;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEditLink)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Message Map
protected:
	//{{AFX_MSG(CDialogEditLink)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnUseDefault();
	afx_msg void OnBtnEdit();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnClose();
	afx_msg void OnChkMultiple();
	afx_msg void OnBtnHelp();
	afx_msg void OnChkHierarchy();
	afx_msg void OnClose();
	afx_msg void OnBtnAddChild();
	afx_msg void OnChkFilter();
	virtual void OnOK();
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};







