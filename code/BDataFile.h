
// BDataFile
// This class stores file references
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "BData.h"


class BDataFile : public BData  
{
	DECLARE_SERIAL(BDataFile)

public:
	BDataFile();
	virtual ~BDataFile();

public:
	virtual BOOL AddMenuItems(CMenu* pMenu, int nPos);
	virtual BData* CreateCopy();
	virtual BOOL EditValue(BObject* pobj, BObject* pobjPropertyDef);
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual LPCTSTR GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	virtual BOOL HandleCommand(UINT nCommandID);
	virtual void OnClick();
	virtual void OnMouseMove();
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);
};


