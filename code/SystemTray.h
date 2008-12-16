// SystemTray.h: interface for the CSystemTray class.


#pragma once



#define WM_TRAY_ICON_NOTIFY_MESSAGE (WM_USER + 1)



class CSystemTray  
{
public:
	CSystemTray();
	virtual ~CSystemTray();

private:
	NOTIFYICONDATA m_nidIconData;
	CMenu m_mnuTrayMenu;
	UINT m_nDefaultMenuItem;

};



