
// CPageFileGeneral



#include "precompiled.h"

#include "NeoMem.h"
#include "PageFileGeneral.h"
#include "StringEx.h"
#include "DialogEditName.h"

#include "ConstantsDatabase.h"
#include "BDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNCREATE(CPageFileGeneral, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageFileGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPageFileGeneral)
	ON_BN_CLICKED(IDC_BTN_PASSWORD, OnBtnPassword)
	ON_BN_CLICKED(IDC_BTN_DATA_MODEL, OnBtnDataModel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CPageFileGeneral::CPageFileGeneral() : CPropertyPage(CPageFileGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPageFileGeneral)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = 0;
}

CPageFileGeneral::~CPageFileGeneral()
{
}



BOOL CPageFileGeneral::OnInitDialog() 
{
	// Call base class
	CPropertyPage::OnInitDialog();
	
	ASSERT_VALID(m_pDoc);

	DisplayInfo();

	// If in admin mode allow some changes
//	m_txtDataModel.EnableWindow(theApp.m_bAdmin);
//	m_txtFileStructure.EnableWindow(theApp.m_bAdmin);

	// Hide file structure if not admin (might confuse users)
	m_lblFileStructure.ShowWindow(theApp.m_bAdmin ? SW_SHOW : SW_HIDE);	
	m_txtFileStructure.ShowWindow(theApp.m_bAdmin ? SW_SHOW : SW_HIDE);

	m_btnDataModel.ShowWindow(theApp.m_bAdmin ? SW_SHOW : SW_HIDE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CPageFileGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageFileGeneral)
	DDX_Control(pDX, IDC_BTN_DATA_MODEL, m_btnDataModel);
	DDX_Control(pDX, IDC_LBL_FILE_STRUCTURE, m_lblFileStructure);
	DDX_Control(pDX, IDC_TXT_LOCATION, m_txtLocation);
	DDX_Control(pDX, IDC_BTN_PASSWORD, m_btnPassword);
	DDX_Control(pDX, IDC_TXT_ENCRYPTION, m_txtEncryption);
	DDX_Control(pDX, IDC_TXT_FILE_STRUCTURE, m_txtFileStructure);
	DDX_Control(pDX, IDC_TXT_DATA_MODEL, m_txtDataModel);
	DDX_Control(pDX, IDC_TXT_SIZE, m_txtSize);
	DDX_Control(pDX, IDC_TXT_OBJECTS, m_txtObjects);
	DDX_Control(pDX, IDC_TXT_NAME, m_txtName);
	DDX_Control(pDX, IDC_TXT_FILE_SIZE, m_txtFileSize);
	DDX_Control(pDX, IDC_TXT_DESCRIPTION, m_txtDescription);
	//}}AFX_DATA_MAP
}




void CPageFileGeneral::OnBtnPassword() 
{
	ASSERT_VALID(m_pDoc);
	m_pDoc->UISetEncryption();
}



// Let admin user change data model number
void CPageFileGeneral::OnBtnDataModel() 
{
	CDialogEditName dlg;
	CString str;
	str.Format("%d", m_pDoc->GetVersionDataModel());
	dlg.m_strName = str;
	if (IDOK == dlg.DoModal())
	{
		m_pDoc->SetVersionDataModel(atol(dlg.m_strName));
		DisplayInfo();
		AfxMessageBox("now close neomem, search on 'const ULONG version' and set to new version or subversion.");
	}
}



void CPageFileGeneral::DisplayInfo()
{
	CStringEx str;

	// Get Name
	m_txtName.SetWindowText(m_pDoc->GetTitle());	

	// Get Location
	m_txtLocation.SetWindowText(m_pDoc->GetPathName());

//	const int nChars = 50;
//	TCHAR szPlain[nChars];
//	TCHAR szFormatted[nChars];

	// Get Number of Objects
/*
	BObject* pobjRoot = m_pDoc->m_pobjRoot;
	BObject* pobjSystem = m_pDoc->GetObject(rootSystem);
	UINT nSystem = pobjSystem->GetChildCount(TRUE) + 2; // 1 for rootsys and 1 for rootmain
	UINT nTotal = pobjRoot->GetChildCount(TRUE) + 1; // 1 for root object
	UINT nUser = nTotal - nSystem;
//	str.Format("%d (%d User, %d System)", nTotal, nUser, nSystem);
//,	str.FormatThousands("%t (%t User, %t System)", nTotal, nUser, nSystem);
//	m_txtObjects.SetWindowText(str);
//	::GetNumberFormat(LOCALE_USER_DEFAULT, 0, str, NULL, szFormatted, nChars);
//	m_txtObjects.SetWindowText(szFormatted);
	str.Format("%s (%s User, %s System)", (LPCTSTR) fc(nTotal), (LPCTSTR) fc(nUser), (LPCTSTR) fc(nSystem));
	m_txtObjects.SetWindowText(str);
*/
	str = m_pDoc->GetNumberOfObjectsString();
	m_txtObjects.SetWindowText(str);

	// Get file size
/*	LPCTSTR pszFile = m_pDoc->GetPathName();
	CFileException e;
	CFile* f = m_pDoc->GetFile(pszFile, CFile::shareDenyNone | CFile::modeRead, &e);
	if (f)
	{
		DWORD dwLength = f->GetLength();
//		str.Format("%d Bytes", dwLength);
		str.Format("%s Bytes", (LPCTSTR) fc(dwLength));
		m_txtFileSize.SetWindowText(str);
		f->Close();
		delete f;
	}
	else
		m_txtFileSize.SetWindowText(_T("(File must be saved first)"));
*/
	str = m_pDoc->GetFileSizeString();
	m_txtFileSize.SetWindowText(str);

	// Get memory used by all objects, starting at root and recursing downwards
	BObject* pobjRoot = m_pDoc->GetRoot();
	str.Format("%s Bytes", (LPCTSTR) Library::fc(pobjRoot->GetMemoryUsed(TRUE)));
	m_txtSize.SetWindowText(str);

	// File Version (eg 1.0) actually an integer but display as a float
	str.Format("Version %.1f", (float) m_pDoc->GetVersionFileStructure());
	m_txtFileStructure.SetWindowText(str);

	// Data Model Version (eg 0.9a)
	// eg versionDataModel 110103 -> version 1.1 release a (01) subversion 03
	float sngDataModel = (float) m_pDoc->GetVersionDataModel() / (float) 100000.0;
	int nRelease = m_pDoc->GetVersionDataModel() - (m_pDoc->GetVersionDataModel() / 10000) * 10000;
	int nSubversion = nRelease - (nRelease / 100) * 100; 
	nRelease /= 100; // get rid of build number
	TCHAR cRelease = nRelease > 0 ? 'a' + nRelease - 1 : ' ';
	// just ignore that dumb build number - it's not a real build number. 
	str.Format("Version %.1f%c (subversion %d)", sngDataModel, cRelease, nSubversion); //, make sure %c works with ansi unicode char etc
	m_txtDataModel.SetWindowText(str);

	// Encryption
	switch (m_pDoc->GetEncryptionType())
	{
	case (BDoc::encryptNone):
		str = "None";
		break;
	case (BDoc::encryptRC4):
		str = "RSA RC4 Encryption"; // 40bit
		break;
//	case (BDoc::encryptAES):
//		str = "Strong Encryption (128 bit AES/Rijndael)";
//		break;
	default:
		str = "Unknown";
		break;
	}
	m_txtEncryption.SetWindowText(str);

	// Compression

}
