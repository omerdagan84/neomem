
// CPageClassInheritance


#include "precompiled.h"

#include "NeoMem.h"
#include "PageClassInheritance.h"

#include "ConstantsDatabase.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNCREATE(CPageClassInheritance, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageClassInheritance, CPropertyPage)
	//{{AFX_MSG_MAP(CPageClassInheritance)
	ON_NOTIFY(NM_DBLCLK, IDC_TVW, OnDblClkTvw)
	ON_BN_CLICKED(IDC_CHK_INHERIT, OnChkInherit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CPageClassInheritance::CPageClassInheritance() : CPropertyPage(CPageClassInheritance::IDD)
{
	//{{AFX_DATA_INIT(CPageClassInheritance)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = 0;
	m_psh = 0;
	m_pobj = 0;
}

CPageClassInheritance::~CPageClassInheritance()
{
}




BOOL CPageClassInheritance::OnInitDialog() 
{
	// Get document and parent sheet
	m_pDoc = BDoc::GetDoc();
	ASSERT_VALID(m_pDoc);
	m_psh = DYNAMIC_DOWNCAST(CSheetWizard, GetParent());
	ASSERT_VALID(m_psh);

	// Call base class
	CPropertyPage::OnInitDialog();	

	// Set font for title
	m_lblTitle.SetFont(&app.m_fontControlsBold);

	// Initialize tree
	m_tvw.SetImageList(m_pDoc->GetImageList(), TVSIL_NORMAL);

	// Set context sensitive help
	SetHelpID(IDD_CLASS_INHERITANCE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void 
CPageClassInheritance::DoDataExchange(CDataExchange* pDX)
{
	// Get temp object we're working with
	m_pobj = m_psh->m_pobj;
	ASSERT_VALID(m_pobj);

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageClassInheritance)
	DDX_Control(pDX, IDC_TVW, m_tvw);
	DDX_Control(pDX, IDC_LBL_TITLE, m_lblTitle);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	DDX_Control(pDX, IDC_CHK_INHERIT, m_chkInherit);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) // Load
	{
		// Fill tree with classes
		BObject* pobjStart = m_pDoc->GetObject(rootClass);
		m_tvw.DeleteAllItems();
		m_tvw.AddObjects(pobjStart, app.m_lngExcludeFlags, TRUE, TRUE);
//		m_tvw.AddObjects(pobjStart, app.m_lngExcludeFlags, FALSE, TRUE); //. bug if you don't include start item
		
		// Select parent
		BObject* pobjParent = m_pobj->GetParent();
		ASSERT_VALID(pobjParent);
		m_tvw.SelectItemData((DWORD) pobjParent);

		// Set checkbox
		BOOL bInherited = (pobjParent->GetObjectID() != rootClass);
		m_chkInherit.SetCheck(bInherited);

		// Update controls
		OnChkInherit();

		// If this is a system class, disable controls and change label
		if (m_pobj->GetFlag(flagNoModify))
		{
			m_lblInstructions.SetWindowText(_T("Because this is a system class, the inheritance cannot be changed."));
			m_chkInherit.EnableWindow(FALSE);
			m_tvw.EnableWindow(FALSE);
		}
		else
		{
			m_lblInstructions.SetWindowText(_T("If you want to inherit properties from another class, check the box and select the class here."));
			m_chkInherit.EnableWindow(TRUE);
		}
	}
	else // Save
	{
		// Save parent selected 
		// Note: We do move the object to the new parent when the wizard is finished
		BOOL bInherited = m_chkInherit.GetCheck();
		BObject* pobjParent = 0;
		if (bInherited)
			pobjParent = (BObject*) m_tvw.GetSelectedItemData();
		else
			pobjParent = m_pDoc->GetObject(rootClass);
		ASSERT_VALID(pobjParent);
		m_pobj->SetParent(pobjParent);
	}
}



BOOL CPageClassInheritance::OnSetActive() 
{
//	CPropertySheet* psheet = (CPropertySheet*) GetParent();
	CSheetWizard* psheet = DYNAMIC_DOWNCAST(CSheetWizard, GetParent());
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	psheet->SetClassWizardTitle();
	return CPropertyPage::OnSetActive();
}




void CPageClassInheritance::OnDblClkTvw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Goto next page in wizard
	m_psh->PressButton(PSBTN_NEXT);
	*pResult = 0;
}



void CPageClassInheritance::OnChkInherit() 
{
	BOOL bInherit = m_chkInherit.GetCheck();
	m_tvw.EnableWindow(bInherit);
}



