
// BDocTemplate



#include "precompiled.h"

#include "NeoMem.h"
#include "NeoDocTemplate.h"

#include "BDoc.h"
#include "BObject.h"
#include "ConstantsDatabase.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNAMIC(BDocTemplate, CMultiDocTemplate)

BEGIN_MESSAGE_MAP(BDocTemplate, CMultiDocTemplate)
	//{{AFX_MSG_MAP(BDocTemplate)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BDocTemplate::BDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, 
			CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) : 
	CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
}

BDocTemplate::~BDocTemplate()
{
}




// Diagnostics

#ifdef _DEBUG
void BDocTemplate::AssertValid() const
{
	CMultiDocTemplate::AssertValid();
}

void BDocTemplate::Dump(CDumpContext& dc) const
{
	CMultiDocTemplate::Dump(dc);
}
#endif //_DEBUG






CDocument* BDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible)
{
	CDocument* pDoc = CMultiDocTemplate::OpenDocumentFile(lpszPathName, bMakeVisible);
	return pDoc;
}



void BDocTemplate::InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc, BOOL bMakeVisible)
{
	// Downcast to our class
	// CFrameWnd:CMDIChildWnd:CFrameChild
	CFrameChild* pFrameChild = DYNAMIC_DOWNCAST(CFrameChild, pFrame); 
	ASSERT_VALID(pFrameChild);

	// Delegate to implementation in CFrameWnd
	pFrame->InitialUpdateFrame(pDoc, bMakeVisible);

	// Now we can set the splitter position
	BDoc* pNeoDoc = DYNAMIC_DOWNCAST(BDoc, pDoc);
	ASSERT_VALID(pNeoDoc);
	ULONG lngSplitterPos = pNeoDoc->GetSplitterPos();
	if (lngSplitterPos)
		pFrameChild->SetSplitterPos(lngSplitterPos);

}

