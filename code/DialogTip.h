
// CDialogTip
// Tip of the day dialog.
// (not used)
//-----------------------------------------------------------------------------------------------------------------



#pragma once


class CDialogTip : public CDialog
{
// Construction
public:
	CDialogTip(CWnd* pParent = NULL);   // standard constructor

// Operations
public:
	void GetNextTip();

// Public Attributes
public:
	CString m_strTip;
	BOOL	m_bShowTips;
	UINT	m_nLastTip;

// Dialog Data
private:
	//{{AFX_DATA(CDialogTip)
	enum { IDD = IDD_TIP };
	CStatic	m_lblInstructions;
	CStatic	m_staticBackground;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTip)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogTip)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnNextTip();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

