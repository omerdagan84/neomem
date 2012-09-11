// CFileDialogEx: Encapsulate Windows-2000 style open dialog, 
// and allow customization of File Save dialog.
//-------------------------------------------------------------------------------------------------------
// MSDN — August 2000
// If this code works, it was written by Paul DiLascia. If not, I don't
// know who wrote it. Compiles with Visual C++ 6.0, runs on Windows 98 
// and probably Windows NT too.
//-------------------------------------------------------------------------------------------------------


#pragma once


#include "BDoc.h" //` for neodoc reference


// Windows 2000 version of OPENFILENAME.
// The new version has three extra members.
// This is copied from commdlg.h
struct OPENFILENAMEEX : public OPENFILENAME 
{ 
	void *        pvReserved;
	DWORD         dwReserved;
	DWORD         FlagsEx;
};


class CFileDialogEx : public CFileDialog 
{
	DECLARE_DYNAMIC(CFileDialogEx)

// Construction
public: 
	CFileDialogEx(BOOL bOpenFileDialog, // TRUE for open, FALSE for FileSaveAs
						LPCTSTR lpszDefExt = NULL,
						LPCTSTR lpszFileName = NULL,
						DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						LPCTSTR lpszFilter = NULL,
						CWnd* pParentWnd = NULL);

// Operations
	virtual int DoModal();
	LPCTSTR GetFilterExtension(int index);

// Attributes
public:
	int GetFilterIndex();
	BDoc* m_pDoc; //` pointer to document
	BOOL m_bShowOptionButton; //` true if want to show option button in save mode (eg set false on export file)
protected:
	OPENFILENAMEEX m_ofnEx; // new Windows 2000 version of OPENFILENAME

// Implementation
protected: 
	// virtual fns that handle various notifications
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnFileNameOK();
	virtual void OnInitDone();
	virtual void OnFileNameChange();
	virtual void OnFolderChange();
	virtual void OnTypeChange();

	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CFileDialogEx)
	afx_msg void OnOptions();
	//}}AFX_MSG
};


