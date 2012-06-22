
// CPageOptionsDisplay



#include "precompiled.h"
#include "NeoMem.h"
#include "PageOptionsDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





IMPLEMENT_DYNCREATE(CPageOptionsDisplay, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageOptionsDisplay, CPropertyPage)
	//{{AFX_MSG_MAP(CPageOptionsDisplay)
	ON_BN_CLICKED(IDC_BTN_FONT, OnBtnFont)
	ON_BN_CLICKED(IDC_BTN_DATE_FORMAT, OnBtnDateFormat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CPageOptionsDisplay::CPageOptionsDisplay() : CPropertyPage(CPageOptionsDisplay::IDD)
{
	//{{AFX_DATA_INIT(CPageOptionsDisplay)
//	m_bShowTips = FALSE;
	m_nNameFormat = -1;
	//}}AFX_DATA_INIT
	m_bFontChanged = FALSE;
}

CPageOptionsDisplay::~CPageOptionsDisplay()
{
}




BOOL CPageOptionsDisplay::OnInitDialog() 
{
	// Call base class (calls DoDataExchange)
	CPropertyPage::OnInitDialog();

	// Create font from logfont and get name
	m_font.CreateFontIndirect(&m_lf);
	UpdateFontName();
	
	m_lblFont.SetFont(&theApp.m_fontControlsBold);
	m_lblDateFormat.SetFont(&theApp.m_fontControlsBold);
	m_lblNameFormat.SetFont(&theApp.m_fontControlsBold);

	// Get available date formats and fill the combo
	// Unfortunately, windows 98 only
//	::EnumDateFormatsEx((DATEFMT_ENUMPROCEX) EnumDateFormatsProcEx, LOCALE_USER_DEFAULT, DATE_LONGDATE);
//	::EnumDateFormatsEx((DATEFMT_ENUMPROCEX) EnumDateFormatsProcEx, LOCALE_USER_DEFAULT, DATE_SHORTDATE);
//	::EnumDateFormatsEx((DATEFMT_ENUMPROCEX) EnumDateFormatsProcEx, LOCALE_USER_DEFAULT, DATE_YEARMONTH);
	UpdateSampleDate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPageOptionsDisplay::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageOptionsDisplay)
	DDX_Control(pDX, IDC_LBL_FONT, m_lblFont);
	DDX_Control(pDX, IDC_LBL_NAME_FORMAT, m_lblNameFormat);
	DDX_Control(pDX, IDC_LBL_DATE_FORMAT, m_lblDateFormat);
	DDX_Control(pDX, IDC_LBL_DATE, m_lblDate);
	DDX_Control(pDX, IDC_TXT_FONT, m_txtFont);
	DDX_Control(pDX, IDC_TXT_DATE, m_txtDate);
	DDX_Control(pDX, IDC_BTN_FONT, m_btnFont);
	DDX_Control(pDX, IDC_BTN_DATE_FORMAT, m_btnDateFormat);
	DDX_Control(pDX, IDC_CBO_NAME_FORMAT, m_cboNameFormat);
	DDX_Control(pDX, IDC_CBO_DATE_FORMAT, m_cboDateFormat);
//	DDX_Check(pDX, IDC_CHK_SHOW_TIPS, m_bShowTips);
	DDX_CBIndex(pDX, IDC_CBO_NAME_FORMAT, m_nNameFormat);
	DDX_CBIndex(pDX, IDC_CBO_DATE_FORMAT, m_nDateFormat);
	//}}AFX_DATA_MAP
}



// Get name of font
void CPageOptionsDisplay::UpdateFontName()
{
	CString strFont = m_font.GetAsText();
	m_txtFont.SetWindowText(strFont);
}


// Select new font
void CPageOptionsDisplay::OnBtnFont() 
{
	CFontDialog dlg(&m_lf);
//.	dlg.m_cf.rgbColors = m_color;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&m_lf);
		m_font.DeleteObject();
		m_font.CreateFontIndirect(&m_lf);
//		m_color = dlg.GetColor();
		UpdateFontName();
		// Set flag
		m_bFontChanged = TRUE;
	}
}




BOOL CPageOptionsDisplay::OnApply() 
{
	// Apply changes
//	theApp.m_fontRtfDefault.DeleteObject();
//	theApp.m_fontRtfDefault.CreateFontIndirect(&m_lf);
//	theApp.m_bShowTips = m_bShowTips;

	//. will need to notify rtf view also - send a hint?
	// ie tell the app, which will walk through docs, calling updateallviewsEx with hintRtfDefaultFontChange
	// or something like that
	// or the app could find all open rtf views and call a method directly on them,
	// or send a custom message
//	AfxGetApp()->NotifyRtfDefaultChange();

	return CPropertyPage::OnApply();
}






// Bring up control panel for regional settings to modify date format
void CPageOptionsDisplay::OnBtnDateFormat() 
{
	// To make a Shortcut to a Control Panel applet, use the syntax 
	//		CONTROL.EXE {cpl filename}, {applet name}, {tab num}
	// You'll find the .CPL files in your System folder and the applet names in the 
	// Control Panel itself. Most applets have several tab sheets. Launch the applet 
	// to find the tab number you want; the first tab on the left is 0.
	// Note: The CreateProcess function is used to run a new program. The WinExec and 
	// LoadModule functions are still available, but they are implemented as calls to CreateProcess. 
	// Note: Some control panels will parse the tab number and some won't - this one doesn't seem to do it. 
	::WinExec("Control.exe Intl.cpl, Regional Settings, 4", SW_NORMAL);
	UpdateSampleDate();
}








void CPageOptionsDisplay::UpdateSampleDate()
{
	COleDateTime dt;
	dt = COleDateTime::GetCurrentTime();
	CString strDate = dt.Format(VAR_DATEVALUEONLY);
	m_txtDate.SetWindowText(strDate);
}




BOOL CPageOptionsDisplay::OnSetActive() 
{
	UpdateSampleDate();
	
	return CPropertyPage::OnSetActive();
}




