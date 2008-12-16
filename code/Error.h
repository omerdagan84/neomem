



#pragma once


#include "winmsg.h"


//. derive from CException? any benefit? i guess just being able to throw it like
// other exceptions. 
//class CError : public CObject  
class CError : public CException  
{
	DECLARE_DYNAMIC(CError)

// Construction
public:
	CError(BOOL bAutoDelete);
//	CError(CString strError, BOOL bSevere=FALSE); //, CString strFilename, int nLine);
	virtual ~CError();

// Operations
	void SetName(CString strName);
	void SetSeverity(BOOL bSevere);
	void SetFileLocation(CString strFilename, int nLine, CString strExpression);
	void SetMsg(const MSG* pMsg);
	void SetDescription(CString str);
	CString GetDialogText();
	CString GetReportText();
	CString GetReportSubject();
//	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL);

// Attributes
public:
	BOOL m_bSevere;
	CString m_strFilesSaved; // info about files being saved to backup copies
	CString m_strError;
	CString m_strDescription; // what you were doing
	CString m_strFilename;
	CString m_strFiletitle;
	CString m_strExpression; // the actual expression that failed in the assertion
	int m_nLine;
	CWinMsg m_winmsg;
};

