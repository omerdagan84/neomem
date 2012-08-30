
// CRichEditCtrlEx



#include "precompiled.h"
#include "NeoMem.h"
#include "RichEditCtrlEx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





// Helpers
/*
_AFX_RICHEDITEX_STATE::_AFX_RICHEDITEX_STATE()
{
	m_hInstRichEdit20 = NULL ;
}

_AFX_RICHEDITEX_STATE::~_AFX_RICHEDITEX_STATE()
{
	if (m_hInstRichEdit20 != NULL)
	{
		::FreeLibrary(m_hInstRichEdit20);
	}
}

_AFX_RICHEDITEX_STATE _afxRichEditStateEx;
*/


/*
// Initialize richedit control library
//* This function must be called prior to creating any dialog which uses the version 
// 2.0 rich edit control.  For created classes, this is called automatically.
// NOTE: This function will not work if AfxInitRichEdit() has been called first and 
// this will clash with the use of the CRichEditCtrl class (including the 
// CRichEditView class).
// Returns True if successful.
BOOL PASCAL AfxInitRichEditEx()
{
	if (!::AfxInitRichEdit())
	{
		return FALSE;
	}
	
	_AFX_RICHEDITEX_STATE* pState = &_afxRichEditStateEx;
	
	if (pState->m_hInstRichEdit20 == NULL)
	{
		pState->m_hInstRichEdit20 = LoadLibraryA("RICHED20.DLL"); // Contains rich edit 2.0 and 3.0
	}
	
	return pState->m_hInstRichEdit20 != NULL;
}
*/




// Macros
//---------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CRichEditCtrlEx, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRichEditCtrlEx)
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




// Constructor/Destructor
//---------------------------------------------------------------------------------------------------

CRichEditCtrlEx::CRichEditCtrlEx()
{
//	m_pRichEditOle = NULL;
//	m_lpObject = NULL;
//	m_lpStorage = NULL;
//	m_lpClientSite = NULL;
}

CRichEditCtrlEx::~CRichEditCtrlEx()
{
	xTRACE("CRichEditCtrlEx::Destructor\n");
}




BOOL CRichEditCtrlEx::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Make sure it's multiline
	cs.style |= ES_MULTILINE;
	
	return CRichEditCtrl::PreCreateWindow(cs);
}



BOOL CRichEditCtrlEx::Create(
//							 LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
//							 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
//							 UINT nID, CCreateContext* pContext) 
					DWORD dwStyle, const RECT& rcRect, CWnd* pParentWnd, UINT nID)
{
	// This is almost exactly what is in CRichEditCtrl::Create(), except the different window class 
	// name, and the call to AfxInitRichEdit() has been changed to AfxInitRichEditEx() which first 
	// calls AfxInitRichEdit (we do not want to break the normal CRichEditCtrl).   
//	if (!::AfxInitRichEdit())
//	if (!::AfxInitRichEditEx())
	if (!::AfxInitRichEdit2())
		return FALSE;

	
	// Requires 2.0 - if not available this will fail and return FALSE
	CWnd* pWnd = this;
	BOOL bRet = pWnd->Create(_T("RichEdit20A"), NULL, dwStyle, rcRect, pParentWnd, nID);
	if (bRet == FALSE)
	{
		//. standardize error msg handling
		AfxMessageBox("NeoMem requires RichEdit control version 2.0 or above, which your system does not have. "
			"", MB_ICONINFORMATION);
		return FALSE;
	}
	

		// Check if we're using the Unicode version of the rtf control or the Ansi version?
	// Use different str routines based on this value?
//	m_bUnicode = ::IsWindowUnicode(m_hWnd);

	// Paragraph markers: 
	// Rich Edit 1.0 used CRLF character combinations for paragraph markers. 
	// Rich Edit 2.0 used only a carriage return character ('\r'). 
	// Rich Edit 3.0 uses only a carriage return character but can emulate Rich Edit 1.0 in this regard. 
	//* might need to do something with this info eventually
	// could this be why cut and paste get messed up sometimes? 

	// Set length limit to 64K (defaults to 32K)
//	LimitText(64 * 1024);

	// Make sure we're in single code page mode (avoids problems with mangling of international 
	// characters, apparently)
	// Q. Locale characters badly interpreted by RichEdit 2.0
	// Since long time I was trying to port my editor code to use 
	// RichEdit 2.0. But there is still the same problem: 
	// RichEdit 2.0 somehow converts locale characters like, 
	// crossed out "l" or "a" and "e" with hook. I was trying 
	// everything I know, and still only RichEdit 1.0 works fine. 
	// The same problem appears under WordPad (For NT). IMPORTANT 
	// this only happens when using English version of operating 
	// system (but still RichEdit 1.0 works correctly). 
	// A. RichEdit 2.0 (and 3.0) uses Unicode characters for internal text representation. 
	// And sometimes it mangles the text. I faced with this when using RichEdit 3.0, 
	// but I suppose this issue is also true for version 2.
	// To solve this problem I just turned off RichEdit's support for multiple codepages 
	// with the following code:
	// int style = SendMessage(RichEditHandle, EM_GETTEXTMODE, 0, 0);
	// style = (style & ~TM_MULTICODEPAGE) | TM_SINGLECODEPAGE;
	// SendMessage(RichEditHandle, EM_SETTEXTMODE, style, 0);
	// In my case this works pretty nice.
	int nStyle = this->SendMessage(EM_GETTEXTMODE, 0, 0);
	nStyle = (nStyle & ~TM_MULTICODEPAGE) | TM_SINGLECODEPAGE;
	this->SendMessage(EM_SETTEXTMODE, nStyle, 0);
	
	return TRUE; // ok
//	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}






class RtfCallbackCookie
{
public:
//	CString& m_rstr; // used for get
	CString m_str; // used for get
	LPCTSTR m_psz; // used for set
	BOOL m_bGet;
	DWORD m_dwError;
	long m_nCurPos;
	long m_nCharsLeft;
	// Constructors
	RtfCallbackCookie() {m_psz = 0; m_bGet = FALSE; m_dwError = 0; m_nCurPos = 0; m_nCharsLeft = 0;}
//	RtfCallbackCookie(CString& rstr, BOOL bGet) : m_rstr(rstr) {m_psz = 0; m_bGet = bGet; m_dwError = 0;}
//	RtfCallbackCookie(CString str, BOOL bGet) : m_str(str) {m_psz = 0; m_bGet = bGet; m_dwError = 0;}
};



// Move text into and out of rtf control.
// Need most speed for SetRtf since that gets called a lot more (for search, navigation, etc).
//, make this code more efficient
// dwCookie - user-defined value
// pbBuff - data buffer
// cb - number of bytes to read or write
// pcb - number of bytes transferred
//
// cb - specifies the number of bytes requested by the rich edit control. 
// pcb - pointer to a long value. The callback function should set this 
// value to the number of bytes actually copied to the buffer. If this 
// value is less than the number of bytes requested by the control, then 
// the control assumes that there is no more text available and it will 
// stop calling this function. 
static DWORD CALLBACK EditStreamCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	xTRACE("EditStreamCallBack\n");

	// Get our callback object
	RtfCallbackCookie* pcc = (RtfCallbackCookie*) dwCookie;

	if (pcc->m_bGet) // GetRtf - get rtf text from the control
	{
		// Get uses CString
		// Copy data from rtf buffer to CString
		//, this allocates cb bytes, then copies the data in buffer to the cstring
		CString strAppend((LPCTSTR) pbBuff, cb);
		//, this allocates a new memory block, then copies the two cstrings to it		
		pcc->m_str += strAppend;
	}
	else // SetRtf - add more rtf text to the control
	{
		// Set uses LPCTSTR m_psz instead of cstring
		LPCTSTR pszStart = pcc->m_psz + pcc->m_nCurPos; // start character
		if (pcc->m_nCharsLeft >= cb)
		{
			*pcb = cb; // number of characters to copy
			memcpy(pbBuff, pszStart, *pcb); // copy chars
			pcc->m_nCurPos += cb;
			pcc->m_nCharsLeft -= cb;

//			CString s((LPCTSTR) pszStart, cb);
//			TRACESTRING("  Set %s\n", (LPCTSTR) s);
		}
		else
		{
			// We don't have the requested number of chars so just copy what we have.
			*pcb = pcc->m_nCharsLeft; // number of characters to copy
			memcpy(pbBuff, pszStart, *pcb); // copy chars
			pcc->m_nCurPos += *pcb;
			pcc->m_nCharsLeft = 0;
//			ASSERT(*(pcc->m_psz + pcc->m_nCurPos) == 0); // should point to null char at end of source string
//			CString s((LPCTSTR) pszStart, *pcb);
//			TRACESTRING("  Set %s\n", (LPCTSTR) s);
		}

	}

	// The callback function should return zero to indicate success.
	return 0;
}



// Set the rich edit text for this control, optionally replacing the current 
// selection only. 
void CRichEditCtrlEx::SetRtf(LPCTSTR pszRtf, BOOL bSelectionOnly /* = FALSE */)
{
	xTRACE("CRichEditCtrlEx::SetRtf\n");

	// Turn drawing off
	::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);

	// Check if the text is rtf or plain
	if (_tcsncmp(pszRtf, "{\\rtf", 5) != 0)
	{
		// we just have plain text, so use setwindowtext instead of streamin.
		// streamin is supposed to work with plain text via SF_TEXT, but it DOESN'T!
		this->SetWindowText(pszRtf);

		// then set the font to the default. not necessary! does this automatically. 
//		this->SetSel(0,-1); // select all
//		this->SetFont(&theApp.m_fontRtfDefault);
//		this->SetSel(-1,-1); // select none

/*
	// We use this CString's buffer if we need to modify the input string
	CString strRtf;
	// Add rtf codes if not there
	// wouldn't it be better to let the control do this? 
	// ie just add the text as PLAIN, not RTF?
	// well, yeah, but can't get that to work
		// Since we have to specify the font for the rtf, use the default font the user specified.
		// Note: using CString should be acceptable here because shouldn't need to do this very often.
		CFontEx& font = theApp.m_fontRtfDefault;
		CString strFontName = font.GetFontName();
		int nPointSize = font.GetPointSize();
		// 1.1 removed \n's from rtf header and footer - was it causing problems with pasting? perhaps
		// 1.1 removed spaces also - was causing problems with default font. maybe also with pasting?
		strRtf.Format(
			"{\\rtf1\\ansi\\deff0\\deftab720\\deflang1033" // header
			"{\\fonttbl{\\f0\\fswiss %s;}}"  // font name   //, fswiss? what if not swiss font?
			"{\\colortbl\\red0\\green0\\blue0;}" //, could set default color here
			"\\pard\\plain\\f0\\fs%d"  // fontsize = pointsize*2
			"{%s}" // plain text
			"}", // footer
			(LPCTSTR) strFontName, 
			nPointSize*2,
			pszRtf
			);
		// Now replace all cr/lf's with \par.
		strRtf.Replace("\r\n", "\\par ");
		//. how about plain \n's also? 
		// Get buffer (read-only)
		pszRtf = (LPCTSTR) strRtf;
*/
	}

	else

	{ 
		// handle rtf text - must be streamed in 

	//	RtfCallbackCookie cc(strRtf, FALSE);
		RtfCallbackCookie cc;
		cc.m_psz = pszRtf;

		//. could this be screwing things up? eg paste unicode chars?
		long nCharsRtf = _tcslen(pszRtf); // initialize number of characters in string
		cc.m_nCharsLeft = nCharsRtf;
		
		EDITSTREAM es = {0};
		es.dwCookie = (DWORD) &cc;
		es.dwError = 0;
		es.pfnCallback = EditStreamCallBack;

		// Stream in the characters
		// SFF_PLAINRTF - If specified, only keywords common to all languages are 
		// streamed in. Language-specific RTF keywords in the stream are ignored. 
		// If not specified, all keywords are streamed in. You can combine this 
		// flag with the SF_RTF flag.
//		int nFormat = SF_RTF;
		int nFormat = SF_RTF | SFF_PLAINRTF; // 1.2c see if this helps with crashing
		if (bSelectionOnly)
			nFormat |= SFF_SELECTION;

		// Although the documentation doesn't say so, you must NOT call
		// StreamIn() if you have nothing to send to the control (that is, 
		// if "sStr" is empty); if you do, you get dwError=0xfffffff1.
		// since we have rtf text, this shouldn't be a problem, (ie has rtf header).

		long nCharsStreamed = StreamIn(nFormat, es);
		// nChars streamed in should match the number of characters in pszRtf?
	//	ASSERT(nCharsStreamed == nCharsRtf); // this fails (streamed = eb, nchars = 2eb)
		// Bug: Was truncating some text because the text limit was at the default 32k
		// Need to always check error return values
		if (es.dwError != 0)
		{
			// 0xfffffff0 (-16) passed blank string, or plaintext, or invalidly 
			//		formatted rtf. 
			// 0x80030070 Insufficient disk space to complete operation.
			//		this happens if rtf LimitText is too small (default is 32k!)
			// 0x80070026 (-2147024858) Reached the end of the file. 
			//		this happens if you don't have a closing brace at the end of 
			//		the file, or there's a null byte in the text somewhere. 

			// windows doesn't seem to have error strings for richedit errors
//			CString strError = GetErrorMessage(es.dwError);
//			TRACE("   Error %s\n", (LPCTSTR) strError);
//			AfxMessageBox(strError);
//			cc.m_psz + cc.m_nC

			// get the rtf text that caused the problem. 
			LPCTSTR pszBad = pszRtf + nCharsStreamed - 10; 
			if (pszBad < pszRtf)
				pszBad = pszRtf;
			CString strBad = CString(pszBad);
			strBad = strBad.Left(30);

			// send error report with dwError code
			CString strMsg;
			strMsg.Format(
				"The richedit control had trouble loading the text for some reason.\r\n"
				"Error Code: 0x%x (%d).\r\n"
				"Offending Text: \r\n %s",
				es.dwError, 
				es.dwError,
				(LPCTSTR) strBad
				);
			Library::ThrowAssertion(THIS_FILE, __LINE__, "es.dwError != 0", (LPCTSTR) strMsg);

		
/*			//, try again as plain text
			// this doesn't work - it returns no error, but the rtf control is empty
			nFormat = SF_TEXT;
			nCharsStreamed = StreamIn(nFormat,es);
			if (es.dwError != 0)
			{
			}
*/
		}
	}

	// Turn drawing back on
	::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);
	::InvalidateRect(m_hWnd, 0, TRUE);
}





// Get rtf text from control.
// Note: We don't use const for this because it's not as important that this be fast.
CString CRichEditCtrlEx::GetRtf(BOOL bSelectionOnly /* = FALSE */)
{
	TRACE("CRichEditCtrlEx::GetRtf(bSelectionOnly = %d)\n", bSelectionOnly);

//	CString str;
//	RtfCallbackCookie cc(str, TRUE);
	RtfCallbackCookie cc;
	cc.m_bGet = TRUE;
	
	EDITSTREAM es;
	es.dwCookie = (DWORD) &cc;
	es.dwError = 0;
	es.pfnCallback = EditStreamCallBack;

	int nFormat = SF_RTF;
	if (bSelectionOnly)
		nFormat |= SFF_SELECTION;
	StreamOut(nFormat, es);
	ASSERT(es.dwError == 0); // make sure no error occurred

	return cc.m_str;
}





/*
void CRichEditCtrlEx::OnNotifyChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CRichEditCtrl::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	// TODO: Add your control notification handler code here
}
*/




// Find next occurrence of string, starting at end of current selection.
// Returns zero-based character position of next match, or -1 if not found.
long CRichEditCtrlEx::FindNext(CString& strFindText, BOOL bMatchCase, BOOL bWholeWord)
{
	// Search from end of current selection to end of doc
	CHARRANGE cr;
	GetSel(cr);
	cr.cpMin = cr.cpMax;
	cr.cpMax = -1;

	FINDTEXTEX ft;
	ft.chrg = cr;
	ft.lpstrText = strFindText.GetBuffer(0);
	DWORD dwSearchFlags = (bMatchCase ? FR_MATCHCASE : 0) | (bWholeWord ? FR_WHOLEWORD : 0);
	dwSearchFlags |= FR_DOWN; //` This is needed for RichEdit 2.0 and later!
	long nPos = FindText(dwSearchFlags, &ft);
	strFindText.ReleaseBuffer();

	// If text was found, select it
	if (nPos != -1)
	{
		cr = ft.chrgText; // range in which text is found
		// This function does not scroll the contents of the CRichEditCtrl so that the caret is visible.
		SetSel(cr);
    //,
//		long nLine = rRtf.LineFromChar(nIndex);
//		rRtf.LineScroll(
//		rRtf.SendMessage(EM_SCROLLCARET);
	}

	return nPos;
}



// See OnRButtonDown
void CRichEditCtrlEx::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	xTRACE("CRichEditCtrlEx::OnContextMenu(point.x,y=%d,%d)\n", point.x, point.y);	
}





// Fill a CFont object with currently selected font information.
BOOL CRichEditCtrlEx::GetCurrentFont(CFontEx &font)
{
	// Call this function to get the character formatting attributes of the current selection. 
	// The cf parameter receives the attributes of the first character in the current selection. 
	// The return value specifies which attributes are consistent throughout the selection.
	CHARFORMAT cf;
	DWORD dwMask = GetSelectionCharFormat(cf);
	return font.CreatePointFont2(cf);
}



// Returns True if current selection is at the beginning of a line.
//. should check if prev char is CR instead of looking at position, as might just be due to word wrapping
BOOL CRichEditCtrlEx::IsAtStartOfLine()
{
	long nStartChar;
	long nEndChar;
	GetSel(nStartChar, nEndChar); // gets current selection
	long nLine = LineFromChar(nStartChar);
	if (nStartChar != 0)
	{
		long nLine2 = LineFromChar(nStartChar - 1);
		if (nLine != nLine2)
			// If start character and previous character are on different lines, then must be at start of line
			return TRUE;
	}

	// The EM_LINEINDEX message retrieves the character index of the first character of a 
	// specified line in a multiline edit control. A wParam value of –1 specifies the current line 
	// number (the line that contains the caret). lParam not used.
	GetSel(nStartChar, nEndChar); // gets current selection
//	long nChar = ::SendMessage(m_hWnd, EM_LINEINDEX, (WPARAM) -1, 0);
//	bStart = (nChar == nStartChar);

	return FALSE;	
}



// Extended the FindText method so that it would save the... 
long CRichEditCtrlEx::FindText2(DWORD dwFlags, FINDTEXT* pFindText)
{
	return 0;
}




// Center the current selection vertically in the control.
// This is used by the search routines.
//, currently just works if text is below halfway point
void CRichEditCtrlEx::CenterSelectionVertically()
{
	// Get position of selection
	long nStartChar, nEndChar;
	GetSel(nStartChar, nEndChar);
	CPoint pt = GetCharPos(nStartChar);

	// If position not in center of screen scroll it one line at a time
	CRect r;
	GetClientRect(&r);
	int nDelta = (pt.y - r.bottom / 2);
	int nLines = 1; 
	// might already be scrolled as far as it will go
	while (nDelta > 0)
	{
		LineScroll(nLines);
		pt = GetCharPos(nStartChar);
		int nNewDelta = (pt.y - r.bottom / 2);
		if (nDelta == nNewDelta) break; // exit if no change from last time - can't scroll any more
		nDelta = nNewDelta;
	}

	// Scroll the calculated number of lines
//	LineScroll(nLines);
//	int nCurLine = LineFromChar(nStartChar);
//	int nCurLine = LineFromChar(-1); // get current line by passing - 1
//	int nFirstLine = GetFirstVisibleLine();
//	int nLines = 0;

}



// Turn auto URL detect on/off
// Requires 2.0
BOOL CRichEditCtrlEx::AutoURLDetect(BOOL bEnable)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL) ::SendMessage(m_hWnd, EM_AUTOURLDETECT, bEnable, 0);
}




/*

// get the length of the text in the control
unsigned long int TMyRichEdit::GetCurrentLength(void)
{
        // get the length of the text in the control
        // if using Rich Edit 1.0 (BCB 1.0 & 3.0 use RE 1.0)
        return ::SendMessage(Handle, WM_GETTEXTLENGTH, 0, 0);

        // if using Rich Edit 2.0, use this instead:
        // GETTEXTLENGTHEX gtlx = { GTL_PRECISE, CP_ACP };
        // return ::SendMessage(Handle, EM_GETTEXTLENGTHEX, (WPARAM) &gtlx, 0);
}

  
// get the line number of the current cursor position.
// returns the line number of the current cursor position in the RE control.
unsigned int TMyRichEdit::GetLine(void)
{
        CHARRANGE chrg;
        ::SendMessage(Handle, EM_EXGETSEL, 0, (LPARAM) &chrg);
        return ::SendMessage(Handle, EM_EXLINEFROMCHAR, 0, (LPARAM) chrg.cpMin);
}


// get the column (character offset) of the cursor position in the control.
// returns the column (character offset) of the cursor position in the RE control.
unsigned int TMyRichEdit::GetColumn(void)
{
        CHARRANGE chrg;
        ::SendMessage(Handle, EM_EXGETSEL, 0, (LPARAM) &chrg);
        return chrg.cpMin - ::SendMessage(Handle, EM_LINEINDEX, (WPARAM) GetLine(), 0);
}


// set the cursor position to a specific line in the control.
//      line - the line number (zero-based) to which the cursor will be moved.
void TMyRichEdit::SetLine(unsigned int line)
{
        CHARRANGE chrg;
        chrg.cpMin = chrg.cpMax = ::SendMessage(Handle, EM_LINEINDEX, (WPARAM) line, 0);
        if (chrg.cpMin == -1)   // past end of text so find last line
                chrg.cpMin = chrg.cpMax = GetCurrentLength();
        ::SendMessage(Handle, EM_EXSETSEL, 0, (LPARAM) &chrg);
}


// set the cursor position to a particular column (in the current line).
//      column - the column to which to move the cursor (zero-based).
void TMyRichEdit::SetColumn(unsigned int column)
{
        CHARRANGE chrg;
        unsigned int currLine = GetLine();
        unsigned int ndx = ::SendMessage(Handle, EM_LINEINDEX, (WPARAM) currLine, 0);
        unsigned int cols = ::SendMessage(Handle, EM_LINEINDEX, (WPARAM) currLine + 1, 0) - ndx - 1;
        if (column > cols) column = cols;
        chrg.cpMin = chrg.cpMax = ndx + column;
        ::SendMessage(Handle, EM_EXSETSEL, 0, (LPARAM) &chrg);
}

*/




void CRichEditCtrlEx::OnDestroy() 
{
	this->Clear();
//	ReleasePointers();

	CRichEditCtrl::OnDestroy();
}



/*
// Insert a bitmap into the richedit control.
bool CRichEditCtrlEx::InsertBitmap(CString strFile)
{
	// Get the interface to the CRichEditCtrl.
	m_pRichEditOle = this->GetIRichEditOle();
	ASSERT(m_pRichEditOle != NULL);

	// Create the object from the bitmap file,
	// the object is now stored in m_lpObject
	CreateFromFile(strFile);

	// Populate the RichEdit Object (REOBJECT) structure so it can call InsertObject 
	// through the IRichEditOle pointer, m_pRichEditOle. The following code fills this 
	// structure:
	REOBJECT reobject;
	::ZeroMemory(&reobject, sizeof(REOBJECT));
	reobject.cbStruct = sizeof(REOBJECT);

	CLSID clsid;
	SCODE sc = m_lpObject->GetUserClassID(&clsid);
	if (sc != S_OK) AfxThrowOleException(sc);

	reobject.clsid = clsid;
	reobject.cp = REO_CP_SELECTION;
	reobject.dvaspect = DVASPECT_CONTENT;
	reobject.dwFlags = REO_RESIZABLE | REO_BELOWBASELINE;
	reobject.dwUser = 0;
	reobject.poleobj = m_lpObject;

	ASSERT(m_lpClientSite != NULL);
	reobject.polesite = m_lpClientSite;

	ASSERT(m_lpStorage != NULL);
	reobject.pstg = m_lpStorage;

	SIZEL sizel;
	sizel.cx = sizel.cy = 0;
	reobject.sizel = sizel; 

	// At this point, make the InsertObject call. 
	m_pRichEditOle->InsertObject(&reobject);

	// Clean up pointers
	ReleasePointers();
	
	// Return the success of this function
	return true;
}


// Create an OLE object from a file.
// To use CreateFromFile, simply pass the full path to a bitmap as the file name parameter. 
// The function creates a byte array in global memory and a compound file storage object 
// in that memory. It then populates the FORMATETC structure, gets the client site, and 
// creates the object within the provided storage. The result is an IUnknown pointer to 
// the object that is then converted to a true IOleObject pointer by a call to 
// QueryInterface.
bool CRichEditCtrlEx::CreateFromFile(LPCTSTR lpszFileName)
{
	USES_CONVERSION;

	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT(m_lpStorage == NULL);
	ASSERT(m_lpClientSite == NULL);

	LPLOCKBYTES lpLockBytes = NULL;
	CLSID clsid = CLSID_NULL;
	OLERENDER render = OLERENDER_DRAW;
	CLIPFORMAT cfFormat = 0;
	LPFORMATETC lpFormatEtc = NULL;

	SCODE sc;

	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK) AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &m_lpStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(m_lpStorage != NULL);

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = &formatEtc;
	lpFormatEtc->cfFormat = cfFormat;
	lpFormatEtc->ptd = NULL;
	lpFormatEtc->dwAspect = DVASPECT_CONTENT;
	lpFormatEtc->lindex = -1;
	lpFormatEtc->tymed = TYMED_NULL;

	// attempt to create the object
	m_pRichEditOle->GetClientSite(&m_lpClientSite);

	sc = ::OleCreateFromFile(clsid, T2COLE(lpszFileName),
		IID_IUnknown, OLERENDER_DRAW, lpFormatEtc, m_lpClientSite, m_lpStorage,
		(void**)&m_lpObject);
	if (sc != S_OK)
		AfxThrowOleException(sc);

	// m_lpObject is currently an IUnknown, convert to IOleObject
	if (m_lpObject != NULL)
	{
		LPUNKNOWN lpUnk = m_lpObject;
		lpUnk->QueryInterface(IID_IOleObject, (void**)&m_lpObject);
		lpUnk->Release();
		if (m_lpObject == NULL) AfxThrowOleException(E_OUTOFMEMORY);
	}

	// all items are "contained" -- this makes our reference to this object
	// weak -- which is needed for links to embedding silent update.
	OleSetContainedObject(m_lpObject, TRUE);

	ASSERT_VALID(this);

	return TRUE;
}



void CRichEditCtrlEx::ReleasePointers()
{
	if (m_lpObject)
	{
		m_lpObject->Release();
		m_lpObject = NULL;
	}
	
	if (m_lpStorage)
	{
		m_lpStorage->Release();
		m_lpStorage = NULL;
	}
	
	if (m_lpClientSite)
	{
		m_lpClientSite->Release();
		m_lpClientSite = NULL;
	} 
	
	if(m_pRichEditOle)
	{
		m_pRichEditOle->Release();
		m_pRichEditOle = NULL;
	}
}
*/




// Requires 2.0
DWORD CRichEditCtrlEx::GetSelectionCharFormat2(CHARFORMAT2 &cf2) const
{
	ASSERT(::IsWindow(m_hWnd));
	cf2.cbSize = sizeof(CHARFORMAT2);
	return (DWORD)::SendMessage(m_hWnd, EM_GETCHARFORMAT, 1, (LPARAM)&cf2);
}


// Requires 2.0
DWORD CRichEditCtrlEx::Redo()
{
	ASSERT(::IsWindow(m_hWnd));
	return (DWORD)::SendMessage(m_hWnd, EM_REDO, 0, 0);
}


// Requires 2.0
DWORD CRichEditCtrlEx::CanRedo()
{
	ASSERT(::IsWindow(m_hWnd));
	return (DWORD)::SendMessage(m_hWnd, EM_CANREDO, 0, 0);
}



void CRichEditCtrlEx::SetDefaultCharFormat2(CHARFORMAT2 cf2)
{
	// Set default char format (pass wparam 0 to set default)
	LRESULT ret = ::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM) &cf2);
	VERIFY(ret != 0); // r == 0 indicates error
}


void CRichEditCtrlEx::Spike()
{
	CString str;
	theApp.GetTextFromClipboard(str);
	str += this->GetSelText();
	this->ReplaceSel("", TRUE);
	theApp.CopyToClipboard(str);
}



// static 
bool CRichEditCtrlEx::Test()
{
	CRichEditCtrlEx rtf;

	TRACE("create invisible rtf control\n");
	rtf.Create(WS_CHILD | ES_MULTILINE, CRect(0,0,0,0), AfxGetMainWnd(), 100);
	rtf.LimitText(0xffffff);  // 16MB // this sends EM_LIMITTEXT message

	CString s = "hello!";
	rtf.SetRtf(s);
	s = rtf.GetRtf();
	rtf.GetWindowText(s);

	// this will bomb
s = "{\\rtf\\smeep";
	rtf.SetRtf(s);
	s = rtf.GetRtf();

/*
//!!
s = ""; // will bomb! -16
s = "gloop"; // will bomb! -16
s = "{\\rtf1}"; // ok (blank)
s = "{\\rtf1\\laijsdliajs}"; // ok(!)
s = "{\\rtf\\smeep}"; // ok
s = "{\\rtf1\\ansi\\deff0\\deftab720\\deflang1033" // header
	"{\\fonttbl{\\f0\\fswiss Arial;}}"  // font name
	"{\\colortbl\\red0\\green0\\blue0;}" //, could set default color here
	"\\pard\\plain\\f0\\fs24"  // fontsize = pointsize*2
	"{ok were going to just add a bunch of text so be patient"
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" // 100 chars
	"12345" 
//	"\000" // bombs...
	"no!!}"
	"}"; // all ok
*/

	
	return true;
}



