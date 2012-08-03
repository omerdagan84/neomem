
// CDialogNewFile
// This dialog will let the user create a new file based on a selected template, etc.
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "ListCtrlEx.h"
#include "resource.h"


class CDialogNewFile : public CDialog
{
// Construction
public:
	CDialogNewFile(CWnd* pParent = NULL);   // standard constructor

// Public Attributes
public:
	CImageList m_iml;

// Dialog Data
	//{{AFX_DATA(CDialogNewFile)
	enum { IDD = IDD_NEW_FILE };
	CListCtrlEx	m_lvw;
	CStatic	m_lblInstructions;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogNewFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogNewFile)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

