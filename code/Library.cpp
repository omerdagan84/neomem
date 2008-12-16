

// Library.cpp
// General purpose routines


#include "precompiled.h"
#include "Library.h"
#include "Error.h"





NUMBERFMT g_nf;

COLORREF g_clrWindow;
COLORREF g_clrWindowText;
COLORREF g_clrHighlight;
COLORREF g_clrHighlightText;
COLORREF g_clrInactive;
COLORREF g_clrInactiveText;
COLORREF g_clr3dFace;
COLORREF g_clr3dHighlight;
COLORREF g_clr3dShadow;
COLORREF g_clrHyperlinkText;






// Returns True if the given file exists.
BOOL 
FileExists(LPCTSTR pszFileName)
{
	CFile f;
	CFileException fe;
	BOOL bExists = f.Open(pszFileName, CFile::modeRead, &fe);
	if (bExists)
		f.Close();
	return bExists;
}




// Get file name with extension from the given path.
// Eg returns "Stuff.neo" from "C:\Documents\Stuff.neo"
// Code from mfc routine.
CString 
GetFileName(LPCTSTR pszPathName)
{
//	CFile f(szPath, CFile::modeRead);
//	CString s = f.GetFileName();
//	f.Close();
//	AfxGetFileName(status.m_szFullName, strResult.GetBuffer(_MAX_FNAME), _MAX_FNAME);

	CString s;

	// Always capture the complete file name including extension (if present)
	LPTSTR pszTemp = (LPTSTR) pszPathName;
	for (LPCTSTR psz = pszPathName; *psz != '\0'; psz = _tcsinc(psz))
	{
		// Remember last directory/drive separator
		if (*psz == '\\' || *psz == '/' || *psz == ':')
			pszTemp = (LPTSTR)_tcsinc(psz);
	}

	s = pszTemp;
	return s;	
}



// Format the given number with proper thousand formatting and return as a new CString object.
// To use in a printf, be sure to cast the result to LPCTSTR!!
//. use FormatThousands in CStringEx instead!
CString 
fc(long n)
{
	const int nChars = 50;
	TCHAR szPlain[nChars];
	TCHAR szFormatted[nChars];

//	_stprintf(szPlain, "%d", n);
	wsprintf(szPlain, "%d", n);

	// this always prints 2 decimal places - lame!
//	::GetNumberFormat(LOCALE_USER_DEFAULT, 0, szPlain, NULL, szFormatted, nChars);
	
//	NUMBERFMT nf;
//	nf.NumDigits = 0; // number of digits after decimal (default is 2)
//	nf.LeadingZero = 0;
//	nf.Grouping = 3;
//	nf.lpDecimalSep = ".";
//	nf.lpThousandSep = ",";
//	nf.NegativeOrder = 1;

//	::GetNumberFormat(0, 0, szPlain, &nf, szFormatted, nChars);
	::GetNumberFormat(0, 0, szPlain, &g_nf, szFormatted, nChars);
	return szFormatted;
}



// Get global colors
void 
GetColors()
{
	g_clrWindow = ::GetSysColor(COLOR_WINDOW);
	g_clrWindowText = ::GetSysColor(COLOR_WINDOWTEXT);
	g_clrHighlight = ::GetSysColor(COLOR_HIGHLIGHT);
	g_clrHighlightText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	g_clrInactive = ::GetSysColor(COLOR_SCROLLBAR); //, use a different color for this
	g_clrInactiveText = ::GetSysColor(COLOR_WINDOWTEXT);
	g_clr3dFace = ::GetSysColor(COLOR_3DFACE);
	g_clr3dHighlight = ::GetSysColor(COLOR_3DHILIGHT);
	g_clr3dShadow = ::GetSysColor(COLOR_3DSHADOW);
//	g_clrHyperlinkText = 0x00d00030; // blue?
//	g_clrHyperlinkText = 0x00f00000; // blue
//	g_clrHyperlinkText = 0x004090a0; // brown
	g_clrHyperlinkText = 0x0020a020; // green
//	g_clrHyperlinkText = 0x00ff0000; // blue - doesn't work!
//	g_clrHyperlinkText = RGB(0, 0, 255); // blue - "
}




// IsEdit: a helper function to determine if a given CWnd pointer points to a CEdit control.
// Use the SDK ::GetClassName() function because MFC IsKindOf fails if no CEdit variable 
// has been created for the control you're trying to test.
// This is used by cut copy paste code - see Q145616 in msdn 
BOOL 
IsEdit(CWnd *pWnd)
{
	ASSERT(pWnd);
	HWND hWnd = pWnd->GetSafeHwnd();
	if (hWnd == NULL)
		return FALSE;

	TCHAR szClassName[6];
	return ::GetClassName(hWnd, szClassName, 6) && _tcsicmp(szClassName, _T("Edit")) == 0;
}






// Function to convert unsigned char to string of length 2, eg 0xAE -> "AE"
void 
Char2Hex(const unsigned char ch, char* szHex)
{
	unsigned char byte[2];
	byte[0] = ch/16;
	byte[1] = ch%16;
	for(int i=0; i<2; i++)
	{
		if(byte[i] >= 0 && byte[i] <= 9)
			szHex[i] = '0' + byte[i];
		else
			szHex[i] = 'A' + byte[i] - 10;
	}
	szHex[2] = 0;
}

// Function to convert string of length 2 to unsigned char, eg "B9" -> 0xB9
void 
Hex2Char(const char* szHex, unsigned char& rch)
{
	rch = 0;
	for(int i=0; i<2; i++)
	{
		if(*(szHex + i) >='0' && *(szHex + i) <= '9')
			rch = (rch << 4) + (*(szHex + i) - '0');
		else if(*(szHex + i) >='A' && *(szHex + i) <= 'F')
			rch = (rch << 4) + (*(szHex + i) - 'A' + 10);
		else if(*(szHex + i) >='a' && *(szHex + i) <= 'f')
			rch = (rch << 4) + (*(szHex + i) - 'a' + 10);
		else
			break;
	}
}    

// Function to convert string of unsigned chars to string of chars, eg 0x9E3D -> "9E3D"
void 
CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iSize)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for(i=0; i<iSize; i++)
	{
		Char2Hex(pucCharStr[i], szHex);
		strcat(pszHexStr, szHex);
	}
}

// Function to convert string of chars to string of unsigned chars, eg "9E3D" -> 0x9E3D
void 
HexStr2CharStr(const char* pszHexStr, unsigned char* pucCharStr, int iSize)
{
	int i;
	unsigned char ch;
	for(i=0; i<iSize; i++)
	{
		Hex2Char(pszHexStr+2*i, ch);
		pucCharStr[i] = ch;
	}
}




// Use this to print debug strings that are longer than 512 characters.
// TRACE is limited to 512 chars!
// none of these work either
//	afxDump << (LPCTSTR) strNewText;
//	afxDump.OutputString(strNewText);
//	AfxOutputDebugString(strNewText);
void 
TRACESTRING(LPCTSTR szFormat, LPCTSTR szString)
{
	CString strString = szString;
	int nLen = strString.GetLength();
	if (nLen > 512)
	{
		CString strLeft = strString.Left(100);
		CString strRight = strString.Right(100);
		CString strNew = strLeft + " ..... " + strRight;
		TRACE(szFormat, (LPCTSTR) strNew);
	}
	else
		TRACE(szFormat, szString);
}

// Output the given string to the given file.
// Good for rtf debugging.
void 
TRACETOFILE(LPCTSTR szFilename, LPCTSTR szString)
{
	TRACE("Writing string to file %s...", szFilename);
	CStdioFile f(szFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	f.WriteString(szString);
	f.Close();
	TRACE(" done.\n");
}


// Get Windows error message
CString 
GetErrorMessage(DWORD dwError)
{
//	LPVOID lpMsgBuf;
	if (dwError == 0) 
		dwError = ::GetLastError();

	CString s;
	const int nChars = 1024;
	LPTSTR lpBuffer = s.GetBuffer(nChars);

	::FormatMessage(
//			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
//			(LPTSTR) &lpMsgBuf,
//			0,
			lpBuffer,
			nChars,
			NULL 
			);
	
	// Display the string.
//	MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
//	AfxMessageBox((LPCTSTR) lpMsgBuf, MB_OK | MB_ICONINFORMATION);
	// Free the buffer.
//	::LocalFree(lpMsgBuf);
//	CString s = (LPCTSTR) lpMsgBuf;
	s.ReleaseBuffer();

	return s;
}





void 
HandleShellExecuteError(HINSTANCE h)
{
	/* 
	Returns a value greater than 32 if successful, or an error value that is less 
	than or equal to 32 otherwise. The following table lists the error values. 
	The return value is cast as an HINSTANCE for backward compatibility with 
	16-bit Windows applications. It is not a true HINSTANCE, however. The 
	only thing that can be done with the returned HINSTANCE is to cast it to 
	an int and compare it with the value 32 or one of the error codes below.
	0	The operating system is out of memory or resources.
	ERROR_FILE_NOT_FOUND	The specified file was not found.
	ERROR_PATH_NOT_FOUND	The specified path was not found.
	ERROR_BAD_FORMAT	The .exe file is invalid (non-Microsoft Win32 .exe or error in .exe image).
	SE_ERR_ACCESSDENIED	The operating system denied access to the specified file.
	SE_ERR_ASSOCINCOMPLETE	The file name association is incomplete or invalid.
	SE_ERR_DDEBUSY	The Dynamic Data Exchange (DDE) transaction could not be completed because other DDE transactions were being processed.
	SE_ERR_DDEFAIL	The DDE transaction failed.
	SE_ERR_DDETIMEOUT	The DDE transaction could not be completed because the request timed out.
	SE_ERR_DLLNOTFOUND	The specified dynamic-link library (DLL) was not found.
	SE_ERR_FNF	The specified file was not found.
	SE_ERR_NOASSOC	There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable.
	SE_ERR_OOM	There was not enough memory to complete the operation.
	SE_ERR_PNF	The specified path was not found.
	SE_ERR_SHARE	A sharing violation occurred.
	*/

	int nError = (int) h;
	if (nError > 32) // no error
		return;
	DisplayLastError("");

}



// Display last Windows error message
// from msdev
void 
DisplayLastError(LPTSTR lpszFunction) 
{ 
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = ::GetLastError(); 

	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	lpDisplayBuf = LocalAlloc(LMEM_ZEROINIT, 
		strlen( (const char*) lpMsgBuf) + strlen( (const char*) lpszFunction) + 40); 
//	wsprintf((char*) lpDisplayBuf, 
//		"%s failed with error %d: %s", 
//		lpszFunction, dw, (char*) lpMsgBuf); 
//	::MessageBox(NULL, (char*) lpDisplayBuf, "Error", MB_OK);
	AfxMessageBox((LPCTSTR) lpMsgBuf, MB_ICONINFORMATION);

	::LocalFree(lpMsgBuf);
	::LocalFree(lpDisplayBuf);
//	ExitProcess(dw); 
}



/*
oops, see BrowseFolder in neomem.cpp

//*********************************************************************************
// Function name		- GetFolder
// Description			- Get a folder path
// Дата модификации		- 25.09.2000
// Кем модифицирована	- S. Sokolenko
// In					-
//						  strSelectedFolder - reference to string for store folder path
// Out				-
//						  lpszTitle - title for caption
//						  hwndOwner - reference to parent window 
//						  strRootFolder - root folder 
//						  strStartFolder - current foldet
// Return				- TRUE if user select OK, else FALSE.
//*********************************************************************************
CString strTmpPath;

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szDir[MAX_PATH];
	switch(uMsg){
	case BFFM_INITIALIZED:
		if (lpData){
			strcpy(szDir, strTmpPath.GetBuffer(strTmpPath.GetLength()));
			SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
		}
		break;
	case BFFM_SELCHANGED: {
	   if (SHGetPathFromIDList((LPITEMIDLIST) lParam ,szDir)){
		  SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
	   }
	   break;
	}
	default:
	   break;
	}
         
	return 0;
}

BOOL GetFolder(CString* strSelectedFolder,
				   const char* lpszTitle,
				   const HWND hwndOwner, 
				   const char* strRootFolder, 
				   const char* strStartFolder)
{
	char pszDisplayName[MAX_PATH];
	LPITEMIDLIST lpID;
	BROWSEINFOA bi;
	
	bi.hwndOwner = hwndOwner;
	if (strRootFolder == NULL){
		bi.pidlRoot = NULL;
	}else{
	   LPITEMIDLIST  pIdl = NULL;
	   IShellFolder* pDesktopFolder;
	   char          szPath[MAX_PATH];
	   OLECHAR       olePath[MAX_PATH];
	   ULONG         chEaten;
	   ULONG         dwAttributes;

	   strcpy(szPath, (LPCTSTR)strRootFolder);
	   if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	   {
		   MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH);
		   pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &chEaten, &pIdl, &dwAttributes);
		   pDesktopFolder->Release();
	   }
	   bi.pidlRoot = pIdl;
	}
	bi.pszDisplayName = pszDisplayName;
	bi.lpszTitle = lpszTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = BrowseCallbackProc;
	if (strStartFolder == NULL){
		bi.lParam = FALSE;
	}else{
		strTmpPath.Format("%s", strStartFolder);
		bi.lParam = TRUE;
	}
	bi.iImage = NULL;
	lpID = SHBrowseForFolderA(&bi);
	if (lpID != NULL){
		BOOL b = SHGetPathFromIDList(lpID, pszDisplayName);
		if (b == TRUE){
			strSelectedFolder->Format("%s",pszDisplayName);
			return TRUE;
		}
	}else{
		strSelectedFolder->Empty();
	}
	return FALSE;
}

*/



void 
pause()
{
//	Sleep(2000); // sleep for 2 seconds

//	DWORD dwTime = ::GetTickCount();
//	DWORD dwWait = 1000;
//	while (::GetTickCount() < dwTime + dwWait);
//	AfxMessageBox("continue");
//	for (ULONG lng = 0; lng < 100000; lng++);
}







// Throw an assertion (pass as an application exception)
// actually, why throw it, when you could just call HandleError directly!??
// oh, because throwing an exception will stop the program, but calling a routine
// will just return it to that place (since we're using WM_CLOSE to exit the app,
// which isn't appropriate sometimes...). So use ShowError for a recoverable error. 

// ** When an exception is thrown, execution of the current function is stopped and 
// jumps directly to the catch block of the innermost exception frame. The 
// exception mechanism bypasses the normal exit path from a function. Therefore, 
// you must be sure to delete those memory blocks that would be deleted in a 
// normal exit. 
void 
ThrowAssertion(LPCSTR lpszFilename, int nLine, 
				LPCSTR lpszExpression, LPCSTR lpszError/*=NULL*/)
{
//	CAppException* pe = new CAppException(TRUE); // pass true if creating on the heap!
	// Bug: can't just copy LPCTSTR pointer to string, because it might be a temporary object!!
	// The error message was getting overwritten with FE EE's and didn't know what was going on - 
	// a CString was being passed here and then going out of scope because of the throw! 
	// So use strcpy or CStrings!
//	pe->m_pszError = pszError; // store error message
//	pe->m_strError = "Assertion Failed";
//	throw pe;

//	CError e(_T("Assertion Failed"), TRUE);
//	e.SetFileLocation(lpszFilename, nLine, lpszExpression);
//	HandleError(e);

//	CError e(FALSE); // false because created on the stack
//	e.SetName(_T("Assertion Failed"));
//	e.SetSeverity(TRUE);
//	e.SetFileLocation(lpszFilename, nLine, lpszExpression);
//	throw &e;
	CError* pe = new CError(TRUE);
	if (lpszError==NULL)
		pe->SetName(_T("Assertion Failed"));
	else
		pe->SetName(lpszError);
	pe->SetSeverity(TRUE);
	pe->SetFileLocation(lpszFilename, nLine, lpszExpression);
	throw pe;
}






// Assertion functions used in release mode

/*
void ExitApp()
{
	// same as double-clicking on main window close box
//	ASSERT(AfxGetApp()->m_pMainWnd != NULL);
	AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
} 

void XAssertValid(const CObject* pobj)
{
	if (!pobj)
	{
		AfxMessageBox("ERROR! An assertion failed due to a null pointer. This is a test version and doesn't have rigorous error handling. To be safe you should just shut down your computer now. Otherwise, the program will try to close normally.", MB_ICONEXCLAMATION);
		ExitApp();
	}
}
*/


