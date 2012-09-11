
// CPageClassViews
// Views page in the Class Wizard
// (not used)
//-----------------------------------------------------------------------------------------------------------------


#pragma once

#include "ListCtrlEx.h"
#include "SheetWizard.h"

#include "BDoc.h"
#include "BObject.h"


class CPageClassViews : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageClassViews)

// Construction
public:
	CPageClassViews();
	~CPageClassViews();

// Dialog Data
private:
	//{{AFX_DATA(CPageClassViews)
	enum { IDD = IDD_CLASS_VIEWS };
	CListCtrlEx	m_lvw;
	CStatic	m_lblInstructions;
	CStatic	m_lblTitle;
	//}}AFX_DATA

private:
	BDoc* m_pDoc;
	CSheetWizard* m_psh;
	BObject* m_pobj;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageClassViews)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void LoadData();
	// Generated message map functions
	//{{AFX_MSG(CPageClassViews)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

