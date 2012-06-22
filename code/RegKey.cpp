
// CRegKeyX
// Taken from ATL source code, downloaded from CodeGuru
//. not needed with vs2010


#include "precompiled.h"
#include "RegKey.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// Opens the specified key and sets m_hKey to the handle of this key. 
// If the lpszKeyName parameter is NULL or points to an empty string, Open opens a new 
// handle of the key identified by hKeyParent, but does not close any previously opened handle.
// Unlike CRegKeyX::Create, Open will not create the specified key if it does not exist.
// Possible values for hKeyParent:
//		HKEY_CLASSES_ROOT
//		HKEY_CURRENT_CONFIG
//		HKEY_CURRENT_USER
//		HKEY_LOCAL_MACHINE
//		HKEY_USERS
LONG CRegKeyX::Open(HKEY hKeyParent, LPCTSTR lpszKeyName, REGSAM samDesired /* = KEY_ALL_ACCESS */)
{
	_ASSERTE(hKeyParent != NULL);
	HKEY hKey = NULL;
	LONG lRes = ::RegOpenKeyEx(hKeyParent, lpszKeyName, 0, samDesired, &hKey);
	if (lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		_ASSERTE(lRes == ERROR_SUCCESS);
		m_hKey = hKey;
	}
	return lRes;
}


// Close the currently open key
LONG CRegKeyX::Close()
{
	LONG lRes = ERROR_SUCCESS;
	if (m_hKey != NULL)
	{
		lRes = ::RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
	return lRes;
}



// Create a registry key - if it already exists, the key is opened.
LONG CRegKeyX::Create(HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPTSTR lpszClass /* = REG_NONE */, 
		DWORD dwOptions /* = REG_OPTION_NON_VOLATILE */,
		REGSAM samDesired /* = KEY_ALL_ACCESS */, 
		LPSECURITY_ATTRIBUTES lpSecAttr /* = NULL */,
		LPDWORD lpdwDisposition /* = NULL */)
{
	_ASSERTE(hKeyParent != NULL);
	DWORD dw;
	HKEY hKey = NULL;
	LONG lRes = ::RegCreateKeyEx(hKeyParent, lpszKeyName, 0,
		lpszClass, dwOptions, samDesired, lpSecAttr, &hKey, &dw);
	if (lpdwDisposition != NULL)
		*lpdwDisposition = dw;
	if (lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		m_hKey = hKey;
	}
	return lRes;
}





// Use lpszValueName = NULL to get the default value.
LONG CRegKeyX::QueryValue(DWORD& dwValue, LPCTSTR lpszValueName)
{
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	LONG lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType,
		(LPBYTE)&dwValue, &dwCount);
	_ASSERTE((lRes!=ERROR_SUCCESS) || (dwType == REG_DWORD));
	_ASSERTE((lRes!=ERROR_SUCCESS) || (dwCount == sizeof(DWORD)));
	return lRes;
}



// Retrieves the string data for the specified value field of m_hKey. 
// lpszValueName specifies the value field to be queried.
// szValue is the value field's string data (writes string to this buffer).
// Use lpszValueName = NULL to get the default value.
LONG CRegKeyX::QueryValue(LPTSTR szValue, LPCTSTR lpszValueName, DWORD* pdwCount)
{
	_ASSERTE(pdwCount != NULL);
	DWORD dwType = NULL;
	LONG lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType, (LPBYTE)szValue, pdwCount);
	_ASSERTE((lRes!=ERROR_SUCCESS) || (dwType == REG_SZ) ||
			 (dwType == REG_MULTI_SZ) || (dwType == REG_EXPAND_SZ));
	return lRes;
}



// Set a string value under the specified key, which is created if necessary.
// Use lpszValueName = NULL to set the default value.
LONG WINAPI CRegKeyX::SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	_ASSERTE(lpszValue != NULL);
	CRegKeyX key;
	LONG lRes = key.Create(hKeyParent, lpszKeyName);
	if (lRes == ERROR_SUCCESS)
		lRes = key.SetValue(lpszValue, lpszValueName);
	return lRes;
}



// Set a string value under the specified key, under the current key. 
LONG CRegKeyX::SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	_ASSERTE(lpszValue != NULL);
	CRegKeyX key;
	LONG lRes = key.Create(m_hKey, lpszKeyName);
	if (lRes == ERROR_SUCCESS)
		lRes = key.SetValue(lpszValue, lpszValueName);
	return lRes;
}



// Set a DWORD value under the currently open key.
// Use lpszValueName = NULL to set the default value.
LONG CRegKeyX::SetValue(DWORD dwValue, LPCTSTR lpszValueName)
{
	_ASSERTE(m_hKey != NULL);
	return RegSetValueEx(m_hKey, lpszValueName, NULL, REG_DWORD,
		(BYTE * const)&dwValue, sizeof(DWORD));
}



// Set a string value under the currently open key.
// Use lpszValueName = NULL to set the default value for the key.
HRESULT CRegKeyX::SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	_ASSERTE(lpszValue != NULL);
	_ASSERTE(m_hKey != NULL);
	// Windows 95: For string-based data types, such as REG_SZ, RegSetValueEx appends a 
	//		NULL to the end of the value string.  The size includes the size of the NULL added to the 
	//		end of the string passed in the lpData parameter. 
	// Windows NT: For string-based data types, RegSetValueEx does not append a NULL to the 
	//		end of the value string. The size is the exact size of the string passed in the lpData parameter.
	return ::RegSetValueEx(m_hKey, lpszValueName, NULL, REG_SZ,
		(BYTE* const) lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
}



// RecurseDeleteKey is necessary because on NT RegDeleteKey doesn't work if the
// specified key has subkeys.
LONG CRegKeyX::RecurseDeleteKey(LPCTSTR lpszKey)
{
	CRegKeyX key;
	LONG lRes = key.Open(m_hKey, lpszKey);
	if (lRes != ERROR_SUCCESS)
		return lRes;
	FILETIME time;
	TCHAR szBuffer[256];
	DWORD dwSize = 256;
	while (RegEnumKeyEx(key.m_hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL, &time)==ERROR_SUCCESS)
	{
		lRes = key.RecurseDeleteKey(szBuffer);
		if (lRes != ERROR_SUCCESS)
			return lRes;
		dwSize = 256;
	}
	key.Close();
	return DeleteSubKey(lpszKey);
}


//` added this
void CRegKeyX::GetString(LPCTSTR lpszValueName, CString& strValue)
{
	DWORD dwCount = 255;
	LPTSTR szValue = strValue.GetBuffer(dwCount);
	QueryValue(szValue, lpszValueName, &dwCount);
	strValue.ReleaseBuffer();
}



/*
// Search through the entire registry, replacing the old text with the new text
// Usage:
//		CRegKeyX r;
//		r.Replace("D:\\Development\\Micro", "C:\\Program Files\\Micro");
LONG CRegKeyX::Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
{
	// The following predefined keys are always open: HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, 
	// HKEY_USERS, and HKEY_CURRENT_USER. 
	// An application can use the RegCloseKey function to close a key and write the data it contains 
	// into the registry. 

	CRegKeyX r;
	HKEY hKeys[] = {HKEY_LOCAL_MACHINE, HKEY_USERS}; // these are the only two - all others are just subkeys
	UINT nhKeys = sizeof(hKeys) / sizeof(HKEY);

	// Loop through all hkeys
	for (UINT nhKey = 0; nhKey < nhKeys; nhKey++)
	{
		HKEY hKey = hKeys[nhKey];
		LONG lRes = r.Open(hKey, NULL);
		if (lRes != ERROR_SUCCESS)
			return lRes;

		ReplaceRecurse(r.m_hKey, lpszOld, lpszNew);

		// Close this hkey
		r.Close();
	}
	
	return ERROR_SUCCESS;
}





// Search through all values for the specified key, then loop through all subkeys and recurse.
// hKey must be an open key.
LONG CRegKeyX::ReplaceRecurse(HKEY hKey, LPCTSTR lpszOld, LPCTSTR lpszNew, int nIndent)
{
	DWORD nSubkeys;
	DWORD nValues;
	DWORD dwMaxSubkeyName;
	DWORD dwMaxValueName;
	DWORD dwMaxValueData;
	::RegQueryInfoKey(hKey, NULL, NULL, NULL, &nSubkeys, &dwMaxSubkeyName,
		NULL, &nValues, &dwMaxValueName, &dwMaxValueData, NULL, NULL);

	// Search through all values for this key
	DWORD dwValueType; // type of field value (string, number, etc)
	DWORD dwNameSize = 255 * sizeof(TCHAR);
	DWORD dwValueSize = 4096 * sizeof(TCHAR);
	TCHAR szValueName[255]; 
	TCHAR szValue[4096]; 
//	DWORD nValue = 0;
	LONG lRes;

	// The RegEnumValue function enumerates the values for the specified open registry key. 
	// The function copies one indexed value name and data block for the key each time it is called. 
//	while ((lRes = ::RegEnumValue(hKey, nValue, szValueName, &dwNameSize, NULL, &dwValueType, 
//				(LPBYTE) szValue, &dwValueSize)) == ERROR_SUCCESS)
	for (DWORD nValue = 0; nValue < nValues; nValue++)
	{
		lRes = ::RegEnumValue(hKey, nValue, szValueName, &dwNameSize, NULL, &dwValueType, 
				(LPBYTE) szValue, &dwValueSize);
//		lRes = ::RegEnumValue(hKey, nValue, szValueName, &dwNameSize, NULL, &dwValueType, 
//			NULL, NULL);
		if (lRes == ERROR_SUCCESS)
		{
			TRACE("  Value \"%s\"\n", szValueName);
			// Only interested in string values
			if (dwValueType == REG_SZ)
			{
				// Only interested in non-null strings
//				if (*szValue) 
				if (0)
				{
					CString strValue = szValue;
					if (strValue.Replace(lpszOld, lpszNew))
					{
						LPTSTR szNewValue = strValue.GetBuffer(0);
						TRACE("%s: %s  ---> %s\n", szValueName, szValue, szNewValue);
	//					lRes = key.SetValue(szNewValue, szValueName);
						lRes = ::RegSetValueEx(hKey, szValueName, NULL, REG_SZ,
							(BYTE* const) szNewValue, (lstrlen(szNewValue)+1)*sizeof(TCHAR));
						strValue.ReleaseBuffer();
						if (lRes != ERROR_SUCCESS)
							return lRes;
					}
				}
			}
		}
		// Get next value
//		nValue++;
	}

	
	// Now loop through all subkeys and recurse.
	// The RegEnumKeyEx function enumerates subkeys of the specified open registry key. 
	// The function retrieves information about one subkey each time it is called. 
	// The key identified by hKey must have been opened with KEY_ENUMERATE_SUB_KEYS access 
	// (KEY_READ includes KEY_ENUMERATE_SUB_KEYS). Use the RegCreateKeyEx or RegOpenKeyEx 
	// function to open the key. 
	DWORD nKey = 0;
	FILETIME time;
	TCHAR szKeyName[255]; 
	while ((lRes = ::RegEnumKeyEx(hKey, nKey, szKeyName, &dwNameSize, NULL, NULL, NULL, &time))==ERROR_SUCCESS)
	{
		CRegKeyX r;
		CString strSpaces = "                                                                                                     ";
		TRACE("%s%s\n", (LPCTSTR) strSpaces.Left(nIndent*4), szKeyName);
		lRes = r.Open(hKey, szKeyName); // open a subkey
		if (lRes != ERROR_SUCCESS)
			return lRes;
		ReplaceRecurse(r.m_hKey, lpszOld, lpszNew, nIndent+1);
		lRes = r.Close(); // close the subkey
		if (lRes != ERROR_SUCCESS)
			return lRes;

		// Get next subkey
		nKey++;
	}

	return ERROR_SUCCESS;
}
*/





// Copy a registry key to a new location
BOOL CRegKeyX::CopyKey()
{

	return FALSE;
}
