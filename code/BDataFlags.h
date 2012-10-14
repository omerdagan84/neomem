
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
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual CString GetBDataText(BDoc* pDoc=NULL, ULONG lngPropertyID=0, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);
//x	virtual BOOL UIEditValue(BObject* pobj, BObject* pobjPropertyDef);

public:
	void SetFlags(ULONG lngFlags) { m_lngFlags = lngFlags; };
	ULONG GetFlags() { return m_lngFlags; };

public:
	static ULONG StringToFlags(LPCTSTR pszText);
	static CString FlagsToString(ULONG lngFlags);

private:
	ULONG m_lngFlags;

};

