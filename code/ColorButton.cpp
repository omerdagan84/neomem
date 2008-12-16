// AUTHOR:	James White (feel free to remove or otherwise mangle any part)


#include "precompiled.h"
#include "ColorButton.h"

#include "..\resources\resource.h" //`
#include "stringex.h" //`
#include "neomem.h" //`


//` from msdn
/*
//. need this for win95
// actually will work if they have ie3 installed!
#if (_WIN32_WINNT < 0x0400)
#define WM_MOUSELEAVE	WM_USER+2
#define TME_LEAVE				1

typedef struct tagTRACKMOUSEEVENT 
{
	DWORD cbSize;
	DWORD dwFlags;
	HWND  hwndTrack;
} TRACKMOUSEEVENT, *LPTRACKMOUSEEVENT;

VOID CALLBACK TrackMouseTimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime) 
{
	RECT rect;
	POINT pt;
	GetClientRect(hWnd, &rect);
	MapWindowPoints(hWnd, NULL, (LPPOINT)&rect, 2);
	GetCursorPos(&pt);
	if (!PtInRect(&rect, pt) || (WindowFromPoint(pt) != hWnd)) 
	{
		if (!KillTimer(hWnd, idEvent)) 
		{
			// Error killing the timer!
		}
		PostMessage(hWnd, WM_MOUSELEAVE, 0, 0);
	}
}

BOOL TrackMouseEvent(LPTRACKMOUSEEVENT ptme) 
{
	OutputDebugString(TEXT("TrackMouseEvent\n"));
	
	if (!ptme || ptme->cbSize < sizeof(TRACKMOUSEEVENT)) 
	{
		OutputDebugString(TEXT("TrackMouseEvent: invalid TRACKMOUSEEVENT structure\n"));
		return FALSE;
	}
	
	if (!IsWindow(ptme->hwndTrack)) 
	{
		OutputDebugString(TEXT("TrackMouseEvent: invalid hwndTrack\n"));
		return FALSE;
	}
	
	if (!(ptme->dwFlags & TME_LEAVE)) {OutputDebugString(TEXT("TrackMouseEvent: invalid dwFlags\n"));
		return FALSE;
	}
	
	return SetTimer(ptme->hwndTrack, ptme->dwFlags, 100, (TIMERPROC) TrackMouseTimerProc);
}

#endif
*/







//**						 MFC Debug Symbols						   **
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif






//**						 Message Handlers						  **
BEGIN_MESSAGE_MAP(CColorButton, CButton)
//BEGIN_MESSAGE_MAP(CColorButton, CWnd)
	//{{AFX_MSG_MAP(CColorButton)
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELENDOK,	 OnSelEndOK)
	ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
	ON_MESSAGE(CPN_SELCHANGE,	 OnSelChange)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave) //`
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()




//**							DDX Method							  **

void AFXAPI DDX_ColorButton(CDataExchange *pDX, int nIDC, COLORREF& crColour)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	ASSERT (hWndCtrl != NULL);				  
	
	CColorButton* pColourButton = (CColorButton*) CWnd::FromHandle(hWndCtrl);
	if (pDX->m_bSaveAndValidate)
	{
		crColour = pColourButton->Color;
	}
	else // initializing
	{
		pColourButton->Color = crColour;
	}
}

//**							 Constants							   **
const int g_ciDropDownSizeX = 4 ;
const int g_ciDropDownSizeY = 2 ;

//**							MFC Macros							  **
IMPLEMENT_DYNCREATE(CColorButton, CButton)
//IMPLEMENT_DYNCREATE(CColorButton, CWnd)

// Method:	CColorButton::CColorButton(void)
// Notes:	Default Constructor.
CColorButton::CColorButton(void):
	_Inherited(),
	m_Color(CLR_DEFAULT),
	m_DefaultColor(::GetSysColor(COLOR_APPWORKSPACE)),
	m_strDefaultText(_T("Automatic")),
	m_strCustomText(_T("More Colors...")),
	m_bPopupActive(FALSE),
	m_bTrackSelection(FALSE)
{
		m_bDrawEdge = FALSE; //`
		m_uButtonID = 0;
		m_uDropDownID = 0;
}



BOOL CColorButton::Create2(const RECT& rect, CWnd* pParentWnd, UINT uButtonID, UINT uDropDownID)
{
	m_uButtonID = uButtonID;
	m_uDropDownID = uDropDownID;

//	if (!CButton::Create("", WS_CHILD | WS_VISIBLE, rect, pParentWnd, uButtonID))
	if (!_Inherited::Create("", WS_CHILD | WS_VISIBLE, rect, pParentWnd, uButtonID))
//	CString strClassName = AfxRegisterWndClass(CS_CLASSDC | CS_SAVEBITS | CS_HREDRAW | CS_VREDRAW,
//											  (HBRUSH) (COLOR_BTNFACE+1), 
//											  0,
//											  0);
//	if (!CWnd::Create("BUTTON", "", WS_CHILD | WS_VISIBLE, rect, pParentWnd, uButtonID))
//	if (!CWnd::Create(strClassName, "", WS_CHILD | WS_VISIBLE, rect, pParentWnd, uButtonID))
		return FALSE;

	return TRUE;
}



//***********************************************************************
// Method:	CColorButton::~CColorButton(void)
// Notes:	Destructor.
//***********************************************************************
CColorButton::~CColorButton(void)
{
}

//***********************************************************************
// Method:	CColorButton::GetColor()
// Notes:	None.
//***********************************************************************
COLORREF CColorButton::GetColor(void) const
{
	return m_Color;
}


//***********************************************************************
// Method:	CColorButton::SetColor()
// Notes:	None.
//***********************************************************************
void CColorButton::SetColor(COLORREF Color)
{
	m_Color = Color;

	if (::IsWindow(m_hWnd)) 
		RedrawWindow();
}


//***********************************************************************
// Method:	CColorButton::GetDefaultColor()
// Notes:	None.
//***********************************************************************
COLORREF CColorButton::GetDefaultColor(void) const
{
	return m_DefaultColor;
}

//***********************************************************************
// Method:	CColorButton::SetDefaultColor()
// Notes:	None.
//***********************************************************************
void CColorButton::SetDefaultColor(COLORREF Color)
{
	m_DefaultColor = Color;
}

//***********************************************************************
// Method:	CColorButton::SetCustomText()
// Notes:	None.
//***********************************************************************
void CColorButton::SetCustomText(LPCTSTR tszText)
{
	m_strCustomText = tszText;
}

//***********************************************************************
// Method:	CColorButton::SetDefaultText()
// Notes:	None.
//***********************************************************************
void CColorButton::SetDefaultText(LPCTSTR tszText)
{
	m_strDefaultText = tszText;
}


//***********************************************************************
// Method:	CColorButton::SetTrackSelection()
// Notes:	None.
//***********************************************************************
void CColorButton::SetTrackSelection(BOOL bTrack)
{
	m_bTrackSelection = bTrack;
}

//***********************************************************************
// Method:	CColorButton::GetTrackSelection()
// Notes:	None.
//***********************************************************************
BOOL CColorButton::GetTrackSelection(void) const
{
	return m_bTrackSelection;
}

//***********************************************************************
//**						 CButton Overrides						   **
//***********************************************************************
void CColorButton::PreSubclassWindow() 
{
	ModifyStyle(0, BS_OWNERDRAW);	   

	_Inherited::PreSubclassWindow();
}


//***********************************************************************
// Method:	CColorButton::OnSelEndOK()
// Notes:	None.
//***********************************************************************
//LONG CColorButton::OnSelEndOK(UINT lParam, LONG /*wParam*/)
LPARAM CColorButton::OnSelEndOK(WPARAM wParam, LPARAM /*lParam*/) //`
{
	m_bPopupActive = FALSE;

	COLORREF OldColor = m_Color;
	
//	Color = (COLORREF)lParam;
	Color = (COLORREF) wParam; //`

	CWnd *pParent = GetParent();

	if (pParent) 
	{
//		pParent->SendMessage(CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
//		pParent->SendMessage(CPN_SELENDOK, lParam, (WPARAM) GetDlgCtrlID());
		pParent->SendMessage(CPN_CLOSEUP, wParam, (LPARAM) GetDlgCtrlID());
		pParent->SendMessage(CPN_SELENDOK, wParam, (LPARAM) GetDlgCtrlID());
	}

	if (OldColor != m_Color)
//		if (pParent) pParent->SendMessage(CPN_SELCHANGE, m_Color, (WPARAM) GetDlgCtrlID());
		if (pParent) pParent->SendMessage(CPN_SELCHANGE, m_Color, (LPARAM) GetDlgCtrlID());

	// Save new color and redraw
	m_Color = Color; //`
	RedrawWindow(); //`

	return TRUE;
}


//***********************************************************************
// Method:	CColorButton::OnSelEndCancel()
// Notes:	None.
//***********************************************************************
LONG CColorButton::OnSelEndCancel(UINT lParam, LONG /*wParam*/)
{
	m_bPopupActive = FALSE;
	
	Color = (COLORREF) lParam;

	CWnd *pParent = GetParent();

	if (pParent) 
	{
		pParent->SendMessage(CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
		pParent->SendMessage(CPN_SELENDCANCEL, lParam, (WPARAM) GetDlgCtrlID());
	}

	return TRUE;
}


//***********************************************************************
// Method:	CColorButton::OnSelChange()
// Notes:	None.
//***********************************************************************
LONG CColorButton::OnSelChange(UINT lParam, LONG /*wParam*/)
{
	if (m_bTrackSelection) 
		Color = (COLORREF)lParam;

	CWnd *pParent = GetParent();

	if (pParent) pParent->SendMessage(CPN_SELCHANGE, lParam, (WPARAM) GetDlgCtrlID());

	return TRUE;
}

//***********************************************************************
// Method:	CColorButton::OnCreate()
// Notes:	None.
//***********************************************************************
int CColorButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
//	if (CButton::OnCreate(lpCreateStruct) == -1)
	if (_Inherited::OnCreate(lpCreateStruct) == -1)
		return -1;

	//` Create the tooltips
	CreateToolTips();

	return 0;
}

//***********************************************************************
// Method:	CColorButton::OnClicked()
// Notes:	None.
//***********************************************************************
BOOL CColorButton::OnClicked()
{
	m_bPopupActive = TRUE;

	CRect rDraw;
	GetWindowRect(rDraw);

	//`
	CPoint ptScreen(::GetMessagePos()); // Get position of message source
	CPoint ptClient = ptScreen;
	ScreenToClient(&ptClient);

	//` Added case for click on button vs arrow
	if (ptClient.x < 24) // clicked on button
	{
		// Return False to pass the message on to the parent!
		// If your handler function returns TRUE, MFC interprets this to mean you have handled the 
		// message and MFC will not route it to the parent; if you return FALSE, MFC will route the 
		// message to the parent window from which the message originated. 
		CWnd *pParent = GetParent();
		if (pParent)
//			pParent->SendMessage(WM_COMMAND, (WPARAM) GetDlgCtrlID(), (LPARAM) m_hWnd); //`
			pParent->SendMessage(CBN_CLICK, (WPARAM) m_Color, (LPARAM) m_uButtonID); //`

//		return FALSE;
	}
	else // clicked on DropDown
	{
		DropDown(FALSE); //`
	}

	return TRUE;
}




//***********************************************************************
//**						  Static Methods						  **
//***********************************************************************

//***********************************************************************
// Method:	CColorButton::DrawDropDown()
// Notes:	None.
//***********************************************************************
void CColorButton::DrawDropDown(CDC* pDC, 
							 RECT* pRect, 
							 int iDirection,
							 COLORREF clrDropDown /*= RGB(0,0,0)*/)
{
	POINT ptsDropDown[3];

	switch (iDirection)
	{
		case 0 : // Down
		{
			ptsDropDown[0].x = pRect->left;
			ptsDropDown[0].y = pRect->top;
			ptsDropDown[1].x = pRect->right;
			ptsDropDown[1].y = pRect->top;
			ptsDropDown[2].x = (pRect->left + pRect->right)/2;
			ptsDropDown[2].y = pRect->bottom;
			break;
		}

		case 1 : // Up
		{
			ptsDropDown[0].x = pRect->left;
			ptsDropDown[0].y = pRect->bottom;
			ptsDropDown[1].x = pRect->right;
			ptsDropDown[1].y = pRect->bottom;
			ptsDropDown[2].x = (pRect->left + pRect->right)/2;
			ptsDropDown[2].y = pRect->top;
			break;
		}

		case 2 : // Left
		{
			ptsDropDown[0].x = pRect->right;
			ptsDropDown[0].y = pRect->top;
			ptsDropDown[1].x = pRect->right;
			ptsDropDown[1].y = pRect->bottom;
			ptsDropDown[2].x = pRect->left;
			ptsDropDown[2].y = (pRect->top + pRect->bottom)/2;
			break;
		}

		case 3 : // Right
		{
			ptsDropDown[0].x = pRect->left;
			ptsDropDown[0].y = pRect->top;
			ptsDropDown[1].x = pRect->left;
			ptsDropDown[1].y = pRect->bottom;
			ptsDropDown[2].x = pRect->right;
			ptsDropDown[2].y = (pRect->top + pRect->bottom)/2;
			break;
		}
	}
	
	CBrush brsDropDown(clrDropDown);
	CPen penDropDown(PS_SOLID, 1 , clrDropDown);

	CBrush* pOldBrush = pDC->SelectObject(&brsDropDown);
	CPen*	pOldPen   = pDC->SelectObject(&penDropDown);
	
	pDC->SetPolyFillMode(WINDING);
	pDC->Polygon(ptsDropDown, 3);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}




//`Track mouse so we know when it leaves (for hotbutton drawing)
void CColorButton::OnMouseMove(UINT nFlags, CPoint ptClient)
{
	TRACKMOUSEEVENT tme;
	if (m_bDrawEdge == FALSE)
	{
		xTRACE("OnMouseEnter\n");
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;

		// The TrackMouseEvent function posts messages when the mouse pointer leaves a 
		// window or hovers over a window for a specified amount of time.
		// Note  The _TrackMouseEvent function calls TrackMouseEvent if it exists, otherwise 
		// _TrackMouseEvent emulates TrackMouseEvent. The _TrackMouseEvent function is in 
		// commctrl.h and is exported by COMCTRL32.DLL. 
		if (!::_TrackMouseEvent(&tme)) 
		{
			AfxMessageBox("TrackMouseEvent Failed");
		}
		
		m_bDrawEdge = TRUE;
		RedrawWindow();
	}
}



// WM_MOUSELEAVE handler.
// Message indicates that the mouse left the client area of the window specified in a prior 
// call to TrackMouseEvent. All tracking requested by TrackMouseEvent is canceled when this 
// message is generated. The application must call TrackMouseEvent when the mouse reenters 
// its window if it requires further tracking of mouse hover behavior.
// If an application processes this message, it should return zero. 
LPARAM CColorButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	xTRACE("OnMouseLeave\n");
	m_bDrawEdge = FALSE;
	RedrawWindow();
	return 0;
}









//***********************************************************************
// Method:	CColorButton::DrawItem()
// Notes:	None.
//***********************************************************************
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct);

	CDC*	pDC 	 = CDC::FromHandle(lpDrawItemStruct->hDC);
	UINT	state	 = lpDrawItemStruct->itemState;
	CRect	rDraw	 = lpDrawItemStruct->rcItem;
	CRect	rDropDown;

	if (m_bPopupActive)
		state |= ODS_SELECTED | ODS_FOCUS;


	//` Fill background
	pDC->FillSolidRect(rDraw, ::GetSysColor(COLOR_3DFACE));

	// Draw Outer Edge
//	UINT uFrameState = DFCS_BUTTONPUSH | DFCS_ADJUSTRECT;
//	if (state & ODS_SELECTED)
//		uFrameState |= DFCS_PUSHED;
//	if (state & ODS_DISABLED)
//		uFrameState |= DFCS_INACTIVE;
//	uFrameState |= DFCS_FLAT; //`
//	uFrameState |= DFCS_HOT;
//	uFrameState |= DFCS_MONO;
	if (m_bDrawEdge) //`
	{
//		pDC->DrawFrameControl(&rDraw,
//							DFC_BUTTON,
//							uFrameState);
//		pDC->DrawEdge(&rDraw, EDGE_ETCHED, BF_RECT);
//		pDC->DrawEdge(&rDraw, BDR_RAISEDOUTER, BF_RECT);
		CRect r;
		r = rDraw;
		r.right = 24;
		pDC->DrawEdge(&r, BDR_RAISEDINNER, BF_RECT);
		r.left = 24;
		r.right = 36;
		pDC->DrawEdge(&r, BDR_RAISEDINNER, BF_RECT);
	}

	if (state & ODS_SELECTED)
		rDraw.OffsetRect(1,1);

	//**					 Draw Focus
/*
	if (state & ODS_FOCUS) 
	{
		RECT rFocus = {rDraw.left,
					   rDraw.top,
					   rDraw.right - 1,
					   rDraw.bottom};
  
		pDC->DrawFocusRect(&rFocus);
	}
*/
	rDraw.DeflateRect(::GetSystemMetrics(SM_CXEDGE),
					  ::GetSystemMetrics(SM_CYEDGE));

	// Draw DropDown
//	rDropDown.left 	= rDraw.right - g_ciDropDownSizeX - ::GetSystemMetrics(SM_CXEDGE) /2;
	rDropDown.left 	= rDraw.right - g_ciDropDownSizeX - ::GetSystemMetrics(SM_CXEDGE) / 2 - 2;
	rDropDown.right	= rDropDown.left + g_ciDropDownSizeX;
	rDropDown.top		= (rDraw.bottom + rDraw.top)/2 - g_ciDropDownSizeY / 2;
	rDropDown.bottom	= (rDraw.bottom + rDraw.top)/2 + g_ciDropDownSizeY / 2;
	DrawDropDown(pDC, &rDropDown, 0, (state & ODS_DISABLED) ? ::GetSysColor(COLOR_GRAYTEXT) : RGB(0,0,0));
	rDraw.right = rDropDown.left - ::GetSystemMetrics(SM_CXEDGE)/2;


	// Draw Separator
//	pDC->DrawEdge(&rDraw, EDGE_ETCHED, BF_RIGHT);
//	rDraw.right -= (::GetSystemMetrics(SM_CXEDGE) * 2) + 1 ;

	//` Draw icon 
	//, hardcoded for now
//	UINT uIconID = (GetDlgCtrlID() == ID_FORMAT_FORECOLOR) ? IDI_FORECOLOR : IDI_BACKCOLOR;
	UINT uIconID = (m_uDropDownID == ID_FORMAT_FORECOLOR) ? IDI_FORECOLOR : IDI_BACKCOLOR;
	HICON hIcon = AfxGetApp()->LoadIcon(uIconID);

	CPoint ptOffset(0, 0);
	if (state & ODS_SELECTED)
	{
		ptOffset.x = 1;
		ptOffset.y = 1;
	}


	CPoint ptStart(3, 3);
	ptStart.Offset(ptOffset);
	if (hIcon)
		pDC->DrawIcon(ptStart, hIcon);


	// Draw Color
	if ((state & ODS_DISABLED) == 0)
	{
		rDraw.SetRect(1, 12, 15, 14);
//		rDraw.OffsetRect(3, 3);
		rDraw.OffsetRect(ptStart);
//		rDraw.DeflateRect(1, 1, 1, 1);
		pDC->FillSolidRect(&rDraw,
						   (m_Color == CLR_DEFAULT)
						   ? m_DefaultColor
						   : m_Color);
//		::FrameRect(pDC->m_hDC,
//					&rDraw,
//					(HBRUSH)::GetStockObject(BLACK_BRUSH));

	}
}




//` For tooltips
BOOL CColorButton::PreTranslateMessage(MSG* pMsg) 
{
	m_ToolTip.RelayEvent(pMsg);

    // Fix (Adrian Roman): Sometimes if the picker loses focus it is never destroyed
//	if (GetCapture()->GetSafeHwnd() != m_hWnd)
//		SetCapture(); 

	// 
//	if (pMsg->message == WM_COMMAND)
//	{
//	}

//    return CWnd::PreTranslateMessage(pMsg);
    return _Inherited::PreTranslateMessage(pMsg);
}



//`Set tooltip for button based on string resources
void CColorButton::CreateToolTips()
{
	const int nDividerPos = 24; //, hardcoded

	// Create the tool tip object, which contains multiple "tools"
	if (!m_ToolTip.Create(this)) return;

	// Add a tool for button 
	CRect r;
	GetClientRect(r);
	r.right = nDividerPos; // just want left part of button now
	CStringEx str;
	str.LoadString(m_uButtonID);
	str = str.RightFrom('\n');
	m_ToolTip.AddTool(this, str, r, m_uButtonID);

	// Add tool for dropdown arrow
	GetClientRect(r);
	r.left = nDividerPos; //, hardcoded 
	str.LoadString(m_uDropDownID);
	str = str.RightFrom('\n');
	m_ToolTip.AddTool(this, str, r, m_uDropDownID);

}





//` Do the dropdown function for this button.
//` added keyboard mode (not used?)
void CColorButton::DropDown(BOOL bKeyboardMode)
{
	CRect rDraw;
	GetWindowRect(rDraw);

	new CColourPopup(CPoint(rDraw.left, rDraw.bottom),		// Point to display popup
					 m_Color,								// Selected colour
					 this,									// parent
					 m_strDefaultText,						// "Default" text area
					 m_strCustomText,					// Custom Text
					 bKeyboardMode);			//` added this param

	CWnd *pParent = GetParent();
	if (pParent)
//		pParent->SendMessage(CPN_DROPDOWN, (LPARAM)m_Color, (WPARAM) GetDlgCtrlID());
//		pParent->SendMessage(CPN_DROPDOWN, (WPARAM) m_Color, (LPARAM) GetDlgCtrlID()); //`
		pParent->SendMessage(CPN_DROPDOWN, (WPARAM) m_Color, (LPARAM) m_uDropDownID); //`
//		pParent->SendMessage(CBN_CLICK, (WPARAM) m_Color, (LPARAM) m_uDropDownID); //`
	
}


