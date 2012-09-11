
// CViewHeader

//, some of this code could be consolidates with cviewex code, now
// that it inherits from that, instead of cview. 



#include "precompiled.h"

#include "NeoMem.h"
#include "ViewHeader.h"
#include "Constants.h"

#include "ConstantsDatabase.h"
#include "BDoc.h"
#include "BObject.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//-------------------------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CViewHeader, CViewEx)

BEGIN_MESSAGE_MAP(CViewHeader, CViewEx)
	//{{AFX_MSG_MAP(CViewHeader)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// CViewHeader
//-------------------------------------------------------------------------------------------

CViewHeader::CViewHeader()
{
	m_pviewTabs = NULL;
	m_nHeight = 0;
}

CViewHeader::~CViewHeader()
{
	//, Why don't we delete m_pView here?
	// MFC handles that for you - when Windows destroys a window, MFC will 
	// delete the attached CWnd object?
}


/* this code is in cviewex
BOOL CViewHeader::PreCreateWindow(CREATESTRUCT& cs) 
{
	// WS_CLIPCHILDREN   Excludes the area occupied by child windows when you draw 
	// within the parent window. Used when you create the parent window.
	// This helps reduce flickering
	cs.style |= WS_CLIPCHILDREN;
	return CViewEx::PreCreateWindow(cs);
}
*/

int CViewHeader::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CViewEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create tab view as child and set it to receive Contents
	m_pviewTabs = DYNAMIC_DOWNCAST(CViewTabs, CreateChildView(RUNTIME_CLASS(CViewTabs), GetDocument(), CRect(0,0,0,0), IDC_TAB_CONTROL));
	ASSERT(m_pviewTabs);
	if (m_pviewTabs)
		m_pviewTabs->SetMode(modeContents);

	m_nHeight = 26; //. height of header - get from font size!!

	return 0;
}


// Drawing
//-------------------------------------------------------------------------------------------


// Display icon and name of currently selected object
void CViewHeader::OnDraw(CDC* pDC)
{
	xTRACE("CViewHeader::OnDraw\n");

	// Exit if display is turned off
	if (!theApp.m_bDisplayHeader) 
		return;

	BObject* pobj = m_pDoc->GetCurrentObject();
	if (pobj == NULL) return; // exit if no current object (eg during delete operation)
	
	// Draw background
	//. could have pattern wallpaper background instead of just a solid color
	//. could change with each item or with class of item
	CRect r;
	GetClientRect(r);
	r.bottom = m_nHeight;
	pDC->FillSolidRect(r, theApp.m_clrHeaderBackground);
	pDC->SetTextColor(theApp.m_clrHeaderForeground);

	// Draw 3d border around all
	pDC->DrawEdge(r,  BDR_RAISEDINNER, BF_RECT);

	// Select font
	CFont* pOldFont = pDC->SelectObject(&theApp.m_fontHeader);

	// Draw icon of currently selected item
	CImageList* piml = m_pDoc->GetImageList();
	int intBorder = (m_nHeight - 16) / 2; // 16 is height of icon
	CPoint ptIcon(intBorder, intBorder);
	int nImage = pobj->GetIconIndex();
	piml->Draw(pDC, nImage, ptIcon, ILD_NORMAL);

	// Draw text of currently selected item
	CRect rectText(intBorder + 16 + intBorder, 1, r.right, m_nHeight-1); // 16 is icon width
	LPCTSTR pszText = pobj->GetPropertyText(propName);
	pDC->DrawText(pszText, -1, rectText, DT_LEFT | DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER);
	
	//. Validate region where child controls go?

	// Deselect font
	pDC->SelectObject(pOldFont);

}



// CViewHeader diagnostics
//-------------------------------------------------------------------------------------------

#ifdef _DEBUG
void CViewHeader::AssertValid() const
{
	CViewEx::AssertValid();
}

void CViewHeader::Dump(CDumpContext& dc) const
{
	CViewEx::Dump(dc);
}
#endif //_DEBUG



// Message Handlers
//-------------------------------------------------------------------------------------------

void CViewHeader::OnSize(UINT nType, int cx, int cy) 
{
	CViewEx::OnSize(nType, cx, cy);
	
	// Size the contained view, if any
	RecalcLayout(FALSE);
}


void CViewHeader::RecalcLayout(BOOL bRepaint)
{
	// Size the contained view, if any
	if (m_pviewTabs)
	{
		CRect r;
		GetClientRect(r);
		if (theApp.m_bDisplayHeader)
			m_pviewTabs->MoveWindow(0, m_nHeight, r.right, r.bottom - m_nHeight, bRepaint);
		else
			m_pviewTabs->MoveWindow(0, 0, r.right, r.bottom, bRepaint);
	}
}





// Bring up context menu
//.. make this a generic menu which comes up on toolbar right click
void CViewHeader::OnRButtonDown(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);

//	if (CViewEx::InitPopup(IDR_POPUP_HEADER))
//	{

	// Display pop up menu
	CMenu menu;
	if (menu.LoadMenu(IDR_POPUP_HEADER))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		if (pPopup)
		{
//			pPopup->SetDefaultItem(ID_OBJ_OPEN);
			// let main window handle commands since theApp handles them
//			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
		}
	}

//	CViewEx::OnRButtonDown(nFlags, point);
}




// Typically you should not perform any drawing directly from OnUpdate. Instead, determine 
// the rectangle describing, in device coordinates, the area that requires updating; pass this 
// rectangle to CWnd::InvalidateRect. This causes painting to occur the next time a WM_PAINT 
// message is received. 
void CViewHeader::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	TRACE("    CViewHeader::OnUpdate %s\n", theApp.GetHintName(lHint));

	switch (lHint)
	{

//	case hintRename:
//		Invalidate();
//		break;

	case hintRefresh:
		Invalidate();
		break;

	case hintPropertyChange:
		Invalidate();
		break;

	case hintResize:
		// This lets the app object force this view's layout to be recalculated.
		// Pass TRUE so that the child view's background is repainted.
		RecalcLayout(TRUE);
		break;

	case hintSelect:
		{
			// Invalidate the header display area so it will be redrawn in OnDraw
			if (theApp.m_bDisplayHeader)
			{
				CRect r;
				GetClientRect(r);
				r.bottom = m_nHeight;
				InvalidateRect(r, FALSE);
			}
			break;
		}

	case hintRefreshAllIcons:
		Invalidate();
		break;

	}	
}







void CViewHeader::OnInitialUpdate() 
{
	// Call base class
	// Note: Base class does this:
	// invalidate the entire pane, erase background too
//	Invalidate(TRUE);
	CViewEx::OnInitialUpdate();
}



void CViewHeader::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	xTRACE("CViewHeader::OnActivateView bActivate %d - if True, set focus to child view (tabs view)\n", bActivate);
	CViewEx::OnActivateView(bActivate, pActivateView, pDeactiveView);
	
	// If this view is being activated, set the focus to the child view
	if (bActivate && m_pviewTabs) 
	{
		ASSERT_VALID(m_pviewTabs);
		m_pviewTabs->SetFocus();
	}
}




void CViewHeader::OnSetFocus(CWnd* pOldWnd) 
{
	xTRACE("CViewHeader::OnSetFocus - set focus to child view (tabview)\n");

	CViewEx::OnSetFocus(pOldWnd);

	// Don't want this view to get focus - want to set focus to its child view.
	if (m_pviewTabs)
	{
		ASSERT_VALID(m_pviewTabs);
		m_pviewTabs->SetFocus();
	}
}




int CViewHeader::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	xTRACE("CViewHeader::OnMouseActivate\n");	
//	return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
//	return MA_NOACTIVATEANDEAT; // Do not activate CWnd object and discard the mouse event

	// If clicked in the visible part of the header window, don't activate the window
	CPoint p(::GetMessagePos()); // get position of message source in screen coords
	CWnd* pwndClick = WindowFromPoint(p);
	ASSERT_VALID(pwndClick);
	// Note: WindowFromPoint may return a temporary cwnd object, so use hwnd instead
//	BOOL bClickedInHeader = (pwndClick == this);
	BOOL bClickedInHeader = (pwndClick->m_hWnd == this->m_hWnd);
	if (bClickedInHeader)
	{
		xTRACE("  clicked in header window, so don't activate\n");
		return MA_NOACTIVATE; // Do not activate CWnd object
	}

	// Otherwise call base class as you normally would
	return CViewEx::OnMouseActivate(pDesktopWnd, nHitTest, message);
}



