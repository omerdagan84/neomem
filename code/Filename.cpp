


#include "precompiled.h"
#include "neomem.h"
#include "Filename.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CFilename::CFilename() {
}

CFilename::CFilename(CString str)
:CString(str) // call the superclass constructor 
{
}

CFilename::~CFilename() {
}


void CFilename::RemoveBadChars() {

	// Check for illegal characters
	CString strBadChars = "\\/:*?\"<>|"; //, _T
	int nBadChar = this->FindOneOf(strBadChars);
	if (nBadChar != -1) {
		int nChars = strBadChars.GetLength();
		for (int i = 0; i < nChars; i++) {
			TCHAR ch = strBadChars.GetAt(i);
			this->Remove(ch);
		}
	}
}


/*
void CFilename::RemoveExtension() {

	int nPos = this->ReverseFind('.');
	if (nPos != -1)
//		strModifiedName = strOriginalName.Left(nPos) + szAppendText + theApp.m_strFileExtension;
		this->Delete(nPos, -1);
		this->
}
*/

