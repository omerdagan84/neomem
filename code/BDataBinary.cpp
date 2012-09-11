
// BDataBinary


#include "precompiled.h"

#include "BDataBinary.h"
#include "ConstantsDatabase.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




IMPLEMENT_SERIAL(BDataBinary, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataBinary::BDataBinary() :
	m_pData (0),
	m_nBytes (0)
{
}

// Delete any memory allocated on heap
BDataBinary::~BDataBinary() {
	if (m_pData) {
		delete m_pData;
		m_pData = 0;
	}
}


// Set data via text parser
BOOL BDataBinary::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */) {
	// Don't have much need for this. 
  // If this was a file name then we could read the file name.
	// Otherwise, don't have much use for SetText with binary data,
	// unless it were mime format or something.
//	m_lngValue = atol( (LPCTSTR) str);
	return FALSE;
}


// Get text representation of data into string
LPCTSTR BDataBinary::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion) {
	m_strText.Format(_T("[Binary Data - %d bytes]"), sizeof(m_nBytes));
	return m_strText;
}



//, when start using this class will need to implement this method
void BDataBinary::Serialize(CArchive &ar) {

	// Call base class first
	BData::Serialize(ar);

	if (ar.IsStoring()) {
//		ar << m_lngSize;
//		ar.Write(m_pData, m_lngSize);
	}
	else {
//		int nFileVersion = ar.GetObjectSchema();
//		ar >> m_lngSize;
		// allocate m_lngSize bytes at m_pData
		// verify that right number of bytes was read
//		ar.Read(m_pData, m_lngSize);
	}
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataBinary::FindReferences(BObject* pobjFind) {
	return FALSE;
}



ULONG BDataBinary::GetMemoryUsed(BOOL bRecursive) {
	ULONG nBytes = sizeof(BDataBinary);
	nBytes += m_nBytes;
	return nBytes;
}


