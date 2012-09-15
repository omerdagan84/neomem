
// CViewEx


#include "precompiled.h"

#include "NeoMem.h"
#include "Constants.h"
#include "DialogEditLink.h"
#include "FrameChild.h"
#include "Hint.h"
#include "ListCtrlEx.h"
#include "PreviewViewEx.h"
#include "PrintInfoMore.h"
#include "ViewEx.h"
#include "ViewHeader.h"

#include "ConstantsDatabase.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CViewEx, CView)

BEGIN_MESSAGE_MAP(CViewEx, CView)
	//{{AFX_MSG_MAP(CViewEx)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_PLAIN, OnUpdateNeedClip)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE_PLAIN, OnEditPastePlain)
	ON_WM_RBUTTONDOWN()
	ON_WM_MEASUREITEM()
	ON_WM_MENUCHAR()
	ON_COMMAND(ID_VIEW_HIDE, OnViewHide)
	ON_COMMAND(ID_VIEW_MOVE_UP, OnViewMoveUp)
	ON_WM_DEVMODECHANGE()
//	ON_COMMAND(ID_OBJ_MOVE_TO, OnObjMoveTo)
	//}}AFX_MSG_MAP
	//.. toolbar "tooltip" notification
//	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
//	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText) // id of a tooltip is always zero
	// user-defined message
END_MESSAGE_MAP()





// Construction/Destruction
//-----------------------------------------------------------------------------------------------

CViewEx::CViewEx() 
{
	m_pDoc = NULL;
	m_pviewChild = NULL;
	m_ptbr = NULL;
	m_hAccel = 0;
	m_bShowTitleBar = FALSE;
	m_nTitleBarHeight = 0;
	m_pwndDropTarget = NULL;
}

CViewEx::~CViewEx() 
{
	// m_pDropTarget is part of CWnd
	if (m_pDropTarget) {
		delete m_pDropTarget;
		m_pDropTarget = NULL; // bug: (potential?) hadn't cleared this variable after deleting reference
	}
}


BOOL CViewEx::PreCreateWindow(CREATESTRUCT& cs) 
{
	// WS_CLIPCHILDREN   Excludes the area occupied by child windows when you draw 
	// within the parent window. Used when you create the parent window.
	// This helps reduce flickering. 
	cs.style |= WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}



int CViewEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Get document pointer
	// MFC presumably sets m_pDocument before calling this. 
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(BDoc)));
//	m_pDoc = (BDoc*) m_pDocument;
	m_pDoc = DYNAMIC_DOWNCAST(BDoc, m_pDocument);

	m_nTitleBarHeight = 20; //. how do we get this? base on text height
//		CFont* pfontOld = pDC->SelectObject(&theApp.m_fontControls);
//	SetFont(&theApp.m_fontControls, FALSE);
	
	return 0;
}



// Call this to tell the view about the child view attached to it
void CViewEx::SetChildView(CWnd* pView)
{
	m_pviewChild = pView;
}

// Call this to tell the view that there is a toolbar attached to it (optional)
void CViewEx::SetToolBar(CToolBarCtrl* pWnd)
{
	m_ptbr = pWnd;
}


// Create a View window as a child window
// Also attaches the view to the document via a context object
// Note: Doesn't display the new window
// Returns a pointer to the created CView object, or NULL if failed
// Adapted from MFC
// okay to cast the resulting cview ptr to whatever class you passed it. 
//. make this a method?
///* static */ CView* 
//CViewEx::CreateChildView(CWnd* pParent, CRuntimeClass* pViewClass, CDocument* pDoc, CRect rPos, UINT nControlID)
CView* CViewEx::CreateChildView(CRuntimeClass* pViewClass, CDocument* pDoc, CRect rPos, UINT nControlID)
{
	xTRACE("CViewEx::CreateChildView of Class %s\n", pViewClass->m_lpszClassName);

	// Check assumptions
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
	ASSERT_VALID(pDoc);

	// Create a Context object which will associate the newly created View with the Document
	CCreateContext cc;
	cc.m_pCurrentDoc = pDoc;
	cc.m_pCurrentFrame = 0;
	cc.m_pNewViewClass = pViewClass;
	cc.m_pNewDocTemplate = pDoc->GetDocTemplate();

	// Create the specified CView object
	// (this is why you include the DECLARE_DYNCREATE macro in a class -
	// it defines the CreateObject function)
	CWnd* pWnd;
	TRY
	{
		//, why is it okay to cast a CObject to a CWnd like this? (mfc code)
		// CreateObject must create a CView object of some type, then 
		// return it through a CObject pointer. 
		pWnd = (CWnd*) pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		// Note: DELETE_EXCEPTION(e) not required
		TRACE("! Out of memory creating a tab control child window.\n");
		return NULL;
	}
	END_CATCH_ALL
	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL); // Not yet created
	

	// Create Windows window associated with the specified CView object
	//. pass style here?
	// too bad we don't have more control over the style of the border
	// bug: Very strange - if you create the window without WS_VISIBLE, then
	// the rtf control will not accept enters
	//, why is it okay to pass null for classname here? 
//	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	if (!pWnd->Create(
					NULL,					// classname
					NULL,					// window name
					dwStyle,				// style
					rPos,					// position
//					pParent,				// parent window
					this,					// parent window
					nControlID,				// Control ID
					&cc				// Context of window
					))
	{
		// Note: pWnd will be cleaned up by PostNcDestroy
		TRACE("! Warning: couldn't create Window associated with CView object.\n");
		return NULL;
	}
	
	// Put the child view at the top of the z-order so that it repaints correctly
	pWnd->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS);
	
	return (CView*) pWnd;
}




// Diagnostics
//-----------------------------------------------------------------------------------------------

#ifdef _DEBUG

void CViewEx::AssertValid() const
{
	CView::AssertValid();
}

void CViewEx::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

/*
BDoc* CViewEx::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(BDoc)));
	return (BDoc*)m_pDocument;
}
*/

#endif //_DEBUG

/*
BDoc* CViewEx::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(BDoc)));
	return (BDoc*)m_pDocument;
}
*/


// Drawing
//-----------------------------------------------------------------------------------------------


// Reposition child view and toolbar (if any)
void CViewEx::OnSize(UINT nType, int cx, int cy) 
{
	xTRACE("CViewEx::OnSize\n");

	// Call base class
	CView::OnSize(nType, cx, cy);
	
	int nToolbarTop = 0; // top of toolbar
	int nToolbarHeight = 0; // height of toolbar plus borders

	if (m_bShowTitleBar)
	{
		nToolbarTop = m_nTitleBarHeight;
	}

	// If this view has a toolbar, then resize that as well.
	// Get height of toolbar control.
	// There is a one pixel border around the toolbar, and a one pixel border around the view's client area.
	if (m_ptbr)
	{
		CRect r;
		m_ptbr->GetClientRect(r);
		// Get height of toolbar control plus 1 pixel border on top and bottom (+2) that will be drawn around it.
		nToolbarHeight = (r.bottom - r.top + 1) + 2; // 26 - 0 + 1 + 2= 29 pixels
		m_ptbr->MoveWindow(2, nToolbarTop + 2, cx - 4, nToolbarHeight - 3);
		nToolbarHeight--;
	}

	// Position child view in correct place
	if (m_pviewChild)
	{
		m_pviewChild->MoveWindow(1, nToolbarTop + nToolbarHeight + 1, cx - 2, cy - nToolbarTop - 2 - nToolbarHeight);
	}

}



BOOL CViewEx::OnEraseBkgnd(CDC* pDC) 
{
	xTRACE("CViewEx::OnEraseBkgnd\n");

	// The default implementation erases the background using the window class background brush 
	// specified by the hbrBackground member of the window class structure. 
//	return CView::OnEraseBkgnd(pDC);

	// An overridden OnEraseBkgnd should return nonzero in response to WM_ERASEBKGND if it 
	// processes the message and erases the background; this indicates that no further erasing is 
	// required. If it returns 0, the window will remain marked as needing to be erased. (Typically, 
	// this means that the fErase member of the PAINTSTRUCT structure will be TRUE.) 
	
	// one problem - if we return TRUE here, then the background for the toolbar does not
	// get erased - need to fill a rectangle if there is a toolbar for this view

	CRect r;		
	COLORREF clrBackground = Library::clr3dFace;

	// Erase client area only if no child view
	if (m_pviewChild != NULL)
	{
//		clrBackground = 0x0000ffff;
		GetClientRect(r);
		pDC->FillSolidRect(r, clrBackground);
	}

	// Erase TitleBar area if visible
	if (m_bShowTitleBar)
	{
//		clrBackground = 0x00ff0000;
		GetClientRect(r);
		r.bottom = m_nTitleBarHeight + 1;
		pDC->FillSolidRect(r, clrBackground);
	}

	// Erase the background under the toolbar, if any
	if (m_ptbr)
	{
//		clrBackground = 0x0000ff00;
		m_ptbr->GetClientRect(r);
		r.OffsetRect(2,2); // shift to match toolbar position
		if (m_bShowTitleBar) 
		{
			r.top += m_nTitleBarHeight;
			r.bottom += m_nTitleBarHeight;
		}
		pDC->FillSolidRect(r, clrBackground);
	}
	
	return TRUE;
}


// Draw border around client area and toolbar if any, and titlebar if visible.
void CViewEx::OnDraw(CDC* pDC)
{
	xTRACE("CViewEx::OnDraw - draw border around client area and toolbar (if any)\n");

	CRect r;

	// Draw MiniHeader (TitleBar) if specified
	// Need to calculate this on the fly because this view might be included in more than one
	// position (ie at top of one tab and bottom of another)
	if (m_bShowTitleBar)
	{
		// Draw 3D border around title bar
		GetClientRect(r);
		r.bottom = m_nTitleBarHeight;
		r.OffsetRect(1, 1);
		r.DeflateRect(0, 0, 2, 0);
		pDC->Draw3dRect(r, Library::clr3dHighlight, Library::clr3dShadow);

		pDC->SetTextColor(Library::clrWindowText);
		pDC->SetBkColor(Library::clr3dFace);
		CFont* pfontOld = pDC->SelectObject(&theApp.m_fontControls);
		r.left = 6; // pixels?
//		pDC->SetBkMode(TRANSPARENT); // transparent background for text
//		int nFormat = DT_LEFT | DT_SINGLELINE | DT_BOTTOM;
		int nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
		pDC->DrawText(m_strTitleBar, &r, nFormat);
		pDC->SelectObject(pfontOld); // deselect font

		//, Draw close button in right corner
	}

	// Draw 3d border around client area of view
	if (m_pviewChild)
	{
		GetClientRect(r);
		pDC->Draw3dRect(r, Library::clr3dShadow, Library::clr3dHighlight);
	}

	// Draw toolbar border here as well, if the view has a toolbar
	if (m_ptbr)
	{
		m_ptbr->GetClientRect(r);
		r.OffsetRect(2,2); // shift to match toolbar position
		r.InflateRect(1, 1); // now inflate the rectangle by one pixel so we can draw a border around it
		if (m_bShowTitleBar) 
		{
			r.top += m_nTitleBarHeight;
			r.bottom += m_nTitleBarHeight;
		}
		pDC->Draw3dRect(r, Library::clr3dHighlight, Library::clr3dShadow);
	}

}



// Print support
//-----------------------------------------------------------------------------------------------------------------


// Virtual method that each view should implement do handle printing/print preview
BOOL CViewEx::DoPrint(BOOL bPreview)
{
	return FALSE; // not handled by base class
}


// copied from mfc ViewPrev.cpp
void CViewEx::OnFilePrintPreview()
{
	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.

	// must not create this on the frame.  Must outlive this function
	CPrintPreviewState* pState = new CPrintPreviewState;

	// DoPrintPreview's return value does not necessarily indicate that
	// Print preview succeeded or failed, but rather what actions are necessary
	// at this point.  If DoPrintPreview returns TRUE, it means that
	// OnEndPrintPreview will be (or has already been) called and the
	// pState structure will be/has been deleted.
	// If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
	// WILL NOT be called and that cleanup, including deleting pState
	// must be done here.
	//` use our own toolbar and view class
//`	if (!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this, RUNTIME_CLASS(CPreviewView), pState))
	if (!DoPrintPreview(IDD_PREVIEW_TOOLBAR, this, RUNTIME_CLASS(CPreviewViewEx), pState))
	{
		// In derived classes, reverse special window handling here for
		// Preview failure case
		TRACE0("Error: DoPrintPreview failed.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}
}


// dev change code from mfc richeditview 
// This is the WM_DEVMODECHANGE handler.
// The framework calls this member function for all top-level CWnd objects when the user 
// changes device-mode settings. 
void CViewEx::OnDevModeChange(LPTSTR /*lpDeviceName*/)
{
	CDC dc;
	AfxGetApp()->CreatePrinterDC(dc);
	OnPrinterChanged(dc);
}


// This is typically called by the view when it gets a WM_DEVMODECHANGE.
// Also called during page setup.
//, this doesn't seem to get called when you change orientation...
void CViewEx::OnPrinterChanged(const CDC& dcPrinter)
{
/*
	CSize size;
	if (dcPrinter.m_hDC != NULL)
	{
		// This will fill in the page size
		size.cx = MulDiv(dcPrinter.GetDeviceCaps(PHYSICALWIDTH), 1440,
			dcPrinter.GetDeviceCaps(LOGPIXELSX));
		size.cy = MulDiv(dcPrinter.GetDeviceCaps(PHYSICALHEIGHT), 1440,
			dcPrinter.GetDeviceCaps(LOGPIXELSY));
	}
	else
		size = CSize(8*1440+720, 11*1440); // 8.5" by 11"
	if (GetPaperSize() != size)
	{
		SetPaperSize(size);
//`		if (m_nWordWrap == WrapToTargetDevice) //wrap to ruler
//`			WrapChanged();
	}
*/
}



void CViewEx::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
//	CView::OnBeginPrinting(pDC, pInfo);

	// Initialize extra print info
	CPrintInfoMore& rpim = theApp.m_printinfo;
	rpim.Initialize(pDC);
}


void CViewEx::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
//	CView::OnEndPrinting(pDC, pInfo);

	// Delete gdi objects
	CPrintInfoMore& rpim = theApp.m_printinfo;
	rpim.Terminate();
}



//-----------------------------------------------------------------------------------------------------------------




// This routine was added to make up for lack of ability to send update hint to a single view.
// This works just like UpdateAllViews, but just targets this view.
void CViewEx::UpdateView(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// Call OnUpdate, which is a virtual function overridden by each view derived from ViewEx.
	OnUpdate(pSender, lHint, pHint);
}




// Used by tooltiptext handler:

// Supply ToolTip text for toolbar
// Toolbar control sends TTN_NEEDTEXT to PARENT of Toolbar control! (usually CViewEx)
// Code adapted from CFrameWnd class
BOOL CViewEx::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[256];
	CString strTipText;
	UINT nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
//`		nID = _AfxGetDlgCtrlID((HWND)nID);
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	if (nID != 0) // will be zero on a separator
	{
		// don't handle the message if no string resource found
		if (AfxLoadString(nID, szFullText) == 0)
			return FALSE;

		// this is the command id, not the button index
		AfxExtractSubString(strTipText, szFullText, 1, '\n');
	}

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);

	return TRUE;    // message was handled
}


// my simple version
/*
BOOL CViewEx::OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
	TRACE("CViewEx OnToolTipText %d\n", nID);
	if(nID)
	{
		pTTT->lpszText = MAKEINTRESOURCE(nID);
		pTTT->hinst = AfxGetResourceHandle();
		return(TRUE);
	}
	return FALSE; // message was not handled
}
*/



// Use this method to interrupt or modify MFC's normal command routing. 
// Eg normally commands are send to the View and then the Document. 
// But we want them to go from the View to the Child Frame (ie the
// Document Frame). 
// or - do we need to handle each message explicitly in this class? 
// try that. 
// um, or use OnCmdMsg instead. 
// If PreTranslateMessage() returns TRUE, MFC will assume that you have 
// handled the message. If it returns FALSE, MFC assumes that you have 
// elected not to handle it and will proceed with its normal dispatch process.
BOOL CViewEx::PreTranslateMessage(MSG* pMsg) 
{
	// Translate a message using a custom accelerator table (if any) for the view.
	if (m_hAccel && pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// TranslateAccelerator does all the magic to translate WM_KEYUP and WM_KEYDOWN 
		// messages into WM_COMMAND messages when the keys pressed match commands 
		// in the accelerator table.
		return ::TranslateAccelerator(m_hWnd, m_hAccel, pMsg);
	}
	return CView::PreTranslateMessage(pMsg);
}


// Load the specified accelerator resource for this view.
// Returns handle to accelerator, or NULL.
// Also stores handle to accelerator in m_hAccel variable.
// Ex. LoadAccel(IDR_VIEW_SEARCH);
HACCEL CViewEx::LoadViewAccelerators(int nAccelID)
{
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(nAccelID));
	return m_hAccel;
}






// Drag Drop Handler routines
//---------------------------------------------------------------------------------------------------------


// Register this window as an OLE drop target
// Note: Revoke will be called automatically when the view window is destroyed
void CViewEx::DragRegister(CWnd* pwndDropTarget)
{
//	ASSERT_VALID(pwndDropTarget);
	m_DropTarget.Register(this);
	m_pwndDropTarget = pwndDropTarget;
}


/*
void CViewEx::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	xTRACE("CViewEx::OnBeginDrag\n");

	// Get array of selected objects from derived view to start drag operation with
	BObjects aObjects;
//	if (!DragGetSelectedObjects(pNMHDR, aObjects))
	if (!DragGetSelectedObjects(aObjects))
		return;

	// Write array to global memory block
	HGLOBAL hGlobal = aObjects.SaveToGlobal();
	if (hGlobal)
	{
		// Wrap global memory block in data source object
		COleDataSource ods;
		ods.CacheGlobalData(theApp.m_cfObjects, hGlobal);

		// Do Drag Drop
		DWORD dwEffects = DROPEFFECT_COPY | DROPEFFECT_MOVE;
		DROPEFFECT de = ods.DoDragDrop(dwEffects);
		switch (de)
		{
			case DROPEFFECT_MOVE:
				xTRACE("  Dropeffect move\n");
				break;
			case DROPEFFECT_COPY:
				xTRACE("  Dropeffect copy\n");
				break;
			case DROPEFFECT_NONE:
				xTRACE("  Dropeffect none\n");
				break;
		}
		// Free global memory here
		::GlobalFree(hGlobal);
	}
}
*/



// Start a drag-drop operation by wrapping the drag objects into an hGlobal memory block.
// Derived class should fill m_aDragObjects with objects to be dragged and then call this routine.
void CViewEx::DragStart()
{
	xTRACE("CViewEx::DragStart\n");

	// Exit if no drag objects
	if (m_aDragObjects.GetSize() == 0)
		return;

	// Write array to global memory block
	HGLOBAL hGlobal = m_aDragObjects.SaveToGlobal();
	if (hGlobal)
	{
		// Wrap global memory block in data source object
		COleDataSource ods;
		ods.CacheGlobalData(theApp.m_cfObjects, hGlobal);

		// Do Drag Drop
		DWORD dwEffects = DROPEFFECT_COPY | DROPEFFECT_MOVE;
		DROPEFFECT de = ods.DoDragDrop(dwEffects);
		switch (de)
		{
			case DROPEFFECT_MOVE:
				xTRACE("  Dropeffect move\n");
				break;
			case DROPEFFECT_COPY:
				xTRACE("  Dropeffect copy\n");
				break;
			case DROPEFFECT_NONE:
				xTRACE("  Dropeffect none\n");
				break;
		}
		// Free global memory here
		::GlobalFree(hGlobal);
	}
}


// Called by the framework when the mouse first enters the non-scrolling region of the 
// drop target window. Default implementation is to do nothing and return DROPEFFECT_NONE.
// Override this function to prepare for future calls to the OnDragOver member function. 
// Any data required from the data object should be retrieved at this time for later use in 
// the OnDragOver member function. The view should also be updated at this time to give 
// the user visual feedback.
DROPEFFECT CViewEx::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	// Call base class
	CView::OnDragEnter(pDataObject, dwKeyState, point);

	// Save key state
	m_dwDragKeyState = dwKeyState;

	// See if the data object has any data that this view is interested in (text or BObjects)
	//. cviewex could store array of formats we're interested in
	// so could automatically do this
	// or have flags for each one (predefined flags)
	// theApp could supply the format given a flag
	// for now, let's just move this to cviewex
	m_bInterestedInData = pDataObject->IsDataAvailable(CF_TEXT) || 
			pDataObject->IsDataAvailable(theApp.m_cfRtf) ||
			pDataObject->IsDataAvailable(theApp.m_cfObjects);

	// If we're interested in the data, return move or copy depending on status of control key
	if (m_bInterestedInData)
		return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
	else
		// Not interested in anything
		return DROPEFFECT_NONE;
}


// Called by the framework during a drag operation when the mouse is moved over the 
// drop target window. The default implementation is to do nothing and return DROPEFFECT_NONE.
// Override this function to give the user visual feedback during the drag operation. Since 
// this function is called continuously, any code contained within it should be optimized as much as possible. 
// If this function returns DROPEFFECT_NONE, then OnDrop will NOT get called on mouseup!
// This should be overridden in each derived CView* class to highlight the object the cursor is over.
DROPEFFECT CViewEx::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint ptClient) 
{
	// Call base class
	return CView::OnDragOver(pDataObject, dwKeyState, ptClient);
/*
	// Ignore if not interested in the data
	if (!m_bInterestedInData) return DROPEFFECT_NONE;

	pobjDropTarget = DragGetDropTarget(point);
	if (pobjDropTarget)
		de = (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
	else
		de = DROPEFFECT_NONE;

	if (pobjDropTarget != m_pobjDropTarget)
		DragHighlightTarget(pobjDropTarget);

	m_pobjDropTarget = pobjDropTarget;
	
	return de;
*/
}



// Called by the framework before calling OnDragEnter or OnDragOver to determine whether 
// the point is in the scrolling region. Override this function when you want to provide special 
// behavior for this event. The default implementation automatically scrolls windows when the 
// cursor is dragged into the default scroll region inside the border of each window.
// This will scroll the window specified in DragRegister, for instance a treeview window.
DROPEFFECT CViewEx::OnDragScroll(DWORD dwKeyState, CPoint point)
{
	xTRACE("CViewEx::OnDragScroll\n");

	ASSERT_VALID(m_pwndDropTarget);

	ClientToScreen(&point);
	CRect r;
	m_pwndDropTarget->GetWindowRect(r);

	// See if point is inside window
	if (r.PtInRect(point))
	{
		// Now see if point is inside border of window
		r.DeflateRect(20, 20); //.. 
		if (!r.PtInRect(point))
		{
			// The point is in the border, so scroll the window
			if (point.y < r.top)
				m_pwndDropTarget->SendMessage(WM_VSCROLL, MAKEWORD(SB_LINEUP, 0), NULL);
			if (point.y > r.bottom)
				m_pwndDropTarget->SendMessage(WM_VSCROLL, MAKEWORD(SB_LINEDOWN, 0), NULL);
			if (point.x < r.left)
				m_pwndDropTarget->SendMessage(WM_HSCROLL, MAKEWORD(SB_LINELEFT, 0), NULL);
//			if (point.x > r.right)
//				m_pwndDropTarget->SendMessage(WM_HSCROLL, MAKEWORD(SB_LINERIGHT, 0), NULL);
			return DROPEFFECT_SCROLL;
		}
	}
	return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
}


// Called by the framework when the user releases a data object over a valid drop target. 
// The default implementation does nothing and returns FALSE.
// Override this function to implement the effect of an OLE drop into the client area of the view. 
// The data object can be examined via pDataObject for Clipboard data formats and data 
// dropped at the specified point.
BOOL CViewEx::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint ptClient) 
{
	xTRACE("CViewEx::OnDrop\n");
	
	// Call OnDragLeave to clean up
	OnDragLeave();

	// See which item is the target
	BObject* pobjTarget = DragGetDropTarget(ptClient);
//	CString strTarget = pobjTarget->GetText();
//	TRACE("  Drop onto %s\n", (LPCTSTR) strTarget);

	// Exit if it's over the dummy new row
	if ((DWORD) pobjTarget == CListCtrlEx::keyDummyRow)
		return FALSE;

	// Initialize clipboard format structure
	FORMATETC fe = {NULL, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};

	// Check for Rich Text Format 
	fe.cfFormat = theApp.m_cfRtf;
	BOOL bText = pDataObject->IsDataAvailable(fe.cfFormat, &fe);
	if (!bText)
	{
		// Check for unformatted text
		fe.cfFormat = CF_TEXT;
		bText = pDataObject->IsDataAvailable(fe.cfFormat, &fe);
	}

	// If text is available, retrieve it and add it to the target object
	if (bText)
	{
		xTRACE("  Text is available!\n");

		// Get global memory block
		HGLOBAL hGlobal = pDataObject->GetGlobalData(fe.cfFormat, &fe);
		if (!hGlobal) return FALSE;

		// Get text from global memory into a cstring object
		CString strText;
		LPCSTR pData = (LPCSTR) ::GlobalLock(hGlobal);
		if (pData)
			strText = pData;
		::GlobalUnlock(hGlobal);

		// Add text to target object
		// specify which property you want to add it to, and beginning, curpos, or end (default)
		pobjTarget->AddRtf(propRtfText, strText);

		// Return FALSE if move fails (eg object does not accept rtf)
		// If we return TRUE here, the text is removed from the source
		// If we return FALSE, the text remains in the source
		if (dropEffect == DROPEFFECT_COPY)
			return FALSE;
		else
			return TRUE;
	}


	// Check for collection of objects being copied or moved
	fe.cfFormat = theApp.m_cfObjects;
	if (pDataObject->IsDataAvailable(fe.cfFormat, &fe))
	{
		xTRACE("  NeoMem objects are available!\n");
		HGLOBAL hGlobal = pDataObject->GetGlobalData(fe.cfFormat);
		if (!hGlobal) return FALSE;
		
		// Unpack array of objects from global memory block into a local array
		BObjects aObjects;
		if (!aObjects.LoadFromGlobal(hGlobal)) return FALSE;

		// Exit if no object contained
		if (aObjects.GetSize() < 1) return FALSE;

		// Make sure objects are from same document as the target...
		BObject* pobjFirst = DYNAMIC_DOWNCAST(BObject, aObjects.GetAt(0));
		ASSERT_VALID(pobjFirst);
		if (pobjFirst->GetDoc() != pobjTarget->GetDoc())
		{
			AfxMessageBox("Can't move or copy items between documents!", MB_ICONINFORMATION);
			return FALSE;
		}

		// for TRACE...
		CString strItems = aObjects.GetText();
		xTRACE("   Objects: %s\n", (LPCTSTR) strItems);

		// Handle copy
		if (dropEffect == DROPEFFECT_COPY)
		{
			// If this is a copy operation, create copies of the selected objects
			AfxMessageBox("Copying objects not available.");
//			xTRACE("  Copy %s and add them to %s's child collection\n", (LPCTSTR) strItems, (LPCTSTR) strTarget);
//			aObjectsCopy = aObjects.Copy();
//			aObjectsCopy.SetParent(pobjTarget);
//			GetDocument()->SetModifiedFlag();
			// Now tell all views about the move
//			xTRACE("  Now update the views\n");
//			CHint objHint;
//			objHint.m_paObjects = &aObjects;
//			objHint.m_pobjParent = pobjTarget;
//			GetDocument()->UpdateAllViewsEx(NULL, hintMove, &objHint);
			return TRUE;
		}

		// Handle move
		if (dropEffect == DROPEFFECT_MOVE)
		{
			// Check if move is valid
			// Displays message if not allowed to drop any of the objects on the target
//			if (!GetDocument()->IsMoveValid(aObjects, pobjTarget))
			if (!aObjects.IsMoveValid(pobjTarget))
				return FALSE;

			// Move the items to the new location by changing their parent.
			// This will set the doc modified flag and update the views
//			xTRACE("  Move %s to %s\n", (LPCTSTR) strItems, (LPCTSTR) strTarget);
			aObjects.MoveTo(pobjTarget);

			return TRUE;
		}
	}

	// Call base class
	return CView::OnDrop(pDataObject, dropEffect, ptClient);
}



void CViewEx::OnDragLeave() 
{
	CView::OnDragLeave();
}



// Virtual function that derived view should override and return pointer to object under the given point.
BObject* CViewEx::DragGetDropTarget(CPoint ptClient)
{
	return NULL;
}



//------------------------------------------------------------------------------------------------------



// Get a sibling view
//. also look in nav view
CView* CViewEx::GetSiblingView(ULONG lngViewID)
{
	CFrameChild* pChild = DYNAMIC_DOWNCAST(CFrameChild, GetParentFrame());
	CViewHeader* pviewHeader = pChild->GetViewHeader();
	CView* pview = NULL;
	if (pviewHeader)
	{
		CViewTabs* pviewTabs = pviewHeader->m_pviewTabs;
		if (pviewTabs)
			pview = pviewTabs->GetView(lngViewID);
	}
	return pview;
}



// Find and show the selected view, and optionally set the focus to it.
//, need to fix all this view handling code
CView* CViewEx::ShowView(ULONG lngViewID, BOOL bSetActive, BOOL bAskAddIfNotFound)
{
	CFrameChild* pFrame = DYNAMIC_DOWNCAST(CFrameChild, GetParentFrame());
	if (pFrame)
	{
		CView* pview = pFrame->ShowView(lngViewID, bSetActive);
		if ((pview == 0) && (bAskAddIfNotFound))
		{
			BObject* pobjView = m_pDoc->GetObject(lngViewID);
//			CString strView = pobjView->GetPropertyString(propName);
			CString strView = pobjView->GetPropertyString(propCaption);
			if (strView.IsEmpty())
				strView = pobjView->GetPropertyString(propName); // use name if caption is blank
			strView.Remove('&');
			CString strMsg;
			strMsg.Format("The %s View is not visible for this object - search results cannot be displayed without it. "
					"Do you want to add this view to objects of this class?", (LPCTSTR) strView);
			if (IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION + MB_YESNO))
			{
				// bug: passed view the wrong way - used a hint object on accident!
				m_pDoc->UpdateAllViewsEx(NULL, hintAddView, pobjView);
				// try again...
				pview = pFrame->ShowView(lngViewID, bSetActive);
			}
		}
		return pview;
	}
	return NULL;
}




// UPDATE_COMMAND_UI handler for Edit Copy and Edit Cut which both require that 
// the current focus is on an edit control that has text selected.
void CViewEx::OnUpdateNeedSel(CCmdUI* pCmdUI) 
{
	// Get the current focus & determine if it's on a CEdit control.
	CWnd* pWnd = GetFocus();
	if ((pWnd == NULL) || !Library::IsEdit(pWnd))
		pCmdUI->Enable(FALSE);
	else
	{
		CEdit* pEdit = STATIC_DOWNCAST(CEdit, pWnd);
		int nBeg, nEnd;
		pEdit->GetSel(nBeg, nEnd);
		pCmdUI->Enable(nBeg != nEnd);
	}
}


// UPDATE_COMMAND_UI handlers for Edit Paste requires that focus be on an edit 
// control and that the clipboard contains text to be pasted into the control.
void CViewEx::OnUpdateNeedClip(CCmdUI* pCmdUI) 
{
	// Get the current focus & determine if it's on a CEdit control also check 
	// to see if the control is read-only.
	CWnd* pWnd = GetFocus();
	if ((pWnd == NULL) || 
		(!Library::IsEdit(pWnd)) || 
		((pWnd->GetStyle() & ES_READONLY) != 0)
		)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));
}


// Copy handler
void CViewEx::OnEditCopy() 
{
	CWnd* pWnd = GetFocus();
	if (Library::IsEdit(pWnd)) {
		CEdit* pEdit = STATIC_DOWNCAST(CEdit, pWnd);
		pEdit->Copy();
	}
}

// Cut handler
void CViewEx::OnEditCut() 
{
	CWnd* pWnd = GetFocus();
	if (Library::IsEdit(pWnd)) {
		CEdit* pEdit = STATIC_DOWNCAST(CEdit, pWnd);
		pEdit->Cut();
	}
}

// Paste handler
void CViewEx::OnEditPastePlain() 
{
	CWnd* pWnd = GetFocus();
	if (Library::IsEdit(pWnd) && ::IsClipboardFormatAvailable(CF_TEXT)) {
		CEdit* pEdit = STATIC_DOWNCAST(CEdit, pWnd);
		pEdit->Cut();
	}
}








// Prompt user and then remove this view from the tab by sending a hint to the tab view.
void CViewEx::OnViewHide() 
{
	ASSERT(m_lngViewID);
	BObject* pobjView = m_pDoc->GetObject(m_lngViewID);
	ASSERT_VALID(pobjView);

/*
	CString strViewName = pobjView->GetPropertyString(propName);
	strViewName.Remove('&');
	BObject* pobj = m_pDoc->GetCurrentObject();
	CString strClassName = pobj->GetPropertyString(propClassName);
	CString strMsg;
	strMsg.Format("This will remove the %s View for this object and all other objects of the same class (%s). To restore it, you can right click on the tab containing this view. Continue?",
		(LPCTSTR) strViewName,
		(LPCTSTR) strClassName);
*/
//	if (IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION + MB_YESNO))
//	{
		m_pDoc->UpdateAllViewsEx(0, hintRemoveView, pobjView);
//	}

}




// Move this view up in the view order
void CViewEx::OnViewMoveUp() 
{
	ASSERT(m_lngViewID);
	BObject* pobjView = m_pDoc->GetObject(m_lngViewID);
	ASSERT_VALID(pobjView);
	m_pDoc->UpdateAllViewsEx(0, hintMoveViewUp, pobjView);
}





// Virtual function to be overridden by each view that will forward the given 
// keypress to the main control.
BOOL CViewEx::PressKey(UINT nVK)
{
	return FALSE;
}



void CViewEx::ShowTitleBar(BOOL bShow)
{
	m_bShowTitleBar = bShow;
}


void CViewEx::SetTitleBarName(LPCTSTR pszName)
{
	m_strTitleBar = pszName;
}


void CViewEx::SetViewID(ULONG lngViewID)
{
	m_lngViewID = lngViewID;
}


ULONG CViewEx::GetViewID()
{
	return m_lngViewID;
}



// Is this view the active view?
BOOL CViewEx::IsActiveView()
{
	CFrameChild* pChild = DYNAMIC_DOWNCAST(CFrameChild, GetParentFrame());
	if (pChild) {
		CViewEx* pview = pChild->GetCurrentView();
		return (pview == this);
	}
	return FALSE;
}







void CViewEx::OnRButtonDown(UINT nFlags, CPoint point) 
{
/*
	// Exit if on Home Object
	//, allow changes in future versions?
	if (m_pDoc->GetCurrentObject()->GetObjectID() == rootUser)
	{
		AfxMessageBox("The Home Folder views cannot be modified.", MB_ICONINFORMATION);
		return;
	}

	ClientToScreen(&point);

	// Display pop up menu
	CMenu menu;
	if (menu.LoadMenu(IDR_POPUP_MINI_HEADER))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
//		pPopup->SetDefaultItem(ID_OBJ_OPEN);

		CString strView = m_strTitleBar;
		strView.Remove('&');

		CString strName = _T("&Move ") + strView + _T(" View Up");
		pPopup->ModifyMenu(ID_VIEW_MOVE_UP, MF_BYCOMMAND | MF_STRING, 
										ID_VIEW_MOVE_UP, (LPCTSTR) strName);
		
		strName = _T("&Hide ") + strView + _T(" View");
		pPopup->ModifyMenu(ID_VIEW_HIDE, MF_BYCOMMAND | MF_STRING, ID_VIEW_HIDE, (LPCTSTR) strName);

		// If this is the Home View, disable the hide menu
		//, in future might be based on bobject flags
//		if (m_lngViewID == viewHome)
//			pPopup->EnableMenuItem(ID_VIEW_HIDE, MF_BYCOMMAND | MF_GRAYED);

//		// let main window handle commands since theApp handles them
//		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	
//	CView::OnRButtonDown(nFlags, point);
*/
//}
//

	
//void CMymenuView::OnRButtonDown(UINT /*nFlags*/, CPoint point) //SK: removed warning C4100: 'nFlags' : unreferenced formal parameter
//{

/*
//	m_popmenu.LoadMenu(IDR_RIGHT_CLICK);
	m_popmenu.LoadMenu(nPopupID);
	m_popmenu.LoadToolbar(IDR_MAINFRAME);
	ClientToScreen(&point);
	BCMenu* psub = (BCMenu*) m_popmenu.GetSubMenu(0);
//	BCMenu* pSelect = (BCMenu*) psub->GetSubMenu(1);
//	if (pSelect)
//	{
//		pSelect->AppendMenu(MF_STRING, ID_WINDOW_TILE_HORZ, _T("&Tile"));
//		pSelect->ModifyODMenu(NULL, ID_WINDOW_TILE_HORZ, IDB_WINDOW_TILE);
//	}
	psub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
	m_popmenu.DestroyMenu();
*/
}


// Popup menu handler for bcmenu
void CViewEx::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	BOOL bFlag = FALSE;
	if (lpMeasureItemStruct->CtlType == ODT_MENU)
	{
		if (IsMenu((HMENU) lpMeasureItemStruct->itemID) && 
			BCMenu::IsMenu((HMENU) lpMeasureItemStruct->itemID))
		{
			m_popmenu.MeasureItem(lpMeasureItemStruct);
			bFlag = TRUE;
		}
	}
	if (!bFlag) CView::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


// Popup menu handler for bcmenu
LRESULT CViewEx::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	LRESULT lresult;
	if (BCMenu::IsMenu(pMenu))
		lresult = BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lresult = CView::OnMenuChar(nChar, nFlags, pMenu);
	return (lresult);
}


// Initialize a popup menu for use with bcmenu routines.
// Returns a pointer to a BCMenu object
// Call this from the child view.
BCMenu* CViewEx::InitPopup(UINT nPopupID, UINT nDefaultID)
{
	m_popmenu.DestroyMenu();
	m_popmenu.LoadMenu(nPopupID);
	m_popmenu.LoadToolbar(IDR_MAINFRAME);
	//. load other toolbars here (all of them?)
	BCMenu* pPopup = DYNAMIC_DOWNCAST(BCMenu, m_popmenu.GetSubMenu(0));
	ASSERT(pPopup);
//	BCMenu* pSelect = (BCMenu*) psub->GetSubMenu(1);
//	if (pSelect)
//	{
//		pSelect->AppendMenu(MF_STRING, ID_WINDOW_TILE_HORZ, _T("&Tile"));
//		pSelect->ModifyODMenu(NULL, ID_WINDOW_TILE_HORZ, IDB_WINDOW_TILE);
//	}
//	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
//	m_popmenu.DestroyMenu();
	return pPopup;
}






//...

/*
// Add an object. 
// This is the default ID_OBJ_ADD handler.
void CViewEx::OnCmdAddObject() 
{
	if (m_pobjTarget)
	{
		//. pass UI ptr also!
		BObject* pobj = UIAddNewObject(m_pobjTarget);
	}
}
*/



//.. goes into endless loop
// because the childframe will hand the command to its child view. which is us

/*
from mfc:

BOOL CView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// first pump through pane
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// then pump through document
	if (m_pDocument != NULL)
	{
		// special state for saving view before routing to document
		CPushRoutingView push(this);
		return m_pDocument->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}

	return FALSE;
}
*/


// so it should be okay to have cmd handlers here in cviewex
// it just seems an odd place for them
// 
/*
BOOL CViewEx::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// 

	// call base class version - ie let this view have a crack at the cmd. 
	// i think this will pass it to the document next. 
	BOOL bHandled = CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	// if the view and the doc didn't handle it, try the child/doc frame. 
	if (!bHandled) {
		//. this is bombing because there's no childframe yet! so check. 
		CFrameChild* pframe = theApp.GetChildFrame();
		if (pframe)
			bHandled = pframe->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}

	return bHandled;
}
*/


