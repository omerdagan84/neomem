

#pragma once

#include "BObject.h"


class BDataColumns;


class BFolder : public BObject {

public:
	BFolder(BDoc& doc, LPCTSTR pszName = 0, OBJID idParent = 0, OBJID idDefaultClass = 0, OBJID idIcon = 0, ULONG lngFlags = 0);
	static BFolder& New(BDoc& doc, LPCTSTR pszName = NULL, OBJID idParent = 0, OBJID idDefaultClass = 0, OBJID idIcon = 0, ULONG lngFlags = 0);

};
