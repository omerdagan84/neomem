
// CPageClassName


#include "precompiled.h"

#include "NeoMem.h"
#include "PageClassName.h"

#include "ConstantsDatabase.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CPageClassName, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageClassName, CPropertyPage)
	//{{AFX_MSG_MAP(CPageClassName)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CPageClassName::CPageClassName() : CPropertyPage(CPageClassName::IDD)
{
	//{{AFX_DATA_INIT(CPageClassName)
	m_strDescription = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT
}

CPageClassName::~CPageClassName()
{
}


// Note: This routine only gets called once!!
BOOL CPageClassName::OnInitDialog() 
{
	// Get document and parent sheet
	m_pDoc = CNeoDoc::GetDoc();
	ASSERT_VALID(m_pDoc);
	m_psh = (CSheetWizard*) GetParent();
	ASSERT_VALID(m_psh);

	// Call base class (calls DoDataExchange)
	CPropertyPage::OnInitDialog();

	// Set font for title
	m_lblTitle.SetFont(&theApp.m_fontControlsBold);

	// Set context sensitive help
	SetHelpID(IDD_CLASS_NAME);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPageClassName::DoDataExchange(CDataExchange* pDX)
{
	// Get temp object we're working with
	m_pobj = m_psh->m_pobj;
	ASSERT_VALID(m_pobj);

	if (!pDX->m_bSaveAndValidate) // Load
	{
		// Get name and description of object into cstrings
		m_strName = m_pobj->GetPropertyText(propName);
		m_strDescription = m_pobj->GetPropertyText(propDescription);
	}

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageClassName)
	DDX_Control(pDX, IDC_LBL_TITLE, m_lblTitle);
	DDX_Control(pDX, IDC_TXT_NAME, m_txtName);
	DDX_Control(pDX, IDC_TXT_DESCRIPTION, m_txtDescription);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	DDX_Text(pDX, IDC_TXT_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_TXT_NAME, m_strName);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) // Save
	{
		m_pobj->SetPropertyText(propName, m_strName, FALSE, FALSE);
		m_pobj->SetPropertyText(propDescription, m_strDescription, FALSE, FALSE);
	}

}



BOOL CPageClassName::OnSetActive() 
{
//	CPropertySheet* psheet = (CPropertySheet*) GetParent();
	CSheetWizard* psheet = (CSheetWizard*) GetParent();
	if (m_psh->m_nAddEditMode == CSheetWizard::modeAddOrEdit)
		psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	else
		psheet->SetWizardButtons(PSWIZB_NEXT);
	psheet->SetClassWizardTitle();
	return CPropertyPage::OnSetActive();
}



void CPageClassName::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//. goto next page if hit enter
	if (nChar == VK_RETURN)	
	{
	}
	CPropertyPage::OnKeyDown(nChar, nRepCnt, nFlags);
}



