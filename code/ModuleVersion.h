
// CModuleVersion version info about a module.
// To use:
// CModuleVersion ver
// if (ver.GetFileVersionInfo("_T("mymodule))) {
//    // info is in ver, you can call GetValue to get variable info like
//    CString s = ver.GetValue(_T("CompanyName"));
// }
//
// You can also call the static fn DllGetVersion to get DLLVERSIONINFO.
//

#pragma once



class CModuleVersion : public VS_FIXEDFILEINFO   
{
public:
	CModuleVersion();
	virtual ~CModuleVersion();

	BOOL GetFileVersionInfo(LPCTSTR modulename);
	CString GetValue(LPCTSTR lpKeyName);
	static BOOL DllGetVersion(LPCTSTR modulename, DLLVERSIONINFO& dvi);

protected:
	BYTE* m_pVersionInfo;   // all version info

	struct TRANSLATION 
	{
		WORD langID;         // language ID
		WORD charset;        // character set (code page)
	} m_translation;

};



