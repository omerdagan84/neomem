
// CStaticEx



#include "precompiled.h"
#include "NeoMem.h"
#include "StaticEx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/*
//, could set different default tab stop - this is 1/2 inch
// ideally, let user pick one
// unless...
// could read default font info here
	CString strPrefix = 
		"{\\rtf1\\ansi\\deff0\\deftab720\\deflang1033"
		"{\\fonttbl{\\f0\\fswiss Tahoma;}}\n"
		"{\\colortbl\\red0\\green0\\blue0;}\n"
		"\\pard"
//		"\\tx360\\tx720\\tx1080\\tx1440\\tx1800\\tx2160\\tx2520\\tx2880\\tx3240\\tx3600\\tx3960\\tx4320\\tx4680\\tx5040\\tx5400\\tx5760\\tx6120\\tx6480"
		"\\plain\\f0\\fs16 "; // fs16 means to use 8 pt font!
*/




//---------------------------------------------------------------------------------------------------------


BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
	//{{AFX_MSG_MAP(CStaticEx)
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




CStaticEx::CStaticEx()
{
	m_bAutosize = TRUE;
//	m_bHighlight = FALSE;

	m_nCommandID = 0;

	m_bOpaque = TRUE;
	m_nBkMode = OPAQUE;
//	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
//	m_clrBackground = ::GetSysColor(COLOR_3DFACE);
//	m_clrText = g_clrWindowText;
	m_clrText = -1; // use windows system color
//	m_clrBackground = g_clr3dFace;
	m_clrBackground = -1; // none
	m_clrBackground2 = -1; // none
//	m_clrHighlight = ::GetSysColor(COLOR_HIGHLIGHT);
//	m_clrHighlight = g_clrHighlight;
	m_clrHighlight = -1; // use windows system color

	m_hCursor = 0;
	m_nSound = 0;
	m_nTimer = 0;
	m_rMargins.SetRect(2, 1, 2, 1);
	m_rPos.SetRectEmpty();
	m_rTextSize.SetRectEmpty();
//	m_rDraw.SetRectEmpty();

	m_font.CreateStockObject(ANSI_VAR_FONT);

	m_nFlags = 0;
}

CStaticEx::~CStaticEx()
{
//	if (m_nTimer)
//		KillTimer(m_nTimer);
}



//---------------------------------------------------------------------------------------------------------


void CStaticEx::SetFontEx(LPCTSTR szFontName /* = "Arial" */, float sngFontSize /* = 8.0 */, 
										BOOL bFontBold /* = FALSE */, BOOL bFontItalic /* = FALSE */)
{
	// Create and set font
	VERIFY(m_font.DeleteObject()); // delete existing font
	int nPointSizeTimes10 = (int) (sngFontSize * 10);
	VERIFY(m_font.CreatePointFont2(nPointSizeTimes10, szFontName, bFontBold, bFontItalic));
//	LOGFONT lf;
//	m_font.GetLogFont(&lf);
	// Set font if window has been created
	if (::IsWindow(m_hWnd))
		SetFont(&m_font);
}


void CStaticEx::SetColors(BOOL bOpaque /* = TRUE */
//										int nText /* = COLOR_TEXT */
//										int nTextBack /* = COLOR_3DFACE */
//										int nHot /* = 
//										COLORREF clrText /* = -1 */
//										COLORREF clrBackground /* = -1 */
//										COLORREF clrHighlight /* = -1 */
//										COLORREF clrBackground2 /* = -1 */
										)
{
	// Get background drawing mode
	m_bOpaque = bOpaque;
	m_nBkMode = m_bOpaque ? OPAQUE : TRANSPARENT;

	// Get colors
//	m_clrText = (clrText == -1) ? ::GetSysColor(COLOR_TEXT) : clrText;
//	m_clrBackground = (clrBackground == -1) ? ::GetSysColor(COLOR_WINDOW) : clrBackground;
//	m_clrHighlight = (clrHighlight == -1) ? ::GetSysColor(COLOR_HIGHLIGHTTEXT) : clrHighlight;
//	m_clrBackground2 = (clrBackground2 == -1) ? ::GetSysColor(COLOR_TEXT) : clrText;

}


BOOL CStaticEx::Create(		
											LPCTSTR szText, 
											CRect &r, 
											CWnd* pParentWnd, 
											UINT nCommandID /* = 0 */, 
											BOOL bAutosize /* = TRUE */, 
											UINT nFormat /* = DT_LEFT | DT_BOTTOM */,
											UINT nFlags /* = 0 */
										)
{
	// Call base class to create window
	if (!CStatic::Create(szText, WS_CHILD | WS_VISIBLE | SS_NOTIFY, r, pParentWnd, nCommandID))
//	if (!CStatic::Create(szText, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | SS_NOTIFY, r, pParentWnd, nCommandID))
		return FALSE;

	// Save parameters
//	m_strText = szText;
	m_nCommandID = nCommandID;
	m_bAutosize = bAutosize;
	if (nFormat & DT_TOP || nFormat & DT_BOTTOM || nFormat & DT_VCENTER)
		nFormat |= DT_SINGLELINE;
	m_nFormat = nFormat;

	// If font has not been specified yet, use the default font
//	if (m_font.m_hObject == 0)
//		m_font.CreateStockObject(ANSI_VAR_FONT);
	SetFont(&m_font, FALSE);

	// Get size of text
	CString strText = szText;
	CDC* pdc = GetDC();
//	CSize sz = pdc->GetTextExtent(m_strText);
	CSize sz = pdc->GetTextExtent(strText);
	m_rTextSize.SetRect(0, 0, sz.cx, sz.cy);

	// Get size and position
	if (m_bAutosize)
	{
		m_rMargins.SetRect(0, 0, 0, 0);
		m_rControlSize = m_rTextSize + m_rMargins;
		// Set window width and height
		// this will trigger OnSize
		SetWindowPos(0, 0, 0, m_rControlSize.right, m_rControlSize.bottom, SWP_NOMOVE | SWP_NOZORDER);
//		m_rDraw = m_rControlSize;
	}
	else
	{
//		m_rDraw.SetRect(m_rMargins.left, m_rMargins.top, m_rTextSize.right + m_rMargins.left + m_rMargins.right,
//									m_rTextSize.bottom + m_rMargins.top + m_rMargins.bottom);
	}

	// Turn on highlight timer
	//...... better if we just have one global timer that checks if mouse is over same window
	// otherwise would bog program down
	//. actually could start the timer when mouse moves over label, if one hasn't already been started,
	// and if at the timeout period the mouse is no longer over the label, unhighlight it
//	if (m_nCommand)
//	{
//		m_nTimer = SetTimer(
//	}

	// Use hand cursor if command ID specified (ie it's a button)
	if (m_nCommandID)
	{
//		m_hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_HAND);
		m_hCursor = theApp.m_hCursorHand;
	}

	// Save flags
	m_nFlags = nFlags;

	return TRUE;
}




void CStaticEx::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// Save width and height of control
	m_rControlSize.SetRect(0, 0, cx, cy);

	m_rDraw = m_rControlSize;
	m_rDraw.DeflateRect(&m_rMargins);

}



void CStaticEx::OnPaint() 
{
	// Do not call CStatic::OnPaint() for painting messages
	CPaintDC dc(this); // device context for painting

	COLORREF clrBackground = (m_clrBackground == -1) ? Library::clrWindow : m_clrBackground;
	COLORREF clrForeground = (m_clrText == -1) ? Library::clrWindowText : m_clrText;

//	dc.SetTextColor(m_clrText);
//	dc.SetBkColor(m_clrBackground);
	dc.SetTextColor(clrForeground);
	dc.SetBkColor(clrBackground);
//	dc.SetBkMode(m_nBkMode); // opaque or transparent, for drawtext
	dc.SetBkMode(TRANSPARENT); 

	// Fill background if in opaque mode
//	if (m_bOpaque)
	if (m_clrBackground != -1)
	{
		dc.FillSolidRect(m_rControlSize, m_clrBackground);
	}

	// Fill with even line color also if specified
	if (m_clrBackground2 != -1)
	{
		// walk through and color even lines
		CPen* pPen = new CPen(PS_SOLID, 0, m_clrBackground2);
		CPen* pOldPen = dc.SelectObject(pPen);
		for (int y = 0; y < m_rControlSize.bottom; y += 2)
		{
			dc.MoveTo(0, y);
			dc.LineTo(m_rControlSize.right, y);
		}
		dc.SelectObject(pOldPen);
		delete pPen;
	}

	// Draw text
	CFont* poldfont = dc.SelectObject(&m_font);
//	dc.DrawText(m_strText, m_rDraw, m_nFormat);
	CString strText;
	GetWindowText(strText);
	dc.DrawText(strText, m_rDraw, m_nFormat);
	dc.SelectObject(poldfont);

	// Draw underline if specified
	if (m_nFlags && flagUnderline)
	{
		int x = m_rControlSize.right;
		int y = m_rControlSize.bottom;
//		CPen* pPen = new CPen(PS_SOLID, 0, m_clrText);
		CPen* pPen = new CPen(PS_SOLID, 0, clrForeground);
		CPen* pOldPen = dc.SelectObject(pPen);
		dc.MoveTo(0, y);
		dc.LineTo(x, y);
		dc.SelectObject(pOldPen);
		delete pPen;
	}
}






BOOL CStaticEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hCursor)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}



void CStaticEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	xTRACE("CStaticEx::OnMouseMove %d %d\n", point.x, point.y);

	// Show status bar text if this label has it.
	// Gets status string from string table (resource ID should match command ID).
	if (m_nCommandID)
	{
		CString str;
		if (str.LoadString(m_nCommandID))
			theApp.SetStatusBarText(str);
		else
			theApp.SetStatusBarText();
	}
	else
		theApp.SetStatusBarText();
	CStatic::OnMouseMove(nFlags, point);
}




// Set the width of the label and get the height necessary to fit all the text
int CStaticEx::SetWidthGetHeight(int nWidth)
{
	CClientDC dc(this);

//	CSize sz = dc.GetTextExtent(m_strHeader);
//	CSize sz = dc.GetTextExtent("OP", 2);
//	m_nHeaderHeight = (int) ((float) sz.cy * 1.2f); // 30
//	CSize sz = dc.GetTextExtent(m_strText);
//	int y = (r.bottom - sz.cy) / 2;

	// Get height using DrawText
	// DT_CALCRECT   Determines the width and height of the rectangle. If there are multiple lines of 
	// text, DrawText will use the width of the rectangle pointed to by lpRect and extend the base 
	// of the rectangle to bound the last line of text. If there is only one line of text, DrawText will 
	// modify the right side of the rectangle so that it bounds the last character in the line. 
	// In either case, DrawText returns the height of the formatted text, but does not draw the text.
	CString strText;
	GetWindowText(strText);
	CRect r;
	r.SetRect(0, 0, nWidth, 0);
	CFont* poldfont = dc.SelectObject(&m_font);
//	int nHeight = dc.DrawText(m_strText, r, m_nFormat | DT_CALCRECT);
	int nHeight = dc.DrawText(strText, r, m_nFormat | DT_CALCRECT);
	dc.SelectObject(poldfont);

	return nHeight;
}



void CStaticEx::OnClicked() 
{
	xTRACE("CStaticEx::OnClicked\n");
	if (m_nCommandID)
	{
		CWaitCursor cw;
		GetParent()->SendMessage(WM_COMMAND, m_nCommandID);
	}
}


BOOL CStaticEx::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// On receiving a command, pass it on to this label's parent, as
	// it might have a child label which is sending it this message.
	GetParent()->SendMessage(WM_COMMAND, wParam);
	return TRUE; // message has been processed

//	return CStatic::OnCommand(wParam, lParam);
}



void CStaticEx::OnSysColorChange() 
{
	CStatic::OnSysColorChange();
	
	// TODO: Add your message handler code here
		
}


