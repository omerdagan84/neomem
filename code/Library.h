
// Library.h
// General purpose routines
//-----------------------------------------------------------------------------------------------------------------


#pragma once



BOOL FileExists(LPCTSTR pszFileName);
CString GetFileName(LPCTSTR szPath);
CString fc(long n);
void GetColors();
BOOL IsEdit(CWnd* pWnd); 

void TRACESTRING(LPCTSTR szFormat, LPCTSTR szString);
void TRACETOFILE(LPCTSTR szFilename, LPCTSTR szString);
CString GetErrorMessage(DWORD dwError);
void HandleShellExecuteError(HINSTANCE h);
void DisplayLastError(LPTSTR lpszFunction);
//BOOL GetFolder(CString* strSelectedFolder,const char* lpszTitle,const HWND hwndOwner, const char* strRootFolder, const char* strStartFolder);


// Convert unsigned char to string of length 2, eg 0xAE -> "AE"
void Char2Hex(const unsigned char ch, char* szHex);
// Convert string of length 2 to unsigned char, eg "B9" -> 0xB9
void Hex2Char(const char* szHex, unsigned char& rch);
// Convert string of unsigned chars to string of chars, eg 0x9E3D -> "9E3D"
void CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iSize);
// Convert string of chars to string of unsigned chars, eg "9E3D" -> 0x9E3D
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



/*
// Convert the given character to lowercase
inline char lowercase(char a)
{
	return ((a >= 'A' && a <= 'Z') ? a-'A'+'a' : a );
}
*/

void pause();
void ThrowAssertion(LPCSTR lpszFilename, int nLine, 
					LPCSTR lpszExpression, LPCSTR lpszError=NULL);

