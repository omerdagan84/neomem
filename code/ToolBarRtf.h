
// CToolBarRtf
// This encapsulates the toolbar for the Text View.
//-----------------------------------------------------------------------------------------------------------------



#pragma once

#include "ToolBarCtrlEx.h"
#include "ColorButton.h"



class CToolBarRtf : public CToolBarCtrlEx
{
	DECLARE_DYNAMIC(CToolBarRtf)

// Construction
public:
	CToolBarRtf();
	virtual ~CToolBarRtf();

// Operations
public:

// Public Attributes
public:
	CComboBox m_cboFontName;
	CComboBox m_cboFontSize;
	CColorButton m_ctlForecolor;
	CColorButton m_ctlBackcolor;

// Attributes
protected:
//	CFont m_font;

// Implementation
protected:
	static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarRtf)
	protected:
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CToolBarRtf)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnSelectFont();
	afx_msg void OnSelectSize();
	afx_msg LRESULT OnSelEndOK(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorButtonClick(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};




