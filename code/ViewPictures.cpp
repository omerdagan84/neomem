
// CViewPictures



#include "precompiled.h"
#include "NeoMem.h"
#include "ViewPictures.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CViewPictures, CViewEx)

BEGIN_MESSAGE_MAP(CViewPictures, CViewEx)
	//{{AFX_MSG_MAP(CViewPictures)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




CViewPictures::CViewPictures()
{
}

CViewPictures::~CViewPictures()
{
}




#ifdef _DEBUG
void CViewPictures::AssertValid() const
{
	CViewEx::AssertValid();
}

void CViewPictures::Dump(CDumpContext& dc) const
{
	CViewEx::Dump(dc);
}
#endif //_DEBUG




BOOL CViewPictures::OnEraseBkgnd(CDC* pDC) 
{
//	return CViewEx::OnEraseBkgnd(pDC);

	// Change background color (get from object - handle in onupdate?)
    
	// Set brush to desired background color.
//    CBrush backBrush(RGB(128, 128, 255));
    
	// Save old brush.
//	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
//	CRect rect;
//	pDC->GetClipBox(&rect);     // Erase the area needed.
//	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
//	pDC->SelectObject(pOldBrush);


	CRect r;
	GetClientRect(r);
//	COLORREF color = ::GetSysColor(COLOR_3DFACE);
//	COLORREF color = 0x00e0c0c0;
	COLORREF color = 0x00ff8080;
	pDC->FillSolidRect(r, color);

    return TRUE;
}


