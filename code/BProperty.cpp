

#include "Precompiled.h"


#include "BProperty.h"
#include "ConstantsDatabase.h"



BProperty::BProperty(BDoc& doc, LPCTSTR pszName, LPCTSTR pszDescription, OBJID idProptype, OBJID idParent, OBJID idIcon, ULONG lngFlags) : 
	BObject(doc, classProperty, pszName, folderProperties, idIcon, lngFlags) // put overrides here
{
	if (pszDescription)
		SetPropertyString(propDescription, pszDescription);
	ASSERT(idProptype);
	SetPropertyLink(propPropertyType, idProptype);
}


// static
BProperty& BProperty::New(BDoc& doc, LPCTSTR pszName, LPCTSTR pszDescription, OBJID idParent, OBJID idIcon, ULONG lngFlags) {
	BProperty* p = new BProperty(doc, pszName, pszDescription, idParent, idIcon, lngFlags);
	ASSERT_VALID(p);
	return *p;
}



