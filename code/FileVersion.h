
// CFileVersion
// This class derives from CFile, and provides access to version information data.
// Date: March 2000
//-----------------------------------------------------------------------------------------------------------------



#pragma once




class CFileVersion : public CFile
{
// Construction
public:
	CFileVersion();
	~CFileVersion();

// Operations
//	BOOL GetVersionValue(CString& strVariable, CString& strValue); 
	BOOL GetVersionValue(LPCTSTR pszVariable, CString& strValue); 

// Attributes
private:	
	BYTE* GetVersionInfo();
	BYTE* m_pVerInfo;
};





