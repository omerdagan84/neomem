
// CNeoDocTemplate
// Extension to CMultiDocTemplate class
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "FrameChild.h"



class CNeoDocTemplate : public CMultiDocTemplate
{
	DECLARE_DYNAMIC(CNeoDocTemplate)

public:
	CNeoDocTemplate::CNeoDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	virtual ~CNeoDocTemplate();

// Attributes
public:
//	CFrameChild* GetMDIChildFrame();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeoDocTemplate)
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
	//{{AFX_MSG(CNeoDocTemplate)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


