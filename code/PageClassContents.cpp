
// CPageClassContents


#include "precompiled.h"

#include "PageClassContents.h"
#include "NeoMem.h"
#include "SheetWizard.h"

#include "ConstantsDatabase.h"
#include "BDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CPageClassContents, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageClassContents, CPropertyPage)
	//{{AFX_MSG_MAP(CPageClassContents)
	ON_BN_CLICKED(IDC_OPT_CHILDREN_YES, OnOptChildrenYes)
	ON_BN_CLICKED(IDC_OPT_CHILDREN_NO, OnOptChildrenNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CPageClassContents::CPageClassContents() : CPropertyPage(CPageClassContents::IDD)
{
	//{{AFX_DATA_INIT(CPageClassContents)
	//}}AFX_DATA_INIT
	m_pDoc = 0;
	m_psh = 0;
	m_pobj = 0;
}

CPageClassContents::~CPageClassContents()
{
}


BOOL CPageClassContents::OnInitDialog() 
{
	// Get document and parent sheet
	m_pDoc = BDoc::GetDoc();
	ASSERT_VALID(m_pDoc);
	m_psh = DYNAMIC_DOWNCAST(CSheetWizard, GetParent());
	ASSERT_VALID(m_psh);

	// Call base class (calls DoDataExchange also)
	CPropertyPage::OnInitDialog();

	// Set font for title
	m_lblTitle.SetFont(&app.m_fontControlsBold);

	// Set context sensitive help
	SetHelpID(IDD_CLASS_CONTENTS);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CPageClassContents::DoDataExchange(CDataExchange* pDX)
{
	// Get temp object we're working with
	m_pobj = m_psh->m_pobj;
	ASSERT_VALID(m_pobj);

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageClassContents)
	DDX_Control(pDX, IDC_CBO_CLASS, m_cboClass);
	DDX_Control(pDX, IDC_LBL_SORTED, m_lblSorted);
	DDX_Control(pDX, IDC_LBL_DEFAULT_CLASS, m_lblDefaultClass);
	DDX_Control(pDX, IDC_LBL_TITLE, m_lblTitle);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	DDX_Control(pDX, IDC_OPT_CHILDREN_YES, m_optChildrenYes);
	DDX_Control(pDX, IDC_OPT_CHILDREN_NO, m_optChildrenNo);
	DDX_Control(pDX, IDC_OPT_SORT_YES, m_optSortYes);
	DDX_Control(pDX, IDC_OPT_SORT_NO, m_optSortNo);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) // Load
	{
		// Set imagelist if haven't already done so
		if (m_cboClass.GetImageList() == NULL)
			m_cboClass.SetImageList(m_pDoc->GetImageList());

		// Load data into controls
		LoadData();
	}
	else // Save
	{
		// Save children yes/no
//		BOOL bHasChildren = m_optChildrenYes.GetCheck();
//		m_pobj->SetFlag(flagObjectHasChildren, bHasChildren, FALSE);

		// Save default class
		BObject* pobjDefaultClass = (BObject*) m_cboClass.GetSelectedItemData();
		ASSERT_VALID(pobjDefaultClass);
		m_pobj->SetPropertyLink(propObjectDefaultClass, pobjDefaultClass->id, FALSE, FALSE);

		// Save autosort
		BOOL bNoAutosort = m_optSortNo.GetCheck();
		m_pobj->DeleteProperty(propObjectFlags); // always delete the existing value, if any (set to null).
		// If new value <> 0 then set the property value, otherwise leave null to save space!
		if (bNoAutosort)
		{
//x			BDataFlags* pdatFlags = new BDataFlags;
//			pdatFlags->SetFlags(bNoAutosort ? flagNoAutosort : 0);
//			m_pobj->SetPropertyData(propObjectFlags, pdatFlags, FALSE, FALSE);
//			delete pdatFlags;
			BDataFlags datFlags;
			datFlags.SetFlags(bNoAutosort ? flagNoAutosort : 0);
			m_pobj->SetPropertyData(propObjectFlags, &datFlags, false, false);
		}
	}
}



BOOL CPageClassContents::OnSetActive() 
{
	CSheetWizard* psheet = STATIC_DOWNCAST(CSheetWizard, GetParent()); 
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	psheet->SetClassWizardTitle();
	return CPropertyPage::OnSetActive();
}




void CPageClassContents::LoadData()
{
	// Has Children? yes/no
//	BOOL bHasChildren = m_pobj->GetFlag(flagObjectHasChildren);
//	if (bHasChildren)
//		m_optChildrenYes.SetCheck(1);
//	else
//		m_optChildrenNo.SetCheck(1);

	// Fill cbo with classes and select default
	BObject* pobjStart = m_pDoc->GetObject(rootClass);
	m_cboClass.DeleteAllItems();
	m_cboClass.AddObjects(pobjStart, app.m_lngExcludeFlags, FALSE, TRUE);
	OBJID idDefaultClass = m_pobj->GetPropertyLink(propObjectDefaultClass);
	BObject* pobjDefaultClass = m_pDoc->GetObject(idDefaultClass);
	m_cboClass.SelectItemData((LPARAM) pobjDefaultClass);

	// Get autosort
//x	BDataFlags* pdatFlags = DYNAMIC_DOWNCAST(BDataFlags, m_pobj->GetPropertyData(propObjectFlags));
//	BOOL bNoAutosort = FALSE;
//	if (pdatFlags) {
//		bNoAutosort = pdatFlags->GetFlags() & flagNoAutosort;
//		delete pdatFlags;
//	}
	ULONG lngFlags = m_pobj->GetPropertyFlags(propObjectFlags);
	BOOL bNoAutosort = lngFlags & flagNoAutosort;


	// Since this form gets reloaded, you might wind up with both checks on unless you explicitly set both!
	m_optSortNo.SetCheck(bNoAutosort ? 1 : 0);
	m_optSortYes.SetCheck(bNoAutosort ? 0 : 1);

	// Enable/disable controls
//	UpdateControls();
}





void CPageClassContents::OnOptChildrenYes() 
{
//	UpdateControls();
}

void CPageClassContents::OnOptChildrenNo() 
{
//	UpdateControls();
}


// Enable/disable controls based on selections
void CPageClassContents::UpdateControls()
{
//	BOOL bChildren = m_optChildrenYes.GetCheck();
//	m_lblDefaultClass.EnableWindow(bChildren);
//	m_cboClass.EnableWindow(bChildren);
//	m_lblSorted.EnableWindow(bChildren);
//	m_optSortYes.EnableWindow(bChildren);
//	m_optSortNo.EnableWindow(bChildren);
}




