
// CDialogViews



#include "precompiled.h"

#include "NeoMem.h"
#include "DialogViews.h"

#include "ConstantsDatabase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CDialogViews, CDialog)
	//{{AFX_MSG_MAP(CDialogViews)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CDialogViews::CDialogViews(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogViews::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogViews)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_pdatViews = NULL;
	m_nTab = 0;
}




BOOL CDialogViews::OnInitDialog() 
{
	// Call base class (calls DoDataExchange)
	CDialog::OnInitDialog();
	
	// Fill listview with all available views
	
	//, simplest for now is just list all available views and check off for this tab
	// later could have more sophisticated dialog

	// Get document and parent sheet
	m_pDoc = BDoc::GetDoc();
	ASSERT_VALID(m_pDoc);

	// Initialize listview
	m_lvw.m_pDoc = m_pDoc;
	m_lvw.SetImageList(m_pDoc->GetImageList(), LVSIL_SMALL);
	m_lvw.SetFullRowSelect(TRUE);
	m_lvw.SetCheckboxes(TRUE);
	m_lvw.m_bDisabledItems = TRUE; // Special disabled items mode (draws some items in gray and won't let user change their checkbox)

	m_lvw.InitializeColumns(&(m_pDoc->m_datColumnsViewFolder), FALSE, TRUE);

	BObject* pobjStart = m_pDoc->GetObject(folderViews);
	m_lvw.AddObjects(pobjStart, theApp.m_lngExcludeFlags);

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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogViews::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogViews)
	DDX_Control(pDX, IDC_LVW, m_lvw);
	DDX_Control(pDX, IDC_INSTRUCTIONS, m_lblInstructions);
	DDX_Control(pDX, IDC_BTN_UP, m_btnUp);
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnDown);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) // Save
	{
	}

}





void CDialogViews::OnBtnHelp() 
{
	theApp.WinHelp(HID_BASE_RESOURCE + IDD_VIEWS);
}

void CDialogViews::OnBtnUp() 
{
}

void CDialogViews::OnBtnDown() 
{
}


