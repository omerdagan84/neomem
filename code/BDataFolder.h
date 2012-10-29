
// BDataFolder
// This class stores folder references
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "BData.h"


class BDataFolder : public BData  
{
	DECLARE_SERIAL(BDataFolder)

public:
	BDataFolder();
	virtual ~BDataFolder();

public:
	virtual BData* CreateCopy();
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual CString GetBDataText(BDoc* pDoc=NULL, ULONG lngPropertyID=0, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

//x	virtual BOOL UIAddMenuItems(CMenu* pMenu, int nPos);

//x	virtual BOOL UIEditValue(BObject* pobj, BObject* pobjPropertyDef);
	virtual CStringArray& UICommands();
	virtual BOOL UIHandleCommand(UINT nCommandID);
	virtual void UIOnClick();
//x	virtual void UIOnMouseMove();
	virtual int UICursorOnMouseover();

};


