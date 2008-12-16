
// CPageClassSummary



#include "precompiled.h"

#include "NeoMem.h"
#include "PageClassSummary.h"

#include "ConstantsDatabase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CPageClassSummary, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageClassSummary, CPropertyPage)
	//{{AFX_MSG_MAP(CPageClassSummary)
//	ON_NOTIFY(NM_SETFOCUS, IDC_RTF, OnSetFocusRtf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CPageClassSummary::CPageClassSummary() : CPropertyPage(CPageClassSummary::IDD)
{
	//{{AFX_DATA_INIT(CPageClassSummary)
	//}}AFX_DATA_INIT
	m_pDoc = 0;
	m_psh = 0;
	m_pobj = 0;
}

CPageClassSummary::~CPageClassSummary()
{
}



// Note: This routine only gets called once!!
BOOL CPageClassSummary::OnInitDialog() 
{
	// Get document and parent sheet
	m_pDoc = CNeoDoc::GetDoc();
	ASSERT_VALID(m_pDoc);
	m_psh = (CSheetWizard*) GetParent();
	ASSERT_VALID(m_psh);

	// Subclass the rich edit control to use our ex class
//	m_rtf.SubclassDlgItem(IDC_RTF, this);
//	ASSERT_VALID (&m_rtf);

	// Call base class
	CPropertyPage::OnInitDialog();
	
	// Set font for title
	m_lblTitle.SetFont(&theApp.m_fontControlsBold);

	// Set instructions if in edit mode
	if (!m_psh->m_bAdd)
	{
		m_lblInstructions.SetWindowText(_T("That's all the information NeoMem needs to modify the class."));
	}

	// Set context sensitive help
	SetHelpID(IDD_CLASS_SUMMARY);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CPageClassSummary::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageClassSummary)
	DDX_Control(pDX, IDC_LBL_TITLE, m_lblTitle);
	DDX_Control(pDX, IDC_LBL_INSTRUCTIONS, m_lblInstructions);
	//}}AFX_DATA_MAP

	// Get temp object we're working with
	m_pobj = m_psh->m_pobj;
	ASSERT_VALID(m_pobj);
	ASSERT_VALID(m_pobj->m_pobjParent);
	BObject* pobjIcon = m_pDoc->GetObject(m_pobj->GetIconID());
	ASSERT_VALID(pobjIcon);

/*

	if (!pDX->m_bSaveAndValidate) // Load
	{
		// Fill summary text box with information about the class

		// Get names of properties associated with this class, excluding any admin only props
		BObjects aProps;
		int nProps = m_pobj->GetPropertyDefs(aProps, FALSE, TRUE);
		CString strProps = aProps.GetText(flagAdminOnly);

		// Get default class
		BObject* pobjDefaultClass = m_pobj->GetPropertyLink(propObjectDefaultClass);
		ASSERT_VALID(pobjDefaultClass);

		// Get autosort
		//, make some method!
		BDataFlags* pdat = DYNAMIC_DOWNCAST(BDataFlags, m_pobj->GetPropertyData(propObjectFlags));
		BOOL bAutosort = FALSE;
		if (pdat)
		{
			ASSERT_VALID(pdat);
			bAutosort = !(pdat->m_lngFlags & flagNoAutosort);
		}

//		CString strFolder;
//		if (m_psh->m_bAddFolder)
//			strFolder.Format("Add a new folder named \"%s\".", (LPCTSTR) m_psh->m_strFolderName);
//		else
//			strFolder.Format("No new folder will be added.");

		CString strHeader = "{\\rtf1\\ansi\\ansicpg1252 \\deff0\\deflang1033 \n"
			"{\\fonttbl{\\f0\\fswiss Arial;}}\n"  // font name
//			"\\widowctrl \\ftnbj \\aenddoc \\formshade \\viewkind4 \\viewscale100 \n"
//			"\\pgbrdrhead \\pgbrdrfoot \\fet0 \\sectd \\linex0 \\endnhere \\sectdefaultcl \n"
			"\\sectd\n"
//			"\\pard \\plain \\fi-1170 \\li1170 \\ri3960 \\widctlpar \\adjustright  \\fs20 \\cgrid \\par \n";
			"\\pard \\plain \\fi-1170 \\li1170 \\fs16 \n";

		CString strFooter = "\\par }}\n";

		CString str;
		str.Format("%s"
						  "{\\b Name}{\\tab %s \\par}\n"
						  "{\\b Description}{\\tab %s \\par}\n"
//						  "{\\b Parent}{\\tab %s \\par}\n"
						  "{\\b Icon}{\\tab %s \\par}\n"
						  "{\\b Properties}{\\tab %s \\par}\n"
						  "{\\b Contains}{\\tab %s \\par}\n"
						  "{\\b Autosort}{\\tab %s \\par}\n"
//						  "\\par %s \\par"
						  "%s \\par \\par", 
						  strHeader,
						  m_pobj->GetPropertyText(propName), 
						  m_pobj->GetPropertyText(propDescription),
//						  m_pobj->m_pobjParent->GetPropertyText(propName),
						  pobjIcon->GetPropertyText(propName),
						  strProps,
						  pobjDefaultClass->GetPropertyText(propName),
						  bAutosort ? "On" : "Off",
//						  strFolder,
						  strFooter
						  );
//		m_lblSummary.SetWindowText(str);
//		m_rtf.SetWindowText(str);
//		m_rtf.SetRtf(str);
		m_rtf.SetRtfConst(str);
		m_rtf.SetSel(0, 0);

	}
*/

}



BOOL CPageClassSummary::OnSetActive() 
{
//	CPropertySheet* psheet = (CPropertySheet*) GetParent();
	CSheetWizard* psheet = (CSheetWizard*) GetParent();
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);	
	psheet->SetClassWizardTitle();
	return CPropertyPage::OnSetActive();
}




/*
void CPageClassSummary::OnSetFocusRtf(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_psh->SetFocus();	
	*pResult = 0;
}
*/



