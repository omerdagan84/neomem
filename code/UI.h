
// CUI
// Defines a generic User Interface.
// The Gui and ConsoleUI would need to implement these functions. 

// this is part of the database. 
// it defines the callback interface. 
// the app/ui would need to implement this in an object,
// to be passed to the various db routines which might
// need user feedback. 

// and could also make a testUi, for mock tests!

// the gui ui would bring up dialogs. 
// the console ui would print and input, etc.



#pragma once

class CUI {

public:

	CUI();
	virtual ~CUI();


	virtual bool GetFileName(LPCTSTR strTitle, LPCTSTR strExtension, LPCTSTR strFilter, CString& strFilename) = 0;
	virtual bool GetString(LPCTSTR strTitle, LPCTSTR strInstructions, CString& strString) = 0;
	

};


