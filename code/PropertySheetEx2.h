
// CPropertySheetEx2
// Extension of the CPropertySheetEx class.
//-----------------------------------------------------------------------------------------------------------------


#pragma once


class CPropertySheetEx2 : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetEx2)

public:
	CPropertySheetEx2::CPropertySheetEx2();
	CPropertySheetEx2(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropertySheetEx2(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

protected:
	HACCEL m_hAccel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertySheetEx2)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropertySheetEx2();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertySheetEx2)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


