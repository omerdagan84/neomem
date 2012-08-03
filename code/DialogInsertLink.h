
// CDialogInsertLink



#pragma once


#include "Resource.h"
#include "DialogEx.h"
#include "ListCtrlEx.h"



class CDialogInsertLink : public CDialogEx
{
// Construction
public:
	CDialogInsertLink(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
private:
	//{{AFX_DATA(CDialogInsertLink)
	enum { IDD = IDD_INSERT_LINK };
	CStatic	m_lblLink;
	CEdit	m_txtLink;
	CListCtrlEx	m_lvw;
	CButton	m_btnBrowse;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogInsertLink)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogInsertLink)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


