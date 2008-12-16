
// CEditEx
// This is an extension of the edit control which is used to customize how it handles
// certain events.
//-----------------------------------------------------------------------------------------------------------------



#pragma once


class CEditEx : public CEdit
{
// Construction
public:
	CEditEx();
	virtual ~CEditEx();

// Operations
public:

// Attributes
public:
	BOOL m_bWantReturn; // Set True if you want control to let user insert carriage returns using the Enter key.
	BOOL m_bHighlightOnEntry; // Set True for control to highlight all text on gaining focus.
	BOOL m_bReadOnly; // Set True to make control text read-only.

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditEx)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

