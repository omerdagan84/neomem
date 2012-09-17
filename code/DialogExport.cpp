// DialogExport.cpp : implementation file
//

#include "precompiled.h"
#include "neomem.h"
#include "DialogExport.h"


#include "FileDialogEx.h"
#include "FileFormat.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogExport dialog


CDialogExport::CDialogExport(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogExport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogExport)
	m_bRecurse = FALSE;
	m_bSystem = FALSE;
	m_strItem = _T("");
	m_strFilename = _T("");
	//}}AFX_DATA_INIT
}


void CDialogExport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogExport)
	DDX_Control(pDX, IDC_LBL_ITEM, m_lblItem);
	DDX_Control(pDX, IDC_CHK_SYSTEM, m_chkSystem);
	DDX_Control(pDX, IDC_CHK_RECURSE, m_chkRecurse);
	DDX_Control(pDX, IDC_TXT_FILENAME, m_txtFilename);
	DDX_Control(pDX, IDC_CBO_FORMAT, m_cboFormat);
	DDX_Control(pDX, IDC_BTN_CHOOSE_FILE, m_btnChooseFile);
	DDX_Check(pDX, IDC_CHK_RECURSE, m_bRecurse);
	DDX_Check(pDX, IDC_CHK_SYSTEM, m_bSystem);
	DDX_Text(pDX, IDC_LBL_ITEM, m_strItem);
	DDX_Text(pDX, IDC_TXT_FILENAME, m_strFilename);
	//}}AFX_DATA_MAP

	// Save values
	if (pDX->m_bSaveAndValidate)
	{
		int n = m_cboFormat.GetCurSel();
		CFileFormat* ff = (CFileFormat*) m_cboFormat.GetItemDataPtr(n);
		if (ff)
			m_strFormat = ff->m_strExtension;
		else
			m_strFormat = _T("csv");
	}

}


BEGIN_MESSAGE_MAP(CDialogExport, CDialog)
	//{{AFX_MSG_MAP(CDialogExport)
	ON_BN_CLICKED(IDC_BTN_CHOOSE_FILE, OnBtnChooseFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogExport message handlers



BOOL CDialogExport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	// add items to export
//	m_cboItems.AddString("Current Object");
//	m_cboItems.AddString("Current Object and Children");
//	m_cboItems.AddString("All Objects");
//	m_cboItems.AddString("All Objects and System Objects (Classes, Properties, Icons, etc)");
//	m_cboItems.SetCurSel(0);


	// get these from doc obj!
//	m_pdoc->GetExportFormats

	// add formats
//	m_cboFormat.AddString("Comma Separated Values (text file) (.txt)");
//	m_cboFormat.AddString("Tab Separated Values (text file) (.txt)");
//	m_cboFormat.AddString("Rich Text Format (.rtf)");
//	m_cboItems.SetCurSel(0);

/*	POSITION pos = theApp.m_listExportFormats.GetHeadPosition();
	while (pos)
	{
		CFileFormat* ff = (CFileFormat*) theApp.m_listExportFormats.GetNext(pos);
		if (ff)
		{
			int i = m_cboFormat.AddString(ff->m_strDescription);
			m_cboFormat.SetItemDataPtr(i, (void*) ff);
		}
	}
	m_cboFormat.SetCurSel(0);
*/

	int nFormats = theApp.m_listExportFormats.GetSize();
	for (int i=0; i < nFormats; i++)
	{
		CFileFormat* ff = theApp.m_listExportFormats.GetAt(i);
		if (ff)
		{
			int i = m_cboFormat.AddString(ff->m_strName);
			m_cboFormat.SetItemDataPtr(i, (void*) ff);
		}
	}
	m_cboFormat.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void CDialogExport::OnBtnChooseFile() 
{
	m_txtFilename.GetWindowText(m_strFilename);

	int i = m_cboFormat.GetCurSel();
	CFileFormat* ff = (CFileFormat*) m_cboFormat.GetItemDataPtr(i);
	ASSERT_VALID(ff);

	// Initialize file dialog
//	CFileDialogEx dlg(FALSE, _T("rtf"), m_strFilename, 
	CFileDialogEx dlg(FALSE, ff->m_strExtension, m_strFilename, 
//,	CFileDialog dlg(FALSE, ff->m_strExtension, m_strFilename, 
//		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szExportFilter);
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR) theApp.m_strExportFilter);
	CString strCaption = _T("Export to file");
	dlg.m_ofn.lpstrTitle = strCaption;
	dlg.m_bShowOptionButton = FALSE; // because exporting, don't need save options!

	// Get export filename
	if (dlg.DoModal() == IDOK)
	{
//		CString strFilename = dlg.GetPathName();
//		AfxMessageBox(strFilename);
		m_strFilename = dlg.GetPathName(); // full/path/test.rtf
		m_txtFilename.SetWindowText(m_strFilename);

//		CString strFileTitle = dlg.GetFileTitle(); // test
//		CString strExtension = dlg.GetFileExt(); // rtf
	}
	
}



void CDialogExport::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

