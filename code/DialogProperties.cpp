
// CDialogProperties



#include "precompiled.h"
#include "NeoMem.h"
#include "DialogProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




BEGIN_MESSAGE_MAP(CDialogProperties, CDialog)
	//{{AFX_MSG_MAP(CDialogProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDialogProperties::CDialogProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogProperties)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



BOOL CDialogProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialogProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogProperties)
	DDX_Control(pDX, IDC_LVW, m_lvw);
	//}}AFX_DATA_MAP
}



