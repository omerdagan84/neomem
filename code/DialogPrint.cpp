
// CDialogPrint


#include "precompiled.h"

#include "NeoMem.h"
#include "DialogPrint.h"

#include "ConstantsDatabase.h"
#include "NeoDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CDialogPrint, CDialog)
	//{{AFX_MSG_MAP(CDialogPrint)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CDialogPrint::CDialogPrint(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPrint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPrint)
	m_bPrintHeaderFooter = FALSE;
	//}}AFX_DATA_INIT
	m_pDoc = 0;
}



BOOL CDialogPrint::OnInitDialog() 
{
	// Call base class (calls DoDataExchange)
	CDialog::OnInitDialog();
	
	// Get some pointers
	m_pDoc = CNeoDoc::GetDoc();
	ASSERT_VALID(m_pDoc);

	// Initialize listview
	m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
	m_lvw.m_pDoc = m_pDoc;

	m_lvw.SetFullRowSelect(TRUE);
//	m_lvw.SetCheckboxes(TRUE);
	m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsViewFolder), TRUE, TRUE);

	// Fill listview with available views
	// Add objects to list
	BObject* pobjStart = m_pDoc->GetObject(folderViews);
	ASSERT_VALID(pobjStart);
	m_lvw.AddObjects(pobjStart, theApp.m_lngExcludeFlags);

	// Select view
	BObject* pobjView = m_pDoc->GetObject(m_lngSelectedViewID);
	m_lvw.SelectItemData((LPARAM) pobjView);


/*
	// Select views
	// Check off views that are visible in this tab
	//, get an array of views for a tab? then just walk through the array
	ASSERT_VALID(m_pdatViews);
	int nIndex = m_pdatViews->GetTabIndex(m_nTab);
	if (nIndex != -1)
	{
		// Walk through and check off views 
		int nViews = m_pdatViews->GetViewCount(m_nTab);
		for (int i = nIndex; i < nIndex + nViews; i++)
		{
			ViewInfo& rvi = m_pdatViews->GetViewInfo(i);
			ULONG lngViewID = rvi.m_lngViewID;
			BObject* pobjView = m_pDoc->GetObject(lngViewID);
			int nItem = m_lvw.FindItemData((LPARAM) pobjView);
			if (nItem != -1)
				m_lvw.SetCheck(nItem, TRUE);
		}
	}
*/
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPrint)
	DDX_Control(pDX, IDC_CHK_HEADERFOOTER, m_chkHeaderFooter);
	DDX_Control(pDX, IDC_LVW, m_lvw);
	DDX_Control(pDX, IDC_CHK_LIMIT, m_chkLimit);
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelp);
	DDX_Check(pDX, IDC_CHK_HEADERFOOTER, m_bPrintHeaderFooter);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) // Save
	{
		m_lngSelectedViewID = m_lvw.GetSelectedObjectID();
	}
}



void CDialogPrint::OnBtnHelp() 
{
	theApp.WinHelp(HID_BASE_RESOURCE + IDD_PRINT);
}



int CDialogPrint::DoModalParameters(ULONG lngSelectedViewID)
{
	m_lngSelectedViewID = lngSelectedViewID;
	return DoModal();
}


