
// CDialogEditPersonName




#include "precompiled.h"
#include "NeoMem.h"
#include "DialogEditPersonName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CDialogEditPersonName, CDialog)
	//{{AFX_MSG_MAP(CDialogEditPersonName)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CDialogEditPersonName::CDialogEditPersonName(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEditPersonName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEditPersonName)
	m_strTitle = _T("");
	m_strFirst = _T("");
	m_strLast = _T("");
	m_strMiddle = _T("");
	m_strNickname = _T("");
	m_strSuffix = _T("");
	//}}AFX_DATA_INIT
}


void CDialogEditPersonName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditPersonName)
	DDX_Control(pDX, IDC_CBO_TITLE, m_cboTitle);
	DDX_Control(pDX, IDC_CBO_SUFFIX, m_cboSuffix);
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelp);
	DDX_Text(pDX, IDC_TXT_FIRST, m_strFirst);
	DDV_MaxChars(pDX, m_strFirst, 80);
	DDX_Text(pDX, IDC_TXT_LAST, m_strLast);
	DDV_MaxChars(pDX, m_strLast, 80);
	DDX_Text(pDX, IDC_TXT_MIDDLE, m_strMiddle);
	DDV_MaxChars(pDX, m_strMiddle, 80);
	DDX_Text(pDX, IDC_TXT_NICKNAME, m_strNickname);
	DDV_MaxChars(pDX, m_strNickname, 80);
	DDX_CBString(pDX, IDC_CBO_SUFFIX, m_strSuffix);
	DDV_MaxChars(pDX, m_strSuffix, 80);
	DDX_CBString(pDX, IDC_CBO_TITLE, m_strTitle);
	DDV_MaxChars(pDX, m_strTitle, 80);
	//}}AFX_DATA_MAP
}



void CDialogEditPersonName::OnBtnHelp() 
{
	app.WinHelp(HID_BASE_RESOURCE + IDD_EDIT_PERSON_NAME);
}
