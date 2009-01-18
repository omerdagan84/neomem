
// CViewHome


#include "precompiled.h"

#include "NeoMem.h"
#include "Constants.h"
#include "ViewHome.h"
#include "Color.h"

#include "ConstantsDatabase.h"
#include "NeoDoc.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




BEGIN_MESSAGE_MAP(CViewHome, CViewEx)
	//{{AFX_MSG_MAP(CViewHome)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_COMMAND(IDC_LBL_WEBSITE, OnLblWebsite)
	ON_COMMAND(IDC_LBL_EMAIL, OnLblEmail)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CViewHome, CViewEx)



CViewHome::CViewHome()
{
	m_nHeaderHeight = 0;
	m_nFooterHeight = 0;

	m_pview = 0;
//	m_pszDescription = 0;
}

CViewHome::~CViewHome()
{
}



#ifdef _DEBUG
void CViewHome::AssertValid() const
{
	CViewEx::AssertValid();
}

void CViewHome::Dump(CDumpContext& dc) const
{
	CViewEx::Dump(dc);
}
#endif //_DEBUG



/*
	// Initialize progress control
	CRect r;
	r.SetRectEmpty();
	if (!m_progDaysLeft.Create(WS_CHILD | WS_VISIBLE, r, this, 0))
		return -1;
	m_progDaysLeft.SetRange32(0, 60);
	m_progDaysLeft.SetPos(theApp.m_nDaysLeft);
*/

int CViewHome::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// Call base class
	if (CViewEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Tell ViewEx about this window so it knows to draw 3d border around it
	SetChildView(this);

	CRect r;
	r.SetRectEmpty();

	// Create header
	m_lblHeader.m_clrBackground = theApp.m_clrHomeTopBack;
	m_lblHeader.m_clrBackground2 = theApp.m_clrHomeTopBack2;
	m_lblHeader.m_clrText = theApp.m_clrHomeTopFore;
	m_lblHeader.SetFontEx("Verdana", 16.0);
	m_lblHeader.Create(" NeoMem", r, this, 0, FALSE, DT_LEFT | DT_VCENTER);
	m_nHeaderHeight = 30; //.

	// Create footer
	m_lblFooter.m_clrBackground = theApp.m_clrHomeBottomBack;
	m_lblFooter.m_clrBackground2 = theApp.m_clrHomeBottomBack2;
	m_lblFooter.m_clrText = theApp.m_clrHomeBottomFore;
	m_lblFooter.SetFontEx("MS Sans Serif", 10.0);
	m_lblFooter.Create("  NeoMem.org", r, this, 0, FALSE, DT_LEFT | DT_VCENTER);
	m_nFooterHeight = 22; //.

	// Create footer buttons
	m_lblWebsite.m_clrText = theApp.m_clrHomeBottomFore;
	m_lblDot.m_clrText = theApp.m_clrHomeBottomFore;
	m_lblEmail.m_clrText = theApp.m_clrHomeBottomFore;
	m_lblWebsite.Create("website", r, &m_lblFooter, IDC_LBL_WEBSITE, TRUE, DT_CENTER | DT_VCENTER);
	m_lblDot.Create("·", r, &m_lblFooter, 0, TRUE, DT_CENTER | DT_VCENTER);
	m_lblEmail.Create("email", r, &m_lblFooter, IDC_LBL_EMAIL, TRUE, DT_CENTER | DT_VCENTER);

	// Goto next available tip of the day
	theApp.GotoNextTip();

	// Create inner view window
//	m_pview = (CViewHomeInner*) CViewEx::CreateChildView(this, RUNTIME_CLASS(CViewHomeInner), m_pDoc, r, 0);
	m_pview = (CViewHomeInner*) CreateChildView(RUNTIME_CLASS(CViewHomeInner), m_pDoc, r, 0);

/*
	// Create rtf control, read-only
	m_rtf.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL, 
						CRect(0,0,0,0), this, 100);
	
	GetFileDescription();
	m_strTip = theApp.GetTip();
	m_strFileSize = m_pDoc->GetFileSizeString();
	m_strObjects = m_pDoc->GetNumberOfObjectsString();

	// Use default formatting rectangle
	m_rtf.SetRect(NULL);

	CString strColorDescription = CColor::GetRtfFormat(theApp.m_clrHomeDescriptionFore);
	CString str;
	str.Format( 
					"{\\rtf1\\ansi\\deff0\\deftab720\\deflang1033"
					"{\\fonttbl{\\f0\\fswiss\\fprq2 Arial;}}"
					"{\\colortbl"
					"\\red0\\green0\\blue0;" // cf0
					"\\red160\\green160\\blue160;" // cf1
					"\\red172\\green160\\blue140;" // cf2
					"\\red255\\green255\\blue255;" // cf3
					"%s}" // description color // cf4
					"\\pard \\li120\\ri120 " // li = left indent!
					"\\plain\\fs16\\cf4 \\par %s \\par" // file description
					"\\plain\\fs16 "
//					"\\par {\\b\\ul Tip of the Day                                        \\cf3 a }" 
					"\\par {\\b Tip of the Day }" 
					"\\par %s " // tip
					"\\par " 
					"\\par {\\b File }" 
					"\\par Objects: \\tab %s " // nobjects
					"\\par File Size: \\tab %s " // file size
					"\\par }",
					(LPCTSTR) strColorDescription,
					(LPCTSTR) m_strFileDescription,
					(LPCTSTR) m_strTip,
					(LPCTSTR) m_strObjects,
					(LPCTSTR) m_strFileSize
					);

	m_rtf.SetRtf(str);
	
//	TRACESTRING("%s\n", (LPCTSTR) str);
*/

	return 0;
}




void CViewHome::OnDraw(CDC* pDC)
{
	// Call base class
	CViewEx::OnDraw(pDC);

	// Draw icon of currently selected item
//	CImageList* piml = pDoc->GetImageList();
//	int intBorder = (m_nHeight - 16) / 2; // 16 is height of icon
//	CPoint ptIcon(intBorder, intBorder);
//	int nImage = pobj->GetIconIndex();
//	piml->Draw(pDC, nImage, ptIcon, ILD_NORMAL);

}



void CViewHome::OnSize(UINT nType, int cx, int cy) 
{
	xTRACE("CViewHome::OnSize\n");

	// Call base class
//	CViewEx::OnSize(nType, cx, cy);

	if ((cx == 0) || (cy == 0)) return;

	// Resize/reposition controls
	CRect r;
	CRect rPos;

	int nTop = 0;

	// If showing title bar, leave room for it
	if (m_bShowTitleBar)
		nTop = m_nTitleBarHeight;

	// Header
	rPos.SetRect(1, nTop + 1, cx - 1, nTop + 1 + m_nHeaderHeight); 
	m_lblHeader.MoveWindow(rPos);

	// Footer
	rPos.SetRect(1, cy - 1 - m_nFooterHeight, cx - 1, cy - 1); 
	m_lblFooter.MoveWindow(rPos);

	// Get start position for labels (lower right corner, since they're right and bottom aligned)
	int nFooterWidth = cx - 2;
	int nX = nFooterWidth - 20;

	m_lblEmail.GetClientRect(r);
	rPos.SetRect(nX - r.right, 0, nX, m_nFooterHeight);
	m_lblEmail.MoveWindow(rPos);
	nX = nX - r.right;

	m_lblDot.GetClientRect(r);
	rPos.SetRect(nX - r.right, 0, nX, m_nFooterHeight);
	m_lblDot.MoveWindow(rPos);
	nX = nX - r.right;

	m_lblWebsite.GetClientRect(r);
	rPos.SetRect(nX - r.right, 0, nX, m_nFooterHeight);
	m_lblWebsite.MoveWindow(rPos);
	nX = nX - r.right;

	// Position inner window
	rPos.SetRect(1, nTop + 1 + m_nHeaderHeight, cx - 1, cy - m_nFooterHeight - 1);
	m_pview->MoveWindow(rPos);

//	m_rtf.MoveWindow(rPos);
//	m_rtf.SetRect(NULL);
	// redraw line
//	DrawLines();
}



void CViewHome::OnMouseMove(UINT nFlags, CPoint point) 
{
	// Clear status bar text, because some labels in this view have status text associated with them.
	theApp.SetStatusBarText();
	CViewEx::OnMouseMove(nFlags, point);
}


void CViewHome::OnLblWebsite() 
{
	theApp.GotoWebsite();
}


void CViewHome::OnLblEmail() 
{
//	theApp.SendEmail();
	theApp.OnCmdHelpEmail();
}


void CViewHome::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	TRACE("    CViewHome::OnUpdate %s\n", theApp.GetHintName(lHint));

	switch (lHint)
	{
		case hintRefresh:
			Invalidate();
			break;
	}
}


BOOL CViewHome::OnEraseBkgnd(CDC* pDC) 
{
//	return CViewEx::OnEraseBkgnd(pDC);

	if (m_bShowTitleBar)
	{
		COLORREF clrBackground = g_clr3dFace;
		CRect r;
		GetClientRect(r);
		r.bottom = m_nTitleBarHeight + 1;
		pDC->FillSolidRect(r, clrBackground);
	}

	return TRUE; // so base class won't draw a gray background which will be drawn over anyway
}



void CViewHome::GetFileDescription()
{
/*
	ASSERT_VALID(m_pDoc);
	BObject* pobjHome = m_pDoc->GetObject(rootUser);
	ASSERT_VALID(pobjHome);
	m_strFileDescription = pobjHome->GetPropertyText(propDescription);
	// If description is blank and there are no objects, show an intro message
	if (m_strFileDescription.GetLength() == 0)
	{
		if (pobjHome->GetChildCount(FALSE, FALSE) == 0)
			m_strFileDescription = _T("To get started, add a new object by selecting \"New Object...\" from the Object menu.");
	}
*/
}




void CViewHome::DrawLines()
{
/*
	// search through rtf text for line indicator and draw lines over text at those points
	FINDTEXTEX ft;
	ft.chrg.cpMin = 0;
	ft.chrg.cpMax = -1;
	ft.lpstrText = "o";
//	long nChar = m_rtf.FindText(FR_MATCHCASE, &ft);
	long nChar = m_rtf.FindText(FR_DOWN, &ft); // FR_DOWN needed for 2.0 and later!
	if (nChar != -1)
	{
//		CClientDC dc(this);
		CClientDC dc(&m_rtf);

		// get position (top-left corner) of the given character in client coordinates
		CPoint pt = m_rtf.GetCharPos(nChar);
		
		// draw a line at that position
		CRect r;
		m_rtf.GetClientRect(&r);
		int y = pt.y;
		int xLeft = 0;
		int xRight = r.right;
		dc.SelectStockObject(LTGRAY_BRUSH);
		dc.MoveTo(xLeft, y);
		dc.LineTo(xRight, y);
		
	}
*/
}



void CViewHome::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CViewEx::OnPaint() for painting messages
	
//	DrawLines();
}
