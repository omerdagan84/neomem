
// BObjects


#include "precompiled.h"

#include "NeoDoc.h"
#include "BObject.h"
#include "BObjects.h"
#include "ConstantsDatabase.h"

#include "NeoMem.h"
#include "resource.h" // for IsMoveValid error strings




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_SERIAL(BObjects, CObArray, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//----------------------------------------------------------------------------

BObjects::BObjects() :
	m_bOwnsItems (FALSE) // We don't own items in this array, so not responsible for deleting them in destructor.
{
}


BObjects::~BObjects()
{
	// Walk through the collection, releasing all objects and their children recursively.
	// Caution: Objects in collections can be shared. That is, the collection keeps a 
	// pointer to the object, but other parts of the program may also have pointers to 
	// the same object. You must be careful not to delete an object that is shared until 
	// all the parts have finished using the object.
	// If this BObject is the owner of the BObjects in the array, then we can delete the objects.
	// This is usually the case, unless we create a copy of a BObjects array for UI purposes.
	// Note: BDataLink also points to BObject objects but does not own them, so it doesn't delete them.

	// Only delete the BObjects if we own them!!
	if (m_bOwnsItems)
	{
		int nItems = CObArray::GetSize();
		for (int i = 0; i < nItems; i++)
		{
			BObject* pobj = (BObject*) CObArray::GetAt(i);
			ASSERT_VALID(pobj);
			delete pobj; // Delete the object from memory
		}
	}
	CObArray::RemoveAll(); // Remove all items from array
	ASSERT(CObArray::GetSize() == 0);
}


// Copy constructor (will be good for right click Copy...)
//BObjects::BObjects( const BObjects& a )
//{
//}
//BObjects& BObjects::operator=( const BObjects& a )
//{
//    m_years = a.m_years; return *this;
//}
//BOOL BObjects::operator==(BObject a)
//{
//    return m_years == a.m_years;
//}




// Serialization
//----------------------------------------------------------------------------

void 
BObjects::Serialize(CArchive& ar)
{
//	int nFileVersion = ar.GetObjectSchema(); // only on load
	// Note: This will automatically walk through the members and serialize them
	CObArray::Serialize(ar);
	ASSERT_VALID(this);
}


// Diagnostics
//----------------------------------------------------------------------------

#ifdef _DEBUG

void 
BObjects::AssertValid() const
{
	CObArray::AssertValid();
}

void 
BObjects::Dump(CDumpContext& dc) const
{
//	CObArray::Dump(dc);
}
#endif //_DEBUG



// Methods
//----------------------------------------------------------------------------

/*
// Set the array's text representation
// parse out using commas (and quotes), and assign to new child objects(?)
// would need to know what class of objects to create.
// not likely to need this anyway though.
void 
BObjects::SetText(CString strText)
{
	m_strText = strText;
}
*/



// Returns the text representation of the bobjects, eg "Boston, Miami, New York, Austin".
LPCTSTR 
BObjects::GetText(ULONG lngExcludeFlags /* = 0 */)
{
	ASSERT_VALID(this);
	m_strTextCache.Empty(); // clear cache first
	int nItems = GetSize();
	int iIndex = 0;
	for (int i = 0; i < nItems; i++)
	{
//		BObject* pobj = (BObject*) GetAt(i);
//		BObject* pobj = DYNAMIC_DOWNCAST(BObject, GetAt(i));
		BObject* pobj = STATIC_DOWNCAST(BObject, GetAt(i));
		ASSERT_VALID(pobj);
		if (!(pobj->GetFlag(lngExcludeFlags)))
		{
			if (iIndex == 0)
				m_strTextCache = pobj->GetPropertyText(propName);
			else
				m_strTextCache += g_strCommaSpace + pobj->GetPropertyText(propName);
			iIndex++; // used for figuring out commas
		}
	}
	return m_strTextCache;
}



// Find the specified object's index in the array, or -1 if not found.
// To recurse through child objects also, specify bRecursive true.
//. Note: This is a little flaky because the index returned is meaningless if object was found in
// one of the children! Just be careful how you use this routine for now!!
int 
BObjects::FindObject(const BObject* pobjFindThis, BOOL bRecursive /* = FALSE */)
{
	ASSERT_VALID(this);

	int nItems = GetSize();
	for (int i = 0; i < nItems; i++)
	{
		BObject* pobj = (BObject*) GetAt(i);
		ASSERT_VALID(pobj);
		if (pobj == pobjFindThis)
			return i;
		
		// If recursive option is on and this object has children, search through them as well
		if (bRecursive && pobj->GetChildren())
		{
			ASSERT_VALID(pobj);
			ASSERT_VALID(pobj->GetChildren());
			int n = pobj->GetChildren()->FindObject(pobjFindThis, bRecursive);
			if (n != -1)
				return n;
		}
	}
	// Object not found in this collection
	return -1;
}


// Find an object of the specified class in the list.
// This is used to find a specific propertyid, for instance (since ClassID and PropertyID are 
// equivalent in that case).
// Returns index to item or -1 if not found.
int 
BObjects::FindObjectClassID(ULONG lngClassID) // , BOOL bRecursive /* = FALSE */)
{
	ASSERT_VALID(this);
	ASSERT(lngClassID);

	int nItems = GetSize();
	for (int i = 0; i < nItems; i++)
	{
//		BObject* pobj = (BObject*) GetAt(i);
		BObject* pobj = STATIC_DOWNCAST(BObject, GetAt(i));
		ASSERT_VALID(pobj);
		if (pobj->GetClassID() == lngClassID)
			return i;
		
		// If recursive option is on and this object has children, search through them as well
//		if (bRecursive && pobj->GetChildren())
//		{
//			int n = pobj->GetChildren()->FindObject(pobjFindThis, bRecursive);
//			if (n != -1)
//				return n;
//		}
	}
	// Object not found in this collection
	return -1;
}



// Compare objects for use in qsort.
// Returns negative if 1 < 2, 0 if ==, positive if 1 > 2.
// for now, just sorts by name property
static int 
CompareObjects(BObject** ppobj1, BObject** ppobj2)
{
	// Dereference pointers (ppobj1 and ppobj2 are pointers to pointers)
	BObject* pobj1 = *ppobj1;
	BObject* pobj2 = *ppobj2;
	//. get sort property from static member var?
	// we don't have control over what qsort passes to this routine, so can't wrap a structure in a param...
	ULONG lngPropertyID = propName;
	int iResult = 0;
	if (pobj1 && pobj2)
	{
		LPCTSTR psz1 = pobj1->GetPropertyText(lngPropertyID);
		LPCTSTR psz2 = pobj2->GetPropertyText(lngPropertyID);
		iResult = lstrcmpi(psz1, psz2);
	}
	return iResult;
}


// Sort the objects according to the specified property.
// Note: This physically rearranges the BObject* pointers in the array so that they are in sorted order.
// This is how we'll sort the child objects in the tree.
// Note: For contents view, if you click on a column, it will perform the sorting in the view, not here.
void 
BObjects::Sort(ULONG lngPropertyID)
{
	ASSERT_VALID(this);
	ASSERT(lngPropertyID == propName); //, for now

	// To sort an array of structures using more than one field as a key, create a comparison 
	// function that relates the appropriate fields in each structure and returns a comparison 
	// value to indicate the result.

	// First see if we have any objects to sort at all...
	int nItems = CObArray::GetSize();
	if (nItems)
	{
		int nBytes = nItems * sizeof(BObject*);
		void* pData = CObArray::GetData();
		//. pass property to compare function somehow
//?		m_lngSortPropertyID = lngPropertyID;
		// Pass to qsort: addr of array, size of array, size of elements, comparison function.
		qsort(pData, (size_t) nItems, (size_t) sizeof(BObject*), (int (__cdecl*)(const void*, const void*)) CompareObjects);
	}
}




// Set parent for all objects in this collection.
// Note: This is for initialization purposes only - it does not actually handle moving the items;
// use MoveTo for that.
// This was only being used in one place (BObject serialize), 
// and removing the m_pobjParent reference is confusing, so ditching it. 
/* void 
BObjects::SetParent(BObject* pobjParent)
{
	ASSERT_VALID(this);
	int nItems = CObArray::GetSize();
	for (int i = 0; i < nItems; i++)
	{
		BObject* pobj = STATIC_DOWNCAST(BObject, GetAt(i));
		ASSERT_VALID(pobj);
		// Set the object's parent to the new parent
		pobj->m_pobjParent = pobjParent;
		
	}
}
*/


// Move the objects in this array to a new parent.
// Note: This handles actually moving the items (removing from existing parent,
// adding to new parent's child list, etc).
// This will set document modified flag and update all views.
// See also SetParent.
// This is called by drag drop routines.
BOOL 
BObjects::MoveTo(BObject* pobjNewParent)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjNewParent);

	// Walk through objects, remove from current parent, and add to new one.
	int nItems = CObArray::GetSize();
	if (nItems > 0)
	{
		for (int i = 0; i < nItems; i++)
		{
			BObject* pobj = STATIC_DOWNCAST(BObject, GetAt(i));
			ASSERT_VALID(pobj);

			// This will display a message and return false if any object cannot be moved.
			// Also sets doc modified flag and updates views.
			if (!pobj->MoveTo(pobjNewParent))
				return FALSE;
		}
	}
	return TRUE;
}




// Load a bobjects array from a global memory block.
// This is used in clipboard transfers.
// Returns number of items retrieved.
int 
BObjects::LoadFromGlobal(HGLOBAL hGlobal)
{
	ASSERT_VALID(this);

	// Create a shared file from global memory block
	CSharedFile sf(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);
	sf.SetHandle(hGlobal);

	// Read data to recreate bobjects object
	//. Note: this won't work if coming from another instance (right?) as we won't have
	// access to that instances data (pobjects).
	UINT nBytes;
	sf.Read(&nBytes, sizeof(UINT));
	int nItems = nBytes / sizeof(BObjects*);
	CObArray::SetSize(nItems);
	void* pData = (void*) CObArray::GetData();
	sf.Read(pData, nBytes);
	sf.Close();

	return nItems;
}


// Create a global memory block and write the array of BObject pointers to it.
// Used by clipboard and drag drop routines.
HGLOBAL 
BObjects::SaveToGlobal()
{
	ASSERT_VALID(this);

	void* pData = (void*) GetData(); // get array data containing bobject pointers
	UINT nBytes = GetSize() * sizeof(BObject*); // size of array data
	CSharedFile sf(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);
	sf.Write(&nBytes, sizeof(UINT));
	sf.Write(pData, nBytes);
	HGLOBAL hGlobal = sf.Detach();
	return hGlobal;
}



// Check if the objects can be moved to the target object.
// Displays a message box and returns False if a rule is broken.
BOOL 
BObjects::IsMoveValid(BObject* pobjTarget)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjTarget);

	int nError = 0;
	int nItems = GetSize();
	for (int i = 0; i < nItems; i++)
	{
		BObject* pobj = STATIC_DOWNCAST(BObject, GetAt(i));
		ASSERT_VALID(pobj);

		if (!pobj->IsMoveValid(pobjTarget, TRUE))
			return FALSE;
	}

	// Move is valid
	return TRUE;
}





// See if the objects and their children can be deleted - if not, give a message as to why not and return False.
// eg "Object is marked as undeletable", "Object is referenced by ....."
// Note: We implement this in BObjects instead of BObject because all of the delete handlers
// package all the objects to be deleted into a BObjects array, even if it's just one item.
BOOL 
BObjects::IsDeleteValid()
{
	ASSERT_VALID(this);

	CString strMsg;
	int nItems = GetSize();
	for (int i = 0; i < nItems; i++)
	{
		BObject* pobj = STATIC_DOWNCAST(BObject, GetAt(i));
		ASSERT_VALID(pobj);

		// Get object's class name (lowercase)
		CString strClassName = pobj->GetPropertyText(propClassName);
		strClassName.MakeLower();

		// Check flags
		if (pobj->GetFlag(flagNoDelete))
		{
			strMsg.Format(_T("The %s \"%s\" cannot be deleted - it is marked as undeletable."), 
									(LPCTSTR) strClassName, 
									(LPCTSTR) pobj->GetPropertyText(propName));
			AfxMessageBox(strMsg);
			return FALSE;
		}

/*
// this was moved to pobj->DeleteObject()

		// Check for links to the object recursively through the entire document
		BObjects aReferences;
		ASSERT_VALID(pobj->m_pDoc);
		int nLinks = pobj->m_pDoc->FindReferences(pobj, aReferences);
		if (nLinks)
		{
//			strMsg.Format(_T("The %s \"%s\" cannot be deleted because it is referenced by the following object(s): %s."), 
//									(LPCTSTR) strClassName, 
//									(LPCTSTR) pobj->GetPropertyText(propName), (LPCTSTR) aReferences.GetText());
//			AfxMessageBox(strMsg);
//			return FALSE;
			// Ask the user if they want to remove all references to the object and delete it
			strMsg.Format(_T("The %s \"%s\" is referenced by the following object(s): %s. Do you want to remove all references to the object and then delete it?"), 
									(LPCTSTR) strClassName, 
									(LPCTSTR) pobj->GetPropertyText(propName), (LPCTSTR) aReferences.GetText());
			if (IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION | MB_YESNO))
			{
				// If we're deleting a class, we'll want to replace all references with classPaper.
				BObject* pobjNew = 0; // default is to just remove all references
				if (pobj->GetClassID() == classClass)
					pobjNew = pobj->m_pDoc->GetObject(classPaper);
				// Remove/replace all references recursively and return true.
//				pobj->m_pDoc->m_pobjRoot->ReplaceReferences(pobj);
				pobj->m_pDoc->m_pobjRoot->ReplaceReferences(pobj, pobjNew);
				return TRUE;
			}
			return FALSE;
		}
*/

		// Check children recursively also
		if (pobj->GetChildren())
		{
			ASSERT_VALID(pobj->GetChildren());
			if (!pobj->GetChildren()->IsDeleteValid())
				return FALSE;
		}

	}
	return TRUE;
}



// Create a copy of this array, and set it so that it's not the owner of the BObjects.
// Caller is responsible for deleting the copy!
BObjects* 
BObjects::CreateCopy()
{
	ASSERT_VALID(this);
	BObjects* pCopy = new BObjects;
	ASSERT_VALID(pCopy);
	pCopy->Append(*this); // cool!
//	pCopy->m_bOwnsItems = FALSE; // default
	return pCopy;
}



