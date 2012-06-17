
// CFileVersion



#include "precompiled.h"
#include "NeoMem.h"
#include "FileVersion.h"

// libraries: version.lib



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//---------------------------------------------------------------------------------------------------

CFileVersion::CFileVersion() :
	m_pVerInfo(0)
{
}

CFileVersion::~CFileVersion()
{
	// Free the version info data
	if (m_pVerInfo)
		delete m_pVerInfo;
}




//---------------------------------------------------------------------------------------------------



// Retrieves the value of the specified version info variable.
//  "CompanyName"
//	"FileDescription"
//	"FileVersion"
//	"InternalName"
//	"LegalCopyright"
//	"OriginalFilename"
//	"ProductName"
//	"ProductVersion"
// Returns TRUE if found successfully.
BOOL CFileVersion::GetVersionValue(LPCTSTR pszVariable, CString& strValue)
{
	// Load version info data if hasn't already been loaded
	if (!m_pVerInfo)
	{
		if (!GetVersionInfo())
			return FALSE;
	}
	ASSERT(m_pVerInfo);

	char szQueryStr[0x100];
	char szQueryStr2[0x100];

	// bug: by getting the user's default language id here,
	// the value wasn't being found! just specify what it was written with
	LANGID nLanguage = 0x0409; // ::GetUserDefaultLangID();
	LCID nLocale = 1200; // ::GetUserDefaultLCID();
	
	// Format the string with the 1200 codepage (Unicode)
	wsprintf(szQueryStr, "\\StringFileInfo\\%04X%04X\\%s", nLanguage, nLocale, (LPCTSTR) pszVariable);

	// We may want to format a string with the "0000" codepage //, what?
	PSTR pszValue = 0;
	UINT uValueSize = 0;
	BOOL bFound = FALSE;

	// Try first with the 1200 codepage
	bFound = VerQueryValue(m_pVerInfo, szQueryStr, (LPVOID*)&pszValue, &uValueSize);
	if (!bFound)
	{
		// not found, so try with other standard codepage
		nLocale = 1252; // Latin 1 US/Western Europe
		wsprintf(szQueryStr2, "\\StringFileInfo\\%04X%04X\\%s", nLanguage, nLocale, (LPCTSTR) pszVariable);
		bFound = VerQueryValue(m_pVerInfo, szQueryStr2, (LPVOID*)&pszValue, &uValueSize);
	}

	// Convert to CString
	if (bFound)
	{
		strValue = pszValue;
	}
	
	return bFound;
}





// Load the version information for this file, if any.
// Returns pointer to version info data, or NULL if failed.
// Also sets m_pVerInfo variable to point to the data.
// Private routine.
BYTE* CFileVersion::GetVersionInfo()
{
	CString strFile = CFile::GetFilePath();
	LPSTR pszFile = strFile.GetBuffer(0);
	DWORD dwDummy;
	BYTE* pVerInfo = 0;

	DWORD dwSize = ::GetFileVersionInfoSize(pszFile, &dwDummy);
	if (dwSize)
	{
		// Create a buffer with dwSize bytes to receive the information.
		pVerInfo = new BYTE[dwSize];
		if (pVerInfo)
		{
			// The GetFileVersionInfo function returns version information about a specified file into a buffer.
			// The file version information is always in Unicode format. 
			if (::GetFileVersionInfo(pszFile, 0, dwSize, pVerInfo))
			{
				// Save pointer to data
				m_pVerInfo = pVerInfo;
			}
		}
	}
	strFile.ReleaseBuffer();
	return pVerInfo;
}



