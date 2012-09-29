
// XComboBoxEx


#include "precompiled.h"

#include "XComboBoxEx.h"

#include "Brooklyn.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





BEGIN_MESSAGE_MAP(XComboBoxEx, CComboBoxEx)
	//{{AFX_MSG_MAP(XComboBoxEx)
	ON_NOTIFY_REFLECT(CBEN_GETDISPINFO, OnGetDispInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



XComboBoxEx::XComboBoxEx()
{
}

XComboBoxEx::~XComboBoxEx()
{
}





// Return text or other information requested by the combo box.
// This is the CBEN_GETDISPINFO handler.
void XComboBoxEx::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMCOMBOBOXEX* pi = (NMCOMBOBOXEX*) pNMHDR;
	COMBOBOXEXITEM& rcbi = pi->ceItem;

	UINT nMask = rcbi.mask;
	int nItem = rcbi.iItem;

	BObject* pobj = (BObject*) GetItemData(nItem);
	if (pobj)
	{
		ASSERT_VALID(pobj);

		// Depending on what information is being requested, return different info.
		// Note cast from LPCTSTR to LPSTR, ie const char* to char*
		// see other instances of pszText
		if (nMask & TVIF_TEXT)
			rcbi.pszText = const_cast <LPSTR> ((LPCTSTR) pobj->GetPropertyString(propName)); //, propName
	}

	*pResult = 0;
}


// Select the item with the associated item data, or first item if not found.
// Returns the index of the selected item or -1 if not found.
int XComboBoxEx::SelectItemData(DWORD dwData, BOOL bSelectFirstIfNotFound /* = TRUE */)
{
	int nItem = FindItemData(dwData);
	if (nItem != -1)
	{
		SetCurSel(nItem);
	}
	else
	{
		// If user specified, select the first item since we didn't find what he was looking for
		if (bSelectFirstIfNotFound && (GetCount() > 0))
		{
			nItem = 0;
			SetCurSel(nItem);
		}
	}
	return nItem;
}



// Returns the item data associated with the currently selected item
// Returns 0 if no selected item
DWORD XComboBoxEx::GetSelectedItemData()
{
	int nItem = GetCurSel();
	if (nItem == -1)
		return 0;
	return GetItemData(nItem);
}




// Clear selected item
void XComboBoxEx::SelectNone()
{
	SetCurSel(-1);
}



// Add a new object as a child of the specified parent object.
// pobjParent is optional
// Returns index to new item, or -1 if failed.
int XComboBoxEx::AddObject(BObject *pobj, BObject *pobjParent)
{
	ASSERT_VALID(pobj);

	// If parent specified, find the parent item and get its indent level
	int nParent = 0;
	int nIndent = 0;
	if (pobjParent)
	{
		ASSERT_VALID(pobjParent);
		nParent = FindItemData((LPARAM) pobjParent);
		nIndent = GetItemIndent(nParent);
	}	

	// Get index of icon associated with this object
	int nImage = pobj->GetIconIndex();
	
	// Add this BObject to the combo
	COMBOBOXEXITEM cbi;
	cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	cbi.pszText = LPSTR_TEXTCALLBACK;
//	cbi.iItem = nItem;
	cbi.iItem = nParent;
	cbi.iImage = nImage;
	cbi.iSelectedImage = nImage;
	cbi.iIndent = nIndent;
	cbi.lParam = (LPARAM) pobj;

	int nIndex = InsertItem(&cbi);
	ASSERT (nIndex != -1);

	return nIndex;
}



// Add children of specified object to the combobox
// includestart, recurse, indent, pdatvalidclasses are optional
void XComboBoxEx::AddObjects(BObject* pobjStart, ULONG lngExcludeFlags, 
								BOOL bIncludeStart, BOOL bRecurse, int nIndent,
								BDataLink* pdatValidClasses) //, BOOL bAddNone)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjStart);

	// Set up structure used to add items
	COMBOBOXEXITEM cbi;
	cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	cbi.pszText = LPSTR_TEXTCALLBACK;

	// Add the start object if specified
	if (bIncludeStart)
	{
		BOOL bValidFlags = !(pobjStart->GetFlag(lngExcludeFlags));
		BOOL bValidClass = (pdatValidClasses == 0) ? TRUE : pdatValidClasses->FindReferences(pobjStart->GetClassID());
		if (bValidFlags && bValidClass)
		{
			// Get index of icon associated with this object
			int nImage = pobjStart->GetIconIndex();
		
			// Get the new item number
			int nItem = GetCount(); // always add to end of the list because of recursion

			// Add this BObject to the combo
			cbi.iItem = nItem;
			cbi.iImage = nImage;
			cbi.iSelectedImage = nImage;
			cbi.iIndent = nIndent;
			cbi.lParam = (LPARAM) pobjStart;

			int nIndex = InsertItem(&cbi);
			ASSERT (nIndex != -1);

			// Increase the indent level
			nIndent++;
		}
	}

	// Walk through child objects (if any) and add them to the list
	BObjects* paChildren = pobjStart->GetChildren();
	if (paChildren)
	{
		ASSERT_VALID(paChildren);
		int nItems = paChildren->GetSize();

		// Sort the children alphabetically (otherwise they show up in combo unsorted).
		// We must copy the children array to do this.
		// Note: We only need to sort them if autosort is on for the parent bobject.
		BObjects* paCopy = paChildren->CreateCopy();
		ASSERT_VALID(paCopy);
		if (pobjStart->IsSorted())
			paCopy->Sort(propName);

		int nAdded = 0;
		for (int i = 0; i < nItems; i++)
		{
			BObject* pobj = DYNAMIC_DOWNCAST(BObject, paCopy->GetAt(i));
			ASSERT_VALID(pobj);

			// Check that flags and class are valid
			BOOL bValidFlags = !(pobj->GetFlag(lngExcludeFlags));
			BOOL bValidClass = (pdatValidClasses == 0) ? TRUE : pdatValidClasses->FindReferences(pobj->GetClassID());
			if (bValidFlags && bValidClass)
			{
				// Get index of icon associated with this object
				int nImage = pobj->GetIconIndex();

				// Get the new item number
				int nItem = GetCount(); // always add to the end of the list because of recursion

				// Add this BObject to the list and store pointer to object in item data
				cbi.iItem = nItem;
				cbi.iImage = nImage;
				cbi.iSelectedImage = nImage;
				cbi.iIndent = nIndent;
				cbi.lParam = (LPARAM) pobj;

				int nIndex = InsertItem(&cbi);
				ASSERT (nIndex != -1);

				// Add children if recurse specified
				if (bRecurse && pobj->HasChildren())
					AddObjects(pobj, lngExcludeFlags, FALSE, bRecurse, nIndent + 1, pdatValidClasses);
			}
		}
		// Delete the copy of the children bobjects
		delete paCopy;
	}

}









// Find the item with the specified item data and return index to it, or -1 if not found.
// Search linearly through combobox data.
int XComboBoxEx::FindItemData(DWORD dwData)
{
	// Walk through all items, comparing their item data
	int nItems = GetCount();
	for (int i = 0; i < nItems; i++)
	{
		if (dwData == GetItemData(i))
			return i;
	}
	return -1;
}







// Get indent level of specified item
int XComboBoxEx::GetItemIndent(int nIndex)
{
	COMBOBOXEXITEM cbi;
	cbi.mask = CBEIF_INDENT;
	cbi.iItem = nIndex;

	GetItem(&cbi);
	return cbi.iIndent;
}


// Add item "None" using the specified image for its icon.
int XComboBoxEx::AddNone(int nImage)
{
	int nIndent = 0;
	COMBOBOXEXITEM cbi;
	cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	cbi.pszText = "(None)";
	cbi.iItem = 0;
	cbi.iImage = nImage;
	cbi.iSelectedImage = nImage;
	cbi.iIndent = nIndent;
	cbi.lParam = 0; //(LPARAM) pobj;

	int nIndex = InsertItem(&cbi);
	ASSERT (nIndex != -1);
	return nIndex;	
}



// Delete all items from the combobox
void XComboBoxEx::DeleteAllItems()
{
	while (CComboBoxEx::GetCount() > 0)
		CComboBoxEx::DeleteItem(0);
}






void XComboBoxEx::SetItemIcon(int nItem, int nImage)
{
	COMBOBOXEXITEM cbei = {0};
	cbei.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
	cbei.iItem = nItem;
	cbei.iImage = nImage;
	cbei.iSelectedImage = nImage;

	this->SetItem(&cbei);
}



// Delete the item with the specified item data, if found in list.
// Returns True if delete item successfully.
BOOL XComboBoxEx::DeleteItemData(LPARAM lParam)
{
	int nItem = FindItemData(lParam);
	if (nItem != -1)
		return DeleteItem(nItem);
	return FALSE;
}




// Select first item if nothing is currently selected,
// and there are actually items in the list. 
BOOL XComboBoxEx::SelectAnything()
{
	if (this->GetCurSel() == -1)
	{
		if (this->GetCount() > 0)
		{
			this->SetCurSel(0);
			return TRUE;
		}
	}
	return FALSE;

}


