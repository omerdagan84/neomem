
// CPageClassProperties


#include "precompiled.h"

#include "PageClassProperties.h"
#include "DialogEditProperty.h"
#include "Hint.h"
#include "NeoMem.h"
#include "SheetWizard.h"

#include "BDataColumns.h"
#include "ConstantsDatabase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNCREATE(CPageClassProperties, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageClassProperties, CPropertyPage)
	//{{AFX_MSG_MAP(CPageClassProperties)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBtnEdit)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LVW, OnDblClkLvw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CPageClassProperties::CPageClassProperties() : CPropertyPage(CPageClassProperties::IDD)
{
	//{{AFX_DATA_INIT(CPageClassProperties)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_psh = NULL;
	m_pobj = NULL;
	m_pobjPropertiesFolder = NULL;
//	m_pdatColumns = NULL;
	m_bInitialized = FALSE;
}

CPageClassProperties::~CPageClassProperties()
{
//	if (m_pdatColumns)
//		delete m_pdatColumns;
}





BOOL CPageClassProperties::OnInitDialog() 
{
	// Get document and parent sheet
	m_pDoc = CNeoDoc::GetDoc();
	ASSERT_VALID(m_pDoc);
	m_psh = (CSheetWizard*) GetParent();
	ASSERT_VALID(m_psh);

	// Call base class (required)
	CPropertyPage::OnInitDialog();
	
	// Set font for title
	m_lblTitle.SetFont(&theApp.m_fontControlsBold);

	// Set context sensitive help
	SetHelpID(IDD_CLASS_PROPERTIES);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPageClassProperties::DoDataExchange(CDataExchange* pDX)
{
	// Get temp object we're working with
	m_pobj = m_psh->m_pobj;
	ASSERT_VALID(m_pobj);

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageClassProperties)
	DDX_Control(pDX, IDC_LVW, m_lvw);
	DDX_Control(pDX, IDC_LBL_TITLE, m_lblTitle);
	DDX_Control(pDX, IDC_BTN_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BTN_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) // Load
	{
		if (!m_bInitialized)
		{
			// Initialize listview
			m_lvw.m_pDoc = m_pDoc;
			m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
			m_lvw.SetFullRowSelect(TRUE);
			m_lvw.SetCheckboxes(TRUE);
			m_lvw.m_bDisabledItems = TRUE; // Special disabled items mode (draws some items in gray and won't let user change their checkbox)
			m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsPropertyFolder), FALSE, TRUE);
			m_bInitialized = TRUE;
		}
		
		// Select properties
		LoadData();

	}
	else // Save
	{
		// Save selected properties to array
		// Walk through listview and see which properties are checked off
		// Add them to a BDataLink object
		// Note: This does not add items if they are disabled (ie the inherited props)
		BDataLink* pdatLink = new BDataLink;
		m_lvw.GetSelectedItems(pdatLink);

		// Save bdata to the class we're editing
		m_pobj->SetPropertyData(propObjectProperties, pdatLink, FALSE, FALSE);
	}
}


BOOL CPageClassProperties::OnSetActive() 
{
//	CPropertySheet* psheet = (CPropertySheet*) GetParent();
	CSheetWizard* psheet = (CSheetWizard*) GetParent();
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);	
	psheet->SetClassWizardTitle();
	return CPropertyPage::OnSetActive();
}



// Add a new property
void CPageClassProperties::OnBtnAdd() 
{
	BObject* pobjProperty = m_pDoc->UIAddNewPropertyDef();
	if (pobjProperty)
	{
		ASSERT_VALID(pobjProperty);

		// Add new property to this treeview and select it
		int nItem = m_lvw.AddObject(pobjProperty);
		m_lvw.SelectItem(nItem);

		// Also check it off
		m_lvw.SetCheck(nItem, TRUE);
	}

	// Set focus back to treeview
	m_lvw.SetFocus();
}



// Edit an existing property
void CPageClassProperties::OnBtnEdit() 
{
	BObject* pobjProp = (BObject*) m_lvw.GetSelectedItemData();	
	if (!pobjProp) return;
	ASSERT_VALID(pobjProp);

//	pobjProp->EditPropertyDef();
	if (m_pDoc->UIEditPropertyDef(pobjProp))
	{
		// Update listview
		m_lvw.RedrawWindow();
	}

	// Set focus back to treeview
	m_lvw.SetFocus();
}


// Delete an existing property
void CPageClassProperties::OnBtnDelete() 
{
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();	
	if (pobj)
	{
		ASSERT_VALID(pobj);
		// Check if property is marked NoDelete
		if (pobj->GetFlag(flagNoDelete))
		{
			AfxMessageBox("This property is a system property and cannot be deleted.");
		}
		else
		{
			// Delete the property from the document
			if (m_pDoc->UIDeleteObject(pobj))
			{
				// Remove from list
				m_lvw.DeleteSelectedItem();
			}
		}
	}

	// Set focus back to treeview
	m_lvw.SetFocus();
}




void CPageClassProperties::OnDblClkLvw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Edit the selected property
	// too easy to do accidentally
//	OnBtnEdit();	
	*pResult = 0;
}





void CPageClassProperties::LoadData()
{
	// Clear list
	m_lvw.DeleteAllItems();

	// Add inherited properties to list first
	// Note: GetPropertyDefs doesn't filter based on admin settings, so must do it here
	CObArray aInheritedProps;
//	int nPropDefs = m_pobj->GetPropertyDefs(aInheritedProps, TRUE);
	int nPropDefs = m_pobj->GetPropertyDefs(aInheritedProps, TRUE, TRUE);
	for (int i = 0; i < nPropDefs; i++)
	{
		BObject* pobjPropDef = (BObject*) aInheritedProps.GetAt(i);
		ASSERT_VALID(pobjPropDef);
		if (!(pobjPropDef->GetFlag(theApp.m_lngExcludeFlags)))
		{
			int nItem = m_lvw.AddObject(pobjPropDef);
			m_lvw.SetCheck(nItem, TRUE);
			// Mark item as disabled
			// can't do this because the control uses the state image to display the checkbox!
//			m_lvw.SetItemState(nItem, CListCtrlEx::stateDisabled, LVIS_STATEIMAGEMASK);
			m_lvw.SetItemState(nItem, CListCtrlEx::stateDisabled, LVIS_OVERLAYMASK);
			// somehow this is getting lost by the listview
//			UINT nState = m_lvw.GetItemState(nItem, LVIS_OVERLAYMASK);
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
	ASSERT_VALID(m_pobjPropertiesFolder);
	BObjects* paChildren = m_pobjPropertiesFolder->GetChildren();
	ASSERT_VALID(paChildren);
	BObjects* paCopy = paChildren->CreateCopy();
	ASSERT_VALID(paCopy);
	paCopy->Sort(propName);
	ASSERT_VALID(paCopy);
	
	// Now we have a sorted array of all properties
	// walk through them and add to the list if not already there
	nPropDefs = paCopy->GetSize();
	for (int i = 0; i < nPropDefs; i++)
	{
		BObject* pobjPropDef = (BObject*) paCopy->GetAt(i);
		ASSERT_VALID(pobjPropDef);
		if (!(pobjPropDef->GetFlag(theApp.m_lngExcludeFlags)))
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
		delete pdatLink;
	}

	// Now select the first property, just to have something selected
	m_lvw.SelectItem(0);
}


