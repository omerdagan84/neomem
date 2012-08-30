
// CClock



#include "precompiled.h"

#include "neomem.h"
#include "Clock.h"
#include "DialogEditDate.h" //.
#include "DialogTimer.h"
#include "FrameMain.h" //.

//#include "help\HelpIDs.h"
#include "HelpIDs.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//BEGIN_MESSAGE_MAP(CClock, CStatic)
BEGIN_MESSAGE_MAP(CClock, CWnd)
	//{{AFX_MSG_MAP(CClock)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_COMMAND(ID_CLOCK_COUNTDOWN, OnClockCountdown)
	ON_COMMAND(ID_CLOCK_STOPWATCH, OnClockStopwatch)
	ON_COMMAND(ID_CLOCK_NORMAL, OnClockNormal)
	ON_COMMAND(ID_CLOCK_PAUSE, OnClockPause)
	ON_COMMAND(ID_CLOCK_RESET, OnClockReset)
	ON_COMMAND(ID_CLOCK_STOP, OnClockStop)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CLOCK_ALARM, OnClockAlarm)
	ON_WM_DESTROY()
	ON_COMMAND(ID_CLOCK_HELP, OnClockHelp)
	ON_COMMAND(ID_CLOCK_FORMAT, OnClockFormat)
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




CClock::CClock() :
	m_nTimerID(0),
	m_nTimerMode(tmNormal),
	m_bPauseTimer(FALSE),
	m_clrBackground(0x00888888),
	m_clrForeground(0x00ffffff),
	m_clrBackHighlight(m_clrBackground + 0x00202020),
	m_clrBackShadow(m_clrBackground + 0x00202020)
{
}


CClock::~CClock()
{
	xTRACE("cclock destructor\n");
	// Bug: Don't put this here because by the time it gets called the window is gone, 
	// and it will screw up MFC - wound up not calling ExitInstance
//	if (m_nTimerID) 
//		KillTimer(m_nTimerID);
}



BOOL CClock::Create(CRect& r, CWnd* pParentWnd)
{
	// Call base class to create window
	if (!CWnd::Create(NULL, "", WS_CHILD | WS_VISIBLE, r, pParentWnd, -1))
		return FALSE;

	// Create font
	m_font.CreatePointFont(85, "Tahoma");

	// Initialize rectangles
	m_rControlSize.SetRectEmpty();
	m_rDraw.SetRectEmpty();
	m_rMargins.SetRect(1, 1, 1, 1);

	// Set text output format
	m_nDrawTextFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	// Define intervals in milliseconds
	m_nNormalInterval = 60 * 1000; // 60 sec = 1 min
	m_nCountdownInterval = 1000; // 1 sec
	m_nStopwatchInterval = 100; // 0.1 sec

	//, could save this in registry
	m_nDefaultCountdownMin = 0;
	m_nDefaultCountdownSec = 30; 

	//.
	m_odtAlarm.SetDateTime(2003, 07, 21, 19, 03, 00);


	// Create clock timer
	StartTimer(tmNormal, m_nNormalInterval);

	return TRUE;
}



void CClock::OnSize(UINT nType, int cx, int cy) 
{
	xTRACE("Clock on size\n");

	CWnd::OnSize(nType, cx, cy);
	
	// Save width and height of control
	// Note: If the main window is maximized the control goes all the way to the edge of the screen,
	// so shrink it a bit on the right side
	m_rControlSize.SetRect(0, 0, cx, cy);
//	if (theApp.IsMaximized())
//		m_rControlSize.right -= 2; 

	// Get draw rectangle also
	m_rDraw = m_rControlSize;
	m_rDraw.DeflateRect(&m_rMargins);

}



// Note: m_strClockDisplay has been updated in OnTimer
void CClock::OnPaint() 
{
	xTRACE("Clock on paint\n");

	CPaintDC dc(this); // device context for painting
	
	CFont* poldfont = dc.SelectObject(&m_font);

	// could draw to memory dc, then blit it to stop flickering
	// or just be careful with order of these statements...
	dc.SetTextColor(m_clrForeground);
	dc.SetBkColor(m_clrBackground);
	dc.SetBkMode(TRANSPARENT); 

	// Fill background and add 3d border
	dc.FillSolidRect(m_rControlSize, m_clrBackground);
	dc.Draw3dRect(m_rControlSize, m_clrBackShadow, m_clrBackHighlight);

	// Draw text
	//, use winapi for speed
	dc.DrawText(m_strClockDisplay, m_rDraw, m_nDrawTextFormat);

	//, draw little buttons
	//, draw any little icons - like alarm light, etc
	//, draw countdown/stopwatch separately?
	dc.SelectObject(poldfont);

	// Note: Do not call CWnd::OnPaint() for painting messages
}




//-----------------------------------------------------------------------------------------------------------------


// Right click menu

void CClock::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ShowPopup();
	CWnd::OnLButtonDown(nFlags, point);
}

void CClock::OnRButtonDown(UINT nFlags, CPoint point) 
{
	ShowPopup();	
	CWnd::OnRButtonDown(nFlags, point);
}



// Show popup menu
void CClock::ShowPopup()
{
	BCMenu menu;
	CRect r;
	GetWindowRect(r);
	int xScreen = r.left;
	int yScreen = r.top;
	if (menu.LoadMenu(IDR_POPUP_CLOCK))
	{
		menu.LoadToolbar(IDR_MAINFRAME);
		menu.LoadToolbar(IDR_TOOLBAR_OTHER);
		CMenu* pPopup = menu.GetSubMenu(0);
		if (pPopup)
		{
//			BOOL bNoModifyClass = m_pobjPopup->GetFlag(flagNoModifyClass);
//			pPopup->EnableMenuItem(ID_OBJ_CHANGE_OBJECT_CLASS, bNoModifyClass ? MF_GRAYED : MF_ENABLED);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, xScreen, yScreen, this);
		}
	}	
}




// Start stopwatch
void CClock::OnClockStopwatch() 
{
	AfxMessageBox("Click OK to start Stopwatch...");
	m_nStopwatchTimer = 0;
	StartTimer(tmStopwatch, m_nStopwatchInterval);
}


// Start countdown timer
void CClock::OnClockCountdown() 
{
	// Bring up dialog to let user enter time to countdown from
	CDialogTimer dlg;
	dlg.m_nMin = m_nDefaultCountdownMin;
	dlg.m_nSec = m_nDefaultCountdownSec;
	if (dlg.DoModal())
	{
		m_nCountdownTimer = (dlg.m_nMin * 60 + dlg.m_nSec) * 1000; // don't forget to put it into ms!
		// save as default for next time also
		m_nDefaultCountdownMin = dlg.m_nMin;
		m_nDefaultCountdownSec = dlg.m_nSec; 
		StartTimer(tmCountdown, m_nCountdownInterval); 
	}
}


// Return clock to normal mode
void CClock::OnClockNormal() 
{
	StartTimer(tmNormal, m_nNormalInterval); 
}


// Pause / restart timer
void CClock::OnClockPause() 
{
	m_bPauseTimer = !m_bPauseTimer;	
}


// Reset timer
void CClock::OnClockReset() 
{
	m_nStopwatchTimer = 0;
}


// Stop timer
void CClock::OnClockStop() 
{
	m_bPauseTimer = TRUE;	
}




// Start the timer with the specified time interval and set the timer mode. 
BOOL CClock::StartTimer(eTimerModes nTimerMode, UINT nMilliseconds)
{
	m_nTimerMode = nTimerMode;
	m_bPauseTimer = FALSE;

	// Kill any old timer
	if (m_nTimerID)
		KillTimer(m_nTimerID);
	
	// Create timer
	// Timers are a limited global resource; therefore it is important that an application check the 
	// value returned by the SetTimer member function to verify that a timer is actually available.
	m_nTimerID = SetTimer(1, nMilliseconds, NULL);
	if (m_nTimerID == 0)
	{
		m_strClockDisplay = "No Timers available!";
		return FALSE;
	}

	// Call ontimer code to get clock text
	OnTimer(0);

	// Force redraw
	RedrawWindow();

	return TRUE;
}



// Handle timer event sent by Windows timer
// Note: This might be called by this code with eventid 0 
void CClock::OnTimer(UINT nIDEvent) 
{
	//, Check for alarms/reminders
	switch (m_nTimerMode)
	{
		case tmNormal:
			{
				// Get date/time into a string
				//, use crt routines for speed
				//, option - might just want date, not time also
				//, use smart code to prevent doing this when not needed!
//				m_time = CTime::GetCurrentTime();
//				CString str = m_time.Format(" %A, %B %#d, %Y"); // eg "Friday, March 5, 1999" 
//				CString str = m_time.Format(" %A, %B %#d, %Y  %I:%M:%S %p"); // eg "Friday, March 5, 1999  9:54:32 PM" 
//				m_strClockDisplay = m_time.Format(" %A, %B %#d, %Y  %#I:%M %p"); // eg "Friday, March 5, 1999  9:54 PM" 
//				m_strClockDisplay = m_time.Format(" %A, %B %#d, %Y "); // eg "Friday, March 5, 1999" 

//				COleDateTime dt;
//				dt = COleDateTime::GetCurrentTime();
//				m_strClockDisplay = CString(" ") + dt.Format() + CString(" "); // use system settings
//				m_strClockDisplay = dt.Format(VAR_DATEVALUEONLY); // use system settings (Short Date!)

				// Format codes:
				// %x  Date representation for current locale (short)
				// %X  Time representation for current locale (short)
				// %#x Long date representation, appropriate to current locale. For example: “Tuesday, March 14, 1995”. 
				// %#c Long date and time representation, appropriate for current locale. For example: “Tuesday, March 14, 1995, 12:41:29”. 
				// Bug: How fucked up is this - when it says uses short/long format, it means those that
				// it has defined in the C runtime library, NOT those set by the user in the control panel!???!
//				m_strClockDisplay = dt.Format(_T("%#x")); // Long date format for current locale (doesn't work!)

				// Get the current system time in Long Date format for the current locale
				// (this will actually use the Windows Long Date setting, unlike dt.Format!)
				const int nChars = 128;
				LPTSTR pszBuffer = m_strClockDisplay.GetBuffer(nChars);
				::GetDateFormat(NULL, DATE_LONGDATE, NULL, NULL, pszBuffer, nChars);
				m_strClockDisplay.ReleaseBuffer();

			}
			break;

		case tmStopwatch:
			if (!m_bPauseTimer)
			{
				m_strClockDisplay.Format("  Stopwatch:  %.1f sec", (float) m_nStopwatchTimer / 1000.0f);
				m_nStopwatchTimer += m_nStopwatchInterval;
			}
			break;

		case tmCountdown:
			if (!m_bPauseTimer)
			{
				m_strClockDisplay.Format("  Countdown Timer:  %d sec", m_nCountdownTimer / 1000);
				m_nCountdownTimer -= m_nCountdownInterval;
				if (m_nCountdownTimer <= 0)
				{
					m_nCountdownTimer = 0;
					m_bPauseTimer = TRUE;

					AlertUser();
				}
			}
			break;
	}

	// Check for alarms (if on minute!)


	// Refresh display
	RedrawWindow();

	// Call base class if this was received from Windows
	if (nIDEvent)
	{
		CWnd::OnTimer(nIDEvent);
	}
}








void CClock::AlertUser()
{
	// Play sound (if speaker available)
	// Bring up dialog (if option specified and user is not typing or anything)
	// Flash the window
	// Start a little blinking light in the clock - click to stop it / bring up message

	// Flashes the given window once. For successive flashing, create a system timer and repeatedly 
	// call FlashWindow. Flashing the CWnd means changing the appearance of its title bar as if the 
	// CWnd were changing from inactive to active status, or vice versa. (An inactive title bar changes 
	// to an active title bar; an active title bar changes to an inactive title bar.) 
	// Typically, a window is flashed to inform the user that it requires attention but that it does not 
	// currently have the input focus.
	// The bInvert parameter should be FALSE only when the window is getting the input focus and 
	// will no longer be flashing; it should be TRUE on successive calls while waiting to get the input focus. 
	// This function always returns nonzero for minimized windows. If the window is minimized, 
	// FlashWindow will simply flash the window’s icon; bInvert is ignored for minimized windows.
//	FlashWindow(TRUE);
//	::MessageBeep(MB_OK);
//	AfxMessageBox("Countdown finished!");
//	FlashWindow(FALSE);

	CFrameMain* pwnd = DYNAMIC_DOWNCAST(CFrameMain, AfxGetMainWnd());
	pwnd->AddIcon(2, IDI_ALARM1, "NeoMem Alarm - click to view", 0);

}




void CClock::OnClockAlarm() 
{
	// Get time for alarm
	
	CDialogEditDate dlg;

	dlg.m_dtDate = m_odtAlarm;
	dlg.m_dtTime = m_odtAlarm;
	dlg.m_bUseTime = TRUE;
	if (dlg.DoModal() == IDOK)
	{
		// Save new date and time
		COleDateTime& dtDate = dlg.m_dtDate;
		COleDateTime& dtTime = dlg.m_dtTime;
		m_odtAlarm.SetDateTime(dtDate.GetYear(), dtDate.GetMonth(), dtDate.GetDay(),
									dtTime.GetHour(), dtTime.GetMinute(), dtTime.GetSecond());
	}


}





void CClock::OnDestroy() 
{
	xTRACE("cclock ondestroy\n");
	CWnd::OnDestroy();
	
	if (m_nTimerID) 
		KillTimer(m_nTimerID);
}



// Get size of clock needed
int CClock::GetWidth()
{
	// should use long date format? yes
	//, update on sys change also
	COleDateTime odt;
	odt.SetDate(2000, 9, 30); // Wednesday, September 30, 2000 (a pretty long date)
//	CString strDate = odt.Format(VAR_DATEVALUEONLY);
	CString strDate = odt.Format("%#x"); // Long date format for current locale
	xTRACE("Clock width based on date '%s'\n", (LPCTSTR) strDate);

	CClientDC dc(this);
	CSize sz = dc.GetTextExtent(strDate);

	return sz.cx;
}



void CClock::OnClockHelp() 
{
	theApp.WinHelp(HID_CLOCK);
}



void CClock::OnClockFormat() 
{
	if (theApp.ShowMessage(0) == IDOK)
	{
		// Note: If user clicks Apply or OK Windows will fire the WM_SETTINGCHANGE message,
		// which this control will handle to redraw the time in the new format.
		::WinExec("Control.exe Intl.cpl, Regional Settings, 4", SW_NORMAL);
	}
}




void CClock::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CWnd::OnSettingChange(uFlags, lpszSection);
	
	// redraw time
	OnTimer(0);
}


