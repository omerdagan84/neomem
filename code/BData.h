
// BData
// This is the base (abstract) class for all BData objects, each of which implements
// a different type of data (string, number, link, etc).
// Date: November 1999
//-----------------------------------------------------------------------------------------------------------------


#pragma once


class BObject;
class BDoc;
class CUI;


class BData : public CObject {

	DECLARE_SERIAL(BData)

	// Construction
public:
	BData();
	~BData();

	// Operations
public:
	virtual void ConvertToSoftLinks();
	virtual void ConvertToHardLinks(BDoc* pDoc);
	virtual BData* CreateCopy();
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
//x	virtual LPCTSTR GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	virtual CString GetBDataText(BDoc* pDoc = NULL, ULONG lngPropertyID = 0, BOOL bMachineVersion=FALSE);
	virtual BOOL IsValid(BDoc* pDoc);
	virtual BOOL ReplaceReferences(BObject* pobjFind, BObject* pobjNew = 0);
	virtual void ResetData(); // use this to reset bdata's m_bCacheValid flag
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);


//, don't like how all these ui things wound up getting into this class. 
// eventually handle with a ui callback object. 

//x	virtual BOOL UIAddMenuItems(CMenu* pMenu, int nPos);
	virtual BOOL UIEditValue(BObject* pobj, BObject* pobjPropertyDef, CUI& ui); //, ?
	virtual CStringArray& UIGetCommands(); // maybe more common to do uigetcommands(csa& sa)?
	virtual BOOL UIHandleCommand(UINT nCommandID);
	virtual void UIOnClick();
//x	virtual void UIOnMouseMove();
	virtual int UIGetCursor();


//x	BDataColumns* ToColumns();
//x	BDataColumns ToColumns();
//x	BDataColumns& ToColumns();



	// Attributes
protected:
	//, get rid of this
	CString m_strText; // This is a cache for the text-representation for this data object
	// This is moved to the individual bdata's that need it in order to save memory
//	BOOL m_bCacheValid; // Flag indicating if string cache is valid. Set to false when data changes
};

