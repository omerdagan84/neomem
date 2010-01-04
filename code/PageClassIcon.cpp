
// CPageClassIcon



#include "precompiled.h"

#include "PageClassIcon.h"
#include "Constants.h"
#include "DialogEditName.h"
#include "NeoMem.h"
#include "Strings.h"

#include "NeoDoc.h"
#include "BDataIcon.h"
#include "ConstantsDatabase.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNCREATE(CPageClassIcon, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageClassIcon, CPropertyPage)
	//{{AFX_MSG_MAP(CPageClassIcon)
	ON_BN_CLICKED(IDC_BTN_IMPORT, OnBtnImport)
	ON_BN_CLICKED(IDC_BTN_DEFAULT, OnBtnDefault)
	ON_NOTIFY(NM_DBLCLK, IDC_LVW, OnDblClkLvw)
	ON_BN_CLICKED(IDC_BTN_RENAME, OnBtnRename)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CPageClassIcon::CPageClassIcon() : CPropertyPage(CPageClassIcon::IDD)
{
	//{{AFX_DATA_INIT(CPageClassIcon)
	//}}AFX_DATA_INIT
	m_pDoc = 0;
	m_psh = 0;
	m_pobj = 0;
	m_pobjIconFolder = 0;
	m_pobjStart = 0;
}

CPageClassIcon::~CPageClassIcon()
{
}




BOOL CPageClassIcon::OnInitDialog() 
{
	// Get document and parent sheet
	m_pDoc = CNeoDoc::GetDoc();
	m_psh = (CSheetWizard*) GetParent(); //,cast
	ASSERT_VALID(m_psh);

	// Call base class (calls DoDataExchange)
	CPropertyPage::OnInitDialog();

	// Set font for title
	m_lblTitle.SetFont(&theApp.m_fontControlsBold);

	// Set context sensitive help
	SetHelpID(IDD_CLASS_ICON);


	// Initialize listview
	m_lvw.m_pDoc = m_pDoc;
	m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
	m_lvw.SetFullRowSelect(TRUE);
	m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsIconFolder), TRUE, TRUE);

	// Add icons to list
	m_pobjIconFolder = m_pDoc->GetObject(folderIcons);
	ASSERT_VALID(m_pobjIconFolder);
	m_lvw.AddObjects(m_pobjIconFolder, theApp.m_lngExcludeFlags, FALSE, FALSE);
	m_lvw.SortByProperty(propName, 1);

	// Get default icon
	m_lngDefaultIconID = m_pobj->GetDefaultIconID();

	// Select icon
	ULONG lngIconID = m_pobj->GetIconID();
	BObject* pobjIcon = m_pDoc->GetObject(lngIconID);
	ASSERT_VALID(pobjIcon);
	m_lvw.SelectItemData((LPARAM) pobjIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPageClassIcon::DoDataExchange(CDataExchange* pDX)
{
	// Get temp object we're working with
	// Note: This must be here so it gets called each visit to the page(?)
	m_pobj = m_psh->m_pobj;
	ASSERT_VALID(m_pobj);

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageClassIcon)
	DDX_Control(pDX, IDC_LVW, m_lvw);
	DDX_Control(pDX, IDC_LBL_TITLE, m_lblTitle);
	DDX_Control(pDX, IDC_BTN_RENAME, m_btnRename);
	DDX_Control(pDX, IDC_BTN_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BTN_IMPORT, m_btnImport);
	DDX_Control(pDX, IDC_BTN_DEFAULT, m_btnDefault);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) // Load
	{
		if (m_lvw.GetItemCount() > 0)
		{
			// Get default icon
			m_lngDefaultIconID = m_pobj->GetDefaultIconID();

			// Get current icon and select it
			ULONG lngIconID = m_pobj->GetIconID();
			BObject* pobjIcon = m_pDoc->GetObject(lngIconID);
			m_lvw.SelectItemData((LPARAM) pobjIcon);
		}
	}
	else // Save
	{
		BObject* pobjIcon = (BObject*) m_lvw.GetSelectedItemData();
		ASSERT_VALID(pobjIcon);
		ULONG lngIconID = pobjIcon->GetObjectID();
		// If user chose to use the default icon, we should set iconid to zero
//		if (lngIconID == m_lngDefaultIconID)
//			m_pobj->m_lngIconID = 0;
//		else
//			m_pobj->m_lngIconID = pobjIcon->GetObjectID();
		m_pobj->SetIconID(lngIconID); // this should be okay, right?
	}
}



BOOL CPageClassIcon::OnSetActive() 
{
//	CPropertySheet* psheet = (CPropertySheet*) GetParent();
	CSheetWizard* psheet = (CSheetWizard*) GetParent();
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	psheet->SetClassWizardTitle();
	return CPropertyPage::OnSetActive();
}



// Import an .ico file and create a new icon from it
void CPageClassIcon::OnBtnImport()
{
	BObject* pobjIcon = m_pDoc->UIImportIcon();
	if (pobjIcon)
	{
		// Add new icon to this listview and select it
		m_lvw.AddObject(pobjIcon);
		m_lvw.SelectItemData((LPARAM) pobjIcon);
	}

	// Set focus back to listview
	m_lvw.SetFocus();
}





void CPageClassIcon::OnDblClkLvw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Goto next page in wizard
	m_psh->PressButton(PSBTN_NEXT);
	*pResult = 0;
}



void CPageClassIcon::OnBtnRename() 
{
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();
	if (pobj)
	{
		ASSERT_VALID(pobj);
		CDialogEditName dlg;
		dlg.m_strCaption = "Rename Icon";
		dlg.m_strInstructions = "Enter the new name for the icon:";
		dlg.m_strName = pobj->GetPropertyText(propName);
		if (dlg.DoModal() == IDOK)
		{
			// This will set document modified flag and update views
			pobj->SetPropertyText(propName, dlg.m_strName); 
			int nItem = m_lvw.GetSelectedItem();
			m_lvw.RedrawItems(nItem, nItem);
		}
	}
	m_lvw.SetFocus();
}


void CPageClassIcon::OnBtnDelete() 
{	
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();
	if (pobj)
	{
		ASSERT_VALID(pobj);
		if (m_pDoc->UIDeleteObject(pobj)) // this will update all views
		{
			// Make sure all views refresh their icons
			m_pDoc->UpdateAllViewsEx(NULL, hintRefreshAllIcons);
			// Remove from listview also
			m_lvw.DeleteSelectedItem();
			// Select another icon
			m_lvw.SelectItem(0);
		}
	}
	m_lvw.SetFocus();
}



// Select default icon for the object
void CPageClassIcon::OnBtnDefault() 
{
	BObject* pobjIcon = m_pDoc->GetObject(m_lngDefaultIconID);
	ASSERT_VALID(pobjIcon);
	m_lvw.SelectItemData((LPARAM) pobjIcon);
	m_lvw.SetFocus();
}



