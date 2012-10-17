
// CDialogTip


#include "precompiled.h"
#include "NeoMem.h"
#include "DialogTip.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDialogTip dialog


CDialogTip::CDialogTip(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTip::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTip)
	m_bShowTips = FALSE;
	//}}AFX_DATA_INIT
//	m_brush.CreateSolidBrush(0x00ffffff); //m_colorBackground);
}


void CDialogTip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTip)
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	DDX_Control(pDX, IDC_BACKGROUND, m_staticBackground);
	DDX_Check(pDX, IDC_CHK_SHOW_TIPS, m_bShowTips);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTip, CDialog)
	//{{AFX_MSG_MAP(CDialogTip)
	ON_BN_CLICKED(IDC_BTN_NEXT_TIP, OnBtnNextTip)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDialogTip message handlers

BOOL CDialogTip::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Set label to bold
	m_lblInstructions.SetFont(&app.m_fontControlsBold);

	GetNextTip();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogTip::GetNextTip()
{
	if (m_nLastTip == 0)
		m_nLastTip = IDS_TIP1;

	// If can't find tip, then reached the end of the list, so start over at the beginning
	//. catch memory exception
	if (!m_strTip.LoadString(m_nLastTip))
	{
		m_nLastTip = IDS_TIP1;
		m_strTip.LoadString(m_nLastTip);
	}

	m_nLastTip++;
//	if (m_nLastTip > IDS_TIPLAST)
//		m_nLastTip = 0;

	Invalidate();
}


void CDialogTip::OnBtnNextTip() 
{
	GetNextTip();
}





HBRUSH CDialogTip::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
//	switch (nCtlColor)
//	{
//	case CTLCOLOR_BTN:
//	case CTLCOLOR_STATIC:
//		pDC->SetTextColor(0x0); // m_colorForeground);
//		pDC->SetBkColor(0x00ffffff); // m_colorBackground);
//		// Drop through to return the background brush.
//	case CTLCOLOR_DLG:
//		return (HBRUSH) (m_brush.GetSafeHandle());
//	}
	return hbr;
}





void CDialogTip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Fill in white background
	CRect r;
	m_staticBackground.GetWindowRect(r);
	ScreenToClient(r);
	dc.FillSolidRect(r, 0x00ffffff);

	// Draw 3d borders
	dc.Draw3dRect(r, Library::clr3dShadow, Library::clr3dHighlight);

	// Write tip
	CGdiObject* pgdiobj = dc.SelectStockObject(ANSI_VAR_FONT);
	r.DeflateRect(40, 25, 5, 5);
	dc.SetBkColor(0x00ffffff);
	dc.DrawText(m_strTip, r, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK);
	dc.SelectObject(pgdiobj);

	// Draw lightbulb
	CBitmap bmp;
	if (bmp.LoadBitmap(IDB_LIGHTBULB))
	{
		// Get the size of the bitmap
		//, move this to CBitmapEx
		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);

		// Create an in-memory DC compatible with the display DC we're using to paint
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(&dc);

		// Select the bitmap into the in-memory DC
		CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);

		// Get position for bitmap
		r.InflateRect(40 - 5, 0, 0, 0);
		int nX = r.left;
		int nY = r.top;

		// Copy the bits from the in-memory DC into the on-screen DC to 
		// actually do the painting. Use the centerpoint we computed for the target offset.
		dc.BitBlt(nX, nY, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY);

		dcMemory.SelectObject(pOldBitmap);

	}

	// Do not call CDialog::OnPaint() for painting messages
}


