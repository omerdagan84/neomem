
// CFilename
// just add some extensions to cstring to handle filenames

//, oh, merge with CPath though


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

