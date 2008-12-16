
// CFrameMain
// This class encapsulates all code associated with the main window, which includes 
// the menu, toolbar, status bar, and child frame.
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "StatusBarEx.h"
#include "RichEditCtrlEx.h"
//class CRichEditCtrlEx;
//class CStatusBarEx;
#include "MDITrayFrame.h"

#include "BCMenu.h" // bcmenu



// This is nice, but I think the problem is that #defines use a global namespace, so you can only 
// do this with one file!?
//#define BaseClass CMDIFrameWnd
#define BaseClass CMDITrayFrame


//class CFrameMain : public CMDIFrameWnd
//class CFrameMain : public CMDITrayFrame
class CFrameMain : public BaseClass
{
	DECLARE_DYNAMIC(CFrameMain)

// Construction
public:
	CFrameMain();
	virtual ~CFrameMain();

// Operations
public:
	void SaveWindowPlacement(LPCTSTR szSection, LPCTSTR szEntry);
	BOOL LoadWindowPlacement(LPCTSTR szSection, LPCTSTR szEntry);
	HMENU NewMenu(); // bcmenu
	HMENU NewDefaultMenu(); // bcmenu
	CDocument* GetActiveDocumentMDI();
	BOOL SetAutoRecoverTimer(UINT uMinutes);

// Attributes
public:
	CStatusBarEx m_sbr;
	BCMenu m_menu, m_default; // bcmenu
private:
	CToolBar m_tbr;
	CReBar m_wndReBar;
	CDialogBar m_wndDlgBar;
//	CProgressCtrl m_wndProgressBar;
//	CRichEditCtrlEx m_rtfInvisible; // invisible rtf used for various routines (search, drag drop, etc)
	UINT m_uAutoRecoverTimerID;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameMain)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle); // advanced override



public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CFrameMain)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnCmdViewTest();
	afx_msg void OnTimeChange();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnFontChange();
	afx_msg void OnDevModeChange(LPTSTR lpDeviceName);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnPaletteIsChanging(CWnd* pRealizeWnd);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnViewMinimize();
	afx_msg void OnViewRestore();
	afx_msg void OnDestroy();
	afx_msg void OnEndSession(BOOL bEnding);
	afx_msg void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	//}}AFX_MSG
	afx_msg void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHelp();
//	afx_msg void OnFilePrint();
//	afx_msg void OnFilePrintPreview();

	DECLARE_MESSAGE_MAP()
};




