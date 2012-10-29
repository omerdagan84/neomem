
// BDataFolder


#include "precompiled.h"
#include "BDataFolder.h"
#include "ConstantsDatabase.h"
#include "BDoc.h"

#include "NeoMem.h"
#include "DialogEditString.h"
#include "library.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



IMPLEMENT_SERIAL(BDataFolder, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataFolder::BDataFolder()
{
}

BDataFolder::~BDataFolder()
{
}



BOOL BDataFolder::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	ASSERT_VALID(this);
	m_strText = str;
	return TRUE;
}



CString BDataFolder::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	ASSERT_VALID(this);
	return m_strText;
}



ULONG BDataFolder::GetMemoryUsed(BOOL bRecursive)
{
	ASSERT_VALID(this);
	ULONG nBytes = sizeof(BDataFolder);
	nBytes += m_strText.GetLength() * sizeof(TCHAR);
	return nBytes;
}



void BDataFolder::Serialize(CArchive &ar)
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


//x
/*
// Bring up dialog to select a Folder
// Updates value and returns TRUE if user hit OK in dialog.
BOOL BDataFolder::UIEditValue(BObject* pobj, BObject* pobjPropertyDef)
{
	ASSERT_VALID(this);

	CString strPath = m_strText;
	if (app.BrowseFolder(_T("Select folder to link to:"), strPath))
	{
		if (!strPath.IsEmpty())
		{
			m_strText = strPath;
			return TRUE;
		}
	}
	return FALSE;
}
*/


// Create a copy of this object
BData* BDataFolder::CreateCopy()
{
	ASSERT_VALID(this);
	BDataFolder* pdatCopy = new BDataFolder();
	ASSERT_VALID(pdatCopy);
	pdatCopy->m_strText = m_strText;
	return pdatCopy;
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataFolder::FindReferences(BObject* pobjFind)
{
	return FALSE;
}



//,, pass to ui
void BDataFolder::UIOnClick()
{
	CString strMsg;
	strMsg.Format("Open the folder %s?", (LPCTSTR) m_strText);
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
void BDataFolder::UIOnMouseMove()
{
	::SetCursor(app.m_hCursorHand);
}
*/
int BDataFolder::UICursorOnMouseover() {
	return 1; // hand
}

//x
/*
// Add menu items for bdata value popup
BOOL BDataFolder::UIAddMenuItems(CMenu* pMenu, int nPos)
{
	pMenu->InsertMenu(nPos, MF_BYPOSITION | MF_STRING, ID_POPUP_BDATA_START, "&Open Folder...");
	pMenu->InsertMenu(nPos + 1, MF_BYPOSITION | MF_SEPARATOR);
	return TRUE;
}
*/

CStringArray& BDataFolder::UICommands() {
	static CStringArray cmds; // static so can return reference to it
	if (cmds.IsEmpty()) {
		cmds.Add("&Open Folder...");
	}
	return cmds;
}


BOOL BDataFolder::UIHandleCommand(UINT nCommandID)
{
//x	if (nCommandID == ID_POPUP_BDATA_START)
	if (nCommandID == 0) { // open folder
		UIOnClick();
		return TRUE;
	}
	return FALSE;
}



