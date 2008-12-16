

#pragma once


class CEmail : public CObject  
{
// Construction
public:
	CEmail();
	CEmail(CString ToName, CString ToAddress, CString Subject, CString Message);
	virtual ~CEmail();

// Operations
	void AskSend();
	void Send();

// Attributes
	CString m_strToName;
	CString m_strToAddress;
	CString m_strSubject;
	CString m_strMessage;
};

