
// ColumnInfo


#include "precompiled.h"

#include "BDataColumns.h"
#include "ConstantsDatabase.h"
#include "BDoc.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



IMPLEMENT_SERIAL(BDataColumns, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number




// ColumnInfo class
//------------------------------------------------------------------------------------------------------

ColumnInfo::ColumnInfo() :
	m_lngPropertyID (0),
	m_nColAlignment (LVCFMT_LEFT), // left align
	m_nColWidth (120), // pixels
	m_pobjPropertyDef (NULL) // pointer to a property def bobject - we don't own it
{
}



// Construction/Destruction
//------------------------------------------------------------------------------------------------------

const int m_nGrowBy = 8;

BDataColumns::BDataColumns() :
	m_nColumns (0)
	// the arrays just have plain constructors, so no need to initialize here
{
	m_aci.SetSize(0, m_nGrowBy);
	m_anOrder.SetSize(0, m_nGrowBy);
}


BDataColumns::~BDataColumns()
{
}


// Copy constructor
BDataColumns::BDataColumns(BDataColumns* pdat)
{
	// Copy the contents of the source arrays to our arrays.
	// It's safe to copy m_aci this way because the ColumnInfo elements in the array don't 
	// own any memory themselves.
	m_nColumns = pdat->m_nColumns;
	m_aci.Copy(pdat->m_aci);
	m_anOrder.Copy(pdat->m_anOrder);
}



// Create a copy of this bdata object
BData* BDataColumns::CreateCopy()
{
	// Use the copy constructor
	BDataColumns* pdatCopy = new BDataColumns(this);
	return pdatCopy;
}



// Set data via text parser
// This will be a read only property value
BOOL BDataColumns::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	return FALSE;
}


// Get text representation of data into string
CString BDataColumns::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	m_strText = "PropertyIDs: ";
	CString str;
	for (int i = 0; i < m_nColumns; i++)
	{
		ColumnInfo& rci = m_aci[i];
		str.Format(_T("%d "), rci.m_lngPropertyID);
		m_strText += str;
	}
	return m_strText;
}


void BDataColumns::Serialize(CArchive &ar)
{
	// Call base class first
	BData::Serialize(ar);
	
	if (ar.IsStoring())
	{
		// Save number of columns
		ar << m_nColumns;
		
		ASSERT(m_aci.GetSize() >= m_nColumns); // (might be >)

		// Walk through columninfo objects and serialize them
		for (int i = 0; i < m_nColumns; i++)
		{
			ColumnInfo& rci = m_aci[i];
			ar << rci.m_lngPropertyID;
			ar << rci.m_nColWidth;
			ar << rci.m_nColAlignment;
		}
		
		// Save the order array also
		// Note: CUintArray doesn't serialize its members like this
//		m_anOrder.Serialize(ar); // why not this?
		int nCols = m_anOrder.GetSize();
		ar << nCols;
		for (int i = 0; i < nCols; i++)
		{
			ar << m_anOrder[i];
		}
	}

	else // Loading
	{
		// Get number of columns
		ar >> m_nColumns;
		
		// Walk through and retrieve all columninfo objects into array
		m_aci.SetSize(m_nColumns + 4, m_nGrowBy);
		for (int i = 0; i < m_nColumns; i++)
		{
			ar >> m_aci[i].m_lngPropertyID;
			ar >> m_aci[i].m_nColWidth;
			ar >> m_aci[i].m_nColAlignment;
		}

		// Get order array
		int nCols = 0;
		ar >> nCols;
		m_anOrder.SetSize(nCols);
		for (int i = 0; i < nCols; i++)
		{
			ar >> m_anOrder[i];
		}

	}

}



// Get ColumnInfo object associated with the specified column
ColumnInfo& BDataColumns::GetColumnInfo(int nCol)
{
	ASSERT(nCol >= 0 && nCol < m_nColumns);
	return m_aci[nCol];
}


// Get the number of columns in the array
const int BDataColumns::GetColumnCount()
{
	return m_nColumns;
}








// Insert the specified PropertyID at the specified location, or at the end if none specified.
// Returns a reference to the new ColumnInfo object.
ColumnInfo& BDataColumns::InsertColumn(ULONG lngPropertyID, BDoc* pDoc, int nWidth /* = 0 */, int nCol /* = -1 */)
{
	ASSERT(lngPropertyID);
	ASSERT_VALID(pDoc);

	// Look up the property def object
	BObject* pobjPropDef = pDoc->GetObject(lngPropertyID);
	ASSERT_VALID(pobjPropDef);

	// Create a new ColumnInfo object
	ColumnInfo ci;
	ci.m_lngPropertyID = lngPropertyID;
	ci.m_pobjPropertyDef = pobjPropDef;
	ci.m_nColAlignment = pobjPropDef->GetPropertyDefAlignment();
	if (nWidth)
		ci.m_nColWidth = nWidth;
	else
		ci.m_nColWidth = pobjPropDef->GetPropertyDefWidth();

	// Add the new column to the array
	int nIndex = nCol; // insert at specified position 
	if (nCol == -1) nIndex = m_nColumns; // add to end of array
	m_aci.InsertAt(nIndex, ci);

	// Get the new number of columns
	m_nColumns++;

	// Return a reference to the new ColumnInfo object as stored in the array
	return m_aci[nIndex];
}




// Set the width for the specified column
BOOL BDataColumns::SetColumnWidth(int nCol, int nWidth)
{
	ASSERT(nCol >= 0 && nCol < m_nColumns);
	ColumnInfo& rci = GetColumnInfo(nCol);
	rci.m_nColWidth = nWidth;
	return TRUE;
}



// Get the width for the specified column
int BDataColumns::GetColumnWidth(int nCol)
{
	ASSERT(nCol >= 0 && nCol < m_nColumns);
	ColumnInfo& rci = GetColumnInfo(nCol);
	return rci.m_nColWidth;
}



// Remove the specified column
BOOL BDataColumns::RemoveColumn(int nCol)
{
	ASSERT(nCol >= 0 && nCol < m_nColumns);
	ASSERT(m_nColumns > 0);
	m_aci.RemoveAt(nCol);
	m_nColumns--;
	return TRUE;
}



// Get the name for the specified column
//xLPCTSTR BDataColumns::GetColumnName(int nCol, BDoc* pDoc)
CString BDataColumns::GetColumnName(int nCol, BDoc* pDoc)
{
	ColumnInfo& rci = GetColumnInfo(nCol);
	// Get pointer to object if don't have already
	if (rci.m_pobjPropertyDef == 0)
	{
		ASSERT_VALID(pDoc);
		ASSERT(rci.m_lngPropertyID);
		rci.m_pobjPropertyDef = pDoc->GetObject(rci.m_lngPropertyID);
	}
	ASSERT_VALID(rci.m_pobjPropertyDef);
	return rci.m_pobjPropertyDef->GetPropertyString(propName);
}



// Get the order of the columns in an array to be used by the listview control
LPINT BDataColumns::GetColumnOrder()
{
	UINT* pdata = m_anOrder.GetData();
	int nCols = m_anOrder.GetSize();
	// If number of items in array doesn't match the number of columns it's supposed to have,
	// or there is no data in array, initialize it to a standard order.
	if (nCols != m_nColumns || pdata == NULL)
	{
		m_anOrder.SetSize(m_nColumns);
		for (int i = 0; i < m_nColumns; i++)
			m_anOrder[i] = i;
	}
	pdata = m_anOrder.GetData();
//	ASSERT(pdata); // might be zero if there are no columns
	return (LPINT) pdata;
}



// Store the order for the columns as retrieved from the listview control
void BDataColumns::SetColumnOrder(LPINT anOrder, int nColumns)
{
	ASSERT(nColumns == m_nColumns); // number of listview columns should match what we have
	m_anOrder.SetSize(nColumns);
	for (int i = 0; i < nColumns; i++)
	{
		m_anOrder[i] = anOrder[i];
	}
}




ULONG BDataColumns::GetMemoryUsed(BOOL bRecursive)
{
	ULONG nBytes = sizeof(BDataColumns);
	nBytes += m_anOrder.GetSize() * sizeof(UINT);
	nBytes += m_aci.GetSize() * sizeof(ColumnInfo);
	return nBytes;
}



// Remove all columns
void BDataColumns::RemoveColumns()
{
	m_aci.RemoveAll();
	m_anOrder.RemoveAll();
	m_nColumns = 0;
}



// Returns True if can find any references to the specified object in this bdata
BOOL BDataColumns::FindReferences(BObject* pobjFind)
{
	ASSERT_VALID(pobjFind);

	ULONG lngPropertyID = pobjFind->GetObjectID();
	if (lngPropertyID) { // could be zero, eg if it's a temporary object
		ASSERT(lngPropertyID); //. bombing on canceling from import icon
		ASSERT(m_aci.GetSize() >= m_nColumns);
		// Walk through columninfo objects and look for references
		for (int i = 0; i < m_nColumns; i++)
		{
			if (lngPropertyID == m_aci[i].m_lngPropertyID)
				return TRUE;
		}
	}
	return FALSE;
}



// Remove any references to the specified object, or replace with new one if specified.
BOOL BDataColumns::ReplaceReferences(BObject* pobjFind, BObject* pobjNew /* = 0 */)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjFind);

	BOOL bFound = FALSE;
	ULONG lngPropertyID = pobjFind->GetObjectID();
	ASSERT(lngPropertyID);
	ASSERT(m_aci.GetSize() >= m_nColumns);

	// Walk through columninfo objects and look for references
	for (int i = 0; i < m_nColumns; i++)
	{
		if (lngPropertyID == m_aci[i].m_lngPropertyID)
		{
			if (pobjNew)
			{
				// Replace the reference with new propertydef
				m_aci[i].m_lngPropertyID = pobjNew->GetObjectID();
				m_aci[i].m_pobjPropertyDef = pobjNew;
				m_aci[i].m_nColWidth = pobjNew->GetPropertyDefWidth();
				m_aci[i].m_nColAlignment = pobjNew->GetPropertyDefAlignment();
			}
			else
			{
				// Remove the reference
				m_aci.RemoveAt(i);
				// Adjust index so can continue through array
				i--;
				m_nColumns--;
			}
			// Set flag
			bFound = TRUE;
		}
	}
	return bFound;
}




// Find the column with the specified PropertyID.
// Returns the zero-based column index, or -1 if not found.
int BDataColumns::GetColumnIndex(ULONG lngPropertyID)
{
	ASSERT(m_nColumns <= m_aci.GetSize());
	for (int i = 0; i < m_nColumns; i++)
	{
		if (lngPropertyID == m_aci[i].m_lngPropertyID)
			return i;
	}
	return -1;
}


// Find the column with the specified propertydef.
// Returns the zero-based column index, or -1 if not found.
int BDataColumns::GetColumnIndex(BObject *pobjPropertyDef)
{
	ASSERT(m_nColumns <= m_aci.GetSize());
	for (int i = 0; i < m_nColumns; i++)
	{
		if (pobjPropertyDef == m_aci[i].m_pobjPropertyDef)
			return i;
	}
	return -1;
}


// Return the PropertyID associated with a column, or 0 if not found
ULONG BDataColumns::GetPropertyID(int nCol)
{
	if (nCol >= 0 && nCol < m_nColumns)
	{
		ASSERT(m_aci.GetSize() >= m_nColumns); // might be bigger
		return m_aci[nCol].m_lngPropertyID;
	}
	return 0;
}



// Get the propertydef associated with the specified column, or 0 if not found.
BObject* BDataColumns::GetPropertyDef(int nCol, BDoc* pDoc)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDoc);

	BObject* pobjPropDef = 0;
	if (nCol >= 0 && nCol < m_nColumns)
	{
		ASSERT(m_aci.GetSize() >= m_nColumns); // might be bigger
		ColumnInfo& rci = GetColumnInfo(nCol);
		// Lookup propertydef object if haven't done already
		if (!rci.m_pobjPropertyDef)
		{
			ULONG lngPropertyID = rci.m_lngPropertyID;
			ASSERT(lngPropertyID);
			rci.m_pobjPropertyDef = pDoc->GetObject(lngPropertyID);
		}
		pobjPropDef = rci.m_pobjPropertyDef;
		// In case we screwed up and the property is missing
		if (!pobjPropDef)
		{
			CString str;
			str.Format(_T("Missing PropertyID %d as specified in BDataColumns - replacing it with Name property"), rci.m_lngPropertyID);
			AfxMessageBox(str);
			rci.m_lngPropertyID = propName;
			pobjPropDef = pDoc->GetObject(propName);
			rci.m_pobjPropertyDef = pobjPropDef;
		}
		ASSERT_VALID(pobjPropDef);
	}
	return pobjPropDef;
}




BOOL BDataColumns::IsValid(BDoc* pDoc)
{
	//, not likely to want to undo deletion of column bdata
	// could fill in code later though
	return FALSE; 
}


