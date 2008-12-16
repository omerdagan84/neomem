

// CDialogSaveOptions



#include "precompiled.h"
#include "neomem.h"
#include "DialogSaveOptions.h"


#include "DialogEditName.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




CDialogSaveOptions::CDialogSaveOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSaveOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSaveOptions)
	m_strPassword = _T("");
	m_nEncryptionType = -1;
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CDialogSaveOptions, CDialog)
	//{{AFX_MSG_MAP(CDialogSaveOptions)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	ON_CBN_CLOSEUP(IDC_CBO_ENCRYPTION, OnCloseupCboEncryption)
	ON_CBN_SELCHANGE(IDC_CBO_ENCRYPTION, OnSelchangeCboEncryption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSaveOptions message handlers



BOOL CDialogSaveOptions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Get text for combo box
	// RSA RC4 40-bit Encryption
//	m_cboEncryption.AddItem

	// Copy password to confirmation box
	CString s;
	m_txtPassword.GetWindowText(s);
	m_txtPassword2.SetWindowText(s);

	OnCloseupCboEncryption();
	m_cboEncryption.SetFocus();

//	return TRUE;  // return TRUE unless you set the focus to a control
//	              // EXCEPTION: OCX Property Pages should return FALSE
	return FALSE;
}



void CDialogSaveOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSaveOptions)
	DDX_Control(pDX, IDC_LBL_PASSWORD2, m_lblPassword2);
	DDX_Control(pDX, IDC_TXT_PASSWORD2, m_txtPassword2);
	DDX_Control(pDX, IDC_CBO_ENCRYPTION, m_cboEncryption);
	DDX_Control(pDX, IDC_TXT_PASSWORD, m_txtPassword);
	DDX_Control(pDX, IDC_LBL_PASSWORD, m_lblPassword);
	DDX_Text(pDX, IDC_TXT_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 16);
	DDX_CBIndex(pDX, IDC_CBO_ENCRYPTION, m_nEncryptionType);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) // Save
	{
		if (!Validate())
//			pDX->Fail(); // will throw an exception
			AfxThrowUserException();
	}
}


/*
void CDialogSaveOptions::OnChkEncrypt() 
{
	BOOL bEncrypt = m_chkEncrypt.GetCheck();
	m_lblPassword.EnableWindow(bEncrypt);
	m_txtPassword.EnableWindow(bEncrypt);
	if (bEncrypt && m_txtPassword.IsWindowVisible())
	{
		m_txtPassword.SetFocus();
	}
}
*/


void CDialogSaveOptions::OnBtnHelp() 
{
	theApp.WinHelp(HID_BASE_RESOURCE + IDD_FILE_SECURITY); 
}



void CDialogSaveOptions::OnCloseupCboEncryption() 
{
	m_nEncryptionType = m_cboEncryption.GetCurSel();
	BOOL bEncrypt = (m_nEncryptionType > 0);
	m_lblPassword.EnableWindow(bEncrypt);
	m_txtPassword.EnableWindow(bEncrypt);
	m_lblPassword2.EnableWindow(bEncrypt);
	m_txtPassword2.EnableWindow(bEncrypt);
	if (bEncrypt && m_txtPassword.IsWindowEnabled())
	{
		m_txtPassword.SetFocus();
	}
}

// Bug: have to handle this event in addition to closeup because only this one 
// gets fired if you just use down arrow to change selection!
void CDialogSaveOptions::OnSelchangeCboEncryption() 
{
	m_nEncryptionType = m_cboEncryption.GetCurSel();
	BOOL bEncrypt = (m_nEncryptionType > 0);
	m_lblPassword.EnableWindow(bEncrypt);
	m_txtPassword.EnableWindow(bEncrypt);
	m_lblPassword2.EnableWindow(bEncrypt);
	m_txtPassword2.EnableWindow(bEncrypt);
}




BOOL CDialogSaveOptions::Validate()
{
	// If encrypting file make sure there's a password
	if (m_nEncryptionType > 0)
	{
		m_txtPassword.GetWindowText(m_strPassword);
		m_strPassword.TrimLeft();
		m_strPassword.TrimRight();
		if (m_strPassword.IsEmpty())
		{
			AfxMessageBox(_T("Please enter a password of up to 16 characters."), MB_ICONINFORMATION);
			m_txtPassword.SetFocus();
			return FALSE;
		}
		if (!m_strPassword.IsAscii())
		{
			AfxMessageBox(_T("Please use only standard ASCII characters in the password (i.e. A-Z, a-z, 0-9, !@#$%^&* etc.). This will ensure that the password will work for future releases of NeoMem."), MB_ICONINFORMATION);
			m_txtPassword.SetFocus();
			return FALSE;
		}

		// Check confirmation password
		CString s;
		m_txtPassword2.GetWindowText(s);
		s.TrimLeft();
		s.TrimRight();
		if (m_strPassword != s)
		{
			AfxMessageBox(_T("Passwords do not match - please try again."), MB_ICONINFORMATION);
			m_txtPassword.SetFocus();
			return FALSE;
		}
	}

	return TRUE;
}



void CDialogSaveOptions::OnOK() 
{
/*
	// Confirm password
	m_nEncryptionType = m_cboEncryption.GetCurSel();
	if (m_nEncryptionType > 0)
	{
		m_txtPassword.GetWindowText(m_strPassword);
		CDialogEditName dlg;
		dlg.m_strCaption = "Enter Password";
		dlg.m_strInstructions = "Please re-enter the password for verification:";
		dlg.m_bPassword = TRUE;
		if (dlg.DoModal())
		{
			if (m_strPassword != dlg.m_strName)
			{
				AfxMessageBox("Incorrect password!", MB_ICONINFORMATION);
				m_txtPassword.SetFocus();
				return;
			}
		}
	}
*/	
	CDialog::OnOK();
}


