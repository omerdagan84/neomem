// DialogPassword.cpp : implementation file
//

#include "precompiled.h"
#include "neomem.h"
#include "DialogPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPassword dialog


CDialogPassword::CDialogPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPassword)
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
}




BEGIN_MESSAGE_MAP(CDialogPassword, CDialog)
	//{{AFX_MSG_MAP(CDialogPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPassword message handlers

BOOL CDialogPassword::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	m_lblFilename.SetWindowText(m_strFilename);
	m_txtFilename.SetWindowText(m_strFilename);
	m_txtPassword.SetFocus();
	
//	return TRUE;  // return TRUE unless you set the focus to a control
//	              // EXCEPTION: OCX Property Pages should return FALSE
	return FALSE;

}




void CDialogPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPassword)
	DDX_Control(pDX, IDC_TXT_FILE, m_txtFilename);
	DDX_Control(pDX, IDC_LBL_FILE, m_lblFilename);
	DDX_Control(pDX, IDC_TXT_PASSWORD, m_txtPassword);
	DDX_Text(pDX, IDC_TXT_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 16);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) // Save
	{
		if (!Validate())
			pDX->Fail(); // will throw an exception
	}
	

}



BOOL CDialogPassword::Validate()
{
	// Verify password is correct
	m_txtPassword.GetWindowText(m_strPassword);
	if (m_strValidPassword != m_strPassword)
	{
		AfxMessageBox(_T("Invalid password entered."), MB_ICONINFORMATION);
		m_txtPassword.SetFocus();
		return FALSE;
	}

	return TRUE;
}




