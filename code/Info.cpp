
// CInfo


#include "precompiled.h"
#include "NeoMem.h"
#include "Info.h"

#include "FileVersion.h"
#include "RegKey.h"
#include "ModuleVersion.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//, tell linker to link with version.lib
//#pragma comment(linker, "/defaultlib:version.lib")



#define PACKVERSION(major,minor) MAKELONG(minor,major)


// GetDiskFreeSpaceEx function pointer
typedef BOOL (WINAPI *P_GDFSE)(LPCTSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);


// Helper for processor speed calcs.
// This causes a compiler warning as there is no return statement, however the _emits return a __int64 value.
// Because the inline version of power2 doesn’t execute a C return statement, it causes a 
// harmless warning if you compile at warning level 2 or higher. The function does return a 
// value, but the compiler cannot tell that in the absence of a return statement. You can 
// use #pragma warning to disable the generation of this warning.
#pragma warning (disable: 4035)
inline unsigned __int64 theCycleCount(void)
{
	_asm    _emit 0x0F
	_asm    _emit 0x31
}
#pragma warning (default: 4035)


// CTimer
// Helper class for calculating processor speed
//-----------------------------------------------------------------------------------------------------------------

class CTimer
{
public:
    unsigned __int64  m_start;
    unsigned __int64  m_overhead;

    CTimer(void)
    {
        m_overhead = 0;
        Start(); /// we get the start cycle
        m_overhead = Stop();  // then we get the stop cycle catching the overhead time
    }
    
    void Start(void)
    {
        m_start = theCycleCount();
    }

    unsigned __int64 Stop(void)
    {
        /// with the stop cycle we remove the overhead's time
        return theCycleCount() - m_start - m_overhead;
    }
};



//------------------------------------------------------------------------------------------------------------

CInfo::CInfo()
{
	m_bLoadedWindowsInfo = FALSE;
	m_bLoadedProgramInfo = FALSE;
	m_nMajor = 0;
	m_nMinor = 0;
	m_nRevision = 0;
	m_bUnicode = FALSE;
}

CInfo::~CInfo()
{
}



// Public methods
//------------------------------------------------------------------------------------------------------------



// Returns True if Windows version is Unicode-based.
BOOL CInfo::Unicode()
{
	LoadWindowsInfo();
	return m_bUnicode;
}


// Gets Windows version as string eg "Windows 98 SR2".
CString& CInfo::GetWindowsVersion()
{
	LoadWindowsInfo();
	return m_strWindowsVersion;
}

// Get Windows Common Controls version number, eg 5.8
float CInfo::GetCommonControlsVersionNumber()
{
	LoadWindowsInfo();
	return m_fCommonControlsVersionNumber;
}

// Get Rich Edit version number
float CInfo::GetRichEditVersionNumber()
{
	LoadWindowsInfo();
	return m_fRichEditVersionNumber;
}


// Get Rich Edit version string
LPCTSTR CInfo::GetRichEditVersionString()
{
	LoadWindowsInfo();
	return m_strRichEditVersion;
}




// Gets program version as a string eg "1.0d"
CString& CInfo::GetProgramVersion()
{
	LoadProgramInfo();
	return m_strProgramVersion;
}


CString& CInfo::GetProgramBuild() // eg "Build 92 (2006/01/02)"
{
	LoadProgramInfo();
	return m_strProgramBuild;
}


// Gets program copyright as a string eg "Copyright (c) 2001-2004 NeoMem.org"
CString& CInfo::GetProgramCopyright()
{
	LoadProgramInfo();
	return m_strProgramCopyright;
}


/*
// Gets program .exe modified date eg "15 June 2002"
CString& CInfo::GetProgramDate()
{
	LoadProgramInfo();
	return m_strProgramDate;
}
*/


// Get version number in integer form, eg for version 1.3.20 will return 1320.
int CInfo::GetProgramVersionNumber()
{
	LoadProgramInfo();
	int nVersion;
//	nVersion = m_nMajor * 10000000 + m_nMinor * 1000000 + m_nRevision * 10000 + m_nRelease;
	nVersion = m_nMajor * 1000 + m_nMinor * 100 + m_nRevision;
	return nVersion;
}



// Gets amount of memory available as a string eg "32.5MB".
CString& CInfo::GetMemoryAvailable()
{
	LoadMemoryInfo();
//	float fMBAvail = m_ms.dwTotalVirtual/1024.0f/1024.0f;
//	float fMBAvail = m_ms.dwAvailVirtual/1024.0f/1024.0f;
	float fAvailVirtual = m_ms.dwAvailVirtual/1024.0f/1024.0f;
	float fTotalVirtual = m_ms.dwTotalVirtual/1024.0f/1024.0f;
	float fAvailPhys = m_ms.dwAvailPhys/1024.0f/1024.0f;
	float fTotalPhys = m_ms.dwTotalPhys/1024.0f/1024.0f;
//	m_strMemoryAvailable.Format("%.1fMB", fTotalVirtual);
//	m_strMemoryAvailable.Format("%.1fMB", fAvailVirtual);
//	m_strMemoryAvailable.Format("%.1fMB Virtual Memory Available", fMBAvail);
	m_strMemoryAvailable.Format("%.1fMB of %.1fMB Virtual Memory Available, %.1fMB or %.1fMB Physical Memory Available", 
		fAvailVirtual, fTotalVirtual, fAvailPhys, fTotalPhys
		);
	return m_strMemoryAvailable;
}



CString& CInfo::GetVirtualMemory()
{
	LoadMemoryInfo();
	float fAvailVirtual = m_ms.dwAvailVirtual/1024.0f/1024.0f;
	float fTotalVirtual = m_ms.dwTotalVirtual/1024.0f/1024.0f;
//	m_strVirtualMemory.Format("%.1fMB of %.1fMB Available", fAvailVirtual, fTotalVirtual);
	m_strVirtualMemory.Format("%.0fMB of %.0fMB Virtual", fAvailVirtual, fTotalVirtual);
	return m_strVirtualMemory;
}

CString& CInfo::GetPhysicalMemory()
{
	LoadMemoryInfo();
	float fAvailPhys = m_ms.dwAvailPhys/1024.0f/1024.0f;
	float fTotalPhys = m_ms.dwTotalPhys/1024.0f/1024.0f;
//	m_strPhysicalMemory.Format("%.1fMB of %.1fMB Available", fAvailPhys, fTotalPhys);
	m_strPhysicalMemory.Format("%.0fMB of %.0fMB Physical", fAvailPhys, fTotalPhys);
	return m_strPhysicalMemory;
}



// Gets amount of disk space available as a string eg "105MB on C:"
CString& CInfo::GetDiskSpaceAvailable()
{
	LoadDiskInfo();
	m_strDiskSpace.Format("%I64uMB on %s", m_i64DiskFreeSpace.QuadPart / (1024 * 1024), (LPCSTR) m_strDrive);
	return m_strDiskSpace;
}



CString& CInfo::GetProcessor()
{
	LoadProcessorInfo();
	//, handle GHz
	m_strProcessor.Format("%s %s (%dMHz)", 
		(LPCTSTR) m_strProcessorVendor, 
		(LPCTSTR) m_strProcessorIdentifier, 
		m_lngProcessorSpeed
		);
	return m_strProcessor;
}






// Private routines
//------------------------------------------------------------------------------------------------------------

// Get the version number of the specified dll file. 
// Packs the major and minor version into a DWORD.
DWORD GetDllVersion(LPCTSTR lpszDllName)
{
	HINSTANCE hinstDll;
	DWORD dwVersion = 0;

	//, Be sure to use AfxFreeLibrary and AfxLoadLibrary (instead of the Win32 functions 
	// FreeLibrary and LoadLibrary) if your application uses multiple threads. 
	// Using AfxLoadLibrary and AfxFreeLibrary ensures that the startup and shutdown 
	// code that executes when the extension DLL is loaded and unloaded does not 
	// corrupt the global MFC state.
	hinstDll = ::LoadLibrary(lpszDllName);
//	hinstDll = AfxLoadLibrary(lpszDllName);
	if (hinstDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC) ::GetProcAddress(hinstDll, "DllGetVersion");

		// Because some DLLs may not implement this function, you
		// must test for it explicitly. Depending on the particular 
		// DLL, the lack of a DllGetVersion function may
		// be a useful indicator of the version.
		if (pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			HRESULT hr;
			::ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			hr = (*pDllGetVersion) (&dvi);
			if (SUCCEEDED(hr))
			{  
				dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
			}
		}
		VERIFY(::FreeLibrary(hinstDll));
//		VERIFY(AfxFreeLibrary(hinstDll));
	}
	return dwVersion;
}




// Get Windows version information.
void CInfo::LoadWindowsInfo()
{
	// Only need to update once
	if (m_bLoadedWindowsInfo) return;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure, which is 
	// supported on Windows 2000. If that fails, try using the OSVERSIONINFO structure.
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;
	::ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = ::GetVersionEx((OSVERSIONINFO*) &osvi);
	if (!bOsVersionInfoEx)
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (!::GetVersionEx((OSVERSIONINFO*) &osvi))
			return;
	}

	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		{
			// Set unicode flag 
			m_bUnicode = TRUE;

			// Test for the product
			if (osvi.dwMajorVersion <= 4)
			{
				m_strWindowsVersion = _T("Windows NT ");
			}
			else if (osvi.dwMajorVersion == 5)
			{
				if (osvi.dwMinorVersion==0)
					m_strWindowsVersion = _T("Windows 2000 ");
				else if (osvi.dwMinorVersion==1)
					m_strWindowsVersion = _T("Windows XP ");
				else
					//for unknown windows/newest windows version
					m_strWindowsVersion = _T("Windows ");
			}
			else
				//for unknown windows/newest windows version
				m_strWindowsVersion = _T("Windows ");

			// Test for workstation versus server
//			if (bOsVersionInfoEx)
//			{
//				if (osvi.wProductType == VER_NT_WORKSTATION)
//					m_strPlatform += "Professional ";
//				if (osvi.wProductType == VER_NT_SERVER)
//					m_strPlatform += "Server ";
//			}
//			else
//			{
//				HKEY hKey;
//				char szProductType[80];
//				DWORD dwBufLen;
//				::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
//						0, KEY_QUERY_VALUE, &hKey);
//				::RegQueryValueEx(hKey, "ProductType", NULL, NULL, (LPBYTE) szProductType, &dwBufLen);
//				::RegCloseKey(hKey);
//				if (lstrcmpi( "WINNT", szProductType) == 0)
//					m_strPlatform += "Workstation ";
//				if (lstrcmpi( "SERVERNT", szProductType) == 0)
//					m_strPlatform += "Server ";
//			}

		}
		break;

	case VER_PLATFORM_WIN32_WINDOWS:
		if ((osvi.dwMajorVersion > 4) || ((osvi.dwMajorVersion == 4) && (osvi.dwMinorVersion > 0)))
			m_strWindowsVersion = _T("Windows 98 ");
		else
		{
			// If the dwPlatformId member of that structure is VER_PLATFORM_WIN32_WINDOWS, and the low 
			// word of the dwBuildNumber member is "1111", the system is Windows 95 OSR 2. For Windows 
			// 95 and Windows 95 OSR 2, the dwMajorVersion element returns "4" and dwMinorVersion returns 
			// "0." Future versions or OSRs of Windows 95 will have higher build numbers. They may also have 
			// higher major/minor version numbers.
			if (LOWORD(osvi.dwBuildNumber) >= 1111)
				m_strWindowsVersion = _T("Windows 95 OSR 2 ");
			else
				m_strWindowsVersion = _T("Windows 95 ");
		}
		break;

	case VER_PLATFORM_WIN32s:
		m_strWindowsVersion = _T("Win32s ");
		break;
	}

	// Display version, service pack (if any), and build number.
	CString s;
	if (osvi.szCSDVersion)
		s.Format(_T("Version %d.%d %s (Build %d)"), 
			osvi.dwMajorVersion, 
			osvi.dwMinorVersion, 
			osvi.szCSDVersion, 
			osvi.dwBuildNumber & 0xFFFF);
	else
		s.Format(_T("Version %d.%d (Build %d)"), 
			osvi.dwMajorVersion, 
			osvi.dwMinorVersion, 
			osvi.dwBuildNumber & 0xFFFF);
	m_strWindowsVersion += s;


	// Now get common control version information
	LPCTSTR szLibraryName = _T("comctl32.dll");
	DWORD dwVersion = GetDllVersion(szLibraryName);
	WORD wMajor = HIWORD(dwVersion);
	WORD wMinor = LOWORD(dwVersion);
//	CString str;
//	str.Format("%d.%d", wMajor, wMinor);
//	m_fCommonControlsVersionNumber = (float) atof(str); //... tchar???
	m_fCommonControlsVersionNumber = (float) ((float) wMajor + (float) wMinor / 100.0f);


	// Get richedit control version information
	// Note: GetDllVersion won't work because the dll's don't export DllGetVersion!
	CModuleVersion ver;
	if (ver.GetFileVersionInfo("Riched20.dll")) // see if user has this file
	{
		wMajor = HIWORD(ver.dwFileVersionMS);
		wMinor = LOWORD(ver.dwFileVersionMS);
		m_strRichEditVersion.Format("%d.%d.%d.%d",
			wMajor, wMinor, HIWORD(ver.dwFileVersionLS), LOWORD(ver.dwFileVersionLS)
			);
		m_fRichEditVersionNumber = (float) ((float) wMajor + (float) wMinor / 100.0f);
	}
	else
	{
//		ver.GetFileVersionInfo(_T("Riched32.dll")); // this might return 5.0, so just set manually
		m_strRichEditVersion = "1.0";
		m_fRichEditVersionNumber = 1.0f;
	}

	// Check for richedit enhanced features
//	LPCTSTR szEnhancedFeatures = "";
//	if (m_fRichEditVersionNumber < 5.0f)
//		szEnhancedFeatures = "(Enhanced features not available)";


	//, Get cryptographic provider version info
	// To determine the actual version of the provider in use, call CryptGetProvParam with the 
	// dwParam argument set to PP_VERSION. If 0x0200 is returned in pbData, then you have version 2.0.
//	::CryptGetProvParam(

	// Set flag
	m_bLoadedWindowsInfo = TRUE;

}



// maybe it's better to just hardcode the labels for now
// fill a structure with program, company, version, release date, etc. info
// will be used by about dialog and error log
void CInfo::LoadProgramInfo()
{
	// Only need to update once
	if (m_bLoadedProgramInfo) return;

	// Get name of the executable file
	// The GetModuleFileName function retrieves the full path and filename for the 
	// executable file containing the specified module. 
	TCHAR szExeName[MAX_PATH];
	if (!::GetModuleFileName(NULL, szExeName, MAX_PATH))
		return;

	// Open the NeoMem.exe file to get version info
	CFileVersion f;
	CFileException e;
	UINT nOpenFlags = CFile::modeRead | CFile::shareDenyNone;
	if (!f.Open(szExeName, nOpenFlags, &e))
	{
		e.ReportError();
		return;
	}

	// Get program date
	// 1.1d not a reliable way of getting the build date!
//	CFileStatus fs;
//	f.GetStatus(fs);
//	m_tProgramDate = fs.m_mtime;
//	m_strProgramDate = m_tProgramDate.Format("%#d %B %Y"); // eg 15 June 2002

	// Get some strings
	f.GetVersionValue("ProductName", m_strProgramName); // eg "NeoMem"
//	f.GetVersionValue("ProductVersion", m_strProgramVersion); // eg "0.9"
//	f.GetVersionValue("FileVersion", m_strFileVersion); // eg "0.9f"
	f.GetVersionValue("LegalCopyright", m_strProgramCopyright); // eg "Copyright (c) 2001-2004 NeoMem.org"
//	"CompanyName"
//	"FileDescription"
//	"FileVersion"
//	"InternalName"
//	"OriginalFilename"

	CString str;
	f.GetVersionValue("ProductVersion", str); // eg "0.9i, Build 77 (2006-01-02)"
//	m_strProgramVersion = str.Left(str.GetLength() - 1); // eg "0.9"
//	m_strProgramBuild = str.Right(1); // eg "i"
//	m_strProgramVersion = str.Left(3); // eg "0.9"
//	m_strProgramBuild = str.Mid(3); // eg "i (build 77)"
	int nComma = str.Find(_T(','));
	m_strProgramVersion = str.Left(nComma); //  eg "0.9i"
	m_strProgramBuild = str.Mid(nComma+2); // eg "Build 77 (2006-01-02)"

	//. Get version numbers from version # binary resource (two long ints)
	// Specifies the binary version number for the file. The version consists of two 32-bit integers, 
	// defined by four 16-bit integers. For example, "FILEVERSION 3,10,0,61" is translated into two 
	// doublewords: 0x0003000a and 0x0000003d, in that order. Therefore, if version is defined by 
	// the DWORD values dw1 and dw2, they need to appear in the FILEVERSION statement as follows: 
	// HIWORD(dw1), LOWORD(dw1), HIWORD(dw2), LOWORD(dw2).
//	m_nMajor = 0;
//	m_nMinor = 9;
//	m_nRevision = 0;

	// Set flag
	m_bLoadedProgramInfo = TRUE;

}



// Get memory status information
void CInfo::LoadMemoryInfo()
{
	memset(&m_ms, sizeof(MEMORYSTATUS), 0);
	m_ms.dwLength = sizeof(MEMORYSTATUS);
	::GlobalMemoryStatus(&m_ms);

	// The "Percent memory in use" figure has nothing to do with the percentage of virtual 
	//     memory in use at the time. In Microsoft documentation it is defined as "a measure of how 
	//     busy the memory management system is." It can be calculated as 50 percent of the ratio 
	//     of the committed (requested for use by an application) memory pages to the physical 
	//     (actual RAM) memory pages managed by Windows 95. 
	// The "Bytes of physical memory present" figure is the amount of physical RAM minus any 
	//     holes in the upper memory area (UMA), used by adapters, and so forth. 
	// The "Bytes of available physical memory" figure shows how much of this physical memory is available. 
	// The "Bytes of paging file" figure is the size of the current paging file. This figure represents 
	//     the number of bytes in the paging file that are uncommitted to any process. 
	// The "Total Virtual Memory" figure indicates the size of the process' address space. Under 
	//     Windows 95, this figure is exactly 4 MB short of being 2 GB. Under Windows 95 every 
	//     process has a 2 GB private address space and the lower 4 MB of this address space is 
	//     protected. Under Windows NT, this figure would always be 128 KB short of 2 GB, since 
	//     Windows NT protects the 64 KB at the start and end of the process' 2 GB private address space. 
	// The difference between total vm and avail vm is the amount of memory reserved by the 
	//     process. 

	xTRACE("Memory usage: \n"
		"    Load %d pct\n"
		"    Physical Memory (Physical RAM minus upper memory area (UMA) used by adapters) %d bytes\n"
		"    Physical Memory Available %d bytes\n"
		"    Physical Memory Used %d bytes\n"
		"    Total Virtual Memory (size of process' address space) %d bytes\n"
		"    Virtual Memory Available %d bytes\n"
		"    Virtual Memory Allocated %d bytes\n",
		m_ms.dwMemoryLoad,
		m_ms.dwTotalPhys,
		m_ms.dwAvailPhys,
		m_ms.dwTotalPhys - m_ms.dwAvailPhys,
		m_ms.dwTotalVirtual,
		m_ms.dwAvailVirtual,
		m_ms.dwTotalVirtual - m_ms.dwAvailVirtual
		);

}





// Get disk info (free space available).
// Fills m_i64DiskFreeSpace with number of bytes free.
void CInfo::LoadDiskInfo()
{
	BOOL bResult = FALSE;
	m_i64DiskFreeSpace.QuadPart = 0;

	// Note: GetDiskFreeSpaceEx is only available on Win95 SR2 and up, so need to handle both cases here.

	// First get the system drive name or UNC path
	TCHAR pszSystemDir[MAX_PATH];
	CString strDrive; 
	if (::GetSystemDirectory(pszSystemDir, MAX_PATH))
	{
		m_strDrive = pszSystemDir;

		// If the drive is a drive letter and not a UNC path, append a trailing backslash to 
		// the drive letter and colon. This is required on Windows 95 and 98.      
		if (m_strDrive[1] == ':')
		{
//			m_strDrive = m_strDrive.Left(1) + ":\\";
			m_strDrive = m_strDrive.Left(1) + ":"; // eg "C:"
			strDrive = m_strDrive + "\\"; // eg "C:\\"
		}
		else
		{
			strDrive = m_strDrive;
		}
	}

	// See if GetDiskFreeSpaceEx is available
	// If your program can run on retail Windows 95, you should not directly call GetDiskFreeSpaceEx 
	// because this function is not implemented in retail Windows 95, and calling it directly will 
	// prevent your program from loading. Instead, you should dynamically link to it via 
	// GetProcAddress. If the returned pointer is non-NULL, then your application can safely call 
	// GetDiskFreeSpaceEx through the pointer; if the returned pointer is NULL, then you should 
	// revert to GetDiskFreeSpace. 
	// Note: Since GetDiskFreeSpaceEx is not in Windows 95 Retail, we dynamically link to it 
	// and only call it if it is present.  We don't need to call LoadLibrary on KERNEL32.DLL because it is 
	// already loaded into every Win32 process's address space.  
	P_GDFSE pGetDiskFreeSpaceEx = (P_GDFSE) ::GetProcAddress(::GetModuleHandle("kernel32.dll"), "GetDiskFreeSpaceExA");
	if (pGetDiskFreeSpaceEx)
	{
		ULARGE_INTEGER i64FreeBytesToCaller; // Windows 2000: If per-user quotas are in use, this value may be less than the total number of free bytes on the disk. 
		ULARGE_INTEGER i64TotalBytes; // Windows 2000: If per-user quotas are in use, this value may be less than the total number of bytes on the disk. 
		ULARGE_INTEGER i64FreeBytes;
//		bResult = ::GetDiskFreeSpaceEx(m_strDrive, &i64FreeBytesToCaller, &i64TotalBytes, &i64FreeBytes);
		bResult = ::GetDiskFreeSpaceEx(strDrive, &i64FreeBytesToCaller, &i64TotalBytes, &i64FreeBytes);
		if (bResult)
		{
			m_i64DiskFreeSpace = i64FreeBytesToCaller;
		}
	}
	else 
	{
		// Because many existing applications failed on drives larger than 2 GB, a change has 
		// been made in the API function GetDiskFreeSpace. The function now returns a maximum 
		// value of 2 GB, even if the drive has more than 2 GB free. A new API function 
		// GetDiskFreeSpaceEx will return the true free drive space. Applications running on 
		// Windows 95 or Windows NT that need the true free drive space should call GetDiskFreeSpaceEx.
		// The proper way to multiply the returned values of GetDiskFreeSpace is to use 64-bit math. 
		DWORD dwSectorsPerCluster;
		DWORD dwBytesPerSector; 
		DWORD dwFreeClusters; 
		DWORD dwTotalClusters; 
//		bResult = ::GetDiskFreeSpace(m_strDrive, &dwSectorsPerCluster, &dwBytesPerSector, &dwFreeClusters, &dwTotalClusters);
		bResult = ::GetDiskFreeSpace(strDrive, &dwSectorsPerCluster, &dwBytesPerSector, &dwFreeClusters, &dwTotalClusters);
		if (bResult)
		{
//			m_i64DiskTotalSpace = (__int64)dwTotalClusters * dwSectPerClust * dwBytesPerSect;
            m_i64DiskFreeSpace.QuadPart = (__int64)dwFreeClusters * dwSectorsPerCluster * dwBytesPerSector;
		}
	}

	if (!bResult)
	{
		// Error - could not get free space for ...
//		AfxMessageBox("error!");
	}
	
}




// Get processor type and speed
void CInfo::LoadProcessorInfo()
{
	// Only do once
	if (m_bLoadedProcessorInfo) return;

	CString strRawClockFrequency;
	CTimer timer;

//	long tickslong;
//	long tickslongextra;

	timer.Start();
	Sleep(1000);
	unsigned cpuspeed100 = (unsigned)(timer.Stop()/10000);
//	tickslong = cpuspeed100/100;
	m_lngProcessorSpeed = cpuspeed100/100;
//	tickslongextra = cpuspeed100 - (tickslong*100);
//	strRawClockFrequency.Format("%d.%d   MHz   estimate ", tickslong,tickslongextra);
//	strRawClockFrequency.Format("%d MHz", tickslong);
//	m_strProcessorSpeed.Format("%d MHz", tickslong);
//	m_RawClockFrequencyControl.SetWindowText(strRawClockFrequency);	

//	m_strProcessorVendor = GetProfileString(szSettings, szRegistrationName, ""); // eg "GenuineIntel"
//	m_strProcessorIdentifier = GetProfileString(szSettings, szRegistrationName, ""); // eg "Pentium(r) II Processor"

	CRegKey r;
	LPTSTR psz;
	DWORD dwChars = 256;

	r.Open(HKEY_LOCAL_MACHINE, szCentralProcessor);
	
	psz = m_strProcessorIdentifier.GetBuffer(dwChars);
	r.QueryValue(psz, "ProcessorNameString", &dwChars); // eg "Pentium(r) II Processor"
	m_strProcessorIdentifier.ReleaseBuffer();
	m_strProcessorIdentifier.TrimLeft();
	m_strProcessorIdentifier.TrimRight();

	if (m_strProcessorIdentifier.IsEmpty())
	{
		psz = m_strProcessorIdentifier.GetBuffer(dwChars);
		r.QueryValue(psz, "Identifier", &dwChars); // eg "Pentium(r) II Processor"
		m_strProcessorIdentifier.ReleaseBuffer();
	}

	psz = m_strProcessorVendor.GetBuffer(dwChars);
	r.QueryValue(psz, "VendorIdentifier", &dwChars); // eg "GenuineIntel"
	m_strProcessorVendor.ReleaseBuffer();
	
	r.Close();

	// Set flag
	m_bLoadedProcessorInfo = TRUE;
}



CString& CInfo::GetScreenInfo()
{
	int nX = ::GetSystemMetrics(SM_CXSCREEN);
	int nY = ::GetSystemMetrics(SM_CYSCREEN);
	CWindowDC dc(AfxGetMainWnd());
	int nBits = dc.GetDeviceCaps(BITSPIXEL);
	m_strScreen.Format("%d x %d, Depth %d bits",
		nX,
		nY,
		nBits
		);
	return m_strScreen;
}





CString& CInfo::GetLocaleInfo()
{
	const int nChars = 256;

	// Get language and locale information
	LANGID nLanguage = ::GetUserDefaultLangID();
	LCID nLocale = ::GetUserDefaultLCID();

	LPTSTR pszLocaleName = m_strLocaleName.GetBuffer(nChars);
	::GetLocaleInfo(nLocale, LOCALE_SENGLANGUAGE, pszLocaleName, nChars);
	m_strLocaleName.ReleaseBuffer();

//	m_strLocaleInfo.Format("LocaleID=%x LanguageID=%x", nLocale, nLanguage);
	m_strLocaleInfo.Format("LocaleID=%x (%s) LanguageID=%x", nLocale, (LPCTSTR) m_strLocaleName, nLanguage);

	return m_strLocaleInfo;
}




