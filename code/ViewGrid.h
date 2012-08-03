
// CViewGrid
// This view will provide a mini-spreadsheet.
//-----------------------------------------------------------------------------------------------------------------



#pragma once

#include "ViewEx.h"


class CViewGrid : public CViewEx
{
	DECLARE_DYNCREATE(CViewGrid)

// Construction
protected:
	CViewGrid();           // protected constructor used by dynamic creation
	virtual ~CViewGrid();

// Operations
public:

// Attributes
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewGrid)
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
	//{{AFX_MSG(CViewGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};





