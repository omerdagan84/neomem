// PageClassAdvanced.cpp : implementation file
//

#include "precompiled.h"
#include "neomem.h"
#include "PageClassAdvanced.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageClassAdvanced property page

IMPLEMENT_DYNCREATE(CPageClassAdvanced, CPropertyPage)

CPageClassAdvanced::CPageClassAdvanced() : CPropertyPage(CPageClassAdvanced::IDD)
{
	//{{AFX_DATA_INIT(CPageClassAdvanced)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageClassAdvanced::~CPageClassAdvanced()
{
}

void CPageClassAdvanced::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageClassAdvanced)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageClassAdvanced, CPropertyPage)
	//{{AFX_MSG_MAP(CPageClassAdvanced)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageClassAdvanced message handlers
