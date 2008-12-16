
// CDialogNotice


#include "precompiled.h"
#include "neomem.h"
#include "DialogNotice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDialogNotice::CDialogNotice(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogNotice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogNotice)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogNotice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogNotice)
	DDX_Control(pDX, IDC_BTN_DOWNLOAD, m_btnDownload);
	DDX_Control(pDX, IDC_TXT, m_txt);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogNotice, CDialog)
	//{{AFX_MSG_MAP(CDialogNotice)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD, OnBtnDownload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CDialogNotice::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_txt.m_bReadOnly = TRUE; // make readonly
	m_txt.SetWindowText(m_strMessage);
	
//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	m_btnDownload.SetFocus();
	return FALSE;
}



void CDialogNotice::OnBtnClose() 
{
	CDialog::EndDialog(0);	
//	CDialog::OnOK();
}



void CDialogNotice::OnBtnDownload() 
{
//	if (IDYES == AfxMessageBox("This will attempt to open the NeoMem website. Continue?", MB_ICONQUESTION + MB_YESNO))
//	if (IDOK == AfxMessageBox("From the download This will attempt to open the NeoMem website. Continue?", MB_ICONQUESTION + MB_YESNO))
//	{
//		CString strURL;
//		strURL.Format("http://%s", (LPCTSTR) theApp.m_strWebsite); // www.neomem.org
		LPCTSTR pszURL = "http://www.microsoft.com/msdownload/ieplatform/ie/comctrlx86.asp";
		CWaitCursor wc;
		HINSTANCE h = ::ShellExecute(NULL, "open", pszURL, NULL, NULL, SW_SHOWNORMAL);	
		HandleShellExecuteError(h);
//	}
}



