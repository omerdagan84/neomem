
// MirrorFileEx.h: interface for the CMirrorFileEx class.
// Copy of MFC's CMirrorFile class
// Needed to modify to use CCryptoFile class instead of CFile class





#pragma once


#include "CryptoFile.h"


class CMirrorFileEx : public CCryptoFile
{
// Operations
public:
	virtual void Abort();
	virtual void Close();
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL);
	static CString GetTempName(LPCTSTR pstrOriginalFile, BOOL bCreate);

// Attributes
protected:
	CString m_strMirrorName;
};


