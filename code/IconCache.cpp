
// CIconCache




#include "precompiled.h"

#include "IconCache.h"
#include "NeoMem.h"

#include "BDoc.h"
#include "BObject.h"
#include "ConstantsDatabase.h"
#include "BDataIcon.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




// Construction/Destruction
//-----------------------------------------------------------------------------

CIconCache::CIconCache(CDocument* pDoc) :
	m_pDocument (pDoc)
{
	ASSERT_VALID(pDoc);

	// Create an image list to store icons in
	if (!m_iml.Create(16, 16, ILC_MASK | ILC_COLOR16, 10, 10))
		xTRACE("! CIconCache Constructor - Failed to create image list\n");
}



CIconCache::~CIconCache()
{
	// Clear the image list and the icon map
	RemoveIcons();
	// Delete the windows image list object
	m_iml.DeleteImageList();
}



// Methods
//-----------------------------------------------------------------------------


// Returns a pointer to the image list associated with the icon cache
CImageList* CIconCache::GetImageList()
{
//	return m_pImageList;
	return &m_iml;
}


// Remove icons and clear the map pointing from iconid to icon index
void CIconCache::RemoveIcons()
{
	// Remove all images from image list
	while (m_iml.Remove(0));
	
	// Clear the map
	m_mapIcons.RemoveAll();
}



// Get the index of the specified icon as stored in the image list.
// Uses a map to associate an IconID with the index number.
// If the IconID is not in the image map then it will load it.
// Returns index, or -1 on failure.
int CIconCache::GetIconIndex(ULONG lngIconID)
{
	CString strFile;
	HICON hIcon = 0;
	int nIndex = -1;

	BDoc* pDoc = DYNAMIC_DOWNCAST(BDoc, m_pDocument);

	// Use default if none specified
	if (lngIconID == 0) lngIconID = iconDefault;

	// Look for the specified Icon in the map
	if (m_mapIcons.Lookup(lngIconID, nIndex) == 0)
	{
		// Icon was not in the map, so load it into the image list and add to the map
		// Create an icon from the data
//		strFile = "e:\\projects\\NeoMem\\res\\Infocom Spellbreaker.ico";
//		HICON hIcon = (HICON) LoadImage(0, strFile, IMAGE_ICON, 16, 16, 
//								LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		// Get the icon object so we can get the raw data for the icon
		BObject* pobjIcon = pDoc->GetObject(lngIconID);
		ASSERT_VALID(pobjIcon);

		//, note - getpropertydata might return wrong type, or 0, so fix below. 
		BDataIcon* pData = DYNAMIC_DOWNCAST(BDataIcon, pobjIcon->GetPropertyData(propIconData));
		if (pData)
		{
			// Get HICON handle from raw data (a BDataIcon object)
			ASSERT_VALID(pData);
			hIcon = pData->GetIcon(16, 16); // creates icon from raw data
		}
		else // icon resource needs to be loaded
		{
			UINT nResourceID = pobjIcon->GetPropertyLong(propIconResourceID);
			if (nResourceID)
				hIcon = theApp.LoadIcon(nResourceID); //. bug: this has bombed in release mode (why?)
			else
			{
				//.
//				strFile = pobjIcon->GetPropertyText(propFileSource);
//				hIcon = (HICON) LoadImage(0, strFile, IMAGE_ICON, 16, 16, 
//									LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				return -1;
			}
		}

		delete pData;

		if (!hIcon) return -1;

		// Add the icon to the image list
		nIndex = m_iml.Add(hIcon);
		if (nIndex == -1) return -1;
		
		// Now destroy the icon
		if (!DestroyIcon(hIcon))
			xTRACE("! DestroyIcon failed\n");

		// Add the IconID to the map
		m_mapIcons.SetAt(lngIconID, nIndex);
		xTRACE("CIconCache GetIconIndex - Added IconID=%d to image list at index %d\n", lngIconID, nIndex);
		
	}
	else
	{
		// IconID was found
		xTRACE("CIconCache GetIconIndex - Found IconID=%d in image list at index %d\n", lngIconID, nIndex);
	}
	
	return nIndex;
}





