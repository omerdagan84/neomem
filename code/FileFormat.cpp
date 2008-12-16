

#include "precompiled.h"

#include "FileFormat.h"
#include "FileFormats.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CFileFormat::CFileFormat()
{
}

CFileFormat::~CFileFormat()
{
}

void CFileFormat::Init(eFileFormat id, CString name, CString description, CString extension)
{
	m_id = id;
	m_strName = name;
	m_strDescription = description;
	m_strExtension = extension;
}



// eg "Comma Separated Values (*.csv)|*.csv|"

CString CFileFormat::GetFilter()
{
	CString str;
	str.Format("%s Files (*.%s)|*.%s|", 
		(LPCTSTR) m_strName,
		(LPCTSTR) m_strExtension,
		(LPCTSTR) m_strExtension
		);
	return str;
}


