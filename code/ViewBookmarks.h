
// CViewBookmarks
// This view will store and display favorites/bookmarks. 
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "ViewEx.h"



class CViewBookmarks : public CViewEx
{
	DECLARE_DYNCREATE(CViewBookmarks)

		// Construction
protected:
	CViewBookmarks();           // protected constructor used by dynamic creation
	virtual ~CViewBookmarks();

// Operations
public:

// Attributes
public:

// Implementation

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewBookmarks)
	protected:
	//}}AFX_VIRTUAL

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewBookmarks)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




