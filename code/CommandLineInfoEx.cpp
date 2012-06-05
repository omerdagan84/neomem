
#include "precompiled.h"
#include "CommandLineInfoEx.h"


CCommandLineInfoEx::CCommandLineInfoEx()
{
	// set defaults
	m_bTest=FALSE;
	m_bAdmin=FALSE;
	// call base class
	CCommandLineInfo::CCommandLineInfo();
}


void CCommandLineInfoEx::ParseParam (LPCTSTR pszParam, BOOL bFlag, BOOL bLast)
{
	if (bFlag && stricmp(pszParam, "Test")==0)
		m_bTest = TRUE;
	else if (bFlag && stricmp(pszParam, "Admin")==0)
		m_bAdmin = TRUE;
	else 
		// let base class try
		CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
}

