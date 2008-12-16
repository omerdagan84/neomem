
#pragma once


class CPageClassAdvanced : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageClassAdvanced)

// Construction
public:
	CPageClassAdvanced();
	~CPageClassAdvanced();

// Dialog Data
	//{{AFX_DATA(CPageClassAdvanced)
	enum { IDD = IDD_CLASS_ADVANCED };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageClassAdvanced)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageClassAdvanced)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


