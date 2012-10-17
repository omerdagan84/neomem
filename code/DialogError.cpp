
// CDialogError


#include "precompiled.h"
#include "NeoMem.h"
#include "DialogError.h"


#include "Email.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CDialogError, CDialog)
	//{{AFX_MSG_MAP(CDialogError)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_BN_CLICKED(IDC_BTN_COPY2, OnBtnCopy)
	ON_BN_CLICKED(IDC_BTN_CONTINUE, OnBtnContinue)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




CDialogError::CDialogError(CWnd* pParent /*=NULL*/)
		: CDialog(CDialogError::IDD, pParent),
	m_bEnableSendButton(TRUE)
{
	//{{AFX_DATA_INIT(CDialogError)
//	m_strErrorMessage = _T("");
	//}}AFX_DATA_INIT
}


void CDialogError::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogError)
	DDX_Control(pDX, IDC_LBL_TITLE, m_lblTitle);
	DDX_Control(pDX, IDC_DESCRIPTION, m_txtDescription);
	DDX_Control(pDX, IDC_TXT_ERROR, m_txtErrorMessage);
	DDX_Control(pDX, IDC_BTN_SEND, m_btnSend);
	DDX_Control(pDX, IDC_BTN_CONTINUE, m_btnContinue);
	DDX_Control(pDX, IDC_BTN_EXIT, m_btnExit);
	//}}AFX_DATA_MAP
}




BOOL CDialogError::OnInitDialog() 
{
	// Call base class
	CDialog::OnInitDialog();

	m_lblTitle.SetFont(&app.m_fontControlsBold);

	// Enable/disable send email button
	m_btnSend.EnableWindow(m_bEnableSendButton);

	// Display the error message in the textbox
	CString str = m_pe->GetDialogText();
	m_txtErrorMessage.SetWindowText(str);

	// Show the Continue button if we're in admin mode
//	m_btnContinue.ShowWindow(app.m_bAdmin ? SW_SHOW : SW_HIDE);

	// Return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
//	return	TRUE;  
	
	m_txtDescription.SetFocus();
	return FALSE;
}




// Send the error report via email
void CDialogError::OnBtnSend() 
{
	CWaitCursor wc;

	UpdateDescription();

	CString strSubject = m_pe->GetReportSubject();
	CString strMessage = m_pe->GetReportText();

	// create and send the email
	CEmail email("NeoMem Support", "errors@neomem.org", strSubject, strMessage);
	email.Send();
}



void CDialogError::OnBtnContinue() 
{
	CDialog::EndDialog(TRUE);
}


void CDialogError::OnBtnExit() 
{
	CDialog::EndDialog(FALSE);
}



void CDialogError::OnClose() 
{	
	CDialog::OnClose();
}




// copy error to clipboard
void CDialogError::OnBtnCopy() 
{
	CWaitCursor wc;
	UpdateDescription();
	CString str = m_pe->GetReportText();
	app.CopyToClipboard(str);	
}



void CDialogError::UpdateDescription()
{
	CString str;
	m_txtDescription.GetWindowText(str);
	m_pe->SetDescription(str);
}
