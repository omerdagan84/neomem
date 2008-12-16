
// CDialogEditRichText



#include "precompiled.h"
#include "NeoMem.h"
#include "DialogEditRichText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDialogEditRichText::CDialogEditRichText(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEditRichText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEditRichText)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogEditRichText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditRichText)
	DDX_Control(pDX, IDC_RTF, m_rtf);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEditRichText, CDialog)
	//{{AFX_MSG_MAP(CDialogEditRichText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()





BOOL CDialogEditRichText::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}





