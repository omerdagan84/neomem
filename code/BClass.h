
#pragma once

#include "BObject.h"




class BClass : public BObject {

public:
	BClass(BDoc& doc, LPCTSTR pszName = 0, LPCTSTR pszDescription = 0, OBJID idParent = 0, OBJID idIcon = 0, ULONG lngFlags = 0);
	static BClass& New(BDoc& doc, LPCTSTR pszName = NULL, LPCTSTR pszDescription = 0, OBJID idParent = 0, OBJID idIcon = 0, ULONG lngFlags = 0);


};
