
// CDialogEditName



#include "precompiled.h"
#include "NeoMem.h"
#include "DialogEditName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CDialogEditName::CDialogEditName(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEditName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEditName)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_bPassword = FALSE;
}


void CDialogEditName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditName)
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	DDX_Control(pDX, IDC_TXT_VALUE, m_txtName);
	DDX_Text(pDX, IDC_TXT_VALUE, m_strName);
	//}}AFX_DATA_MAP

/*
	if (pDX->m_bSaveAndValidate)
	{
		if (m_bPassword)
		{
			// Verify that password contains only ASCII characters (ie < 0x80)!
			if (!m_strName.IsAscii())
			{
				AfxMessageBox("
			}
		}
	}
*/
}


BEGIN_MESSAGE_MAP(CDialogEditName, CDialog)
	//{{AFX_MSG_MAP(CDialogEditName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CDialogEditName::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (!m_strCaption.IsEmpty())
		SetWindowText(m_strCaption);
	
	if (!m_strInstructions.IsEmpty())
		m_lblInstructions.SetWindowText(m_strInstructions);

	if (m_bPassword)
		m_txtName.SetPasswordChar('*');
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




