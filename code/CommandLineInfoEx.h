// Extend MFC class to handle /test and /admin

#pragma once

#include "neomem.h"


class CCommandLineInfoEx : public CCommandLineInfo  
{
public:
	CCommandLineInfoEx();
	virtual void ParseParam(LPCTSTR pszParam, BOOL bFlag, BOOL bLast);

	BOOL IsTest() { return m_bTest; };
	BOOL IsAdmin() { return m_bAdmin; };

private:
	BOOL m_bTest;
	BOOL m_bAdmin;

};


