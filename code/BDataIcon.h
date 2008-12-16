
// BDataIcon
// This class encapsulates a Windows icon.
//-----------------------------------------------------------------------------------------------------------------

//, also have methods for creating icons from a bitmap (pass an hbitmap, coords, sizes to create)
//, also have method to save to a file



#pragma once

#include "BData.h"


class BDataIcon : public BData  
{
	DECLARE_SERIAL(BDataIcon)

public:
	BDataIcon();
	virtual ~BDataIcon(); 

	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual LPCTSTR GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);
	BOOL VerifyData();

	HICON GetIcon(int x, int y);
	BOOL LoadFile(LPCTSTR pszFileName);

private:
	void* m_pData; // pointer to raw icon data (taken directly from .ico file)
	UINT m_nBytes; // number of bytes of data
};


