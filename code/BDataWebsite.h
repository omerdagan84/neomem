
// BDataWebsite
// This class stores website addresses
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "BData.h"


class BDataWebsite : public BData  
{
	DECLARE_SERIAL(BDataWebsite)

public:
	BDataWebsite();
	virtual ~BDataWebsite();

public:
	virtual BData* CreateCopy();
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual CString GetBDataText(BDoc* pDoc=NULL, ULONG lngPropertyID=0, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

	virtual CStringArray& UIGetCommands();
	virtual BOOL UIEditValue(CUI& ui, BObject* pobj = NULL, BObject* pobjPropertyDef = NULL);
	virtual BOOL UIHandleCommand(UINT nCommandID, CUI& ui);
	virtual void UIOnClick(CUI& ui);
	virtual int UIGetCursor();

};


