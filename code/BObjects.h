
// BObjects
// Collection class for BObject
// basically a wrapper around mfc's CObArray.
// but can also use BDataLink to handle an array of bobjects.
// so there's a bit of duplication there...
// Date: January 2000
//-----------------------------------------------------------------------------------------------------------------




#pragma once

// You can derive your own classes from CObList. Your new list class, designed to 
// hold pointers to objects derived from CObject, adds new data members and new 
// member functions. Note that the resulting list is not strictly type safe, because 
// it allows insertion of any CObject pointer.

class BObject;

class BObjects : public CObArray
{
	DECLARE_SERIAL(BObjects)

// Construction
public:
    BObjects();
	virtual ~BObjects();
//	BObjects( const BObject& a ); // Copy constructor
//	const BObjects& operator=( const BObjects& a ); // Assignment operator
//	BOOL operator==(BObjects a); // Equivalence operator

// Operations
public:
	BObjects* CreateCopy();
	int FindObject(const BObject* pobjFindThis, BOOL bRecursive = FALSE);
	int FindObjectClassID(ULONG lngClassID);
	LPCTSTR GetText(ULONG lngExcludeFlags = 0);
	BOOL IsDeleteValid();
	BOOL IsMoveValid(BObject* pobjTarget);
	int LoadFromGlobal(HGLOBAL hGlobal);
	BOOL MoveTo(BObject* pobjNewParent);
	HGLOBAL SaveToGlobal();
    virtual void Serialize(CArchive& ar);
//	void SetParent(BObject* pobjParent);
	void SetOwnership(BOOL bOwnsItems) { m_bOwnsItems = bOwnsItems; }; // inline
	void Sort(ULONG lngPropertyID); // Sort contents on a property


// Attributes
// not serialized
private:
	CString m_strTextCache; // A string used to store a text representation of this collection of objects.
	BOOL m_bOwnsItems; // If True this array owns the BObjects and will delete them in the destructor.


#ifdef _DEBUG
public:
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

};



