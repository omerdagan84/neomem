
// BDataBinary
// This class stores binary data.
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "BData.h"

class BDataBinary : public BData {

	DECLARE_SERIAL(BDataBinary)

public:
	BDataBinary();
	virtual ~BDataBinary();

public:
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual LPCTSTR GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

private:
	void* m_pData;
	int m_nBytes;
};

