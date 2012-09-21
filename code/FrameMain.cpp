
// CFrameMain


#include "precompiled.h"

#include "FrameMain.h"
#include "DialogException.h"
#include "DialogNewFile.h"
#include "DialogWelcome.h"
#include "FrameChild.h"
#include "NeoMem.h"
#include "StatusBarEx.h"
#include "Strings.h"
#include "ViewHeader.h"
#include "ViewRtf.h"

#include "ConstantsDatabase.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// Macros
//---------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(CFrameMain, BaseClass)

BEGIN_MESSAGE_MAP(CFrameMain, BaseClass)
	//{{AFX_MSG_MAP(CFrameMain)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_CHAR()
	ON_WM_INITMENU()
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_VIEW_TEST, OnCmdViewTest)
	ON_WM_TIMECHANGE()
	ON_WM_SETTINGCHANGE()
	ON_WM_FONTCHANGE()
	ON_WM_DEVMODECHANGE()
	ON_WM_PALETTECHANGED()
	ON_WM_PALETTEISCHANGING()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_QUERYENDSESSION()
	ON_COMMAND(ID_VIEW_MINIMIZE, OnViewMinimize)
	ON_COMMAND(ID_VIEW_RESTORE, OnViewRestore)
//	ON_COMMAND(ID_VIEW_SET_CLOCK_FORMAT, OnSetClockFormat)
	ON_WM_DESTROY()
	ON_WM_ENDSESSION()
	ON_WM_TIMER()
	ON_COMMAND(ID_CANCEL, OnCancel)
	ON_WM_MEASUREITEM()
	ON_WM_MENUCHAR()
	//}}AFX_MSG_MAP

	// Global help commands
	// Notice that all of these commands are mapped to member functions of class 
	// CMDITrayFrame (in the case of an MDI application), or to CFrameWnd (in the case 
	// of an SDI application). Unlike most of the other commands you place into the message map, 
	// these have handler functions that are predefined by the class library. Making the 
	// message-map entry enables the command.
	// The application’s accelerator table defines F1 for ID_HELP and SHIFT+F1 for ID_CONTEXT_HELP.
	
	// Responds to the Help Topics item on the Help menu by displaying the Windows Contents screen. 
	ON_COMMAND(ID_HELP_FINDER, BaseClass::OnHelpFinder) 
	
	// Responds to F1 by displaying a specific topic in Windows Help.
	ON_COMMAND(ID_HELP, OnHelp)
	
	// Responds to SHIFT+F1 by putting the application into Help mode.
	ON_COMMAND(ID_CONTEXT_HELP, BaseClass::OnContextHelp)
	
	// Used when a specific help context cannot be found.
	ON_COMMAND(ID_DEFAULT_HELP, BaseClass::OnHelpFinder)


	// Print handlers
//	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()


/*
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
*/



// Construction/Destruction
//---------------------------------------------------------------------------

CFrameMain::CFrameMain() :
	m_uAutoRecoverTimerID(0)
{
}

CFrameMain::~CFrameMain()
{
	xTRACE("cframemain destructor\n");
}



int CFrameMain::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// This routine creates all the child windows (toolbar, statusbar, rebar, dialogbar)
	
	// A control-bar object uses information about its parent window’s client rectangle 
	// to position itself. Then it alters the parent’s remaining client-window rectangle 
	// so that the client view or MDI client window fills the rest of the client window.

	if (BaseClass::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create ToolBar
	if (m_tbr.CreateEx(this) && m_tbr.LoadToolBar(IDR_MAINFRAME))
	{
		// Set button style to toggle 
//		int nIndex = m_tbr.CommandToIndex(ID_FILE_SELECTED_TEXT_ONLY);
//		if (nIndex != -1)
//			m_tbr.SetButtonStyle(nIndex, TBBS_CHECKBOX);

		// Add strings
//		CToolBarCtrl& rtbr = m_tbr.GetToolBarCtrl();
//		//rtbr.AddStrings("Add...\0Classes...\0Properties...\0");
//		TBBUTTONINFO tbbi;
//		tbbi.cbSize = sizeof(TBBUTTONINFO);
//		tbbi.dwMask = TBIF_TEXT;
//		tbbi.pszText = "Add...";
//		rtbr.SetButtonInfo(ID_OBJ_ADD, &tbbi);


		// Turn on Tool Tips
		m_tbr.SetBarStyle(m_tbr.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	}
	else return -1;

	// Create ReBar and add ToolBar to it
	if (!m_wndReBar.Create(this) || !m_wndReBar.AddBar(&m_tbr))
		return -1;

	// Create Status Bar
	// See CStatusBarEx
	if (!m_sbr.Create(this))
		return -1;

	// Initialize system tray icon
//	TraySetIcon(IDR_MAINFRAME);
//	TraySetToolTip("NeoMem - click to restore");
//	TraySetMenu(IDR_SYSTEM_TRAY);


	//. ole - Create the richeditdoc
//	theApp.m_pRedoc = new CRichEditDocEx();

	return 0;
}


BOOL CFrameMain::PreCreateWindow(CREATESTRUCT& cs)
{
	// Modify the Window class or styles here by modifying the CREATESTRUCT cs
//	cs.style |= FWS_ADDTOTITLE;
//	cs.style |= FWS_PREFIXTITLE;
	if( !BaseClass::PreCreateWindow(cs) )
		return FALSE;
//	cs.style &= ~FWS_ADDTOTITLE;
//	cs.style |= FWS_ADDTOTITLE;
//	cs.style |= FWS_PREFIXTITLE;
	return TRUE;
//	return BaseClass::PreCreateWindow(cs);
}



// Diagnostics
//---------------------------------------------------------------------------

#ifdef _DEBUG
void CFrameMain::AssertValid() const
{
	BaseClass::AssertValid();
}

void CFrameMain::Dump(CDumpContext& dc) const
{
	BaseClass::Dump(dc);
}

#endif //_DEBUG



// Message Handlers
//---------------------------------------------------------------------------


/* 
void CFrameMain::OnFilePrint()
{
	CWaitCursor cw;
	xTRACE("CFrameMain OnFilePrint - send message to viewrtf\n");
	CFrameChild* pChild = (CFrameChild*) MDIGetActive();
	if (pChild)
	{
		CView* pView = pChild->GetView(viewText);
		if (pView)
			pView->SendMessage(WM_COMMAND, ID_FILE_PRINT, 0);
	}
}
*/


/* 
void CFrameMain::OnFilePrintPreview()
{
	CWaitCursor cw;
	xTRACE("CFrameMain OnFilePrintPreview - send message to viewrtf\n");

	// pass command to rtf view handler

	// Get rtf view and send message to it
	CFrameChild* pChild = (CFrameChild*) MDIGetActive();
	if (pChild)
	{
		CView* pView = pChild->GetView(viewText);
		if (pView)
			pView->SendMessage(WM_COMMAND, ID_FILE_PRINT_PREVIEW, 0);
	}

}
*/



/*
void CFrameMain::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	BOOL bDoc = (MDIGetActive() != NULL);
	pCmdUI->Enable(bDoc);
}

void CFrameMain::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	BOOL bDoc = (MDIGetActive() != NULL);
	pCmdUI->Enable(bDoc);	
}
*/




// Load the window placement info from the registry
BOOL CFrameMain::LoadWindowPlacement(LPCTSTR szSection, LPCTSTR szEntry)
{
	UINT nBytes;
	BYTE* pData;
	if (theApp.GetProfileBinary(szSection, szEntry, &pData, &nBytes))
	{
		if (pData)
		{
			WINDOWPLACEMENT* pwp = (WINDOWPLACEMENT*) pData;
			// If window was saved in minimized state, start it in maximized!
			if (pwp->showCmd == SW_SHOWMINIMIZED)
				pwp->showCmd = SW_MAXIMIZE;
			SetWindowPlacement(pwp);
			delete pData;
			return TRUE;
		}
	}
	return FALSE;
}


// Save the window placement info to the registry
void CFrameMain::SaveWindowPlacement(LPCTSTR szSection, LPCTSTR szEntry)
{
	WINDOWPLACEMENT wp;
	if (GetWindowPlacement(&wp))
		theApp.WriteProfileBinary(szSection, szEntry, (LPBYTE) &wp, sizeof(WINDOWPLACEMENT));
}








/*
// we could copy the accelerator table, and then add the view & letters to it,
// translating them to commands
// would need a map from the command id to a pView
// would need to reserve space in resource.h for the command ids
// framechild could handle the commands in a range handler
// looking up the pView in the map from the command,
// and setting it as the active view
// this could tie in with the view menu as well -
// eg View/Data would have the same command id, and would be handled in the same way by the
// child frame
//* so childframe would handle creating the accel table, a map from cmdid to pview, and the dynamic menu
BOOL CFrameMain::PreTranslateMessage(MSG* pMsg) 
{
//	if (pMsg->message = WM_SYSCHAR)
//	{
//		xTRACE("syschar!!!!!\n");
//	}
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) 
	{
		xTRACE("CFrameMain PreTranslateMessage Key Msg: %d %d %d %d\n", pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
		// Translate the message using accelerator table
		// TranslateAccelerator does all the magic to translate WM_KEYUP and WM_KEYDOWN 
		// messages into WM_COMMAND messages when the keys pressed match commands 
		// in the accelerator table.
//		ASSERT(m_hAccel);
//		return ::TranslateAccelerator(m_hWnd, m_hAccel, pMsg);
	}
	
	return BaseClass::PreTranslateMessage(pMsg);
}
*/



void CFrameMain::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	xTRACE("CFrameMain OnChar %d\n", nChar);	
	BaseClass::OnChar(nChar, nRepCnt, nFlags);
}


void CFrameMain::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	xTRACE("CFrameMain OnSysChar %d\n", nChar);	
	BaseClass::OnSysChar(nChar, nRepCnt, nFlags);
}



//DEL void CFrameMain::OnPaint() 
//DEL {
//DEL 	// Note: This must be here even though we don't use it, otherwise
//DEL 	// region remains invalidated
//DEL 	CPaintDC dc(this); // device context for painting
//DEL 	
//DEL 	CString strFilename;
//DEL 	CString strTitle = _T("NeoMem");
//DEL 	BDoc* pDoc = (BDoc*) GetActiveDocument();
//DEL 	if (pDoc)
//DEL 	{
//DEL 		strFilename = pDoc->GetTitle();
//DEL 		strTitle = strFilename + _T(" - ") + strTitle;
//DEL 	}
//DEL 	SetWindowText(strTitle);
//DEL /*
//DEL 	BOOL bMaximized;
//DEL 	CFrameChild* pChild = (CFrameChild*) MDIGetActive(&bMaximized);
//DEL 	if (pChild)
//DEL 	{
//DEL //		pChild->GetTitle();
//DEL 		if (bMaximized)
//DEL 			pChild->SetWindowText(_T(""));
//DEL 		else
//DEL 			pChild->SetWindowText(strFilename);
//DEL 	}
//DEL */
//DEL 	// Do not call BaseClass::OnPaint() for painting messages
//DEL }



void CFrameMain::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	BaseClass::OnUpdateFrameTitle(bAddToTitle);
/*	
	CMDIChildWnd* pActiveChild = NULL;
	CDocument* pDoc = GetActiveDocument();
	LPCTSTR lpszDocName = NULL;
	if (pDoc)
		lpszDocName = pDoc->GetTitle();
//	if (bAddToTitle &&
//	  (pActiveChild = MDIGetActive()) != NULL &&
//	  (pActiveChild->GetStyle() & WS_MAXIMIZE) == 0 &&
//	  (pDoc != NULL ||
//	  (pDoc = pActiveChild->GetActiveDocument()) != NULL))
//			lpszDocName = pDoc->GetTitle();
//	else
//	{
//		LPCTSTR lpstrTitle = NULL;
//		CString strTitle;
//		if (pActiveChild != NULL)
//		{
//			strTitle = pActiveChild->GetTitle();
//			if (!strTitle.IsEmpty())
//				lpstrTitle = strTitle;
//		}
//		lpszDocName = lpstrTitle;
//	}

	// copy first part of title loaded at time of frame creation
	TCHAR szText[256+_MAX_PATH];
	szText[0] = '\0';   // start with nothing
	// get name of currently active view
	if (lpszDocName != NULL)
	{
		lstrcpy(szText, lpszDocName);
		lstrcat(szText, _T(" - "));
	}
	lstrcat(szText, m_strTitle);
	// set title if changed, but don't remove completely
	// Note: will be excessive for MDI Frame with maximized child
	AfxSetWindowText(m_hWnd, szText);
*/
//	AfxSetWindowText(m_hWnd, "biya - hiya");
}



void CFrameMain::OnInitMenu(CMenu* pMenu) 
{
	BaseClass::OnInitMenu(pMenu);
}




// CWinApp help::
// The default implementation of this message-handler function determines the Help 
// context that corresponds to the current window, dialog box, or menu item and then 
// calls WINHELP.EXE. If no context is currently available, the function uses the default context.
// Override this member function to set the Help context to something other than the 
// window, dialog box, menu item, or toolbar button that currently has the focus. 
// Call WinHelp with the desired Help context ID.
void CFrameMain::OnHelp()
{
	// OnHelp just brings up the help topic appropriate for the current window, whatever that is.
	// Note: All the frame version of OnHelp does is call CWinApp's version.
	BaseClass::OnHelp();
}



// Note: This gets called when time changes, not when date/time format changes
void CFrameMain::OnTimeChange() 
{
	BaseClass::OnTimeChange();
	// Refresh date in status bar
	RedrawWindow();
}



// This is broadcast to all top level windows when a system setting changes,
// such as date/time format
void CFrameMain::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	BaseClass::OnSettingChange(uFlags, lpszSection);

	// Refresh date in status bar
	RedrawWindow();

}



void CFrameMain::OnFontChange() 
{
	BaseClass::OnFontChange();
	//, reload list of available fonts in rtf view toolbar
}


// This is the WM_DEVMODECHANGE handler, which the framework sends to all top level
// windows when the user changes device mode settings (eg change paper orientation, print quality, etc).
// lpDeviceName Points to the device name specified in the Windows initialization file, WIN.INI.
// The base class calls the app's DevModeChange method and then broadcasts the
// WM_DEVMODECHANGE message to all child windows. 
void CFrameMain::OnDevModeChange(LPTSTR lpDeviceName) 
{
	BaseClass::OnDevModeChange(lpDeviceName);
}


void CFrameMain::OnPaletteChanged(CWnd* pFocusWnd) 
{
	BaseClass::OnPaletteChanged(pFocusWnd);
	// Add your message handler code here
}


void CFrameMain::OnPaletteIsChanging(CWnd* pRealizeWnd) 
{
	BaseClass::OnPaletteIsChanging(pRealizeWnd);	
	// Add your message handler code here
}


// This is the handler for the WM_SYSCOLORCHANGE message
void CFrameMain::OnSysColorChange() 
{
	BaseClass::OnSysColorChange();
	
	// Get system colors into static variables
	Library::LoadColors();
	
	// Redraw all windows
	RedrawWindow();	
}


// Windows is trying to shut down
BOOL CFrameMain::OnQueryEndSession() 
{
	if (!BaseClass::OnQueryEndSession())
		return FALSE;
		
	return TRUE;
}



// The framework calls this member function to inform the CWnd object that it is being destroyed. 
// OnDestroy is called after the CWnd object is removed from the screen (hidden). 
// OnDestroy is called first for the CWnd being destroyed, then for the child windows of CWnd 
// as they are destroyed. It can be assumed that all child windows still exist while OnDestroy runs. 
void CFrameMain::OnDestroy() 
{
	xTRACE("cframemain ondestroy\n");

	BaseClass::OnDestroy();

	// Add your message handler code here

	// Kill existing AutoRecover timer, if any
	if (m_uAutoRecoverTimerID)
		KillTimer(m_uAutoRecoverTimerID);
}


// This gets called when Windows is shutting down
void CFrameMain::OnEndSession(BOOL bEnding) 
{
	BaseClass::OnEndSession(bEnding);

	// Set flag so app will know Windows is shutting down
	theApp.m_bWindowsShuttingDown = bEnding;
}




BOOL CFrameMain::DestroyWindow() 
{
	// Add your specialized code here and/or call the base class
	xTRACE("cframemain destroywindow\n");
	
	return BaseClass::DestroyWindow();
}








// Minimize NeoMem to the System Tray
void CFrameMain::OnViewMinimize() 
{
//	AddIcon(1, IDR_MAINFRAME, "NeoMem - click to restore", IDR_SYSTEM_TRAY);
	AddIcon(1, IDR_SYSTRAY, "NeoMem - click to restore", IDR_POPUP_SYSTEM_TRAY);
	this->ShowWindow(SW_HIDE);
//	TrayShow();	
//	TraySetIcon(IDR_MAINFRAME);
//	TraySetToolTip("NeoMem - click to restore");
//	TraySetMenu(IDR_SYSTEM_TRAY);
}


void CFrameMain::OnViewRestore() 
{
//	this->ShowWindow(SW_RESTORE);
	this->ShowWindow(SW_SHOW);
//	TrayHide();
	HideIcon(1);
}



// Called from system tray popup (ID_CANCEL handler)
void CFrameMain::OnCancel() 
{
	// Do nothing 
}






// Create AutoRecover timer. call with 0 minutes to kill it. 
// See also: OnTimer
BOOL CFrameMain::SetAutoRecoverTimer(UINT uMinutes)
{
	// Kill existing timer
	if (m_uAutoRecoverTimerID)
		KillTimer(m_uAutoRecoverTimerID);

	if (uMinutes == 0)
	{
		// no more timer
		m_uAutoRecoverTimerID = 0;
	}
	else
	{
		// Create new timer
		// Timers are a limited global resource; therefore it is important that an application check the 
		// value returned by the SetTimer member function to verify that a timer is actually available.
		UINT uMilliseconds = uMinutes * 60 * 1000;
		m_uAutoRecoverTimerID = SetTimer(1, uMilliseconds, NULL);
		if (m_uAutoRecoverTimerID == 0)
		{
			AfxMessageBox("No timers available for AutoRecover feature!");
			return FALSE;
		}
	}

	return TRUE;
}




void CFrameMain::OnTimer(UINT nIDEvent) 
{
	// AutoRecover timer
	// Bug: This was firing during debugging and walking through load code - 
	// could also possibly fire during loading of a large file. So added flag.
  //, removed it
//	if (!theApp.m_bDoingFileLoad)
//	{
		if (nIDEvent == m_uAutoRecoverTimerID)
			theApp.DoAutoRecoverSave();
//	}
	BaseClass::OnTimer(nIDEvent);
}





HMENU CFrameMain::NewMenu() // bcmenu
{
//	static UINT toolbars[] = {
//				IDR_MAINFRAME, 
//				IDR_TOOLBAR_CONTENTS,
//				IDR_TOOLBAR_PROPERTIES,
//				IDR_TOOLBAR_RTF,
//				IDR_TOOLBAR_SEARCH
//				};

	// Load the menu from the resources
	// *replace IDR_MENUTYPE with your menu ID*
	m_menu.LoadMenu(IDR_NEOMEM_TYPE);

	// One method for adding bitmaps to menu options is through the LoadToolbars member function. 
	// This method allows you to add all the bitmaps in a toolbar object to menu options (if they exist). 
	// The first function parameter is an array of toolbar id's. 
	// The second is the number of toolbar id's. 
	// There is also a function called LoadToolbar that just takes an id.
//	m_menu.LoadToolbars(toolbars, 1);
	m_menu.LoadToolbar(IDR_MAINFRAME);
	m_menu.LoadToolbar(IDR_TOOLBAR_CONTENTS);
	m_menu.LoadToolbar(IDR_TOOLBAR_PROPERTIES);
	m_menu.LoadToolbar(IDR_TOOLBAR_RTF);
	m_menu.LoadToolbar(IDR_TOOLBAR_SEARCH);
	m_menu.LoadToolbar(IDR_TOOLBAR_OTHER);

	return(m_menu.Detach());
}


HMENU CFrameMain::NewDefaultMenu() // bcmenu
{
	m_menuDefault.LoadMenu(IDR_MAINFRAME);
	m_menuDefault.LoadToolbar(IDR_MAINFRAME);
	return (m_menuDefault.Detach());
}


// This handler ensure that the popup menu items are drawn correctly. // bcmenu
void CFrameMain::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	BOOL setflag=FALSE;
	if (lpMeasureItemStruct->CtlType==ODT_MENU)
	{
		if (IsMenu((HMENU) lpMeasureItemStruct->itemID))
		{
			CMenu* cmenu = CMenu::FromHandle((HMENU)lpMeasureItemStruct->itemID);
			if (m_menu.IsMenu(cmenu) || m_menuDefault.IsMenu(cmenu))
			{
				m_menu.MeasureItem(lpMeasureItemStruct);
				setflag=TRUE;
			}
		}
	}
	if (!setflag) BaseClass::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

// This handler ensures that keyboard shortcuts work. // bcmenu
LRESULT CFrameMain::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	LRESULT lresult;
	if (m_menu.IsMenu(pMenu) || m_menuDefault.IsMenu(pMenu))
		lresult = BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lresult = BaseClass::OnMenuChar(nChar, nFlags, pMenu);
	return (lresult);
}



void CFrameMain::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
//	xTRACE("CFrameMain OnInitMenuPopup %d\n", nIndex);
//	BaseClass::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);	
	// actually, could add all available views in app or create frame,
	// then update handler could disable view if it's not avail for the current object,
	// and check it if it's visible
	// that way, you could hide views if they were in the way,
	// and you could add views if you wanted
	// for v1.0, view changes would apply globally, or to classes of objects
//	if (nIndex == 3)
//	{
//		xTRACE("  modify view menu!\n");
//		pPopupMenu->InsertMenu(ID_VIEW_VIEWS, MF_BYCOMMAND | MF_STRING, 
//													ID_VIEW_SEARCH, _T("Search"));
//	}
	// This handler updates the menus from time to time. // bcmenu
	BaseClass::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	if (!bSysMenu)
	{
		if (m_menu.IsMenu(pPopupMenu) || m_menuDefault.IsMenu(pPopupMenu))
			BCMenu::UpdateMenu(pPopupMenu);
	}
}





// Bug: GetActiveDocument function doesn't work for MDI frames
//* Use this instead of GetActiveDocument
// GetActiveDocument calls GetActiveView, which returns NULL when called for an MDI main 
// frame window. In an MDI application, the MDI main frame window does not have a view 
// associated with it. Instead, each individual child window (CMDIChildWnd) has one or more 
// associated views. The active view in an MDI application can be obtained by first finding 
// the active MDI child window and then finding the active view for that child window. The 
// active MDI child window can be found by calling the function MDIGetActive or GetActiveFrame.
CDocument* CFrameMain::GetActiveDocumentMDI()
{
	CMDIChildWnd* pChild = MDIGetActive();
	if (!pChild)
		return NULL;
	CDocument* pDoc = pChild->GetActiveDocument();	
	if (!pDoc)
		return NULL;
	// Fail if doc is of wrong kind
//	if (!pDoc->IsKindOf(RUNTIME_CLASS(BDoc)))
//		return NULL;
	return pDoc;
}




void CFrameMain::OnClose() 
{
	yTRACE("CFrameMain::OnClose()\n");

	// Save active document as the auto reload file
	if (theApp.m_bAutoLoad)
	{
		// Bug: Used GetActiveDocument which doesn't work for MDI
		CDocument* pDoc = GetActiveDocumentMDI(); 
		if (pDoc)
		{
			theApp.m_strAutoLoadFileName = pDoc->GetPathName();
			yTRACE("    AutoLoad file set to \"%s\"\n", (LPCTSTR) theApp.m_strAutoLoadFileName);
		}
	}

	// Save position to registry
	SaveWindowPlacement(Strings::szSettings, Strings::szWindowPosition);

	// Set flag in the app so it knows we're closing
	theApp.m_bAppShuttingDown = TRUE;

	// Asks if you want to save the file and destroys the frame window
//	BaseClass::OnClose();

	// Delete the richeditdoc object
	//. ole - access violation here - insert a link in rtf, close without saving
//	if (theApp.m_pRedoc)
//		delete theApp.m_pRedoc;


	// This code is copied from MFC's CFrameWnd::OnClose() - Winfrm.cpp........
	//.........................

	if (m_lpfnCloseProc != NULL && !(*m_lpfnCloseProc)(this))
		return;

	// Note: only queries the active document
	CDocument* pDocument = GetActiveDocument();
	if (pDocument != NULL && !pDocument->CanCloseFrame(this))
	{
		// document can't close right now -- don't close it
		return;
	}
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL && pApp->m_pMainWnd == this)
	{
		// attempt to save all documents
		if (pDocument == NULL && !pApp->SaveAllModified())
			return;     // don't close it

		// hide the application's windows before closing all the documents
		pApp->HideApplication();

		// close all documents first
		pApp->CloseAllDocuments(FALSE);

		// don't exit if there are outstanding component objects
		if (!AfxOleCanExitApp())
		{
			// take user out of control of the app
			AfxOleSetUserCtrl(FALSE);

			// don't destroy the main window and close down just yet
			//  (there are outstanding component (OLE) objects)
			return;
		}

		// there are cases where destroying the documents may destroy the
		//  main window of the application.
		if (!afxContextIsDLL && pApp->m_pMainWnd == NULL)
		{
			AfxPostQuitMessage(0);
			return;
		}
	}

	// detect the case that this is the last frame on the document and
	// shut down with OnCloseDocument instead.
	if (pDocument != NULL && pDocument->m_bAutoDelete)
	{
		BOOL bOtherFrame = FALSE;
		POSITION pos = pDocument->GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = pDocument->GetNextView(pos);
			ASSERT_VALID(pView);
			if (pView->GetParentFrame() != this)
			{
				bOtherFrame = TRUE;
				break;
			}
		}
		if (!bOtherFrame)
		{
			pDocument->OnCloseDocument();
			return;
		}

		// allow the document to cleanup before the window is destroyed
		pDocument->PreCloseFrame(this);
	}

	// destroy the window
	//.? this is where the program stops
	DestroyWindow();

	//.........................

}




// this is called by ctrl+shift+alt+T (hidden command)
void CFrameMain::OnCmdViewTest() 
{

	if (IDYES == AfxMessageBox("Run tests?", MB_YESNO))
	{
		// this will throw a memory exception
//		AfxThrowMemoryException();

		BObject* pobj = 0;
//		pobj->GetName(); // just does ASSERT_VALID(pobj) in here
		ASSERT_VALID(pobj);
		ULONG lngX = pobj->GetObjectID(); // will throw an unhandled exception. 

		// Note: This doesn't get called!
//		AfxMessageBox("After exception");
	}


//	AfxMessageBox("save autorecover info");
//	OnTimer(m_uAutoRecoverTimerID);
//	return;

//	CDialogException dlg;
//	dlg.DoModal();
//	AfxMessageBox("After exception");

//	CDialogNewFile dlg;
//	dlg.DoModal();
//	theApp.m_strRegistrationKey = "";
//	AfxMessageBox("registration key reset to ''");

/*
	// Show list of views attached to the current document
	CDocument* pDoc = GetActiveDocument();
	POSITION pos = pDoc->GetFirstViewPosition();
	CRuntimeClass* pClass;
	CString str = "Views attached to current document:\n";
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		pClass = pView->GetRuntimeClass();
		str += CString(pClass->m_lpszClassName) + "\n";
	}   
	CWnd* pWnd = AfxGetMainWnd()->GetActiveWindow();
	CString strActive;
	pClass = pWnd->GetRuntimeClass();
	strActive.Format("\n\nActive window class: %s\n", (LPCTSTR) CString(pClass->m_lpszClassName));
	str += strActive;
	AfxMessageBox(str);
*/

}



