

#pragma once

#include "BObject.h"


class BDataColumns;


class BFolder : public BObject {

public:
	BFolder(BDoc& doc, LPCTSTR pszName = 0, OBJID idParent = 0, OBJID idDefaultClass = 0, OBJID idIcon = 0, ULONG lngFlags = 0);
	static BFolder& New(BDoc& doc, LPCTSTR pszName = NULL, OBJID idParent = 0, OBJID idDefaultClass = 0, OBJID idIcon = 0, ULONG lngFlags = 0);


	void AddColumn(OBJID idProperty, int nWidth = 0, int iPos = -1);
	void RemoveColumn(OBJID idProperty);

};
