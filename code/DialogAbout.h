
// CDialogAbout 
// Generic about dialog box. Should be able to reuse most of this code for different 
// programs - it pulls the text from the Version Info resource.
// Date: February 2000
// obsolete
//-----------------------------------------------------------------------------------------------------------------


#pragma once

 
#include "Resource.h"
//#include "StaticLink.h"
#include "StaticEx.h"

class CDialogAbout : public CDialog
{
// Construction
public:
	CDialogAbout();
	~CDialogAbout();

// Implementation
protected:
	void GetVersionInfo();
//	void GetWindowsInfo();
//	void GetMemoryInfo();


// Attributes
private:
	COLORREF m_colorBackground;
	COLORREF m_colorForeground;
//	CStaticLink m_linkWebsite;
//	CStaticLink m_linkEmail;
	CStaticEx m_linkWebsite;
	CStaticEx m_linkEmail;
	CBrush m_brush; // background brush


	// Dialog Data
	//{{AFX_DATA(CDialogAbout)
	enum { IDD = IDD_ABOUT };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


	//{{AFX_MSG(CDialogAbout)
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

