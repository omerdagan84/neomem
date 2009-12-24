
// CNeoDocTemplate



#include "precompiled.h"

#include "NeoMem.h"
#include "NeoDocTemplate.h"

#include "NeoDoc.h"
#include "BObject.h"
#include "ConstantsDatabase.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNAMIC(CNeoDocTemplate, CMultiDocTemplate)

BEGIN_MESSAGE_MAP(CNeoDocTemplate, CMultiDocTemplate)
	//{{AFX_MSG_MAP(CNeoDocTemplate)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CNeoDocTemplate::CNeoDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, 
			CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) : 
			CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
}

CNeoDocTemplate::~CNeoDocTemplate()
{
}




// Diagnostics

#ifdef _DEBUG
void CNeoDocTemplate::AssertValid() const
{
	CMultiDocTemplate::AssertValid();
}

void CNeoDocTemplate::Dump(CDumpContext& dc) const
{
	CMultiDocTemplate::Dump(dc);
}
#endif //_DEBUG






CDocument* CNeoDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible)
{
	CDocument* pDoc = CMultiDocTemplate::OpenDocumentFile(lpszPathName, bMakeVisible);
	return pDoc;
}



void CNeoDocTemplate::InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc, BOOL bMakeVisible)
{
	// Cast to our class
	CFrameChild* pFrameChild = (CFrameChild*) pFrame; //.cast
	ASSERT_VALID(pFrameChild);

	// Delegate to implementation in CFrameWnd
	pFrame->InitialUpdateFrame(pDoc, bMakeVisible);

	// NOW WE CAN SET THE DAMN SPLITTER POS!!
	CNeoDoc* pNeoDoc = (CNeoDoc*) pDoc; //.cast
	ASSERT_VALID(pNeoDoc);
	ULONG lngSplitterPos = pNeoDoc->GetSplitterPos();
	if (lngSplitterPos)
		pFrameChild->SetSplitterPos(lngSplitterPos);

}

