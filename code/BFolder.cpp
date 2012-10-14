

#include "Precompiled.h"




#include "BFolder.h"
#include "ConstantsDatabase.h"
#include "Brooklyn.h"


// 'new' handler - create a BObject
BFolder::BFolder(BDoc& doc, LPCTSTR pszName, OBJID idParent, OBJID idDefaultClass, OBJID idIcon, ULONG lngFlags) : 
	// this is an object of class 'folder', not something in the 'class' folder. confusing. 
	BObject(doc, classFolder, pszName, idParent, idIcon, lngFlags) // put overrides here
{
	if (idDefaultClass)
		SetPropertyLink(propDefaultClass, idDefaultClass);
}


// static
BFolder& BFolder::New(BDoc& doc, LPCTSTR pszName, OBJID idParent, OBJID idDefaultClass, OBJID idIcon, ULONG lngFlags) {
	BFolder* p = new BFolder(doc, pszName, idParent, idDefaultClass, idIcon, lngFlags);
	ASSERT_VALID(p);
	return *p;
}




void BFolder::RemoveColumn(OBJID idProperty) {
	BData* pdat = GetPropertyData(propColumnInfoArray);
	if (!pdat) return;
	BDataColumns* pdatCols = DYNAMIC_DOWNCAST(BDataColumns, pdat);
	if (!pdatCols) return;
	int nCol = pdatCols->GetColumnIndex(idProperty);
	if (nCol==-1) return;
	pdatCols->RemoveColumn(nCol);
	SetPropertyData(propColumnInfoArray, pdatCols);
}


void BFolder::AddColumn(OBJID idProperty, int nWidth, int nCol) {
	BData* pdat = GetPropertyData(propColumnInfoArray);
	if (!pdat) return;
	BDataColumns* pdatCols = DYNAMIC_DOWNCAST(BDataColumns, pdat);
	if (!pdatCols) return;
	pdatCols->InsertColumn(idProperty, m_pDoc, nWidth, nCol);
	SetPropertyData(propColumnInfoArray, pdatCols);	
}



