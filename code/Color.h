

#pragma once


class CColor  
{

public:
	CColor();
	virtual ~CColor();
public:
	static CString GetRtfFormat(COLORREF cr);
	LPCTSTR GetRtfFormat();
protected:
	COLORREF m_cr;
	CString m_str;
};

