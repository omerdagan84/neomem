
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
	virtual BData* CreateCopy();
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual CString GetBDataText(BDoc* pDoc=NULL, ULONG lngPropertyID=0, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

	virtual BOOL UIAddMenuItems(CMenu* pMenu, int nPos);
	virtual BOOL UIEditValue(BObject* pobj, BObject* pobjPropertyDef, CUI& ui);
	virtual BOOL UIHandleCommand(UINT nCommandID);
	virtual void UIOnClick();
	virtual void UIOnMouseMove();

};


