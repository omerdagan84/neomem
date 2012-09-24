
// BDataLink


#include "precompiled.h"

#include "BDoc.h"
#include "BDataLink.h"
#include "BDataPersonName.h"
#include "BObject.h"
#include "ConstantsDatabase.h"

#include "NeoMem.h"
#include "Constants.h"
#include "DialogEditLink.h"
#include "StringEx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



IMPLEMENT_SERIAL(BDataLink, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataLink::BDataLink() :
	m_p (0)
{
	//, need initializer
	m_bits.nFlags = flagSingle;
	m_bits.nCurrentItem = 0;
}

BDataLink::~BDataLink()
{
	// Delete any arrays used
	ClearMemory();
}


// Set data via text parser.
// pobjPropertyDef MUST be filled in for this bdata type.
BOOL BDataLink::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjPropertyDef);
	ASSERT(IsHard());

	// Get document
	BDoc* pDoc = pobjPropertyDef->GetDoc();
	ASSERT_VALID(pDoc);

	// Get property we're editing
	ULONG lngPropertyID = pobjPropertyDef->GetObjectID();

	// Make copy of string and remove leading and trailing spaces
	CString strCopy = str;
	strCopy.TrimLeft();
	strCopy.TrimRight();

	//, See if this property def allows multiple links
//	ULONG lngLinks = pobjPropertyDef->GetPropertyLong(propLimitNumberOfLinks);
//	BOOL bMultipleLinks = (lngLinks != 1);

	// Look up object and set to closest match
	// search through objects, starting at link source object and recursing downwards
	//, could do a fuzzy search
	//, exclude system objects? no - used to set classname, etc.

	// For generic handling, the Link Source object is defined by the propertydef, and
	// bRecurse will be assumed to be true

	// if gets more than one hit, then should bring up a dialog asking user to choose one
	// eg if you typed in "Andre" it might come back with "Andre Kertesz" and "Andre Norton"
	// if no match found, ask user if they want to create a new x, return false
	//, for now, just go with the first object found

	// Get the Link Source object, or use root as default.
	// e.g. this might be an Authors folder.
	OBJID idLinkSource = pobjPropertyDef->GetPropertyLink(propLinkSource);
	if (idLinkSource == 0) 
		idLinkSource = theApp.GetTopObjectID(); // get root object
	BObject* pobjLinkSource = pDoc->GetObject(idLinkSource);
	ASSERT_VALID(pobjLinkSource);

	// First check if text is blank, in which case this property value should be deleted
	//, for now, just delete link info
	// some properties can't be blank - e.g. class, so what about those?
	// could have a required field in the propertydef?
	// then check that, and if set to blank, say "Sorry, this property must have a value"
	// for now, just check if propertydef is class!
	if (strCopy.IsEmpty())
	{
		if (lngPropertyID == propClassName)
		{
			AfxMessageBox("This property must always have a value and cannot be deleted.", MB_ICONINFORMATION);
			return FALSE;
		}
		SetSingle();
		m_p = 0;
		m_strText.Empty();
		return TRUE;
	}

	// Now search for the text through all objects
	// Note: We need to exclude hidden objects, because otherwise user could type in "View" for
	// classname and it would be accepted.
	BObjects aResults;
	int nObjects = pDoc->SearchForText(pobjLinkSource, propName, strCopy, aResults, theApp.m_lngExcludeFlags);
	if (nObjects == 0)
	{
		// No matches found, so try parsing the search string as a person name and try again...
		BDataPersonName datPerson;
		datPerson.SetBDataText(str, 0, FALSE);
		CString strPerson = datPerson.GetBDataText(pDoc, 0);
		nObjects = pDoc->SearchForText(pobjLinkSource, propName, strPerson, aResults, theApp.m_lngExcludeFlags);

		// Still no matches found, so ask user if they want to add a new object of the default class to the 
		// link source.
		// If a match was found, code will fall through.
		if (nObjects == 0)
		{
			CString strMsg;
			OBJID idClass = pobjLinkSource->GetPropertyLink(propDefaultClass);
			BObject* pobjClass = pDoc->GetObject(idClass);
			ASSERT(pobjClass);
			CString strClassName = pobjClass->GetPropertyString(propName);
			CString strLinkSourceClass = pobjLinkSource->GetPropertyString(propClassName);
			CString strLinkSourceName = pobjLinkSource->GetPropertyString(propName);
			strClassName.MakeLower();
			strLinkSourceClass.MakeLower();
			// Special case for classes - just give message for now
			//, bring up class wizard if user says yes
			if (idClass == classClass)
			{
				strMsg.Format("No match was found for \"%s\" in the %s %s. Hit F4 to see a list of possible values.",
					(LPCTSTR) strCopy, (LPCTSTR) strLinkSourceClass, (LPCTSTR) strLinkSourceName);
				AfxMessageBox(strMsg, MB_ICONINFORMATION);
				return FALSE;
			}
			strMsg.Format("No match was found for \"%s\" in the %s %s. Do you want to add a new %s named \"%s\" and link to it with this property?",
				(LPCTSTR) strCopy, (LPCTSTR) strLinkSourceClass, (LPCTSTR) strLinkSourceName, (LPCTSTR) strClassName, (LPCTSTR) str);
			if (IDYES == AfxMessageBox(strMsg, MB_YESNO + MB_ICONQUESTION))
			{
				BObject& objNew = BObject::New(*pDoc, idClass, strCopy, pobjLinkSource->id);
				SetLink(&objNew);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	// probably best for now to always bring up dialog, unless only one match and it's exact
//	else if (nObjects > 1)
//	{
//		//, bring up link dialog to let user choose one of the found objects or add a new one
		// would need new method to pass a list of objects to fill listview with
		// and where would new one be added? to parent object
		// ideally would rank items in order of likelihood - eg "fiction" would match closer than "science fiction"
//		AfxMessageBox("more than one found - bring up dialog");
//		return FALSE;
//	}
//	else // nObjects == 1
//	{
		// need to pass the default here 
		// if user enters items separated by commas, might be more than one
		// need to create a new bdatalink object, set its link values, and call uieditvalue on it
		// if returns true, then copy results to this bdatalink object.
//		BObject* pobjMatch = DYNAMIC_DOWNCAST(BObject, aResults.GetAt(0));
//		ASSERT_VALID(pobjMatch);
//		BDataLink datLink;
//		datLink.SetLink(pobjMatch);
//		if (datLink.UIEditValue(0, pobjPropertyDef))
//		{
//			// Copy results to this bdatalink object
//			CreateCopyFrom(&datLink);
//			return TRUE;
//		}
		// One match found, so link to it
		// What if you entered "Fiction" a new item and it matched "Science Fiction"?
		// also, we need to arrange the found items in best fit order somehow...
		// ie we have this list of bobjects, and we want to arrange them so that their name properties
		// are the least distance from the search string...
		// need a good generic algorithm to do this - fuzzy string searching i guess - look on internet
		//, for now, we can just add up the bytes in each string and pick the closest match
		int nBestMatch = 0;
		if (nObjects > 1)
		{
			CStringEx strName = str;
			strName.MakeLower();
			int nCheckSum = strName.GetCheckSum();
			int nMinDifference = 100000;
			for (int i = 0; i < nObjects; i++)
			{
				BObject* pobj = DYNAMIC_DOWNCAST(BObject, aResults.GetAt(i));
				ASSERT_VALID(pobj);
				strName = pobj->GetPropertyString(propName);
				strName.MakeLower();
				int nCheckSum2 = strName.GetCheckSum();
				int nDifference = abs(nCheckSum2 - nCheckSum);
				if (nDifference < nMinDifference)
				{
					nMinDifference = nDifference;
					nBestMatch = i;
				}
			}			
		}

		BObject* pobj = DYNAMIC_DOWNCAST(BObject, aResults.GetAt(nBestMatch));
		ASSERT_VALID(pobj);
		SetLink(pobj);
		return TRUE;
//	}
	return FALSE;
}



// Bring up dialog to select object(s) to refer to for this link.
// Updates value and returns TRUE if user hit OK in dialog.
BOOL BDataLink::UIEditValue(BObject* pobj, BObject* pobjPropertyDef)
{
	// Check assumptions
	ASSERT_VALID(this);
//	ASSERT_VALID(pobj);
	ASSERT_VALID(pobjPropertyDef);
//	ASSERT_VALID(m_pobj); // not always true
	ASSERT(IsHard());

	// See if this property def allows multiple links (default is true).
	ULONG lngLinks = pobjPropertyDef->GetPropertyLong(propLimitNumberOfLinks);
	BOOL bMultiSelectEnabled = (lngLinks != 1);
	BOOL bMultiSelectVisible = TRUE; //, always, for now
	BOOL bMultiSelectOn = IsMultiple();

	// Get link source from propertydef
	OBJID idStart = pobjPropertyDef->GetPropertyLink(propLinkSource);
	if (idStart == 0)
		idStart = theApp.GetTopObjectID(); // default is root object

	TCHAR* pszCaption = _T("Edit Link");
	CString strPropName = pobjPropertyDef->GetPropertyString(propName);
	CString strInstructions;
	if (bMultiSelectEnabled)
		strInstructions.Format("Select a new value or values for the %s property.", (LPCTSTR) strPropName);
	else
		strInstructions.Format("Select a new value for the %s property.", (LPCTSTR) strPropName);

	// Bring up the Edit Link dialog, which allows user to modify the links and their order. 
	CDialogEditLink dlg;
	dlg.m_nHelpID = IDD_EDIT_LINK; //, use sethelpid
	if (dlg.DoModalLink(pszCaption, strInstructions, bMultiSelectVisible, bMultiSelectEnabled, bMultiSelectOn, 
									idStart, this, theApp.m_lngExcludeFlags) == IDOK)
	{
		// Note: Link(s) has already been saved to this bdata object by the dialog
		//, don't want UI to have code like that
//		SetLink(dlg.m_pobjSelected);
		return TRUE;
	}
	return FALSE;
}




// Get text representation of link(s) into a string, ie get list of objects linked to. 
// Eg "Houston, Austin, Berkeley Heights"
// pass true for bMachineVersion to get exportable version
//xLPCTSTR BDataLink::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
CString BDataLink::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDoc);
	ASSERT(IsHard());

	if (IsSingle())
	{
		BObject* pobj = GetLink();
		if (pobj)
		{
			ASSERT_VALID(pobj);
			if (bMachineVersion)
			{
				m_strText.Format("%d", pobj->GetObjectID());
				return m_strText;
			}
			else
			{

				// Get property def
				BObject* pobjPropertyDef = pDoc->GetObject(lngPropertyID);
				ASSERT_VALID(pobjPropertyDef);

				LPCTSTR pszText = 0;
				BOOL bDisplayLinkHierarchy = pobjPropertyDef->GetPropertyLong(propDisplayLinkHierarchy);
				OBJID idAdditionalProp = pobjPropertyDef->GetPropertyLink(propAdditionalDisplayProperty);
				BObject* pobjAdditionalProp = pDoc->GetObject(idAdditionalProp);

				// Just display the name of the link object if no other options specified
				if (bDisplayLinkHierarchy == FALSE && idAdditionalProp == 0)
				{
					return pobj->GetPropertyString(propName);
				}

				// Display Link Hierarchy
				// If this propertydef says to display the link object's hierarchy,
				// then we'll want to get the names of its parents, and add them to the string!
				// e.g. "Houston, Texas, United States"
				// Most properties won't have this set (default is false).
				if (bDisplayLinkHierarchy)
				{
					// Get the Link Source, which is where the GetParents method will stop.
					// If none specified, use the root object (shouldn't ever happen).
					OBJID idLinkSource = pobjPropertyDef->GetPropertyLink(propLinkSource);
					if (!idLinkSource)
						idLinkSource = pDoc->GetObject(rootMain)->id;
					BObject* pobjLinkSource = pDoc->GetObject(idLinkSource);
					ASSERT_VALID(pobjLinkSource);

					// If the link object is a direct child of the link source, then don't bother with getting the parents.
					if (pobj->GetParent()->id == idLinkSource)
					{
						// If no more stuff to be added then just exit here with object name.
						if (idAdditionalProp == 0)
							return pobj->GetPropertyString(propName);
						else
							// Otherwise, store in cstring and continue
							m_strText = pobj->GetPropertyString(propName);
					}
					else
					{
						// Get the parents of the link object, stopping at the link source object, then
						// get the text of the objects.
						// Note that GetParents will also include the link object in the array, to make
						// getting the text easier.
						//, global options can change the order of the hierarchy display
						BObjects aParents;
						int nParents = pobj->GetParents(aParents, pobjLinkSource);
						m_strText = aParents.GetText();
					}
				}

				// Additional Display Property
				// See if there's an additional property to display to make name unique
				if (pobjAdditionalProp)
				{
					ULONG lngAdditionalPropID = pobjAdditionalProp->GetObjectID();
					// If display link hierarchy is false, then we haven't gotten the object name yet, so get it now.
					if (bDisplayLinkHierarchy == FALSE)
						m_strText = pobj->GetPropertyString(propName); // eg "Museum of Fine Arts"
					// Only append it if it's not blank
//x					LPCTSTR szAdditional = pobj->GetPropertyString(lngAdditionalPropID); 
					CString strAdditional = pobj->GetPropertyString(lngAdditionalPropID); 
//x					if (*szAdditional != 0)
					if (strAdditional.IsEmpty() == FALSE)
						m_strText += g_strCommaSpace + strAdditional; // eg ", Canberra"
				}

				return m_strText;

			}
		}
	}
	else // IsMultiple
	{
		// This bdatalink object points to multiple items - walk through the link objects and get text.
		ASSERT_VALID(this);
		CObArray* pa = GetLinkArray();
		m_strText.Empty();
		CString s;
		int nItems = pa->GetSize();
		for (int i = 0; i < nItems; i++)
		{
			BObject* pobj = DYNAMIC_DOWNCAST(BObject, pa->GetAt(i));
			ASSERT_VALID(pobj);
			if (bMachineVersion)
			{
				s.Format("%d", pobj->GetObjectID());
				if (i == 0)
					m_strText += s;
				else
					m_strText += g_strCommaSpace + s;
			}
			else
			{
				if (i == 0)
					m_strText += pobj->GetPropertyString(propName);
				else
					m_strText += g_strCommaSpace + pobj->GetPropertyString(propName);
			}
		}
		return m_strText;
	}

	//, good for debugging - better to delete the prop value somehow though
//	m_strText = "[No Link]";
	m_strText.Empty();
	return m_strText;
}




void BDataLink::Serialize(CArchive &ar)
{
	ASSERT(IsHard());

	// Call base class first
	BData::Serialize(ar);

	// Serialize the data
	if (ar.IsStoring())
	{
		ASSERT( ((BOOL) (m_bits.nFlags & flagSingle)) != (BOOL) (m_bits.nFlags & flagMultiple)); // mutually exclusive
		ar << m_bits.nFlags;
		ar << m_bits.nCurrentItem;
		
		if (IsSingle())
		{
			// Note: m_p can be zero - that's how it was in 1.0 and carchive doesn't mind, so no need to check here.
			//, so how can you check the type is a BObject? use rtti?
			BObject* pobj = DYNAMIC_DOWNCAST(BObject, m_p); 
			ar << pobj;
		}
		else
		{
			ASSERT(m_p); // should never be zero here
			CObArray* pa = DYNAMIC_DOWNCAST(CObArray, m_p);
			ASSERT_VALID(pa);
			ar << pa;
		}
	}
	else // Loading
	{

		// Temporary variables to read data into, since CArchive won't let you read directly into a bit field.
		unsigned int nFlags;
		unsigned int nCurrentItem;

		ar >> nFlags;
		ar >> nCurrentItem;
		m_bits.nFlags = nFlags;
		m_bits.nCurrentItem = nCurrentItem;

		// Note: Didn't seem to be any way to read directly into m_p, even with all sorts of casts, etc.
		//, what good does serializing a pointer do?
		if (IsSingle()) // make sure m_bits is set before using IsSingle!
		{
			BObject* pobj;
			ar >> pobj;
			m_p = pobj;
		}
		else
		{
			CObArray* pa;
			ar >> pa;
			m_p = pa;
		}
	}
}





// Get amount of memory used by this bdata object.
ULONG BDataLink::GetMemoryUsed(BOOL bRecursive)
{
	ASSERT_VALID(this);
	ASSERT(IsHard());

	ULONG nBytes = sizeof(BDataLink);
	if (IsMultiple())
	{
		// Add array size if multiple link
		int nItems = GetLinkCount();
		nBytes += nItems * sizeof(CObject*);
	}
	return nBytes;
}



// Returns True if this link bdata object links to just one bobject.
BOOL BDataLink::IsSingle()
{
	ASSERT_VALID(this);
	ASSERT( ((BOOL) (m_bits.nFlags & flagSingle)) != (BOOL) (m_bits.nFlags & flagMultiple)); // mutually exclusive
	return (m_bits.nFlags & flagSingle);
}


// Returns True if this link bdata object links to more than one object (ie stores an array).
BOOL BDataLink::IsMultiple()
{
	ASSERT_VALID(this);
	ASSERT( ((BOOL) (m_bits.nFlags & flagSingle)) != (BOOL) (m_bits.nFlags & flagMultiple)); // mutually exclusive
	return (m_bits.nFlags & flagMultiple);
}


// Returns True if links are hard links (ie point directly to objects in memory) - default behavior
BOOL BDataLink::IsHard()
{
	ASSERT_VALID(this);
	return (! (m_bits.nFlags & flagSoftLinks));
}

// Returns True if links are soft links (ie just store ID number) - used in upgrading file
BOOL BDataLink::IsSoft()
{
	ASSERT_VALID(this);
	return (m_bits.nFlags & flagSoftLinks);
}



// Set to store single link
void BDataLink::SetSingle()
{
	ASSERT_VALID(this);

	// 1.1b first clear any memory
	ClearMemory();

	// Clear the relevant bits and set appropriate bit
	m_bits.nFlags &= ~(flagSingle + flagMultiple);
	m_bits.nFlags |= flagSingle;
}


// Set to store multiple links
void BDataLink::SetMultiple()
{
	ASSERT_VALID(this);

	// 1.1b first clear any memory (lack of this was causing bug on switching
	// from single to multi state, with m_p still pointing to a bobject!)
	// All calls to setmultiple involve new bdata objects except the one from the edit dialog,
	// so this should be okay to add. 
	ClearMemory();

	// Clear the relevant bits and set appropriate bit
	m_bits.nFlags &= ~(flagSingle + flagMultiple);
	m_bits.nFlags |= flagMultiple;
}



// Set a single link to a bobject.
// Note: pobj may be zero.
BOOL BDataLink::SetLink(BObject *pobj)
{
	ASSERT_VALID(this);
	ASSERT(IsHard());

	SetSingle();
	m_p = pobj;
	return TRUE;
}



// Get single bobject linked to, or NULL if none.
BObject* BDataLink::GetLink()
{
	ASSERT_VALID(this);
	ASSERT(IsSingle());
	ASSERT(IsHard());

	if (m_p)
	{
		BObject* pobj = DYNAMIC_DOWNCAST(BObject, m_p);
		ASSERT_VALID(pobj);
		return pobj;
	}
	return NULL;
}



// Get object ID of single link bobject, or 0 if no link
ULONG BDataLink::GetLinkObjectID()
{
	ASSERT_VALID(this);
	ASSERT(IsSingle());
	ASSERT(IsHard());

	BObject* pobj = GetLink();
	if (pobj)
	{
		ASSERT_VALID(pobj);
		return pobj->GetObjectID();
	}
	return 0;
}



// Get number of links this bdata object contains.
// If it's a single link with no bobject, returns 0.
int BDataLink::GetLinkCount()
{
	ASSERT_VALID(this);
	ASSERT(IsHard());

	if (IsSingle())
	{
		if (GetLink() == 0)
			return 0;
		else
			return 1;
	}
	CObArray* pa = GetLinkArray();
	return pa->GetSize();
}




// Remove all links from array
BOOL BDataLink::RemoveAll()
{
	ASSERT_VALID(this);
	ASSERT(IsMultiple());
	ASSERT(IsHard());

	CObArray* pa = GetLinkArray();
	pa->RemoveAll();
	return TRUE;
}



// Get the link from the array as specified by the index
BObject* BDataLink::GetLinkAt(int nIndex)
{
	ASSERT_VALID(this);
	ASSERT(IsMultiple());
	ASSERT(IsHard());

	CObArray* pa = GetLinkArray();
	return DYNAMIC_DOWNCAST(BObject, pa->GetAt(nIndex));
}



// Add a link to the specified object, assuming we're in multilink mode.
// Returns the index of the new link in the array, or -1 if not added (e.g. if duplicate).
// Doesn't allow adding duplicate objects!
int BDataLink::AddLink(BObject *pobj)
{
	ASSERT_VALID(this);
	ASSERT(IsMultiple());

	if (IsSoft())
	{
		CUIntArray* pa = DYNAMIC_DOWNCAST(CUIntArray, m_p);
		ASSERT_VALID(pa);

		// Make sure objectID isn't already in array
		UINT nID = pobj->GetObjectID();
		int nItems = pa->GetSize();
		for (int i = 0; i < nItems; i++)
		{
			// If object is already in array, just return -1!
			if (nID == pa->GetAt(i))
				return -1;
		}

		// Add the objectID to the array and return the index of the new link
		return pa->Add(nID);
	}
	else // hard links
	{
		// GetLinkArray will create array if necessary
		CObArray* pa = GetLinkArray();

		// Make sure object isn't already in array
		int nItems = pa->GetSize();
		for (int i = 0; i < nItems; i++)
		{
			// If object is already in array, just return -1!
			if (pobj == DYNAMIC_DOWNCAST(BObject, pa->GetAt(i)))
				return -1;
		}

		// Add the object to the array and return the index of the new link
		return pa->Add(pobj);
	}
}


// Add a link to the specified object, assuming we're in multilink mode.
// Returns the index of the new link in the array, or -1 if not added (e.g. if duplicate).
// Doesn't allow adding duplicate objects!
int BDataLink::AddLinkID(ULONG lngObjectID, BDoc *pDoc)
{
	ASSERT_VALID(this);
	ASSERT(IsMultiple());
	ASSERT(lngObjectID);
	ASSERT_VALID(pDoc);
	BObject* pobj = pDoc->GetObject(lngObjectID);
	ASSERT_VALID(pobj);
	return AddLink(pobj);
}


// Fill an array with the ObjectID's of the BObjects pointed to by this BData object.
// Returns the number of elements in the array.
//, this may be used to get an array of classid's to filter objects, eg from a propValidClasses property.
int BDataLink::GetObjectIDArray(CUIntArray &aObjectIDs)
{
	ASSERT_VALID(this);
	ASSERT(IsMultiple());
	ASSERT(IsHard());
	ASSERT_VALID(&aObjectIDs);

	CObArray* pa = GetLinkArray();
	int nItems = pa->GetSize();
	aObjectIDs.SetSize(nItems);
	// Walk through and copy links to new array
	for (int i = 0; i < nItems; i++)
	{
		BObject* pobj = DYNAMIC_DOWNCAST(BObject, pa->GetAt(i));
		ASSERT_VALID(pobj);
		aObjectIDs.SetAt(i, pobj->GetObjectID());
	}
	return aObjectIDs.GetSize();	
}




// BDataLink also includes this special method which searches for the specified ObjectID in 
// the list of objects it links to. This can be used in filtering objects in adding to listview, etc.
// Eg propValidClasses property can store links to valid classes, and then use this to filter objects
// when adding them.
// Returns True if the ObjectID is found.
BOOL BDataLink::FindReferences(ULONG lngFindObjectID)
{
	ASSERT_VALID(this);
//	ASSERT(IsMultiple()); // for now
	ASSERT(IsHard());

	if (IsMultiple())
	{
		CObArray* pa = GetLinkArray();
		int nItems = pa->GetSize();
		for (int i = 0; i < nItems; i++)
		{
			BObject* pobj = DYNAMIC_DOWNCAST(BObject, pa->GetAt(i));
			ASSERT_VALID(pobj);
			if (pobj->GetObjectID() == lngFindObjectID)
				return TRUE;
		}
	}
	else // single link
	{
		BObject* pobj = GetLink();
		if (pobj)
		{
			ASSERT_VALID(pobj);
			if (pobj->GetObjectID() == lngFindObjectID)
				return TRUE;
		}
	}
	return FALSE;
}



// Returns True if can find any references to the specified object in this bdata.
BOOL BDataLink::FindReferences(BObject* pobjFind)
{
	ASSERT_VALID(this);
	ASSERT(IsHard());

	if (IsSingle())
	{
		if (m_p == pobjFind)
			return TRUE;
	}
	else // multiple links
	{
		// Search through array for pobj
		CObArray* pa = GetLinkArray();
		ASSERT_VALID(pa);
		int nItems = pa->GetSize();
		for (int i = 0; i < nItems; i++)
		{
			BObject* pobj = DYNAMIC_DOWNCAST(BObject, pa->GetAt(i));
//			ASSERT_VALID(pobj);
			if (pobj == pobjFind)
				return TRUE;
		}
	}
	return FALSE;
}


// Remove any references to the specified object, or replace with new one if specified.
// Returns True if replaced reference.
BOOL BDataLink::ReplaceReferences(BObject* pobjFind, BObject* pobjNew /* = 0 */)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjFind);
	ASSERT(IsHard());

	BOOL bFound = FALSE;

	if (IsSingle())
	{
		// If points to object, set pointer to zero.
		//, ideally, delete this bdata object afterwards also
		if (m_p == pobjFind)
		{
			if (pobjNew)
				m_p = pobjNew;
			else
				m_p = 0;
			// Set flag
			bFound = TRUE;
		}
	}
	else // multiple links
	{
		// Search through array for pobjFind and remove occurrences
		CObArray* pa = GetLinkArray();
		ASSERT_VALID(pa);
		int nItems = pa->GetSize();
		for (int i = 0; i < nItems; i++)
		{
			BObject* pobj = DYNAMIC_DOWNCAST(BObject, pa->GetAt(i));
//			ASSERT_VALID(pobj);
			if (pobj == pobjFind)
			{
				if (pobjNew)
				{
					pa->SetAt(i, pobjNew);
				}
				else
				{
					// Remove pointer from array
					pa->RemoveAt(i);
					// Adjust indexes so can continue through rest of array
					i--;
					nItems--;
				}
				// Set flag
				bFound = TRUE;
			}
		}
		//, delete this bdata object if down to zero items
	}
	return bFound;
}




// Create a copy of this object.
// See also CreateCopyFrom
BData* BDataLink::CreateCopy()
{
	ASSERT_VALID(this);
	BDataLink* pdatCopy = new BDataLink();
	ASSERT(pdatCopy);
	ASSERT(pdatCopy->m_p == 0);

	if (IsSoft())
	{
		ASSERT_KINDOF(CUIntArray, m_p);
		// Create a new array and copy the old array to it
		CUIntArray* paNew = new CUIntArray();
		ASSERT(paNew);
		CUIntArray* paOld = DYNAMIC_DOWNCAST(CUIntArray, m_p);
		paNew->Append(*paOld);

		// Now save the new array.
		pdatCopy->m_p = paNew; 
		pdatCopy->m_bits = m_bits;
	}
	else // hard links
	{
		if (IsSingle())
		{
			pdatCopy->SetLink(GetLink());
			pdatCopy->m_bits = m_bits;
		}
		else // multiple links
		{
			// If we're linking to multiple objects, we'll need to copy the array.

			// Create a new array and copy the old array's pointers to it.
			CObArray* paNew = new CObArray();
			ASSERT(paNew);
			CObArray* paOld = GetLinkArray();
			paNew->Append(*paOld);

			// Now save the new array.
			pdatCopy->m_p = paNew; 
			pdatCopy->m_bits = m_bits;
		}
	}
	return pdatCopy;
}


// Copy the specified bdatalink object values to <this> bdatalink object.
// See also CreateCopy.
// Returns True if successful.
BOOL BDataLink::CreateCopyFrom(BDataLink *pdatSource)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pdatSource);
	ASSERT(IsHard());

	if (pdatSource->IsSingle())
	{
		SetLink(pdatSource->GetLink());
		m_bits = pdatSource->m_bits;
	}
	else // multiple links
	{
		// If we're linking to multiple objects, we'll need to copy the array.

		// Create a new array and copy the old array's pointers to it.
		CObArray* paNew = new CObArray();
		ASSERT(paNew);
		CObArray* paOld = pdatSource->GetLinkArray();
		paNew->Append(*paOld);

		// Now save the new array.
		m_p = paNew; 
		m_bits = pdatSource->m_bits;
	}
	return TRUE;
}




// Add menu items for bdata value popup
BOOL BDataLink::UIAddMenuItems(CMenu* pMenu, int nPos)
{
	ASSERT_VALID(this);
	ASSERT(IsHard());

	const int nChars = 256;
	TCHAR szBuffer[nChars];

	//, just want to walk through links - need an iterator
	if (IsSingle())
	{
		BObject* pobj = DYNAMIC_DOWNCAST(BObject, m_p);
		if (pobj)
		{
			if (!(pobj->GetFlag(theApp.m_lngSearchExcludeFlags)))
			{
				_sntprintf_s(szBuffer, nChars, "Goto %s", pobj->GetPropertyString(propName)); //, wsprintf
				pMenu->InsertMenu(nPos, MF_BYPOSITION | MF_STRING, ID_POPUP_BDATA_START, szBuffer);
				pMenu->InsertMenu(nPos + 1, MF_BYPOSITION | MF_SEPARATOR);
			}
		}
	}
	else // multiple links
	{
		// Search through array for pobj
		CObArray* pa = GetLinkArray();
		ASSERT_VALID(pa);
		int nItems = pa->GetSize();
		int nAdded = 0;
		for (int i = 0; i < nItems; i++)
		{
			BObject* pobj = DYNAMIC_DOWNCAST(BObject, pa->GetAt(i));
			ASSERT_VALID(pobj);
			if (!(pobj->GetFlag(theApp.m_lngSearchExcludeFlags)))
			{
				_sntprintf_s(szBuffer, nChars, "Goto %s", pobj->GetPropertyString(propName)); //, wsprintf
				pMenu->InsertMenu(nPos + nAdded, MF_BYPOSITION | MF_STRING, ID_POPUP_BDATA_START + i, szBuffer);
				nAdded++;
			}
		}
		if (nAdded > 0)
			pMenu->InsertMenu(nPos + nAdded, MF_BYPOSITION | MF_SEPARATOR);
	}
	return TRUE;
}


// Goto object
BOOL BDataLink::UIHandleCommand(UINT nCommandID)
{
	ASSERT_VALID(this);
	ASSERT(IsHard());

	int nIndex = nCommandID - ID_POPUP_BDATA_START; // 0 based

	BObject* pobj = 0;
	if (IsSingle())
		pobj = DYNAMIC_DOWNCAST(BObject, m_p);
	else
		pobj = GetLinkAt(nIndex);
	if (pobj)
	{
		ASSERT_VALID(pobj);
		pobj->GetDoc()->SetCurrentObject(pobj, NULL);
		return TRUE;
	}
	return FALSE;
}



// Check if objects linked to are still valid.
// Used by undo routines.
BOOL BDataLink::IsValid(BDoc* pDoc)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDoc);
	ASSERT(IsSingle() != IsMultiple()); // mutually exclusive
	ASSERT(IsHard());

	BOOL bValid = TRUE;

	//, just want to walk through links regardless of single/multiple - need an iterator
	if (IsSingle())
	{
		if (m_p)
			bValid = pDoc->IsBObjectValid(DYNAMIC_DOWNCAST(BObject, m_p));
	}
	else // multiple links
	{
		// Search through array for pobj
		CObArray* pa = GetLinkArray();
		ASSERT_VALID(pa);
		int nItems = pa->GetSize();
		for (int i = 0; i < nItems; i++)
		{
			BObject* pobj = DYNAMIC_DOWNCAST(BObject, pa->GetAt(i));
			if (!pDoc->IsBObjectValid(pobj))
			{
				bValid = FALSE;
				break;
			}
		}
	}

	return bValid;
}




// Clear any memory that has been declared
void BDataLink::ClearMemory()
{
	ASSERT_VALID(this);

	if (IsSoft())
	{
		// Release array of soft links (IDs)
		if (m_p)
		{
			ASSERT_KINDOF(CUIntArray, m_p);
			delete m_p;
			m_p = 0;
		}
	}
	else // hard links
	{
		if (IsSingle())
		{
			// Single pointer - no array to release
			m_p = 0;
		}
		else // multiple links
		{
			// Release array of BObject pointers
			if (m_p)
			{
				ASSERT_KINDOF(CObArray, m_p);
				delete m_p;
				m_p = 0;
			}
		}
	}
}



// Convert links to soft links (ie just referencing objects by their ObjectID's
// instead of pointing directly to the objects).
// Used in synchronizing, import/export, copy/paste, etc.
void BDataLink::ConvertToSoftLinks()
{
	ASSERT_VALID(this);
	ASSERT(IsHard());

	// Get array of ObjectIDs (treat single link as array also, for simplicity)
	CUIntArray* paIDs = new CUIntArray; // stored in m_p, deleted in ConvertToHardLinks
	if (IsSingle())
	{
		// Add the single ObjectID to the array
		// bug: didn't check for m_p = 0 case
		if (m_p)
		{
			BObject* pobj = DYNAMIC_DOWNCAST(BObject, m_p);
			ASSERT_VALID(pobj);
			paIDs->Add(pobj->GetObjectID());
		}
		// so array could be empty at this point
	}
	else // multiple links
	{
		// Fill the array with the linked ObjectIDs
		int nItems = GetObjectIDArray(*paIDs);
	}

	// Clear memory so we can use the m_p pointer
	ClearMemory();

	m_p = paIDs; // store array with this bdata object temporarily 
	m_bits.nFlags |= flagSoftLinks; // set flag so we know what type of data we're storing (but save info about type of links stored!)
}



// Convert soft links to hard links. 
void BDataLink::ConvertToHardLinks(BDoc* pDoc)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDoc);
//	ASSERT(IsSoft());

	// Exit if the links are already hard links.
	if ((m_bits.nFlags & flagSoftLinks) == 0)
		return;

	// All info is stored in a UInt array
	ASSERT_KINDOF(CUIntArray, m_p);
	CUIntArray* paIDs = DYNAMIC_DOWNCAST(CUIntArray, m_p);
	m_p = 0; // important to clear this here, so AddLink will create a new hard array
	
	// Clear soft links flag
	// Note: Need to do this here so in calling AddLink below it will add as hard links!
	m_bits.nFlags &= ~(flagSoftLinks); 

	int nItems = paIDs->GetSize(); // could be zero
	if (IsSingle())
	{
		if (nItems == 0)
		{
			m_p = 0;
		}
		else 
		{
			ASSERT(nItems == 1);
			UINT nID = paIDs->GetAt(0);
			BObject* pobj = pDoc->GetObject(nID);
			ASSERT_VALID(pobj);
			m_p = pobj;
		}
	}
	else // multiple links
	{
		CreateMultipleArray(); // this will create the new array if necessary (if nitems==0 then addlink won't get called!)
		for (int i = 0; i < nItems; i++)
		{
			UINT nID = paIDs->GetAt(i);
			AddLinkID(nID, pDoc); 
		}
	}

	// Delete the array of ID's - no longer needed.
	// m_p now points to something else. 
	delete paIDs; // created in ConvertToSoftLinks
	paIDs = 0;

	ASSERT(IsHard());
}


// Get link array, assuming that this bdata object is in multi link mode.
// Ie returns a pointer to the array of pointers to bobjects. 
// Will create a new array if one does not already exist. 
// The array is owned by this object, so caller must not delete it!
CObArray* BDataLink::GetLinkArray()
{
	ASSERT_VALID(this);
	ASSERT(IsMultiple());  // this may fail here if a class has "object properties" set to single link instead of multi link..
	ASSERT(IsHard());

	CreateMultipleArray(); // create array if needed
	// m_p is a CObject*
	CObArray* pa = STATIC_DOWNCAST(CObArray, m_p);
	ASSERT_VALID(pa);
	return pa;
}





// Create new link array if none exists yet, store in m_p. 
void BDataLink::CreateMultipleArray()
{
	ASSERT_VALID(this);
	ASSERT(IsMultiple());
	ASSERT(IsHard());

	CObArray* pa = 0;
	if (m_p)
	{
//x		// Get existing array
//x		pa = STATIC_DOWNCAST(CObArray, m_p);
//x		ASSERT_VALID(pa);
	}
	else // need to create array
	{
		pa = new CObArray();
		ASSERT_VALID(pa);
		m_p = pa; // Save it
	}
}



bool BDataLink::Test(BDoc* pdoc)
{
	// get some object to mess with
//	BObject* pobj = pdoc->GetCurrentObject();

	ULONG idHobo = 500865;
	ULONG idAuthor = 500862;
	ULONG idJulia = 500864;
	ULONG idTed = 500858;
	BObject* pobj = pdoc->GetObject(idHobo);

	// create an empty bdatalink object
//	BDataLink dat;

	// put it in single mode
//	dat.SetSingle();

	// this will bomb because must be in multiple mode
	//. need a way to catch expected assertions like this
//	dat.AddLink(pobj);

	// Get property to edit
	ULONG lngPropertyID = idAuthor;

	// Edit the property value in an appropriate dialog. See BObject::UIEditValue
	if (pobj->UIEditValue(lngPropertyID))
	{
		// verify object
		BDataLink* pdat = DYNAMIC_DOWNCAST(BDataLink, pobj->GetPropertyData(lngPropertyID));
		//. this bombs
		pdat->IsValid(pdoc);
		delete pdat;
	}

//	if (dat.UIEditValue(pobj, pobjPropdef))
	{
		// would do notifies here

	}

	return true;
}



