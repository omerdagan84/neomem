
// CDialogEx



#include "precompiled.h"

#include "neomem.h"
#include "DialogEx.h"

#include "BDoc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CDialogEx, CDialog)
	//{{AFX_MSG_MAP(CDialogEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//CDialogEx::CDialogEx(CWnd* pParent /*=NULL*/)
//	: CDialog(CDialogEx::IDD, pParent)
CDialogEx::CDialogEx(UINT nTemplateID, CWnd* pParent /* = NULL*/)
	: CDialog(nTemplateID, pParent)
{
	// Get document (used by domodal methods also)
	m_pDoc = BDoc::GetDoc();
	ASSERT_VALID(m_pDoc);

	//{{AFX_DATA_INIT(CDialogEx)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



BOOL CDialogEx::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEx)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
