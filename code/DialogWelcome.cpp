
// CDialogWelcome



#include "precompiled.h"
#include "NeoMem.h"
#include "DialogWelcome.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CDialogWelcome, CDialog)
	//{{AFX_MSG_MAP(CDialogWelcome)
	ON_BN_CLICKED(IDC_BTN_EXAMPLE, OnBtnExample)
	ON_BN_CLICKED(IDC_BTN_EXISTING_FILE, OnBtnExistingFile)
	ON_BN_CLICKED(IDC_BTN_NEW_FILE, OnBtnNewFile)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDB_BTN_CLOSE, OnBtnClose)
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(IDB_BTN_HELP, OnBtnHelp)
//	ON_MESSAGE(WM_DDE_EXECUTE, OnDDEExecute)
END_MESSAGE_MAP()


CDialogWelcome::CDialogWelcome(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWelcome::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogWelcome)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}




BOOL CDialogWelcome::OnInitDialog() 
{
	// Call base class
	CDialog::OnInitDialog();

	// Note: You can't put this in a string resource because VC++ has 256 char limit, though Win32 has 4k limit.
	CString strText;
	strText =  "{\\rtf1\\ansi\\deff0\\deftab720\\deflang1033"
					"{\\fonttbl{\\f0\\fswiss MS Sans Serif;}"
					"{\\f1\\froman\\fcharset2 Symbol;}{\\f2\\fswiss\\fprq2 Arial;}"
				    "{\\f3\\fswiss\\fcharset1 Arial;}{\\f4\\fswiss\\fprq2 System;}}"
					"{\\colortbl\\red0\\green0\\blue0;\\red0\\green128\\blue0;\\red128\\green128\\blue0;\\red192\\green192\\blue192;}"
					"\\pard\\plain \\li60 \\f2\\fs24\\cf1\\b " // li = indent left!
					"Welcome to NeoMem"
					"\\plain\\f2\\fs16\\cf0 "
					"\\par \\par "
					"If you are just getting started you might want to take a look at the example file - "
					"it includes several objects and classes already set up, and a tutorial on various "
					"aspects of the program. To open it, click on the {\\b Open Example File} button below. \\par "
					"\\par "
					"To create a new NeoMem file, click on the {\\b Create New File} button below. "
					"To open an existing NeoMem file, click on the {\\b Open Existing File} button below. \\par "
					"\\par "
					"If you no longer want to see this screen when there are no files open, turn off the check box below. \\par "
					"}";

	// Add text to control
	m_rtf.SetRtf((LPCTSTR) strText);

	// Set checkbox
	m_chkShowDialog.SetCheck(app.m_bShowWelcome ? 1 : 0);

//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	m_btnExample.SetFocus();
	return FALSE;
}



void CDialogWelcome::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWelcome)
	DDX_Control(pDX, IDC_PIC_HEADER, m_picHeader);
	DDX_Control(pDX, IDC_BTN_NEW_FILE, m_btnNewFile); 
	DDX_Control(pDX, IDC_BTN_EXISTING_FILE, m_btnExistingFile);
	DDX_Control(pDX, IDC_BTN_EXAMPLE, m_btnExample);
	DDX_Control(pDX, IDC_RTF, m_rtf);
	DDX_Control(pDX, IDC_CHK_SHOW, m_chkShowDialog);
	//}}AFX_DATA_MAP

	// Save checkbox setting
	if (pDX->m_bSaveAndValidate)
	{
		app.m_bShowWelcome = (m_chkShowDialog.GetCheck() == 1);
	}
}



/*
// this window doesn't seem to get the dde message! so where does it go?
LRESULT CDialogWelcome::OnDDEExecute(WPARAM wParam, LPARAM lParam)
{
//	AfxMessageBox("CDialogWelcome::OnDDEExecute - passing message on to main frame window!");
	// Pass the message on to the main frame window, where it belongs
	return AfxGetMainWnd()->SendMessage(WM_DDE_EXECUTE, wParam, lParam);
}
*/

/*
BOOL CDialogWelcome::PreTranslateMessage(MSG* pMsg) 
{
//	if (pMsg->message == WM_DDE_EXECUTE)
//	{
//		AfxMessageBox("aha"); // no dice
//	}
	return CDialog::PreTranslateMessage(pMsg);
}
*/



void CDialogWelcome::OnBtnExample() 
{
	CWaitCursor cw;
	// Open the example file
	CString strFile;
	strFile.LoadString(IDS_EXAMPLE_FILE); // "Example.neo"
	CString strFilePath = app.m_strApplicationFolder + _T("\\") + strFile; 
	if (app.OpenDocumentFile(strFilePath))
	{
		// Close the dialog
		CDialog::OnOK();
	}
}



// Open an existing file with the file open dialog.
void CDialogWelcome::OnBtnExistingFile() 
{
	CWaitCursor cw;
	if (app.m_pMainWnd->SendMessage(WM_COMMAND, ID_FILE_OPEN, 0))
	{
		// Close this welcome dialog if the user actually opened a file
		if (app.GetDocCount() > 0)
		{
			CDialog::OnOK();
		}
	}
}


// Create a new file
void CDialogWelcome::OnBtnNewFile() 
{
	CWaitCursor cw;
//	app.OnFileNew(); // protected
	if (app.m_pMainWnd->SendMessage(WM_COMMAND, ID_FILE_NEW, 0))
	{
		// Close this welcome dialog
		CDialog::OnOK();
	}
}





void CDialogWelcome::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here	
	// Do not call CDialog::OnPaint() for painting messages
}



/*
void CDialogWelcome::OnBtnHelp() 
{
//	app.WinHelp(HID_BASE_RESOURCE + IDR_NEOMEM_TYPE); // shows default topic (overview)	
}
*/


void CDialogWelcome::OnBtnClose() 
{
	// bug: assumed that enddialog would save the dialog controls, but it doesn't!
//	CDialog::EndDialog(0);	
	CDialog::OnOK();
}

