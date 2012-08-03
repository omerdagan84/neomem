
// CPageObjectGeneral
// General tab in the Object Properties dialog
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "Resource.h"

#include "BObject.h"



class CPageObjectGeneral : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageObjectGeneral)

// Construction
public:
	CPageObjectGeneral();
	~CPageObjectGeneral();

// Dialog Data
private: 
	//{{AFX_DATA(CPageObjectGeneral)
	enum { IDD = IDD_OBJECT_GENERAL };
	CStatic	m_lblObjectID;
	CEdit	m_txtObjectID;
	CEdit	m_txtFlags;
	CEdit	m_txtSize;
	CEdit	m_txtName;
	CEdit	m_txtLocation;
	CEdit	m_txtDescription;
	CEdit	m_txtClass;
	CEdit	m_txtChildren;
	//}}AFX_DATA

public:
	BObject* m_pobj;

private:
	BOOL m_bNameChanged;
	BOOL m_bDescriptionChanged;
	BOOL m_bIDChanged;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageObjectGeneral)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageObjectGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeTxtName();
	afx_msg void OnChangeTxtDescription();
	afx_msg void OnChangeTxtObjectID();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};




