
// BDataFile


#include "precompiled.h"
#include "BDataFile.h"
#include "ConstantsDatabase.h"
#include "BDoc.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



// BUG:: If you put the wrong base class here, then serialization will fail with badClass exception,
// because it tries to create a BData object, and checks to see if the object it creates (ex BDataFile)
// is derived from BData. If you have CObject here for the base class, then it says NO
IMPLEMENT_SERIAL(BDataFile, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataFile::BDataFile()
	// no members to initialize
{
}

BDataFile::~BDataFile()
{
}



BOOL BDataFile::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	ASSERT_VALID(this);
	m_strText = str;
	return TRUE;
}



CString BDataFile::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	ASSERT_VALID(this);
	return m_strText;
}



ULONG BDataFile::GetMemoryUsed(BOOL bRecursive)
{
	ASSERT_VALID(this);
	ULONG nBytes = sizeof(BDataFile);
	nBytes += m_strText.GetLength() * sizeof(TCHAR);
	return nBytes;
}



void BDataFile::Serialize(CArchive &ar)
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



// Create a copy of this object
BData* BDataFile::CreateCopy()
{
	ASSERT_VALID(this);
	BDataFile* pdatCopy = new BDataFile();
	ASSERT_VALID(pdatCopy);
	pdatCopy->m_strText = m_strText;
	return pdatCopy;
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataFile::FindReferences(BObject* pobjFind)
{
	return FALSE;
}


//,, pass to ui
void BDataFile::UIOnClick()
{
	CString strMsg;
	strMsg.Format("Open the file %s?", (LPCTSTR) m_strText);
	if (IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION + MB_YESNO))
	{
		CString strURL = m_strText;
		CWaitCursor wc;
		HINSTANCE h = ::ShellExecute(NULL, "open", strURL, NULL, NULL, SW_SHOWNORMAL);
		Library::HandleShellExecuteError(h);
	}
}



//x
/*
void BDataFile::UIOnMouseMove()
{
	::SetCursor(app.m_hCursorHand);
}
*/

int BDataFile::UIGetCursor() {
	return 1; // hand
}


// Add menu items for bdata value popup
CStringArray& BDataFile::UIGetCommands() {
	static CStringArray cmds; // static so can return reference to it
	if (cmds.IsEmpty()) {
		cmds.Add("&Open File...");
	}
	return cmds;
}


BOOL BDataFile::UIHandleCommand(UINT nCommandID) {
	if (nCommandID == 0) { // open file
		UIOnClick();
		return TRUE;
	}
	return FALSE;
}



// Bring up dialog to edit string value.
// Updates value and returns TRUE if user hit OK in dialog.
BOOL BDataFile::UIEditValue(CUI& ui, BObject* pobj, BObject* pobjPropertyDef)
{
	ASSERT_VALID(this);
	// pobj and pobjpd can be zero

	// okay to edit mstrtext directly because it IS the underlying representation
	return ui.EditString(m_strText);
}



