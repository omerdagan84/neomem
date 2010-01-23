
// CClock
// This static GUI control will display date, time, etc.
//-----------------------------------------------------------------------------------------------------------------



#pragma once


//class CClock : public CStatic
class CClock : public CWnd
{
// Construction
public:
	CClock();
	virtual ~CClock();

// enums
	enum eTimerModes {tmNormal, tmStopwatch, tmCountdown};

// Operations
public:
	int GetWidth();
	void ShowPopup();
	BOOL StartTimer(eTimerModes nTimerMode, UINT nMilliseconds);
	BOOL Create(CRect& r, CWnd* pParentWnd);

// Attributes
private:
	COLORREF m_clrBackground;
	COLORREF m_clrBackShadow;
	COLORREF m_clrBackHighlight;
	COLORREF m_clrForeground;
	CFont m_font;
	CRect m_rControlSize;
	CRect m_rDraw;
	CRect m_rMargins;
	UINT m_nDrawTextFormat;

	CTime m_time;

	UINT m_nTimerID;
	CString m_strClockDisplay; 
	UINT m_nNormalInterval;
	UINT m_nCountdownInterval;
	UINT m_nStopwatchInterval;
	BOOL m_bPauseTimer; // pause timer if true
	UINT m_nCountdownTimer; // cumulative milliseconds
	UINT m_nStopwatchTimer; // cumulative milliseconds
	UINT m_nDefaultCountdownMin; // default start for countdown timer
	UINT m_nDefaultCountdownSec; 

	eTimerModes m_nTimerMode;

	//. alarms
	COleDateTime m_odtAlarm;


// Implementation
private:
	void AlertUser();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClock)
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CClock)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClockCountdown();
	afx_msg void OnClockStopwatch();
	afx_msg void OnClockNormal();
	afx_msg void OnClockPause();
	afx_msg void OnClockReset();
	afx_msg void OnClockStop();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClockAlarm();
	afx_msg void OnDestroy();
	afx_msg void OnClockHelp();
	afx_msg void OnClockFormat();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


