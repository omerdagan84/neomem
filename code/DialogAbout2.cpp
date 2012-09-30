
// CDialogAbout2


#include "precompiled.h"
#include "DialogAbout2.h"

#include "NeoMem.h"
#include "HelpIDs.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CDialogAbout2, CDialog)
	//{{AFX_MSG_MAP(CDialogAbout2)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_CREDITS, OnBtnCredits)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TBC, OnSelchangeTbs)
	ON_COMMAND(ID_TAB_ABOUT, OnTabAbout)
	ON_COMMAND(ID_TAB_CREDITS, OnTabCredits)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TBS, OnSelchangeTbs)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CDialogAbout2::CDialogAbout2(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAbout2::IDD, pParent)
{
	// Get accelerators
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DIALOG_ABOUT2)); 

	//{{AFX_DATA_INIT(CDialogAbout2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}




BOOL CDialogAbout2::OnInitDialog() 
{
	// Call base class
	CDialog::OnInitDialog();

	// Subclass control
//	m_rtf.SubclassDlgItem(IDC_RTF_ABOUT, this);

	// Initialize tab control
	m_tbc.SetFont(&theApp.m_fontControls);
	m_tbc.InsertItem(TCIF_TEXT, 0, "  &About    ", 0, 0);
	m_tbc.InsertItem(TCIF_TEXT, 1, "  C&redits  ", 0, 0);

	// Put rtf inside tab control
	CRect r;
	m_tbc.GetClientRect(r);
	m_tbc.AdjustRect(FALSE, r);
	m_rtf.SetParent(&m_tbc);
	m_rtf.MoveWindow(r, TRUE);
	
	// Check for richedit enhanced features
	LPCTSTR szEnhancedFeatures = "";
	float fVersion = theApp.m_objInfo.GetRichEditVersionNumber();
	if (fVersion < 5.0f)
		szEnhancedFeatures = "{\\b (Enhanced features not available) }";

	// Build string
	// these work!
//	"{3.1x10\\super 18\\par}"
//	"{Say f\\sub x = 312.3y\\par}"
	// these don't!
//	"{\\uld This is underline dotted\\par}"
//	"{\\ulhair This is underline hair!!\\par}"
//	"{\\ulwave This is underline wave!!\\par}"
//	CString strText;
	m_strAbout.Format(
					"{\\rtf1\\ansi\\deff0\\deftab720\\deflang1033"
					"{\\fonttbl{\\f0\\fswiss MS Sans Serif;}"
					"{\\f1\\froman\\fcharset2 Symbol;}{\\f2\\fswiss\\fprq2 Arial;}"
				    "{\\f3\\fswiss\\fcharset1 Arial;}{\\f4\\fswiss\\fprq2 System;}}"
					"{\\colortbl\\red0\\green0\\blue0;\\red0\\green128\\blue0;\\red128\\green128\\blue0;\\red160\\green160\\blue160;}"
					"\\pard\\plain \\li60 \\f2\\fs32\\cf1 " // li = left indent
					"NeoMem Version %s \\par" // version 
					"\\plain\\f2\\fs16\\cf3 %s" // build
					"\\par %s " // copyright
					"\\par \\plain\\f2\\fs16 "
					"\\par An information database. "
					"\\par NeoMem is free software, released under the MIT X11 license. "
					"\\par "
					//, must include "mailto:" to get email to work
//					"\\par Visit our web site at: \\plain\\f2\\fs16\\cf2\\ul %s\\plain\\f3\\fs16 " // website
					"\\par For the latest updates and information visit \\plain\\f2\\fs16\\cf2\\ul %s\\plain\\f3\\fs16 " // website
					"\\par Send any comments and suggestions to \\plain\\f2\\fs16\\cf2\\ul %s\\plain\\f2\\fs16 " // email
					"\\par "
					"\\par {\\b System Information }"
//					"\\par     NeoMem Build Date: %s " // build date
					"\\par     Processor: %s " // processor
					"\\par     OS: %s " // windows version
					"\\par     Common Controls Version: %.2f " // common controls version
					"\\par     RichEdit Control Version: %s %s" // rich edit version, enhanced features avail
					"\\par     Memory: %s, %s " // Virtual, physical memory
					"\\par     Screen: %s " // screen
//					"\\par     Disk Space Available: %s " // disk free info 
//					"\\par "
//					"\\par "
					"\\par }",
					(LPCTSTR) theApp.m_objInfo.GetProgramVersion(),
					(LPCTSTR) theApp.m_objInfo.GetProgramBuild(),
//					(LPCTSTR) theApp.m_objInfo.GetProgramDate(),
					(LPCTSTR) theApp.m_objInfo.GetProgramCopyright(),

					(LPCTSTR) theApp.m_strWebsite,
					(LPCTSTR) theApp.m_strEmail,

					(LPCTSTR) theApp.m_objInfo.GetProcessor(),
					(LPCTSTR) theApp.m_objInfo.GetWindowsVersion(),
					theApp.m_objInfo.GetCommonControlsVersionNumber(),
					(LPCTSTR) theApp.m_objInfo.GetRichEditVersionString(),
					(LPCTSTR) szEnhancedFeatures, 
					(LPCTSTR) theApp.m_objInfo.GetVirtualMemory(),
					(LPCTSTR) theApp.m_objInfo.GetPhysicalMemory(),
					(LPCTSTR) theApp.m_objInfo.GetScreenInfo()
//					(LPCTSTR) theApp.m_objInfo.GetDiskSpaceAvailable(),
					);

	m_strCredits = 
					"{\\rtf1\\ansi\\deff0\\deftab720\\deflang1033"
					"{\\fonttbl{\\f0\\fswiss\\fprq2 Arial;}}"
					"{\\colortbl\\red0\\green0\\blue0;\\red160\\green160\\blue160;\\red172\\green160\\blue140;}"
					"\\pard\\plain \\li60 " // li = left indent
					"\\plain\\fs32\\cf2 Credits \\par "
					"\\plain\\fs16 "
//					"\\par NeoMem was developed with the following tools: "
//					"\\par \\tab Development Environment: Microsoft Visual C++ 6.0/MFC"
//					"\\par \\tab Installation Program: Inner-Smile INF-Tool"
					"\\par Thanks to the following: "
					"\\par "
					"\\par     {\\b Brent Corkum} for Owner Draw Menus "
					"\\par     {\\b Chris Maunder and James White} for Color Picker Control"
					"\\par     {\\b The Code Project} for various code samples"
					"\\par "
					"\\par Thanks also to those who registered version 1.0 and have written in with comments and suggestions. "
					"\\par "
					"\\par Brian Burns"
					"\\par NeoMem.org"
					"\\par "
					"\\par }";


	// Turn on auto url detect
	// looks better without it (must add mailto: to email blech)
//	m_rtf.AutoURLDetect(TRUE);

	// Add text to control
	m_tbc.SetCurSel(0); // about 
	m_rtf.SetRtf(m_strAbout);

	// Make rtf read-only
	// (already done in dialog editor)

//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	m_btnClose.SetFocus();
	return FALSE;
}



void CDialogAbout2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogAbout2)
	DDX_Control(pDX, IDC_TBC, m_tbc);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_RTF, m_rtf);
	//}}AFX_DATA_MAP
}





void CDialogAbout2::OnBtnClose() 
{
	//. bug: beeping after this routine - in call to ::DestroyWindow
	//		sends WM_DESTROY message, received here alright (OnDestroy),
	//		but then after return, some windows code beeps on an int 2bh instruction, which
	//		apparently is called on return from a callback function, which should be a normal
	//		occurrence within mfc, but why the beep? tried disabling various features in the dialog but
	//		to no avail. 
//	m_rtf.SetParent(this);
//	CDialog::EndDialog(IDOK);
	CDialog::OnOK();
}


void CDialogAbout2::OnBtnCredits() 
{
}


void CDialogAbout2::OnSelchangeTbs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LoadTab();
	*pResult = 0;
}



BOOL CDialogAbout2::PreTranslateMessage(MSG* pMsg) 
{
	// Handle accelerators
	if (m_hAccel) {
		if (::TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) {
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CDialogAbout2::OnTabAbout() 
{
	m_tbc.SetCurSel(0);
	LoadTab();
}

void CDialogAbout2::OnTabCredits() 
{
	m_tbc.SetCurSel(1);
	LoadTab();
}



void CDialogAbout2::LoadTab()
{
	int nTab = m_tbc.GetCurSel();
	switch (nTab)
	{
		case 0:
			m_rtf.SetRtf(m_strAbout);
			break;
		case 1:
			m_rtf.SetRtf(m_strCredits);
			break;
	}
}



void CDialogAbout2::OnDestroy() 
{
	CDialog::OnDestroy();
}

