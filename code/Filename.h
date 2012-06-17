
// CFilename
//, just add some extensions to cstring to handle filenames
//, merge with CPath


#pragma once


class CFilename : public CString  
{
public:
	CFilename(CString str);
	CFilename();
	virtual ~CFilename();

	void RemoveBadChars();
//	void RemoveExtension();

};

