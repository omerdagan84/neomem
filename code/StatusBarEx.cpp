
// CStatusBarEx



#include "precompiled.h"
#include "NeoMem.h"
#include "StatusBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




BEGIN_MESSAGE_MAP(CStatusBarEx, CStatusBar)
	//{{AFX_MSG_MAP(CStatusBarEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
//	ON_UPDATE_COMMAND_UI(ID_INDICATOR_DATE, OnUpdateDateIndicator)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CStatusBarEx::CStatusBarEx()
{
	m_nClockPane = -1;
	m_lngProgressLeft = 0;
}

CStatusBarEx::~CStatusBarEx()
{
}





int CStatusBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// By default, the first indicator is “elastic”: it takes up the status-bar length not used by 
	// the other indicator panes, so that the other panes are right-aligned.
	static UINT nIndicators[] = 
	{
		ID_SEPARATOR,           // status line indicator
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
		ID_INDICATOR_DATE, // day and date indicator
	};
	
	// Add panes to status bar
	SetIndicators(nIndicators, sizeof (nIndicators) / sizeof (UINT));
	
/*
	// Size the status bar panes
	TEXTMETRIC tm;
	CClientDC dc (this);
	CFont* pFont = GetFont();
	CFont* pOldFont = dc.SelectObject(pFont);
	dc.GetTextMetrics(&tm);
	dc.SelectObject (pOldFont);

	int cxWidth;
	UINT nID, nStyle;
	GetPaneInfo(1, nID, nStyle, cxWidth);
	SetPaneInfo(1, nID, nStyle, tm.tmAveCharWidth * 24);
	GetPaneInfo (2, nID, nStyle, cxWidth);
	SetPaneInfo(2, nID, SBPS_NOBORDERS, tm.tmAveCharWidth * 24);
*/

/*
	// Size the panes
	TEXTMETRIC tm;
	CClientDC dc(this);
	CFont* pFont = GetFont();
	CFont* pOldFont = dc.SelectObject(pFont);
	dc.GetTextMetrics(&tm);
	dc.SelectObject(pOldFont);

	// Size date pane
	int cxWidth;
	UINT nID;
	UINT nStyle;
	int nIndex = CommandToIndex(ID_INDICATOR_DATE);
	ASSERT(nIndex != -1);
	GetPaneInfo(nIndex, nID, nStyle, cxWidth);
	SetPaneInfo(nIndex, nID, nStyle, tm.tmAveCharWidth * 30); //should be enough chars for long dates
*/

	// Place a progress control in the left pane (invisible for now)
	CRect r;
	GetItemRect(0, &r);
//	m_wndProgress.Create(WS_CHILD | PBS_SMOOTH, r, this, -1);
	m_wndProgress.Create(WS_CHILD, r, this, -1);
	m_wndProgress.SetRange(0, 100);
	m_wndProgress.SetPos(0);
	
	// Create clock control in last panel
	r.SetRectEmpty();
	m_clock.Create(r, this);

	// Set clock size
	UINT nID;
	UINT nStyle;
	int cxWidth;
	m_nClockPane = CommandToIndex(ID_INDICATOR_DATE);
	ASSERT(m_nClockPane != -1);
	GetPaneInfo(m_nClockPane, nID, nStyle, cxWidth);
	SetPaneInfo(m_nClockPane, nID, nStyle, m_clock.GetWidth());


	// Get required size for status bar so progress bar won't block it
	Resize();

	// Set icon
	// Nice, but not very standard
//	HICON hIcon = (HICON) ::LoadImage(AfxGetResourceHandle(), 
//												MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0);
//	GetStatusBarCtrl().SetIcon(0, hIcon);

	return 0;
}



void CStatusBarEx::OnSize(UINT nType, int cx, int cy) 
{
	xTRACE("CStatusBarEx::OnSize\n");

	CStatusBar::OnSize(nType, cx, cy);
	
	// Resize progress control to fit the left pane of the resized status bar
	Resize();

	// Update clock position also (client pixels)
	if (m_nClockPane != -1)
	{
		CRect r;
		GetItemRect(m_nClockPane, &r);
		m_clock.MoveWindow(r, TRUE);
	}

}




/*
// Write todays date in pane
void CStatusBarEx::OnUpdateDateIndicator(CCmdUI *pCmdUI)
{
	// this is handled by CClock

	// could measure text extent and resize pane to fit it here
	pCmdUI->Enable();
	CTime t = CTime::GetCurrentTime();
//	CString strDate = t.Format("%A, %B %d, %Y"); // eg "Friday, March 05, 1999" 
//	CString strDate = t.Format("%A, %B %#d, %Y"); // eg "Friday, March 5, 1999" 
//	CString strDate = t.Format("%#x"); // Long date format appropriate to current local
//	CString strDate = t.Format("%#c"); // Long date and time format appropriate to current local
//	CString strDate = t.Format("%A, %B %#d, %Y   %I:%M:%S %p"); // eg "Friday, March 5, 1999  12:54:32 PM"
	CString strDate = t.Format("%A, %B %#d, %Y   %I:%M %p"); // eg "Friday, March 5, 1999  12:54 PM"
	CClientDC dc(this);
	CSize sz = dc.GetTextExtent(strDate);

	// Size pane
	int cxWidth;
	UINT nID;
	UINT nStyle;
	int nIndex = CommandToIndex(ID_INDICATOR_DATE);
	ASSERT(nIndex != -1);
	GetPaneInfo(nIndex, nID, nStyle, cxWidth);
//	SetPaneInfo(nIndex, nID, nStyle, tm.tmAveCharWidth * 30); //should be enough chars for long dates
	SetPaneInfo(nIndex, nID, nStyle, sz.cx);

//.	pCmdUI->SetText(strDate);
	//, need to make this right-aligned - draw it manually?

}
*/


void CStatusBarEx::Resize()
{
	// Get end of status bar text 
	CString str;
	GetWindowText(str);
	CClientDC dc(this);
	CSize sz = dc.GetTextExtent(str);
	m_lngProgressLeft = sz.cx;

	// Position progress bar to right of status bar text
	CRect r;
	GetItemRect(0, &r);
	r.left = m_lngProgressLeft;
	int nRight = r.left + 200;
	if (nRight < r.right) r.right = nRight;
	m_wndProgress.MoveWindow(r);
}


