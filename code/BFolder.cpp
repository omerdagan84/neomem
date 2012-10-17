

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



// add a column to this folder's contents view
void BFolder::AddColumn(OBJID idProperty, int nWidth, int nCol) {
	BData* pdat = GetPropertyData(propColumnInfoArray);
	if (!pdat) return;
	BDataColumns* pdatCols = DYNAMIC_DOWNCAST(BDataColumns, pdat);
	if (!pdatCols) {delete pdat; return;} //, d_d will eventually throw, so won't need this
	pdatCols->InsertColumn(idProperty, m_pDoc, nWidth, nCol);
	SetPropertyData(propColumnInfoArray, pdatCols);	// saves a copy of pdatcols
	delete pdat; // don't forget this!
}


// remove a column from this folder's contents view
void BFolder::RemoveColumn(OBJID idProperty) {
	BData* pdat = GetPropertyData(propColumnInfoArray);
	if (!pdat) return;
	BDataColumns* pdatCols = DYNAMIC_DOWNCAST(BDataColumns, pdat);
	if (!pdatCols) return;
	int nCol = pdatCols->GetColumnIndex(idProperty);
	if (nCol==-1) return;
	pdatCols->RemoveColumn(nCol);
	SetPropertyData(propColumnInfoArray, pdatCols);
	delete pdat;
}



