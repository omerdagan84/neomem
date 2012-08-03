
// CPropertyPageEx2
// Extension of the CPropertyPageEx class.
//-----------------------------------------------------------------------------------------------------------------

//. delete this class



#pragma once



class CPropertyPageEx2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropertyPageEx2)

// Construction
public:
	CPropertyPageEx2();
	~CPropertyPageEx2();

// Dialog Data
private: 
	//{{AFX_DATA(CPropertyPageEx2)
	enum { IDD = IDD_ERROR };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropertyPageEx2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropertyPageEx2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



