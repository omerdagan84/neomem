
// BDataString


#include "precompiled.h"
#include "BDataString.h"

#include "ConstantsDatabase.h"
#include "BDoc.h"

//x#include "NeoMem.h"
//x#include "DialogEditString.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



// BUG:: If you put the wrong base class here, then serialization will fail with badClass exception,
// because it tries to create a BData object, and checks to see if the object it creates (ex BDataString)
// is derived from BData. If you have CObject here for the base class, then it says NO!
IMPLEMENT_SERIAL(BDataString, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataString::BDataString()
{
}

BDataString::~BDataString()
{
}



BOOL BDataString::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	ASSERT_VALID(this);
	m_strText = str;
	return TRUE;
}



CString BDataString::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	ASSERT_VALID(this);
	return m_strText;
}



ULONG BDataString::GetMemoryUsed(BOOL bRecursive)
{
	ASSERT_VALID(this);
	ULONG nBytes = sizeof(BDataString);
	nBytes += m_strText.GetLength() * sizeof(TCHAR);
	return nBytes;
}



void BDataString::Serialize(CArchive &ar)
{
	// Call base class first
	BData::Serialize(ar);

	// serialize the string
	if (ar.IsStoring())
	{
		ar << m_strText;
	}
	else
	{
		ar >> m_strText;
	}
}



// Bring up dialog to edit string value.
// Updates value and returns TRUE if user hit OK in dialog.
BOOL BDataString::UIEditValue(BObject* pobj, BObject* pobjPropertyDef, CUI& ui)
{
	ASSERT_VALID(this);
	return ui.EditString(m_strText);
}



// Create a copy of this object
BData* BDataString::CreateCopy()
{
	ASSERT_VALID(this);
	BDataString* pdatCopy = new BDataString();
	ASSERT_VALID(pdatCopy);
	pdatCopy->m_strText = m_strText;
	return pdatCopy;
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataString::FindReferences(BObject* pobjFind)
{
	return FALSE;
}


