

#include "Precompiled.h"




#include "BFolder.h"
#include "ConstantsDatabase.h"
#include "Brooklyn.h"


BFolder::BFolder(BDoc& doc, LPCTSTR pszName, OBJID idParent, OBJID idIcon, ULONG lngFlags) : 
	BObject(doc, classFolder, pszName, idParent, idIcon, lngFlags) // put overrides here
{
}


BFolder& BFolder::New(BDoc& doc, LPCTSTR pszName, OBJID idParent, OBJID idIcon, ULONG lngFlags) {
	BFolder* pf = new BFolder(doc, pszName, idParent, idIcon, lngFlags);
	ASSERT_VALID(pf);
	return *pf;
}


//, any object can have column info, so move to bobject.
BDataColumns& BFolder::GetColumns() {
	BDataColumns* pdatColumns = DYNAMIC_DOWNCAST(BDataColumns, this->GetPropertyData(propColumnInfoArray));
	ASSERT(pdatColumns);
	return *pdatColumns;
}

