// DialogMessage2.cpp : implementation file
//

#include "precompiled.h"
#include "neomem.h"
#include "DialogMessage2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMessage2 dialog


CDialogMessage2::CDialogMessage2(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMessage2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMessage2)
	m_bShowMessage = FALSE;
	m_strMessage = _T("");
	//}}AFX_DATA_INIT
}


void CDialogMessage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMessage2)
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_LBL_MESSAGE, m_lblMessage);
	DDX_Control(pDX, IDC_CHK_OPTION, m_chkOption);
	DDX_Check(pDX, IDC_CHK_OPTION, m_bShowMessage);
	DDX_Text(pDX, IDC_LBL_MESSAGE, m_strMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMessage2, CDialog)
	//{{AFX_MSG_MAP(CDialogMessage2)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_CHK_OPTION, OnChkOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMessage2 message handlers

BOOL CDialogMessage2::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!m_strCaption.IsEmpty())
		SetWindowText(m_strCaption);

//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	m_btnClose.SetFocus();
	return FALSE;
}



void CDialogMessage2::OnChkOption() 
{
	m_btnClose.SetFocus();
}

void CDialogMessage2::OnBtnClose() 
{
	CDialog::OnOK();
}


