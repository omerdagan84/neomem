
// CDialogSelectProperty



#include "precompiled.h"

#include "NeoMem.h"
#include "DialogSelectProperty.h"

#include "ConstantsDatabase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CDialogSelectProperty, CDialog)
	//{{AFX_MSG_MAP(CDialogSelectProperty)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBtnEdit)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_CBN_SELCHANGE(IDC_CBO_CLASS, OnSelChangeCboClass)
	ON_BN_CLICKED(IDC_OPT_SHOW_ALL, OnOptShowAll)
	ON_BN_CLICKED(IDC_OPT_SHOW_SELECTED, OnOptShowSelected)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CDialogSelectProperty::CDialogSelectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSelectProperty::IDD, pParent)
{
	// Get document (used by domodal methods also)
	m_pDoc = CNeoDoc::GetDoc();
	ASSERT_VALID(m_pDoc);
	m_pobjClasses = 0;
	m_pobjProperties = 0;
	m_lngSelectedID = 0;
//	m_pobjParent = 0;
	m_pobjDefaultClass = 0;
//	m_bShowAll = TRUE;
	m_bShowAll = FALSE;
	m_nMode = IDD_ADD_PROPERTY;
	//{{AFX_DATA_INIT(CDialogSelectProperty)
	//}}AFX_DATA_INIT
}



BOOL CDialogSelectProperty::OnInitDialog() 
{
	// Call base class (calls DoDataExchange)
	CDialog::OnInitDialog();

	// Modify dialog layout based on mode
	switch (m_nMode)
	{
		case IDD_PROPERTY_WIZARD:
			m_lblInstructions.SetWindowText("You can create new properties, or edit or delete existing properties here. All actions apply to the current file only.");
			m_bShowAll = TRUE;
//			m_btnOK.ShowWindow(SW_HIDE);
//			m_btnCancel.ShowWindow(SW_HIDE);
//			m_btnClose.ShowWindow(SW_SHOW);
			m_btnOK.SetWindowText("&Close");
			m_btnCancel.EnableWindow(FALSE);
			break;
		case IDD_ADD_PROPERTY:
		default:
//			m_btnOK.ShowWindow(SW_SHOW);
//			m_btnCancel.ShowWindow(SW_SHOW);
//			m_btnClose.ShowWindow(SW_HIDE);
			break;
	}

	// Get start objects
	m_pobjClasses = m_pDoc->GetObject(rootClass);
	m_pobjProperties = m_pDoc->GetObject(folderProperties);
	ASSERT_VALID(m_pobjClasses);
	ASSERT_VALID(m_pobjProperties);

	// Initialize class list
	m_cboClass.SetImageList(m_pDoc->GetImageList());
	m_cboClass.AddObjects(m_pobjClasses, theApp.m_lngExcludeFlags, FALSE, TRUE);

	// Initialize listview
	m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
	m_lvw.m_pDoc = m_pDoc;
	m_lvw.SetFullRowSelect(TRUE);
	m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsPropertyFolder), TRUE, TRUE);
	m_lvw.m_bDisabledItems = TRUE; // Special disabled items mode (draws some items in gray and won't let user change their checkbox)

	// Get default class
//	if (m_pobjParent)
//	{
//		m_pobjDefaultClass = m_pobjParent->GetPropertyLink(propDefaultClass);
//		if (m_pobjDefaultClass)
//		{
//			ASSERT_VALID(m_pobjDefaultClass);
//			m_cboClass.SelectItemData((DWORD) m_pobjDefaultClass);
//		}
//	}
//	if (m_cboClass.GetCurSel() == -1)
//		m_cboClass.SetCurSel(0);
	if (m_pobjDefaultClass == 0)
		m_pobjDefaultClass = m_pDoc->GetObject(classPaper);
	ASSERT_VALID(m_pobjDefaultClass);
	m_cboClass.SelectItemData((DWORD) m_pobjDefaultClass);
	
	// Fill properties list if still empty
//	if (m_lvw.GetItemCount() == 0)
//		AddProperties();

	// Select option
	// this will also fill list with properties
	if (m_bShowAll)
	{
		m_optShowAll.SetCheck(1);
		OnOptShowAll();
	}
	else
	{
		m_optShowSelected.SetCheck(1);
		OnOptShowSelected();
	}

//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	m_lvw.SelectItem(0); // select first item
	m_lvw.SetFocus();
	return FALSE;
}



void CDialogSelectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSelectProperty)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_LVW, m_lvw);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	DDX_Control(pDX, IDC_CBO_CLASS, m_cboClass);
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_BTN_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BTN_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_OPT_SHOW_ALL, m_optShowAll);
	DDX_Control(pDX, IDC_OPT_SHOW_SELECTED, m_optShowSelected);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) // Save
	{
//		if (m_pdatLink)
//			m_lvw.GetSelectedItems(m_pdatLink);
		BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();
		if (pobj)
			m_lngSelectedID = pobj->m_lngObjectID;
	}
}





void CDialogSelectProperty::OnBtnHelp() 
{
//	theApp.WinHelp(HID_BASE_RESOURCE + IDD_INSERT_COLUMN);
//	theApp.WinHelp(HID_BASE_RESOURCE + IDD_ADD_PROPERTY);
//	theApp.WinHelp(HID_BASE_RESOURCE + m_nHelpID);
	theApp.WinHelp(HID_BASE_RESOURCE + m_nMode); // IDD_ADD_PROPERTY or IDD_PROPERTY_WIZARD
}


void CDialogSelectProperty::OnBtnAdd() 
{
	CWaitCursor cw;

	// Get parent object
	BObject* pobjParent = m_pobjProperties;
	ASSERT_VALID(pobjParent);

	BObject* pobjNew = 0;

	// Add a new property using the edit property dialog
	pobjNew = m_pDoc->UIAddNewPropertyDef();

	// If user said OK to new object, add it to listview and select it.
	if (pobjNew)
	{
		ASSERT_VALID(pobjNew);

		// Add the new property to the currently selected class.
		// I.e. get the class's propertydefs array and add the new one to it.
		// But only do this if we're limiting properties visible to a certain class!
		if (m_bShowAll == FALSE)
		{
			BObject* pobjClass = (BObject*) m_cboClass.GetSelectedItemData();
			if (pobjClass)
			{
				// this will assert if not bdatalink
				//, duplicate code in 3 places!!
				BDataLink* pdatLinks = STATIC_DOWNCAST(BDataLink, pobjClass->GetPropertyData(propObjectProperties));
				if (pdatLinks)
				{
					ASSERT_VALID(pdatLinks);
					pdatLinks->AddLink(pobjNew);
				}
				else
				{
					// No bdata yet - create a new one and store it with the class def
					pdatLinks = new BDataLink;
					pdatLinks->SetMultiple();
					pdatLinks->AddLink(pobjNew);
					pobjClass->SetPropertyData(propObjectProperties, pdatLinks);
				}
			}
		}

		// Add the property to the current list of properties
		int nItem = m_lvw.AddObject(pobjNew);
		ASSERT(nItem != -1);
		m_lvw.SelectItem(nItem); // select it

	}

	// Set focus back to list
	m_lvw.SetFocus();
}



void CDialogSelectProperty::OnBtnEdit() 
{
	CWaitCursor cw;

	BOOL bModified = FALSE;

	// Get selected object, exit if none selected
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();	
	if (pobj == 0) return;
	ASSERT_VALID(pobj);

	// Edit the property def in the edit property dialog
	bModified = m_pDoc->UIEditPropertyDef(pobj);

	if (bModified)
	{
		// Update item in list, including icon in case it changed
		int nItem = m_lvw.GetSelectedItem();
		BObject* pobj = (BObject*) m_lvw.GetItemData(nItem);
		ASSERT_VALID(pobj);
		int nImage = pobj->GetIconIndex();
		m_lvw.SetItem(nItem, 0, LVIF_IMAGE, 0, nImage, 0, 0, 0);
		m_lvw.RedrawItems(nItem, nItem);
	}
	
	// Set focus back to list
	m_lvw.SetFocus();
}



void CDialogSelectProperty::OnBtnDelete() 
{
	CWaitCursor cw;

	// Get selected object, exit if none selected
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();	
	if (pobj == 0) return;
	ASSERT_VALID(pobj);

	if (m_pDoc->UIDeleteObject(pobj))
	{
		// Remember current position.
		int nItem = m_lvw.GetSelectedItem();

		// Remove item and any children from list. 
		m_lvw.DeleteSelectedItem();

		// Select next item, if any
		//, move to listview code!
		if (m_lvw.GetItemCount() > 0)
		{
			if (nItem > (m_lvw.GetItemCount() - 1))
				nItem--;
			m_lvw.SelectItem(nItem);
		}

		// should we remove the property from the selected class's list of properties also?
		// that will have already been done! right?

	}

	// Set focus back to list
	m_lvw.SetFocus();
}




void CDialogSelectProperty::OnSelChangeCboClass() 
{
	AddProperties();
}



void CDialogSelectProperty::AddProperties()
{
	CWaitCursor cw;

	// Set filter flag on properties
	m_pobjProperties->SetFlag(flagFilter, TRUE, TRUE);
	BObject* pobjClass = (BObject*) m_cboClass.GetSelectedItemData();
	if (pobjClass)
	{
		CObArray aPropertyDefs;
		int nProps = pobjClass->GetPropertyDefs(aPropertyDefs, FALSE, TRUE);
		for (int i = 0; i < nProps; i++)
		{
			BObject* pobjPropDef = (BObject*) aPropertyDefs.GetAt(i);
			ASSERT_VALID(pobjPropDef);
			// Clear this property's filter flag!
			pobjPropDef->SetFlag(flagFilter, FALSE, FALSE);
		}
	}
	
//	ULONG lngExcludeFlags = theApp.m_lngExcludeFlags | flagFilter;
	ULONG lngExcludeFlags = theApp.m_lngExcludeFlags;
	if (!m_bShowAll)
		lngExcludeFlags |= flagFilter;
	m_lvw.DeleteAllItems();
	m_lvw.AddObjects(m_pobjProperties, lngExcludeFlags);

	// Sort if showing all properties
	if (m_bShowAll)
		m_lvw.SortByProperty(propName, 1);

	// Clear all filter flags
	m_pobjProperties->SetFlag(flagFilter, FALSE, TRUE);

}


void CDialogSelectProperty::OnOptShowAll()
{
	m_bShowAll = m_optShowAll.GetCheck();
	m_cboClass.EnableWindow(FALSE);	
	AddProperties();

	// bug: wasn't selecting an item, which caused a bomb if you clicked OK
	// 1.1d select first item
	m_lvw.SelectItem(0);

	m_lvw.SetFocus();
}


void CDialogSelectProperty::OnOptShowSelected()
{
	m_bShowAll = m_optShowAll.GetCheck();
	m_cboClass.EnableWindow(TRUE);
	AddProperties();

	// bug: wasn't selecting an item, which caused a bomb if you clicked OK
	// 1.1d select first item
	m_lvw.SelectItem(0);

	m_cboClass.SetFocus();
}






void CDialogSelectProperty::OnOK() 
{
	// If clicked on a disabled property, let them know
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();	
	if (pobj)
	{
		// Bug: Was crashing if no property was selected because pobj was 0! Added a check
		if (pobj->m_lngFlags & flagDisabled)
		{
			AfxMessageBox("That property is already visible in the view.", MB_ICONINFORMATION);
			return;
		}
	}
	
	CDialog::OnOK();
}



// Close the dialog
void CDialogSelectProperty::OnBtnClose() 
{
	CDialog::EndDialog(0);
}


