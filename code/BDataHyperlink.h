
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
	virtual CString GetBDataText(BDoc* pDoc=NULL, ULONG lngPropertyID=0, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

	virtual CStringArray& UIGetCommands();
	virtual BOOL UIEditValue(CUI& ui, BObject* pobj = NULL, BObject* pobjPropertyDef = NULL);
//	virtual BOOL UIHandleCommand(UINT nCommandID, CUI& ui);
//	virtual void UIOnClick(CUI& ui);
	virtual BOOL UIHandleCommand(UINT nCommandID);
	virtual void UIOnClick();
	virtual int UIGetCursor();

};


