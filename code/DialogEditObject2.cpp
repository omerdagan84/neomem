
// CDialogEditObject2



#include "precompiled.h"

#include "DialogEditObject2.h"
#include "NeoMem.h"

#include "BDataColumns.h"
#include "ConstantsDatabase.h"
#include "NeoDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




BEGIN_MESSAGE_MAP(CDialogEditObject2, CDialog)
	//{{AFX_MSG_MAP(CDialogEditObject2)
	ON_EN_CHANGE(IDC_TXT_NAME, OnChangeEditName)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LVW_CLASS, OnNotifyItemChanged)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	ON_CBN_DROPDOWN(IDC_CBO_LOCATION, OnDropdownCboLocation)
	ON_BN_CLICKED(IDC_BTN_NEW_CLASS, OnBtnNewClass)
	ON_BN_CLICKED(IDC_BTN_NEW_CLASS2, OnBtnNewClass2)
	ON_CBN_SELCHANGE(IDC_CBO_LOCATION, OnSelchangeCboLocation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CDialogEditObject2::CDialogEditObject2(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEditObject2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEditObject2)
	//}}AFX_DATA_INIT
	m_lngClassID = 0;
	m_pobjParent = NULL;
	m_pobjClass = NULL;
	m_pobjDefaultClass = NULL;
	m_bNameChanged = FALSE;
	m_bLocationComboFilled = FALSE;
	m_bAddMode = FALSE;
}


CDialogEditObject2::~CDialogEditObject2()
{
}



// You typically override OnInitDialog to initialize the dialog box’s controls, such as 
// setting the initial text of an edit box. You must call the OnInitDialog member function 
// of the base class, CDialog, from your OnInitDialog override. 
BOOL CDialogEditObject2::OnInitDialog() 
{	
	// Call base class (required)
	// This calls DoDataExchange, so we put this AFTER setting m_nClassIndex
	// to the specified ClassID, so that the framework will select the correct class
	// in the combo for us.
	// The framework automatically calls UpdateData with bSaveAndValidate set to FALSE when 
	// a modal dialog box is created in the default implementation of CDialog::OnInitDialog. 
	CDialog::OnInitDialog();

	// Get document
	m_pDoc = CNeoDoc::GetDoc();
	ASSERT_VALID(m_pDoc);

	// Initialize listview
	m_lvw.m_pDoc = m_pDoc;
	m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
	m_lvw.SetFullRowSelect(TRUE);
	m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsClassFolder), TRUE, TRUE); // class folder columns

	// Add classes to list
	BObject* pobjStart = m_pDoc->GetObject(rootClass);
	ASSERT_VALID(pobjStart);
	m_lvw.AddObjects(pobjStart, theApp.m_lngExcludeFlags, FALSE, TRUE);
	m_lvw.SortByProperty(propName, 1);

	// Select class if specified
	if (m_pobjClass)
	{
		ASSERT_VALID(m_pobjClass);
		m_lvw.SelectItemData((LPARAM) m_pobjClass);
	}

	// Initialize default class controls
	m_cboDefaultClass.SetImageList(m_pDoc->GetImageList());
	OnClassChanged();

	// Display location in combobox (will fill all items on dropdown event)
	m_cboLocation.SetImageList(m_pDoc->GetImageList());
	ASSERT_VALID(m_pobjParent);
	m_cboLocation.AddObject(m_pobjParent);
	m_cboLocation.SetCurSel(0);

	// Disable location combo if in edit mode (don't let user change location that way for now)
	if (m_bAddMode == FALSE)
		m_cboLocation.EnableWindow(FALSE);

	// Select text in name textbox
	m_txtName.SetSel(0, -1);

	// Change labels based on add/edit mode
	if (m_bAddMode == FALSE)
	{
		m_lblClass.SetWindowText("&Class of object");
		m_lblLocation.SetWindowText("&Location of object");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void CDialogEditObject2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDialogEditObject2)
	DDX_Control(pDX, IDC_BTN_NEW_CLASS2, m_btnNewClass2);
	DDX_Control(pDX, IDC_BTN_NEW_CLASS, m_btnNewClass);
	DDX_Control(pDX, IDC_LBL_CLASS, m_lblClass);
	DDX_Control(pDX, IDC_LBL_LOCATION, m_lblLocation);
	DDX_Control(pDX, IDC_CBO_LOCATION, m_cboLocation);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	DDX_Control(pDX, IDC_TXT_NAME, m_txtName);
	DDX_Text(pDX, IDC_TXT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 254);
	DDX_Control(pDX, IDC_TXT_DESCRIPTION, m_txtDescription);
	DDX_Control(pDX, IDC_LVW_CLASS, m_lvw);
	DDX_Control(pDX, IDC_LBL_DEFAULT_CLASS, m_lblDefaultClass);
	DDX_Control(pDX, IDC_CBO_DEFAULT_CLASS, m_cboDefaultClass);
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelp);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) // Load
	{
	}
	else // Save
	{
		// Get class
		m_pobjClass = (BObject*) m_lvw.GetSelectedItemData();
		if (m_pobjClass)
		{
			ASSERT_VALID(m_pobjClass);
			m_lngClassID = m_pobjClass->GetObjectID();
		}

		// Get default class
		m_pobjDefaultClass = (BObject*) m_cboDefaultClass.GetSelectedItemData();
		if (m_pobjDefaultClass)
		{
			ASSERT_VALID(m_pobjDefaultClass);
		}

		// Get location (parent)
		m_pobjParent  = (BObject*) m_cboLocation.GetSelectedItemData();
		if (m_pobjParent)
		{
			ASSERT_VALID(m_pobjParent);
		}
	}
}


//----------------------------------------------------------------------------------------------------------------------



// Add a new object
// Output: m_lngClassID, m_pobjDefaultClass
int CDialogEditObject2::DoModalAddObject(CString &strName, BObject* pobjParent, BObject* pobjClass, BObject* pobjDefaultClass)
{
	m_strName = strName;
	m_pobjParent = pobjParent;
	m_pobjClass = pobjClass;
	m_pobjDefaultClass = pobjDefaultClass;
	m_bAddMode = TRUE;
	return DoModal();
}



// Edit an existing object
int CDialogEditObject2::DoModalEditObject(CString &strName, BObject* pobjParent, BObject *pobjClass, BObject *pobjDefaultClass)
{
	m_strName = strName;
	m_pobjParent = pobjParent;
	m_pobjClass = pobjClass;
	m_pobjDefaultClass = pobjDefaultClass;
	m_bNameChanged = TRUE; // prevent "New <classname>"
	m_bAddMode = FALSE;
	return DoModal();
}


//----------------------------------------------------------------------------------------------------------------------


// If user selects a different class, we should change the text, if user has not changed it
void CDialogEditObject2::OnChangeEditName() 
{
	// Set flag
	m_bNameChanged = TRUE;
}


// Class selection changed
void CDialogEditObject2::OnNotifyItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pnmlv = (NMLISTVIEW*)pNMHDR;
	// If this message is regarding a new item being selected, handle it
	if (pnmlv->uNewState & LVIS_SELECTED)
		OnClassChanged();
	*pResult = 0;
}



// Fill default class combo with list of classes
void CDialogEditObject2::FillDefaultClassCombo()
{
	if (m_cboDefaultClass.GetCount() == 0)
	{
		CWaitCursor cw;
		BObject* pobjStart = m_pDoc->GetObject(rootClass);
		m_cboDefaultClass.AddObjects(pobjStart, theApp.m_lngExcludeFlags, FALSE, TRUE);
	}
}



// Class selection changed
void CDialogEditObject2::OnClassChanged()
{
	BObject* pobjClass = (BObject*) m_lvw.GetSelectedItemData();
	if (pobjClass)
	{
		ASSERT_VALID(pobjClass);

		// If text has not changed then update name to New <classname>
		if (!m_bNameChanged)
		{
			CString strName;
			pobjClass->GetClassDefNewName(strName);
			m_txtName.SetWindowText(strName);
			m_bNameChanged = FALSE;
		}
		
		// Update class description label also
		LPCTSTR pszClassDescription = pobjClass->GetPropertyText(propDescription);
		m_txtDescription.SetWindowText(pszClassDescription);

		// Enable default class controls if Folder selected
		BOOL bFolder = (pobjClass->GetObjectID() == classFolder);
		m_lblDefaultClass.EnableWindow(bFolder);
		m_cboDefaultClass.EnableWindow(bFolder);
		m_btnNewClass2.EnableWindow(bFolder);
		if (bFolder)
		{
			FillDefaultClassCombo();
			// By default, folders will always contain paper objects.
			//.... this is a 'business' rule and should not be in the ui!!
//			// Get default class from parent folder (if parent is a folder).
//			// This way, if you're adding a subfolder to a Book folder, the new folder
//			// would also store Books by default.
//			BObject* pobjClass = m_pobjParent->GetPropertyLink(propDefaultClass);
			BObject* pobjClass = m_pDoc->GetObject(classPaper);
			m_cboDefaultClass.SelectItemData((DWORD) pobjClass);
		}
		else
		{
			m_cboDefaultClass.SelectNone();
		}
	}	
}



// Bring up help for this dialog
void CDialogEditObject2::OnBtnHelp() 
{
	theApp.WinHelp(HID_BASE_RESOURCE + IDD_EDIT_OBJECT);
}



// User is trying to view contents of the location combo - so fill it if it hasn't already been filled.
// We do this because the combo could potentially have thousands of items in it and
// don't want to have to load that on dialog startup.
void CDialogEditObject2::OnDropdownCboLocation() 
{
	if (m_bLocationComboFilled == FALSE)
	{
		CWaitCursor cw;

		// First remove the one that's already there
		m_cboLocation.DeleteItem(0);

		// Now add all user objects and reselect the original one, if specified
		BObject* pobjStart = m_pDoc->GetObject(rootUser);
		m_cboLocation.AddObjects(pobjStart, theApp.m_lngExcludeFlags, TRUE, TRUE);
		if (m_pobjParent)
			m_cboLocation.SelectItemData((DWORD) m_pobjParent);

		// Set flag
		m_bLocationComboFilled = TRUE;
	}
}





// User selected a new location - change the class accordingly (based on parent's default class).
void CDialogEditObject2::OnSelchangeCboLocation() 
{
	BObject* pobjParent = (BObject*) m_cboLocation.GetSelectedItemData();
	if (pobjParent)
	{
		BObject* pobjClass = pobjParent->GetPropertyLink(propDefaultClass);
		ASSERT_VALID(pobjClass);
		// This will trigger listview selchange event
		m_lvw.SelectItemData((LPARAM) pobjClass);
	}
}




// Add a new class
void CDialogEditObject2::OnBtnNewClass() 
{
	BObject* pobjNew = AddNewClass();
	m_lvw.SelectItemData((LPARAM) pobjNew);
	m_lvw.SetFocus(); // Set focus back to list
}


// Add a new class
void CDialogEditObject2::OnBtnNewClass2() 
{
	BObject* pobjNew = AddNewClass();
	m_cboDefaultClass.SelectItemData((LPARAM) pobjNew);
	m_cboDefaultClass.SetFocus(); // Set focus back to cbo
}



// Add a new class and add it to all class controls in this dialog.
BObject* CDialogEditObject2::AddNewClass()
{
	BObject* pobjNew = m_pDoc->UIAddNewClass();
	if (pobjNew)
	{
		ASSERT_VALID(pobjNew);
		m_lvw.AddObject(pobjNew);
		m_cboDefaultClass.AddObject(pobjNew);
	}
	return pobjNew;
}



