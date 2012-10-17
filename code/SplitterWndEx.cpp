
// CSplitterWndEx



#include "precompiled.h"

#include "NeoMem.h"
#include "SplitterWndEx.h"
#include "TabCtrlEx.h"
#include "ViewTabs.h"

#include "ConstantsDatabase.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



BEGIN_MESSAGE_MAP(CSplitterWndEx, CSplitterWnd)
	//{{AFX_MSG_MAP(CSplitterWndEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CSplitterWndEx::CSplitterWndEx()
{
	// Make border one pixel instead of 2 pixels (looks better)
	m_cxBorder = 1;
	m_cyBorder = 1;

	m_lngMode = 0;
}

CSplitterWndEx::~CSplitterWndEx()
{
}


/*
// Override CreateView so we can modify the initial position of the child views
BOOL CSplitterWndEx::CreateView(int row, int col, CRuntimeClass *pViewClass, SIZE sizeInit, CCreateContext *pContext)
{
	// bpb Make border one pixel instead of 2 pixels (looks better)
//	m_cxBorder = 1;
//	m_cyBorder = 1;

	// Call base class
	return CSplitterWnd::CreateView(row, col, pViewClass, sizeInit, pContext);
}
*/



// Draw splitter bar and borders
// This code is taken from CSplitterWnd, with modifications marked by // bpb
void CSplitterWndEx::OnDrawSplitter(CDC* pDC, ESplitType nType,	const CRect& rectArg)
{
	
	// if pDC == NULL, then just invalidate
	if (pDC == NULL)
	{
		RedrawWindow(rectArg, NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
		return;
	}
	ASSERT_VALID(pDC);

	// otherwise, actually draw
	CRect rect = rectArg;
	switch (nType)
	{

	// bpb Draw thin border
	case splitBorder: {
//		ASSERT(afxData.bWin4);
//		pDC->Draw3dRect(rect, afxData.clrBtnShadow, afxData.clrBtnHilite);
		DWORD clrBtnShadow = GetSysColor(COLOR_BTNSHADOW);
		DWORD clrBtnHilite = GetSysColor(COLOR_BTNHILIGHT);
		pDC->Draw3dRect(rect, clrBtnShadow, clrBtnHilite);

//		rect.InflateRect(-CX_BORDER, -CY_BORDER);
//		pDC->Draw3dRect(rect, afxData.clrWindowFrame, afxData.clrBtnFace);
					  
		return;
	}

//	case splitIntersection:
//		ASSERT(!afxData.bWin4);
//		break;

	case splitBox:
//		if (afxData.bWin4)	
		{
		DWORD clrBtnFace = GetSysColor(COLOR_BTNFACE);
		DWORD clrWindowFrame = GetSysColor(COLOR_WINDOWFRAME);
		DWORD clrBtnShadow = GetSysColor(COLOR_BTNSHADOW);
		DWORD clrBtnHilite = GetSysColor(COLOR_BTNHILIGHT);
		pDC->Draw3dRect(rect, clrBtnFace, clrWindowFrame);
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
		pDC->Draw3dRect(rect, clrBtnHilite, clrBtnShadow);
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
		break;
	}
//		// fall through...
	case splitBar:
//		if (!afxData.bWin4)
		{
			DWORD clrBtnShadow = GetSysColor(COLOR_BTNSHADOW);
			DWORD clrBtnHilite = GetSysColor(COLOR_BTNHILIGHT);
			pDC->Draw3dRect(rect, clrBtnHilite, clrBtnShadow);
			rect.InflateRect(-CX_BORDER, -CY_BORDER);
		}
		break;

	default:
		ASSERT(FALSE);  // unknown splitter type
	}

	// fill the middle
//	COLORREF clr = afxData.clrBtnFace;
	DWORD clrBtnFace = GetSysColor(COLOR_BTNFACE);
	pDC->FillSolidRect(rect, clrBtnFace);

}



// Set the view at the specified row and column
//. does this work??
BOOL CSplitterWndEx::SetPane(int row, int col, CView *pView)
{
	//ASSERT(is child);
	ASSERT (GetDlgItem(IdFromRowCol(row, col)) == NULL);

	// make view a child of the splitter
	pView->SetParent(this);

	// Assign control id to view
	pView->SetDlgCtrlID(IdFromRowCol(row, col));

//	ASSERT((int)_AfxGetDlgCtrlID(pView->m_hWnd) == IdFromRowCol(row, col));

	return TRUE;
}


// Clear the contents of the specified pane
BOOL CSplitterWndEx::ClearPane(int row, int col)
{
	CView* pView = DYNAMIC_DOWNCAST(CView, GetPane(row, col));
	if (pView)
		pView->SetDlgCtrlID(0);
	return TRUE;
}


void CSplitterWndEx::SetMode(ULONG lngMode)
{
	m_lngMode = lngMode;
	if (lngMode == modeSplitterContents)
	{
		m_cxBorder = 0;
		m_cyBorder = 0;
	}
}




// advanced overridable 
void CSplitterWndEx::RecalcLayout()
{
	// Call base class
	CSplitterWnd::RecalcLayout();

	// Now tell parent that panes were resized
//	if (m_lngMode == modeSplitterContents)
//	{
//		AfxMessageBox("recalclayout!");
//	}
}



// User stopped tracking
/*
void CSplitterWndEx::StopTracking(BOOL bAccept)
{
	// Call base class
//	CSplitterWnd::Stop
//	AfxMessageBox("User stopped tracking - modify pdattabs if bAccept true");	
}
*/




void CSplitterWndEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	//trace("CSplitterWndEx::OnLButtonDown\n");

	// Call base class handler
	CSplitterWnd::OnLButtonDown(nFlags, point);

	//. Note: If you want to prevent splitter from resizing, call CWnd's version instead!
	// could use in home page, for instance
//	CWnd::OnLButtonDown(nFlags, point);
}


// This gets called after user resizes a splitter pane
void CSplitterWndEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	//trace("CSplitterWndEx::OnLButtonUp\n");

	// Call base class handler
	CSplitterWnd::OnLButtonUp(nFlags,point);
	
	// Now tell parent that panes were resized
	if (m_lngMode == modeSplitterContents)
	{
		// Note: This splitter window is contained in a tab control which is contained in the cviewtabs window
		// so get the parent of the parent
		CTabCtrlEx* pwnd = STATIC_DOWNCAST(CTabCtrlEx, GetParent());
		ASSERT_VALID(pwnd);
		CViewTabs* pview = DYNAMIC_DOWNCAST(CViewTabs, pwnd->GetParent());
		ASSERT_VALID(pview);
		// Call method in CViewTabs which will store the new heights
		pview->OnSplitterPaneResized();
	}
	else
	{
		//. user repositioned main splitter - store new % width in document?
	}
} 




void CSplitterWndEx::OnSize(UINT nType, int cx, int cy)
{
	// Call base class handler
	CSplitterWnd::OnSize(nType, cx, cy);

	//. bug - this is getting called after closing print preview for some reason, and repositioning the splitter. 20030315
	//	    OnSetPreviewMode takes over window and gets this messed up

	// Now tell parent that splitter was resized
	if (m_lngMode == modeSplitterContents)
	{
		//trace("CSplitterWndEx::OnSize cx %d  cy %d\n", cx, cy);

		// Note: This splitter window is contained in a tab control which is contained in the cviewtabs window
		// so get the parent of the parent
		CTabCtrlEx* pwnd = STATIC_DOWNCAST(CTabCtrlEx, GetParent());
		ASSERT_VALID(pwnd);
		CViewTabs* pview = DYNAMIC_DOWNCAST(CViewTabs, pwnd->GetParent());
		ASSERT_VALID(pview);
		// Call method in CViewTabs which will set new heights
		pview->OnSplitterResized();
	}
	else
	{
		//. adjust main splitter position
	}
}




// Get height of specified row as percent of total splitter window height
int CSplitterWndEx::GetRowHeightPct(int nRow)
{
	int nHeight, nMin;
	GetRowInfo(nRow, nHeight, nMin);

	CRect r;
	GetClientRect(r);
	int nHeightPct = nHeight * 100 / r.bottom;
	return nHeightPct;
}


// Set height of specified row as percent of total splitter window height
void CSplitterWndEx::SetRowHeightPct(int nRow, int nHeightPct, BOOL bRecalcLayout)
{
	CRect r;
	GetClientRect(r);
	int nTotalHeight = r.bottom;
	// If this splitter has no height, use parent window's height
	if (nTotalHeight == 0)
	{
		CWnd* pParent = GetParent();
		pParent->GetClientRect(r);
		nTotalHeight = r.bottom;
	}
	if (nTotalHeight != 0)
	{
		int nPaneHeight = nHeightPct * nTotalHeight / 100;
		//trace("CSplitterWndEx::SetRowHeightPct nRow %d  nHeightPct %d  nPaneHeight %d\n", nRow, nHeightPct, nPaneHeight);
		SetRowInfo(nRow, nPaneHeight, 0);
		// Now recalculate the layout!
		if (bRecalcLayout) RecalcLayout();
	}
}




// Get height of specified row
int CSplitterWndEx::GetRowHeight(int nRow)
{
	int nHeight, nMin;
	GetRowInfo(nRow, nHeight, nMin);
	return nHeight;
}


// Set height of specified row 
void CSplitterWndEx::SetRowHeight(int nRow, int nHeight, BOOL bRecalcLayout)
{
	SetRowInfo(nRow, nHeight, 0);
	// Now recalculate the layout!
	if (bRecalcLayout) RecalcLayout();
}


