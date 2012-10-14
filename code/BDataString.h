
// BDataString
// This class stores string information.
//-----------------------------------------------------------------------------------------------------------------



#pragma once



#include "BData.h"



class BDataString : public BData  
{
	DECLARE_SERIAL(BDataString)

public:
	BDataString();
	virtual ~BDataString();

public:
	virtual BData* CreateCopy();
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual CString GetBDataText(BDoc* pDoc=NULL, ULONG lngPropertyID=0, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);
	virtual BOOL UIEditValue(BObject* pobj, BObject* pobjPropertyDef, CUI& ui);


protected:
	// Note: Don't need this here because the base class defines m_strText
//	CString m_strText;
};


