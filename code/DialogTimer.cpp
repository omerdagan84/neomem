
// CDialogTimer



#include "precompiled.h"
#include "neomem.h"
#include "DialogTimer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




BEGIN_MESSAGE_MAP(CDialogTimer, CDialog)
	//{{AFX_MSG_MAP(CDialogTimer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CDialogTimer::CDialogTimer(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTimer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTimer)
	m_nHours = 0;
	m_nMin = 0;
	m_nSec = 0;
	//}}AFX_DATA_INIT
}


BOOL CDialogTimer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_slideHours.SetRange(0, 24);

	m_spinHours.SetRange32(0, 1000);
	m_spinMin.SetRange32(0, 1000);
	m_spinSec.SetRange32(0, 1000);

//	m_wndTime

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void CDialogTimer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTimer)
	DDX_Control(pDX, IDC_SLIDE_HOURS, m_slideHours);
	DDX_Control(pDX, IDC_TXT_HOURS, m_txtHours);
	DDX_Control(pDX, IDC_SPIN_HOURS, m_spinHours);
	DDX_Control(pDX, IDC_TXT_SEC, m_txtSec);
	DDX_Control(pDX, IDC_TXT_MIN, m_txtMin);
	DDX_Control(pDX, IDC_SPIN_SEC, m_spinSec);
	DDX_Control(pDX, IDC_SPIN_MIN, m_spinMin);
	DDX_Text(pDX, IDC_TXT_HOURS, m_nHours);
	DDX_Text(pDX, IDC_TXT_MIN, m_nMin);
	DDX_Text(pDX, IDC_TXT_SEC, m_nSec);
	//}}AFX_DATA_MAP

/*
	if (pDX->m_bSaveAndValidate)
	{
		CString str;
		m_txtMin.GetWindowText(str);
		m_nMin = atol(str);
		m_txtSec.GetWindowText(str);
		m_nSec = atol(str);
	}
	else
	{
		CString str;
		str.Format("%u", m_nHours);
		m_txtMin.SetWindowText(str);
		str.Format("%u", m_nMin);
		m_txtMin.SetWindowText(str);
		str.Format("%u", m_nSec);
		m_txtSec.SetWindowText(str);
	}
*/
}


