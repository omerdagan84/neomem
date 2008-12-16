
// CPropertySheetEx2



#include "precompiled.h"
#include "NeoMem.h"
#include "PropertySheetEx2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC(CPropertySheetEx2, CPropertySheet)

CPropertySheetEx2::CPropertySheetEx2()
{
	m_hAccel = 0;
}


CPropertySheetEx2::CPropertySheetEx2(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_hAccel = 0;
}

CPropertySheetEx2::CPropertySheetEx2(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_hAccel = 0;
}

CPropertySheetEx2::~CPropertySheetEx2()
{
}


BEGIN_MESSAGE_MAP(CPropertySheetEx2, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetEx2)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CPropertySheetEx2::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// Add context help button to property sheets:

	// This statement is needed only if you are using wizard style property sheet.
	if (m_psh.dwFlags & PSH_WIZARD)
		ModifyStyle(0, WS_SYSMENU, 0);

//	ModifyStyleEx(0, WS_EX_CONTEXTHELP, 0);
	
	return bResult;
}



BOOL CPropertySheetEx2::PreTranslateMessage(MSG* pMsg) 
{
	// Handle accelerators
	if (m_hAccel) 
	{
		if (::TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) 
		{
			return (TRUE);
		}
	}
	
	return CPropertySheet::PreTranslateMessage(pMsg);
}


