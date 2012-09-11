
// BDataWebsite


#include "precompiled.h"

#include "BDataWebsite.h"
#include "ConstantsDatabase.h"
#include "BDoc.h"

#include "NeoMem.h"
#include "DialogEditString.h"


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



LPCTSTR BDataWebsite::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
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
BOOL BDataWebsite::UIEditValue(BObject* pobj, BObject* pobjPropertyDef)
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
	return pdatCopy;
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataWebsite::FindReferences(BObject* pobjFind)
{
	return FALSE;
}



void BDataWebsite::UIOnClick()
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
		Library::HandleShellExecuteError(h);
	}
}




void BDataWebsite::UIOnMouseMove()
{
	::SetCursor(theApp.m_hCursorHand);
}



// Add menu items for bdata value popup
BOOL BDataWebsite::UIAddMenuItems(CMenu* pMenu, int nPos)
{
	pMenu->InsertMenu(nPos, MF_BYPOSITION | MF_STRING, ID_POPUP_BDATA_START, "&Open Website...");
	pMenu->InsertMenu(nPos + 1, MF_BYPOSITION | MF_SEPARATOR);
	return TRUE;
}



BOOL BDataWebsite::UIHandleCommand(UINT nCommandID)
{
	if (nCommandID == ID_POPUP_BDATA_START)
	{
		UIOnClick();
		return TRUE;
	}
	return FALSE;
}



