
// CDocManagerEx — extension to CDocManager that uses CFileOpenEx.
// MSDN — August 2000
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably Windows NT too.



#pragma once

#include "BDoc.h" //`


class CDocManagerEx : public CDocManager 
{
	DECLARE_DYNAMIC(CDocManagerEx)

// Construction
public:
	CDocManagerEx();
	~CDocManagerEx();

// Operations
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
	virtual BOOL DoPromptFileName2(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate, BDoc* pDoc);

// Attributes
private:
	BDoc* m_pDoc; //` pointer to document, specified in call to DoPromptFileName2

};


