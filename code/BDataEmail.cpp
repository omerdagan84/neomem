
// BDataEmail


#include "precompiled.h"

#include "BDataEmail.h"
#include "ConstantsDatabase.h"
#include "BDoc.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



// BUG:: If you put the wrong base class here, then serialization will fail with badClass exception,
// because it tries to create a BData object, and checks to see if the object it creates (ex BDataEmail)
// is derived from BData. If you have CObject here for the base class, then it says NO.
IMPLEMENT_SERIAL(BDataEmail, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataEmail::BDataEmail()
	// no members to initialize
{
}

BDataEmail::~BDataEmail()
{
}



BOOL BDataEmail::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	ASSERT_VALID(this);
	m_strText = str;
	return TRUE;
}



CString BDataEmail::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	ASSERT_VALID(this);
	return m_strText;
}



ULONG BDataEmail::GetMemoryUsed(BOOL bRecursive)
{
	ASSERT_VALID(this);
	ULONG nBytes = sizeof(BDataEmail);
	nBytes += m_strText.GetLength() * sizeof(TCHAR);
	return nBytes;
}



void BDataEmail::Serialize(CArchive &ar)
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
BOOL BDataEmail::UIEditValue(BObject* pobj, BObject* pobjPropertyDef, CUI& ui)
{
	ASSERT_VALID(this);

	return ui.EditString(m_strText);
}



// Create a copy of this object
BData* BDataEmail::CreateCopy()
{
	ASSERT_VALID(this);
	BDataEmail* pdatCopy = new BDataEmail();
	ASSERT_VALID(pdatCopy);
	pdatCopy->m_strText = m_strText;
	return pdatCopy;
}


// Returns True if can find any references to the specified object in this bdata
BOOL BDataEmail::FindReferences(BObject* pobjFind)
{
	return FALSE;
}


//,, pass to ui
void BDataEmail::UIOnClick()
{
	CString strMsg;
	strMsg.Format("Send email to %s?", (LPCTSTR) m_strText);
	if (IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION + MB_YESNO))
	{
		CString strURL;
		if (_tcsncmp((LPCTSTR) m_strText, _T("mailto:"), 7) == 0)
			strURL = m_strText;
		else
			strURL.Format("mailto:%s", (LPCTSTR) m_strText);
		CWaitCursor wc;
		HINSTANCE h = ::ShellExecute(NULL, "open", strURL, NULL, NULL, SW_SHOWNORMAL);			
		Library::HandleShellExecuteError(h);
	}
}



//x
/*
void BDataEmail::UIOnMouseMove()
{
	// Windows 95: The width and height of the cursor must be the values returned by the 
	// GetSystemMetrics function for SM_CXCURSOR and SM_CYCURSOR. In addition, either the 
	// cursor bit depth must match the bit depth of the display or the cursor must be monochrome. 
	// If your application must set the cursor while it is in a window, make sure the class cursor 
	// for the specified window's class is set to NULL. If the class cursor is not NULL, the system 
	// restores the class cursor each time the mouse is moved. 
	::SetCursor(app.m_hCursorHand);
}
*/
int BDataEmail::UIGetCursor() {
	return 1; // hand
}



CStringArray& BDataEmail::UIGetCommands() {
	static CStringArray cmds; // static so can return reference to it
	if (cmds.IsEmpty()) {
		cmds.Add("&Send Email...");
	}
	return cmds;
}


BOOL BDataEmail::UIHandleCommand(UINT nCommandID) {
	if (nCommandID == 0) { // send email
		UIOnClick();
		return TRUE;
	}
	return FALSE;
}



