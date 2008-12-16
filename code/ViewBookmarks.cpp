
// CViewBookmarks



#include "precompiled.h"
#include "NeoMem.h"
#include "ViewBookmarks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CViewBookmarks, CViewEx)

BEGIN_MESSAGE_MAP(CViewBookmarks, CViewEx)
	//{{AFX_MSG_MAP(CViewBookmarks)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CViewBookmarks::CViewBookmarks()
{
}

CViewBookmarks::~CViewBookmarks()
{
}






// CViewBookmarks diagnostics
//------------------------------------------------------------------------------------------------

#ifdef _DEBUG
void CViewBookmarks::AssertValid() const
{
	CViewEx::AssertValid();
}

void CViewBookmarks::Dump(CDumpContext& dc) const
{
	CViewEx::Dump(dc);
}
#endif //_DEBUG



// CViewBookmarks message handlers
//------------------------------------------------------------------------------------------------


