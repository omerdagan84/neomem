
// CPageClassIntro


#include "precompiled.h"

#include "NeoMem.h"
#include "PageClassIntro.h"

#include "ConstantsDatabase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CPageClassIntro, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageClassIntro, CPropertyPage)
	//{{AFX_MSG_MAP(CPageClassIntro)
	ON_BN_CLICKED(IDC_OPT_EDIT, OnOptEdit)
	ON_BN_CLICKED(IDC_OPT_ADD, OnOptAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_NOTIFY(NM_SETFOCUS, IDC_LVW, OnSetFocusLvwClass)
	ON_NOTIFY(NM_DBLCLK, IDC_LVW, OnNotifyDblClkLvw)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LVW, OnItemChangedLvw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CPageClassIntro::CPageClassIntro() : CPropertyPage(CPageClassIntro::IDD) {
	//{{AFX_DATA_INIT(CPageClassIntro)
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_psh = NULL;
	m_pobj = NULL;
}

CPageClassIntro::~CPageClassIntro() {
}


// Note: This only gets called once!

BOOL CPageClassIntro::OnInitDialog() {

	// Get document and parent sheet
	m_pDoc = CNeoDoc::GetDoc();
	m_psh = STATIC_DOWNCAST(CSheetWizard, GetParent());
	ASSERT_VALID(m_psh);

	// Call base class
	CPropertyPage::OnInitDialog();
	
	// Initialize listview
	m_lvw.m_pDoc = m_pDoc;
	m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
	m_lvw.SetFullRowSelect(TRUE);
	m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsClassFolder), TRUE, TRUE);

	// Fill listview with classes
	// Include root class if we're in admin mode
	BOOL bIncludeRoot = theApp.m_bAdmin ? TRUE : FALSE;
	BObject* pobjStart = m_pDoc->GetObject(rootClass);
	ASSERT_VALID(pobjStart);
	m_lvw.AddObjects(pobjStart, theApp.m_lngExcludeFlags, bIncludeRoot, TRUE);
	m_lvw.SortByProperty(propName, 1);

	// Remove the dummy new class object, which is included in the list because 
	// the temp flag is off for this object so it will show up in the Contents step.
	m_lvw.DeleteItemData((LPARAM) m_psh->m_pobjAdd);

	// Select the object to edit
//	if (!m_psh->m_bAddMode && m_pobj == 0)
//		// If we're in add mode and no valid object, select first one
//		m_lvw.SelectItem(0);
//	else
//		m_lvw.SelectItemData((DWORD) m_pobj);

	if (m_psh->m_bAddMode) {
		m_lvw.SelectItem(0); // select first class
		m_optAdd.SetCheck(1); // select add
		OnOptAdd(); // enable/disable controls
	}
	else {
		m_lvw.SelectItemData((DWORD) m_pobj); // select specified class
		m_optEdit.SetCheck(1); // select edit
		OnOptEdit(); // enable/disable controls
	}

	// If in add only or edit only mode, disable appropriate option
	if (m_psh->m_nAddEditMode == CSheetWizard::modeAddOnly) {
		m_optEdit.EnableWindow(FALSE);
		m_lvw.EnableWindow(FALSE);
//		m_lblClass.EnableWindow(FALSE);
		m_btnDelete.EnableWindow(FALSE);
	}
	else if (m_psh->m_nAddEditMode == CSheetWizard::modeEditOnly) {
		m_optAdd.EnableWindow(FALSE);
	}
	
	// Set context sensitive help
	SetHelpID(IDD_CLASS_INTRO);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CPageClassIntro::DoDataExchange(CDataExchange* pDX) {

	// Get temp object we're working with (stored in the sheet)
	m_pobj = m_psh->m_pobj;
	ASSERT_VALID(m_pobj);

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageClassIntro)
	DDX_Control(pDX, IDC_BTN_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_LVW, m_lvw);
	DDX_Control(pDX, IDC_OPT_ADD, m_optAdd);
	DDX_Control(pDX, IDC_OPT_EDIT, m_optEdit);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) { // Load
	}
	else { // Save

		// Set a variable in the wizard sheet so other sheets can have access to it
		m_psh->m_bAddMode = m_optAdd.GetCheck();

		if (m_psh->m_bAddMode) { // Add mode

			// Use the add object
			m_psh->m_pobj = m_psh->m_pobjAdd;
//			m_psh->m_bAddFolder = FALSE;

			// Make sure the Temp flag is cleared so the new class will 
			// show up in the contents step!
			m_psh->m_pobjAdd->SetFlag(flagTemp, FALSE, FALSE);

			// Make sure controls are enabled properly
			OnOptAdd();

		}
		else { // Edit mode
			
			// Make sure the Temp flag is set on the new class so it 
			// won't show up in the contents step!
			m_psh->m_pobjAdd->SetFlag(flagTemp, TRUE, FALSE);

			// Save the item the user chose to edit to the sheet
			BObject* pobjOrig = (BObject*) m_lvw.GetSelectedItemData();
			ASSERT_VALID(pobjOrig);

			m_psh->SetEditClass(pobjOrig);

			// Make sure controls are enabled properly
			OnOptEdit();
		}

	}
}


BOOL CPageClassIntro::OnSetActive() {
//	CPropertySheet* psheet = (CPropertySheet*) GetParent();
	CSheetWizard* psheet = DYNAMIC_DOWNCAST(CSheetWizard, GetParent());
	psheet->SetWizardButtons(PSWIZB_NEXT);
	psheet->SetClassWizardTitle();
	return CPropertyPage::OnSetActive();
}




void CPageClassIntro::OnOptAdd() {

	// Disable list of avail classes
//	m_lvw.ShowWindow(SW_HIDE);
//	m_lblClass.ShowWindow(SW_HIDE);
//	m_btnDelete.ShowWindow(SW_HIDE);

//	m_lvw.EnableWindow(FALSE);
//	m_lblClass.EnableWindow(FALSE);
//	m_btnDelete.EnableWindow(FALSE);
	m_optEdit.SetCheck(0);

	// Disable delete button
	m_btnDelete.EnableWindow(FALSE);

	m_lvw.SelectNone();
}

void CPageClassIntro::OnOptEdit() {

	// Enable list of classes
//	m_lvw.ShowWindow(SW_SHOW);
//	m_lblClass.ShowWindow(SW_SHOW);
//	m_btnDelete.ShowWindow(SW_SHOW);

//	m_lvw.EnableWindow(TRUE);
//	m_lblClass.EnableWindow(TRUE);
//	m_btnDelete.EnableWindow(TRUE);
	m_optAdd.SetCheck(0);
	
	// Enable delete button
	m_btnDelete.EnableWindow(TRUE);

	// Set focus to list
	m_lvw.SetFocus();

	// Select first item if none selected
	m_lvw.SelectAnything();
	
}


void CPageClassIntro::OnBtnDelete() {

	// Delete the selected class
	BObject* pobjClass = (BObject*) m_lvw.GetSelectedItemData();
	if (pobjClass) {
		ASSERT_VALID(pobjClass);
		if (m_pDoc->UIDeleteObject(pobjClass)) {
			// Remove class from list
			m_lvw.DeleteSelectedItem();
			// bug: and select something!
			m_lvw.SelectAnything();
		}
	}
}


void CPageClassIntro::OnNotifyDblClkLvw(NMHDR* pNMHDR, LRESULT* pResult) {
	// Goto next page in wizard
	m_psh->PressButton(PSBTN_NEXT);
	*pResult = 0;
}



void CPageClassIntro::OnSetFocusLvwClass(NMHDR* pNMHDR, LRESULT* pResult) {
	// Select edit radio button and turn off add
//	m_optAdd.SetCheck(0);
//	m_optEdit.SetCheck(1);

	// Enable delete button
//	m_btnDelete.EnableWindow(TRUE);

	// If nothing selected, select something
//	if (m_lvw.GetSelectedItem() == -1)
//		m_lvw.SelectItem(0);

	*pResult = 0;
}




void CPageClassIntro::OnItemChangedLvw(NMHDR* pNMHDR, LRESULT* pResult) {

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// If item selected, switch to add mode
	if (m_lvw.GetSelectedItem() != -1) {

		// Select edit radio button and turn off add
		m_optAdd.SetCheck(0);
		m_optEdit.SetCheck(1);

		// Enable delete button
		m_btnDelete.EnableWindow(TRUE);	
	}

	*pResult = 0;
}




