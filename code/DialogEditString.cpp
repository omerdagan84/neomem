
// CDialogEditString




#include "precompiled.h"
#include "NeoMem.h"
#include "DialogEditString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDialogEditString::CDialogEditString(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEditString::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEditString)
	m_strValue = _T("");
	//}}AFX_DATA_INIT
}


void CDialogEditString::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditString)
	DDX_Control(pDX, IDC_TXT_VALUE, m_txtValue);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	DDX_Text(pDX, IDC_TXT_VALUE, m_strValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEditString, CDialog)
	//{{AFX_MSG_MAP(CDialogEditString)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CDialogEditString::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Let control get returns
	m_txtValue.m_bWantReturn = TRUE;

	if (!m_strCaption.IsEmpty())
		SetWindowText(m_strCaption);
	
	if (!m_strInstructions.IsEmpty())
		m_lblInstructions.SetWindowText(m_strInstructions);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}












