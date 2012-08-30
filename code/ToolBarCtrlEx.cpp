
// CToolBarCtrlEx




#include "precompiled.h"
#include "NeoMem.h"
#include "ToolBarCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// MFC internal message:
#ifndef WM_IDLEUPDATECMDUI
#define WM_IDLEUPDATECMDUI  0x0363  // wParam == bDisableIfNoHandler
#endif



// Macros
//--------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(CToolBarCtrlEx, CToolBarCtrl)

BEGIN_MESSAGE_MAP(CToolBarCtrlEx, CToolBarCtrl)
	//{{AFX_MSG_MAP(CToolBarCtrlEx)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	// i think only certain messages are reflected back to the control - maybe this isn't one of them
//	ON_NOTIFY_REFLECT(TTN_NEEDTEXT, OnToolTipText)
END_MESSAGE_MAP()



// Construction/Destruction
//--------------------------------------------------------------------------------------------

CToolBarCtrlEx::CToolBarCtrlEx()
{
	m_bDelayedButtonLayout = TRUE;
	
	m_hbmImageWell = 0;
	m_hInstImageWell = 0;
	m_hRsrcImageWell = 0;
	
	m_nCount = 0;

	// default image sizes
	m_sizeImage.cx = 16;
	m_sizeImage.cy = 15;

	// default button sizes
	m_sizeButton.cx = 23;
	m_sizeButton.cy = 22;
}

CToolBarCtrlEx::~CToolBarCtrlEx()
{
	AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);
//	delete m_pStringMap;
	m_nCount = 0;
}





// Message handlers
//--------------------------------------------------------------------------------------------

/*

// Window Styles

#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_MINIMIZE         0x20000000L
#define WS_VISIBLE          0x10000000L
#define WS_DISABLED         0x08000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_MAXIMIZE         0x01000000L
#define WS_CAPTION          0x00C00000L    
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_VSCROLL          0x00200000L
#define WS_HSCROLL          0x00100000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L


// Common Control Styles

#define CCS_TOP                 0x00000001L
#define CCS_NOMOVEY             0x00000002L
#define CCS_BOTTOM              0x00000003L
#define CCS_NORESIZE            0x00000004L
#define CCS_NOPARENTALIGN       0x00000008L
#define CCS_ADJUSTABLE          0x00000020L
#define CCS_NODIVIDER           0x00000040L
#define CCS_VERT                0x00000080L


// ToolBar Styles

#define TBSTYLE_BUTTON          0x0000
#define TBSTYLE_SEP             0x0001
#define TBSTYLE_CHECK           0x0002
#define TBSTYLE_GROUP           0x0004
#define TBSTYLE_CHECKGROUP      (TBSTYLE_GROUP | TBSTYLE_CHECK)
#define TBSTYLE_DROPDOWN        0x0008
#define TBSTYLE_AUTOSIZE        0x0010 // automatically calculate the cx of the button
#define TBSTYLE_NOPREFIX        0x0020 // if this button should not have accel prefix
#define TBSTYLE_TOOLTIPS        0x0100
#define TBSTYLE_WRAPABLE        0x0200
#define TBSTYLE_ALTDRAG         0x0400
#define TBSTYLE_FLAT            0x0800
#define TBSTYLE_LIST            0x1000
#define TBSTYLE_CUSTOMERASE     0x2000
#define TBSTYLE_REGISTERDROP    0x4000
#define TBSTYLE_TRANSPARENT     0x8000


// ToolBar Extended Styles

#define TBSTYLE_EX_DRAWDDARROWS 0x00000001

*/



// Called by the framework before the creation of the Windows window attached 
// to this CWnd object.
BOOL CToolBarCtrlEx::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Bug: Toolbar does NOT like: passing a CRect other than 0 with CCS_NOPARENTALIGN style -
	// stack overflows and uses up ALL system resources!!!!
	// Bug: Also does NOT like trying to move the toolbar when CSS_NOPARENTALIGN style is set -
	// does the same thing

	// Override styles passed to Create
	cs.style = 
			WS_CHILD | 
//			WS_VISIBLE |
			WS_CLIPCHILDREN |
			WS_CLIPSIBLINGS |
			CCS_TOP |
			CCS_NODIVIDER |
//			CCS_NOPARENTALIGN |
//			CCS_NORESIZE | 
//			CCS_NOMOVEY |
			TBSTYLE_FLAT |
//			TBSTYLE_LIST | // lets you add text to right of buttons (but then all buttons must be same size - LAME!)
			TBSTYLE_TRANSPARENT |
			TBSTYLE_TOOLTIPS |
			0;
	return CToolBarCtrl::PreCreateWindow(cs);
}






// The framework calls this member function when an application requests that the Windows 
// window be created by calling the Create or CreateEx member function. The CWnd object 
// receives this call after the window is created but before it becomes visible. OnCreate is 
// called before the Create or CreateEx member function returns. 
int CToolBarCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBarCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Set extended styles
	// Note: Draw down arrows requires 4.71 (IE4)
	//, Note: Mixed buttons requires 5.81 (Win 2000)
	// Call SetButtonDropDown after creating toolbar to set buttons as dropdown buttons.
//	this->SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS | TBSTYLE_EX_MIXEDBUTTONS);
	this->SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	
	return 0;
}


// Position the toolbar
// This MUST be called after creating the toolbar
void CToolBarCtrlEx::Position()
{
	// Get the size of the toolbar control
	CRect r;
	GetClientRect(r); // {top=0 bottom=26 left=0 right=397}	
	r.OffsetRect(2, 2); // shift into position

	// Turn off Top align and turn on NoResize style
	ModifyStyle(CCS_TOP, CCS_NORESIZE);

	// Move the toolbar to the new position and show it
	MoveWindow(r);
	ModifyStyle(0, WS_VISIBLE);
}


// code to create buttons manually

/*
		// Load standard images
		//. we'll load our own bitmap here from the resource file
		m_wndToolBar.LoadImages(IDB_STD_SMALL_COLOR, HINST_COMMCTRL);

		const int nButtons = 6;
		TBBUTTON tbArray[nButtons];

		//. how can you have an invisible separator?
		int i = 0;

		// 0
		tbArray[i].iBitmap = 0;
		tbArray[i].idCommand = 0;
		tbArray[i].fsState = TBSTATE_HIDDEN;
		tbArray[i].fsStyle = TBSTYLE_SEP;
		tbArray[i].dwData = 0;
		tbArray[i].iString = NULL;
		
		// 1
		i++;
		tbArray[i].iBitmap = STD_CUT;
		tbArray[i].idCommand = ID_EDIT_CUT;
		tbArray[i].fsState = TBSTATE_ENABLED;
		tbArray[i].fsStyle = TBSTYLE_BUTTON;
		tbArray[i].dwData = 0;
		tbArray[i].iString = NULL;

		// 2
		i++;
		tbArray[i].iBitmap = STD_COPY;
		tbArray[i].idCommand = ID_EDIT_COPY;
		tbArray[i].fsState = TBSTATE_ENABLED;
		tbArray[i].fsStyle = TBSTYLE_BUTTON;
		tbArray[i].dwData = 0;
		tbArray[i].iString = NULL;

		// 3
		i++;
		tbArray[i].iBitmap = STD_PASTE;
		tbArray[i].idCommand = ID_EDIT_PASTE_PLAIN;
		tbArray[i].fsState = TBSTATE_ENABLED;
		tbArray[i].fsStyle = TBSTYLE_BUTTON;
		tbArray[i].dwData = 0;
		tbArray[i].iString = NULL;

		// 4
		i++;
		tbArray[i].iBitmap = 0;
		tbArray[i].idCommand = 0;
		tbArray[i].fsState = 0;
		tbArray[i].fsStyle = TBSTYLE_SEP;
		tbArray[i].dwData = 0;
		tbArray[i].iString = NULL;
		
		// 5
		i++;
		tbArray[i].iBitmap = STD_UNDO;
		tbArray[i].idCommand = ID_EDIT_UNDO;
		tbArray[i].fsState = TBSTATE_ENABLED;
		tbArray[i].fsStyle = TBSTYLE_BUTTON;
		tbArray[i].dwData = 0;
		tbArray[i].iString = NULL;

		ASSERT (i < nButtons);
		m_wndToolBar.AddButtons(nButtons, &tbArray[0]);
//		if (!m_wndToolBar.AddButtons(nButtons, &tbArray[0]))
//			return -1;
*/





// Most of this code is from Bartool.cpp
//---------------------------------------------------------------------------------------------------


struct CToolBarData
{
	WORD wVersion;
	WORD wWidth;
	WORD wHeight;
	WORD wItemCount;
	//WORD aItems[wItemCount] // this is commented out in the mfc source also!

	WORD* items()
		{ return (WORD*)(this+1); }
};


// Load a toolbar specified by the resource ID
// LoadToolBar(MAKEINTRESOURCE(nIDResource));
BOOL CToolBarCtrlEx::LoadToolBar(LPCTSTR lpszResourceName, int nExtraHeight /* = 0 */)
{
	ASSERT_VALID(this);
	ASSERT(lpszResourceName != NULL);

	// Determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return FALSE;

	CToolBarData* pData = (CToolBarData*) LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;
	ASSERT(pData->wVersion == 1);

	// Set button size before loading them
	CSize sizeImage(pData->wWidth, pData->wHeight);
	CSize sizeButton(pData->wWidth + 7, pData->wHeight + 7 + nExtraHeight);
	SetSizes(sizeButton, sizeImage);

	// Initialize buttons
	UINT* pItems = new UINT[pData->wItemCount];
	for (int i = 0; i < pData->wItemCount; i++)
		pItems[i] = pData->items()[i];
	BOOL bResult = SetButtons(pItems, pData->wItemCount);
	delete[] pItems;

	// Load button images from bitmap now that sizes are known by the toolbar control.
	if (bResult)
	{
		bResult = LoadBitmap(lpszResourceName);
	}

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return bResult;

}



BOOL CToolBarCtrlEx::SetButtons(const UINT* lpIDArray, int nIDCount)
{
	ASSERT_VALID(this);
	ASSERT(nIDCount >= 1);  // must be at least one of them
	ASSERT(lpIDArray == NULL ||
		AfxIsValidAddress(lpIDArray, sizeof(UINT) * nIDCount, FALSE));

	// delete all existing buttons
	int nCount = (int)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
	while (nCount--)
		VERIFY(DefWindowProc(TB_DELETEBUTTON, 0, 0));

	TBBUTTON button; 
	memset(&button, 0, sizeof(TBBUTTON));
	button.iString = -1;
	if (lpIDArray != NULL)
	{
		// add new buttons to the common control
		int iImage = 0;
		for (int i = 0; i < nIDCount; i++)
		{
			button.fsState = TBSTATE_ENABLED;
			if ((button.idCommand = *lpIDArray++) == 0)
			{
				// separator
				button.fsStyle = TBSTYLE_SEP;
				// width of separator includes 8 pixel overlap
//`				ASSERT(_afxComCtlVersion != -1);
//`				if ((GetStyle() & TBSTYLE_FLAT) || _afxComCtlVersion == VERSION_IE4)
					button.iBitmap = 6;
//`				else
//`					button.iBitmap = 8;
			}
			else
			{
				// a command button with image
				button.fsStyle = TBSTYLE_BUTTON;
				button.iBitmap = iImage++;
			}
			if (!DefWindowProc(TB_ADDBUTTONS, 1, (LPARAM)&button))
				return FALSE;
		}
	}
	else
	{
		// add 'blank' buttons
		button.fsState = TBSTATE_ENABLED;
		for (int i = 0; i < nIDCount; i++)
		{
			ASSERT(button.fsStyle == TBSTYLE_BUTTON);
			if (!DefWindowProc(TB_ADDBUTTONS, 1, (LPARAM)&button))
				return FALSE;
		}
	}
	m_nCount = (int)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
	m_bDelayedButtonLayout = TRUE;

	return TRUE;
}



void CToolBarCtrlEx::SetSizes(SIZE sizeButton, SIZE sizeImage)
{
	ASSERT_VALID(this);

	// sizes must be non-zero and positive
	ASSERT(sizeButton.cx > 0 && sizeButton.cy > 0);
	ASSERT(sizeImage.cx > 0 && sizeImage.cy > 0);

	// button must be big enough to hold image
	//   + 7 pixels on x
	//   + 6 pixels on y
	ASSERT(sizeButton.cx >= sizeImage.cx + 7);
	ASSERT(sizeButton.cy >= sizeImage.cy + 6);

	if (::IsWindow(m_hWnd))
	{
		// set the sizes via TB_SETBITMAPSIZE and TB_SETBUTTONSIZE
		VERIFY(SendMessage(TB_SETBITMAPSIZE, 0, MAKELONG(sizeImage.cx, sizeImage.cy)));
		VERIFY(SendMessage(TB_SETBUTTONSIZE, 0, MAKELONG(sizeButton.cx, sizeButton.cy)));

		Invalidate();   // just to be nice if called when toolbar is visible
	}
	else
	{
		// just set our internal values for later
		m_sizeButton = sizeButton;
		m_sizeImage = sizeImage;
	}
}




BOOL CToolBarCtrlEx::LoadBitmap(LPCTSTR lpszResourceName)
{
	ASSERT_VALID(this);
	ASSERT(lpszResourceName != NULL);

	// determine location of the bitmap in resource fork
	HINSTANCE hInstImageWell = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);
	HRSRC hRsrcImageWell = ::FindResource(hInstImageWell, lpszResourceName, RT_BITMAP);
	if (hRsrcImageWell == NULL)
		return FALSE;

	// load the bitmap
	HBITMAP hbmImageWell;
	hbmImageWell = AfxLoadSysColorBitmap(hInstImageWell, hRsrcImageWell);

	// tell common control toolbar about the new bitmap
	if (!AddReplaceBitmap(hbmImageWell))
		return FALSE;

	// remember the resource handles so the bitmap can be recolored if necessary
	m_hInstImageWell = hInstImageWell;
	m_hRsrcImageWell = hRsrcImageWell;
	return TRUE;
}



BOOL CToolBarCtrlEx::AddReplaceBitmap(HBITMAP hbmImageWell)
{
	// need complete bitmap size to determine number of images
	BITMAP bitmap;
	VERIFY(::GetObject(hbmImageWell, sizeof(BITMAP), &bitmap));

	// add the bitmap to the common control toolbar
	BOOL bResult;
	if (m_hbmImageWell == NULL)
	{
		TBADDBITMAP addBitmap;
		addBitmap.hInst = NULL; // makes TBADDBITMAP::nID behave a HBITMAP
		addBitmap.nID = (UINT)hbmImageWell;
		bResult =  DefWindowProc(TB_ADDBITMAP,
			bitmap.bmWidth / m_sizeImage.cx, (LPARAM)&addBitmap) == 0;
	}
	else
	{
		TBREPLACEBITMAP replaceBitmap;
		replaceBitmap.hInstOld = NULL;
		replaceBitmap.nIDOld = (UINT)m_hbmImageWell;
		replaceBitmap.hInstNew = NULL;
		replaceBitmap.nIDNew = (UINT)hbmImageWell;
		replaceBitmap.nButtons = bitmap.bmWidth / m_sizeImage.cx;
		bResult = (BOOL)DefWindowProc(TB_REPLACEBITMAP, 0,
			(LPARAM)&replaceBitmap);
	}
	// remove old bitmap, if present
	if (bResult)
	{
		AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);
		m_hbmImageWell = hbmImageWell;
	}

	return bResult;
}


UINT CToolBarCtrlEx::GetButtonStyle(int nIndex) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	TBBUTTON button;
	_GetButton(nIndex, &button);
	return MAKELONG(button.fsStyle, button.fsState);
}


void CToolBarCtrlEx::SetButtonStyle(int nIndex, UINT nStyle)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	TBBUTTON button;
	_GetButton(nIndex, &button); 
	if (button.fsStyle != (BYTE)LOWORD(nStyle) || button.fsState != (BYTE)HIWORD(nStyle))
	{
		button.fsStyle = (BYTE)LOWORD(nStyle);
		button.fsState = (BYTE)HIWORD(nStyle);
		_SetButton(nIndex, &button);
		m_bDelayedButtonLayout = TRUE;
	}
}



void CToolBarCtrlEx::_GetButton(int nIndex, TBBUTTON* pButton) const
{
	//, why is this okay? casting a const pointer to a const object to a plain pointer? 
	CToolBarCtrlEx* pBar = (CToolBarCtrlEx*)this;
	VERIFY(pBar->DefWindowProc(TB_GETBUTTON, nIndex, (LPARAM)pButton));
	// TBSTATE_ENABLED == TBBS_DISABLED so invert it
	pButton->fsState ^= TBSTATE_ENABLED;
}




void CToolBarCtrlEx::_SetButton(int nIndex, TBBUTTON* pButton)
{
	// get original button state
	TBBUTTON button;
	VERIFY(DefWindowProc(TB_GETBUTTON, nIndex, (LPARAM)&button));

	// prepare for old/new button comparsion
	button.bReserved[0] = 0;
	button.bReserved[1] = 0;
	// TBSTATE_ENABLED == TBBS_DISABLED so invert it
	pButton->fsState ^= TBSTATE_ENABLED;
	pButton->bReserved[0] = 0;
	pButton->bReserved[1] = 0;

	// nothing to do if they are the same
	if (memcmp(pButton, &button, sizeof(TBBUTTON)) != 0)
	{
		// don't redraw everything while setting the button
		DWORD dwStyle = GetStyle();
		ModifyStyle(WS_VISIBLE, 0);
		VERIFY(DefWindowProc(TB_DELETEBUTTON, nIndex, 0));
		VERIFY(DefWindowProc(TB_INSERTBUTTON, nIndex, (LPARAM)pButton));
		ModifyStyle(0, dwStyle & WS_VISIBLE);

		// invalidate appropriate parts
		if (((pButton->fsStyle ^ button.fsStyle) & TBSTYLE_SEP) ||
			((pButton->fsStyle & TBSTYLE_SEP) && pButton->iBitmap != button.iBitmap))
		{
			// changing a separator
			Invalidate();
		}
		else
		{
			// invalidate just the button
			CRect rect;
			if (DefWindowProc(TB_GETITEMRECT, nIndex, (LPARAM)&rect))
				InvalidateRect(rect);
		}
	}
}




// from WinUtil.cpp
void CToolBarCtrlEx::AfxDeleteObject(HGDIOBJ* pObject)
{
	ASSERT(pObject != NULL);
	if (*pObject != NULL)
	{
		DeleteObject(*pObject);
		*pObject = NULL;
	}
}




// Idle update processing
//--------------------------------------------------------------------------------------------


// This is the handler for the WM_IDLEUPDATECMDUI message, which is sent by the main
// thread when there are no other messages in the message queue.
LRESULT CToolBarCtrlEx::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	if (GetStyle() & WS_VISIBLE)
	{
		// We should make the target the view containing the toolbar, not the child frame (duh!).
		// MFC was built to only allow toolbars in frame windows, so modifying to allow them in cviews.
		CView* pTarget = DYNAMIC_DOWNCAST(CView, GetParent());
		ASSERT_VALID(pTarget); // will fail here if parent is not a cview
		OnUpdateCmdUI(pTarget, (BOOL)wParam);
	}
	return 0L;
}




// CToolCmdUI
// CCmdUI derived class used to update status of toolbar buttons.
// This is taken from bartool.cpp file.
// Modified to work with our toolbar extension class.
// Renamed to CToolCmdUI2 to avoid conflict with bartool.cpp's version when you do static linking.
//---------------------------------------------------------------------------------------------------------------------------
class CToolCmdUI2 : public CCmdUI        // class private to this file !
{
public: // re-implementations only
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetText(LPCTSTR lpszText);
};


// Enable  a button
void CToolCmdUI2::Enable(BOOL bOn)
{
//	if (m_nID == ID_FORMAT_BOLD)
//		TRACE("CToolCmdUI Enable ID_FORMAT_BOLD %d\n", bOn);

	m_bEnableChanged = TRUE;
	CToolBarCtrlEx* pToolBar = (CToolBarCtrlEx*) m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT_KINDOF(CToolBarCtrlEx, pToolBar); // bomb?
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) & ~TBBS_DISABLED;
	if (!bOn)
	{
		nNewStyle |= TBBS_DISABLED;
		// WINBUG: If a button is currently pressed and then is disabled
		// COMCTL32.DLL does not unpress the button, even after the mouse
		// button goes up!  We work around this bug by forcing TBBS_PRESSED
		// off when a button is disabled.
		nNewStyle &= ~TBBS_PRESSED;
	}
	ASSERT(!(nNewStyle & TBBS_SEPARATOR));
	pToolBar->SetButtonStyle(m_nIndex, nNewStyle);
}


void CToolCmdUI2::SetCheck(int nCheck)
{
//	if (m_nID == ID_FORMAT_BOLD)
//		TRACE("CToolCmdUI SetCheck ID_FORMAT_BOLD %d\n", nCheck);
//	TRACE("CToolCmdUI SetCheck for button %d to state %d\n", this->m_nID, nCheck);

	ASSERT(nCheck >= 0 && nCheck <= 2); // 0=>off, 1=>on, 2=>indeterminate
	CToolBarCtrlEx* pToolBar = (CToolBarCtrlEx*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT_KINDOF(CToolBarCtrlEx, pToolBar); // bomb?
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) &
				~(TBBS_CHECKED | TBBS_INDETERMINATE);
	if (nCheck == 1)
		nNewStyle |= TBBS_CHECKED;
	else if (nCheck == 2)
		nNewStyle |= TBBS_INDETERMINATE;
	ASSERT(!(nNewStyle & TBBS_SEPARATOR));
	pToolBar->SetButtonStyle(m_nIndex, nNewStyle | TBBS_CHECKBOX);
}


void CToolCmdUI2::SetText(LPCTSTR)
{
	// ignore it
}




// This is called by OnIdleUpdateCmdUI, the handler for the WM_IDLEUPDATECMDUI message.
// This walks through all the buttons in the toolbar and sends the CN_UPDATE_COMMAND_UI
// message to the specified target (in this case, the parent view).
// Note: This has to come after CToolCmdUI definition, since it uses it.
// Note: The target was changed from CFrameWnd to CView.
void CToolBarCtrlEx::OnUpdateCmdUI(CView* pTarget, BOOL bDisableIfNoHndler)
{
//	CRuntimeClass* pClass = pTarget->GetRuntimeClass();
//	TRACE("CToolBarCtrlEx OnUpdateCmdUI - target is %s\n", (LPCTSTR) pClass->m_lpszClassName);

	ASSERT_VALID(pTarget);

	CToolCmdUI2 state;
	state.m_pOther = this;

	// walk through buttons
	state.m_nIndexMax = (UINT) DefWindowProc(TB_BUTTONCOUNT, 0, 0); // number of buttons
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
	{
		// get button's state
		TBBUTTON button;
		_GetButton(state.m_nIndex, &button);
		state.m_nID = button.idCommand;

		// ignore separators
		if (!(button.fsStyle & TBSTYLE_SEP))
		{
			// allow reflections
			// OnCmdMsg - Called by the framework to route and dispatch command messages and to 
			// handle the update of command user-interface objects. 
			if (CWnd::OnCmdMsg(0,
				MAKELONG((int) CN_UPDATE_COMMAND_UI, WM_COMMAND+WM_REFLECT_BASE), &state, NULL))
				continue;

			// allow the toolbar itself to have update handlers
			if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
				continue;

			// allow the owner to process the update
			// This sends the CN_UPDATE_COMMAND_UI to the specified target,
			// and disables the item if there is no handler - bpb
			state.DoUpdate(pTarget, bDisableIfNoHndler);
		}
	}

	// update the dialog controls added to the toolbar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}


/*
void CToolBarCtrlEx::OnToolTipText(NMHDR* pnm, LRESULT* result)
{
	xTRACE("provide tooltip text!\n");

	*result = 0;
}
*/



BOOL CToolBarCtrlEx::GetItemRectFromID(UINT nID, LPRECT lpRect)
{
	int nIndex = CommandToIndex(nID);
	return GetItemRect(nIndex, lpRect);
}



// Set the specified button to be a dropdown button
void CToolBarCtrlEx::SetButtonDropDown(UINT nButtonID)
{
	int nIndex = CommandToIndex(nButtonID);
	SetButtonStyle(nIndex, TBSTYLE_DROPDOWN);
}



int CToolBarCtrlEx::IndexToCommand(int nItem)
{
	TBBUTTON tbb;
	::ZeroMemory(&tbb, sizeof(TBBUTTON));
	if (this->SendMessage(TB_GETBUTTON, nItem, (LPARAM) &tbb))
		return tbb.idCommand;
	return 0;
}



// Set a button's style and width (in pixels).
void CToolBarCtrlEx::SetButtonInfo2(UINT nID, BYTE bytStyle, WORD wWidth)
{
	TBBUTTONINFO tbi;
	::ZeroMemory(&tbi, sizeof(TBBUTTONINFO));
	tbi.cbSize = sizeof(TBBUTTONINFO);

	tbi.dwMask = TBIF_SIZE | TBIF_STYLE;
	tbi.cx = wWidth;// width in pixels
	tbi.fsStyle = bytStyle; // TBSTYLE_SEP;
	VERIFY(SetButtonInfo(nID, &tbi));
}


// Get dimensions of the specified button.
BOOL CToolBarCtrlEx::GetItemRectFromID(UINT nID, CRect &r)
{
	UINT nIndex = CommandToIndex(nID);
	return GetItemRect(nIndex, &r);
}



void CToolBarCtrlEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	//. this bombs occasionally...	
	// just goes to wm_mousemove handler for control
	// so maybe the buttons aren't set up right?

	CToolBarCtrl::OnMouseMove(nFlags, point);
}
