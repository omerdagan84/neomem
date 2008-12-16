

// FileText.h
// just extends CStdioFile
// used for export to csv, tab delim, xml, etc




//#include "NeoMem.h"
#include "FileFormats.h"

//#include "BDataLink.h"
class BDataLink;



#pragma once

class CFileText : public CStdioFile  
{
// Construction
public:
	CFileText();
	virtual ~CFileText();
//	enum eFormats {formatUnknown=0, formatCsv, formatTab, formatRtf};

// Operations
public:
	void SetFormat(eFileFormat nFormat);
	void WriteDelim();
	void WriteHeader(BDataLink& datProps);
	void WriteNewline();
	void WriteValue(LPCTSTR psz);

// Attributes
public:
	void WriteLine(LPCTSTR psz);
	void WriteFooter();
	CString m_strDelim;
	CString m_strNewline;
	eFileFormat m_nFormat;
};

