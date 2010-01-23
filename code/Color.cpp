


#include "precompiled.h"


#include "Color.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CColor::CColor() :
	m_cr (0) 
{
}

CColor::~CColor() {
}


// Static function
CString CColor::GetRtfFormat(COLORREF cr) {

	CString str;
	str.Format("\\red%d\\green%d\\blue%d;", 
		GetRValue(cr),
		GetGValue(cr),
		GetBValue(cr)
		);
	return str;
}



// Get color in RTF color table format, eg "\\red0\\green0\\blue0;"
LPCTSTR CColor::GetRtfFormat() {

	m_str.Format("\\red%d\\green%d\\blue%d;", 
		GetRValue(m_cr),
		GetGValue(m_cr),
		GetBValue(m_cr)
		);
	return m_str;
}



