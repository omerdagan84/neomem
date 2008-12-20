
// BDataLink
// This class stores link data (ordered links to other object(s) in the file).
// In order to save memory, you can set it to single or multiple reference.
// In single reference mode, it just stores a pointer to the bobject.
// In multiple reference mode, it stores an array of pointers to the bobjects. 
// Call SetMultiple after creation to switch it to that mode (default is single). 
// Maybe better to just store all refs as arrays, to simplify code...
//-----------------------------------------------------------------------------------------------------------------


#pragma once

#include "BData.h"

class BDataLink : public BData  
{
	DECLARE_SERIAL(BDataLink)

public:
	BDataLink();
	~BDataLink();

public:
	virtual BOOL AddMenuItems(CMenu* pMenu, int nPos);
	virtual BData* CreateCopy();
	virtual BOOL EditValue(BObject* pobj, BObject* pobjPropertyDef);
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual LPCTSTR GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	virtual BOOL HandleCommand(UINT nCommandID);
	virtual BOOL IsValid(CNeoDoc* pDoc);
	virtual BOOL ReplaceReferences(BObject* pobjFind, BObject* pobjNew = 0);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

public:
	void ConvertToHardLinks(CNeoDoc* pDoc);
	void ConvertToSoftLinks();
	void ClearMemory();
	BOOL CreateCopyFrom(BDataLink* pdatSource);
	void CreateMultipleArray();
	int AddLink(BObject* pobj);
	int AddLinkID(ULONG lngObjectID, CNeoDoc* pDoc);
	BOOL FindReferences(ULONG lngObjectID);
	BObject* GetLink();
	BObject* GetLinkAt(int nIndex);
	CObArray* GetLinkArray();
	int GetLinkCount();
	ULONG GetLinkObjectID();
	int GetObjectIDArray(CUIntArray& aObjectIDs);
	BOOL IsSingle();
	BOOL IsMultiple();
	BOOL IsSoft();
	BOOL IsHard();
	BOOL RemoveAll();
//	BOOL SetLinks();
	BOOL SetLink(BObject* pobj);
	void SetSingle();
	void SetMultiple();

	static bool Test(CNeoDoc* pdoc);

private:
	// This will either store a pointer to a pobj, or a pointer to an array of pobjs.
	//, use a union!? can't use with cobject because of constructor?
	// or a pointer to an array of soft links - ie objectIDs that will be converted to hard links
	CObject* m_p;

	// Store these items in a bitfield structure to conserve space
	struct sBitFields
	{
		unsigned int nFlags : 16; // eLinkFlags describing the link(s)
		unsigned int nCurrentItem : 16; // index of the 'current' item
	} m_bits;

	// Link Flags
	// Note: Single and multiple are mutually exclusive, so really only need one flag for both, but too late now... 
	enum eLinkFlags {flagSingle = 1, flagMultiple = 2, flagUncertain = 4, flagMultipleOr = 8, flagSoftLinks = 16};


};

