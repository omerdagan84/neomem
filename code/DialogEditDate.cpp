// CDialogEditDate



#include "precompiled.h"
#include "DialogEditDate.h"

#include "NeoMem.h"
#include "HelpIDs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CDialogEditDate, CDialog)
	//{{AFX_MSG_MAP(CDialogEditDate)
	ON_BN_CLICKED(IDC_CHK_TIME, OnChkTime)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDialogEditDate::CDialogEditDate(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEditDate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEditDate)
	m_dtDate = COleDateTime::GetCurrentTime();
	m_dtTime = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_bUseTime = FALSE;
}



BOOL CDialogEditDate::OnInitDialog() 
{
	// Call base class
	CDialog::OnInitDialog();
	
	// Enable or disable time entry based on check box state
	OnChkTime();

	COLORREF crTitleBackground = 0x0050f088;
	m_wndDate.SetColor(MCSC_TITLEBK, crTitleBackground);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogEditDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditDate)
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_TIME, m_wndTime);
	DDX_Control(pDX, IDC_DATE, m_wndDate);
	DDX_Control(pDX, IDC_CHK_TIME, m_chkTime);
	DDX_MonthCalCtrl(pDX, IDC_DATE, m_dtDate);
	DDX_DateTimeCtrl(pDX, IDC_TIME, m_dtTime);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) // Load
	{
		// If time is defined then check off the time box
//		if (m_dtTime.GetHour() == 0 && m_dtTime.GetMinute() == 0 && m_dtTime.GetSecond() == 0)
//			m_chkTime.SetCheck(0);
//		else
//			m_chkTime.SetCheck(1);

		if (m_bUseTime)
			m_chkTime.SetCheck(1);
		else
			m_chkTime.SetCheck(0);
	}
	else // Save
	{
		// Workaround for MFC bug in CMonthCalCtr::GetCurSel() (doesn't set time right so
		// m_dtDate becomes invalid)
		SYSTEMTIME sysTime;
		m_wndDate.GetCurSel(&sysTime);
		sysTime.wHour = sysTime.wMinute = sysTime.wSecond = sysTime.wMilliseconds = 0;
		m_dtDate = COleDateTime(sysTime);
		m_bUseTime = m_chkTime.GetCheck();
		//, Clear time if not checked
//		if (!m_bUseTime)
//		{
//			m_dtTime.SetTime(0, 0, 0);
//		}
	}
}


void CDialogEditDate::OnChkTime() 
{
	// Enable time window if check box checked
	BOOL bEnable = m_chkTime.GetCheck();
	m_wndTime.EnableWindow(bEnable);
}


void CDialogEditDate::OnBtnHelp() 
{
	app.WinHelp(HID_BASE_RESOURCE + IDD_EDIT_DATE);
}


