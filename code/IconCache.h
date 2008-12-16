
// CIconCache
// Encapsulates an Image List control and map relating an IconID to an icon in the 
// Image List.
// Date: February 2000
//-----------------------------------------------------------------------------------------------------------------



#pragma once


// Map from longs to ints
typedef CMap<ULONG, ULONG, int, int> CMapIcons;


class CIconCache : public CObject
{
// Construction
public:
	CIconCache(CDocument* pDoc);
	virtual ~CIconCache();

// Operations
public:
	int GetIconIndex(ULONG lngIconID);
	CImageList* GetImageList();
	void RemoveIcons();

// Attributes
private:
	CDocument* m_pDocument;	// Pointer to document this cache is associated with
	CImageList m_iml;	// Image List
	CMapIcons m_mapIcons; // Map from IconID to Image List index
	

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconCache)
	//}}AFX_VIRTUAL


// Generated message map functions
protected:

};






