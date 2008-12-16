

// BDataBoolean
// This class stores boolean data.
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "BData.h"

class BObject;



class BDataBoolean : public BData  
{
	DECLARE_SERIAL(BDataBoolean)

public:
	BDataBoolean();
	virtual ~BDataBoolean();

public:
	virtual BData* CreateCopy();
	virtual BOOL EditValue(BObject* pobj, BObject* pobjPropertyDef);
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual LPCTSTR GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

public:
	int m_nValue; // 0 for no, -1 for yes
};


