// Filename.h: interface for the CFilename class.
//
//////////////////////////////////////////////////////////////////////

#pragma once



class CFilename : public CString  
{
public:
	CFilename(CString str);
	CFilename();
	virtual ~CFilename();

	void RemoveBadChars();

};

