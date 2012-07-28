
// BObject
// A BObject contains zero or more property BObjects, zero or more child BObjects,
// and also encapsulates an item of data of a certain type (representing either the 
// name of the object or a property value). 
// Date: November 1999
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "BData.h"
#include "BDataLink.h"
#include "BObjects.h"

#include "StringEx.h"
#include "FileText.h"


//. Define the object handle type (keep it a pointer for now)
typedef BObject* HOBJECT; 
//DECLARE_HANDLE(HOBJECT); //. this works! for strong typing. see http://stackoverflow.com/questions/376452/enforce-strong-type-checking-in-c-type-strictness-for-typedefs


// Define the objectID type
// Windows uses OBJECTID for OLE
typedef ULONG OBJID;


class BObject : public CObject {

    DECLARE_SERIAL(BObject)

public:

	// Construction
	BObject(); // create from serialization only
	BObject(OBJID lngClassID); // so can set class id from neodoc. 
	virtual ~BObject();
//	BObject( const BObject& a ); // Copy constructor
//	const BObject& operator=( const BObject& a ); // Assignment operator
//	BOOL operator==(BObject a); // Equivalence operator

	// Operations
	BOOL AddChild(BObject* pobjChild, BOOL bCheckForDuplicates);
	BOOL AddProperty(BObject* pobjProperty);
	BOOL AddRtf(OBJID lngPropertyID, CString& strRtf); // Add the specified rtf text to the specified property
	void ChangeNamePropertyType(OBJID lngClassID, OBJID lngNewPropertyTypeID);
	void ChangePropertyType(BObject* pobjPropertyDef, BObject* pobjNewPropertyDef, OBJID lngNewPropertyTypeID);
	BOOL ClassDefAddProperty(OBJID lngPropertyID);
	void ClearFlag(ULONG lngFlag);
	void ConvertToHardLinks(BOOL bRecurse);
	void ConvertToSoftLinks(BOOL bRecurse);
	void CopyFrom(BObject* pobj);
	BData* CopyPropertyDataFrom(BObject* pobjSource, OBJID lngPropertyID);
	BOOL DeleteObject(BOOL bSetModifiedFlag = TRUE, BOOL bUpdateViews = TRUE);
	BOOL DeleteProperty(OBJID lngPropertyID, BOOL bSetModifiedFlag = TRUE, BOOL bUpdateViews = TRUE, BOOL bAskUser = FALSE);
	void DisplayProperties(); // const
	void Export(CFileText& file, BOOL bRecurse, BDataLink& datProps);
	BObject* FindProperty(OBJID lngPropertyID, BOOL bAddIfNotFound);
	int FindReferences(BObject* pobjFind, CObArray& aRefs, BOOL bRecurse);
	BObject* GetChild(int nChild);
	int GetChildCount(BOOL bRecurse = FALSE, BOOL bIncludeProperties = FALSE) const;
	BObjects* GetChildren() { return m_paChildren; }; // inline
	BOOL GetClassDefNewName(CString& strName);
	OBJID GetClassID() { return m_lngClassID; }; // inline
	BObject* GetClassObject();
	BData* GetData() { return m_pdat; }; // inline
	ULONG GetDefaultIconID();
	CNeoDoc* GetDoc() { return m_pDoc; }; // inline
	BOOL GetFlag(ULONG lngFlag);
	ULONG GetFlags() { return m_lngFlags; }; // inline
	OBJID GetIconID();
	int GetIconIndex();
//	int GetLinks(BObjects& aObjects, BObject* pobjStart);
	ULONG GetMemoryUsed(BOOL bRecurse) const;
	LPCTSTR GetName(BOOL bIncludeClassName);
	OBJID GetObjectID() { return m_lngObjectID; }; // inline
	BObject* GetParent() { return m_pobjParent; }; // inline
	int GetParents(BObjects& aParents, BObject* pobjStopAt, BOOL bIncludeThisObject = TRUE, BOOL bIncludeStopObject = FALSE);
	BObjects* GetProperties() { return m_paProperties; }; // inline
	BData* GetPropertyData(OBJID lngPropertyID, BOOL bCreateTempBDataIfNotFound = FALSE); // not const
	int GetPropertyDefAlignment();
	CString GetPropertyDefMachineVersionName();
	int GetPropertyDefWidth();
	int GetPropertyDefs(CObArray& aPropertyDefs, BOOL bInheritedOnly, BOOL bThisIsAClass);
	BObject* GetPropertyLink(ULONG lngPropertyID, BOOL bCreateTempBDataIfNotFound = FALSE); // not const
	ULONG GetPropertyLong(ULONG lngPropertyID, BOOL bCreateTempBDataIfNotFound = FALSE); // not const
	LPCTSTR GetPropertyText(ULONG lngPropertyID, BOOL bCreateTempBDataIfNotFound = FALSE);
	BYTE GetViewHeight() { return m_bytViewHeight; }; // inline
	int HasChildren() const;
//	void InitToZero();
	BOOL IsChild(BObject* pobjPossibleParent) const;
	BOOL IsChildOf(BObject* pobjPossibleParent, BOOL bRecurse) const;
	BOOL IsMoveUpDownValid(BOOL bMoveUp);
	BOOL IsMoveValid(BObject* pobjTarget, BOOL bDisplayMessages);
	BOOL IsParentSorted();
	BOOL IsSorted();
	BOOL MoveDown();
	BOOL MoveTo(BObject* pobjNewParent, BOOL bSetModifiedFlag = TRUE, BOOL bUpdateViews = TRUE, BOOL bDisplayMessages = TRUE);
	BOOL MoveUp();
	BOOL PropertyDefHasMachineVersion();
	BOOL RemoveChild(BObject* pobjChild);
	void ReplaceReferences(BObject* pobjFind, BObject* pobjNew = 0, BOOL bRecurse = TRUE);
	int SendMessage(ULONG lngMsg, BOOL bRecurse = TRUE);
    virtual void Serialize( CArchive& ar);
	BOOL SetClassID(OBJID lngNewClassID);
	void SetColumnsBasedOnClass(BObject* pobjDefaultClass);
	BOOL SetData(BData* pData);
	void SetDoc(CNeoDoc* pdoc) { m_pDoc = pdoc; }; // inline
	void SetFlag(ULONG lngFlag, BOOL bValue = TRUE, BOOL bRecurse = FALSE);
	void SetFlags(ULONG lngFlags) { m_lngFlags = lngFlags; }; // inline
	BOOL SetIconID(OBJID lngIconID);
	void SetObjectID(OBJID lngObjectID) { m_lngObjectID = lngObjectID; }; // inline
	void SetObjectText(const CString& strText);
	void SetParent(BObject* pobjNewParent);
	BOOL SetPropertyData(OBJID lngPropertyID, BData *pdat, BOOL bSetModifiedFlag = TRUE, BOOL bUpdateViews = TRUE);
	void SetPropertyLink(OBJID lngPropertyID, BObject* pobj, BOOL bSetModifiedFlag = TRUE, BOOL bUpdateViews = TRUE);
	void SetPropertyLong(OBJID lngPropertyID, ULONG lngValue, BOOL bSetModifiedFlag = TRUE, BOOL bUpdateViews = TRUE);
	BOOL SetPropertyText(OBJID lngPropertyID, LPCTSTR pszText, BOOL bSetModifiedFlag = TRUE, BOOL bUpdateViews = TRUE); //, BOOL bUpdateViews = FALSE);
	void SetViewHeight(BYTE bytViewHeight) { m_bytViewHeight = bytViewHeight; }; // inline
	BOOL SortChildren();

	BOOL UIEditValue(OBJID lngPropertyID);


	// Attributes
	// These are serialized
	private:
	OBJID m_lngObjectID; // Unique ObjectID, or 0 if this is a property BObject.
	OBJID m_lngClassID;	// ClassID of this BObject, or PropertyID if this is a property BObject.
	OBJID m_lngIconID;	// IconID of this BObject (0 means it's using the classdef icon)
	ULONG m_lngFlags; // 32 bit flags
	BObjects* m_paChildren; // pointer to list containing pointers to child bobjects. null if has no children.
	BObjects* m_paProperties; // pointer to list containing pointers to property bobjects. null if has no properties.
	BObject* m_pobjParent; // pointer to parent BObject
	BData* m_pdat; // pointer to data object containing data (name for an object BObject or property value for a property BObject)
	BYTE m_bytViewHeight; //, height of first pane in view as percent of total (eg 50) (this is temporary for v1.0)
//	BDataViews* m_pdatViews; // view and height information for this object (may be null)

//	BYTE m_bytDataFlags; // flags indicating which data has values and therefore should be serialized
//	enum eDataFlags {dataNone = 0, dataObjectID = 1, dataIconID = 2, dataFlags = 4, 
//									dataChildren = 8, dataProperties = 16, dataViewHeight = 32};
	
	// probably best to use DATE instead of coledatetime, because 
	// codt stores data as DATE AND an enum. a DATE is just a double. 
//,	COleDateTime m_dateCreated;
//,	COleDateTime m_dateModified;


	// These are not serialized
	CNeoDoc* m_pDoc; // pointer to the document which stores this bobject
	CStringEx m_strTextCache; // This is a cache for the text-representation for this data object
//	int m_bStringValid; // Flag indicating if string cache is valid. Set to false when data changes


#ifdef _DEBUG
	public:
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

};




