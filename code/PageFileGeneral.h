
// CPageFileGeneral
// General tab in the File Properties dialog, has information about the current file
// (size, number of objects, etc.)
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "BDoc.h"


class CPageFileGeneral : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageFileGeneral)

// Construction
public:
	CPageFileGeneral();
	~CPageFileGeneral();

private:
	void DisplayInfo();

// Dialog Data
private:
	//{{AFX_DATA(CPageFileGeneral)
	enum { IDD = IDD_FILE_GENERAL };
	CButton	m_btnDataModel;
	CStatic	m_lblFileStructure;
	CEdit	m_txtLocation;
	CButton	m_btnPassword;
	CEdit	m_txtEncryption;
	CEdit	m_txtFileStructure;
	CEdit	m_txtDataModel;
	CEdit	m_txtSize;
	CEdit	m_txtObjects;
	CEdit	m_txtName;
	CEdit	m_txtFileSize;
	CEdit	m_txtDescription;
	//}}AFX_DATA

public:
	BDoc* m_pDoc;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageFileGeneral)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageFileGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnPassword();
	afx_msg void OnBtnDataModel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


