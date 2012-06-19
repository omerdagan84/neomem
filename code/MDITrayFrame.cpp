
// CMDITrayFrame



#include "precompiled.h"
#include "MDITrayFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNAMIC(CMDITrayFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMDITrayFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMDITrayFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TRAY_ICON_NOTIFY_MESSAGE, OnTrayNotify)
END_MESSAGE_MAP()


//CMDITrayFrame::CMDITrayFrame(UINT uIDD,CWnd* pParent /*=NULL*/)
//	: CMDIFrameWnd(uIDD, pParent)
CMDITrayFrame::CMDITrayFrame() :
	m_bTrayIconVisible(FALSE),
	m_nDefaultMenuItem(0),
	m_bMinimizeToTray(FALSE)
{
	m_nidIconData.cbSize = sizeof(NOTIFYICONDATA);
//	m_nidIconData.cbSize = NOTIFYICONDATA_V1_SIZE; //` make sure we're using win95 compatible version
	m_nidIconData.szTip[0] = 0;	
	m_nidIconData.hWnd = 0;
	m_nidIconData.uID = 1;
	m_nidIconData.uCallbackMessage = WM_TRAY_ICON_NOTIFY_MESSAGE;
	m_nidIconData.hIcon = 0;
	m_nidIconData.uFlags = NIF_MESSAGE;
}



int CMDITrayFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_nidIconData.hWnd = this->m_hWnd;
	m_nidIconData.uID = 1;
	
	return 0;
}



void CMDITrayFrame::OnDestroy() 
{
	CMDIFrameWnd::OnDestroy();
	if (m_nidIconData.hWnd && m_nidIconData.uID > 0 && TrayIsVisible())
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nidIconData);
	}
}


BOOL CMDITrayFrame::TrayIsVisible()
{
	return m_bTrayIconVisible;
}




BOOL CMDITrayFrame::AddIcon(UINT uID, UINT uIconID, LPCTSTR szTooltip, UINT uMenuID)
{
	ASSERT(uID > 0);
	ASSERT(uIconID > 0);
	ASSERT(strlen(szTooltip) > 0 && strlen(szTooltip) < 64);

	m_nidIconData.uID = uID;
	m_nidIconData.uFlags = NIF_MESSAGE;

	// bug: system tray icon was messed up - bad colors and blotchy - deja said to use loadimage instead
	// of loadicon and it fixed it. also said to make sure the icon you use has just one size in it (the 16x16)
	// and 16 colors. 
	HICON hIcon = (HICON) ::LoadImage(AfxGetApp()->m_hInstance, 
								MAKEINTRESOURCE(uIconID), IMAGE_ICON, 16, 16, 0);
	if (hIcon)
	{
		m_nidIconData.hIcon = hIcon;
		m_nidIconData.uFlags |= NIF_ICON;
	}

	strcpy_s(m_nidIconData.szTip, szTooltip);
	m_nidIconData.uFlags |= NIF_TIP;

	m_auMenuIDs.SetAtGrow(uID, uMenuID);
	
	BOOL bSuccess = Shell_NotifyIcon(NIM_ADD, &m_nidIconData);
	return bSuccess;

}


BOOL CMDITrayFrame::UpdateIcon(UINT uID, UINT uIconID)
{
	ASSERT(uID > 0);
	ASSERT(uIconID > 0);

	m_nidIconData.uID = uID;

	HICON hIcon = AfxGetApp()->LoadIcon(uIconID);
	if (hIcon)
	{
		m_nidIconData.hIcon = hIcon;
		m_nidIconData.uFlags = NIF_ICON;
	}

	BOOL bSuccess = Shell_NotifyIcon(NIM_MODIFY, &m_nidIconData);
	return bSuccess;
}


BOOL CMDITrayFrame::HideIcon(UINT uID)
{
	ASSERT(uID > 0);
	m_nidIconData.uID = uID;
	BOOL bSuccess = Shell_NotifyIcon(NIM_DELETE, &m_nidIconData);
	return bSuccess;
}




// bug: 2009-12-24 had void instead of LRESULT. search Q195032. very nasty. 
// would cause weird crashes via stack corruption in release version, 
// not debug version. just moving the mouse over the tray icon 
// would have been enough to cause problems. 
// code from codeproject
LRESULT CMDITrayFrame::OnTrayNotify(WPARAM wParam, LPARAM lParam) 
{ 
    UINT uID; 
    UINT uMsg; 

    uID = (UINT) wParam; 
    uMsg = (UINT) lParam; 
 
	if (uID < 1) return NULL;
	
	CPoint pt;	

    switch (uMsg) 
	{ 
		case WM_MOUSEMOVE:
			GetCursorPos(&pt);
			ClientToScreen(&pt);
			OnTrayMouseMove(uID, pt);
			break;
		// bug: needed to catch up message, not down, as next icon could get other half of click!
//		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			GetCursorPos(&pt);
			ClientToScreen(&pt);
			OnTrayLButtonUp(uID, pt);
			break;
		case WM_LBUTTONDBLCLK:
			GetCursorPos(&pt);
			ClientToScreen(&pt);
			OnTrayLButtonDblClk(uID, pt);
			break;
		
//		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_CONTEXTMENU:
			GetCursorPos(&pt);
//			ClientToScreen(&pt);
			OnTrayRButtonUp(uID, pt);
			break;
		case WM_RBUTTONDBLCLK:
			GetCursorPos(&pt);
			ClientToScreen(&pt);
			OnTrayRButtonDblClk(uID, pt);
			break;
	} 
	return NULL; 
}


void CMDITrayFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
		CMDIFrameWnd::OnSysCommand(nID, lParam);
}



void CMDITrayFrame::OnTrayRButtonUp(UINT uID, CPoint pt)
{
	// Show popup menu, setting default item
	CMenu mnu;
	UINT uMenuID = m_auMenuIDs.GetAt(uID);
	if (mnu.LoadMenu(uMenuID))
	{
		mnu.GetSubMenu(0)->SetDefaultItem(m_nDefaultMenuItem, TRUE);
		mnu.GetSubMenu(0)->TrackPopupMenu(TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}

}


void CMDITrayFrame::OnTrayLButtonUp(UINT uID, CPoint pt)
{
	if (uID == 1)
	{
		HideIcon(1);
		this->ShowWindow(SW_SHOW);
	}
}


void CMDITrayFrame::OnTrayLButtonDblClk(UINT uID, CPoint pt)
{
}


void CMDITrayFrame::OnTrayRButtonDblClk(UINT uID, CPoint pt)
{
}


void CMDITrayFrame::OnTrayMouseMove(UINT uID, CPoint pt)
{
}









