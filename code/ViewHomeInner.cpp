
// CViewHomeInner



#include "precompiled.h"

#include "NeoMem.h"
#include "ViewHomeInner.h"
#include "Constants.h"
#include "Hint.h"

#include "ConstantsDatabase.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNCREATE(CViewHomeInner, CScrollView)

BEGIN_MESSAGE_MAP(CViewHomeInner, CScrollView)
	//{{AFX_MSG_MAP(CViewHomeInner)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDC_LBL_PREV, OnLblPrev)
	ON_COMMAND(IDC_LBL_NEXT, OnLblNext)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CViewHomeInner::CViewHomeInner()
{
}

CViewHomeInner::~CViewHomeInner()
{
}


#ifdef _DEBUG
void CViewHomeInner::AssertValid() const
{
	CScrollView::AssertValid();
}

void CViewHomeInner::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG




int CViewHomeInner::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// Call base class
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Get document
	m_pDoc = (CNeoDoc*) GetDocument();

	// Set margins
	m_rMargins.SetRect(20, 10, 20, 10); // left top right bottom

	// Goto next available tip of the day
	theApp.GotoNextTip();

	// Create labels
	CString str;
	CRect r;
	r.SetRectEmpty();
	int nFormat = DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX;
	int nFormatMultiline = DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX;

	// Description
//	BObject* pobjHome = m_pDoc->GetObject(rootUser);
//	LPCTSTR pszDescription = pobjHome->GetPropertyText(propDescription);
//	// If description is blank and there are no objects, show an intro message
//	if (_tcslen(pszDescription) == 0)
//	{
//		BObject* pobj = m_pDoc->GetObject(rootUser);
//		ASSERT_VALID(pobj);
//		if (pobj->GetChildCount(FALSE, FALSE) == 0)
//			pszDescription = _T("To get started, add a new object by selecting \"New Object...\" from the Object menu.");
//	}
	m_lblDescription.SetFontEx("Arial", 9.0);
	m_lblDescription.m_clrText = theApp.m_clrHomeDescriptionFore;
//	m_lblDescription.Create(pszDescription, r, this, 0, TRUE, nFormatMultiline);
	m_lblDescription.Create("", r, this, 0, TRUE, nFormatMultiline);
	GetFileDescription(); // set label's text

	// Create buttons
	m_lblPrevTip.m_clrText = theApp.m_clrHomeMiddleFore2;
	m_lblDot.m_clrText = theApp.m_clrHomeMiddleFore2;
	m_lblNextTip.m_clrText = theApp.m_clrHomeMiddleFore2;
	m_lblPrevTip.Create("previous", r, this, IDC_LBL_PREV, TRUE, DT_CENTER | DT_BOTTOM);
	m_lblDot.Create(" · ", r, this, 0, TRUE, DT_CENTER | DT_BOTTOM);
	m_lblNextTip.Create("next", r, this, IDC_LBL_NEXT, TRUE, DT_CENTER | DT_BOTTOM);

	// Tip of the day
	m_lblTip.SetFontEx("Arial", 9.0, TRUE, FALSE);
	m_lblTip.Create("Tip of the Day", r, this, 0, TRUE, nFormat, CStaticEx::flagUnderline);
	LPCTSTR pszTip = theApp.GetTip();
	m_lblTip2.SetFontEx("Arial", 9.0);
	m_lblTip2.Create(pszTip, r, this, 0, TRUE, nFormatMultiline);



	return 0;
}


void CViewHomeInner::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

//	CRect r;
//	GetClientRect(r);
//	if (r.right != 0)
//		RecalculateLayout(r.right, r.bottom);
}



void CViewHomeInner::OnSize(UINT nType, int cx, int cy) 
{
	xTRACE("CViewHomeInner::OnSize\n");
	// Call base class
//	CScrollView::OnSize(nType, cx, cy);
	if ((cx == 0) || (cy == 0)) return;
	RecalculateLayout(cx, cy);
}




void CViewHomeInner::OnDraw(CDC* pDC)
{
	// Call base class
//	CScrollView::OnDraw(pDC);
}


void CViewHomeInner::OnMouseMove(UINT nFlags, CPoint point) 
{
	// Clear status bar text, because some labels in this view have status text associated with them.
	theApp.SetStatusBarText();
	CScrollView::OnMouseMove(nFlags, point);
}




// Call SetScrollSizes when the view is about to be updated. Call it in your override of the 
// OnUpdate member function to adjust scrolling characteristics when, for example, the 
// document is initially displayed or when it changes size.
void CViewHomeInner::RecalculateLayout(int cx /* = 0 */, int cy /* = 0 */)
{
	CRect r;
	CRect rPos;

	if (cx == 0 || cy == 0)
	{
		GetClientRect(r);
		cx = r.right;
		cy = r.bottom;
	}

	// Scroll to top
	CSize sz = GetTotalSize();
	if (sz.cy > 0)
		ScrollToPosition(CPoint(0, 0));

	int nScrollBarWidth = ::GetSystemMetrics(SM_CXVSCROLL);
	int nWidth = cx - m_rMargins.left - m_rMargins.right - nScrollBarWidth;
	int nY = m_rMargins.top;
	int nLeft = m_rMargins.left;
	int nRight = cx - m_rMargins.right;
	int nSpacing = 14; // pixels between sections
	int nLineHeight = 4; // pixels to skip after a line

	// Description
	int nHeight = m_lblDescription.SetWidthGetHeight(nWidth);
	rPos.SetRect(nLeft, nY, nRight, nY + nHeight);
	m_lblDescription.MoveWindow(rPos);
	nY += nHeight + nSpacing;

	// Tip of the Day
	nHeight = m_lblTip.SetWidthGetHeight(nWidth);
	rPos.SetRect(nLeft, nY, nRight, nY + nHeight);
	m_lblTip.MoveWindow(rPos);
	nY += nHeight;
	nY += nLineHeight;
	nHeight = m_lblTip2.SetWidthGetHeight(nWidth);
	rPos.SetRect(nLeft, nY, nRight, nY + nHeight);
	m_lblTip2.MoveWindow(rPos);
	int nYTotal = nY + nHeight + nSpacing + nSpacing; // get max y extent for scrollbars

	// Tip buttons
	nY = nY - nLineHeight - 1; // aligned to line - 1
	int nX = nRight;
	m_lblNextTip.GetClientRect(r); // get height and width
	rPos.SetRect(nX - r.right, nY - r.bottom, nX, nY);
	m_lblNextTip.MoveWindow(rPos);
	nX = rPos.left; // adjust cursor

	m_lblDot.GetClientRect(r); // get height and width
	rPos.SetRect(nX - r.right, nY - r.bottom, nX, nY);
	m_lblDot.MoveWindow(rPos);
	nX = rPos.left; // adjust cursor

	m_lblPrevTip.GetClientRect(r); // get height and width
	rPos.SetRect(nX - r.right, nY - r.bottom, nX, nY);
	m_lblPrevTip.MoveWindow(rPos);
	nX = rPos.left; // adjust cursor

	// When the view is first created, if you want to calculate the size of the scrollable view 
	// based on the size of the document, call the SetScrollSizes member function from your 
	// override of either CView::OnInitialUpdate or CView::OnUpdate. 
//	CSize sz = GetTotalSize();
//	SetScrollSizes(MM_TEXT, sz);
	
	// Calculate the total size of this view
	int nAvailWidth = cx - nScrollBarWidth - 2;
	CSize sizeTotal(nAvailWidth, nYTotal);
	CSize sizePage(nAvailWidth, cy);
	CSize sizeLine(nAvailWidth / 10, cy / 10);
	SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);

}



BOOL CViewHomeInner::OnEraseBkgnd(CDC* pDC) 
{
	xTRACE("CViewHomeInner::OnEraseBkgnd\n");

	// Call base class	
	// The default implementation erases the background using the window class background brush 
	// specified by the hbrBackground member of the window class structure. 
//	return CScrollView::OnEraseBkgnd(pDC);

	// An overridden OnEraseBkgnd should return nonzero in response to WM_ERASEBKGND if it 
	// processes the message and erases the background; this indicates that no further erasing is 
	// required. If it returns 0, the window will remain marked as needing to be erased. (Typically, 
	// this means that the fErase member of the PAINTSTRUCT structure will be TRUE.) 

	// Fill window with white background
	//. could draw a tile bitmap for background!
	CRect r;
//	COLORREF clr = 0x00ffffff; // white
	GetClientRect(&r);
//	pDC->FillSolidRect(&r, clr);
	pDC->FillSolidRect(&r, g_clrWindow);

	return TRUE;
}



void CViewHomeInner::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	TRACE("    CViewHomeInner::OnUpdate %s\n", theApp.GetHintName(lHint));

	switch (lHint)
	{
		case hintPropertyChange:
			{
				// Check if home object description changed.
				// If so, modify m_lbldescription and resize and refresh display.
				CHint* pobjHint = (CHint*) pHint;
				ASSERT_VALID(pobjHint);
				
				BObject* pobj = pobjHint->m_pobjObject;
				ULONG lngPropertyID = pobjHint->m_lngPropertyID;
				BObject* pobjHome = m_pDoc->GetObject(rootUser);
				if ((lngPropertyID == propDescription) && (pobj == pobjHome))
				{
//					LPCTSTR pszDescription = pobjHome->GetPropertyText(propDescription);
//					m_lblDescription.SetWindowText(pszDescription);
					GetFileDescription();
					RecalculateLayout();
					Invalidate();
				}
				break;
			}

		case hintRefresh:
			{
				RecalculateLayout();
				Invalidate();
				break;
			}

		default:
			break;
	}
	
}




void CViewHomeInner::OnLblPrev() 
{
	xTRACE("CViewHomeInner::OnLblPrev\n");
	CWaitCursor wc;
	theApp.GotoPreviousTip();
	m_lblTip2.SetWindowText(theApp.GetTip());
	RecalculateLayout();
	Invalidate();
}


void CViewHomeInner::OnLblNext() 
{
	xTRACE("CViewHomeInner::OnLblNext\n");
	CWaitCursor wc;
	theApp.GotoNextTip();
	m_lblTip2.SetWindowText(theApp.GetTip());
	RecalculateLayout();
	Invalidate();
}



// Get the description associated with the file (stored in the user root object) and
// set a label to display this. 
void CViewHomeInner::GetFileDescription()
{
	ASSERT_VALID(m_pDoc);
	BObject* pobjHome = m_pDoc->GetObject(rootUser);
	ASSERT_VALID(pobjHome);
	LPCTSTR pszDescription = pobjHome->GetPropertyText(propDescription);
	// If description is blank and there are no objects, show an intro message
	if (_tcslen(pszDescription) == 0)
	{
		if (pobjHome->GetChildCount(FALSE, FALSE) == 0)
			pszDescription = _T("To get started, add a new object by selecting \"New Object...\" from the Object menu.");
	}
	m_lblDescription.SetWindowText(pszDescription);
}
