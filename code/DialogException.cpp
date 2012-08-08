
// CDialogException




#include "precompiled.h"

#include "neomem.h"
#include "DialogException.h"

#include "BObject.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDialogException::CDialogException(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogException::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogException)
	//}}AFX_DATA_INIT
}


void CDialogException::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogException)
	DDX_Control(pDX, IDC_LST, m_lst);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogException, CDialog)
	//{{AFX_MSG_MAP(CDialogException)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CDialogException::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_lst.AddString("Memory"); // 0
	m_lst.AddString("File : fileNotFound"); // 1
	m_lst.AddString("File : accessDenied"); // 2
	m_lst.AddString("User"); // 3
	m_lst.AddString("ThrowAssertion()"); // 4 
	m_lst.AddString("ASSERT(0)"); // 5
	m_lst.AddString("ASSERT_VALID(0)"); // 6
	m_lst.AddString("pobj = 0; n = pobj->GetObjectID();"); // 7
	m_lst.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogException::OnOK() 
{
	switch (m_lst.GetCurSel())
	{
	case 0:
		AfxThrowMemoryException();
		break;
	case 1:
		AfxThrowFileException(CFileException::fileNotFound);
		break;
	case 2:
		AfxThrowFileException(CFileException::accessDenied);
		break;
	case 3:
		AfxMessageBox("user exception message (before throwing it)!");
		AfxThrowUserException();
		break;
	case 4:
		Library::ThrowAssertion(THIS_FILE, __LINE__, "test error", "meh"); // default message is "Assertion Failed"
		break;
	case 5:
		ASSERT(0);
		break;
	case 6:
		ASSERT_VALID(0);
		break;
	case 7:
		{
			//.. in release mode, this is NOT CAUGHT by unhandled exception handler!
			BObject* pobj = 0;
			ULONG n = pobj->GetObjectID(); // error: using 0 as address
			CString str;
			str.Format("%d", n);
			AfxMessageBox(str);
		}
		break;
	}	

	// This doesn't show up
	AfxMessageBox("After throwing exception - does this show up?");

	CDialog::OnOK();
}


