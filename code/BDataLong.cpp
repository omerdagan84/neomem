
// BDataLong


#include "precompiled.h"
#include "BDataLong.h"
#include "ConstantsDatabase.h"

#include "NeoMem.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




IMPLEMENT_SERIAL(BDataLong, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataLong::BDataLong() :
	m_lngValue (0)
{
}

BDataLong::~BDataLong()
{
}


// Set data via text parser
BOOL BDataLong::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	// Note: CString includes a special method which will provide an LPCTSTR to the string data
	m_lngValue = atol(str);
	return TRUE;
}


// Get text representation of data into string
LPCTSTR BDataLong::GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	m_strText.Format("%d", m_lngValue);
	return m_strText;
}



void BDataLong::Serialize(CArchive &ar)
{
	// Call base class first
	BData::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_lngValue;
	}
	else
	{
		ar >> m_lngValue;
	}
}


// Create a copy of this bdata object
BData* BDataLong::CreateCopy()
{
	BDataLong* pdat = new BDataLong();
	pdat->m_lngValue = m_lngValue;
	return (BData*) pdat;
}



// Returns True if can find any references to the specified object in this bdata
BOOL BDataLong::FindReferences(BObject* pobjFind)
{
	return FALSE;
}



ULONG BDataLong::GetMemoryUsed(BOOL bRecursive)
{
	ULONG nBytes = sizeof(BDataLong);
	return nBytes;
}






//, this will bring up a calculator dialog to let them
// enter amount and select units from combo.
// for now bring up windows calculator
// (same as bdatanumber)
BOOL BDataLong::UIEditValue(BObject* pobj, BObject* pobjPropertyDef)
{
	CWaitCursor wc;
	HINSTANCE h = ::ShellExecute(NULL, "open", "Calc.exe", NULL, NULL, SW_SHOWNORMAL);
	HandleShellExecuteError(h);
	return FALSE;
}

