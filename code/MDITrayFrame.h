
// CMDITrayFrame
// This is a frame window that lets you put an icon in the system tray. 
// Source: http://www.codeproject.com/KB/shell/YaTrayMin.aspx 
// (complete with a nasty bug. see Q195032)


#pragma once


#define WM_TRAY_ICON_NOTIFY_MESSAGE (WM_USER + 1)


class CMDITrayFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMDITrayFrame)

// Construction
public:

// Operations
public:
	BOOL AddIcon(UINT uID, UINT uIconID, LPCTSTR szTooltip, UINT uMenuID);
	BOOL HideIcon(UINT uID);
	BOOL UpdateIcon(UINT uID, UINT uIconID);

	BOOL TrayIsVisible();
	CMDITrayFrame();   // standard constructor
	
	virtual void OnTrayLButtonUp(UINT uID, CPoint pt);
	virtual void OnTrayLButtonDblClk(UINT uID, CPoint pt);
	
	virtual void OnTrayRButtonUp(UINT uID, CPoint pt);
	virtual void OnTrayRButtonDblClk(UINT uID, CPoint pt);

	virtual void OnTrayMouseMove(UINT uID, CPoint pt);
	

// Attributes
private:
	BOOL m_bMinimizeToTray;
	BOOL m_bTrayIconVisible;
	NOTIFYICONDATA m_nidIconData;
	CMenu m_mnuTrayMenu;
	UINT m_nDefaultMenuItem;
	CUIntArray m_auMenuIDs;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMDITrayFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);	
	private:
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam); // bug: had void instead of LRESULT! search Q195032
	//}}AFX_MSG

protected:
	DECLARE_MESSAGE_MAP()
};



