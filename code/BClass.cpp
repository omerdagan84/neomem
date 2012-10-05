

#include "Precompiled.h"


#include "BClass.h"
#include "ConstantsDatabase.h"



BClass::BClass(BDoc& doc, LPCTSTR pszName, LPCTSTR pszDescription, OBJID idParent, OBJID idIcon, ULONG lngFlags) : 
	BObject(doc, classClass, pszName, folderClasses, idIcon, lngFlags) // put overrides here
{
	if (pszDescription)
		SetPropertyString(propDescription, pszDescription);
}


// static
BClass& BClass::New(BDoc& doc, LPCTSTR pszName, LPCTSTR pszDescription, OBJID idParent, OBJID idIcon, ULONG lngFlags) {
	BClass* p = new BClass(doc, pszName, pszDescription, idParent, idIcon, lngFlags);
	ASSERT_VALID(p);
	return *p;
}



