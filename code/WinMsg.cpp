



#include "precompiled.h"
#include "WinMsg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CWinMsg::CWinMsg()
{
}
CWinMsg::~CWinMsg()
{
}


// the msg structure contains information about the windows message that
// was being passed when the error occurred. 
// so parse it out into something readable. 
void CWinMsg::SetMsg(const MSG* pMsg)
{
	// store pointer to the structure
	m_pMsg = pMsg;

	// parse info into a string
	CString str;
	CString strWindow;
	CString strMessage;

	// Get window information
	// (possibly might not be an hwnd with the msg?)
	if (pMsg->hwnd)
	{
		// Get CWnd-derived object associated with message, if any.
		// Returns a pointer to a CWnd object when given a handle to a window. 
		// If a CWnd object is not attached to the handle, a temporary CWnd object 
		// is created and attached. 
		CWnd* pwnd = CWnd::FromHandle(pMsg->hwnd);
		CRuntimeClass* prc = pwnd->GetRuntimeClass(); // might just be a temporary object

		CString strWindowClass = prc->m_lpszClassName;
		int nWindowID = pwnd->GetDlgCtrlID();
		CString strWindowCaption;
		pwnd->GetWindowText(strWindowCaption);
		if (strWindowCaption.GetLength() > 20)
			strWindowCaption = strWindowCaption.Left(20) + _T("...");

		// yes, we're formatting for the rtf control... wheee...
		strWindow.Format(_T(
			"WindowClass: %s \r\n"
			"WindowID: %i \r\n"
			"WindowCaption: %s \r\n"
			),
			(LPCTSTR) strWindowClass, 
			nWindowID, 
			(LPCTSTR) strWindowCaption
			);
	}

	// Get message information
	//. would want to translate the numbers to strings, eventually.
	// though best kept out of the program, since it would just add bloat. 
	// though not very much, probably...
	UINT nMessageID = pMsg->message;
	if (nMessageID == WM_NOTIFY)
	{
		// Get notification information
		NMHDR* pnmh = (NMHDR*) pMsg->lParam;
		UINT nControlID = pnmh->idFrom;
		UINT nCode = pnmh->code;
		strMessage.Format(_T(
			"MessageID: WM_NOTIFY \r\n"
			"NotificationCode: %i \r\n"
			"ControlID: %i \r\n"
			),
			nCode, 
			nControlID
			);
	}
	else
	{
		// Get generic message information
		// Note: System reserves HIGH portion of nMessageID
		strMessage.Format(_T(
			"MessageID: %x \r\n"
			"wParam: %x \r\n"
			"lParam: %x \r\n"
			), 
			nMessageID, 
			pMsg->wParam, 
			pMsg->lParam
			);
	}

	// now get all in a string
	str = strWindow + strMessage;
	m_str = str;
}


CString CWinMsg::GetString()
{
	return m_str;
}
