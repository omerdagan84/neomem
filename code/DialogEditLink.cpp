
// CDialogEditLink

// selected items are obtained by a call to m_lvw.GetSelectedItems(m_pdatLink);



#include "precompiled.h"

#include "DialogEditLink.h"
#include "NeoMem.h"
#include "Strings.h"
//#include "help\HelpIDs.h"

#include "BDataColumns.h"
#include "BDataIcon.h"
#include "BDataLink.h"
#include "ConstantsDatabase.h"
#include "NeoDoc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





BEGIN_MESSAGE_MAP(CDialogEditLink, CDialog)
	//{{AFX_MSG_MAP(CDialogEditLink)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_USE_DEFAULT, OnBtnUseDefault)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBtnEdit)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_CHK_MULTIPLE, OnChkMultiple)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	ON_BN_CLICKED(IDC_CHK_HIERARCHY, OnChkHierarchy)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ADD_CHILD, OnBtnAddChild)
	ON_BN_CLICKED(IDC_CHK_FILTER, OnChkFilter)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CDialogEditLink dialog
//---------------------------------------------------------------------------

CDialogEditLink::CDialogEditLink(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEditLink::IDD, pParent)
{
	// Get document (used by domodal methods also)
	m_pDoc = CNeoDoc::GetDoc();
	ASSERT_VALID(m_pDoc);

	// Initialize all values so we'll be able to see if user forgot to supply one.
//	m_nMode = -1;
	m_bHierarchy = TRUE;
	m_bIncludeStart = FALSE;
	m_bMultiSelectEnabled = FALSE;
	m_bMultiSelectOn = FALSE;
	m_bMultiSelectVisible = FALSE;
	m_lngButtonFlags = flagShowNone; // show none by default
	m_lngDefaultClassID = 0;
	m_lngDefaultID = 0;
	m_lngExcludeFlags = 0;
	m_lngSelectedID = 0;
	m_lngStartID = 0;
	m_pdatLink = 0;
	m_pobjSelected = 0;
	m_pobjStart = 0;

	m_bFilterVisible = FALSE;
	m_bFilterOn = FALSE;
	m_lngFilterExcludeFlags = 0;

	m_nHelpID = IDD_EDIT_LINK; // default

	//{{AFX_DATA_INIT(CDialogEditLink)
	//}}AFX_DATA_INIT
}





int CDialogEditLink::DoModal() 
{
	// Set up initial values based on mode specified, make sure user supplied all that
	// are necessary.
	return CDialog::DoModal();
}




// Bring up dialog to let user select a single object.
// On return, m_lngSelectedID and m_pobjSelected will contain the selected object.
// Return value is IDOK if user hit OK.
int CDialogEditLink::DoModalLinkSimple(CString strCaption, CString strInstructions, ULONG lngStartID, 
										ULONG lngSelectedID /* = 0 */, ULONG lngDefaultID /* = 0 */, 
										ULONG lngExcludeFlags /* = 0 */, BOOL bIncludeStart /* = FALSE */)
{
	ASSERT_VALID(m_pDoc);

	m_strCaption = strCaption;
	m_strInstructions = strInstructions;
	m_lngStartID = lngStartID;
	m_lngSelectedID = lngSelectedID;
	m_lngDefaultID = lngDefaultID;
	m_lngExcludeFlags = lngExcludeFlags;
	m_bIncludeStart = bIncludeStart;
//	m_lngButtonFlags = flagShowAdd | flagShowEdit | flagShowDelete;
	m_lngButtonFlags = flagShowAdd | flagShowAddChild | flagShowEdit | flagShowDelete;

	m_bMultiSelectVisible = FALSE;
	m_bMultiSelectEnabled = FALSE;
	m_bMultiSelectOn = FALSE;

	// Create a bdatalink object to store link info
	BDataLink* pdatLink = new BDataLink;
	ASSERT_VALID(pdatLink);
	if (lngSelectedID)
	{
		BObject* pobj = m_pDoc->GetObject(lngSelectedID);
		ASSERT_VALID(pobj);
		pdatLink->SetLink(pobj);
	}
	m_pdatLink = pdatLink;

	int nRet = CDialog::DoModal();

	// Get the link object
	m_pobjSelected = pdatLink->GetLink();
	m_lngSelectedID = pdatLink->GetLinkObjectID();

	// Delete our bdatalink object
	delete pdatLink;

	return nRet;
}




// Bring up dialog in multiselect mode.
// Return value is IDOK if user hit OK.
int CDialogEditLink::DoModalLink(CString strCaption, CString strInstructions, BOOL bMultiSelectVisible, 
								 BOOL bMultiSelectEnabled, BOOL bMultiSelectOn, ULONG lngStartID, 
								 BDataLink* pdatLink, ULONG lngExcludeFlags /* = 0 */, 
								 BOOL bIncludeStart /* = FALSE */)
{
	ASSERT_VALID(pdatLink);

	m_strCaption = strCaption;
	m_strInstructions = strInstructions;
	m_lngStartID = lngStartID;
	m_pdatLink = pdatLink;
	m_lngExcludeFlags = lngExcludeFlags;
	m_bIncludeStart = bIncludeStart;

//	m_lngButtonFlags = flagShowAdd | flagShowEdit | flagShowDelete;
	m_lngButtonFlags = flagShowAdd | flagShowAddChild | flagShowEdit | flagShowDelete;

	// depends on property def that we're editing
//	m_bMultiSelectVisible = TRUE;
//	m_bMultiSelectEnabled = TRUE;
//	m_bMultiSelectOn = pdatLink->IsMultiple();
	m_bMultiSelectVisible = bMultiSelectVisible;
	m_bMultiSelectEnabled = bMultiSelectEnabled;
	m_bMultiSelectOn = bMultiSelectOn;
//,	m_bChangeOrder = bChangeOrder;

	return CDialog::DoModal();
}



// Bring up dialog in editing mode, which lets user add, edit, or delete objects, (eg for Properties).
int CDialogEditLink::DoModalEdit(CString strCaption, CString strInstructions, ULONG lngStartID, 
								 ULONG lngExcludeFlags /* = 0 */, BOOL bIncludeStart /* = FALSE */)
{
	m_strCaption = strCaption;
	m_strInstructions = strInstructions;
	m_lngStartID = lngStartID;
	m_lngSelectedID = 0;
	m_lngDefaultID = 0;
	m_lngExcludeFlags = lngExcludeFlags;
	m_bIncludeStart = bIncludeStart;
//	m_lngButtonFlags = flagShowAdd | flagShowEdit | flagShowDelete | flagShowClose;
	m_lngButtonFlags = flagShowAdd | flagShowAddChild | flagShowEdit | flagShowDelete | flagShowClose;
	m_pdatLink = 0; // don't need to select anything

	m_bMultiSelectVisible = FALSE;
	m_bMultiSelectEnabled = FALSE;
	m_bMultiSelectOn = FALSE;
	
	return CDialog::DoModal();
}




// select a single object
int CDialogEditLink::DoModalSelect(CString strCaption, CString strInstructions, ULONG lngStartID, 
										ULONG lngSelectedID /* = 0 */, ULONG lngDefaultID /* = 0 */, 
										ULONG lngExcludeFlags /* = 0 */, BOOL bIncludeStart /* = FALSE */)
{
	m_strCaption = strCaption;
	m_strInstructions = strInstructions;
	m_lngStartID = lngStartID;
	m_lngSelectedID = 0;
	m_lngDefaultID = 0;
	m_lngExcludeFlags = lngExcludeFlags;
	m_bIncludeStart = bIncludeStart;
	m_lngButtonFlags = 0; // don't show any buttons
	m_pdatLink = 0; // don't need to select anything

	m_bMultiSelectVisible = FALSE;
	m_bMultiSelectEnabled = FALSE;
	m_bMultiSelectOn = FALSE;
	
	// Create a bdatalink object to store link info
	BDataLink* pdatLink = new BDataLink;
	ASSERT_VALID(pdatLink);
	if (lngSelectedID)
	{
		BObject* pobj = m_pDoc->GetObject(lngSelectedID);
		ASSERT_VALID(pobj);
		pdatLink->SetLink(pobj);
	}
	m_pdatLink = pdatLink;

	int nRet = CDialog::DoModal();

	// Get the link object
	m_pobjSelected = pdatLink->GetLink();
	m_lngSelectedID = pdatLink->GetLinkObjectID();

	// Delete our bdatalink object
	delete pdatLink;

	return nRet;
}




// You typically override OnInitDialog to initialize the dialog box’s controls, such as 
// setting the initial text of an edit box. You must call the OnInitDialog member function 
// of the base class, CDialog, from your OnInitDialog override. 
BOOL CDialogEditLink::OnInitDialog() 
{
	// Call base class (calls DoDataExchange)
	CDialog::OnInitDialog();

	// Get start object
	m_pobjStart = m_pDoc->GetObject(m_lngStartID);
	ASSERT_VALID(m_pobjStart);

	// Initialize listview
	// BUG:: Copied from treeview so had TVSIL_NORMAL instead of LVSIL_SMALL so no icons would show up!
	m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
	m_lvw.m_pDoc = m_pDoc;
	m_lvw.SetFullRowSelect(TRUE);

	// Add columns, getting column information from object or from object's classdef.
	// Note: If we're adding the Class folder, Property folder, or Icon folder, use the special
	// BDataColumns objects stored in the document. This is so that the admin column
	// arrangements will be left as is for the admin user. 
	m_bHierarchy = FALSE; // show all objects in flat list, even if recursive
	if (m_lngStartID == rootClass)
		// Showing Class folder
		m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsClassFolder), TRUE, TRUE);
	else if (m_lngStartID == folderProperties)
		// Showing Properties folder
		m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsPropertyFolder), TRUE, TRUE);
	else if (m_lngStartID == folderIcons)
		// Showing Icons folder
		m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsIconFolder), TRUE, TRUE);
	else if (m_lngStartID == rootUser)
	{
		// Showing Root object
		// Special handling for root object - just show Name, Class, Description.
		// This is used in MoveTo handling for instance.
//		BDataColumns* pdatColumns = new BDataColumns;
//		pdatColumns->InsertColumn(propName, m_pDoc, 200);
//		pdatColumns->InsertColumn(propClassName, m_pDoc, 200);
//		ASSERT_VALID(pdatColumns);
//		m_lvw.InitializeColumns(pdatColumns, TRUE, FALSE);
		m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsDefault), TRUE, TRUE);
		m_bHierarchy = TRUE; // show in tree form, sorting children alphabetically
	}
	else
	{
		// Showing any other folder
		BDataColumns* pdatColumns = DYNAMIC_DOWNCAST(BDataColumns, m_pobjStart->GetPropertyData(propColumnInfoArray));
		ASSERT_VALID(pdatColumns);
		m_lvw.InitializeColumns(pdatColumns);
		m_bHierarchy = TRUE; // show in tree form, sorting children alphabetically
	}
	

	// Add objects to list and sort if necessary
	AddObjects();

	// Get default class to determine which dialogs to use, etc.
	ASSERT_VALID(m_pobjStart);
	BObject* pobjClass = m_pobjStart->GetPropertyLink(propDefaultClass);
	ASSERT_VALID(pobjClass);
	m_lngDefaultClassID = pobjClass->GetObjectID();

	// Show buttons, etc.
	UpdateControls();

	// Set state of checkbox and listview
	m_chkMultiple.SetCheck(m_bMultiSelectOn ? 1 : 0);
	OnChkMultiple(); // this will set listview mode to single or multi select

	//, Now order the currently selected items (as long as we're not in hierarchy (tree) mode). 
	//, only in admin mode for now
	if (theApp.m_bAdmin && m_bMultiSelectOn)
	{
		CObArray* pa = m_pdatLink->GetLinkArray();
		int nItems = pa->GetSize();
		for (int i = 0; i < nItems; i++)
		{
			// find in list and move to top
			BObject* pobj = (BObject*) pa->GetAt(i);
			int nRow = m_lvw.FindItemData((LPARAM) pobj);
			m_lvw.MoveItemToSibling(nRow, i, FALSE);
		}
	}

	// Select item or items	
	// Note: If pdatlink is 0, will just select first item in list
	m_lvw.SetSelectedItems(m_pdatLink);

	// Show up and down buttons 
	//, only in admin mode for now
	if (theApp.m_bAdmin)
	{
		m_btnUp.ShowWindow(SW_SHOW);
		m_btnDown.ShowWindow(SW_SHOW);
		m_bmpUp.LoadBitmap(IDB_ARROW_UP);
		m_bmpDown.LoadBitmap(IDB_ARROW_DOWN);
		// An application sends a BM_SETIMAGE message to associate a new image (icon or bitmap) 
		// with the button. Use IMAGE_BITMAP or IMAGE_ICON, and pass handle of bitmap or icon as lparam. 
		m_btnUp.SendMessage(BM_SETIMAGE, IMAGE_BITMAP, (LPARAM) ((HBITMAP) m_bmpUp));
		m_btnDown.SendMessage(BM_SETIMAGE, IMAGE_BITMAP, (LPARAM) ((HBITMAP) m_bmpDown));
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialogEditLink::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditLink)
	DDX_Control(pDX, IDC_CHK_FILTER, m_chkFilter);
	DDX_Control(pDX, IDC_BTN_ADD_CHILD, m_btnAddChild);
	DDX_Control(pDX, IDC_CHK_HIERARCHY, m_chkHierarchy);
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_BTN_ARRANGE, m_btnArrange);
	DDX_Control(pDX, IDC_BTN_UP, m_btnUp);
	DDX_Control(pDX, IDC_CHK_MULTIPLE, m_chkMultiple);
	DDX_Control(pDX, IDC_LVW, m_lvw);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BTN_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BTN_USE_DEFAULT, m_btnUseDefault);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	//}}AFX_DATA_MAP

	// This is where we save the data to our BDataLink object!
	if (pDX->m_bSaveAndValidate) // Save
	{
		if (m_pdatLink)
			m_lvw.GetSelectedItems(m_pdatLink);
	}
}






// Edit the selected object in an appropriate dialog
void CDialogEditLink::OnBtnEdit() 
{
	CWaitCursor cw;

	// Get class of object to determine which dialog to use
	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();	
	if (!pobj) return;
	ASSERT_VALID(pobj);
	ULONG lngClassID = pobj->GetClassID();

	BOOL bModified = FALSE;
	switch (lngClassID)
	{
	case classProperty:
		// Edit the property def in the edit property dialog
		bModified = m_pDoc->UIEditPropertyDef(pobj);
		break;
	case classClass:
		// Edit the class using the class wizard
		//, limit to edit!
		bModified = m_pDoc->UIEditClass(pobj);
		break;
	case classIcon:
		// Edit the icon name
		bModified = m_pDoc->UIEditObject(pobj);
		break;
	default:
		// Edit the object name, description, class, and default class
		bModified = m_pDoc->UIEditObject(pobj);
		break;
	}

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


// Delete the selected object
void CDialogEditLink::OnBtnDelete() 
{
	CWaitCursor cw;

	BObject* pobj = (BObject*) m_lvw.GetSelectedItemData();	
	if (!pobj) return;
	ASSERT_VALID(pobj);
	if (m_pDoc->UIDeleteObject(pobj))
	{
		// Remember current position
		int nItem = m_lvw.GetSelectedItem();

		// Remove item from list also
		// also remove any children it has - listview won't do this automatically (unlike treeview)
		m_lvw.DeleteSelectedItem();

		// Select next item, if any
		if (m_lvw.GetItemCount() > 0)
		{
			if (nItem > (m_lvw.GetItemCount() - 1))
				nItem--;
			m_lvw.SelectItem(nItem);
		}
	}

	// Set focus back to list
	m_lvw.SetFocus();
}



// Select the default value
void CDialogEditLink::OnBtnUseDefault() 
{
	if (m_lngDefaultID)
	{
		BObject* pobj = m_pDoc->GetObject(m_lngDefaultID);
		m_lvw.SelectItemData((DWORD) pobj);
	}

	// Set focus back to list
	m_lvw.SetFocus();
}



// Close the dialog without saving anything
void CDialogEditLink::OnBtnClose() 
{
	CDialog::EndDialog(0);
}





// Modify mode of listview to single or multiselect, depending on check box setting.
// Note: Listview handles checking off selected item or removing checks when switching.
void CDialogEditLink::OnChkMultiple() 
{
	m_bMultiSelectOn = m_chkMultiple.GetCheck();
	m_lvw.SetCheckboxes(m_bMultiSelectOn);
}






// Show or hide buttons based on input flags, reposition controls, etc.
// Note: Buttons are invisible at beginning.
void CDialogEditLink::UpdateControls()
{
	// Set caption and instructions if specified
	if (!m_strCaption.IsEmpty())
		SetWindowText(m_strCaption);
	if (!m_strInstructions.IsEmpty())
		m_lblInstructions.SetWindowText(m_strInstructions);

	// Change Add to Import if default class is icon!
	if (m_lngDefaultClassID == classIcon)
		m_btnAdd.SetWindowText("&Import...");

	// Hide checkboxes if necessary, and move listview to cover then if hidden.
	m_chkMultiple.ShowWindow(m_bMultiSelectVisible ? SW_SHOW : SW_HIDE);
	m_chkMultiple.EnableWindow(m_bMultiSelectEnabled);
	m_chkFilter.ShowWindow(m_bFilterVisible ? SW_SHOW : SW_HIDE);
	m_chkFilter.SetCheck(m_bFilterOn);
//	m_chkFilter.EnableWindow(m_bFilterVisible);
//	if (!m_bMultiSelectVisible)
	if ((!m_bMultiSelectVisible) && (!m_bFilterVisible))
	{
		//, frequently want to align controls relative to each other
		// ie position the bottom of the listview to the bottom of the checkbox, list this
		CRect rlvw;
		CRect rchk;
		m_lvw.GetWindowRect(rlvw);
		ScreenToClient(rlvw);
		m_chkMultiple.GetWindowRect(rchk);
		ScreenToClient(rchk);
		rlvw.bottom = rchk.bottom;
		int nHeight = rlvw.bottom - rlvw.top;
		int nWidth = rlvw.right - rlvw.left;
		m_lvw.SetWindowPos(0, 0, 0, nWidth, nHeight, SWP_NOMOVE | SWP_NOZORDER);
	}

//	if (m_lngButtonFlags & flagShowAdd) m_btnAdd.ShowWindow(SW_SHOW);
//	if (m_lngButtonFlags & flagShowAddChild) m_btnAddChild.ShowWindow(SW_SHOW);
//	if (m_lngButtonFlags & flagShowEdit) m_btnEdit.ShowWindow(SW_SHOW);
//	if (m_lngButtonFlags & flagShowDelete) m_btnDelete.ShowWindow(SW_SHOW);
//	if (m_lngButtonFlags & flagShowDefault) m_btnUseDefault.ShowWindow(SW_SHOW);
	m_btnAdd.EnableWindow(m_lngButtonFlags & flagShowAdd);
	m_btnAddChild.EnableWindow(m_lngButtonFlags & flagShowAddChild);
	m_btnEdit.EnableWindow(m_lngButtonFlags & flagShowEdit);
	m_btnDelete.EnableWindow(m_lngButtonFlags & flagShowDelete);
	m_btnUseDefault.EnableWindow(m_lngButtonFlags & flagShowDefault);

	// Hide the add as child checkbox if we're not in hierarchy mode
//	m_chkAddAsChild.ShowWindow(m_bHierarchy ? SW_SHOW : SW_HIDE);
	// Disable add child button if not in hierarchy mode (eg class, properties, icons)
	if (m_lngButtonFlags & flagShowAddChild)
	{
		m_btnAddChild.EnableWindow(m_bHierarchy);
	}

	if (m_lngButtonFlags & flagShowClose)
	{
//		m_btnClose.ShowWindow(SW_SHOW);
//		m_btnOK.ShowWindow(SW_HIDE);
//		m_btnCancel.ShowWindow(SW_HIDE);
		//, or could change cancel to close and hide OK button
		m_btnOK.SetWindowText("&Close");
		m_btnCancel.EnableWindow(FALSE);
		// resize form also to cover ok/cancel space
//		CRect rButton;
//		m_btnOK.GetClientRect(rButton); // screen coords
//		CRect rDialog;
//		GetWindowRect(rDialog);
//		int nHeight = (rDialog.bottom - rDialog.top) - rButton.bottom - 8; //,
//		int nWidth = (rDialog.right - rDialog.left);
//		SetWindowPos(0, 0, 0, nWidth, nHeight, SWP_NOMOVE | SWP_NOZORDER);
	}

/*
	// Reposition listview to cover buttons if no buttons shown
	//, i don't think this is needed
	if (m_lngButtonFlags == 0)
	{
		//, this is insane - such a crappy API!
		CRect rTree;
		CRect rButton;
		m_lvw.GetWindowRect(rTree);
		ScreenToClient(rTree);
		m_btnAdd.GetWindowRect(rButton);
		ScreenToClient(rButton);
		rTree.bottom = rButton.bottom;
		int nHeight = rTree.bottom - rTree.top;
		int nWidth = rTree.right - rTree.left;
		m_lvw.SetWindowPos(0, 0, 0, nWidth, nHeight, SWP_NOMOVE | SWP_NOZORDER);
	}
*/

}



// Bring up help for this dialog
void CDialogEditLink::OnBtnHelp() 
{
	// Bring up different help topic depending on the mode this dialog is in
//	theApp.WinHelp(HID_BASE_RESOURCE + IDD_EDIT_LINK);
	theApp.WinHelp(HID_BASE_RESOURCE + m_nHelpID);
}



void CDialogEditLink::OnChkHierarchy() 
{
	//, Toggle hierarchy on/off	
}



void CDialogEditLink::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}




// Add a new item to the current list, using the appropriate dialog for the default class.
void CDialogEditLink::OnBtnAdd() 
{
	AddObject(FALSE); // add as sibling
}

void CDialogEditLink::OnBtnAddChild() 
{
	AddObject(TRUE); // add as child
}


// Add a sibling or child object
void CDialogEditLink::AddObject(BOOL bAddAsChild)
{
	CWaitCursor cw;

	// Get parent object
	BObject* pobjParent = m_pobjStart;
	BObject* pobjCurrent = (BObject*) m_lvw.GetSelectedItemData();
	if (pobjCurrent)
	{
		ASSERT_VALID(pobjCurrent);
		// If adding as a child of selected item, use current item as parent,
		// otherwise use current item's parent.
		if (bAddAsChild)
			pobjParent = pobjCurrent;
		else
			pobjParent = pobjCurrent->m_pobjParent;
	}
	ASSERT_VALID(pobjParent);

	BObject* pobjNew = 0;

	//. encapsulate this in document?
	switch (m_lngDefaultClassID)
	{
	case classProperty:
		// Add a new property using the edit property dialog
		pobjNew = m_pDoc->UIAddNewPropertyDef();
		break;
	case classClass:
		// Bring up class wizard to add a new class
		pobjNew = m_pDoc->UIAddNewClass();
		break;
	case classIcon:
		// Import new icon from .ico file
		pobjNew = m_pDoc->UIImportIcon();
		break;
	default:
		{
			// Add a new object
			pobjNew = m_pDoc->UIAddNewObject(pobjParent, FALSE);
			break;
		}
	}

	// If user said OK to new object, add it to listview and select it.
	if (pobjNew)
	{
		ASSERT_VALID(pobjNew);
		// If we're adding a child of the (hidden) parent folder, set parent to 0 so listview will add the object
		// (otherwise it won't be able to find the parent object and won't bother adding the new object).
		if (pobjParent == m_pobjStart) pobjParent = 0;
		int nItem = m_lvw.AddObject(pobjNew, pobjParent);
		m_lvw.SelectItemData((LPARAM) pobjNew);
		// If we're in checkbox mode check off the new item.
		if (m_bMultiSelectOn)
		{
//			//  the listview doesn't draw a checkbox for the new item, so refresh it
//			m_lvw.RedrawItems(nItem, nItem);
			m_lvw.SetCheck(nItem);
		}
	}

	// Set focus back to list
	m_lvw.SetFocus();

}



void CDialogEditLink::OnChkFilter() 
{
	// Turn filter on and off
	m_bFilterOn = m_chkFilter.GetCheck();

	// Refill listview based on filter selection
	AddObjects();
}


//, pass LPCTSTR pszFilterLabel also
void CDialogEditLink::ShowFilterCheckbox(BOOL bFilterOn, ULONG lngFilterExcludeFlags)
{
	m_bFilterVisible = TRUE;
	m_bFilterOn = bFilterOn;
	m_lngFilterExcludeFlags = lngFilterExcludeFlags;
}




void CDialogEditLink::AddObjects()
{
	m_lvw.DeleteAllItems();

	// Add objects to list
	if (m_bFilterOn)
		m_lvw.AddObjects(m_pobjStart, m_lngFilterExcludeFlags, m_bIncludeStart, TRUE, 0, TRUE, m_bHierarchy);
	else
		m_lvw.AddObjects(m_pobjStart, m_lngExcludeFlags, m_bIncludeStart, TRUE, 0, TRUE, m_bHierarchy);

	// Sort the list now if it's not hierarchical
	if (!m_bHierarchy)
		m_lvw.SortByProperty(propName, 1);
}


void CDialogEditLink::OnOK() 
{
	// Validate input
	// Note: In multiselect mode we don't care if any item is selected - doesn't affect the checkboxes
	// bug: didn't handle the multiselect case here! (a problem with multi mode dialogs - hard to test)
	if ((m_lvw.GetSelectedCount() == 0) && !m_bMultiSelectOn)
	{
		if (IDNO == AfxMessageBox("No object was selected - this will clear any existing property value. Continue?", MB_ICONQUESTION + MB_YESNO))
			return;
	}

	CDialog::OnOK();
}






/*

//, Sort the items by checkbox and name
void CDialogEditLink::OnBtnArrange() 
{
	// will need to turn off autosort for links then
	// no - won't work for locations folder with hierarchy
	// just have move up and down buttons for now, maybe just in admin mode for v1?
//	m_lvw.Sort(propName, m_pDoc);
	// walk through items, if checkbox on, move the item 
}

*/



// Move the selected item up
void CDialogEditLink::OnBtnUp() 
{
	// exchange the selected item with the one above it
	int nItem = m_lvw.GetSelectedItem();
	if (nItem > 0)
	{
		int nItem2 = nItem - 1;
		int nPos = m_lvw.MoveItemToSibling(nItem, nItem2, FALSE);
		m_lvw.SelectItem(nPos);
	}
	// Set focus back to list
	m_lvw.SetFocus();
}


// Move the selected item down
void CDialogEditLink::OnBtnDown() 
{
	// exchange the selected item with the one below it
	int nItem = m_lvw.GetSelectedItem();
	int nRows = m_lvw.GetItemCount();
	if (nItem < nRows - 1)
	{
		int nItem2 = nItem + 1;
		int nPos = m_lvw.MoveItemToSibling(nItem, nItem2, TRUE);
		m_lvw.SelectItem(nPos);
	}
	// Set focus back to list
	m_lvw.SetFocus();
}

