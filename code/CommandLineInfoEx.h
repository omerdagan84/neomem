


#pragma once

#include "neomem.h"



class CCommandLineInfoEx : public CCommandLineInfo  
{
public:
	CCommandLineInfoEx();
	virtual void ParseParam(LPCTSTR pszParam, BOOL bFlag, BOOL bLast);

public:
	BOOL m_bTest;
	BOOL m_bAdmin;

};


