
// Library.h
// General purpose routines
//-----------------------------------------------------------------------------------------------------------------


#pragma once


class Library {
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


public:
	static BOOL FileExists(LPCTSTR pszFileName);
	static CString GetFileName(LPCTSTR szPath);
	static CString fc(ULONGLONG n);
	static void LoadColors();
};

BOOL IsEdit(CWnd* pWnd); 

void TRACESTRING(LPCTSTR szFormat, LPCTSTR szString);
void TRACETOFILE(LPCTSTR szFilename, LPCTSTR szString);
CString GetErrorMessage(DWORD dwError);
void HandleShellExecuteError(HINSTANCE h);
void DisplayLastError(LPTSTR lpszFunction);


void Char2Hex(const unsigned char ch, char* szHex);
void Hex2Char(const char* szHex, unsigned char& rch);
void CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iBufsize, int iSize);
void HexStr2CharStr(const char* pszHexStr, unsigned char* pucCharStr, int iSize);



// Get number of elements in an array - taken from Afxisapi.h
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif


extern NUMBERFMT g_nf;




void pause();
void ThrowAssertion(LPCSTR lpszFilename, int nLine, 
					LPCSTR lpszExpression, LPCSTR lpszError=NULL);

