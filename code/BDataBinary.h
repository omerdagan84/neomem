
// BDataBinary
// This class stores binary data.
// Currently not used. 
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
	virtual CString GetBDataText(BDoc* pDoc=NULL, ULONG lngPropertyID=0, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

private:
	void* m_pData;
	int m_nBytes;
};

