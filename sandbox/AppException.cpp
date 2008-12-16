
// CAppException


#include "precompiled.h"
#include "neomem.h"
#include "AppException.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// "Any CException-derived class that you create should use the 
// IMPLEMENT_DYNAMIC macro, too."
IMPLEMENT_DYNAMIC(CAppException, CException)



CAppException::CAppException(BOOL bAutoDelete)
{
	CException::CException(bAutoDelete);
//	m_pszError = 0;
}

CAppException::~CAppException()
{
}


// not currently using this routine
BOOL CAppException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext /* = NULL */)
{
	// Copy error message to provided buffer
//	_tcsncpy(lpszError, m_pszError, nMaxError);
//	return TRUE;
	return FALSE; // no message text available
}









