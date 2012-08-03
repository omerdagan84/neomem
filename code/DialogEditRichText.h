
// CDialogEditRichText
// This dialog will let user edit formatted text.
//-----------------------------------------------------------------------------------------------------------------



#pragma once



class CDialogEditRichText : public CDialog
{
// Construction
public:
	CDialogEditRichText(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
private:
	//{{AFX_DATA(CDialogEditRichText)
	enum { IDD = IDD_EDIT_RTF };
	CRichEditCtrl	m_rtf;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEditRichText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogEditRichText)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

