
// BData
// This is the base (abstract) class for all BData objects, each of which implements
// a different type of data (string, number, link, etc).
// Date: November 1999
//-----------------------------------------------------------------------------------------------------------------


//, don't like how all these ui things wound up getting into this class. 
// addmenuitems, handlecommand, onclick, onmousemove, editvalue...
// rename with UI prefix for now. 
// eventually handle with a ui callback object. 



#pragma once


class BObject;
class BDoc;

//, don't like having these here, but makes other code nicer
//xclass BDataColumns; //,



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
	virtual CString GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	virtual BOOL IsValid(BDoc* pDoc);
	virtual BOOL ReplaceReferences(BObject* pobjFind, BObject* pobjNew = 0);
	virtual void ResetData(); // use this to reset bdata's m_bCacheValid flag
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

	virtual BOOL UIAddMenuItems(CMenu* pMenu, int nPos);
	virtual BOOL UIEditValue(BObject* pobj, BObject* pobjPropertyDef);
	virtual BOOL UIHandleCommand(UINT nCommandID);
	virtual void UIOnClick();
	virtual void UIOnMouseMove();


//x	BDataColumns* ToColumns();
//x	BDataColumns ToColumns();
//x	BDataColumns& ToColumns();



	// Attributes
protected:
	CString m_strText; // This is a cache for the text-representation for this data object
	// This is moved to the individual bdata's that need it in order to save memory
//	BOOL m_bCacheValid; // Flag indicating if string cache is valid. Set to false when data changes
};

