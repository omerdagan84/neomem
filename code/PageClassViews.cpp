
// CPageClassViews



#include "precompiled.h"

#include "NeoMem.h"
#include "PageClassViews.h"

#include "ConstantsDatabase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNCREATE(CPageClassViews, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageClassViews, CPropertyPage)
	//{{AFX_MSG_MAP(CPageClassViews)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CPageClassViews::CPageClassViews() : CPropertyPage(CPageClassViews::IDD)
{
	//{{AFX_DATA_INIT(CPageClassViews)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = 0;
}

CPageClassViews::~CPageClassViews()
{
}



BOOL CPageClassViews::OnInitDialog() 
{
	// Get document and parent sheet
	m_pDoc = BDoc::GetDoc();
	ASSERT_VALID(m_pDoc);
	m_psh = DYNAMIC_DOWNCAST(CSheetWizard, GetParent());
	ASSERT_VALID(m_psh);

	// Call base class (calls DoDataExchange)
	CPropertyPage::OnInitDialog();
	
	// Set font for title
	m_lblTitle.SetFont(&app.m_fontControlsBold);

	// Initialize listview
	m_lvw.m_pDoc = m_pDoc;
	m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
	m_lvw.SetFullRowSelect(TRUE);
	m_lvw.SetCheckboxes(TRUE);
	m_lvw.m_bDisabledItems = TRUE; // Special disabled items mode (draws some items in gray and won't let user change their checkbox)

	m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsViewFolder), FALSE, TRUE);

	// Select views
	LoadData();
	
	// Set context sensitive help
	SetHelpID(IDD_CLASS_VIEWS);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CPageClassViews::DoDataExchange(CDataExchange* pDX)
{
	// Get temp object we're working with
	m_pobj = m_psh->m_pobj;
	ASSERT_VALID(m_pobj);

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageClassViews)
	DDX_Control(pDX, IDC_LVW, m_lvw);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	DDX_Control(pDX, IDC_LBL_TITLE, m_lblTitle);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) // Load
	{
	}
	else // Save
	{
		// Save selected properties to array
		// Walk through listview and see which properties are checked off
		// Add them to a BDataLink object
		// Note: This does not add items if they are disabled (ie the inherited props)
//		BDataLink* pdatLink = new BDataLink;
//		m_lvw.GetSelectedItems(pdatLink);

		// Save bdata to the class we're editing
//		m_pobj->SetPropertyData(propObjectProperties, pdatLink, FALSE, FALSE);
	}
}




BOOL CPageClassViews::OnSetActive() 
{
//	CPropertySheet* psheet = (CPropertySheet*) GetParent();
	CSheetWizard* psheet = DYNAMIC_DOWNCAST(CSheetWizard, GetParent());
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);	
	psheet->SetClassWizardTitle();
	return CPropertyPage::OnSetActive();
}



void CPageClassViews::LoadData()
{
	// Clear list
	m_lvw.DeleteAllItems();

	// Fill with View objects
	BObject* pobjStart = m_pDoc->GetObject(folderViews);
	m_lvw.AddObjects(pobjStart, flagAdminOnly, FALSE, FALSE);

/*	// Add inherited properties to list first
	// Note: GetPropertyDefs doesn't filter based on admin settings, so must do it here
	CObArray aInheritedProps;
	int nPropDefs = m_pobj->GetPropertyDefs(aInheritedProps, TRUE);
	for (int i = 0; i < nPropDefs; i++)
	{
		BObject* pobjPropDef = (BObject*) aInheritedProps.GetAt(i);
		ASSERT_VALID(pobjPropDef);
		if (!(pobjPropDef->GetFlag(app.m_lngExcludeFlags)))
		{
			int nItem = m_lvw.AddObject(pobjPropDef);
			m_lvw.SetCheck(nItem, TRUE);
			// Mark item as disabled
			// can't do this because the control uses the state image to display the checkbox!
//			m_lvw.SetItemState(nItem, CListCtrlEx::stateDisabled, LVIS_STATEIMAGEMASK);
			m_lvw.SetItemState(nItem, CListCtrlEx::stateDisabled, LVIS_OVERLAYMASK);
		}
	}

	// Now go through and mark the inherited property defs to indicate to the listview that
	// they should be disabled.
//	nPropDefs = aInheritedProps.GetSize();
//	for (i = 0; i < nPropDefs; i++)
//	{
//		BObject* pobjPropDef = (BObject*) aInheritedProps.GetAt(i);
//		ASSERT_VALID(pobjPropDef);
//		int nItem = m_lvw.FindItemData((LPARAM) pobjPropDef);
//		m_lvw.SetItemState(nItem, CListCtrlEx::stateDisabled, LVIS_STATEIMAGEMASK);
//	}

	// Add all PropertyDefs to list
	// Get array of all properties, sort it, then walk through add prop if not in listview
	m_pobjPropertiesFolder = m_pDoc->GetObject(folderProperties);
	BObjects* paChildren = m_pobjPropertiesFolder->m_paChildren;
	ASSERT_VALID(paChildren);
	BObjects* paCopy = paChildren->CreateCopy();
	ASSERT_VALID(paCopy);
	paCopy->Sort(propName);
	ASSERT_VALID(paCopy);
	
	// Now we have a sorted array of all properties
	// walk through them and add to the list if not already there
	nPropDefs = paCopy->GetSize();
	for (i = 0; i < nPropDefs; i++)
	{
		BObject* pobjPropDef = (BObject*) paCopy->GetAt(i);
		ASSERT_VALID(pobjPropDef);
		if (!(pobjPropDef->GetFlag(app.m_lngExcludeFlags)))
		{
			// Add it if it's not in the list already
			if (m_lvw.FindItemData((LPARAM) pobjPropDef) == -1)
			{
				int nItem = m_lvw.AddObject(pobjPropDef);
			}
		}
	}

	// Delete the copy of the array
	delete paCopy;

	// Now check off all propertydefs included in this class, if any
	BDataLink* pdatLink = DYNAMIC_DOWNCAST(BDataLink, m_pobj->GetPropertyData(propObjectProperties));
	if (pdatLink)
	{
		ASSERT_VALID(pdatLink);
		m_lvw.SetSelectedItems(pdatLink);
	}

	// Now select the first property, just to have something selected
	m_lvw.SelectItem(0);
*/

}


