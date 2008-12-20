
// BDataWebsite


#include "precompiled.h"

#include "BDataWebsite.h"
#include "ConstantsDatabase.h"
#include "NeoDoc.h"

#include "NeoMem.h"
#include "DialogEditString.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



// BUG:: If you put the wrong base class here, then serialization will fail with badClass exception,
// because it tries to create a BData object, and checks to see if the object it creates (ex BDataWebsite)
// is derived from BData. If you have CObject here for the base class, then it says NO!
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



LPCTSTR BDataWebsite::GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
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
//		int nFileVersion = ar.GetObjectSchema();
		ar >> m_strText;
	}
}




// Bring up dialog to edit string value.
// Updates value and returns TRUE if user hit OK in dialog.
BOOL BDataWebsite::EditValue(BObject* pobj, BObject* pobjPropertyDef)
{
	ASSERT_VALID(this);

	CDialogEditString dlg;
	dlg.m_strValue = m_strText;
	if (dlg.DoModal() == IDOK)
	{
		// Save new string value
		m_strText = dlg.m_strValue;
		return TRUE;
	}
	return FALSE;
}



// Create a copy of this object
BData* BDataWebsite::CreateCopy()
{
	ASSERT_VALID(this);
	BDataWebsite* pdatCopy = new BDataWebsite();
	ASSERT_VALID(pdatCopy);
	pdatCopy->m_strText = m_strText;
	return (BData*) pdatCopy;
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataWebsite::FindReferences(BObject* pobjFind)
{
	return FALSE;
}



void BDataWebsite::OnClick()
{
	CString strMsg;
	strMsg.Format("Open the website %s?", (LPCTSTR) m_strText);
	if (IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION + MB_YESNO))
	{
		CString strURL;
		if (_tcsncmp((LPCTSTR) m_strText, _T("http://"), 7) == 0)
			strURL = m_strText;
		else
			strURL.Format("http://%s", (LPCTSTR) m_strText);
		CWaitCursor wc;
		HINSTANCE h = ::ShellExecute(NULL, "open", strURL, NULL, NULL, SW_SHOWNORMAL);
		HandleShellExecuteError(h);
	}
}




void BDataWebsite::OnMouseMove()
{
	// Windows 95: The width and height of the cursor must be the values returned by the 
	// GetSystemMetrics function for SM_CXCURSOR and SM_CYCURSOR. In addition, either the 
	// cursor bit depth must match the bit depth of the display or the cursor must be monochrome. 
	// If your application must set the cursor while it is in a window, make sure the class cursor 
	// for the specified window's class is set to NULL. If the class cursor is not NULL, the system 
	// restores the class cursor each time the mouse is moved. 
//	::SetCursor(hCursor);
	::SetCursor(theApp.m_hCursorHand);
}



// Add menu items for bdata value popup
BOOL BDataWebsite::AddMenuItems(CMenu* pMenu, int nPos)
{
	pMenu->InsertMenu(nPos, MF_BYPOSITION | MF_STRING, ID_POPUP_BDATA_START, "&Open Website...");
	pMenu->InsertMenu(nPos + 1, MF_BYPOSITION | MF_SEPARATOR);
	return TRUE;
}



BOOL BDataWebsite::HandleCommand(UINT nCommandID)
{
	if (nCommandID == ID_POPUP_BDATA_START)
	{
		OnClick();
		return TRUE;
	}
	return FALSE;
}


