
// CRegKeyX
// The class encapsulates the Registry.
// Taken from ATL source code (CRegKey)
//-----------------------------------------------------------------------------------------------------------------

// To open a particular registry key, call CRegKeyX::Open. To retrieve or modify a 
// data value, call CRegKeyX::QueryValue or CRegKeyX::SetValue. To close a key, 
// call CRegKeyX::Close.

// When you close a key, its registry data is written (flushed) to the hard disk. 
// This process may take several seconds. If your application must explicitly write registry data 
// to the hard disk, you can call the RegFlushKey Win32 function. However, RegFlushKey uses 
// many system resources and should be called only when absolutely necessary.

// Note: If any of the registry functions fail, the return value is a nonzero error code defined in Winerror.h. 
// You can use the FormatMessage function with the FORMAT_MESSAGE_FROM_SYSTEM flag to 
// get a generic description of the error.




#pragma once


class CRegKeyX
{
// Construction
public:
	CRegKeyX();
	~CRegKeyX();

// Attributes
public:
	HKEY m_hKey;

// Operations
public:
	operator HKEY() const;
	BOOL CopyKey();
	void GetString(LPCTSTR lpszValueName, CString& strValue); //` added this
	LONG SetValue(DWORD dwValue, LPCTSTR lpszValueName);
	LONG QueryValue(DWORD& dwValue, LPCTSTR lpszValueName);
	LONG QueryValue(LPTSTR szValue, LPCTSTR lpszValueName, DWORD* pdwCount);
	LONG SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);
	LONG SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);
	static LONG WINAPI SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);
	LONG Create(HKEY hKeyParent, LPCTSTR lpszKeyName, LPTSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE, REGSAM samDesired = KEY_ALL_ACCESS, LPSECURITY_ATTRIBUTES lpSecAttr = NULL, LPDWORD lpdwDisposition = NULL);
	LONG Open(HKEY hKeyParent, LPCTSTR lpszKeyName, REGSAM samDesired = KEY_ALL_ACCESS);
	LONG Close();
	HKEY Detach();
	void Attach(HKEY hKey);
	LONG DeleteSubKey(LPCTSTR lpszSubKey);
	LONG RecurseDeleteKey(LPCTSTR lpszKey);
	LONG DeleteValue(LPCTSTR lpszValue);
//	LONG Replace(LPCTSTR lpszOld, LPCTSTR lpszNew); //` added this
//	LONG ReplaceRecurse(HKEY hKey, LPCTSTR lpszOld, LPCTSTR lpszNew, int nIndent = 0); //` added this
};

inline CRegKeyX::CRegKeyX() {m_hKey = NULL;}
inline CRegKeyX::~CRegKeyX() {Close();}
inline CRegKeyX::operator HKEY() const {return m_hKey;}

inline HKEY CRegKeyX::Detach()
{
	HKEY hKey = m_hKey;
	m_hKey = NULL;
	return hKey;
}

inline void CRegKeyX::Attach(HKEY hKey)
{
	_ASSERTE(m_hKey == NULL);
	m_hKey = hKey;
}

inline LONG CRegKeyX::DeleteSubKey(LPCTSTR lpszSubKey)
{
	_ASSERTE(m_hKey != NULL);
	return RegDeleteKey(m_hKey, lpszSubKey);
}

inline LONG CRegKeyX::DeleteValue(LPCTSTR lpszValue)
{
	_ASSERTE(m_hKey != NULL);
	return RegDeleteValue(m_hKey, (LPTSTR)lpszValue);
}


