


#include "precompiled.h"
#include "FileFormats.h"
#include "FileFormat.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CFileFormats::CFileFormats()
{
}

CFileFormats::~CFileFormats()
{
}


CFileFormat* CFileFormats::GetAt(int i)
{
	return DYNAMIC_DOWNCAST(CFileFormat, CObArray::GetAt(i));
}


// eg "Rich Text Format Files (*.rtf)|*.rtf|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"
CString CFileFormats::GetFilter()
{
	CString str;
	int nFormats = this->GetSize();
	for (int i=0; i < nFormats; i++)
	{
		CFileFormat* ff = this->GetAt(i);
		if (ff)
		{
			str += ff->GetFilter();
		}
	}
//	str += "|";
	str += "All Files (*.*)|*.*||";
	return str;
}


// get the id for the specified format (zero-based index in the array).
// the id will match ffCsv, ffText, etc. 
// if out of bounds, will return ffUnknown (0)
eFileFormat CFileFormats::GetFormatId(int nIndex)
{
	int nCount = this->GetSize();
	if (nIndex >= nCount)
		return ffUnknown;
	CFileFormat* ff = this->GetAt(nIndex);
	if (ff)
		return ff->m_id;
	else
		return ffUnknown;
}


