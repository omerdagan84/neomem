
// CDialogUpgradeFile.cpp



#include "precompiled.h"
#include "neomem.h"
#include "DialogUpgradeFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




CDialogUpgradeFile::CDialogUpgradeFile(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogUpgradeFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogUpgradeFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogUpgradeFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogUpgradeFile)
	DDX_Control(pDX, IDC_RTF, m_rtf);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogUpgradeFile, CDialog)
	//{{AFX_MSG_MAP(CDialogUpgradeFile)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()





BOOL CDialogUpgradeFile::OnInitDialog() 
{
	// Call base class
	CDialog::OnInitDialog();

	// vc: You can't put this in a string resource because VC++ has 256 char limit, though Win32 has 4k limit.
	CString strText;
	strText.Format(
					"{\\rtf1\\ansi\\deff0\\deftab720"
					"{\\fonttbl{\\f0\\fswiss Arial;}}"
					"{\\colortbl\\red0\\green0\\blue0;\\red0\\green128\\blue0;"
					"\\red128\\green128\\blue0;\\red192\\green192\\blue192;}"
					"\\deflang1033\\pard\\plain \\li60 \\f0\\fs24\\cf1\\b " // li = indent left!
					"Upgrade File Structure"
					"\\plain\\f0\\fs16\\cf0 "
					"\\par \\par "
					"The file '%s' needs to be upgraded to the newer file structure and data model. "
					"NeoMem will make a backup copy of the current file, then upgrade the file so it can be used with the current version of NeoMem. \\par"
					"\\par "
					"Click OK to continue, or Cancel if you do not want to upgrade the file. \\par"
					"}",
					(LPCTSTR) m_strFile
					);

	// Add text to control
	m_rtf.SetRtf((LPCTSTR) strText);

	// Reduce the formatting rect of the rich edit control
	//, why doesn't this work?
//	CRect r;
//	m_rtf.GetRect(&r);
//	r.DeflateRect(8, 4, 8, 4);
//	m_rtf.SetRect(&r);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialogUpgradeFile::OnBtnHelp() 
{
	app.WinHelp(HID_BASE_RESOURCE + IDD_UPGRADE_FILE); 
}


