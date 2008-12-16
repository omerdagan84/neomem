// SystemTray.cpp: implementation of the CSystemTray class.



#include "precompiled.h"
#include "neomem.h"
#include "SystemTray.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




CSystemTray::CSystemTray()
{
	m_nidIconData.cbSize = sizeof(NOTIFYICONDATA);
	m_nidIconData.hWnd = 0;
	m_nidIconData.uID = 1;
	m_nidIconData.uCallbackMessage = WM_TRAY_ICON_NOTIFY_MESSAGE;
	m_nidIconData.hIcon = 0;
	m_nidIconData.szTip[0] = 0;	
	m_nidIconData.uFlags = NIF_MESSAGE;
}

CSystemTray::~CSystemTray()
{
}



// NIM_MODIFY



