
// Library.h
// General purpose routines
//-----------------------------------------------------------------------------------------------------------------


#pragma once


class Library {

public:
	static BOOL FileExists(LPCTSTR pszFileName);
	static CString GetFileName(LPCTSTR szPath);
	static CString fc(ULONGLONG n);
	static void LoadColors();
	static BOOL IsEdit(CWnd* pWnd); 
	static CString GetErrorMessage(DWORD dwError);
	static void HandleShellExecuteError(HINSTANCE h);
	static void DisplayLastError(LPTSTR lpszFunction);
	static void Char2Hex(const unsigned char ch, char* szHex);
	static void Hex2Char(const char* szHex, unsigned char& rch);
	static void CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iBufsize, int iSize);
	static void HexStr2CharStr(const char* pszHexStr, unsigned char* pucCharStr, int iSize);
	static void ThrowAssertion(LPCSTR lpszFilename, int nLine, LPCSTR lpszExpression, LPCSTR lpszError=NULL);

public:
	static COLORREF clrWindow;
	static COLORREF clrWindowText;
	static COLORREF clrHighlight;
	static COLORREF clrHighlightText;
	static COLORREF clrInactive;
	static COLORREF clrInactiveText;
	static COLORREF clr3dFace;
	static COLORREF clr3dHighlight;
	static COLORREF clr3dShadow;
	static COLORREF clrHyperlinkText;

};


void TRACESTRING(LPCTSTR szFormat, LPCTSTR szString);
void TRACETOFILE(LPCTSTR szFilename, LPCTSTR szString);


// Get number of elements in an array - taken from Afxisapi.h
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif


extern NUMBERFMT g_nf;



// macros from macros.h, which was merged here. it was included in precompiled.h


// In release mode, we want asserts to at least raise an exception if they are false.
// Note: The brackets are necessary because if you had an assert inside an if statement 
// that was followed by an else statement, the logic could get messed up.
// Note: The # is the stringizing operator, which returns the parameter in double quotes
//.#ifndef _DEBUG
#undef ASSERT
#undef ASSERT_VALID
#define ASSERT(f) if (!(f)) { Library::ThrowAssertion(THIS_FILE, __LINE__, #f); }
#define ASSERT_VALID(p) if (!(p)) { Library::ThrowAssertion(THIS_FILE, __LINE__, #p); }
//#endif

//. should be doing this somehow...
//#define ASSERT_VALID(pOb)  (::AfxAssertValidObject(pOb, THIS_FILE, __LINE__))


// Turn on debug logfile
//#define _DEBUG_LOG  



// aliases
#define VK_PAGE_DOWN VK_NEXT
#define VK_PAGE_UP VK_PRIOR


// This TRACEDUMP macro saves having to enclose .Dump calls in #ifdef _DEBUGs
// ex. just say TRACEDUMP(col);
#ifndef TRACEDUMP
#ifdef _DEBUG
#define TRACEDUMP(a)   a.Dump(afxDump)
#else
#define TRACEDUMP(a)   a;
#endif // _DEBUG
#endif // TRACEDUMP


// Note: TRACE calls AfxTrace which says afxDump << szBuffer;

// To turn off trace statement just put an x in front of it - makes it easier to track down trace's also.
// This is how MFC turns off TRACE for the release compilation - 
#define xTRACE     1 ? (void)0 : ::AfxTrace

// This makes it easier to track down which trace statements are turned on -
// ie if you use yTRACE instead of TRACE, then you can search on yTRACE to find them all.
#define yTRACE TRACE







