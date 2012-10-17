
// BDataFlags


#include "precompiled.h"
#include "BDataFlags.h"
#include "ConstantsDatabase.h"
#include "BDoc.h"
#include "BObject.h"

#include "NeoMem.h"
#include "DialogEditLink.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif





IMPLEMENT_SERIAL(BDataFlags, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataFlags::BDataFlags() :
	m_lngFlags (0)
{
}

BDataFlags::~BDataFlags()
{
}


// Set data via text parser
BOOL BDataFlags::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	//, For now, just convert text to integer.
	char* pStopChar;
	m_lngFlags = strtoul(str, &pStopChar, 10);
	return TRUE;
}



// Get text representation of data into string
//, this could look for values in a system folder containing flag value objects
CString BDataFlags::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	// Show numbers if in admin, for now
	if (app.m_bAdmin)
//		m_strText.Format("%u (%x): ", m_lngFlags, m_lngFlags);
//		m_strText.Format("0x%x: ", m_lngFlags);
		m_strText.Format("%d: ", m_lngFlags);
	else
		m_strText.Empty();

	if (m_lngFlags & flagSystem) m_strText += "System, ";
	if (m_lngFlags & flagAdminOnly) m_strText += "Admin Only, ";
	if (m_lngFlags & flagNoDelete) m_strText += "No Delete, ";
	if (m_lngFlags & flagNoModify) m_strText += "No Modify, ";
	if (m_lngFlags & flagNoAutosort) m_strText += "Autosort Off, ";
	if (m_lngFlags & flagNoModifyClass) m_strText += "No Modify Class, ";
	if (m_lngFlags & flagPropValueReadOnly) m_strText += "Read-Only, ";

	// Remove last comma and space
	if (!m_strText.IsEmpty())
		m_strText = m_strText.Left(m_strText.GetLength() - 2);

	return m_strText;
}



void BDataFlags::Serialize(CArchive &ar)
{
	// Call base class first
	BData::Serialize(ar);

	// serialize the data
	if (ar.IsStoring())
	{
		ar << m_lngFlags;
	}
	else
	{
		ar >> m_lngFlags;
	}

}




//x
/*
BOOL BDataFlags::UIEditValue(BObject* pobj, BObject* pobjPropertyDef)
{
	return FALSE;
}
*/



BData* BDataFlags::CreateCopy()
{
	BDataFlags* pdatCopy = new BDataFlags;
	pdatCopy->m_lngFlags = m_lngFlags;
	return pdatCopy;
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataFlags::FindReferences(BObject* pobjFind)
{
	return FALSE;
}


ULONG BDataFlags::GetMemoryUsed(BOOL bRecursive)
{
	ULONG nBytes = sizeof(BDataFlags);
	return nBytes;
}



// Convert a string representation of flags to numeric
// static
ULONG BDataFlags::StringToFlags(LPCTSTR pszText) {
	static BDataFlags dat;
	dat.SetBDataText(pszText);
	ULONG lngFlags = dat.GetFlags();
	return lngFlags;
}

// Convert a numeric representation of flags to a string
// static
CString BDataFlags::FlagsToString(ULONG lngFlags) {
	static BDataFlags dat;
	dat.SetFlags(lngFlags);
	LPCTSTR pszFlags = dat.GetBDataText(NULL, 0);
	return pszFlags;
}


