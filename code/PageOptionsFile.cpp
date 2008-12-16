
// CPageOptionsFile


#include "precompiled.h"
#include "NeoMem.h"
#include "PageOptionsFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//. need to ignore enter key and pass it to the parent somehow


IMPLEMENT_DYNCREATE(CPageOptionsFile, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageOptionsFile, CPropertyPage)
	//{{AFX_MSG_MAP(CPageOptionsFile)
	ON_EN_KILLFOCUS(IDC_TXT_FOLDER, OnKillFocusTxtFolder)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_CHK_STARTUP, OnChkStartup)
	ON_BN_CLICKED(IDC_CHK_AUTORECOVER, OnChkAutoRecover)
	ON_BN_CLICKED(IDC_CHK_AUTOBACKUP, OnChkAutoBackup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//	ON_BN_CLICKED(IDC_BTN_BROWSE_STARTUP, OnBtnBrowseStartup)
//	ON_EN_KILLFOCUS(IDC_TXT_STARTUP, OnKillfocusTxtStartup)
//	ON_BN_CLICKED(IDC_BTN_USE_CURRENT, OnBtnUseCurrent)



CPageOptionsFile::CPageOptionsFile() : CPropertyPage(CPageOptionsFile::IDD)
{
	//{{AFX_DATA_INIT(CPageOptionsFile)
	m_bAutoLoad = FALSE;
	m_strDocumentFolder = _T("");
//	m_strAutoLoadFileName = _T("");
	m_bAutoStart = FALSE;
	m_bAutoBackup = FALSE;
	m_bAutoRecover = FALSE;
	m_nAutoRecoverMinutes = 5;
	//}}AFX_DATA_INIT
}

CPageOptionsFile::~CPageOptionsFile()
{
}



BOOL CPageOptionsFile::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// Make headings bold
	m_lblFolder.SetFont(&theApp.m_fontControlsBold);
	m_lblStartup.SetFont(&theApp.m_fontControlsBold);
	m_lblAutoStart.SetFont(&theApp.m_fontControlsBold);
	m_lblAutoRecover.SetFont(&theApp.m_fontControlsBold);
	m_lblAutoBackup.SetFont(&theApp.m_fontControlsBold);

	// Set spin control range
	m_spinAutoRecover.SetRange(1, 60);

	// Enable/disable controls
	OnChkStartup();
	OnChkAutoRecover();

//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	m_txtFolder.SetFocus();
	return FALSE;
}



void CPageOptionsFile::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageOptionsFile)
	DDX_Control(pDX, IDC_TXT_AUTORECOVER, m_txtAutoRecover);
	DDX_Control(pDX, IDC_SPIN_AUTORECOVER, m_spinAutoRecover);
	DDX_Control(pDX, IDC_LBL_AUTORECOVER2, m_lblAutoRecover2);
	DDX_Control(pDX, IDC_LBL_AUTORECOVER, m_lblAutoRecover);
	DDX_Control(pDX, IDC_LBL_AUTOBACKUP, m_lblAutoBackup);
	DDX_Control(pDX, IDC_CHK_AUTORECOVER, m_chkAutoRecover);
	DDX_Control(pDX, IDC_CHK_AUTOBACKUP, m_chkAutoBackup);
	DDX_Control(pDX, IDC_CBO_AUTOBACKUP, m_cboAutoBackup);
	DDX_Control(pDX, IDC_BTN_USE_CURRENT, m_btnUseCurrent);
	DDX_Control(pDX, IDC_LBL_AUTOSTART, m_lblAutoStart);
	DDX_Control(pDX, IDC_CHK_AUTOSTART, m_chkAutoStart);
	DDX_Control(pDX, IDC_LBL_STARTUP, m_lblStartup);
	DDX_Control(pDX, IDC_LBL_FOLDER, m_lblFolder);
	DDX_Control(pDX, IDC_TXT_FOLDER, m_txtFolder);
	DDX_Control(pDX, IDC_CHK_STARTUP, m_chkStartup);
	DDX_Control(pDX, IDC_BTN_BROWSE, m_btnBrowseFolder);
	DDX_Check(pDX, IDC_CHK_STARTUP, m_bAutoLoad);
	DDX_Text(pDX, IDC_TXT_FOLDER, m_strDocumentFolder);
//	DDX_Control(pDX, IDC_BTN_BROWSE_STARTUP, m_btnBrowseStartup);
//	DDX_Control(pDX, IDC_TXT_STARTUP, m_txtAutoLoadFileName);
//	DDX_Text(pDX, IDC_TXT_STARTUP, m_strAutoLoadFileName);
	DDX_Check(pDX, IDC_CHK_AUTOSTART, m_bAutoStart);
	DDX_Check(pDX, IDC_CHK_AUTOBACKUP, m_bAutoBackup);
	DDX_Check(pDX, IDC_CHK_AUTORECOVER, m_bAutoRecover);
	DDX_Text(pDX, IDC_TXT_AUTORECOVER, m_nAutoRecoverMinutes);
	DDV_MinMaxUInt(pDX, m_nAutoRecoverMinutes, 1, 60);
	//}}AFX_DATA_MAP
}



BOOL CPageOptionsFile::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}


// Document Folder
//-------------------------------------------------------------

void CPageOptionsFile::OnBtnBrowse() 
{
	CWaitCursor wc;
	CString strFolder;
	m_txtFolder.GetWindowText(strFolder);
	if (theApp.BrowseFolder(_T("Select the Document Folder"), strFolder))
	{
		// Set current directory here so that browse for startup file will use this folder.
		// Note - this is also set after dialog is finished.
		::SetCurrentDirectory(strFolder);
		m_txtFolder.SetWindowText(strFolder);
	}	
}


void CPageOptionsFile::OnKillFocusTxtFolder() 
{
/*
	// Verify folder exists
	CFile f;
	CString str; 
	m_txtFolder.GetWindowText(str);
	CFileStatus status;
	if (!CFile::GetStatus(str, status))
	{
		AfxMessageBox("Invalid folder - please try again.");
		m_txtFolder.SetFocus();
	}	
*/
}




// Startup File
//-------------------------------------------------------------

void CPageOptionsFile::OnChkStartup() 
{
//	m_bAutoLoad = m_chkStartup.GetCheck();
//	m_txtAutoLoadFileName.EnableWindow(m_bAutoLoad);
//	m_btnBrowseStartup.EnableWindow(m_bAutoLoad);
//	BOOL bCurrentFile = (!m_strCurrentFile.IsEmpty()); // true if there is a current file
//	m_btnUseCurrent.EnableWindow(m_bAutoLoad && bCurrentFile);
}


void CPageOptionsFile::OnBtnBrowseStartup() 
{
/*
	CWaitCursor wc;

	// Get default extension from string resource
	CString strDefaultExtension = theApp.m_strFileExtension.Mid(1); // remove period

//	CFileDialogEx dlg(TRUE, _T("neo"), m_strAutoLoadFileName, 
	CFileDialogEx dlg(TRUE, strDefaultExtension, m_strAutoLoadFileName, 
							OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, 
							szNeoMemFilter, this->GetParent());
	if (dlg.DoModal() == IDOK)
	{
		m_strAutoLoadFileName = dlg.GetPathName();
		m_txtAutoLoadFileName.SetWindowText(m_strAutoLoadFileName);
	}	
*/
}


void CPageOptionsFile::OnKillfocusTxtStartup() 
{
/*
	// Verify startup file
	CFile f;
	CString str;
	m_txtAutoLoadFileName.GetWindowText(str);
	CFileStatus status;
	if (!CFile::GetStatus(str, status))
	{
		AfxMessageBox("Invalid file - please try again.");
		m_txtAutoLoadFileName.SetFocus();
	}		
*/
}




void CPageOptionsFile::OnBtnUseCurrent() 
{
	// Fill in name of current file, if any
//	m_txtAutoLoadFileName.SetWindowText(m_strCurrentFile);
}



BOOL CPageOptionsFile::OnApply() 
{
	if (!Validate())
		return FALSE;
	return CPropertyPage::OnApply();
}


BOOL CPageOptionsFile::OnKillActive() 
{
	if (!Validate())
		return FALSE;
	return CPropertyPage::OnKillActive();
}


BOOL CPageOptionsFile::Validate()
{
	// Check fields

	// Verify folder exists
	CString str; 
	m_txtFolder.GetWindowText(str);
	if (!::SetCurrentDirectory(str))
	{
		AfxMessageBox("Invalid default folder - please try again.");
		
		m_txtFolder.SetFocus();
		return FALSE;
	}

	return TRUE;
}




void CPageOptionsFile::OnChkAutoRecover() 
{
	m_bAutoRecover = m_chkAutoRecover.GetCheck();
	m_txtAutoRecover.EnableWindow(m_bAutoRecover);
	m_spinAutoRecover.EnableWindow(m_bAutoRecover);
}


void CPageOptionsFile::OnChkAutoBackup() 
{
//	m_bAutoBackup = m_chkAutoBackup.GetCheck();
}


