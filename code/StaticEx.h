
// CStaticEx
// This class extends the CStatic class, letting you make labels into buttons, change 
// background color, foreground color, font, etc.
//-----------------------------------------------------------------------------------------------------------------




#pragma once

#include "FontEx.h"



class CStaticEx : public CStatic
{
// Construction
public:
	CStaticEx();
	~CStaticEx();

// Operations
public:
	BOOL Create(
					LPCTSTR szText, 
					CRect &r, 
					CWnd* pParentWnd, 
					UINT nCommandID = 0,
					BOOL bAutosize = TRUE, 
					UINT nFormat = DT_LEFT | DT_BOTTOM,
					UINT nFlags = 0);
//	void SetColors(BOOL bOpaque = TRUE, COLORREF clrText = -1, COLORREF clrBackground = -1, 
//							COLORREF clrHighlight = -1, COLORREF clrBackground2 = -1);
	void SetColors(BOOL bOpaque = TRUE);
	void SetFontEx(LPCTSTR szFontName = "Arial", float sngFontSize = 8.0, BOOL bFontBold = FALSE, 
						BOOL bFontItalic = FALSE);
	int SetWidthGetHeight(int nWidth);

// Attributes
public:
	BOOL m_bAutosize;
	BOOL m_bHighlight;

	int m_nCommandID;
	UINT m_nFormat;
	CString m_strLink;
	CString m_strEmail;
	int m_nSound;
	
	BOOL m_bOpaque;
	COLORREF m_clrText;
	COLORREF m_clrHighlight;
	COLORREF m_clrBackground;
	COLORREF m_clrBackground2;

	HCURSOR m_hCursor;

	CRect m_rTextSize;
	CRect m_rMargins;
	CRect m_rControlSize; // textsize + margins
	CRect m_rPos;
	CRect m_rDraw; // control size + margin top left

	CFontEx m_font;

	int m_nBkMode;
	UINT m_nTimer;

	enum eFlags {flagNone = 0, flagUnderline = 1};
	UINT m_nFlags;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticEx)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticEx)
	afx_msg void OnPaint();
	afx_msg void OnClicked();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


