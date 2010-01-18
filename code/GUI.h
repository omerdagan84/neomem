
// CGUI
// Implements the database's UI callback interface for a GUI

// ie we'd implement those functions by bringing up dialogs to 
// get info from the user, etc. 

#pragma once


#include "UI.h"


class CGUI : public CUI {

public:

	CGUI();
	virtual ~CGUI();

	bool GetFileName(LPCTSTR strTitle, LPCTSTR strExtension, LPCTSTR strFilter, CString& strFilename);
	bool GetString(LPCTSTR strTitle, LPCTSTR strInstructions, CString& strString);

};


