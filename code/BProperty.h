
#pragma once

#include "BObject.h"




class BProperty : public BObject {

public:
	BProperty(BDoc& doc, LPCTSTR pszName = 0, LPCTSTR pszDescription = 0, OBJID idPropertyType = 0, OBJID idParent = 0, OBJID idIcon = 0, ULONG lngFlags = 0);
	static BProperty& New(BDoc& doc, LPCTSTR pszName = NULL, LPCTSTR pszDescription = 0, OBJID idParent = 0, OBJID idIcon = 0, ULONG lngFlags = 0);

};

