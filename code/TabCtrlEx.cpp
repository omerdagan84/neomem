
// CTabCtrlEx



#include "precompiled.h"
#include "TabCtrlEx.h"

#include "macros.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//---------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CTabCtrlEx, CTabCtrl)
	//{{AFX_MSG_MAP(CTabCtrlEx)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// Construction/Destruction
//---------------------------------------------------------------------------------------------------

CTabCtrlEx::CTabCtrlEx()
{
	m_nDragPhase = dragNone;
	m_nTargetTab = -1;
	m_nTabDrag = -1;
}

CTabCtrlEx::~CTabCtrlEx()
{
}




// CTabCtrlEx message handlers
//---------------------------------------------------------------------------------------------------


BOOL CTabCtrlEx::OnEraseBkgnd(CDC* pDC) 
{
	return CTabCtrl::OnEraseBkgnd(pDC);
//	CRect r;
//	GetClientRect(r);
//	COLORREF color = ::GetSysColor(COLOR_3DFACE);
//	COLORREF color = 0x0022c4a6;
//	pDC->FillSolidRect(r, color);
//	return TRUE;
}





void CTabCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	xTRACE("CTabCtrlEx::OnLButtonDown\n");
/*
	//, commented out for v1

	// Don't start drag operation until user has actually moved the mouse outside a certain rectangle
	m_nDragPhase = dragInitial;
	m_ptInitial = point;
	
	// Save which tab is clicked on - will need this to alert parent
	TCHITTESTINFO tc;
	tc.pt = point;
	m_nTabDrag = CTabCtrl::HitTest(&tc);

	// Set rectangle to test for start of drag operation
	int nPixels = 2; // number of pixels to move in any direction
	m_rectTest.SetRect(point.x - nPixels, point.y - nPixels, point.x + nPixels, point.y + nPixels);

	CWnd::SetCapture();
*/

	// Call base class
	// if you don't call this then tab doesn't get changed
	CTabCtrl::OnLButtonDown(nFlags, point);
}



void CTabCtrlEx::OnMouseMove(UINT nFlags, CPoint point) 
{
/*
	//, commented out for v1

	if (m_nDragPhase == dragInitial)
	{
		// If user moved outside of test rectangle then start drag operation
		if ( ! m_rectTest.PtInRect(point))
		{
			m_nDragPhase = dragMove;
			m_rectDraw.SetRectEmpty();
		}
	}
	if (m_nDragPhase == dragMove)
	{
		// Get the new position
		CRect rNew;
		GetClientRect(rNew);
		CWnd::ClientToScreen(&rNew);
		rNew.OffsetRect(point);
		rNew.OffsetRect(-m_ptInitial.x, -m_ptInitial.y);
		rNew.DeflateRect(20, 4, 20, 26);

		// Draw and erase rects
		DrawRect(m_rectDraw, rNew);

		// Highlight target tab
		//. or tab frame
//		TCHITTESTINFO hti;
//		hti.pt = point;
//		int nTab = HitTest(&hti);

		// Erase last highlight
//		if (nTab != m_nTargetTab && m_nTargetTab != -1)
//			HighlightItem(m_nTargetTab, FALSE);
		
		// Draw new highlight
//		if (nTab != -1)
//			HighlightItem(nTab, TRUE);
//		m_nTargetTab = nTab;

		// Save new position
		m_rectDraw = rNew;
	}
*/

	// Call base class
	CTabCtrl::OnMouseMove(nFlags, point);
}


void CTabCtrlEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	xTRACE("CTabCtrlEx::OnLButtonUp\n");
/*
	//, commented out for v1

	// If still in initial phase then user never moved mouse outside of test rectangle,
	// so just treat as a regular click
	if (m_nDragPhase == dragInitial)
	{
		::ReleaseCapture();
		m_nDragPhase = dragNone;
		// need to select the drag tab, since it didn't receive the lbuttondown message (we hid it from it)
		CTabCtrl::SetCurSel(m_nTabDrag);
	}

	if (m_nDragPhase == dragMove)
	{
		// Erase last drawn rectangle
		CRect rNew;
		rNew.SetRectEmpty();
		DrawRect(m_rectDraw, rNew);
		m_nDragPhase = dragNone;
		::ReleaseCapture();

		// Erase last highlight
//		if (m_nTargetTab != -1)
//			HighlightItem(m_nTargetTab, FALSE);

		// Check if released over another tab
		TCHITTESTINFO hti;
		hti.pt = point;
		int nTabDest = CTabCtrl::HitTest(&hti);
		if (nTabDest == -1)
		{
			// Check if released down below
			CRect r;
			CTabCtrl::GetClientRect(r);
			CTabCtrl::AdjustRect(FALSE, r);
			if (r.PtInRect(point))
				nTabDest = CTabCtrl::GetCurSel();
		}

		// Ignore if released over start tab
		if (nTabDest == m_nTabDrag)
			nTabDest = -1;
		
		// If valid destination then alert parent
		//. might also want to say if should merge to top or bottom of view
		if (nTabDest != -1)
		{
			CString strMsg;
			strMsg.Format("Merge tab %d into tab %d", m_nTabDrag, nTabDest);
			AfxMessageBox(strMsg);
			//. send message to parent (start and dest tabs)
		}
	}
*/
	// Call base class
	CTabCtrl::OnLButtonUp(nFlags, point);
}



// Draw and erase rubber band rectangles
void CTabCtrlEx::DrawRect(CRect& rErase, CRect& rDraw)
{
	// Get device context for entire screen
	HDC hDC = ::GetWindowDC(NULL);

	// Attach it to an MFC device context object
	CDC dc;
	dc.Attach(hDC);
	
	// Erase the last position
	if (!rErase.IsRectEmpty()) dc.DrawFocusRect(rErase);

	// Draw the new one
	dc.DrawFocusRect(rDraw);
		
	// Release the device context
	dc.Detach();
	::ReleaseDC(NULL, hDC);
}



