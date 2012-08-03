
// CPageOptionsDisplay
// Display tab in the Options dialog.
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "FontEx.h"


class CPageOptionsDisplay : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageOptionsDisplay)

// Construction
public:
	CPageOptionsDisplay();
	~CPageOptionsDisplay();

// Public Attributes
public:
	LOGFONT m_lf;
	CFontEx m_font;
	BOOL m_bFontChanged;
//	BOOL	m_bShowTips;
	int		m_nNameFormat;
	int		m_nDateFormat;

// Public Methods
public:
	void UpdateSampleDate();
	void UpdateFontName();

// Dialog Data
private:
	//{{AFX_DATA(CPageOptionsDisplay)
	enum { IDD = IDD_OPTIONS_DISPLAY };
	CStatic	m_lblFont;
	CStatic	m_lblNameFormat;
	CStatic	m_lblDateFormat;
	CStatic	m_lblDate;
	CEdit	m_txtFont;
	CEdit	m_txtDate;
	CButton	m_btnFont;
	CButton	m_btnDateFormat;
	CComboBox	m_cboNameFormat;
	CComboBox	m_cboDateFormat;
	//}}AFX_DATA


// Implementation

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageOptionsDisplay)
	public:
	virtual BOOL OnApply();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CPageOptionsDisplay)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnFont();
	afx_msg void OnBtnDateFormat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


