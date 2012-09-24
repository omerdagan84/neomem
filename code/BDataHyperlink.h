
// BDataHyperlink
// This class stores hyperlink data (website address, email address, files, folders, etc).
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "BData.h"


class BDataHyperlink : public BData  
{
	DECLARE_SERIAL(BDataHyperlink)

public:
	BDataHyperlink();
	virtual ~BDataHyperlink();

public:
	BOOL IsWebsite();
	BOOL IsEmail();
	virtual BData* CreateCopy();
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual CString GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

	virtual BOOL UIAddMenuItems(CMenu* pMenu, int nPos);
	virtual BOOL UIEditValue(BObject* pobj, BObject* pobjPropertyDef);
	virtual BOOL UIHandleCommand(UINT nCommandID);
	virtual void UIOnClick();
	virtual void UIOnMouseMove();

};


