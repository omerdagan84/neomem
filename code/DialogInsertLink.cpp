
// CDialogInsertLink



#include "precompiled.h"

#include "neomem.h"
#include "DialogInsertLink.h"

#include "NeoDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CDialogInsertLink, CDialogEx)
	//{{AFX_MSG_MAP(CDialogInsertLink)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDialogInsertLink::CDialogInsertLink(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogInsertLink::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogInsertLink)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



BOOL CDialogInsertLink::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// Initialize listview
	m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
	m_lvw.m_pDoc = m_pDoc;
	m_lvw.SetFullRowSelect(TRUE);


	// Add columns, getting column information from object or from object's classdef.
	// Note: If we're adding the Class folder, Property folder, or Icon folder, use the special
	// BDataColumns objects stored in the document. This is so that the admin column
	// arrangements will be left as is for the admin user. 
//	m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsDefault), TRUE, TRUE);
	m_lvw.InsertColumn(0, "Type", LVCFMT_LEFT, 100);
	m_lvw.InsertColumn(1, "Description", LVCFMT_LEFT, 200);


	//. could have a system folder to hold these types - use also with bdatahyperlink?
	//. each would have a description that would show up, and use a cool icon
	//. could also have prefix chars - http:, mailto:, file:, file:, ftp:, news:, object:

//	BObject* pobj = 
//	m_lvw.AddObject(pobj);
	m_lvw.InsertItem(0, "Website", 1);
	m_lvw.InsertItem(1, "Email", 2);
	m_lvw.InsertItem(2, "File", 3);
	m_lvw.InsertItem(3, "Folder", 4);
	m_lvw.InsertItem(4, "FTP", 5);
	m_lvw.InsertItem(5, "News", 6);
	m_lvw.InsertItem(6, "Object", 7);

	
	// Get default class to determine which dialogs to use, etc.
//	ASSERT_VALID(m_pobjStart);
//	BObject* pobjClass = m_pobjStart->GetPropertyLink(propDefaultClass);
//	ASSERT_VALID(pobjClass);
//	m_lngDefaultClassID = pobjClass->GetObjectID();
	// Show buttons, etc.
//	UpdateControls();
	// Set state of checkbox and listview
//	m_chkMultiple.SetCheck(m_bMultiSelectOn ? 1 : 0);
//	OnChkMultiple(); // this will set listview mode to single or multi select

	// Select item or items	
	// Note: If pdatlink is 0, will just select first item in list
//	m_lvw.SetSelectedItems(m_pdatLink);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialogInsertLink::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogInsertLink)
	DDX_Control(pDX, IDC_LBL_LINK, m_lblLink);
	DDX_Control(pDX, IDC_TXT_LINK, m_txtLink);
	DDX_Control(pDX, IDC_LVW_TYPE, m_lvw);
	DDX_Control(pDX, IDC_BTN_BROWSE, m_btnBrowse);
	//}}AFX_DATA_MAP
}






