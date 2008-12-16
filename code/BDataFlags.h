
// BDataFlags
// This class is used to store and access flag data.
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "BData.h"


class BDataFlags : public BData  
{
	DECLARE_SERIAL(BDataFlags)

public:
	BDataFlags();
	~BDataFlags();

	virtual BData* CreateCopy();
	virtual BOOL EditValue(BObject* pobj, BObject* pobjPropertyDef);
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual LPCTSTR GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

//protected:
//	BObject* m_pobj;
//	ULONG m_lngID;
//	CString m_strLinkName;
	ULONG m_lngFlags;

};

