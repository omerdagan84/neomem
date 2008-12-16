



#pragma once

class CFileFormat;



enum eFileFormat {ffUnknown=0, ffCsv, ffTab, ffRtf, ffText, ffXml, ffHtml, ffOpml};



class CFileFormats : public CObArray  
{
public:
	CFileFormats();
	virtual ~CFileFormats();

// Operations
public:
	eFileFormat GetFormatId(int nIndex);
	CString GetFilter();
	CFileFormat* GetAt(int i);

};


