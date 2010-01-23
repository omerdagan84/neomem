
// BDataFile


#include "precompiled.h"
#include "BDataFile.h"
#include "ConstantsDatabase.h"
#include "NeoDoc.h"

#include "NeoMem.h"
#include "DialogEditString.h"
#include "FileDialogEx.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



// BUG:: If you put the wrong base class here, then serialization will fail with badClass exception,
// because it tries to create a BData object, and checks to see if the object it creates (ex BDataFile)
// is derived from BData. If you have CObject here for the base class, then it says NO!
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



LPCTSTR BDataFile::GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
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
//		int nFileVersion = ar.GetObjectSchema();
		ar >> m_strText;
	}
}




// Bring up dialog to select a file
// Updates value and returns TRUE if user hit OK in dialog.
BOOL BDataFile::UIEditValue(BObject* pobj, BObject* pobjPropertyDef)
{
	ASSERT_VALID(this);

/*	CDialogEditString dlg;
	dlg.m_strValue = m_strText;
	if (dlg.DoModal() == IDOK)
	{
		// Save new string value
		m_strText = dlg.m_strValue;
		return TRUE;
	}
*/

	// Initialize file dialog
//	CFileDialogEx dlg(FALSE, _T("rtf"), m_strFilename, 
	CFileDialogEx dlg(FALSE, "", m_strText, NULL);
	CString strCaption = _T("Select file to link to");
	dlg.m_ofn.lpstrTitle = strCaption;
	dlg.m_bShowOptionButton = FALSE; // because exporting, don't need save options!

	// Get export filename
	if (dlg.DoModal() == IDOK)
	{
		m_strText = dlg.GetPathName(); // full/path/test.rtf
		return TRUE;
	}
	
	return FALSE;
}



// Create a copy of this object
BData* BDataFile::CreateCopy()
{
	ASSERT_VALID(this);
	BDataFile* pdatCopy = new BDataFile();
	ASSERT_VALID(pdatCopy);
	pdatCopy->m_strText = m_strText;
	return (BData*) pdatCopy;
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataFile::FindReferences(BObject* pobjFind)
{
	return FALSE;
}



void BDataFile::UIOnClick()
{
	CString strMsg;
	strMsg.Format("Open the file %s?", (LPCTSTR) m_strText);
	if (IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION + MB_YESNO))
	{
//		CString strURL;
//		if (_tcsncmp((LPCTSTR) m_strText, _T("file://"), 7) == 0)
//			strURL = m_strText;
//		else
//			strURL.Format("file://%s", (LPCTSTR) m_strText);
		CString strURL = m_strText;
		CWaitCursor wc;
		HINSTANCE h = ::ShellExecute(NULL, "open", strURL, NULL, NULL, SW_SHOWNORMAL);
		HandleShellExecuteError(h);
	}
}




void BDataFile::UIOnMouseMove()
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
BOOL BDataFile::UIAddMenuItems(CMenu* pMenu, int nPos)
{
	pMenu->InsertMenu(nPos, MF_BYPOSITION | MF_STRING, ID_POPUP_BDATA_START, "&Open File...");
	pMenu->InsertMenu(nPos + 1, MF_BYPOSITION | MF_SEPARATOR);
	return TRUE;
}



BOOL BDataFile::UIHandleCommand(UINT nCommandID)
{
	if (nCommandID == ID_POPUP_BDATA_START)
	{
		UIOnClick();
		return TRUE;
	}
	return FALSE;
}



