
// BDataViews


#include "precompiled.h"
#include "BDataViews.h"
#include "ConstantsDatabase.h"
#include "BObject.h"
#include "BDoc.h"

#include "NeoMem.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



// ViewInfo helper class
//------------------------------------------------------------------------------------------------------
ViewInfo::ViewInfo() :
	m_lngViewHeight (0),
	m_lngViewID (0)
{
}


IMPLEMENT_SERIAL(BDataViews, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number




// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataViews::BDataViews()
{
}

BDataViews::~BDataViews()
{
}


// Copy constructor
BDataViews::BDataViews(BDataViews* pdat)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pdat);
	// Copy the contents of the source arrays to our arrays.
	// It's safe to copy m_avi this way because the ViewInfo elements in the array don't 
	// own any memory themselves.
	m_avi.Copy(pdat->m_avi);
}



// Create a copy of this bdata object
BData* BDataViews::CreateCopy()
{
	ASSERT_VALID(this);
	// Use the copy constructor
	BDataViews* pdatCopy = new BDataViews(this);
	ASSERT_VALID(pdatCopy);
	return pdatCopy;
}



BOOL BDataViews::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	return FALSE;
}


LPCTSTR BDataViews::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDoc);

	m_strText = "ViewIDs: ";
	CString str;
	int nItems = m_avi.GetSize();
	for (int i = 0; i < nItems; i++)
	{
		ViewInfo& rvi = m_avi[i];
		str.Format("%d ", rvi.m_lngViewID);
		m_strText += str;
	}
	return m_strText;
}


void BDataViews::Serialize(CArchive &ar)
{
	// Call base class first
	BData::Serialize(ar);

	if (ar.IsStoring())
	{
		// Save number of elements in array
		int nItems = m_avi.GetSize();
		ar << nItems;

		// Walk through viewinfo objects and serialize them
		for (int i = 0; i < nItems; i++)
		{
			ViewInfo& rvi = m_avi[i];
			ar << rvi.m_lngViewID;
			ar << rvi.m_lngViewHeight;
		}
	}
	else
	{
		// Get number of elements
		int nItems;
		ar >> nItems;
		
		// Walk through and retrieve all viewinfo objects into array
		m_avi.SetSize(nItems);
		for (int i = 0; i < nItems; i++)
		{
			ar >> m_avi[i].m_lngViewID;
			ar >> m_avi[i].m_lngViewHeight;
		}
	}
}



// Get ViewInfo object associated with the specified index
ViewInfo& BDataViews::GetViewInfo(int nIndex)
{
	ASSERT_VALID(this);
	ASSERT_VALID(&m_avi);

	int nItems = m_avi.GetSize();
	ASSERT(nIndex >= 0 && nIndex < nItems);
	return m_avi[nIndex];
}



// Get the number of tabs represented in the array
int BDataViews::GetTabCount()
{
	ASSERT_VALID(this);
	ASSERT_VALID(&m_avi);

	// walk through array, checking for 0's indicating end of tab
	int nTabs = 0;
	int nItems = m_avi.GetSize();
	for (int i = 0; i < nItems; i++)
	{
		ViewInfo& rvi = GetViewInfo(i);
		if (rvi.m_lngViewID == 0)
			nTabs++;
	}
	return nTabs;
}



ULONG BDataViews::GetMemoryUsed(BOOL bRecursive)
{
	ASSERT_VALID(this);
	ULONG nBytes = sizeof(BDataViews);
	int nItems = m_avi.GetSize();
	nBytes += nItems * sizeof(ViewInfo);
	return nBytes;
}



// Get the name for the specified tab, made up of its view names (eg "Text, Contents")
//, could also store custom name for tab
BOOL BDataViews::GetTabName(int nTab, CString& strName, BDoc* pDoc)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDoc);

	// Walk through views to build up name for tab

	// Clear string first
	strName.Empty();

	// first we need to walk through the array till we get to the specified tab
	// then continue walking through array till we reach the end or hit a 0 0
	int nStart = GetTabIndex(nTab);
	int nIndex = nStart;
	int nItems = m_avi.GetSize();
	int nViews = 0;
	while (TRUE)
	{
		ViewInfo& rvi = GetViewInfo(nIndex);
		ULONG lngViewID = rvi.m_lngViewID;

		// Exit if we've reached the end of the views for this tab
		if (lngViewID == 0)
			break;

		ASSERT(lngViewID);

		BObject* pobjView = pDoc->GetObject(lngViewID);
		ASSERT_VALID(pobjView);

		// Get view caption
		CString strView = pobjView->GetPropertyString(propCaption);
		if (strView.IsEmpty()) // if caption property is empty, just use the name
			strView = pobjView->GetPropertyString(propName);

		if (nViews > 0)
		{
			// Remove any ampersand so only first view name will get underline character.
			// (otherwise Windows will just underline the last &'d character in the string).
			strView.Remove(_T('&'));
			strName += g_strCommaSpace;
			strName += strView;
		}
		else
		{			
			strName += strView;
		}

		nIndex++;
		nViews++;

		// Exit if we're past the end of the array
		if (nIndex >= nItems)
			break;
	}

	return TRUE;
}




// Get the index for the first view in the specified tab.
// Returns -1 if tab not found.
int BDataViews::GetTabIndex(int nTab)
{
	ASSERT_VALID(this);
	ASSERT_VALID(&m_avi);

	// Walk through array, checking for 0's that indicate the end of a tab.
	int nTabs = 0;
	int nItems = m_avi.GetSize();
	for (int i = 0; i < nItems; i++)
	{
		if (nTabs == nTab)
			return i;
		ViewInfo& rvi = GetViewInfo(i);
		if (rvi.m_lngViewID == 0)
			nTabs++;
	}
	return -1;
}



// Insert a new view into the array at the specified tab and view position.
// Will shift all remaining array elements if necessary.
// Returns the index to the new ViewInfo object (position in array).
// Note: Both tab and view are zero-based.
//, this is not fully implemented yet, at least as far as adding new tabs goes
int BDataViews::InsertView(int nTab, int nView, ULONG lngViewID, ULONG lngViewHeight)
{
	ASSERT_VALID(this);
	ASSERT(lngViewID);
//	ASSERT(nAfter == 0); //, for now

	// Get index of first view within specified tab
	int nIndex = GetTabIndex(nTab);

	// If tab not found, add a 0 and try again
	if (nIndex == -1)
	{
		ViewInfo vi;
		vi.m_lngViewID = 0;
		vi.m_lngViewHeight = 0;
		m_avi.Add(vi);
		nIndex = GetTabIndex(nTab);
	}

	if (nIndex != -1)
	{
		// Create a new ViewInfo object
		ViewInfo vi;
		vi.m_lngViewID = lngViewID;
		vi.m_lngViewHeight = lngViewHeight;

		// Insert the new view into the array
		nIndex += nView;
		m_avi.InsertAt(nIndex, vi); // may throw memory exception
	}

	// Return the index of the new ViewInfo object
	return nIndex;
}




// Find which tab the specified view is on, if any.
// Returns 0 based tab index, or -1 if view not available.
int BDataViews::GetViewTab(ULONG lngViewID)
{
	ASSERT_VALID(this);

	int nTab = -1; // default

	// Walk through array to find specified view, keeping track of tab count
	int nTabs = 0;
	int nItems = m_avi.GetSize();
	for (int i = 0; i < nItems; i++)
	{
		ViewInfo& rvi = GetViewInfo(i);
		// Exit loop if we found the view
		if (rvi.m_lngViewID == lngViewID)
		{
			nTab = nTabs;
			break;
		}
		if (rvi.m_lngViewID == 0)
			nTabs++;
	}

/*
	// Walk through tab data to find specified view
	int iTabs = GetTabCount();
	for (int iTab = 0; iTab < iTabs; iTab++)
	{
		// Each tab stores an additional array of BDataView objects
//		BDataArray* pdatViews = (BDataArray*) m_pdatTabs->m_apdat[iTab];
//		int nViews = pdatViews->m_apdat.GetSize();

		// Walk through view objects, looking for specified view
		for (int iView = 0; iView < nViews; iView++)
		{
			BDataView* pdatView = (BDataView*) pdatViews->m_apdat[iView];
			ASSERT_VALID(pdatView);
//			ASSERT_VALID(pdatView->m_pobjView);
			if (pdatView->m_lngViewID == lngViewID)
//			if (pdatView->m_pobjView->GetObjectID() == lngViewID)
			{
				// Found view
				// Note: Easier to return here than try to break out of all the loops
				return iTab;
			}
		}
	}
	// Return -1 if view not found
	return -1;
*/

	return nTab;
}


// Get the number of views on the specified tab
int BDataViews::GetViewCount(int nTab)
{
	ASSERT_VALID(this);
	ASSERT_VALID(&m_avi);

	int nStart = GetTabIndex(nTab);
	int nItems = m_avi.GetSize();
	int nViews = 0;
	for (int i = nStart; i < nItems; i++)
	{
		ViewInfo& rvi = m_avi[i];
		// Skip if this is a zero (indicates end of tab)
		if (rvi.m_lngViewID == 0)
			break;
		nViews++;
	}
	return nViews;
}





// Returns True if can find any references to the specified object in this bdata
BOOL BDataViews::FindReferences(BObject* pobjFind)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjFind);

	ULONG lngViewID = pobjFind->GetObjectID();
	// Walk through viewinfo objects and look for references
	int nItems = m_avi.GetSize();
	for (int i = 0; i < nItems; i++)
	{
		if (lngViewID == m_avi[i].m_lngViewID)
			return TRUE;
	}
	return FALSE;
}



// Remove any references to the specified object, or replace with new one if specified.
BOOL BDataViews::ReplaceReferences(BObject* pobjFind, BObject* pobjNew /* = 0 */)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjFind);

	BOOL bFound = FALSE;

	ULONG lngViewID = pobjFind->GetObjectID();
	// Walk through viewinfo objects and look for references
	int nItems = m_avi.GetSize();
	for (int i = 0; i < nItems; i++)
	{
		if (lngViewID == m_avi[i].m_lngViewID)
		{
			if (pobjNew)
			{
				// Replace reference
				m_avi[i].m_lngViewID = pobjNew->GetObjectID();
			}
			else
			{
				// Remove from array
				//, what about 0's after it?
				m_avi.RemoveAt(i);
				// Adjust indexes so can continue through array
				i--;
				nItems--;
			}
			// Set flag
			bFound = TRUE;
		}
	}
	return bFound;
}




// Remove the specified view from this arrangement
// Returns True if successful.
BOOL BDataViews::RemoveView(ULONG lngViewID)
{
	ASSERT_VALID(this);
	ASSERT_VALID(&m_avi);
	ASSERT(lngViewID);

	// Walk through viewinfo objects and look for references
	int nItems = m_avi.GetSize();
	for (int i = 0; i < nItems; i++)
	{
		if (lngViewID == m_avi[i].m_lngViewID)
		{
			// Remove the view
			m_avi.RemoveAt(i);
			//, what about the 0's? will they be alright?
			return TRUE;
		}
	}

	// Not found - return false
	return FALSE;
}



// Returns the maximum index of the array
int BDataViews::GetMaxIndex()
{
	ASSERT_VALID(&m_avi);
	return m_avi.GetSize();
}


// Move specified view up relative to siblings
BOOL BDataViews::MoveViewUp(ULONG lngViewID)
{
	ASSERT_VALID(&m_avi);

	// Walk through viewinfo objects and look for references
	int nItems = m_avi.GetSize();
	for (int i = 0; i < nItems; i++)
	{
		if (lngViewID == m_avi[i].m_lngViewID)
		{
			// If there is a previous view, and it's not a 0, swap their positions
			if (i > 0)
			{
				ULONG lngPrevViewID = m_avi[i - 1].m_lngViewID;
				if (lngPrevViewID != 0)
				{
					m_avi[i].m_lngViewID = lngPrevViewID;
					m_avi[i - 1].m_lngViewID = lngViewID;
					ULONG lngHeight = m_avi[i].m_lngViewHeight;
					m_avi[i].m_lngViewHeight = m_avi[i - 1].m_lngViewHeight;
					m_avi[i - 1].m_lngViewHeight = lngHeight;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}



// Sets the height of the specified view.
// nTab and nView are zero-based, nHeight is in %
BOOL BDataViews::SetViewHeight(int nTab, int nView, int nHeight)
{
	ASSERT(nTab < GetTabCount());
	ASSERT(nView < GetViewCount(nTab));
	int nIndex = GetTabIndex(nTab) + nView;
	m_avi[nIndex].m_lngViewHeight = nHeight;
	return TRUE;
}



// Gets the height of the specified view.
// nTab and nView are zero-based, nHeight is in %
int BDataViews::GetViewHeight(int nTab, int nView)
{
	ASSERT(nTab < GetTabCount());
	ASSERT(nView < GetViewCount(nTab));
	int nIndex = GetTabIndex(nTab) + nView;
	return m_avi[nIndex].m_lngViewHeight;
}
