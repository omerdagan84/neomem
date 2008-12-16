
// CAppException
// General purpose application exception
// Date: January 2001
//------------------------------------------------------------------------------------------------------------------


#pragma once



class CAppException : public CException  
{
	DECLARE_DYNAMIC(CAppException)

public:
	CAppException(BOOL bAutoDelete);
	virtual ~CAppException();
	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL);

public:
//	LPCTSTR m_pszError; // error message
	CString m_strError; // error message
	LPCTSTR m_lpszFilename;
	int m_nLine;
};





