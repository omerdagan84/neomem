
// CStringEx
// Extension of the CString class - provides parsing etc.
//-----------------------------------------------------------------------------------------------------------------


#pragma once



class CStringEx : public CString
{
// enums
public:
	enum Options {NO_OPTIONS=0, COMMA_DELIMIT=1};

// Operations
public:
	void RemoveBadChars();
	// from codeproject - this is cool - hadn't been able to figure out how to handle the constructor...
	CStringEx(void) {};
	CStringEx(const CString& s) : CString(s) {}
//	CStringEx(LPCTSTR s) : CString(s) {}
	virtual ~CStringEx() {};

//	CStringEx(int i, LPCTSTR szFormat="%d", DWORD dwOptions=NO_OPTIONS);
//	CStringEx(double d, LPCTSTR szFormat="%02lf", DWORD dwOptions=NO_OPTIONS);

	CStringEx LeftFrom(TCHAR c, int nOccurrence=1) const; // everything to the left of the nth occurance of <c>
	CStringEx RightFrom(TCHAR c, int nOccurrence=1) const; // everything to the right of the nth occurance of <c>
//	CStringEx Rightmost(TCHAR c) const;							// everything to the right of the last occurance of <c>
//	CStringEx LeftOfRightmost(TCHAR c) const;					// everything to the left of the last occurance of <c>
//	CStringEx SubStr(int begin, int len) const;					// substring from s[begin] to s[begin+len]
	void Trim(void) {TrimLeft(); TrimRight();};					// trims both left and right sides
	CStringEx& operator=(const CString& s) {CString::operator=(s); return *this;}
//	CStringEx CommaDelimitNumber(LPCTSTR s);


public: 
	BOOL IsAscii();
	int GetCheckSum();
//x	UINT GetNextTokenID(UINT nStartID, UINT nStopID, BOOL bSwallowToken);
//x	UINT GetNextTokenID(CStringArray& sa, UINT nStartID, UINT nStopID, BOOL bSwallowToken);
	UINT GetNextTokenID(char* sa[], UINT nStartID, UINT nStopID, BOOL bSwallowToken);
	LPCTSTR GetNextToken(LPCTSTR pszDelimiter);
	void AppendComma(LPCTSTR lpsz);
	void FormatBytes(ULONG nBytes);
	void FormatThousands(ULONG n);

};


