
// CPreviewViewEx
// from MFC


#include "precompiled.h"
#include "NeoMem.h"
#include "PreviewViewEx.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CPreviewViewEx, CPreviewView)


BEGIN_MESSAGE_MAP(CPreviewViewEx, CPreviewView)
	//{{AFX_MSG_MAP(CPreviewViewEx)
	ON_BN_CLICKED(ID_PREVIEW_PRINT, OnPreviewPrint)
	ON_BN_CLICKED(ID_PREVIEW_SETUP, OnPreviewSetup)
	ON_BN_CLICKED(ID_PREVIEW_HELP, OnPreviewHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CPreviewViewEx::CPreviewViewEx()
{
}

CPreviewViewEx::~CPreviewViewEx()
{
}



// Drawing

void CPreviewViewEx::OnInitialUpdate()
{
	CPreviewView::OnInitialUpdate();

	//. calculate the total size of this view
//	CSize sizeTotal;
//	sizeTotal.cx = sizeTotal.cy = 100;
//	SetScrollSizes(MM_TEXT, sizeTotal);
}


//` Code taken from base class
void CPreviewViewEx::OnDraw(CDC* pDC)
{
	// Call base class
	CPreviewView::OnDraw(pDC);

	// base class code...
/*
	ASSERT_VALID(pDC);

	// don't do anything if not fully initialized
	if (m_pPrintView == NULL || m_dcPrint.m_hDC == NULL)
		return;

	CPoint ViewportOrg = pDC->GetViewportOrg();

	CPen rectPen;
	rectPen.CreatePen(PS_SOLID, 2, GetSysColor(COLOR_WINDOWFRAME));
	CPen shadowPen;
	shadowPen.CreatePen(PS_SOLID, 3, GetSysColor(COLOR_BTNSHADOW));

	m_pPreviewInfo->m_bContinuePrinting = TRUE;     // do this once each paint

	for (UINT nPage = 0; nPage < m_nPages; nPage++)
	{
		int nSavedState = m_dcPrint.SaveDC();       // Save pristine state of DC

		// Use paint DC for print preview output
		m_pPreviewDC->SetOutputDC(pDC->GetSafeHdc());

		m_pPreviewInfo->m_nCurPage = m_nCurrentPage + nPage;

		// Only call PrepareDC if within page range, otherwise use default
		// rect to draw page rectangle
		if (m_nCurrentPage + nPage <= m_pPreviewInfo->GetMaxPage())
			m_pPrintView->OnPrepareDC(m_pPreviewDC, m_pPreviewInfo);

		// Set up drawing rect to entire page (in logical coordinates)
		m_pPreviewInfo->m_rectDraw.SetRect(0, 0,
			m_pPreviewDC->GetDeviceCaps(HORZRES),
			m_pPreviewDC->GetDeviceCaps(VERTRES));
		m_pPreviewDC->DPtoLP(&m_pPreviewInfo->m_rectDraw);

		// Draw empty page on screen

		pDC->SaveDC();          // save the output dc state

		CSize* pRatio = &m_pPageInfo[nPage].sizeScaleRatio;
		CRect* pRect = &m_pPageInfo[nPage].rectScreen;

		if (pRatio->cx == 0)
		{   // page position has not been determined
			PositionPage(nPage);    // compute page position
			if (m_nZoomState != ZOOM_OUT)
				ViewportOrg = -GetDeviceScrollPosition();
		}

		pDC->SetMapMode(MM_TEXT);   // Page Rectangle is in screen device coords
		pDC->SetViewportOrg(ViewportOrg);
		pDC->SetWindowOrg(0, 0);

		pDC->SelectStockObject(HOLLOW_BRUSH);
		pDC->SelectObject(&rectPen);
		pDC->Rectangle(pRect);

		pDC->SelectObject(&shadowPen);

		pDC->MoveTo(pRect->right + 1, pRect->top + 3);
		pDC->LineTo(pRect->right + 1, pRect->bottom + 1);
		pDC->MoveTo(pRect->left + 3, pRect->bottom + 1);
		pDC->LineTo(pRect->right + 1, pRect->bottom + 1);

		// erase background to white (most paper is white)
		CRect rectFill = *pRect;
		rectFill.left += 1;
		rectFill.top += 1;
		rectFill.right -= 2;
		rectFill.bottom -= 2;
		::FillRect(pDC->m_hDC, rectFill, (HBRUSH)GetStockObject(WHITE_BRUSH));

		pDC->RestoreDC(-1);     // restore to synchronized state

		if (!m_pPreviewInfo->m_bContinuePrinting ||
				m_nCurrentPage + nPage > m_pPreviewInfo->GetMaxPage())
		{
			m_pPreviewDC->ReleaseOutputDC();
			m_dcPrint.RestoreDC(nSavedState);   // restore to untouched state

			// if the first page is not displayable, back up one page
			// but never go below 1
			if (nPage == 0 && m_nCurrentPage > 1)
				SetCurrentPage(m_nCurrentPage - 1, TRUE);
			break;
		}

		// Display page number
		OnDisplayPageNumber(m_nCurrentPage, nPage + 1);

		// Set scale ratio for this page
		m_pPreviewDC->SetScaleRatio(pRatio->cx, pRatio->cy);

		CSize PrintOffset;
		VERIFY(m_pPreviewDC->Escape(GETPRINTINGOFFSET, 0, NULL, (LPVOID)&PrintOffset));
		m_pPreviewDC->PrinterDPtoScreenDP((LPPOINT)&PrintOffset);
		PrintOffset += (CSize)pRect->TopLeft();
		PrintOffset += CSize(1, 1);
		PrintOffset += (CSize)ViewportOrg;  // For Scrolling

		m_pPreviewDC->SetTopLeftOffset(PrintOffset);

		m_pPreviewDC->ClipToPage();
		m_pPrintView->OnPrint(m_pPreviewDC, m_pPreviewInfo);

		m_pPreviewDC->ReleaseOutputDC();

		m_dcPrint.RestoreDC(nSavedState);   // restore to untouched state

	}

	rectPen.DeleteObject();
	shadowPen.DeleteObject();
*/
}



// Diagnostics

#ifdef _DEBUG
void CPreviewViewEx::AssertValid() const
{
	CPreviewView::AssertValid();
}

void CPreviewViewEx::Dump(CDumpContext& dc) const
{
	CPreviewView::Dump(dc);
}
#endif //_DEBUG



// CPreviewViewEx message handlers
void CPreviewViewEx::OnPreviewSetup() 
{
	//, this works, but getting preview to recognize changes might take a while, so leaving button out for v1
//	AfxMessageBox("hi!");
//	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
//	ON_COMMAND(ID_FILE_PAGE_SETUP, OnFilePageSetup)
	app.m_pMainWnd->SendMessage(WM_COMMAND, ID_FILE_PRINT_SETUP);
}


void CPreviewViewEx::OnPreviewPrint() 
{
	// code taken from mfc's base class..
//	CPreviewView::OnPreviewPrint();

	OnPreviewClose(); // force close of Preview

	// set variable with view to print...
//	app.m_lngPrintViewID = ;

	// cause print (can be overridden by catching the command)
	CWnd* pMainWnd = AfxGetThread()->m_pMainWnd;
	ASSERT_VALID(pMainWnd);
	pMainWnd->SendMessage(WM_COMMAND, ID_FILE_PRINT_FROM_PREVIEW);

	// clear view variable
	app.m_lngPrintViewID = 0;
}



void CPreviewViewEx::OnActivateView(BOOL bActivate, CView*, CView*)
{
	if (bActivate)
	{
		CWnd* pFocusWnd = GetFocus();
		if (pFocusWnd == NULL ||
			(m_pToolBar != NULL && !m_pToolBar->IsChild(pFocusWnd)))
		{
			// focus is not already on a toolbar button - set it to one
			// bug: had to override this method because i had change this button id - 
			// barely managed to catch this by using calculator and switching back to neomem
//`			m_pToolBar->GetDlgItem(AFX_ID_PREVIEW_PRINT)->SetFocus();
			// this bombed too - just set to toolbar
//			m_pToolBar->GetDlgItem(ID_FILE_PRINT_FROM_PREVIEW)->SetFocus(); 
			m_pToolBar->SetFocus();
		}
	}
}




void CPreviewViewEx::OnPreviewHelp() 
{
	app.WinHelp(HID_BASE_RESOURCE + IDD_PREVIEW_TOOLBAR); // shows default topic (overview)		
}


