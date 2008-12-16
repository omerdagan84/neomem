
// CViewPictures
// This view will allow display and storage of image files. 
//-----------------------------------------------------------------------------------------------------------------



#pragma once

#include "ViewEx.h"


class CViewPictures : public CViewEx
{
	DECLARE_DYNCREATE(CViewPictures)

// Construction
protected:
	CViewPictures();           // protected constructor used by dynamic creation
	virtual ~CViewPictures();

// Operations
public:


// Attributes
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewPictures)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewPictures)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




