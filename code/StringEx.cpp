
// CStringEx


#include "precompiled.h"
#include "StringEx.h"

#include "library.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/*
CStringEx::CStringEx(const CString &str)
{
//	CString::CString(str);
//	*this = str;
	Format("%s", (LPCTSTR) str);
}

CStringEx::CStringEx(LPCTSTR lpsz)
{
//	CString::CString(lpsz);	
	Format("%s", lpsz);
}
*/


// Appends the text, inserting a ", " if the string already has characters in it
void CStringEx::AppendComma(LPCTSTR lpsz)
{
	if (GetLength() > 0)
		this->operator +=( _T(", "));
	this->operator +=(lpsz);
}





LPCTSTR CStringEx::GetNextToken(LPCTSTR pszDelimiter)
{
	return 0;
}


// Search through this string for tokens obtained from the string table whose resourceID's are specified.
// If a token was found, returns the resourceID of the matching token.
// If bSwallowToken True will also remove the token and any trailing spaces from this string.
// If no token matches, returns 0.
// Just used by bdatadate
//xUINT CStringEx::GetNextTokenID(CStringArray& sa, UINT nStartID, UINT nStopID, BOOL bSwallowToken)
UINT CStringEx::GetNextTokenID(char* sa[], UINT nStartID, UINT nStopID, BOOL bSwallowToken)
{
	TCHAR pszDelimiters[] = _T(",");

	UINT nFoundID = 0;
	for (UINT nID = nStartID; (nFoundID == 0) && (nID <= nStopID); nID++)
	{
//		CString strTokens;
//		strTokens.LoadString(nID);
		CString strTokens = sa[nID];
		LPTSTR pszTokens = strTokens.GetBuffer(0);
		char* pos = 0;

		// Skip first token, as it's used for output, not input
//		TCHAR* pszToken = _tcstok(pszTokens, pszDelimiters); 
		TCHAR* pszToken = _tcstok_s(pszTokens, pszDelimiters, &pos); 
		ASSERT(pszToken); // there should always be at least one token

		// Get next token
		pszToken = _tcstok_s(NULL, pszDelimiters, &pos);
		while ((pszToken != 0) && (nFoundID == 0))
		{
			// Check if string matches the token
			int nTokenLength = _tcslen(pszToken);
			if (_tcsncmp(pszToken, (LPCTSTR) *this, nTokenLength) == 0)
			{
				// Found a token!
				nFoundID = nID;
				// Remove token and following whitespace from this string if specified
				if (bSwallowToken)
				{
					*this = Mid(nTokenLength);
					TrimLeft();
				}
			}
			else
			{
				// Get next token
				pszToken = _tcstok_s(NULL, pszDelimiters, &pos);
			}
		}
		strTokens.ReleaseBuffer();
	}
	// Return token resourceID, if found.
	return nFoundID;
}




// Get the checksum of this string (add up all the bytes).
// Used in narrowing down the string search to the best match (a pretty cheesy way of doing it)
int CStringEx::GetCheckSum()
{
	int nCheckSum = 0;
	int nLength = GetLength();
	for (int i = 0; i < nLength; i++)
	{
		TCHAR c = GetAt(i);
		nCheckSum += (int) c;
	}
	return nCheckSum;
}




// Verify that the string contains only standard ASCII characters.
// Used to make sure passwords are good. 
BOOL CStringEx::IsAscii()
{
#ifdef _UNICODE
	// If wcstombs successfully converts the multibyte string, it returns the number of bytes written 
	// into the multibyte output string, excluding the terminating NULL (if any). If the mbstr argument 
	// is NULL, wcstombs returns the required size of the destination string. If wcstombs encounters 
	// a wide character it cannot be convert to a multibyte character, it returns –1 cast to type size_t.
//,	UINT nSize = (UINT) ::wcstombs(NULL, , 0);
//	if (nSize == -1)
	ASSERT(FALSE); // not implemented yet
	return FALSE; //, for now
#else
	// MBCS case
	int iLen = GetLength(); // ncharacters
	for (int i = 0; i < iLen; i++)
	{
		TCHAR c = GetAt(i);
		if (c > 0x7f) // don't allow any weird european characters
			return FALSE;
	}
	return TRUE;
#endif
}






// from codeproject
/*
CStringEx::CStringEx(int i, LPCTSTR szFormat, DWORD dwOptions)
{
	char s[64];							// Eeek, a hard coded limit on integer sizes!
	sprintf(s, "%d", i);
	if (dwOptions & COMMA_DELIMIT)
	{
		CString::operator=(CommaDelimitNumber(s));
	}
	else
	{
		CString::operator=(s);
	}
}


CStringEx::CStringEx(double d, LPCTSTR szFormat, DWORD dwOptions)
{
	char s[64];							// Eeek, a hard coded limit on double sizes!
	sprintf(s, szFormat, d);
	if (dwOptions & COMMA_DELIMIT)
	{
		CString::operator=(CommaDelimitNumber(s));
	}
	else
	{
		CString::operator=(s);
	}
}



// extracts string to the left of c, or entire string if c does not exist in str
CStringEx CStringEx::Left(TCHAR c, int q) const
{
	int n=0;
	while (q)
	{
		n = Find(c, n);
		if (n != -1)
		{
			++n;
		}
		else
		{
			return *this;
		}
		--q;
	}

	return CString::Left(n-1);
}




// return the string to the right of the rightmost instance of c, or an empty string if c does not exist in str
CStringEx CStringEx::Rightmost(TCHAR c) const
{
	int n=ReverseFind(c);
	if (n == -1)
	{
		return "";
	}
	return CString::Right(GetLength()-n-1);
}


// return the string to the left of the rightmost instance of c, or an empty string if c does not exist in str
CStringEx CStringEx::LeftOfRightmost(TCHAR c) const
{
	int n=ReverseFind(c);
	if (n == -1)
	{
		return "";
	}
	return CString::Left(n);
}


CStringEx CStringEx::SubStr(int begin, int len) const
{
	return CString::Mid(begin, len);
}



CStringEx CStringEx::CommaDelimitNumber(LPCTSTR s)
{
	CStringEx s2=s;										// convert to CStringEx
	CStringEx q=s2.Left('.');								// get everything to the left of the first decimal point
	char posNegChar=q[0];									// get the first char
	bool posNeg=!isdigit(posNegChar);							// if not digit, then assume + or -
	if (posNeg)											// if so, strip off
	{
		q=q.SubStr(1, strlen(q)-1);
	}
	CStringEx dp=s2.Right('.');								// remember everything to the right of the decimal point
	CStringEx q2;											// working string
	while (strlen(q) > 3)									// if more than three digits...
	{
		CStringEx s3=CStringEx(",")+q.SubStr(strlen(q)-3, 3);		// insert a comma before the last three digits (100's)
		q2=s3+q2;											// append this to our working string
		q=q.SubStr(0, strlen(q)-3);							// get everything except the last three digits
	}
	q2=q+q2;												// prepend remainder to the working string
	if (dp != "")											// if we have decimal point...
	{
		q2=q2+"."+dp;										// append it and the digits
	}
	if (posNeg)											// if we stripped off a +/- ...
	{
		q2=CStringEx(posNegChar)+q2;							// add it back in
	}
	return q2;											// this is our final comma delimited string
}

*/



// Extracts string to the right of character c, or returns empty string if c does not exist in string.
CStringEx CStringEx::RightFrom(TCHAR c, int nOccurrence) const
{
	int nPos = 0;
	while (nOccurrence)
	{
		nPos = Find(c, nPos);
		if (nPos == -1)
			return "";
		--nOccurrence;
		++nPos;
	}
	return CString::Right(GetLength() - nPos);
}


// Extracts string to the left of character c, or returns empty string if c does not exist in string.
CStringEx CStringEx::LeftFrom(TCHAR c, int nOccurrence) const
{
	ASSERT(nOccurrence==1); // for now... 
	int nPos = 0;
	while (nOccurrence)
	{
		nPos = ReverseFind(c);
		if (nPos == -1)
			return "";
		--nOccurrence;
//		++nPos;
	}
	return CString::Left(nPos);
}



// Format the given integer into a string with commas separating the thousands places
//, call this FormatEx and Replace %t's with thousands formatted string - handle variable params, etc.
// see library function fc
void CStringEx::FormatThousands(ULONG n)
{
	// Convert number to a plain string, eg "1313", then call windows routine to add commas
	// (or whatever, depending on international settings)
	const int nChars = 50;
	TCHAR szPlain[nChars];
	wsprintf(szPlain, "%d", n);
	LPTSTR szFormatted = GetBuffer(nChars);
//	::GetNumberFormat(LOCALE_USER_DEFAULT, 0, szPlain, NULL, szFormatted, nChars); 	// this always prints 2 decimal places!
	::GetNumberFormat(0, 0, szPlain, &g_nf, szFormatted, nChars);
	ReleaseBuffer();
}



// currently rounds up or down to nearest 1024 bytes
void CStringEx::FormatBytes(ULONG nBytes)
{
	CStringEx strNum;

	//, make this optional
//	LPCTSTR szUnits = " bytes"; // default
//	if (nBytes >= 1024)
//	{
//		nBytes /= 1024;
//		szUnits = " KB";
//	}
//	if (nBytes >= 1024)
//	{
//		nBytes /= 1024;
//		szUnits = " MB";
//	}
	nBytes = (nBytes + 512) / 1024;
	LPCTSTR szUnits = "KB";
	strNum.FormatThousands(nBytes);
	this->Format("%s%s", (LPCTSTR) strNum, szUnits);
}






// allow only a-z and underscore. 
// used for propname to xml element name
void CStringEx::RemoveBadChars()
{
	CString strNew;
//	LPTSTR sz = strNew.GetBuffer(nChars);
	// walk through string, check if allowed. if not, skip this char.
	int nChars = this->GetLength();
	for (int i = 0; i < nChars; i++)
	{
		TCHAR ch = this->GetAt(i);
		if ((ch >='a' && ch <='z') || (ch >='A' && ch<='Z') || (ch=='_'))
			strNew += ch;
		else if (ch==' ')
			strNew += '_';
	}

//	strNew.ReleaseBuffer();
	this->operator = (strNew);

}



