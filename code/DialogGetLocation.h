
// CDialogGetLocation



#pragma once

#include "TreeCtrlEx.h"

class CNeoDoc;
class BObject;
#include "ConstantsDatabase.h"


class CDialogGetLocation : public CDialog
{
// Construction
public:
	CDialogGetLocation(CWnd* pParent = NULL);   // standard constructor

// Public Attributes
public:
	CString m_strCaption;
	CString m_strInstructions;
	BObject* m_pobj;
	BOOL m_bAllowSelectHome;
	UINT m_nHelpID;

// Dialog Data
private:
	//{{AFX_DATA(CDialogGetLocation)
	enum { IDD = IDD_GET_LOCATION };
	CButton	m_btnOK;
	CTreeCtrlEx	m_tvw;
	CStatic	m_lblInstructions;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogGetLocation)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CNeoDoc* m_pDoc;

protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogGetLocation)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTvw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


