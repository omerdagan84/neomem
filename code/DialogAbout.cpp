
// CDialogAbout
// obsolete



#include "precompiled.h"
#include "DialogAbout.h"
#include "FileVersion.h"
#include "NeoMem.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CDialogAbout, CDialog)
	//{{AFX_MSG_MAP(CDialogAbout)
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDialogAbout::CDialogAbout() : CDialog(CDialogAbout::IDD)
{
	//{{AFX_DATA_INIT(CDialogAbout)
	//}}AFX_DATA_INIT
	m_colorBackground = 0x00ffffff;
	m_colorForeground = 0x00000000;
	m_brush.CreateSolidBrush(m_colorBackground);
}


CDialogAbout::~CDialogAbout() 
{
}



void CDialogAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogAbout)
	//}}AFX_DATA_MAP
}





BOOL CDialogAbout::OnInitDialog() 
{
	// Call base class
	CDialog::OnInitDialog();

	// Subclass static controls
	// can also do with icons!
//	m_linkWebsite.SubclassDlgItem(IDC_WEBSITE, this);
//	m_linkWebsite.m_strURL = theApp.m_strWebsite;
//	m_linkEmail.SubclassDlgItem(IDC_EMAIL, this);
//	m_linkEmail.m_strURL = _T("mailto:someone@somewhere.org"); //, use string resource

	// Set caption to "About <application name>"
	CString strCaption = _T("About ") + CString(theApp.m_pszAppName);
	SetWindowText(strCaption);

	// Get file version info
	GetVersionInfo();

	// Set bold font for some controls
	CWnd* pLabel;
	pLabel = GetDlgItem(IDC_PROGRAMNAME);
	if (pLabel) pLabel->SetFont(&theApp.m_fontControlsBold);
	pLabel = GetDlgItem(IDC_COMPANYNAME);
	if (pLabel) pLabel->SetFont(&theApp.m_fontControlsBold);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




//. see what happens if you change the language
// maybe it's better to just hardcode the labels for now
void CDialogAbout::GetVersionInfo()
{
	// Get name of the executable file
	// The GetModuleFileName function retrieves the full path and filename for the executable 
	// file containing the specified module. 
	// Windows 95: The GetModuleFilename function will return long filenames when an 
	// application's version number is greater than or equal to 4.00 and the long filename is 
	// available. Otherwise, it returns only 8.3 format filenames.
	TCHAR szExeName[MAX_PATH];
	if (!::GetModuleFileName(NULL, szExeName, MAX_PATH))
		return;

	// Open the NeoMem.exe file to get version info
	CFileVersion f;
	CFileException e;
	UINT nOpenFlags = CFile::modeRead | CFile::shareDenyNone;
	if (!f.Open(szExeName, nOpenFlags, &e))
	{
		e.ReportError();
		return;
	}

	// Walk through dialog controls, replacing text with version info data
	// E.g. a static control labelled "CompanyName" will be replaced with the value for the 
	// "CompanyName" variable.
	// handle ProductVersion differently
	CString strVar;
	CString strValue;
	for (int i = IDC_VERSION_FIRST; i <= IDC_VERSION_LAST; i++)
	{
		if (GetDlgItemText(i, strVar))
		{
			if (f.GetVersionValue(strVar, strValue))
			{
				SetDlgItemText(i, strValue);
			}
		}
	}

//	"CompanyName"
//	"FileDescription"
//	"FileVersion"
//	"InternalName"
//	"LegalCopyright"
//	"OriginalFilename"
//	"ProductName"
//	"ProductVersion"

}



void CDialogAbout::OnMouseMove(UINT nFlags, CPoint point) 
{
	// Invalidate so that hyperlinks will be redrawn with normal (unhover) color
//	Invalidate(FALSE);
	CDialog::OnMouseMove(nFlags, point);
}



void CDialogAbout::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// paint background rectangle
	
	CRect r;
	GetClientRect(&r);
	r.DeflateRect(10, 10, 10, 44);

	dc.FillSolidRect(&r, m_colorBackground);

	dc.Draw3dRect(&r, g_clr3dShadow, g_clr3dHighlight);

	// Do not call CDialog::OnPaint() for painting messages
}



