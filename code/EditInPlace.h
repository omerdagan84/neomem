
// CEditInPlace
// This is an extension of the edit control which is used to edit values in the
// Contents View and Properties View. 
// Adapted from various CodeGuru articles.
//-----------------------------------------------------------------------------------------------------------------

// Like the built in edit control for the first column, our edit control also sends the LVN_ENDLABELEDIT 
// notification when the edit is completed. If this notification message isn’t already being handled, 
// add a handler so that any changes made with the edit control can be accepted. 

// We need to subclass the CEdit class to provide for our special requirement. 
// The main requirements placed on this class is that 
// It should send the LVN_ENDLABELEDIT message when edit is complete 
// It should expand to accommodate the text 
// It should destroy itself when the edit is complete 
// The edit should be terminated when the user presses the Escape or the Enter key or 
// when the edit control loses focus. 

// The reason I caught some events in the KeyDown, some in the KeyUp, and some in the OnChar, 
// had to do with how the application behaved. If I would have caught the VK_PRIOR or VK_NEXT 
// in the OnKeyDown, I would get several calls depending on how long the button was pressed. 
// This is very annoying. If you don't believe me, move the code, add several items to the list and 
// try it (it isn't pretty). 

// I also caught the ESC key so that I could cancel an edit. The return key is being used to end an edit 
// and when it is on the last record, it insert a new one. Just don't forget to override the 
// OnGetDlgCode() so that you can get the arrow keys and tabs. 





#pragma once


#include "ListCtrlEx.h"



class CEditInPlace : public CEdit
{
// Construction
public:
	CEditInPlace(CListCtrlEx* plvw, int iItem, int iSubItem, CString sInitText);
	virtual ~CEditInPlace();

// Operations
public:

// Attributes
public:
private:
	CListCtrlEx* m_plvw;
	int m_iItem;
	int m_iSubItem;
	CString m_sInitText;
	BOOL m_bEscape;	 	// To indicate whether ESC key was pressed

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditInPlace)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	UINT OnGetDlgCode();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditInPlace)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};





