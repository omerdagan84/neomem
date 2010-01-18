

// CNeoMem
// This class encapsulates all code associated with the main application,
// which is responsible for registering the window with Windows,
// handling messages from Windows, including menu and toolbar commands, etc.
// Date: November 1999
//-----------------------------------------------------------------------------------------------------------------


#pragma once

#ifndef __AFXWIN_H__
	#error "include 'precompiled.h' before including this file for PCH"
#endif


#include "resource.h"       // main symbols
#include "Error.h"
#include "FileFormat.h"
#include "FileFormats.h"
#include "FontEx.h"
#include "Info.h"
#include "Library.h"
//#include "LogFile.h"
#include "PrintInfoMore.h"
#include "RichEditCtrlEx.h"
#include "RichEditDocEx.h"
#include "Strings.h"
#include "StringEx.h"
#include "Test.h"

#include "GUI.h"


class CFrameChild;
class CFrameMain;
class BObject;



class CNeoMem : public CWinApp
{
// Construction
public:
	CNeoMem();


// Operations
public:
	BOOL BrowseFolder(LPCTSTR szInstructions, CString &strFolder);
	BOOL ConvertPlainToRtf(CString& strPlain, CString& strRtf);
	BOOL ConvertRtfToPlain(LPCTSTR pszRtf, CString& strPlain);
	BOOL CopyToClipboard(CString& str);
	void DoAutoRecoverSave();
	void DoTests();
	void Export(BObject* pobj);
	CImageList* GetAppImageList();
	CFrameChild* GetChildFrame();
	int GetDocCount();
	CFrameMain* GetMainFrame();
	BOOL GetTextFromClipboard(CString& str);
	LPCTSTR GetTip();
	ULONG GetTopObjectID();
	void GotoNextTip();
	void GotoPreviousTip();
	void GotoWebsite();
	BOOL HandleError(CError& e);
	BOOL IsMaximized();
	void LoadStrings();
	void LoadTips();
	void OnFileOpen();
	void ResetAutoRecoverTimer();
	BOOL SaveAllModifiedBackup(CString& strMsg);
	long SearchRtfString(LPCTSTR pszRtfString, LPCTSTR pszSearchFor, BOOL bMatchCase, BOOL bWholeWord);
	void SetStatusBarText(LPCTSTR lpszText = 0);
	UINT ShowMessage(UINT nMessage);
	void ShowWelcomeDialog();
	void UpdateAllDocumentObjects(ULONG lngMsg, BOOL bRecurse = TRUE);
	void UpdateAllDocumentViews(LPARAM lHint, CObject* pHint = 0);
	BOOL UpdateCommonControls();
	void UpdateProgressBar(ULONG nItem, ULONG nItems = 0);


private:
	void LoadImageList();
	void LoadOptions();
	void MoveRegistryToIniFile(CString sInifile);
	void RegisterApplication();
	void SaveOptions();
	void UseIniFile(CString sInifile);


// Attributes
public:
	// These are stored in registry/ini file
	BOOL m_bAdmin; // true if admin privileges available. affects which item is the root in the treeview.
	BOOL m_bDisplayHeader; // option to display the header view
	BOOL m_bDisplayGridlines; // toggle to display gridlines in views
//	BOOL m_bShowTips; // show tips at startup
	BOOL m_bShowWelcome; // show welcome dialog on startup
	BOOL m_bTestMode; // true if in test mode (ie run all unit tests)
	CFontEx m_fontRtfDefault; // default font to use in rtf view
	int m_nUnits; // English (1) or Metric (0) - used by print routines
	CRect m_rectPageMargins; // top, left, right, bottom margins for print in twips
	BOOL m_bPrintHeaderFooter; // print header and footer? 
	CFont m_fontHeader; // font to use in header view
	COLORREF m_clrHeaderBackground; // color of header view background
	COLORREF m_clrHeaderForeground; // color of header view text
	COLORREF m_clrCustomColors[16]; // array of 16 custom colors displayed in Color dialog box
	UINT m_nTip; // current tip number (string resource id)
	UINT m_nNameFormat; // how to display names (0 = jim smith, 1 = smith, jim)
//	UINT m_nDateFormat; // how to display dates (0 = short format, 1 = long format)
	CString m_strDocumentFolder; // folder to store user's documents in
	BOOL m_bAutoLoad; // autoload file on startup?
	CString m_strAutoLoadFileName; // path and name of file to startup with, or blank if none
//	BOOL m_bAddAsSibling; // add new object as sibling of current one
//	BOOL m_bSelectNewObject; // select new object after adding it
	CString m_strTemplateFileName; // name of file to use as template for new documents (no path!)
	BOOL m_bAutoStart; // if true will run neomem on windows startup
	BOOL m_bAutoBackup; // if true will keep backup copy on save
	BOOL m_bAutoRecover; // if true will save recover copy every n minutes
	UINT m_nAutoRecoverMinutes; // number of minutes between AutoRecover saves
//	UINT m_nSearchesToRemember; //, number of search strings to remember in search view
	CString m_strExportFolder;
	CString m_strExportFormat;
	BOOL m_bRegisterFileType; 
	COLORREF m_clrTextViewForecolor;
	COLORREF m_clrTextViewBackcolor;

	COLORREF m_clrHomeTopBack;
	COLORREF m_clrHomeTopBack2;
	COLORREF m_clrHomeMiddleBack;
	COLORREF m_clrHomeBottomBack;
	COLORREF m_clrHomeBottomBack2;
	COLORREF m_clrHomeTopFore;
	COLORREF m_clrHomeMiddleFore;
	COLORREF m_clrHomeMiddleFore2;
	COLORREF m_clrHomeBottomFore;
	COLORREF m_clrHomeDescriptionFore;


	// These aren't stored in registry
	CStringEx m_strApplicationFolder; // folder containing NeoMem.exe
	CStringEx m_strIniFolder; // folder where ini file is stored
	ULONG m_lngSearchExcludeFlags; // Specifies types of objects to exclude from searches
	ULONG m_lngExcludeFlags; // Specifies classes, properties, property types, etc to exclude from various dialogs
	CFontEx m_fontControls; // default font to use for controls (MS Sans Serif 8pt)
	CFontEx m_fontControlsBold; // bold font to use for controls
	CRichEditCtrlEx m_rtf; // Invisible rtf control used by various routines (drag drop, search)
	CInfo m_objInfo; // program version, windows version, memory etc info object
	UINT m_cfObjects; // Clipboard format for collection of objects
	UINT m_cfRtf; // Clipboard format for rich text
	CString m_strTip; // current tip cache
	BOOL m_bOptionsLoaded; // Set to True after options loaded from registry. Prevents error if InitInstance exited prematurely.
	HCURSOR m_hCursorHand; // standard hand cursor used for hyperlinks etc
	ULONG m_lngPrintViewID; // view being print previewed
	BOOL m_bDoingAutoRecover; // set to true during AutoRecover save
	CString m_strTemplatePath; // full path to template file
	BOOL m_bIgnoreLoadHint; // set to true during setcurrentobject hintselect so hintLoad will be ignored during tab select
//	CLogFile* m_plogfile; // for debugging to logfile

	CGUI m_gui; // the ui interface used by the database for callbacks

	// File formats, initialized in InitInstance
	CFileFormat m_ffCsv;
	CFileFormat m_ffTab;
	CFileFormat m_ffRtf;
	CFileFormat m_ffText;
	CFileFormat m_ffHtml;
	CFileFormat m_ffXml;
	CFileFormat m_ffOpml;
	CFileFormats m_listImportFormats; // list of import formats supported
	CFileFormats m_listExportFormats; // list of export formats supported
	CString m_strImportFilter;
	CString m_strExportFilter;

	// Loaded from string resource:
	CString m_strFileExtension; // eg ".neo" - includes period
	CString m_strWebsite;
	CString m_strEmail;
//	CString m_strBuildDate;

	// Tip file loaded from user-defined resource
//	CMemFile m_memTips;

	// Application icons
	int m_nIconArrowUp;
	int m_nIconArrowDown;
	int m_nIconDocument;

	// General purpose text buffers
	TCHAR m_szBuffer256[256];
	TCHAR m_szBuffer512[512];

	// Printing
	CPrintInfoMore m_printinfo;
	CSize m_sizePaper; // size of paper in twips


//. ole 	CRichEditDocEx* m_pRedoc; // pointer to richeditdoc 

	BOOL m_bAppShuttingDown; // Flag set to true on app shut down (set from main frame window)
	BOOL m_bWindowsShuttingDown; // Flag set to true on Windows shut down (set from main frame window)

private:
	CImageList m_imlApp; // application image list
// 1.1	BOOL m_bCrashing; // Set to true if shutting down due to uncaught exception


// debug routines
public: 
	BOOL ShowError(CString strMsg);
#ifdef _DEBUG
	CString GetNotifyCodeString(UINT nCode);
	LPCTSTR GetCodeName(DWORD dwCodeType, DWORD dwCodeID);
	inline LPCTSTR GetHintName(LPARAM lHint) { return GetCodeName(0, (DWORD) lHint); }
#else
	inline LPCTSTR GetHintName(LPARAM lHint) { return ""; };
#endif
	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeoMem)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL SaveAllModified();
	virtual BOOL OnDDECommand(LPTSTR lpszCommand);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual LRESULT ProcessWndProcException(CException* e, const MSG* pMsg);
	//}}AFX_VIRTUAL
	void EnableShellOpen();


// Message Map Handlers
	//{{AFX_MSG(CNeoMem)
	afx_msg void OnUpdateViewHeader(CCmdUI* pCmdUI);
	afx_msg void OnCmdHelpAbout();
	afx_msg void OnCmdViewOptions();
	afx_msg void OnFilePrintSetup();
	afx_msg void OnFilePageSetup();
	afx_msg void OnCmdViewHeader();
	afx_msg void OnCmdViewHeaderFont();
	afx_msg void OnCmdViewHeaderColor();
	afx_msg void OnCmdViewHeaderReset();
	afx_msg void OnCmdHelpWebsite();
	afx_msg void OnCmdHelpEmail();
	afx_msg void OnCmdHelpKeyboard();
	afx_msg void OnCmdHelpTipOfTheDay();
	afx_msg void OnCmdHelpShowWelcome();
	afx_msg void OnCmdHelpWhatIs();
	afx_msg void OnHelpWhatsNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	static CString g_strSpace2;
};





// Globals
//----------------------------------------------------------------------------------------------------------
//, yeesh, fix this

// these are just declarations. see elsewhere for definitions. 

// This lets other modules access the global application object
extern CNeoMem theApp; //, rename to app or gapp


//extern CGUI& gui; // reference to the gui object? can't do that. 
//extern CGUI gui; 
extern CGUI* gpgui; 


// Let other modules access the global strings
extern CString g_strSpace;
extern CString g_strCommaSpace;
extern CString g_strQuote;
extern CString g_strQuoteSpace;



