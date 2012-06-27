
// Library.h
// General purpose routines
//-----------------------------------------------------------------------------------------------------------------


#pragma once



BOOL FileExists(LPCTSTR pszFileName);
CString GetFileName(LPCTSTR szPath);
CString fc(ULONGLONG n);
void GetColors();
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


extern COLORREF g_clrWindow;
extern COLORREF g_clrWindowText;
extern COLORREF g_clrHighlight;
extern COLORREF g_clrHighlightText;
extern COLORREF g_clrInactive;
extern COLORREF g_clrInactiveText;
extern COLORREF g_clr3dFace;
extern COLORREF g_clr3dHighlight;
extern COLORREF g_clr3dShadow;
extern COLORREF g_clrHyperlinkText;



void pause();
void ThrowAssertion(LPCSTR lpszFilename, int nLine, 
					LPCSTR lpszExpression, LPCSTR lpszError=NULL);

