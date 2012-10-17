

#include "precompiled.h"
#include "UI.h"

#include "Brooklyn.h"

//, #include "Dialogs.h"  ?
#include "DialogEditDate.h"
#include "DialogEditString.h"
#include "DialogEditLink.h"
#include "DialogEditPersonName.h"

#include "FileDialogEx.h"
#include "NeoMem.h" // m_pmainwnd



CUI::CUI() {
}


CUI::~CUI() {
}





//----------------------------------





//x
/*


// The code in the case stmts used to be spread out through the BData classes,
// but in separating the db from the ui needed to remove it here.



BOOL CUI::EditDate(BObject* pobj, OBJID idProperty) {
	ASSERT_VALID(pobj);
	ASSERT(idProperty);

	BDataDate* pdat = DYNAMIC_DOWNCAST(BDataDate, pobj->GetPropertyData(idProperty, TRUE));

	// If storing a string bring up edit string dialog
//x	if (m_bitsFlags.Type == flagString)
	if (pdat->IsStringDate()) {
		CDialogEditString dlg;
//x		dlg.m_strValue = m_strText;
		dlg.m_strValue = pdat->GetBDataText(NULL, 0); //, params not used or needed - fix
		if (dlg.DoModal() == IDOK)
		{
			// Save new string value
//x			m_strText = dlg.m_strValue;
			pdat->SetBDataText(dlg.m_strValue); //, this is where you can set flag for showing error/interactive dialog or not. what should we do with it? 
			// This also sets the document modified flag and updates views.
			pobj->SetPropertyData(idProperty, pdat);
			return TRUE;
		}
		return FALSE;
	}

	// Get current date/time if date is empty or invalid
//x	if (m_odt.GetStatus() != COleDateTime::valid) {
//		m_odt = COleDateTime::GetCurrentTime();
//	}

	if (!pdat->IsValid())
		pdat->SetDate(COleDateTime::GetCurrentTime());


	COleDateTime odt = pdat->GetDate();

	// Check if year is before 1753
	if (odt.GetYear() < 1753) {
		if (IDNO == AfxMessageBox("Warning: The calendar control does not handle dates before "
									"September 1, 1752 due to the change from the Julian to the Gregorian "
									"calendar. You can still use the date dialog but you won't be able to select "
									"dates prior to this date. \n"
									"Continue?", MB_YESNO + MB_ICONQUESTION))
			return FALSE;
	}
	// Check if any flags are set
//x	else if (
//		(m_bitsFlags.Modifiers != 0 && m_bitsFlags.Modifiers != flagUseTime) || 
//		((m_bitsFlags.Type != 0) && (m_bitsFlags.Type != flagBlank))
//		)
	else if (pdat->AreFlagsSet())
	{
		if (IDNO == AfxMessageBox("Warning: By selecting a new value with the Edit Date dialog you will lose "
								"any current modifiers (e.g. After, Before, Year Only, Year Month, etc.). \n"
								"Continue?", MB_YESNO + MB_ICONQUESTION))
			return FALSE;
	}


	CDialogEditDate dlg;
	dlg.m_dtDate = odt;
	dlg.m_dtTime = odt;
//x	dlg.m_bUseTime = FALSE; // default is to just enter date
//x	dlg.m_bUseTime = m_bitsFlags.Modifiers && flagUseTime;
	dlg.m_bUseTime = pdat->UseTime();
	if (dlg.DoModal() == IDOK)
	{
		// Save new date and time
		COleDateTime& odtDate = dlg.m_dtDate;
		COleDateTime& odtTime = dlg.m_dtTime;
		int nRet = odt.SetDateTime(odtDate.GetYear(), odtDate.GetMonth(), odtDate.GetDay(),
									odtTime.GetHour(), odtTime.GetMinute(), odtTime.GetSecond());
		if (nRet == 0) 
		{
			pdat->SetDate(odt);

			// Set flags
			pdat->ResetFlags(dlg.m_bUseTime);
//x			m_bitsFlags.Type = 0;
//			m_bitsFlags.Relationship = 0;
//			m_bitsFlags.Modifiers = dlg.m_bUseTime ? flagUseTime : 0;
//			m_bitsFlags.Season = 0;


			// This also sets the document modified flag and updates views.
			pobj->SetPropertyData(idProperty, pdat);

			// Note: BObject's UIEditValue will set document modified flag and update all views
			return TRUE;
		}
		AfxMessageBox("Error in setting date. Please try again.", MB_ICONINFORMATION);
	}
	return FALSE;
}





// Bring up dialog to edit string value.
// Updates value and returns TRUE if user hit OK in dialog.
BOOL CUI::EditEmail(BObject* pobj, OBJID idProperty) {

	CDialogEditString dlg;
//x	dlg.m_strValue = m_strText;
	dlg.m_strValue = pobj->GetPropertyString(idProperty); //,   ,TRUE!
	if (dlg.DoModal() == IDOK)
	{
		// Save new string value
//x		m_strText = dlg.m_strValue;
		pobj->SetPropertyString(idProperty, dlg.m_strValue);
		return TRUE;
	}
	return FALSE;
}





// Bring up dialog to select a file
// Updates value and returns TRUE if user hit OK in dialog.
BOOL CUI::EditFile(BObject* pobj, OBJID idProperty)
{
	// Initialize file dialog
	CString strText = pobj->GetPropertyString(idProperty);

	//, ex version not working
//	CFileDialogEx dlg(FALSE, "", strText, NULL);
	CFileDialog dlg(FALSE, "", strText, NULL);
	CString strCaption = _T("Select file to link to");
	dlg.m_ofn.lpstrTitle = strCaption;
//x	dlg.m_bShowOptionButton = FALSE; // because exporting, don't need save options!

	// Get export filename
	if (dlg.DoModal() == IDOK) {
//x		m_strText = dlg.GetPathName(); // full/path/test.rtf
		strText = dlg.GetPathName(); // full/path/test.rtf
		pobj->SetPropertyString(idProperty, strText);
		return TRUE;
	}
	
	return FALSE;
}



BOOL CUI::EditFolder(BObject* pobj, OBJID idProperty)
{
	CString strPath = pobj->GetPropertyString(idProperty);
	if (BrowseFolder(_T("Select folder to link to:"), strPath))
	{
		if (!strPath.IsEmpty())
		{
//x			m_strText = strPath;
			pobj->SetPropertyString(idProperty, strPath);
			return TRUE;
		}
	}
	return FALSE;
}




BOOL CUI::EditString(BObject* pobj, OBJID idProperty) {
	//, will want to say the object we're editing and the property name
	// Object: System
	// Property: Description
	//, for most string properties, we should use cdialogeditname
	// for description, etc. use the bigger one.
	// for now leave like this

	CDialogEditString dlg;
	dlg.m_strValue = pobj->GetPropertyString(idProperty); //,   ,TRUE!
	if (dlg.DoModal() == IDOK)
	{
		// Save new string value
		// This also sets the document modified flag and updates views.
		pobj->SetPropertyString(idProperty, dlg.m_strValue);
		return TRUE;
	}
	return FALSE;
}



//----------------


// Edit object's property value in a dialog
BOOL CUI::EditValue(BObject* pobj, OBJID idProperty) {
	ASSERT_VALID(pobj);
	ASSERT(idProperty);

	CWaitCursor cw;

	BDoc* pdoc = pobj->GetDoc();
	BObject* pobjPropdef = pdoc->GetObject(idProperty);
	
	// Check if property is read-only
	if (pobjPropdef->GetFlag(flagPropValueReadOnly)) {
		AfxMessageBox("This property value is read-only and cannot be changed.", MB_ICONINFORMATION);
		return FALSE;
	}


	OBJID idProptype = pobjPropdef->GetPropertyLink(propPropertyType);

	switch (idProptype) {
		case proptypeDate:
			return EditDate(pobj, idProperty);
			break;
		case proptypeEmail:
			return EditEmail(pobj, idProperty);
			break;
		case proptypeFile:
			return EditFile(pobj, idProperty);
			break;
		// flags - no
		case proptypeFolder:
			return EditFolder(pobj, idProperty);
			break;
		case proptypeString:
			return EditString(pobj, idProperty);
			break;
		default:
			AfxMessageBox("missing case stmt in cui.editvalue");
			ASSERT(0);
			return FALSE;
	}

	return FALSE;
}

*/




// Callback used by BrowseFolder
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
	TCHAR pszDir[MAX_PATH];
	switch (uMsg) {
		// Indicates the browse dialog box has finished initializing. The lParam value is zero. 
		case BFFM_INITIALIZED:
			// BFFM_SETSELECTION selects the specified folder. 
			// To use a PIDL to specify the folder, set the message's lParam to the PIDL, 
			// and set wParam to FALSE. 
			// To specify the folder's path, set the message's lParam value to point to a 
			// NULL-terminated string with the path, and set wParam to TRUE. 
			if (::GetCurrentDirectory (sizeof(pszDir) / sizeof(TCHAR), pszDir))
			{
				::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM) pszDir);
			}
			break;
                  
		// Indicates the selection has changed. The lParam parameter points to the item identifier list for the newly selected item. 
		case BFFM_SELCHANGED: 
			break;
		default:
			break;
	}
	return 0;
}



// Browse for a folder. 
// Returns True if user hit OK, with name of folder in cstring.
// See also Browse for a Folder the Non-COM Way Brian Hart in MSDN
// Note callback fn above
BOOL CUI::BrowseFolder(LPCTSTR pszInstructions, CString& strFolder) {
	BOOL bOK = FALSE;

	LPMALLOC pMalloc;
	// Get the Shell's default allocator
	if (::SHGetMalloc(&pMalloc) == NOERROR) {
		BROWSEINFO bi;
		char pszBuffer[MAX_PATH];

		// this doesn't work - how do you have it select a folder?
//		LPTSTR szFolder = strFolder.GetBuffer(0);
//		strcpy(pszBuffer, szFolder);
//		strFolder.ReleaseBuffer();

		LPITEMIDLIST pidl;
		// Get help on BROWSEINFO struct - it's got all the bit settings.
//		bi.hwndOwner = GetSafeHwnd();
//		bi.hwndOwner = m_pMainWnd->GetSafeHwnd();
		bi.hwndOwner = app.m_pMainWnd->GetSafeHwnd();
		bi.pidlRoot = NULL;
		bi.pszDisplayName = pszBuffer;
		bi.lpszTitle = pszInstructions;
		bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
//		bi.lpfn = NULL;
		bi.lpfn = BrowseCallbackProc;
		bi.lParam = 0;
		// This next call issues the dialog box
		if ((pidl = ::SHBrowseForFolder(&bi)) != NULL) {
			if (::SHGetPathFromIDList(pidl, pszBuffer)) { 
				// At this point pszBuffer contains the selected path
				strFolder = pszBuffer;
				bOK = TRUE;
			}
			// Free the PIDL allocated by SHBrowseForFolder
			pMalloc->Free(pidl);
		}
		// Release the shell's allocator
		pMalloc->Release();
	}
	return bOK;
}



BOOL CUI::EditString(CString& str) {
	CDialogEditString dlg;
	dlg.m_strValue = str;
	if (dlg.DoModal() == IDOK)
	{
		// Save new string value
		str = dlg.m_strValue;
		return TRUE;
	}
	return FALSE;
}


int CUI::MessageBox(CString& str, UINT nType) {
	return AfxMessageBox(str, nType);
}


BOOL CUI::GetDate(COleDateTime& odt, BOOL& bUseTime) {

	CDialogEditDate dlg;
	dlg.m_dtDate = odt;
	dlg.m_dtTime = odt;
	dlg.m_bUseTime = bUseTime;

	if (dlg.DoModal() == IDOK) {
		// Save new date and time
		COleDateTime& dtDate = dlg.m_dtDate;
		COleDateTime& dtTime = dlg.m_dtTime;
		COleDateTime odtDialog;
		int nRet = odtDialog.SetDateTime(dtDate.GetYear(), dtDate.GetMonth(), dtDate.GetDay(),
									dtTime.GetHour(), dtTime.GetMinute(), dtTime.GetSecond());
		if (nRet == 0) {
			odt = odtDialog;
			bUseTime = dlg.m_bUseTime;
//x			// Set flags?
//			m_bitsFlags.Type = 0;
//			m_bitsFlags.Relationship = 0;
//			m_bitsFlags.Modifiers = dlg.m_bUseTime ? flagUseTime : 0;
//			m_bitsFlags.Season = 0;
			return TRUE;
		}
		AfxMessageBox("Error in setting date. Please try again.", MB_ICONINFORMATION);
		return FALSE;
	}
	// user hit cancel
	return FALSE;
}



BOOL CUI::EditLink(CString strCaption, CString strInstructions, BOOL bMultiSelectVisible, 
								 BOOL bMultiSelectEnabled, BOOL bMultiSelectOn, ULONG lngStartID, 
								 BDataLink* pdatLink, ULONG lngExcludeFlags /* = 0 */, 
								 BOOL bIncludeStart /* = FALSE */) {

	CDialogEditLink dlg;
	dlg.m_nHelpID = IDD_EDIT_LINK; //, use sethelpid
	if (dlg.DoModalLink(strCaption, strInstructions, bMultiSelectVisible, bMultiSelectEnabled, bMultiSelectOn, 
									lngStartID, pdatLink, app.m_lngExcludeFlags) == IDOK)
	{
		return TRUE;
	}
	return FALSE;
}



// Edit a numeric value. Just bring up calculator for now. 
//, 
BOOL CUI::EditNumber() {
	CWaitCursor wc;
	HINSTANCE h = ::ShellExecute(NULL, "open", "Calc.exe", NULL, NULL, SW_SHOWNORMAL);
	Library::HandleShellExecuteError(h);
	return FALSE;
}



BOOL CUI::EditName(CString& strTitle, CString& strFirst, CString& strMiddle, 
					CString& strNickname, CString& strLast, CString& strSuffix) {

	CDialogEditPersonName dlg;

	dlg.m_strTitle = strTitle;
	dlg.m_strFirst = strFirst;
	dlg.m_strMiddle = strMiddle;
	dlg.m_strNickname = strNickname;
	dlg.m_strLast = strLast;
	dlg.m_strSuffix = strSuffix;

	// Bring up simple dialog to edit name parts
	if (dlg.DoModal() == IDOK)
	{
		// Save new string values
		strTitle = dlg.m_strTitle;
		strFirst = dlg.m_strFirst;
		strMiddle = dlg.m_strMiddle;
		strNickname = dlg.m_strNickname;
		strLast = dlg.m_strLast;
		strSuffix = dlg.m_strSuffix;

		return TRUE;
	}
	return FALSE;
}
