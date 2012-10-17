
// CToolBarRtf



#include "precompiled.h"
#include "NeoMem.h"
#include "ToolBarRtf.h"


#include "ViewRtf.h" //. not ideal to have it bound to the view like this




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CToolBarRtf, CToolBarCtrlEx)


BEGIN_MESSAGE_MAP(CToolBarRtf, CToolBarCtrlEx)
	//{{AFX_MSG_MAP(CToolBarRtf)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_CBN_SELENDOK(ID_FONT_NAME, OnSelectFont)
	ON_CBN_SELENDOK(ID_FONT_SIZE, OnSelectSize)
    ON_MESSAGE(CPN_SELENDOK, OnSelEndOK) //,
    ON_MESSAGE(CBN_CLICK, OnColorButtonClick) //,
END_MESSAGE_MAP()


static int nFontSizes[] = {8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 32, 36, 48, 72};	



// Construction/Destruction
//--------------------------------------------------------------------------------------

CToolBarRtf::CToolBarRtf()
{
}

CToolBarRtf::~CToolBarRtf()
{
}






int CToolBarRtf::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect r;

	if (CToolBarCtrlEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Load toobar from resource file
	if (!LoadToolBar(MAKEINTRESOURCE(IDR_TOOLBAR_RTF)))
		return -1;

	// Add font name and size combo boxes to toolbar
	//, clean this up some more

	// Get size of average characters
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&app.m_fontControls);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	int cxChar = tm.tmAveCharWidth;
	int cyChar = tm.tmHeight + tm.tmExternalLeading;

	// Create Font Name combo and add to toolbar at dummy button position.
	int nWidth = cxChar * 30;
	int nHeight = cyChar * 16;
	SetButtonInfo2(ID_FONT_NAME, TBSTYLE_SEP, nWidth);
	GetItemRectFromID(ID_FONT_NAME, &r);
	r.bottom = r.top + nHeight;
	if(!m_cboFontName.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_SORT |
														CBS_DROPDOWNLIST, r, this, ID_FONT_NAME))
		return -1;

	// Fill the combo with list of installed fonts
	CDC* pDC = GetDC();
	HDC hDC = pDC->m_hDC;
	LOGFONT lf;
//	lf.lfCharSet = DEFAULT_CHARSET; // specifies all fonts
	lf.lfCharSet = ANSI_CHARSET; // select the Character Set to use 
						// (this specifies the ASCII based ANSI charset, which is the normal Windows Character Set)
//	strcpy(lf.lfFaceName, ""); // empty string
	::lstrcpyn(lf.lfFaceName, "", LF_FACESIZE); //. on NT this is implemented as Unicode
	lf.lfPitchAndFamily = 0; // except for hebrew and arabic
	::EnumFontFamiliesEx(hDC, &lf, (FONTENUMPROC) EnumFontFamExProc, (LPARAM) this, 0);


	// Create the Font Size combo and add it to the toolbar
	nWidth = cxChar * 10; 
	SetButtonInfo2(ID_FONT_SIZE, TBSTYLE_SEP, nWidth);
	GetItemRectFromID(ID_FONT_SIZE, &r);
	r.bottom = r.top + nHeight;
	if (!m_cboFontSize.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN, r, this, 
					ID_FONT_SIZE))
		return -1;

	// Fill with sizes
	int nCount = sizeof (nFontSizes) / sizeof (int);
	CString str;
	for (int i = 0; i < nCount; i++)
	{
		str.Format(_T("%d"), nFontSizes[i]);
		m_cboFontSize.AddString(str);
	}

	// Assign font to controls
	m_cboFontName.SetFont(&app.m_fontControls);
	m_cboFontSize.SetFont(&app.m_fontControls);


	// Create color button controls
	nWidth = 36; // pixels
	SetButtonInfo2(ID_FORMAT_BACKCOLOR, TBSTYLE_SEP, nWidth);
	SetButtonInfo2(ID_FORMAT_FORECOLOR, TBSTYLE_SEP, nWidth);
	GetItemRectFromID(ID_FORMAT_BACKCOLOR, &r);
	r.right = r.left + nWidth;
	if (!m_ctlBackcolor.Create2(r, this, ID_FORMAT_APPLY_BACKCOLOR, ID_FORMAT_BACKCOLOR))
		return -1;
	m_ctlBackcolor.SetDefaultColor(Library::clrWindow); // should be white // (this value ignored)
	m_ctlBackcolor.SetColor(app.m_clrTextViewBackcolor);
	GetItemRectFromID(ID_FORMAT_FORECOLOR, &r);
	r.right = r.left + nWidth;
	if(!m_ctlForecolor.Create2(r, this, ID_FORMAT_APPLY_FORECOLOR, ID_FORMAT_FORECOLOR))
		return -1;
	m_ctlForecolor.SetDefaultColor(Library::clrWindowText); // (this value ignored)
	m_ctlForecolor.SetColor(app.m_clrTextViewForecolor);



	return 0;
}



int CALLBACK CToolBarRtf::EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam)
{
	
	CToolBarRtf* pWnd = (CToolBarRtf*) lParam;
	pWnd->m_cboFontName.AddString(lpelfe->elfLogFont.lfFaceName);
	// The return value must be a nonzero value to continue enumeration; 
	// to stop enumeration, the return value must be zero.
	return 1;
}





// Message Handlers
//--------------------------------------------------------------------------------------


// A font was selected in the font name combo.
// Need to tell the parent view so it can update the rtf selection.
void CToolBarRtf::OnSelectFont()
{
	int nIndex = m_cboFontName.GetCurSel();
	if (nIndex != CB_ERR)
	{
		CString str;
		m_cboFontName.GetLBText(nIndex, str);
		CViewRtf* pView = DYNAMIC_DOWNCAST(CViewRtf, GetParent());
		ASSERT_VALID(pView);
		pView->ChangeFont(str);
	}
}


// A font was selected in the font size combo.
// Need to tell the parent view so it can update the rtf selection.
void CToolBarRtf::OnSelectSize()
{
	int nIndex = m_cboFontSize.GetCurSel();
	if (nIndex != CB_ERR)
	{
//		m_cboFontSize.GetLBText(nIndex, str);
		int nSize = nFontSizes[nIndex] * 20; // convert to twips
		CViewRtf* pView = DYNAMIC_DOWNCAST(CViewRtf, GetParent());
		ASSERT_VALID(pView);
		pView->ChangeSize(nSize);
	}
}


// Pass message on to parent
//, get rid of this
LRESULT CToolBarRtf::OnSelEndOK(WPARAM wParam, LPARAM lParam) 
{
	CWnd *pParent = GetParent();
	if (pParent) 
	{
		pParent->SendMessage(CPN_SELENDOK, wParam, lParam);
	}	
	return TRUE;
}


// Pass message on to parent
//, get rid of this
LRESULT CToolBarRtf::OnColorButtonClick(WPARAM wParam, LPARAM lParam) 
{
	CWnd *pParent = GetParent();
	if (pParent) 
	{
		pParent->SendMessage(CBN_CLICK, wParam, lParam);
	}	
	return TRUE;
}




