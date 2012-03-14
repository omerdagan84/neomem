
// MirrorFileEx.cpp: implementation of the CMirrorFileEx class.


#include "precompiled.h"
#include "neomem.h"
#include "MirrorFileEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



extern void AFXAPI AfxGetRoot(LPCTSTR lpszPath, CString& strRoot);
extern void AFX_CDECL AfxTimeToFileTime(const CTime& time, LPFILETIME pFileTime);

// WARNING: AfxImpl.h is subject to change in later
// versions of MFC and, as such, may break a project in the future.
// It includes the afxData structure with system colors etc.
// #include <..\src\afximpl.h> 



CString CMirrorFileEx::GetTempName(LPCTSTR lpszOriginalFile, BOOL bCreate)
{
	CString str;

	// get the directory for the file

	TCHAR szPath[_MAX_PATH];
	LPTSTR lpszName;
	GetFullPathName(lpszOriginalFile, _MAX_PATH, szPath, &lpszName);
	*lpszName = NULL;

	// let's create a temporary file name, and create
	// a file too!

	GetTempFileName(szPath, _T("MFC"), 0,
		str.GetBuffer(_MAX_PATH+1));
	str.ReleaseBuffer();

	// delete the file if the user just wants a name

	if (!bCreate)
//`		CFile::Remove(str);
		CCryptoFile::Remove(str);

	return str;
}



BOOL CMirrorFileEx::Open(LPCTSTR lpszFileName, UINT nOpenFlags,
	CFileException* pError)
{
	ASSERT(lpszFileName != NULL);
	m_strMirrorName.Empty();

	CFileStatus status;
	if (nOpenFlags & CFile::modeCreate) //opened for writing
	{
//`		if (CFile::GetStatus(lpszFileName, status))
		if (CCryptoFile::GetStatus(lpszFileName, status))
		{
			CString strRoot;
			AfxGetRoot(lpszFileName, strRoot);

			DWORD dwSecPerClus, dwBytesPerSec, dwFreeClus, dwTotalClus;
			DWORD nBytes = 0;
			if (GetDiskFreeSpace(strRoot, &dwSecPerClus, &dwBytesPerSec, &dwFreeClus,
				&dwTotalClus))
			{
				nBytes = dwFreeClus * dwSecPerClus * dwBytesPerSec;
			}
			if (nBytes > 2 * DWORD(status.m_size)) // at least 2x free space avail
			{
				m_strMirrorName = GetTempName(lpszFileName, TRUE);
			}
		}
	}

	if (!m_strMirrorName.IsEmpty() &&
//		CFile::Open(m_strMirrorName, nOpenFlags, pError))
		CCryptoFile::Open(m_strMirrorName, nOpenFlags, pError))
	{
		m_strFileName = lpszFileName;
		FILETIME ftCreate, ftAccess, ftModify;
		if (::GetFileTime((HANDLE)m_hFile, &ftCreate, &ftAccess, &ftModify))
		{
			AfxTimeToFileTime(status.m_ctime, &ftCreate);
			SetFileTime((HANDLE)m_hFile, &ftCreate, &ftAccess, &ftModify);
		}

		DWORD dwLength = 0;
		PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL;
		if (GetFileSecurity(lpszFileName, DACL_SECURITY_INFORMATION,
			NULL, dwLength, &dwLength))
		{
			pSecurityDescriptor = (PSECURITY_DESCRIPTOR) new BYTE[dwLength];
			if (::GetFileSecurity(lpszFileName, DACL_SECURITY_INFORMATION,
				pSecurityDescriptor, dwLength, &dwLength))
			{
				SetFileSecurity(m_strMirrorName, DACL_SECURITY_INFORMATION, pSecurityDescriptor);
			}
			delete[] (BYTE*)pSecurityDescriptor;
		}
		return TRUE;
	}
	m_strMirrorName.Empty();
//`	return CFile::Open(lpszFileName, nOpenFlags, pError);
	return CCryptoFile::Open(lpszFileName, nOpenFlags, pError);
}



void CMirrorFileEx::Abort()
{
//`	CFile::Abort();
	CCryptoFile::Abort();
	if (!m_strMirrorName.IsEmpty())
//`		CFile::Remove(m_strMirrorName);
		CCryptoFile::Remove(m_strMirrorName);
}



typedef BOOL (WINAPI* ReplaceAPIPtr)(LPCWSTR, LPCWSTR, LPCWSTR,
						DWORD, LPVOID, LPVOID);
#ifndef REPLACEFILE_WRITE_THROUGH
#define REPLACEFILE_WRITE_THROUGH 0x00000001
#endif
#ifndef REPLACEFILE_IGNORE_MERGE_ERRORS
#define REPLACEFILE_IGNORE_MERGE_ERRORS 0x00000002
#endif

#ifndef ERROR_UNABLE_TO_MOVE_REPLACEMENT
#define ERROR_UNABLE_TO_MOVE_REPLACEMENT 1176L
#endif
#ifndef ERROR_UNABLE_TO_MOVE_REPLACEMENT_2
#define ERROR_UNABLE_TO_MOVE_REPLACEMENT_2 1177L
#endif


void CMirrorFileEx::Close()
{
	CString m_strName = m_strFileName; //file close empties string
//`	CFile::Close();
	CCryptoFile::Close();
	if (!m_strMirrorName.IsEmpty())
	{
		BOOL bWorked = FALSE;
		DWORD dwResult = 0;
        ReplaceAPIPtr pfn = NULL;
		CString strBackupName;

		/*` win95 is always true now
		if (!afxData.bWin95)
		{
			HMODULE hModule = GetModuleHandleA("KERNEL32");
			ASSERT(hModule != NULL);

	       pfn = (ReplaceAPIPtr) GetProcAddress(hModule, "ReplaceFile");

			if (pfn != NULL)
			{
				USES_CONVERSION;

				strBackupName = GetTempName(m_strMirrorName, FALSE);

				// this NT API handles copying all attributes for us
				bWorked = (pfn)(T2W((LPTSTR)(LPCTSTR)m_strName),
                            T2W((LPTSTR)(LPCTSTR)m_strMirrorName),
                            T2W((LPTSTR)(LPCTSTR)strBackupName),
					REPLACEFILE_WRITE_THROUGH | REPLACEFILE_IGNORE_MERGE_ERRORS,
					NULL, NULL);

				if (!bWorked)
				   dwResult = GetLastError();
			}
		}
		*/

		if (!bWorked)
		{
			if (dwResult == ERROR_UNABLE_TO_MOVE_REPLACEMENT || dwResult == 0)
//`				CFile::Remove(m_strName);
				CCryptoFile::Remove(m_strName);

			if (dwResult == ERROR_UNABLE_TO_MOVE_REPLACEMENT_2)
//`			   CFile::Remove(strBackupName);
			   CCryptoFile::Remove(strBackupName);

//`			CFile::Rename(m_strMirrorName, m_strName);
			CCryptoFile::Rename(m_strMirrorName, m_strName);
		}
      else if (pfn != NULL)
      {
//`         CCryptoFile::Remove(strBackupName);
         CFile::Remove(strBackupName);
      }
	}
}

