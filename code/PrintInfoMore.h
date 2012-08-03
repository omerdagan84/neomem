
// CPrintInfoMore
// This object is used in printing and print previewing - it stores all sorts of information
// about page dimensions, margins, gdi objects, etc.
// Date: April 2001
//-----------------------------------------------------------------------------------------------------------------


#pragma once

class CPrintInfoMore  
{
// Construction
public:
	CPrintInfoMore();
	virtual ~CPrintInfoMore();

// Methods
public:
	void Initialize(CDC* pDC);
	void InitFonts(CDC* pDC, float fFactor = 1.0);
	void PrintHeader(CDC* pDC, CPrintInfo* pInfo);
	void PrintFooter(CDC* pDC, CPrintInfo* pInfo);
	void DrawMargins(CDC* pDC, CPrintInfo *pInfo);
	void Terminate();

// Public Attributes
public:
	// GDI objects
	CFontEx m_fontPrint;
	CFontEx m_fontPrintBold;
	CFontEx m_fontHeader;
	CPen m_penLine;
	CPen m_penMargins;

	// Base font info
	LPCTSTR m_pszFontName;
	UINT m_nBaseFontSize;

	// Header and footer
	UINT m_nHeaderLines;
	UINT m_nFooterLines;
	CString m_strViewName; // eg "Text View"
	CString m_strObjectName; // eg "Home"
	UINT m_nHeaderLineHeight; // height of header/footer lines

	// All units are pixels unless otherwise noted
	UINT m_nOffsetToLeft; // offset to left margin from leftmost printable area
	UINT m_nOffsetToTop; // offset to top margin from topmost printable area
	UINT m_nPageWidthWithinMargins; // width of page within margins
	UINT m_nPageHeightWithinMargins; // height of page within margins
	UINT m_nPixelsPerInchX;
	UINT m_nPixelsPerInchY;
	UINT m_nPageHeightActual;
	UINT m_nPageWidthActual;
	UINT m_nPageHeightPrintable;
	UINT m_nPageWidthPrintable;
	UINT m_nPrintLineHeight; // height of printed lines
	UINT m_nPrintCharWidth; // average width of printed characters
	CRect m_rMargins;
	CRect m_rPageSize;
	CRect m_rMarginLines;
};

