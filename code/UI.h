
// CUI
// Defines a generic User Interface.
// The Gui and ConsoleUI would need to implement these functions. 

//x
// this is part of the database. 
// it defines the callback interface. 
// the app/ui would need to implement this in an object,
// to be passed to the various db routines which might
// need user feedback. 
// and could also make a testUi, for mock tests
// the gui ui would bring up dialogs. 
// the console ui would print and input, etc.


// the app will interact with this class
// would become a virtual class after developing it, or when need another ui
// (developing as a virtual class is too slow)

#pragma once

#include "Brooklyn.h"



class CUI {

public:
	CUI();
	virtual ~CUI();



public: 
	BOOL BrowseFolder(LPCTSTR pszInstructions, CString& strFolder);
//	virtual bool GetFileName(LPCTSTR strTitle, LPCTSTR strExtension, LPCTSTR strFilter, CString& strFilename) = 0;
//	virtual bool GetString(LPCTSTR strTitle, LPCTSTR strInstructions, CString& strString) = 0;
	BOOL EditString(CString& str);
	BOOL GetDate(COleDateTime& odt, BOOL& bUseTime);
	int MessageBox(CString& str, UINT nType = 0);
	BOOL EditLink(CString strCaption, CString strInstructions, BOOL bMultiSelectVisible, 
								 BOOL bMultiSelectEnabled, BOOL bMultiSelectOn, ULONG lngStartID, 
								 BDataLink* pdatLink, ULONG lngExcludeFlags = 0, 
								 BOOL bIncludeStart = FALSE);
	BOOL EditNumber();
	BOOL EditName(CString& strTitle, CString& strFirst, CString& strMiddle, CString& strNickname, CString& strLast, CString& strSuffix);


//x	BOOL EditValue(BObject* pobj, OBJID idProperty);
/*
private:
	BOOL EditDate(BObject* pobj, OBJID idProperty);
	BOOL EditEmail(BObject* pobj, OBJID idProperty);
	BOOL EditFile(BObject* pobj, OBJID idProperty);
	BOOL EditFolder(BObject* pobj, OBJID idProperty);
	BOOL EditString(BObject* pobj, OBJID idProperty);
*/


};


