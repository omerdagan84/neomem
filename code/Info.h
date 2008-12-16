
// CInfo interface
// Create an object of this class to obtain information about the program version,
// windows version, memory available, disk space available, etc.
//-----------------------------------------------------------------------------------------------------------


#pragma once


class CInfo  
{
// Construction
public:
	CInfo();
	virtual ~CInfo();

// Operations
public:
	CString& GetLocaleInfo();
	CString& GetScreenInfo();
	CString& GetPhysicalMemory();
	CString& GetVirtualMemory();
	CString& GetProcessor();
	CString& GetDiskSpaceAvailable();

	CString& GetWindowsVersion();
//	CString& GetCommonControlsVersion();
	float GetCommonControlsVersionNumber();
	float GetRichEditVersionNumber();
	LPCTSTR GetRichEditVersionString();
	BOOL Unicode();

	CString& GetProgramVersion();
	CString& GetProgramBuild();
	CString& GetProgramCopyright();
//	CString& GetProgramDate();
	int GetProgramVersionNumber();

	CString& GetMemoryAvailable();


// Attributes
private: 
	BOOL m_bLoadedWindowsInfo;
	BOOL m_bLoadedProgramInfo;
	BOOL m_bLoadedProcessorInfo;

	// Windows
	CString m_strWindowsVersion; // eg "Windows 98"
	float m_fCommonControlsVersionNumber; // eg 5.8
	float m_fRichEditVersionNumber; // eg 5.30
	CString m_strRichEditVersion; // full version info eg "5.30.23.1203"

//	CString m_strCommonControlsVersion;
	BOOL m_bUnicode;
//	WORD m_wCommonMajor;
//	WORD m_wCommonMinor;
//	DWORD m_dwCommon;
//	CString m_strLanguageName;
	CString m_strLocaleName;
	CString m_strLocaleInfo;

	// Program
	CString m_strProgramName;
	CString m_strProgramVersion; // eg 0.9i
	CString m_strProgramBuild; // eg Build 92 (2006/01/02)
	CString m_strProgramCopyright;
	CTime m_tProgramDate;
//	CString m_strProgramDate;
	int m_nMajor;
	int m_nMinor;
	int m_nRevision;

	// Memory
	MEMORYSTATUS m_ms;
	CString m_strMemoryAvailable;
	CString m_strPhysicalMemory;
	CString m_strVirtualMemory;

	// Disk space
	CString m_strDrive;
	CString m_strDiskSpace;
	ULARGE_INTEGER m_i64DiskFreeSpace;

	// Processor
	long m_lngProcessorSpeed;
	CString m_strProcessorVendor;
	CString m_strProcessorIdentifier;
	CString m_strProcessor; // eg "GenuineIntel Pentium(r) II Processor 300MHz"

	// Screen
	CString m_strScreen;

// Implementation
private:
	void LoadDiskInfo();
	void LoadWindowsInfo();
	void LoadProcessorInfo();
	void LoadProgramInfo();
	void LoadMemoryInfo();
};


