
// BDataBoolean


#include "precompiled.h"

#include "BDataBoolean.h"
#include "ConstantsDatabase.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




IMPLEMENT_SERIAL(BDataBoolean, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataBoolean::BDataBoolean() :
	m_nValue (0)
{
}

BDataBoolean::~BDataBoolean()
{
}


// Set data via text parser
BOOL BDataBoolean::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	// parse text: y/n -> -1/0
	CString strCopy = str.Left(1);
	strCopy.MakeLower();
	if (strCopy == "y")
		m_nValue = -1;
	else if (strCopy == "n")
		m_nValue = 0;
	else
	{
		if (bShowErrorMessage)
			AfxMessageBox("Invalid boolean value. Should be \"yes\" or \"no\". Please try again.");
		return FALSE;
	}
	return TRUE;
}



// Get text representation of data into string
LPCTSTR BDataBoolean::GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	if (m_nValue == 0)
		m_strText = "No";
	else
		m_strText = "Yes";
	return m_strText;
}





void BDataBoolean::Serialize(CArchive &ar)
{
	// Call base class first
	BData::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_nValue;
	}
	else
	{
//		int nFileVersion = ar.GetObjectSchema();
		ar >> m_nValue;
	}
}




// Create a copy of this bdata object
BData* BDataBoolean::CreateCopy()
{
	BDataBoolean* pdat = new BDataBoolean;
	pdat->m_nValue = m_nValue;
	return (BData*) pdat;
}


BOOL BDataBoolean::UIEditValue(BObject* pobj, BObject* pobjPropertyDef)
{
	return FALSE;
}



// Returns True if can find any references to the specified object in this bdata
BOOL BDataBoolean::FindReferences(BObject* pobjFind)
{
	return FALSE;
}


ULONG BDataBoolean::GetMemoryUsed(BOOL bRecursive)
{
	ULONG nBytes = sizeof(BDataBoolean);
	return nBytes;
}

