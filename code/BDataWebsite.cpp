
// BDataWebsite


#include "precompiled.h"

#include "BDataWebsite.h"
#include "ConstantsDatabase.h"
#include "BDoc.h"


#include "UI.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



IMPLEMENT_SERIAL(BDataWebsite, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataWebsite::BDataWebsite()
{
}

BDataWebsite::~BDataWebsite()
{
}



BOOL BDataWebsite::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	ASSERT_VALID(this);
	m_strText = str;
	return TRUE;
}



CString BDataWebsite::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	ASSERT_VALID(this);
	return m_strText;
}



ULONG BDataWebsite::GetMemoryUsed(BOOL bRecursive)
{
	ASSERT_VALID(this);
	ULONG nBytes = sizeof(BDataWebsite);
	nBytes += m_strText.GetLength() * sizeof(TCHAR);
	return nBytes;
}



void BDataWebsite::Serialize(CArchive &ar)
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
BOOL BDataWebsite::UIEditValue(BObject* pobj, BObject* pobjPropertyDef, CUI& ui)
{
	ASSERT_VALID(this);
	return ui.EditString(m_strText);
}



// Create a copy of this object
BData* BDataWebsite::CreateCopy()
{
	ASSERT_VALID(this);
	BDataWebsite* pdatCopy = new BDataWebsite();
	ASSERT_VALID(pdatCopy);
	pdatCopy->m_strText = m_strText;
	return pdatCopy;
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataWebsite::FindReferences(BObject* pobjFind)
{
	return FALSE;
}


//,, ui stuff
void BDataWebsite::UIOnClick()
{
	CString strMsg;
	strMsg.Format("Open the website %s?", (LPCTSTR) m_strText);
	if (IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION + MB_YESNO))
	//,,
//	if (IDYES == ui.MessageBox(strMsg, MB_ICONQUESTION + MB_YESNO))
	{
		CString strURL;
		if (_tcsncmp((LPCTSTR) m_strText, _T("http://"), 7) == 0)
			strURL = m_strText;
		else
			strURL.Format("http://%s", (LPCTSTR) m_strText);

		//, put in ui.h
		CWaitCursor wc;
		HINSTANCE h = ::ShellExecute(NULL, "open", strURL, NULL, NULL, SW_SHOWNORMAL);
		Library::HandleShellExecuteError(h);
	}
}



int BDataWebsite::UIGetCursor() {
	return 1; // hand
}



CStringArray& BDataWebsite::UIGetCommands() {
	static CStringArray cmds; // static so can return reference to it
	if (cmds.IsEmpty()) {
		cmds.Add("&Goto Website...");
	}
	return cmds;
}




BOOL BDataWebsite::UIHandleCommand(UINT nCommandID) {
	if (nCommandID == 0) {
		UIOnClick();
		return TRUE;
	}
	return FALSE;
}



