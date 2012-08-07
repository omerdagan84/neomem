
// CFontEx



#include "precompiled.h"
#include "NeoMem.h"
#include "FontEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




CFontEx::CFontEx()
{
}

CFontEx::~CFontEx()
{
}


// Convert font height from design units to device units (ie point size).
// Returns size of this font in points. Multiply by 20 to get Twips. 
int CFontEx::GetPointSize()
{
	// DeviceUnits = (DesignUnits/unitsPerEm) * (PointSize/72) * DeviceResolution
	// This formula should not be used to convert device units back to design units. Device units 
	// are always rounded to the nearest pixel. The propagated round-off error can become very 
	// large, especially when an application is working with screen sizes. 

	// lf.lfHeight: Specifies the height, in logical units, of the font's character cell (+) or character (-). 
	// Character Height (Em)
	// This is the height selected on negative lfHeight
	// Em=dpiY*point_size/72;  // where dpiY is dots per inch in Y direction
	// For the MM_TEXT mapping mode, you can use the following formula to specify a height 
	// for a font with a given point size: 
	// lfHeight = -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	// so PointSize = -MulDiv(lfHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY));
	
	LOGFONT lf;
	GetLogFont(&lf);

	// Use screen DC
	HDC hDC = ::GetDC(NULL); // pass null to get screen dc

	// Get number of pixels per logical inch along the display height (96 for VGA)
	int nPixelsPerInch = ::GetDeviceCaps(hDC, LOGPIXELSY); 

	// Get point size
	int nPointSize = -MulDiv(lf.lfHeight, 72, nPixelsPerInch);
//	lf.lfHeight = -(INT) ((cf.yHeight * yPerInch) / 1440
//	cf.yHeight = (LONG) csf.iPointSize * 2; 
//	cf.yHeight: Character height, in twips. A twip is 1/1440 of an inch, or 1/20 of a printer's point. 
//	cf.yHeight = -MulDiv(lf.lfHeight, 72*20, pDC->GetDeviceCaps(LOGPIXELSY));
//	cf.yHeight = nTwips;
	return nPointSize;
}


// Fill CHARFORMAT structure for this font



// Get text representation of font, eg "Arial 12 pt Bold Italic"
CString CFontEx::GetAsText()
{
	int nPointSize = GetPointSize();

	LOGFONT lf;
	GetLogFont(&lf);

	CString str;
	str.Format("%s %d pt %s", (LPCTSTR) lf.lfFaceName, nPointSize, (lf.lfWeight >= 700 ? _T("Bold") : _T("")) );
	return str;
}



// Note: We return a new CString object because the LOGFONT structure is temporary.
//, could have a permanent LOGFONT structure
CString CFontEx::GetFontName()
{
	LOGFONT lf;
	GetLogFont(&lf);
	return lf.lfFaceName;
}



BOOL CFontEx::CreatePointFont2(int nPointSizeTimes10, LPCTSTR lpszFaceName, BOOL bBold, BOOL bItalic, CDC* pDC /* = NULL */)
{
//	ASSERT(pDC == 0); //, for now
	ASSERT(AfxIsValidString(lpszFaceName));

//	LOGFONT logFont;
//	memset(&logFont, 0, sizeof(LOGFONT));
//	logFont.lfCharSet = DEFAULT_CHARSET;
//	logFont.lfHeight = nPointSize;
//	::lstrcpyn(lf.lfFaceName, lpszFaceName, _countof(logFont.lfFaceName));

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfHeight = nPointSizeTimes10;
	lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL; // bug: had these reversed
	lf.lfItalic = bItalic;
	lf.lfPitchAndFamily = FF_SWISS; //. pass as an optional parameter
//	lf.lfHeight = 16; // pixels
//	lf.lfHeight = -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
//	this->GetDC()
//	CClientDC dc(this);
////	lf.lfHeight = -MulDiv(9, GetDeviceCaps(hDC, LOGPIXELSY), 72);
//	lf.lfHeight = -MulDiv(9, GetDeviceCaps(dc.m_hDC, LOGPIXELSY), 72);
	::lstrcpyn(lf.lfFaceName, lpszFaceName, LF_FACESIZE); //, on NT this is implemented as Unicode
//	m_fontMiddleBold.CreateFontIndirect(&lf);  // create the font

//	CFont::CreatePointFont(nPointSize, lpszFaceName, pDC);
//	return CreatePointFontIndirect(&logFont, pDC);
	return CreatePointFontIndirect(&lf, pDC);
}



// Create a font from a CHARFORMAT structure
BOOL CFontEx::CreatePointFont2(CHARFORMAT& cf)
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	// lfHeight = Specifies the height, in logical units, of the font's character cell or character.
	// yHeight = Character height, in twips (1/1440 of an inch or 1/20 of a printer's point). 
	// cf.yHeight = GetPointSize(pDC) * 20; // Convert point size to twips by multiplying by 20 (a twip is 1/20th of a point)
	// nPointSize = cf.yHeight / 20;
	// The lfHeight member of the LOGFONT structure is measured in tenths of a point rather 
	// than logical units. (For instance, set lfHeight to 120 to request a 12-point font.)
	// bug: wasn't multiplying this by 10 and couldn't figure out why measuretextextent was so screwy.
	lf.lfHeight = - cf.yHeight / 20 * 10;
	lf.lfWidth = 0;
	lf.lfEscapement = 0; // angle
	lf.lfOrientation = 0; // angle
	lf.lfWeight = (cf.dwEffects & CFE_BOLD) ? FW_BOLD : FW_NORMAL;
	lf.lfItalic = (BYTE) cf.dwEffects & CFE_ITALIC;
	lf.lfUnderline = FALSE; 
	lf.lfCharSet = DEFAULT_CHARSET;
//	lf.lfCharSet = ANSI_CHARSET;
	::lstrcpyn(lf.lfFaceName, cf.szFaceName, LF_FACESIZE); //, on NT this is implemented as Unicode
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	return CreatePointFontIndirect(&lf);
}



// Fill a CHARFORMAT structure with information about this CFont object. 
// Converts a LOGFONT structure to a CHARFORMAT structure. 
void CFontEx::GetCharFormat(CHARFORMAT &cf)
{
	// Zero memory
	// Bug: didn't do this - might have caused probs in release version
	::ZeroMemory(&cf, sizeof(CHARFORMAT));

	// Get logfont first
	LOGFONT lf;
	GetLogFont(&lf);

	// Now convert to charformat
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_CHARSET | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_OFFSET | 
						CFM_PROTECTED | CFM_SIZE | CFM_STRIKEOUT | CFM_UNDERLINE;
	cf.dwMask = CFM_EFFECTS;
	cf.dwEffects = CFE_AUTOCOLOR | 
							(lf.lfWeight >= 700 ? CFE_BOLD : 0) | 
							(lf.lfItalic ? CFE_ITALIC : 0) |
							(lf.lfUnderline ? CFE_UNDERLINE : 0) |
							(lf.lfStrikeOut ? CFE_STRIKEOUT : 0);
	cf.yHeight = GetPointSize() * 20; // Convert point size to twips by multiplying by 20 (a twip is 1/20th of a point)
	cf.yOffset = 0; // for superscript and subscript!
	cf.crTextColor = 0;
	cf.bCharSet = lf.lfCharSet;
	cf.bPitchAndFamily = lf.lfPitchAndFamily;
	// Check for max length - LF_FACESIZE = 32
	_sntprintf_s(cf.szFaceName, LF_FACESIZE, LF_FACESIZE, lf.lfFaceName);
}



// Fill a CHARFORMAT2 structure with information about this CFont object. 
// Converts a LOGFONT structure to a CHARFORMAT2 structure. 
void CFontEx::GetCharFormat2(CHARFORMAT2 &cf2)
{
	// Zero memory
	// Bug: didn't do this - might have caused probs in release version
	::ZeroMemory(&cf2, sizeof(CHARFORMAT2));

	// Get logfont first
	LOGFONT lf;
	GetLogFont(&lf);

	// Now convert to charformat2
	::ZeroMemory(&cf2, sizeof(CHARFORMAT2));
	cf2.cbSize = sizeof(CHARFORMAT2);
	cf2.dwMask = CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT |
						CFM_SIZE | CFM_OFFSET | CFM_CHARSET | CFM_FACE;
	// CFE_AUTOCOLOR The text color is the return value of GetSysColor (COLOR_WINDOWTEXT). 
	//							If this flag is set, the crTextColor member is ignored. 
	cf2.dwEffects = CFE_AUTOCOLOR | CFE_AUTOBACKCOLOR | 
							(lf.lfWeight >= 700 ? CFE_BOLD : 0) | 
							(lf.lfItalic ? CFE_ITALIC : 0) |
							(lf.lfUnderline ? CFE_UNDERLINE : 0) |
							(lf.lfStrikeOut ? CFE_STRIKEOUT : 0);
//	cf2.wWeight = (WORD) lf.lfWeight;
	cf2.yHeight = GetPointSize() * 20; // Convert point size to twips by multiplying by 20 (a twip is 1/20th of a point)
	cf2.yOffset = 0; // for superscript and subscript!
	cf2.bCharSet = lf.lfCharSet;
	cf2.bPitchAndFamily = lf.lfPitchAndFamily; // CFM_FACE
	// Check for max length - LF_FACESIZE = 32
	_sntprintf_s(cf2.szFaceName, LF_FACESIZE, LF_FACESIZE, lf.lfFaceName);

}



