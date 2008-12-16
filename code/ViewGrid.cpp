
// CViewGrid


#include "precompiled.h"
#include "NeoMem.h"
#include "ViewGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CViewGrid, CViewEx)

BEGIN_MESSAGE_MAP(CViewGrid, CViewEx)
	//{{AFX_MSG_MAP(CViewGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CViewGrid::CViewGrid()
{
}

CViewGrid::~CViewGrid()
{
}



#ifdef _DEBUG
void CViewGrid::AssertValid() const
{
	CViewEx::AssertValid();
}

void CViewGrid::Dump(CDumpContext& dc) const
{
	CViewEx::Dump(dc);
}
#endif //_DEBUG



