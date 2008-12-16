// Filename.cpp: implementation of the CFilename class.
//
//////////////////////////////////////////////////////////////////////

// just add some extensions to cstring to handle filenames



#include "precompiled.h"
#include "neomem.h"
#include "Filename.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilename::CFilename()
{

}

CFilename::CFilename(CString str)
:CString(str) // call the superclass constructor
{
}

CFilename::~CFilename()
{

}


void CFilename::RemoveBadChars()
{
	// Check for illegal characters
	//, make lib func
	CString strBadChars = "\\/:*?\"<>|";
	int nBadChar = this->FindOneOf(strBadChars);
	if (nBadChar != -1)
	{
//		CString strMsg;
//		strMsg.Format("File names cannot contain the characters '%s' - occurrences of these characters will be removed.", (LPCTSTR) strBadChars);
//		AfxMessageBox(strMsg, MB_ICONINFORMATION);
		int nChars = strBadChars.GetLength();
		for (int i = 0; i < nChars; i++)
		{
			TCHAR ch = strBadChars.GetAt(i);
			this->Remove(ch);
		}
	}
}

