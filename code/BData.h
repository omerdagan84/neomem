
// BData
// This is the base (abstract) class for all BData objects, each of which implements
// a different type of data (string, number, link, etc).
// Date: November 1999
//-----------------------------------------------------------------------------------------------------------------


#pragma once


class BObject;
class CNeoDoc;


class BData : public CObject  
{
	DECLARE_SERIAL(BData)

// Construction
public:
	BData();
	~BData();

// Operations
public:
	virtual BOOL AddMenuItems(CMenu* pMenu, int nPos);
	virtual void ConvertToSoftLinks();
	virtual void ConvertToHardLinks(CNeoDoc* pDoc);
	virtual BData* CreateCopy();
	virtual BOOL EditValue(BObject* pobj, BObject* pobjPropertyDef);
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual LPCTSTR GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	virtual BOOL HandleCommand(UINT nCommandID);
	virtual BOOL IsValid(CNeoDoc* pDoc);
	virtual void OnClick();
	virtual void OnMouseMove();
	virtual BOOL ReplaceReferences(BObject* pobjFind, BObject* pobjNew = 0);
	virtual void ResetData(); // use this to reset bdata's m_bCacheValid flag
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

// Attributes
protected:
	CString m_strText; // This is a cache for the text-representation for this data object
	// This is moved to the individual bdata's that need it in order to save memory
//	BOOL m_bCacheValid; // Flag indicating if string cache is valid. Set to false when data changes
};

