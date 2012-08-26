
// CRichEditCtrlEx
// Extension of the MFC CRichEditCtrl class, so we can use the richedit 2.0 dll. 
// Also added methods like GetRtf, AutoURLDetect, FindNext, etc. 
// Date: February 2000
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "FontEx.h"
#include "RichOle.h" // LPRICHEDITOLE

//class CRichEditCtrl;


// In the RichEditCtrlEx.h file, you need to add the DLL holder class and 
// function prototype:

/*
class _AFX_RICHEDITEX_STATE
{
public:
    _AFX_RICHEDITEX_STATE();
    virtual ~_AFX_RICHEDITEX_STATE();
    HINSTANCE m_hInstRichEdit20 ;
};
*/

//BOOL PASCAL AfxInitRichEditEx();


class CRichEditCtrlEx : public CRichEditCtrl
{
// Construction
public:
	CRichEditCtrlEx();
	virtual ~CRichEditCtrlEx();

// Operations
public:
	static bool Test();
	void Spike();
	BOOL AutoURLDetect(BOOL bEnable);
	DWORD CanRedo();
	void CenterSelectionVertically();
	long FindNext(CString& strFindText, BOOL bMatchCase, BOOL bWholeWord);
	long FindText2(DWORD dwFlags, FINDTEXT* pFindText);
	BOOL GetCurrentFont(CFontEx& font);
	CString GetRtf(BOOL bSelectionOnly = FALSE);
	DWORD GetSelectionCharFormat2(CHARFORMAT2 &cf2) const;
	BOOL IsAtStartOfLine();
	DWORD Redo();
	void SetDefaultCharFormat2(CHARFORMAT2 cf2); //`
	void SetRtf(LPCTSTR pszRtf, BOOL bSelectionOnly = FALSE);

/* 
private:
	bool CreateFromFile(LPCTSTR lpszFileName);
	void ReleasePointers();
	bool InsertBitmap(CString strFile); // This needs to be the full path to the bitmap. (ex. C:\Windows\Bitmap.bmp)

	LPRICHEDITOLE m_pRichEditOle;
	LPOLEOBJECT m_lpObject;
	LPSTORAGE m_lpStorage;
	LPOLECLIENTSITE m_lpClientSite;
*/

// Implementation
//private:
//DWORD CALLBACK EditStreamCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEditCtrlEx)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rcRect, CWnd* pParentWnd, UINT nID);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CRichEditCtrlEx)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



