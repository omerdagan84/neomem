
// CDialogGetLocation



#include "precompiled.h"

#include "NeoMem.h"
#include "DialogGetLocation.h"

#include "NeoDoc.h"
#include "BObject.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDialogGetLocation::CDialogGetLocation(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGetLocation::IDD, pParent),
	m_pDoc(0),
	m_bAllowSelectHome(FALSE),
	m_nHelpID(0)
{
	//{{AFX_DATA_INIT(CDialogGetLocation)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogGetLocation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGetLocation)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_TVW, m_tvw);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) // Load
	{
	}
	else // Save
	{
		// Get selected object
		m_pobj = (BObject*) m_tvw.GetSelectedItemData();
		if (m_pobj)
		{
			ASSERT_VALID(m_pobj);
		}
	}
}


BEGIN_MESSAGE_MAP(CDialogGetLocation, CDialog)
	//{{AFX_MSG_MAP(CDialogGetLocation)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TVW, OnSelchangedTvw)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CDialogGetLocation::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Set caption
	if (!m_strCaption.IsEmpty())
		SetWindowText(m_strCaption);

	// Show instructions
	if (!m_strInstructions.IsEmpty())
		m_lblInstructions.SetWindowText(m_strInstructions);

	// Get document
	m_pDoc = CNeoDoc::GetDoc();
	ASSERT_VALID(m_pDoc);

	// Initialize treeview
	m_tvw.SetImageList(m_pDoc->GetImageList(), TVSIL_NORMAL);

	// Add all objects to list
	BObject* pobjStart = m_pDoc->GetObject(rootUser);
	ASSERT_VALID(pobjStart);
	m_tvw.AddObjects(pobjStart, theApp.m_lngExcludeFlags, TRUE, TRUE, FALSE);

	// Select current object
	ASSERT_VALID(m_pobj);
	m_tvw.SelectItemData((LPARAM) m_pobj);
	
//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	
	m_tvw.SetFocus();
	return FALSE;
}




void CDialogGetLocation::OnSelchangedTvw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// Get newly selected item
	TVITEM itemNew = pNMTreeView->itemNew;
	BObject* pobj = (BObject*) itemNew.lParam;
	ASSERT_VALID(pobj);

	// Enable/disable OK button 
	//, do this based on if object has Text View or not
	BOOL bHome = (pobj->GetObjectID() == rootUser);
	m_btnOK.EnableWindow(!(bHome && !m_bAllowSelectHome));

	*pResult = 0;
}


void CDialogGetLocation::OnBtnHelp() 
{
	// Caller should set m_nHelpID to the appropriate resource:
	// IDD_MOVE_OBJECT_TO, IDD_MOVE_TEXT_TO, IDD_GOTO_OBJECT, etc.
	theApp.WinHelp(HID_BASE_RESOURCE + m_nHelpID); 	
}



