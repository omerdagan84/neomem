
// CDialogMessage



#include "precompiled.h"
#include "neomem.h"
#include "DialogMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDialogMessage::CDialogMessage(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMessage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMessage)
	//}}AFX_DATA_INIT
}


void CDialogMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMessage)
	DDX_Control(pDX, IDC_RTF, m_rtf);
	DDX_Control(pDX, IDC_PIC_HEADER, m_picHeader);
	DDX_Control(pDX, IDC_CHK_OPTION, m_chkOption);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BTN_A, m_btnA);
	//}}AFX_DATA_MAP
	
	if (pDX->m_bSaveAndValidate)
	{

	}
}


BEGIN_MESSAGE_MAP(CDialogMessage, CDialog)
	//{{AFX_MSG_MAP(CDialogMessage)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CDialogMessage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	m_btnClose.SetFocus();
	return FALSE;
}




void CDialogMessage::OnBtnClose() 
{
	CDialog::OnOK();	
}




BOOL CDialogMessage::DoModalParameters(CString strMessage)
{
	return TRUE;	
}
