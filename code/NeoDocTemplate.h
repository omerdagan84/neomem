
// BDocTemplate
// Extension to CMultiDocTemplate class
//-----------------------------------------------------------------------------------------------------------------

#pragma once


#include "FrameChild.h"


class BDocTemplate : public CMultiDocTemplate
{
	DECLARE_DYNAMIC(BDocTemplate)

public:
	BDocTemplate::BDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	virtual ~BDocTemplate();

// Attributes
public:

// Operations
public:
//	CFrameChild* GetMDIChildFrame();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BDocTemplate)
	public:
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);
	virtual void InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc, BOOL bMakeVisible = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(BDocTemplate)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


