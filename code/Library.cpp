

// Library.cpp
// General purpose routines

//, move to static class routines


#include "precompiled.h"
//#include "Library.h"
#include "Error.h"





NUMBERFMT g_nf;


// Returns True if the given file exists.
// static
BOOL Library::FileExists(LPCTSTR pszFileName)
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
// Code from MFC routine.
// static
CString Library::GetFileName(LPCTSTR pszPathName)
{
  
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
// To use in a printf, be sure to cast the result to LPCTSTR (const char*)
//. use FormatThousands in CStringEx instead
//. give better name so can search on it
//. bug: won't print llu anymore - worked under vs6. so if # > 2^32, will get truncated.
// static
CString Library::fc(ULONGLONG n)
{
	const int nChars = 50;
	TCHAR szPlain[nChars];
	TCHAR szFormatted[nChars];

	wsprintf(szPlain, "%lu", n); // long long unsigned
	//TRACE("szplain: %s\n", szPlain);
	// this always prints 2 decimal places
//	::GetNumberFormat(LOCALE_USER_DEFAULT, 0, szPlain, NULL, szFormatted, nChars);
	
	// g_nf specifies the number format to use
	::GetNumberFormat(0, 0, szPlain, &g_nf, szFormatted, nChars);
	return szFormatted;
}



// Get global colors
// static
void Library::LoadColors()
{
	// set static members
	clrWindow = ::GetSysColor(COLOR_WINDOW);
	clrWindowText = ::GetSysColor(COLOR_WINDOWTEXT);
	clrHighlight = ::GetSysColor(COLOR_HIGHLIGHT);
	clrHighlightText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	clrInactive = ::GetSysColor(COLOR_SCROLLBAR); //, use a different color for this
	clrInactiveText = ::GetSysColor(COLOR_WINDOWTEXT);
	clr3dFace = ::GetSysColor(COLOR_3DFACE);
	clr3dHighlight = ::GetSysColor(COLOR_3DHILIGHT);
	clr3dShadow = ::GetSysColor(COLOR_3DSHADOW);
	clrHyperlinkText = 0x0020a020; // green
}


// this seems weird, but you have to define the static variables here,
// in addition to declaring them in the class definition. 
// without this you get link errors.
// see: http://msdn.microsoft.com/en-us/library/b1b5y48f.aspx
COLORREF Library::clrWindow;
COLORREF Library::clrWindowText;
COLORREF Library::clrHighlight;
COLORREF Library::clrHighlightText;
COLORREF Library::clrInactive;
COLORREF Library::clrInactiveText;
COLORREF Library::clr3dFace;
COLORREF Library::clr3dHighlight;
COLORREF Library::clr3dShadow;
COLORREF Library::clrHyperlinkText;




// IsEdit: a helper function to determine if a given CWnd pointer points to a CEdit control.
// Use the SDK ::GetClassName() function because MFC IsKindOf fails if no CEdit variable 
// has been created for the control you're trying to test.
// This is used by cut copy paste code - see Q145616 in msdn 
// static
BOOL Library::IsEdit(CWnd *pWnd)
{
	ASSERT(pWnd);
	HWND hWnd = pWnd->GetSafeHwnd();
	if (hWnd == NULL)
		return FALSE;

	TCHAR szClassName[6];
	return ::GetClassName(hWnd, szClassName, 6) && _tcsicmp(szClassName, _T("Edit")) == 0;
}





//, there's nothing in the libraries that will do this stuff? 


// Function to convert unsigned char (byte) to string of length 2, eg 0xAE -> "AE"
// static
void Library::Char2Hex(const unsigned char ch, char* szHex)
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
// static
void Library::Hex2Char(const char* szHex, unsigned char& rch)
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
// static
void Library::CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iBufsize, int iSize)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for(i=0; i<iSize; i++)
	{
		Char2Hex(pucCharStr[i], szHex);
		strcat_s(pszHexStr, iBufsize, szHex);
	}
}

// Function to convert string of chars to string of unsigned chars, eg "9E3D" -> 0x9E3D
// static
void Library::HexStr2CharStr(const char* pszHexStr, unsigned char* pucCharStr, int iSize)
{
	int i;
	unsigned char ch;
	for(i=0; i<iSize; i++)
	{
		Hex2Char(pszHexStr+2*i, ch);
		pucCharStr[i] = ch;
	}
}




// Get Windows error message - pass 0 to get last error
// static
CString Library::GetErrorMessage(DWORD dwError)
{
	if (dwError == 0) 
		dwError = ::GetLastError();

	CString s;
	const int nChars = 1024;
	LPTSTR lpBuffer = s.GetBuffer(nChars);

	::FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			lpBuffer,
			nChars,
			NULL 
			);
	
	s.ReleaseBuffer();

	return s;
}


// static
void Library::HandleShellExecuteError(HINSTANCE h)
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
// static
void Library::DisplayLastError(LPTSTR lpszFunction) 
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
	AfxMessageBox((LPCTSTR) lpMsgBuf, MB_ICONINFORMATION);

	::LocalFree(lpMsgBuf);
	::LocalFree(lpDisplayBuf);
//	ExitProcess(dw); 
}






// Throw an assertion (pass as an application exception)
// actually, why throw it, when you could just call HandleError directly?
// because throwing an exception will stop the program, but calling a routine
// will just return it to that place (since we're using WM_CLOSE to exit the app,
// which isn't appropriate sometimes...). So use ShowError for a recoverable error. 

// ** When an exception is thrown, execution of the current function is stopped and 
// jumps directly to the catch block of the innermost exception frame. The 
// exception mechanism bypasses the normal exit path from a function. Therefore, 
// you must be sure to delete those memory blocks that would be deleted in a 
// normal exit. 
// static
void Library::ThrowAssertion(LPCSTR lpszFilename, int nLine, 
				LPCSTR lpszExpression, LPCSTR lpszError/*=NULL*/)
{
//	CAppException* pe = new CAppException(TRUE); // pass true if creating on the heap
	// Bug: can't just copy LPCTSTR pointer to string, because it might be a temporary object.
	// The error message was getting overwritten with FE EE's and didn't know what was going on - 
	// a CString was being passed here and then going out of scope because of the throw - 
	// so use strcpy or CStrings
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



//------------------------

// not used very much


// Use this to print debug strings that are longer than 512 characters.
// TRACE is limited to 512 chars!
// none of these work either:
//	afxDump << (LPCTSTR) strNewText;
//	afxDump.OutputString(strNewText);
//	AfxOutputDebugString(strNewText);
void TRACESTRING(LPCTSTR szFormat, LPCTSTR szString)
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
void TRACETOFILE(LPCTSTR szFilename, LPCTSTR szString)
{
	TRACE("Writing string to file %s...", szFilename);
	CStdioFile f(szFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	f.WriteString(szString);
	f.Close();
	TRACE(" done.\n");
}


