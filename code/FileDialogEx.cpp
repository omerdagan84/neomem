
// CFileDialogEx



#include "precompiled.h"
#include <afxpriv.h>
#include "FileDialogEx.h"

#include "resource.h" // for IDD_SAVE_EXTENSION
//#include "DialogSaveOptions.h"
#include "library.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static BOOL IsWin2000();


///////////////////////////////////////////////////////////////////////////
// CFileDialogEx

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)



CFileDialogEx::CFileDialogEx(BOOL bOpenFileDialog,
							 LPCTSTR lpszDefExt,
							 LPCTSTR lpszFileName,
							 DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName,
			dwFlags, lpszFilter, pParentWnd)
{
	m_pDoc = 0;
	m_bShowOptionButton = TRUE;
}


BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
//{{AFX_MSG_MAP(CFileDialogEx)
ON_COMMAND(IDC_BTN_OPTIONS, OnOptions)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL IsWin2000() 
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;
	
	// Try calling GetVersionEx using the OSVERSIONINFOEX structure,
	// which is supported on Windows 2000.
	// If that fails, try using the OSVERSIONINFO structure.
	
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	
	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}
	
	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		
		if ( osvi.dwMajorVersion >= 5 )
			return TRUE;
		
		break;
	}
	return FALSE; 
}



int CFileDialogEx::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_ofn.Flags & OFN_ENABLEHOOK);
	ASSERT(m_ofn.lpfnHook != NULL); // can still be a user hook

	// zero out the file buffer for consistent parsing later
	ASSERT(AfxIsValidAddress(m_ofn.lpstrFile, m_ofn.nMaxFile));
	DWORD nOffset = lstrlen(m_ofn.lpstrFile)+1;
	ASSERT(nOffset <= m_ofn.nMaxFile);
	memset(m_ofn.lpstrFile+nOffset, 0, 
		(m_ofn.nMaxFile-nOffset)*sizeof(TCHAR));
	
	// WINBUG: This is a special case for the file open/save dialog,
	//  which sometimes pumps while it is coming up but before it has
	//  disabled the main window.
	HWND hWndFocus = ::GetFocus();
	BOOL bEnableParent = FALSE;
	m_ofn.hwndOwner = PreModal();
	AfxUnhookWindowCreate();
	if (m_ofn.hwndOwner != NULL && ::IsWindowEnabled(m_ofn.hwndOwner))
	{
		bEnableParent = TRUE;
		::EnableWindow(m_ofn.hwndOwner, FALSE);
	}
	
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	ASSERT(pThreadState->m_pAlternateWndInit == NULL);
	
	if (m_ofn.Flags & OFN_EXPLORER)
		pThreadState->m_pAlternateWndInit = this;
	else
		AfxHookWindowCreate(this);
	
	//`Add custom child dialog if in Save mode, and allowed to show option button
	// The system uses the specified template to create a dialog box that is a child of the 
	// default Explorer-style dialog box. 
	if (m_bShowOptionButton)
	{
		if ((!m_bOpenFileDialog) && (m_ofn.Flags & OFN_EXPLORER))
		{
			m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_SAVE_EXTENSION);
			m_ofn.Flags |= OFN_ENABLETEMPLATE;
		}
	}
	
	memset(&m_ofnEx, 0, sizeof(m_ofnEx));
	memcpy(&m_ofnEx, &m_ofn, sizeof(m_ofn));
	if (IsWin2000())
		m_ofnEx.lStructSize = sizeof(m_ofnEx);
	
	int nResult;
	if (m_bOpenFileDialog)
		nResult = ::GetOpenFileName((OPENFILENAME*)&m_ofnEx);
	else
		nResult = ::GetSaveFileName((OPENFILENAME*)&m_ofnEx);
	
	memcpy(&m_ofn, &m_ofnEx, sizeof(m_ofn));
	m_ofn.lStructSize = sizeof(m_ofn);
	
	if (nResult)
		ASSERT(pThreadState->m_pAlternateWndInit == NULL);
	pThreadState->m_pAlternateWndInit = NULL;
	
	// WINBUG: Second part of special case for file open/save dialog.
	if (bEnableParent)
		::EnableWindow(m_ofnEx.hwndOwner, TRUE);
	if (::IsWindow(hWndFocus))
		::SetFocus(hWndFocus);
	
	PostModal();
	return nResult ? nResult : IDCANCEL;
}


// For the Explorer-style Open and Save As dialog boxes, the hook procedure does not receive messages 
// intended for the standard controls in the dialog box. Instead, it receives notification messages from 
// the dialog box and messages for any additional controls that you defined in a custom template.
BOOL CFileDialogEx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	memcpy(&m_ofn, &m_ofnEx, sizeof(m_ofn));
	m_ofn.lStructSize = sizeof(m_ofn);
	
	return CFileDialog::OnNotify( wParam, lParam, pResult);
}


////////////////////////////////////////////////////////////////
// The following functions are provided just to demonstrate that they
// get called; ie, that MFC's internal dialog proc is in fact hooked
// up. You can delete them if you like.
BOOL CFileDialogEx::OnFileNameOK()
{
	xTRACE(_T("CFileDialogEx::OnFileNameOK\n"));
	return CFileDialog::OnFileNameOK();
}

void CFileDialogEx::OnInitDone()
{
	xTRACE(_T("CFileDialogEx::OnInitDone\n"));
	CFileDialog::OnInitDone();
}

void CFileDialogEx::OnFileNameChange()
{
	xTRACE(_T("CFileDialogEx::OnFileNameChange\n"));
	CFileDialog::OnFileNameChange();
}

void CFileDialogEx::OnFolderChange()
{
	xTRACE(_T("CFileDialogEx::OnFolderChange\n"));
	CFileDialog::OnFolderChange();
}





// Handle save options button
void CFileDialogEx::OnOptions()
{
	// Set encryption information for the document
	//, might eventually have other options also
	ASSERT_VALID(m_pDoc);
	m_pDoc->UISetEncryption();
}



// get the address of the string corresponding to the
// given index within the 0-delimited filter extension list
// from http://microsoft.ease.lsoft.com/scripts/wa-msn.exe?A2=ind9904e&L=mfc&T=0&P=1081
LPCTSTR CFileDialogEx::GetFilterExtension(int index)
{
	if (index == 0) // 0 isn't legal index
		return 0;
	// point to the beginning of the filter list
	LPCTSTR pStr = m_ofn.lpstrFilter;
	// The literals 7 and 5 have worked since windows 3.0
	// so chances are they still will for awhile
	for (int i = 0; i < index; i++)
    {
		// look for the null at end of descrip
		while(*pStr++)
		{
			if (!*pStr) // 2nd 0 means end of filter list
			{
				if (!pStr[1]) // end of the line!!!
					i = index + 1; // end for loop
				else
					pStr = &pStr[7]; // point to next descrip
				break;
			}
		}
    }
	return pStr-5; // point back to last extension
}



// The notification message is sent when the user selects a new file type from 
// the list of file types in the Open or Save As dialog box.
// from http://microsoft.ease.lsoft.com/scripts/wa-msn.exe?A2=ind9904e&L=mfc&T=0&P=1081
void CFileDialogEx::OnTypeChange()
{
	TRACE(_T("OnTypeChange(), index = %d\n"), m_ofn.nFilterIndex);

	// update the filename by substituting the new extension
	if (!m_bOpenFileDialog) // don't adjust name if opening a file
	{
		CString sName = GetFileName();
		CString sExt;
		
		_splitpath_s(sName, NULL, 0, NULL, 0, NULL, 0, sExt.GetBuffer(_MAX_EXT), _MAX_EXT);
		sExt.ReleaseBuffer();
		sName.TrimRight();
		sExt.TrimRight();
		if (sName.GetLength())
			sName = sName.Left(sName.GetLength() - sExt.GetLength());
		
		sExt = GetFilterExtension(m_ofn.nFilterIndex);
		sName += sExt;
		GetParent()->SendMessage(CDM_SETCONTROLTEXT, (WPARAM)edt1,
			(LPARAM)sName.GetBuffer(_MAX_PATH));
	}

	// Do not call Default() if you override
	// no default processing needed (mfc code does nothing)
//	CFileDialog::OnTypeChange();
}



int CFileDialogEx::GetFilterIndex()
{
	return m_ofn.nFilterIndex;
}


