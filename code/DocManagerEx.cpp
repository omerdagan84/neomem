
////////////////////////////////////////////////////////////////
// MSDN -- August 2000
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably NT too.
// CDocManagerEx overrides one function in CDocManager,
// DoPromptFileName, to use CFileDialogEx for Windows 2000 open dialog.
// To use this class, just add the line
//    m_pDocManager = new CDocManagerEx;
// to your app's InitInstance function, before you add any document
// templates.

// Modified by bpb (made simpler)



#include "precompiled.h"

#include <afxpriv.h>
#include "DocManagerEx.h"
#include "FileDialogEx.h"



#include "NeoDoc.h" //`


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// this used to link fine, but in vs2010 it wouldn't. so copied the function from docmgr.cpp.
// as inspired by this codeproject - 
// "Unfortunately, this code uses the internal MFC function _AfxAppendFilterSuffix. 
// To make the code compile, the only solution I found was to copy the code verbatim from MFC source files. "
// http://www.codeproject.com/Articles/7054/Opening-multipe-documents-of-several-types-at-once

//extern void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn, CDocTemplate* pTemplate, CString* pstrDefaultExt);
AFX_STATIC void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ENSURE_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
		!strFilterExt.IsEmpty() &&
		pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
		!strFilterName.IsEmpty())
	{
		if (pstrDefaultExt != NULL)
			pstrDefaultExt->Empty();

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please

		int iStart = 0;
		do
		{
			CString strExtension = strFilterExt.Tokenize( _T( ";" ), iStart );

			if (iStart != -1)
			{
				// a file based document template - add to filter list

				// If you hit the following ASSERT, your document template 
				// string is formatted incorrectly.  The section of your 
				// document template string that specifies the allowable file
				// extensions should be formatted as follows:
				//    .<ext1>;.<ext2>;.<ext3>
				// Extensions may contain wildcards (e.g. '?', '*'), but must
				// begin with a '.' and be separated from one another by a ';'.
				// Example:
				//    .jpg;.jpeg
				ASSERT(strExtension[0] == '.');
				if ((pstrDefaultExt != NULL) && pstrDefaultExt->IsEmpty())
				{
					// set the default extension
					*pstrDefaultExt = strExtension.Mid( 1 );  // skip the '.'
					ofn.lpstrDefExt = const_cast< LPTSTR >((LPCTSTR)(*pstrDefaultExt));
					ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
				}

				filter += (TCHAR)'*';
				filter += strExtension;
				filter += (TCHAR)';';  // Always append a ';'.  The last ';' will get replaced with a '\0' later.
			}
		} while (iStart != -1);

		filter.SetAt( filter.GetLength()-1, '\0' );;  // Replace the last ';' with a '\0'
		ofn.nMaxCustFilter++;
	}
}



IMPLEMENT_DYNAMIC(CDocManagerEx, CDocManagerEx)


CDocManagerEx::CDocManagerEx()
{
	m_pDoc = 0; //`
}

CDocManagerEx::~CDocManagerEx()
{
}




// Copied from MFC Docmgr.cpp
BOOL CDocManagerEx::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, 
								   BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
//`	CFileDialog dlgFile(bOpenFileDialog);
	CFileDialogEx dlgFile(bOpenFileDialog);
	dlgFile.m_pDoc = m_pDoc; //` save document (if specified) so dialog can access its properties (encryption etc)
	
	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else
	{
		// do for all doc template
		POSITION pos = m_templateList.GetHeadPosition();
		BOOL bFirst = TRUE;
		while (pos != NULL)
		{
			CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
			_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,
				bFirst ? &strDefault : NULL);
			bFirst = FALSE;
		}
	}

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	return nResult == IDOK;
}




BOOL CDocManagerEx::DoPromptFileName2(CString& fileName, UINT nIDSTitle, DWORD lFlags, 
								   BOOL bOpenFileDialog, CDocTemplate* pTemplate, CNeoDoc* pDoc)
{
	ASSERT_VALID(pDoc);
	m_pDoc = pDoc;
	return DoPromptFileName(fileName, nIDSTitle, lFlags, bOpenFileDialog, pTemplate);
}


