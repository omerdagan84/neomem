
// CPrintInfoMore


#include "precompiled.h"

#include "NeoMem.h"
#include "PrintInfoMore.h"

#include "NeoDoc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CPrintInfoMore::CPrintInfoMore()
{
	m_nHeaderLines = 0;
	m_nFooterLines = 0;
	m_strViewName = _T("Unknown View");
	m_strObjectName = _T("(unknown object)");

	m_pszFontName = 0;	
	m_nBaseFontSize = 0;

	m_nPrintLineHeight = 0;
	m_nPrintCharWidth = 0;
	m_nHeaderLineHeight = 0;

	m_nOffsetToLeft = 0;
	m_nOffsetToTop = 0;
	m_nPageHeightWithinMargins = 0;
	m_nPageWidthWithinMargins = 0;
	m_nPixelsPerInchX = 0;
	m_nPixelsPerInchY = 0;
	m_nPageHeightActual = 0;
	m_nPageWidthActual = 0;
	m_nPageHeightPrintable = 0;
	m_nPageWidthPrintable = 0;
}

CPrintInfoMore::~CPrintInfoMore()
{
}




void CPrintInfoMore::Initialize(CDC *pDC)
{
	int nTwipsPerInch = 1440;

	// Header and footer information
	if (theApp.m_bPrintHeaderFooter)
	{
		m_nHeaderLines = 2;
		m_nFooterLines = 2;
	}
	else
	{
		m_nHeaderLines = 0;
		m_nFooterLines = 0;
	}

	// Get printable page area, normally smaller than 8.5x11" (pixels)
	m_nPageHeightPrintable = pDC->GetDeviceCaps(VERTRES);
	m_nPageWidthPrintable = pDC->GetDeviceCaps(HORZRES);

	// Get offset to first printable area (pixels)
	int nLeft = pDC->GetDeviceCaps(PHYSICALOFFSETX); 
	int nTop = pDC->GetDeviceCaps(PHYSICALOFFSETY);

	// Get pixels per inch for printer DC
	m_nPixelsPerInchX = pDC->GetDeviceCaps(LOGPIXELSX);
	m_nPixelsPerInchY = pDC->GetDeviceCaps(LOGPIXELSY);

	// Get actual page size (eg 8.5x11") (convert twips to pixels)
	m_nPageWidthActual = theApp.m_sizePaper.cx * m_nPixelsPerInchX / nTwipsPerInch; 
	m_nPageHeightActual = theApp.m_sizePaper.cy * m_nPixelsPerInchY / nTwipsPerInch;

	// Get margins (convert twips to pixels)
	m_rMargins.left = theApp.m_rectPageMargins.left * m_nPixelsPerInchX / nTwipsPerInch;
	m_rMargins.right = theApp.m_rectPageMargins.right * m_nPixelsPerInchX / nTwipsPerInch;
	m_rMargins.top = theApp.m_rectPageMargins.top * m_nPixelsPerInchY / nTwipsPerInch;
	m_rMargins.bottom = theApp.m_rectPageMargins.bottom * m_nPixelsPerInchY / nTwipsPerInch;

	// Get offsets to margins from start of printable area (pixels)
	m_nOffsetToLeft = m_rMargins.left - nLeft; 
	m_nOffsetToTop = m_rMargins.top - nTop; 

	// Get page dimensions within margins (pixels)
	m_nPageWidthWithinMargins = m_nPageWidthActual - (m_rMargins.left + m_rMargins.right); // width inside margins
	m_nPageHeightWithinMargins = m_nPageHeightActual - (m_rMargins.top + m_rMargins.bottom); // height inside margins

	// Create page size rectangle (pixels)
	m_rPageSize.SetRect(0, 0, m_nPageWidthActual, m_nPageHeightActual);

	// Create margin rectangle used for drawing margin lines (pixels) 
	m_rMarginLines.left = m_nOffsetToLeft;
	m_rMarginLines.right = m_nOffsetToLeft + m_nPageWidthWithinMargins;
	m_rMarginLines.top = m_nOffsetToTop;
	m_rMarginLines.bottom = m_nOffsetToTop + m_nPageHeightWithinMargins;

	// Create GDI objects
	m_penLine.DeleteObject(); // just in case
	m_penMargins.DeleteObject(); // just in case
	m_penLine.CreatePen(PS_SOLID, 0, 0x00c0c0c0);
	m_penMargins.CreatePen(PS_DOT, 0, 0x00d0d0d0);

	// Base font information
	m_pszFontName = _T("Arial");
	m_nBaseFontSize = 80;
}



// Initialize the printer fonts, scaling by the optional factor
void CPrintInfoMore::InitFonts(CDC* pDC, float fFactor /* = 1.0 */)
{
	// Create fonts
	UINT nFontSize = (UINT) (m_nBaseFontSize * fFactor);
	m_fontPrint.DeleteObject();
	m_fontPrintBold.DeleteObject();
	m_fontPrint.CreatePointFont2(nFontSize, m_pszFontName, FALSE, FALSE, pDC);
	m_fontPrintBold.CreatePointFont2(nFontSize, m_pszFontName, TRUE, FALSE, pDC);

	// Get font information
	TEXTMETRIC tm;
	CFont* poldfont = pDC->SelectObject(&m_fontPrint);
	pDC->GetTextMetrics(&tm);
	m_nPrintLineHeight = tm.tmHeight + tm.tmExternalLeading;
	m_nPrintCharWidth = tm.tmAveCharWidth;
	pDC->SelectObject(poldfont);

	// Header/footer font is base size
	m_fontHeader.DeleteObject();
	m_fontHeader.CreatePointFont2(m_nBaseFontSize, m_pszFontName, FALSE, FALSE, pDC);
	poldfont = pDC->SelectObject(&m_fontHeader);
	pDC->GetTextMetrics(&tm);
	m_nHeaderLineHeight = tm.tmHeight + tm.tmExternalLeading; // get header/footer line height
	pDC->SelectObject(poldfont);

}



// Print header
// eg "Text View: @todo								April 7, 2001"
void CPrintInfoMore::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
	if (m_nHeaderLines > 0)
	{
		CString str;
		CFont* poldfont = pDC->SelectObject(&m_fontHeader);
		
		// View and object name eg "Text View: @todo"
		str.Format("%s: %s", m_strViewName, m_strObjectName);
		UINT x = m_nOffsetToLeft;
		UINT y = m_nOffsetToTop;
		pDC->TextOut(x, y, str);

		// Page Number
		str.Format("Page %d", pInfo->m_nCurPage);
		pDC->SetTextAlign(TA_RIGHT);
		x = m_nOffsetToLeft + m_nPageWidthWithinMargins;
		pDC->ExtTextOut(x, y, 0, NULL, str, NULL);
		pDC->SetTextAlign(TA_LEFT);

		pDC->SelectObject(poldfont);
	}

	// See if we are print previewing for a black and white printer
//	BOOL bMono = (pDC->GetDeviceCaps(NUMCOLORS) == 2) && (pDC->m_hDC != pDC->m_hAttribDC);	
	// Convert rgb colors into shades of gray with this formula:
	// rgb = (red * 0.30) + (green * 0.59) + (blue * 0.11)
}


// Print footer
// "E:\Projects\NeoMem\Stuff.neo						Page 1"
void CPrintInfoMore::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
{
	if (m_nFooterLines > 0)
	{
		CString str;
		CFont* poldfont = pDC->SelectObject(&m_fontHeader);

		UINT x = m_nOffsetToLeft;
		UINT y = m_nOffsetToTop + m_nPageHeightWithinMargins - m_nPrintLineHeight;

		// Filename
		CNeoDoc* pDoc = CNeoDoc::GetDoc();
		str = pDoc->GetPathName();
		if (str.GetLength() == 0)
			str = pDoc->GetTitle();
		pDC->TextOut(x, y, str);

//		str.Format("%d, %d, %s", x, y, (LPCTSTR) str); // y = 1845 in release, 1815(?) in debug
//		AfxMessageBox(str); //
//		str.Format("%d, %d, %d", m_nOffsetToTop, m_nPageHeightWithinMargins, m_nPrintLineHeight);
//		AfxMessageBox(str); //
//		str = "uh";
//		pDC->SetTextAlign(TA_CENTER);
//		x = m_nOffsetToLeft + (m_nPageWidthWithinMargins / 2);
//		pDC->ExtTextOut(x, y, 0, NULL, str, NULL);
//		pDC->SetTextAlign(TA_LEFT);

		//. footer sometimes gets cut off at top - fix this
//		y -= m_nPrintLineHeight / 2; // test

		// Date
		COleDateTime odt;
		odt = COleDateTime::GetCurrentTime();
		str = odt.Format(VAR_DATEVALUEONLY);
		pDC->SetTextAlign(TA_RIGHT);
		x = m_nOffsetToLeft + m_nPageWidthWithinMargins;
		pDC->ExtTextOut(x, y, 0, NULL, str, NULL);
		pDC->SetTextAlign(TA_LEFT);

		pDC->SelectObject(poldfont);
	}
}



void CPrintInfoMore::DrawMargins(CDC *pDC, CPrintInfo *pInfo)
{
	// Draw margins if in preview mode
	if ((pDC->m_hAttribDC != NULL) && (pInfo != NULL) && (pInfo->m_bPreview))
	{
		// Save current settings
		CRect rSave = pInfo->m_rectDraw;

		// Set page rectangle
		pInfo->m_rectDraw = m_rPageSize;

		// Draw margins (pixels)
		CPen* poldpen = pDC->SelectObject(&m_penMargins);
		pDC->MoveTo(0, m_rMarginLines.top); pDC->LineTo(12000, m_rMarginLines.top);
		pDC->MoveTo(m_rMarginLines.left, 0); pDC->LineTo(m_rMarginLines.left, 12000);
		pDC->MoveTo(0, m_rMarginLines.bottom); pDC->LineTo(12000, m_rMarginLines.bottom);
		pDC->MoveTo(m_rMarginLines.right, 0); pDC->LineTo(m_rMarginLines.right, 12000);
		pDC->SelectObject(poldpen);

		// Restore settings
		pInfo->m_rectDraw = rSave;
	}
}




void CPrintInfoMore::Terminate()
{
	// Delete gdi objects
	m_fontPrint.DeleteObject();
	m_fontPrintBold.DeleteObject();
	m_fontHeader.DeleteObject();
	m_penLine.DeleteObject();
	m_penMargins.DeleteObject();	
}




