
// CToolBarCtrlEx
// Extension of the CToolBarCtrl class, modified to allow storing in a view instead of just
// in a dialog. 
// Adapted from MFC's CToolBarCtrl code.
//-----------------------------------------------------------------------------------------------------------------

// Note: MUST call Position() after creating the ToolBarEx control


// Bug: Used to crash on mousemove handling - I had commented out an ASSERT
// that seemed innocuous but wasn't.
// If you derive a class from this make sure to include DECLARE_DYNAMIC and IMPLEMENT_DYNAMIC - 
// that was what was causing the bad crashes (that the ASSERT would have caught).
// (Bad crash = move mouse over toolbar and get access violation, then would have to reboot
// because other programs were affected also)
// 2002-08-21 Still crashing sometimes



#pragma once



class CToolBarCtrlEx : public CToolBarCtrl
{
	DECLARE_DYNAMIC(CToolBarCtrlEx)

// Construction
public:
	CToolBarCtrlEx();
	virtual ~CToolBarCtrlEx();

// Operations
public:
	BOOL LoadToolBar(LPCTSTR lpszResourceName, int nExtraHeight = 0);
	void Position();
	BOOL GetItemRectFromID(UINT nID, CRect& r);
	void SetButtonInfo2(UINT nID, BYTE bytStyle, WORD wWidth);
	int IndexToCommand(int nItem); //`
	void SetButtonDropDown(UINT nButtonID); //`
	BOOL GetItemRectFromID(UINT nID, LPRECT lpRect);
	void SetButtonStyle(int nIndex, UINT nStyle);
	UINT GetButtonStyle(int nIndex) const;
protected:
	BOOL SetButtons(const UINT* lpIDArray, int nIDCount);
	void SetSizes(SIZE sizeButton, SIZE sizeImage);
	BOOL LoadBitmap(LPCTSTR lpszResourceName);
	BOOL AddReplaceBitmap(HBITMAP hbmImageWell);
	void AfxDeleteObject(HGDIOBJ* pObject);
	void _SetButton(int nIndex, TBBUTTON* pButton);
	void _GetButton(int nIndex, TBBUTTON* pButton) const;

// Attributes
protected:
	int m_nCount;
	BOOL m_bDelayedButtonLayout;
	SIZE m_sizeButton;
	SIZE m_sizeImage;
	HINSTANCE m_hInstImageWell;
	HRSRC m_hRsrcImageWell;
	HBITMAP m_hbmImageWell;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarCtrlEx)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
protected:
	//{{AFX_MSG(CToolBarCtrlEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateCmdUI(CView* pTarget, BOOL bDisableIfNoHndler);
//	afx_msg void OnToolTipText(NMHDR* pnm, LRESULT* result);
	DECLARE_MESSAGE_MAP()

};

