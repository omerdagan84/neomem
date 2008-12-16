
// CPropertyPageEx2



#include "precompiled.h"
#include "NeoMem.h"
#include "PropertyPageEx2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNCREATE(CPropertyPageEx2, CPropertyPage)

CPropertyPageEx2::CPropertyPageEx2() : CPropertyPage(CPropertyPageEx2::IDD)
{
	//{{AFX_DATA_INIT(CPropertyPageEx2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPropertyPageEx2::~CPropertyPageEx2()
{
}

void CPropertyPageEx2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyPageEx2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPageEx2, CPropertyPage)
	//{{AFX_MSG_MAP(CPropertyPageEx2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageEx2 message handlers


/*

void CPageOptionsDisplay::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_cLastRClickPoint = point;

	// Display pop up menu
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, ID_WHAT_IS_THIS, "What's this");
//	if (menu.LoadMenu(IDR_WHAT_IS_THIS_MENU))
//	{
//		CMenu* pPopup = menu.GetSubMenu(0);
//		ASSERT(pPopup != NULL);
//		pPopup->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, AfxGetMainWnd());
//	}
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

//	CDialog::OnRButtonUp(nFlags, point);
}


void CPageOptionsDisplay::OnWhatIsThis()
{
	AfxMessageBox("Help!");
//    CWnd* pControl = ChildWindowFromPoint (m_cLastRClickPoint);
    // If the click wasn't on one of the controls, open Help for dialog.
  //  if (pControl == NULL || pControl->m_hWnd == m_hWnd)
//        WinHelp (HID_BASE_RESOURCE + m_nIDHelp, HELP_CONTEXTPOPUP);
//    else
//        WinHelp (HID_BASE_CONTROL + pControl->GetDlgCtrlID(), HELP_CONTEXTPOPUP);
}

*/


