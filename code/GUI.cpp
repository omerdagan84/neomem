

#include "precompiled.h"

#include "GUI.h"



#include "DialogEditName.h"
#include "FileDialogEx.h"




CGUI::CGUI() {
}

CGUI::~CGUI() {
}



bool CGUI::GetFileName(LPCTSTR strTitle, LPCTSTR strExtension, LPCTSTR strFilter, CString& strFilename) {

	// Bring up file open dialog to choose file
//x	CFileDialogEx dlg(TRUE, strExtension, _T(""), OFN_HIDEREADONLY, strFilter, AfxGetMainWnd());
	CFileDialog dlg(TRUE, strExtension, _T(""), OFN_HIDEREADONLY, strFilter, AfxGetMainWnd());
	dlg.m_ofn.lpstrTitle = strTitle;
	if (dlg.DoModal() == IDCANCEL)
		return false;
	strFilename = dlg.GetPathName();
	return true;
}



bool CGUI::GetString(LPCTSTR strTitle, LPCTSTR strInstructions, CString& strString) {
	
	CDialogEditName dlg;
	dlg.m_strCaption = strTitle;
	dlg.m_strInstructions = strInstructions;
	dlg.m_strName = strString;
	if (dlg.DoModal() == IDCANCEL)
		return false;
	strString = dlg.m_strName;
	return true;
}



