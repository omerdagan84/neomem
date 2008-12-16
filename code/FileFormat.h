


#pragma once


#include "fileformats.h" // for eFileFormat type


class CFileFormat : public CObject  
{
// Construction
public:
	CString GetFilter();
	CFileFormat();
	virtual ~CFileFormat();

// Operations
	void Init(eFileFormat id, CString name, CString description, CString extension);

// Attributes
	eFileFormat m_id;
	CString m_strExtension;
	CString m_strName; //  eg "csv"
	CString m_strDescription;
};

