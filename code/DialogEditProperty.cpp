
// CDialogEditProperty



#include "precompiled.h"

#include "NeoMem.h"
#include "DialogEditProperty.h"
#include "DialogNewLinkSource.h"
#include "Constants.h"

#include "ConstantsDatabase.h"
#include "NeoDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CDialogEditProperty, CDialog)
	//{{AFX_MSG_MAP(CDialogEditProperty)
	ON_CBN_SELCHANGE(IDC_CBO_TYPE, OnSelChangeCboType)
	ON_CBN_SELCHANGE(IDC_CBO_LINK_SOURCE, OnSelChangeCboLinkSource)
	ON_BN_CLICKED(IDC_BTN_NEW, OnBtnNewLinkSource)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	ON_BN_CLICKED(IDC_CHK_PROPERTY, OnChkAdditionalProperty)
	ON_BN_CLICKED(IDC_CHK_HIERARCHY, OnChkHierarchy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CDialogEditProperty::CDialogEditProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEditProperty::IDD, pParent),
	
	m_nMode(modeNone),
	m_pDoc(0),
	m_pobjPropertyDef(0),
	m_pobjPropertyType(0),
	m_pobjLinkSource(0),
	m_pobjAdditionalProperty(0),
	m_pobjUnits(0),

	m_bAdditionalProperty(FALSE),
	m_bDisplayHierarchy(FALSE),
	m_bLimitLinks(FALSE),
	m_bSystemProperty(FALSE)
	
{
	//{{AFX_DATA_INIT(CDialogEditProperty)
	m_strName = _T("");
	m_strDescription = _T("");
	//}}AFX_DATA_INIT
}



BOOL CDialogEditProperty::OnInitDialog() 
{
	ASSERT(m_nMode != modeNone); // must be set before calling dialog

	CWaitCursor wc;

	// Get some pointers
	m_pDoc = CNeoDoc::GetDoc();
	ASSERT_VALID(m_pDoc);

	// Call base class (required)
	CDialog::OnInitDialog();	

	// Set image lists
	CImageList* pImageList = m_pDoc->GetImageList();
	ASSERT_VALID(pImageList);
	m_cboType.SetImageList(pImageList);
	m_cboLinkSource.SetImageList(pImageList);
	m_cboAdditionalProperty.SetImageList(pImageList);
	m_cboUnits.SetImageList(pImageList);

	// Add property types to list
	// Note: Normal user gets limited list (eg just string, number, link, etc)
	BObject* pobjStart = m_pDoc->GetObject(folderPropertyTypes);
	m_cboType.AddObjects(pobjStart, theApp.m_lngExcludeFlags, FALSE, FALSE);

	// Modify caption and instructions depending on Edit/Add mode
	// bug:: Had = instead of ==
	CString strInstructions;
	if (m_nMode == modeAddProperty)
	{
		SetWindowText(_T("Add New Property"));
		strInstructions = "Enter the name and description and select the type for the new property.";
		// Select default property type (string)
		BObject* pobjDefault = m_pDoc->GetObject(proptypeString);
		m_cboType.SelectItemData((DWORD) pobjDefault);
//		m_cboLinkSource.SetCurSel(0);
//		m_cboUnits.SetCurSel(0);
	}
	else // edit property
	{
		SetWindowText(_T("Edit Property"));
		if (m_bSystemProperty)
			strInstructions = "Edit the name and description for this property. Other values cannot be changed because this is a system property.";
		else
			strInstructions = "Enter the name, description and select the type for the property. ";
		// Select property type
		ASSERT_VALID(m_pobjPropertyType);
		m_cboType.SelectItemData((DWORD) m_pobjPropertyType);
//		m_cboLinkSource.SelectItemData((DWORD) m_pobjLinkSource);
//		m_cboUnits.SelectItemData((DWORD) m_pobjUnits);
		//, Disable property type combo if we're editing a system property
		// this is the case if nodelete flag is set.
	}
	m_lblInstructions.SetWindowText(strInstructions);

	m_chkAdditionalProperty.SetCheck(m_bAdditionalProperty);
	m_chkDisplayHierarchy.SetCheck(m_bDisplayHierarchy);
	m_chkLimitLinks.SetCheck(m_bLimitLinks);

	// Enable/disable controls
	OnSelChangeCboType();

	// Disable controls if editing a system property
	if (m_bSystemProperty)
	{
		m_cboType.EnableWindow(FALSE);
		m_chkAdditionalProperty.EnableWindow(FALSE);
		m_cboAdditionalProperty.EnableWindow(FALSE);
		m_cboLinkSource.EnableWindow(FALSE);
		m_cboUnits.EnableWindow(FALSE);
		m_chkDisplayHierarchy.EnableWindow(FALSE);
		m_chkLimitLinks.EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialogEditProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditProperty)
	DDX_Control(pDX, IDC_TXT_TYPE_DESCRIPTION, m_txtTypeDescription);
	DDX_Control(pDX, IDC_CBO_TYPE, m_cboType);
	DDX_Control(pDX, IDC_CBO_LINK_SOURCE, m_cboLinkSource);
	DDX_Control(pDX, IDC_CBO_PROPERTY, m_cboAdditionalProperty);
	DDX_Control(pDX, IDC_CBO_UNITS, m_cboUnits);
	DDX_Control(pDX, IDC_CHK_PROPERTY, m_chkAdditionalProperty);
	DDX_Control(pDX, IDC_CHK_LIMIT, m_chkLimitLinks);
	DDX_Control(pDX, IDC_CHK_HIERARCHY, m_chkDisplayHierarchy);
	DDX_Control(pDX, IDC_LBL_PROPERTY, m_lblAdditionalProperty);
	DDX_Control(pDX, IDC_LBL_UNITS, m_lblUnits);
	DDX_Control(pDX, IDC_LBL_LINK, m_lblLinkSource);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	DDX_Control(pDX, IDC_TXT_DESCRIPTION, m_txtDescription);
	DDX_Control(pDX, IDC_TXT_NAME, m_txtName);
	DDX_Text(pDX, IDC_TXT_NAME, m_strName);
	DDX_Text(pDX, IDC_TXT_DESCRIPTION, m_strDescription);
	DDX_Control(pDX, IDC_BTN_NEW, m_btnNewLinkSource);
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelp);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) // Save
	{
		// Note: Name and description are handled automatically

		// Save property type
		m_pobjPropertyType = (BObject*) m_cboType.GetSelectedItemData();
		ASSERT_VALID(m_pobjPropertyType);

		// If link source combo is on, save it too
		if (m_cboLinkSource.IsWindowEnabled())
		{
			m_pobjLinkSource = (BObject*) m_cboLinkSource.GetSelectedItemData();
			ASSERT_VALID(m_pobjLinkSource);
			m_bLimitLinks = m_chkLimitLinks.GetCheck();
			m_bAdditionalProperty = m_chkAdditionalProperty.GetCheck();
			m_bDisplayHierarchy = m_chkDisplayHierarchy.GetCheck();
			if (m_bAdditionalProperty)
			{
				m_pobjAdditionalProperty = (BObject*) m_cboAdditionalProperty.GetSelectedItemData();
				ASSERT_VALID(m_pobjAdditionalProperty);
			}
			else
				m_pobjAdditionalProperty = 0;
		}
		else
		{
			m_pobjLinkSource = 0;
			m_pobjAdditionalProperty = 0;
			m_bAdditionalProperty = FALSE;
			m_bDisplayHierarchy = FALSE;
			m_bLimitLinks = FALSE;
		}

		// If units combo is on, save it also
//		if (m_cboUnits.IsWindowEnabled())
//		{
//			m_pobjUnits = (BObject*) m_cboUnits.GetSelectedItemData();
//			ASSERT_VALID(m_pobjUnits);
//		}
//		else
//			m_pobjUnits = 0;
	}
}




// User selected a different link source
void CDialogEditProperty::OnSelChangeCboLinkSource() 
{
	// Remember the selected link source
	m_pobjLinkSource = (BObject*) m_cboLinkSource.GetSelectedItemData();
}



// Add a new property. Returns True if user hit OK.
BOOL CDialogEditProperty::DoModalAdd()
{
	m_nMode = CDialogEditProperty::modeAddProperty;
	m_strName = _T("New Property");
	m_strDescription.Empty();

	if (DoModal() == IDOK) 
		return TRUE;
	return FALSE;
}


//, Edit an existing property
int CDialogEditProperty::DoModalEdit(LPCTSTR strName, LPCTSTR strDescription)
{
	return 0;
}


// Add a new link source folder to the Lookup folder
void CDialogEditProperty::OnBtnNewLinkSource()
{
	// need to:
	// enter name, 
	// enter description,
	// select class (usu folder), 
	// select location (usu references), 
	// select icon (?),
	// select contents class (might be a new class - ugh)
	// sounds like a wizard almost

	// for now, could have some simplified version that would make some assumptions - 
	// ie if you're adding a new link source then you probably want a new class also,
	// and you'll add it as a folder in the references section,
	// so all you'd need to enter is the name for the new class, the name for the new folder (usu plural of class),
	// and code would handle the rest.
	// this would at least get things set up, and you could change the class or folder later if you wanted
	// so when they click this button we need a dialog to let them enter that stuff,
	// and describe what the program will do with it.

	CDialogNewLinkSource dlg;
	if (dlg.DoModal() == IDOK)
	{
		ULONG lngClassID = dlg.m_lngClassID;
		CString strFolderName = dlg.m_strFolderName;

		// Add the new folder and set its properties
//		BObject* pobjParent = m_pDoc->GetObject(folderLookup); // add to lookup folder
		BObject* pobjParent = m_pDoc->GetObject(rootUser); // add to home folder
		HOBJECT hobjNewFolder = m_pDoc->CreateObject(classFolder, strFolderName, pobjParent);
		BObject* pobjClass = m_pDoc->GetObject(lngClassID);
		hobjNewFolder->SetPropertyLink(propDefaultClass, pobjClass, FALSE, FALSE);

		// Tell views about new folder
//		m_pDoc->UpdateAllViewsEx(NULL, hintAdd, pobjNewFolder);
		m_pDoc->AddObject(hobjNewFolder);

		// Add the new folder to the combo and select it
		int nIndex = m_cboLinkSource.AddObject(hobjNewFolder, pobjParent);
		m_cboLinkSource.SetCurSel(nIndex);
	}

	// Set focus
	m_cboLinkSource.SetFocus();

}






// Bring up help for this dialog
void CDialogEditProperty::OnBtnHelp() 
{
	theApp.WinHelp(HID_BASE_RESOURCE + IDD_EDIT_PROPERTY);
}



// Fill link source combo if empty, limiting to folders and home objects
void CDialogEditProperty::FillLinkSourceCombo()
{
	if (m_cboLinkSource.GetCount() == 0)
	{
		CWaitCursor cw;
//		BDataLinks datValidClasses;
//		datValidClasses.AddObject(classFolder, m_pDoc);
//		datValidClasses.AddObject(classHome, m_pDoc);
		BDataLink datValidClasses;
		datValidClasses.SetMultiple();
		datValidClasses.AddLinkID(classFolder, m_pDoc);
		datValidClasses.AddLinkID(classHome, m_pDoc);
		BObject* pobjStart = m_pDoc->GetObject(rootMain);
		ASSERT_VALID(pobjStart);
		BOOL bIncludeStart = theApp.m_bAdmin ? TRUE : FALSE;
		m_cboLinkSource.AddObjects(pobjStart, theApp.m_lngExcludeFlags, bIncludeStart, TRUE, 0, &datValidClasses);
	}
}



void CDialogEditProperty::FillPropertyCombo()
{
	if (m_cboAdditionalProperty.GetCount() == 0)
	{
		CWaitCursor cw;
		// Add "None" as first item, using the icon for any propertydef.
//		BObject* pobjProp = m_pDoc->GetObject(propObjectProperties);
//		int nImage = pobjProp->GetIconIndex();
//		m_cboProperty.AddNone(nImage);

		// Add propertydefs
		BObject* pobjStart = m_pDoc->GetObject(folderProperties);
		m_cboAdditionalProperty.AddObjects(pobjStart, theApp.m_lngExcludeFlags, FALSE, FALSE, 0, 0);
	}
}



// Fill units combo
void CDialogEditProperty::FillUnitsCombo()
{
	if (m_cboUnits.GetCount() == 0)
	{
		CWaitCursor cw;
		BObject* pobjStart = m_pDoc->GetObject(folderUnits);
		m_cboUnits.AddObjects(pobjStart, theApp.m_lngExcludeFlags, FALSE, TRUE);
	}
}





// New property type selected, so enable/disable controls based on selection.
void CDialogEditProperty::OnSelChangeCboType() 
{
	// Get selected type
	BObject* pobjType = (BObject*) m_cboType.GetSelectedItemData();
	if (pobjType)
	{
		ASSERT_VALID(pobjType);
		ULONG lngTypeID = pobjType->GetObjectID();

		// Get type description
		m_txtTypeDescription.SetWindowText(pobjType->GetPropertyText(propDescription));

		// Enable/disable various controls based on selection
//		BOOL bLinkSource = (lngTypeID == proptypeLink || lngTypeID == proptypeLinks);
		BOOL bLinkSource = (lngTypeID == proptypeLink);

		m_lblLinkSource.EnableWindow(bLinkSource);
		m_cboLinkSource.EnableWindow(bLinkSource);
		m_btnNewLinkSource.EnableWindow(bLinkSource);
		m_chkLimitLinks.EnableWindow(bLinkSource);
		m_chkDisplayHierarchy.EnableWindow(bLinkSource);
		m_chkAdditionalProperty.EnableWindow(bLinkSource);

		// Link Source
		if (bLinkSource)
		{
			FillLinkSourceCombo();
			m_cboLinkSource.SelectItemData((DWORD) m_pobjLinkSource);
		}
		else
		{
			m_cboLinkSource.SelectNone();
		}

		// Additional Property
		BOOL bAdditionalProperty = m_chkAdditionalProperty.GetCheck();
		m_lblAdditionalProperty.EnableWindow(bLinkSource && bAdditionalProperty);
		m_cboAdditionalProperty.EnableWindow(bLinkSource && bAdditionalProperty);
		if (bLinkSource && bAdditionalProperty)
		{
			FillPropertyCombo();
			m_cboAdditionalProperty.SelectItemData((DWORD) m_pobjAdditionalProperty);
		}
		else
		{
			m_cboAdditionalProperty.SelectNone();
		}

		// Units
//		FillUnitsCombo();
//		BOOL bUnits = (lngTypeID == proptypeTimeInterval);
//		m_lblUnits.EnableWindow(bUnits);
//		m_cboUnits.EnableWindow(bUnits);
//		if (bUnits)
//			m_cboUnits.SelectItemData((DWORD) m_pobjUnits);
//		else
//			m_cboUnits.SelectNone();
	}
}





void CDialogEditProperty::OnOK()
{
	// Give warning message to user if they changed the property type of an existing property.
	if (m_nMode == modeEditProperty)
	{
		BObject* pobjPropertyType = (BObject*) m_cboType.GetSelectedItemData();
		if (pobjPropertyType != m_pobjPropertyType)
		{
			// Check if this property type is actually used anywhere
			CObArray aRefs;
			int nRefs = m_pDoc->FindReferences(m_pobjPropertyDef, aRefs);
			if (nRefs > 0)
			{
				if (IDNO == AfxMessageBox("Warning: You have selected a new property type for an existing property. "
					"NeoMem will attempt to convert all existing property values to the new type, though this might not always be successful. "
					"Is this what you want?", MB_ICONQUESTION + MB_YESNO))
				{
					return;
				}
			}
		}
	}
	CDialog::OnOK();
}


void CDialogEditProperty::OnChkHierarchy() 
{

}


void CDialogEditProperty::OnChkAdditionalProperty() 
{
	OnSelChangeCboType();
}



