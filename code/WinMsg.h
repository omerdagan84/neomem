

// this class wraps a Windows MSG structure


#pragma once


class CWinMsg : public CObject  
{
public:
	CWinMsg();
	virtual ~CWinMsg();
	void SetMsg(const MSG* pMsg);
	CString GetString();

private:
	const MSG* m_pMsg;
	CString m_str;
};

