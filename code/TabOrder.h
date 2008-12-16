
// CTabOrder
// used by search view to implement tab order for our "pseudo"-controls. 
// ie we're handling moving between controls by hitting the tab key,
// instead of windows. 
//-----------------------------------------------------------------------------------------------------------------


#pragma once



class CTabOrder : public CUIntArray
{

// Construction
public:
	CTabOrder();
	virtual ~CTabOrder();

// Operations
public:
	UINT GetPrevID(UINT nID);
	int FindID(UINT nID);
	UINT GetNextID(UINT nID);


// Attributes
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabOrder)
	//}}AFX_VIRTUAL

};

