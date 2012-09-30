

#include "Precompiled.h"


#include "BClass.h"
#include "ConstantsDatabase.h"


BClass::BClass(BDoc& doc, LPCTSTR pszName, OBJID idParent, OBJID idIcon, ULONG lngFlags) : 
	BObject(doc, classClass, pszName, folderClasses, idIcon, lngFlags) // put overrides here
{
}


BClass& BClass::New(BDoc& doc, LPCTSTR pszName, OBJID idParent, OBJID idIcon, ULONG lngFlags) {
	BClass* pf = new BClass(doc, pszName, idParent, idIcon, lngFlags);
	ASSERT_VALID(pf);
	return *pf;
}



