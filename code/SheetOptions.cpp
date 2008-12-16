
// CSheetOptions



#include "precompiled.h"
#include "neomem.h"
#include "SheetOptions.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNAMIC(CSheetOptions, CPropertySheetEx2)

CSheetOptions::CSheetOptions(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheetEx2(nIDCaption, pParentWnd, iSelectPage)
{
}

CSheetOptions::CSheetOptions(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheetEx2(pszCaption, pParentWnd, iSelectPage)
{
}

CSheetOptions::~CSheetOptions()
{
}


BEGIN_MESSAGE_MAP(CSheetOptions, CPropertySheetEx2)
	//{{AFX_MSG_MAP(CSheetOptions)
	ON_COMMAND(ID_TAB_DISPLAY, OnTabDisplay)
	ON_COMMAND(ID_TAB_FILE, OnTabFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CSheetOptions::OnInitDialog() 
{
	BOOL bResult = CPropertySheetEx2::OnInitDialog();
	
	// Get accelerators
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_OPTIONS)); 

	return bResult;
}

void CSheetOptions::OnTabDisplay() 
{
	SetActivePage(0);	
}

void CSheetOptions::OnTabFile() 
{
	SetActivePage(1);
}
