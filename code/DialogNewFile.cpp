
// CDialogNewFile


#include "precompiled.h"

#include "NeoMem.h"
#include "DialogNewFile.h"

#include "NeoDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




BEGIN_MESSAGE_MAP(CDialogNewFile, CDialog)
	//{{AFX_MSG_MAP(CDialogNewFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDialogNewFile::CDialogNewFile(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogNewFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogNewFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



BOOL CDialogNewFile::OnInitDialog() 
{
	// Call base class
	CDialog::OnInitDialog();

	// Get document
//	m_pDoc = CNeoDoc::GetDoc();
//	ASSERT_VALID(m_pDoc);

	// Set image list
//	CImageList* pImageList = m_pDoc->GetImageList();
//	ASSERT_VALID(pImageList);
//	m_lvw.SetImageList(pImageList, LVSIL_SMALL);
//	m_lvw.SetImageList(theApp.GetImageList(), LVSIL_NORMAL);

	// Create image list containing file icon
	m_iml.Create(32, 32, ILC_MASK | ILC_COLOR24, 1, 1);
	int nImage = m_iml.Add(theApp.LoadIcon(IDR_NEOMEM_TYPE));

	m_lvw.SetImageList(&m_iml, LVSIL_NORMAL);

	// Switch to large icon view
	m_lvw.ModifyStyle(LVS_REPORT, LVS_ICON);
	
	// Fill listview with files in the Templates folder
	CFileFind ff;
	BOOL bWorking = ff.FindFile("*.neo"); //, get from string table
	int nItem = 0;
	while (bWorking)
	{
		bWorking = ff.FindNextFile();
		CString strName = ff.GetFileName();
		m_lvw.InsertItem(nItem, strName, nImage);
		nItem++;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogNewFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogNewFile)
	DDX_Control(pDX, IDC_LVW, m_lvw);
	DDX_Control(pDX, IDC_INSTRUCTIONS, m_lblInstructions);
	//}}AFX_DATA_MAP
}

