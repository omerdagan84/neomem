
// CFontEx
// Extension of the CFont class.
//-----------------------------------------------------------------------------------------------------------------


#pragma once



class CFontEx : public CFont  
{
// Construction
public:
	CFontEx();
	virtual ~CFontEx();

// Operations
public:
	void GetCharFormat2(CHARFORMAT2 &cf2);
	BOOL CreatePointFont2(CHARFORMAT& cf);
	BOOL CreatePointFont2(int nPointSizeTimes10, LPCTSTR lpszFaceName, BOOL bBold, BOOL bItalic, CDC* pDC = NULL);
	CString GetAsText();
	int GetPointSize();
	void GetCharFormat(CHARFORMAT& cf);
	CString GetFontName();
};


