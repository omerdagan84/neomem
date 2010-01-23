
// CColor
// Handles converting colors from COLORREF to RTF format.
//--------------------------------------------------------------------

// not used

#pragma once




class CColor {

public:
	CColor();
	virtual ~CColor();

	static CString GetRtfFormat(COLORREF cr);
	
	LPCTSTR GetRtfFormat();

protected:
	COLORREF m_cr;
	CString m_str;
};


