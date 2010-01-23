// Email.cpp: implementation of the CEmail class.
//
//////////////////////////////////////////////////////////////////////

#include "precompiled.h"
#include "neomem.h"
#include "Email.h"



#include "RegKey.h"


// for email:
typedef ULONG (FAR PASCAL *MAPIFUNC) (LHANDLE lhSession, ULONG ulUIParam, lpMapiMessage lpMessage, FLAGS flFlags, ULONG ulReserved);



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEmail::CEmail()
{
}

CEmail::~CEmail()
{
}


CEmail::CEmail(CString strToName, CString strToAddress, CString strSubject, CString strMessage) :
	m_strToName (strToName),
	m_strToAddress (strToAddress),
	m_strSubject (strSubject),
	m_strMessage (strMessage)
{
}




void CEmail::AskSend()
{
	if (IDYES == AfxMessageBox("This will open your email client to send an email to NeoMem.org. Continue?", MB_ICONQUESTION + MB_YESNO))
	{
		CString strURL;
		strURL.Format("mailto:%s", (LPCTSTR) theApp.m_strEmail); // "mailto:info@neomem.org"
//		HINSTANCE h = ::ShellExecute(NULL, "open", strURL, NULL, NULL, SW_SHOWNORMAL);

		CString strMessage;
		strMessage.Format(
					"\n\n\n\n"
					"The following information is optional: \n"
					"    NeoMem Version %s %s \n" // version and build 
//					"    Processor: %s \n" // processor
					"    %s \n" // windows version
					"    Common Controls Version %.2f \n" // common controls version
					"    Rich Edit Control Version %s" // rich edit version
//					"    Virtual Memory: %s \n" // memory info
//					"    Physical Memory: %s \n" // memory info
//					"    Screen: %s \n" // screen info
//					"    LocaleID: %x \n" // localeID
//					"    LanguageID: %x \n" // languageID
//					"    Locale: %s \n" // locale info
//					"    Disk Space Available: %s \n " // disk free info 
					"\n\n\n\n"
					,
					(LPCTSTR) theApp.m_objInfo.GetProgramVersion(),
					(LPCTSTR) theApp.m_objInfo.GetProgramBuild(),
//					(LPCTSTR) theApp.m_objInfo.GetProgramDate(),
//					(LPCTSTR) theApp.m_objInfo.GetProcessor(),
					(LPCTSTR) theApp.m_objInfo.GetWindowsVersion(),
					theApp.m_objInfo.GetCommonControlsVersionNumber(),
					(LPCTSTR) theApp.m_objInfo.GetRichEditVersionString()
//					(LPCTSTR) theApp.m_objInfo.GetVirtualMemory(),
//					(LPCTSTR) theApp.m_objInfo.GetPhysicalMemory(),
//					(LPCTSTR) theApp.m_objInfo.GetScreenInfo(),
//					nLocale,
//					nLanguage
//					(LPCTSTR) theApp.m_objInfo.GetLocaleInfo()
//					(LPCTSTR) theApp.m_objInfo.GetDiskSpaceAvailable()
					);

		m_strToName = "";
		m_strToAddress = theApp.m_strEmail;
		m_strSubject = "";
		m_strMessage = strMessage;
		Send();
	}
}





// Send email
// Note MAPIFUNC is defined up top...
// Bug: MAPI doesn't work with other email clients, like PocoMail - catch and use ShellExecute instead.
// Use MAPI/Outlook if it's the default, or just ShellExecute otherwise
void CEmail::Send() 
//(LPCTSTR pszToName, LPCTSTR pszAddress, LPCTSTR pszSubject, LPCTSTR pszText)
{

/*
	// See if user has a decent email client - some don't implement MAPI very well and won't
	// handle mailto: commands correctly, so just see if they're using outlook or outlook express.
	CRegKey r;
	r.Open(HKEY_LOCAL_MACHINE, "Software\\Clients\\Mail");
	CString strDefaultMailClient;
	r.GetString("", strDefaultMailClient);
	r.Close();
	// See if we're using Outlook or Outlook Express
	BOOL bGoodMapiClient = (strDefaultMailClient.Find("Outlook") != -1); // This is case-sensitive!
	if (!bGoodMapiClient) // bad mapi client, no biscuit 
	{
		// this goes haywire on amerinote laptop, calling up tons of ie screens.
		// probably a problem with screwy registry settings or something - works fine on desktop.
		HINSTANCE h = ::ShellExecute(NULL, "open", m_strToAddress, NULL, NULL, SW_SHOWNORMAL); 
		return;
	}
*/

	// Note: Even though there is an import library for MAPI, it apparently is not possible to call the 
	// MAPISendMail function directly. Hence the necessity of doing the LoadLibrary and GetProcAddress. 
	// This is the technique that MFC uses in CDocument::OnFileSendMail().
	// For further details, look up MAPISendMail in the on-line help. 
	const HINSTANCE hMAPILib = ::LoadLibrary(_T("MAPI32.DLL"));
//	const HINSTANCE hMAPILib = AfxLoadLibrary(_T("MAPI32.DLL"));
	if (hMAPILib)
	{
		MAPIFUNC lpMAPISendMail = (MAPIFUNC) GetProcAddress(hMAPILib, _T("MAPISendMail"));
		if (lpMAPISendMail)
		{
			// Make sure address is in proper format (eg "SMTP:info@neomem.org")
			CString strAddr = m_strToAddress;
			if (strAddr.Left(4) != _T("SMTP"))
				strAddr.Format(_T("SMTP:%s"), (LPCTSTR) m_strToAddress);

			// Recipient
			static MapiRecipDesc mrd;
			memset(&mrd, 0, sizeof(mrd));
			mrd.ulReserved = 0;
			mrd.ulRecipClass = MAPI_TO;
//			mrd.lpszName = _T("NeoMem Support");
//			mrd.lpszAddress = _T("SMTP:errors@neomem.org");
//			mrd.lpszAddress = pszAddress;
			mrd.lpszName = (LPTSTR) (LPCTSTR) m_strToName; //, note cast
			mrd.lpszAddress = (LPTSTR) (LPCTSTR) strAddr; //, note cast

			// Attached file
			//, for bugs, could attach file?
//			static MapiFileDesc mfd;
//			memset(&mfd, 0, sizeof(mfd));
//			mfd.nPosition = -1;
//			mfd.lpszPathName = _T("d:\\projects\\NeoMem\\_test.rtf");

			// Message
			static MapiMessage msg;
			memset(&msg, 0, sizeof(msg));
//			msg.lpszSubject  = _T("NeoMem");
//			msg.lpszNoteText = _T("Put message text here");
			msg.lpszSubject  = (LPTSTR) (LPCTSTR) m_strSubject; //, note cast!
			msg.lpszNoteText = (LPTSTR) (LPCTSTR) m_strMessage; //, note cast!
			msg.nRecipCount = 1;
			msg.lpRecips = &mrd;
//			msg.nFileCount = 1;
//			msg.lpFiles = &mfd;
			msg.nFileCount = 0;
			msg.lpFiles = 0;

			// See MAPISendMail
//			lpMAPISendMail(NULL, NULL, &msg, 0, 0);
//			lpMAPISendMail(NULL, NULL, &msg, (FLAGS) (MAPI_LOGON_UI), 0);
			lpMAPISendMail(NULL, NULL, &msg, (FLAGS) (MAPI_LOGON_UI | MAPI_DIALOG), 0);
		}
		else
		{
			AfxMessageBox(IDS_ERROR_MAPI_NOT_AVAILABLE, MB_ICONEXCLAMATION);
		}
		VERIFY(::FreeLibrary(hMAPILib));
//		VERIFY(AfxFreeLibrary(hMAPILib));
	}
	else
	{
		AfxMessageBox(IDS_ERROR_MAPI_NOT_AVAILABLE, MB_ICONEXCLAMATION);
	}
}

