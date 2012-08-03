
// CDialogTimer
//-----------------------------------------------------------------------------------------------------------------



#pragma once



class CDialogTimer : public CDialog
{
// Construction
public:
	CDialogTimer(CWnd* pParent = NULL);   // standard constructor

// Public Attributes
public:
	UINT	m_nHours;
	UINT	m_nMin;
	UINT	m_nSec;

// Dialog Data
private:
	//{{AFX_DATA(CDialogTimer)
	enum { IDD = IDD_TIMER };
	CSliderCtrl	m_slideHours;
	CEdit	m_txtHours;
	CSpinButtonCtrl	m_spinHours;
	CEdit	m_txtSec;
	CEdit	m_txtMin;
	CSpinButtonCtrl	m_spinSec;
	CSpinButtonCtrl	m_spinMin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTimer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogTimer)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




