
// BDataLong
// This class stores long integer data.
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "BData.h"



class BDataLong : public BData  
{
	DECLARE_SERIAL(BDataLong)

public:
	BDataLong();
	virtual ~BDataLong();

public:
	virtual BData* CreateCopy();
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual CString GetBDataText(BDoc* pDoc=NULL, ULONG lngPropertyID=0, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);
	virtual BOOL UIEditValue(CUI& ui, BObject* pobj = NULL, BObject* pobjPropertyDef = NULL);

public:
	void SetValue(ULONG lngValue) { m_lngValue = lngValue; };
	ULONG GetValue() { return m_lngValue; };

private:
	ULONG m_lngValue;
};

