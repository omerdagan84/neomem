
// CFrameChild
 

#include "precompiled.h"

#include "FrameChild.h"
#include "Constants.h"
#include "DialogEditObject.h"
#include "DialogExport.h"
#include "DialogPrint.h"
#include "NeoMem.h"
#include "Strings.h"
#include "ViewHeader.h"
#include "ViewRtf.h"
#include "ViewTabs.h"
#include "ViewTree.h"

#include "BObject.h"
#include "ConstantsDatabase.h"
#include "NeoDoc.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




// Macros
//----------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CFrameChild, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CFrameChild, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFrameChild)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_IMPORT, OnCmdFileImport)
	ON_COMMAND(ID_FILE_EXPORT, OnCmdFileExport)
	ON_COMMAND(ID_VIEW_NEXT, OnCmdViewNext)
	ON_COMMAND(ID_VIEW_PREVIOUS, OnCmdViewPrevious)
	ON_COMMAND(ID_OBJ_EDIT_IN_PLACE, OnObjEditInPlace) //ok
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEW_FIRST, ID_VIEW_LAST, OnCmdView)
//	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_FIRST, ID_VIEW_LAST, OnUpdateView)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_FROM_PREVIEW, OnFilePrintFromPreview)
	ON_COMMAND_RANGE(ID_NAVIGATE_START, ID_NAVIGATE_STOP, OnCmdNavigate)
//.	ON_COMMAND(ID_OBJ_ADD, OnCmdAddObject)
END_MESSAGE_MAP()



// Construction/Destruction
//----------------------------------------------------------------------------

CFrameChild::CFrameChild()
{
	m_pviewNavigation = 0;
	m_pviewHeader = 0;
}

CFrameChild::~CFrameChild()
{
}


BOOL 
CFrameChild::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;
//	cs.style &= ~FWS_ADDTOTITLE;
//	cs.style |= FWS_ADDTOTITLE;
//	cs.style |= FWS_PREFIXTITLE;
	return TRUE;
}


// Called by the framework during the execution of OnCreate. 
// The default implementation of this function creates a CView object from the information 
// provided in pContext, if possible.
// Override this function to override values passed in the CCreateContext object or to 
// change the way controls in the main client area of the frame window are created. 
// The CCreateContext members you can override are described in the CCreateContext class.
// Note: Do not replace values passed in the CREATESTRUCT structure. They are for 
// informational use only. If you want to override the initial window rectangle, for 
// example, override the CWnd member function PreCreateWindow.
BOOL 
CFrameChild::OnCreateClient(LPCREATESTRUCT /*lpcs*/,	CCreateContext* pContext)
{
	xTRACE("CFrameChild OnCreateClient - create main splitter and two tab views\n");

	// Create a splitter with 1 row, 2 columns
	// Use the CreateStatic member function to create a static splitter window. 
	// The user can change only the size of the panes in a static splitter window, 
	// not their number or order. 
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	// Now create the child windows (called panes)
	// You must specifically create all the static splitter’s panes when you create 
	// the static splitter. Make sure you create all the panes before the parent 
	// frame’s OnCreateClient member function returns, or the framework will not 
	// display the window correctly.
	// A pane is usually a CView-derived object, but in fact can be any CWnd object 
	// that has the appropriate child window ID.  
	// To do so, simply pass the RUNTIME_CLASS of your CWnd derived class as you would 
	// if you were using a CView derived class. Your class must use DECLARE_DYNCREATE 
	// and IMPLEMENT_DYNCREATE -- the framework uses dynamic creation at runtime.  
	// Although there is a lot of code that is CView specific in CSplitterWnd, 
	// CObject::IsKindOf is always used before those actions are performed.  
	// Certainly, it is much easier to use CSplitterWnd with CView derived classes 
	// than CWnd derived classes.

	// Add Navigation Tab Pane (contains treeview, search, bookmarks)
	// Note: The document has been created at this point but has not been loaded,
	// so there's no way to get splitter info here.
	ULONG lngSplitterPos = 200;
	if (m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CViewTabs), CSize(lngSplitterPos, 100), pContext))
	{
		// Assign navigation mode to tabs window
		m_pviewNavigation = (CViewTabs*) m_wndSplitter.GetPane(0, 0);
		ASSERT_VALID(m_pviewNavigation);
		m_pviewNavigation->SetMode(modeNavigation);
	}
	else return FALSE;

	// Create header view, which will contain the contents view
	if (m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CViewHeader), CSize(200, 100), pContext))
	{
		m_pviewHeader = (CViewHeader*) m_wndSplitter.GetPane(0, 1);
		ASSERT_VALID(m_pviewHeader);
		// make sure it's on top
//		m_pviewHeader->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS);
	}
	else return FALSE;
	
	// Set the active view to the first pane
	SetActiveView( (CView*) m_pviewNavigation);

	return TRUE;
}




// Diagnostics
//----------------------------------------------------------------------------

#ifdef _DEBUG
void 
CFrameChild::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void 
CFrameChild::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG



// Printing
//----------------------------------------------------------------------------
// See CFrameMain



// Methods
//----------------------------------------------------------------------------

/*
CWnd* 
CFrameChild::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	return pWnd;
}
*/



// Message Handlers
//----------------------------------------------------------------------------


// Switch to a view selected by the user.
// This handles the Alt+T, Alt+S, Alt+C type commands that switch between available views.
// Commands handled are from ID_VIEW_FIRST through ID_VIEW_LAST, and include
// ID_VIEW_DATA, ID_VIEW_RTF, etc.
void 
CFrameChild::OnCmdView(UINT nCommandID)
{
	// Convert from CommandID to ViewID
	// Note: For this to work, command id's and view constants must be in same order
	ULONG lngViewID = nCommandID - ID_VIEW_DATA + viewData;
//	SelectView(lngViewID);
	ShowView(lngViewID, TRUE);
}


// Load, enable, disable, check View menu items here
void 
CFrameChild::OnUpdateView(CCmdUI* pCmdUI)
{
	// Enable views based on class of current object
	pCmdUI->Enable(TRUE);
//	if (pCmdUI->m_nID == ID_VIEW_MAP)
//		pCmdUI->Enable(FALSE);
//	else
//		pCmdUI->SetCheck();
}







// Returns the position of the splitter bar
//, return percentage of total splitter control width, eg 10000 = 100%
ULONG 
CFrameChild::GetSplitterPos()
{
	int cxCur;
	int cxMin;
	m_wndSplitter.GetColumnInfo(0, cxCur, cxMin);
	ULONG lngSplitterPos = (ULONG) cxCur;
	return lngSplitterPos;
//	CRect r;
//	m_wndSplitter.GetClientRect(&r);
//	ULONG lngPosPct = lngSplitterPos * 10000 / r.right;
//	xTRACE("GetSplitterPos  pos  %d width %d  pct %d\n", cxCur, r.right, lngPosPct);
//	return lngPosPct;
}


// Set the position of the splitter bar
//, set percentage not twips
void 
CFrameChild::SetSplitterPos(ULONG lngSplitterPos) //lngPosPct) //lngSplitterPos)
{
	// set default here for now
	if (lngSplitterPos == 0)
		lngSplitterPos = 200;
	int cxCur = (int) lngSplitterPos;
	int cxMin = 0;
	m_wndSplitter.SetColumnInfo(0, cxCur, cxMin);
/*
	if (lngPosPct == 0)
		lngPosPct = 5000;
	CRect r;
	m_wndSplitter.GetClientRect(&r);
	ULONG lngSplitterPos = lngPosPct * r.right / 10000;
	int cxCur = (int) lngSplitterPos;
	int cxMin = 0;
	m_wndSplitter.SetColumnInfo(0, cxCur, cxMin);
	xTRACE("SetSplitterPos  pos  %d width %d  pct %d\n", cxCur, r.right, lngPosPct);
*/
}




// Export the entire document or selected item(s) to a rtf, plain text, or NeoMem file
void 
CFrameChild::OnCmdFileExport()
{
	CNeoDoc* pdoc = (CNeoDoc*) GetActiveDocument();
	BObject* pobj = pdoc->GetObject(rootUser);
	theApp.Export(pobj);
}




// Import a file
void 
CFrameChild::OnCmdFileImport() 
{
	// Document handles importing
	CNeoDoc* pDoc = (CNeoDoc*) GetActiveDocument();
	pDoc->Import();
}




// Advance to next visible input view
void 
CFrameChild::OnCmdViewNext() 
{
	xTRACE("CFrameChild OnCmdViewNext\n");

	// Check assumptions
	ASSERT_VALID(m_pviewNavigation);
	ASSERT_VALID(m_pviewHeader);
	ASSERT_VALID(m_pviewHeader->m_pviewTabs);

	// Get the currently active view
	CView* pviewCurrent = GetActiveView();

	// CViewTabs has a method GetNextView which looks through its array of
	// current views and returns the next one, or NULL if view was not in list,
	// or -1 if at the end of the list.
	CView* pviewNext = m_pviewNavigation->GetNextView(pviewCurrent);
	if (pviewNext == NULL)
	{
		// Current view was not in the left side, so it must be in the other side - check there
		pviewNext = m_pviewHeader->m_pviewTabs->GetNextView(pviewCurrent);
		// If at the end of the list, get first view in the left side
		if (pviewNext == (CView*) -1)
			pviewNext = m_pviewNavigation->GetNextView(NULL);
	}
	if (pviewNext == (CView*) -1)
	{
		// Current view was the last view in the left side, so get the first view on the right side
		pviewNext = m_pviewHeader->m_pviewTabs->GetNextView(NULL);
	}

	// If still couldn't find next view, then get first view on left side
	if (pviewNext == NULL)
	{
//		pviewNext = m_pviewNavigation->GetView(viewData);
		pviewNext = (CView*) m_pviewNavigation->m_aCurrentViews.GetAt(0);
	}

	if (pviewNext)
	{
		ASSERT_VALID(pviewNext);
		SetActiveView(pviewNext);
	}

}



// Advance to previous visible input view
void 
CFrameChild::OnCmdViewPrevious() 
{
	xTRACE("CFrameChild OnCmdViewPrevious\n");

	// Check assumptions
	ASSERT_VALID(m_pviewNavigation);
	ASSERT_VALID(m_pviewHeader);
	ASSERT_VALID(m_pviewHeader->m_pviewTabs);

	// Get the currently active view
	CView* pviewCurrent = GetActiveView();

	// CViewTabs has a method GetPreviousView which looks through its array of
	// current views and returns the prev one, or NULL if view was not in list,
	// or -1 if at the end of the list.
	CView* pviewPrevious = m_pviewNavigation->GetPreviousView(pviewCurrent);
	if (pviewPrevious == NULL)
	{
		// Current view was not in the left side, so it must be in the other side - check there
		pviewPrevious = m_pviewHeader->m_pviewTabs->GetPreviousView(pviewCurrent);
		// If at the end of the list, get first view in the left side
		if (pviewPrevious == (CView*) -1)
			pviewPrevious = m_pviewNavigation->GetPreviousView(NULL);
	}
	if (pviewPrevious == (CView*) -1)
	{
		// Current view was the first view in the left side, so get the last view on the right side
		pviewPrevious = m_pviewHeader->m_pviewTabs->GetPreviousView(NULL);
	}

	// If still couldn't find next view, then get first view on left side
	if (pviewPrevious == NULL)
	{
//		pviewNext = m_pviewNavigation->GetView(viewData);
		pviewPrevious = (CView*) m_pviewNavigation->m_aCurrentViews.GetAt(0);
	}

	if (pviewPrevious)
	{
		ASSERT_VALID(pviewPrevious);
		SetActiveView(pviewPrevious);
	}

}




/*
// problem is the accelerator for the frame is intercepting the delete key and translating it 
// to a command (id_edit_delete)
// therefore, probably better to let the views handle the delete key themselves,
// and send a id_edit_delete command to the frame if they're not editing something
void 
CFrameChild::OnCmdEditDelete() 
{
	// assume user wants to delete current item
	CNeoDoc* pDoc = (CNeoDoc*) GetActiveDocument();
	ASSERT_VALID(pDoc);
	BObject* pobj = pDoc->GetCurrentObject();
	ASSERT_VALID(pobj);
	pDoc->DeleteObject(pobj);
}
*/




void 
CFrameChild::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CMDIChildWnd::OnUpdateFrameTitle(bAddToTitle);
/*
	// update our parent window first
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
	CDocument* pDoc = GetActiveDocument();
	if (bAddToTitle)
	{
		TCHAR szText[256+_MAX_PATH];
		if (pDoc == NULL)
			lstrcpy(szText, m_strTitle);
		else
			lstrcpy(szText, pDoc->GetTitle());

		// set title if changed, but don't remove completely
		AfxSetWindowText(m_hWnd, szText);
	}
*/
//	if (GetStyle() & WS_MAXIMIZE)
//		AfxSetWindowText(m_hWnd, "");
//	else
//		AfxSetWindowText(m_hWnd, "biya");
}




void 
CFrameChild::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	// update our parent frame - in case we are now maximized or not
//	OnUpdateFrameTitle(TRUE);
//	AfxMessageBox("child frame resized");
}







// Translate commands to key presses and send to the current left-side view.
// This handles things like Alt+Up.
// Commands handled are ID_NAVIGATE_START through ID_NAVIGATE_STOP.
void 
CFrameChild::OnCmdNavigate(UINT nCommandID) 
{
	xTRACE("CFrameChild OnCmdNavigate\n");

	CViewEx* pview = GetCurrentView(TRUE);
	if (pview)
	{
		UINT nVK = 0;
		switch (nCommandID)
		{
		case ID_NAVIGATE_UP:
			nVK = VK_UP;
			break;
		case ID_NAVIGATE_DOWN:
			nVK = VK_DOWN;
			break;
		case ID_NAVIGATE_LEFT:
			nVK = VK_LEFT;
			break;
		case ID_NAVIGATE_RIGHT:
			nVK = VK_RIGHT;
			break;
		case ID_NAVIGATE_HOME:
			nVK = VK_HOME;
			break;
		case ID_NAVIGATE_END:
			nVK = VK_END;
			break;
//		case ID_NAVIGATE_PAGEDOWN:
//			nVK = VK_NEXT;
//			break;
//		case ID_NAVIGATE_PAGEUP:
//			nVK = VK_PRIOR;
//			break;
		}
		if (nVK)
		{
			xTRACE("  send keydown and up with %d\n", nVK);
//			pViewTree->m_tvw.SendMessage(WM_KEYDOWN, (WPARAM) nVK, 0);
//			pViewTree->m_tvw.SendMessage(WM_KEYUP, (WPARAM) nVK, 0);
//			pview->SendMessage(WM_KEYDOWN, (WPARAM) nVK, 0);
//			pview->SendMessage(WM_KEYUP, (WPARAM) nVK, 0);
			pview->PressKey(nVK);
//			pview->SendMessage(WM_COMMAND, nCommandID, 0);
		}
	}
}





// This is overridden in order to make the child frame maximized
void 
CFrameChild::ActivateFrame(int nCmdShow) 
{
	// Create the child frame window maximized
//	nCmdShow = SW_MAXIMIZE;
//	CMDIChildWnd::ActivateFrame(nCmdShow);

	// got this from MFC FAQ in Help
	// if another window is open, use default
	if (GetMDIFrame()->MDIGetActive())
		CMDIChildWnd::ActivateFrame(nCmdShow); 
	else
		CMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED); // else open maximized
}



//-------------------------------------------------------------------------------------------------------------



// Find and show the specified view, setting it to be the active view if specified.
// Returns a pointer to the view if found, or 0 otherwise.
CView* 
CFrameChild::ShowView(ULONG lngViewID, BOOL bSetActive)
{	
	ASSERT_VALID(this);
	ASSERT_VALID(m_pviewNavigation);
	ASSERT_VALID(m_pviewHeader);
	ASSERT_VALID(m_pviewHeader->m_pviewTabs); // this is the cviewtabs on the right side

	CView* pview = 0;
	//, if view is 0 then it hasn't been loaded yet - load it!
	// how do we know which side it's on?
	// hardcode it for now
//	CView* pView = GetView(lngViewID);
//	if (!pView)
//	{
		// Get tabs view that contains the view we want
		CViewTabs* pviewTabs;
		if (lngViewID == viewData || lngViewID == viewSearch || lngViewID == viewBookmarks || lngViewID == viewIndex)
			pviewTabs = m_pviewNavigation;
		else
			pviewTabs = m_pviewHeader->m_pviewTabs;

		int nTab = pviewTabs->FindView(lngViewID);
		if (nTab != -1)
		{
			pviewTabs->SelectTab(nTab);
//			pviewTabs->ShowTab(nTab);
//			pView = pviewTabs->GetView(lngViewID);
			pview = pviewTabs->GetView(lngViewID);
			if (pview && bSetActive)
			{
				ASSERT_VALID(pview);
				SetActiveView(pview);
			}
		}
//	}
//	if (pView)
//	{
//		ASSERT_VALID(pView);
//		SetActiveView(pView);
//	}
	return pview;
}



// Get the specified view window, looking in both the right and left tab views.
// This will load the view if not found without displaying it.
CView* 
CFrameChild::GetView(ULONG lngViewID)
{
	CView* pview = NULL;
	if (m_pviewHeader)
	{
		CViewTabs* pviewTabs = m_pviewHeader->m_pviewTabs;
		if (pviewTabs)
//			pview = pviewTabs->GetView(lngViewID);
			pview = pviewTabs->LoadView(lngViewID);
	}
	if (pview == NULL)
	{
		CViewTabs* pviewTabs = m_pviewNavigation;
		if (pviewTabs)
//			pview = pviewTabs->GetView(lngViewID);
			pview = pviewTabs->LoadView(lngViewID);
	}
	return pview;
}


// See if the specified view is visible anywhere
BOOL 
CFrameChild::IsViewVisible(ULONG lngViewID)
{
	CView* pview = NULL;
	if (m_pviewHeader)
	{
		CViewTabs* pviewTabs = m_pviewHeader->m_pviewTabs;
		if (pviewTabs)
			return pviewTabs->IsViewVisible(lngViewID);
	}
	if (pview == NULL)
	{
		CViewTabs* pviewTabs = m_pviewNavigation;
		if (pviewTabs)
			return pviewTabs->IsViewVisible(lngViewID);
	}
	return FALSE;
}


//-------------------------------------------------------------------------------------------------------------


void 
CFrameChild::OnFilePrint() 
{
	DoFilePrint(FALSE, TRUE);
}

void 
CFrameChild::OnFilePrintPreview() 
{
	DoFilePrint(TRUE, TRUE);
}

void 
CFrameChild::OnFilePrintFromPreview() 
{
	// print without asking user which view they want to print (already been chosen)
	DoFilePrint(FALSE, FALSE);
}


BOOL 
CFrameChild::DoFilePrint(BOOL bPrintPreview, BOOL bAskForView)
{
	//, will want to handle print preview from here - ie initiate and terminate here,
	// call views as appropriate
	// print dialog could have a checkboxlist of views to print
	// eg in future might have an ancestor view which you might want to print
	// depending on which ones selected, send message to those views as well
	// they just print out in order
	// then clean up print code here
	// for now just print one view - in v2, loop through selected views and print together

	// Get ID of current view
//	CView* pview = GetActiveView();
//	ULONG lngCurrentViewID = viewText; // default
//	if (pview->IsKindOf(RUNTIME_CLASS(CViewEx)))
//		lngCurrentViewID = ((CViewEx*) pview)->GetViewID();
//	CViewEx* pview = GetCurrentView(FALSE);
//	ULONG lngCurrentViewID = pview->GetViewID();
//	ULONG lngCurrentViewID = viewText; // default
//	xTRACE("current view is %d\n", lngCurrentViewID);

	// Get last printed view
	ULONG lngViewID = theApp.m_lngPrintViewID;
	if (lngViewID == 0) lngViewID = viewText; // default

	// Ask user which view they want to print
	if (bAskForView)
	{
		CDialogPrint dlg;
		dlg.m_bPrintHeaderFooter = theApp.m_bPrintHeaderFooter;
		if (IDCANCEL == dlg.DoModalParameters(lngViewID))
			return FALSE;
		lngViewID = dlg.m_lngSelectedViewID;
		theApp.m_bPrintHeaderFooter = dlg.m_bPrintHeaderFooter;
	}

	// Save selected view for next time also (also used when printing from preview)
	theApp.m_lngPrintViewID = lngViewID; 

	// Get selected view and call print method
	CViewEx* pview = DYNAMIC_DOWNCAST(CViewEx, GetView(lngViewID)); // null if not a cviewex
	if (pview)
	{
		pview->DoPrint(bPrintPreview); // print or print preview
	}

	return TRUE;	
}


void 
CFrameChild::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
//	BOOL bDoc = (MDIGetActive() != NULL);
//	pCmdUI->Enable(bDoc);
	// if no frame child exists then the command will be disabled
	pCmdUI->Enable(TRUE);
}


void 
CFrameChild::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
//	BOOL bDoc = (MDIGetActive() != NULL);
//	pCmdUI->Enable(bDoc);	
	// if no frame child exists then the command will be disabled
	pCmdUI->Enable(TRUE);
}


/*
// this code adapted from mfc viewprev.cpp
// Note: CPreviewView :: CScrollView :: CView
BOOL 
CFrameChild::DoPrintPreview(UINT nIDResource, CView* pPrintView, 
								 CRuntimeClass* pPreviewViewClass, CPrintPreviewState* pState)
{
	// Check assumptions
	ASSERT_VALID_IDR(nIDResource);
	ASSERT_VALID(pPrintView);
	ASSERT(pPreviewViewClass != NULL);
	ASSERT(pPreviewViewClass->IsDerivedFrom(RUNTIME_CLASS(CPreviewView)));
	ASSERT(pState != NULL);

	// Get main frame window
	CFrameWnd* pParent = STATIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());
	ASSERT_VALID(pParent);

	CCreateContext context;
	context.m_pCurrentFrame = pParent;
	context.m_pCurrentDoc = GetDocument();
	context.m_pLastView = this;

	// Create the Preview View Window object
	CPreviewView* pPreviewView = (CPreviewView*)pPreviewViewClass->CreateObject();
	if (pPreviewView == NULL)
	{
		TRACE0("Error: Failed to create preview view.\n");
		return FALSE;
	}
	ASSERT_KINDOF(CPreviewView, pPreviewView);
	pPreviewView->m_pPreviewState = pState;        // save pointer

	// Take over Main Frame Window
	// Call this member function to set the application’s main frame window into and out of print-preview mode.
	// The default implementation disables all standard toolbars and hides the main menu and the main 
	// client window. This turns MDI frame windows into temporary SDI frame windows.
	// Override this member function to customize the hiding and showing of control bars and other 
	// frame window parts during print preview. Call the base class implementation from within the 
	// overridden version.
	pParent->OnSetPreviewMode(TRUE, pState);    

	// Create the Print Preview toolbar from the dialog resource
	pPreviewView->m_pToolBar = new CDialogBar;
	if (!pPreviewView->m_pToolBar->Create(pParent, MAKEINTRESOURCE(nIDResource),
		CBRS_TOP, AFX_IDW_PREVIEW_BAR))
	{
		TRACE0("Error: Preview could not create toolbar dialog.\n");
		pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
		delete pPreviewView->m_pToolBar;       // not autodestruct yet
		pPreviewView->m_pToolBar = NULL;
		pPreviewView->m_pPreviewState = NULL;  // do not delete state structure
		delete pPreviewView;
		return FALSE;
	}
	pPreviewView->m_pToolBar->m_bAutoDelete = TRUE;    // automatic cleanup

	// Create the Preview View as a child of the App Main Window.  
	// This is a sibling of this view if this is an SDI app.  This is NOT a sibling if this is an MDI app.
	if (!pPreviewView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0,0,0,0), 
				pParent, AFX_IDW_PANE_FIRST, &context))
	{
		TRACE0("Error: couldn't create preview view for frame.\n");
		pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
		pPreviewView->m_pPreviewState = NULL;  // do not delete state structure
		delete pPreviewView;
		return FALSE;
	}

	// Preview window shown now
	pState->pViewActiveOld = pParent->GetActiveView();
	CView* pActiveView = pParent->GetActiveFrame()->GetActiveView();
	if (pActiveView != NULL)
		pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);

	// Tell the Preview View which View to display
	// SetPrintView calls the view's OnPreparePrinting and OnBeginPrinting
	// OnDraw calls OnPrepareDC and OnPrint
	if (!pPreviewView->SetPrintView(pPrintView))
	{
		pPreviewView->OnPreviewClose();
		return TRUE;            // signal that OnEndPrintPreview was called
	}

	// Make the Preview View the active view, even for MDI
	pParent->SetActiveView(pPreviewView);

	// Update the toolbar and redraw everything
	pPreviewView->m_pToolBar->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
	pParent->RecalcLayout();            // position and size everything
	pParent->UpdateWindow();

	return TRUE;
}


BOOL 
CALLBACK _AfxPreviewCloseProc(CFrameWnd* pFrameWnd)
{
	ASSERT_VALID(pFrameWnd);

	CPreviewView* pView = (CPreviewView*) pFrameWnd->GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT_KINDOF(CPreviewView, pView);

	pView->OnPreviewClose();
	return FALSE;
}

*/







BOOL 
CFrameChild::PreTranslateMessage(MSG* pMsg) 
{
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}




// Frame receives this if view didn't handle it.
// For instance, user might be in rtf view, and hit F2 to rename current item.
// Should be handled by the current navigation pane.
// Assume user wants to rename current item.
void 
CFrameChild::OnObjEditInPlace() 
{
	CView* pView = STATIC_DOWNCAST(CView, m_pviewNavigation->m_aCurrentViews.GetAt(0));
	ASSERT_VALID(pView);
	pView->SendMessage(WM_COMMAND, ID_OBJ_EDIT_IN_PLACE);
}





/*
void 
CFrameChild::OnObjEditInDialog() 
{
	CView* pView = STATIC_DOWNCAST(CView, m_pviewNavigation->m_aCurrentViews.GetAt(0));
	ASSERT_VALID(pView);
	pView->SendMessage(WM_COMMAND, ID_OBJ_EDIT_IN_DIALOG);	
}
*/



/*
void 
CFrameChild::OnPopupCancel() 
{
	// do nothing
}
*/





// Get a pointer to the current view on the left or right side
//, why not determine the active side also?
CViewEx* 
CFrameChild::GetCurrentView(BOOL bLeftSide)
{
	ASSERT(bLeftSide == TRUE); //, for now
	
	ASSERT_VALID(m_pviewNavigation); // left
	ASSERT_VALID(m_pviewHeader); // right
	ASSERT_VALID(m_pviewHeader->m_pviewTabs);

	CViewEx* pView = NULL;
	CViewTabs* pTabs = NULL;

	if (bLeftSide)
		pTabs = m_pviewNavigation;
	else
		pTabs = m_pviewHeader->m_pviewTabs;

	if (pTabs)
		pView = pTabs->GetCurrentView();

	return pView;
}


// Returns a pointer to the current view (left or right side)
CViewEx* 
CFrameChild::GetCurrentView()
{
	// null if not a cviewex
	CViewEx* pview = DYNAMIC_DOWNCAST(CViewEx, this->GetActiveView()); 
	return pview;
}


/*
//.test 
// Add an object. 
// This is the default ID_OBJ_ADD handler.
void 
CFrameChild::OnCmdAddObject() 
{
	CNeoDoc* pdoc = (CNeoDoc*) GetActiveDocument(); //.cast
	BObject* pobjTarget = pdoc->GetTargetObject();

	if (pobjTarget)
	{
		BObject* pobj = pdoc->UIAddNewObject(pobjTarget);
	}
}
*/


