
// CEditEx



#include "precompiled.h"

#include "NeoMem.h"
#include "EditEx.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




CEditEx::CEditEx() :
	m_bHighlightOnEntry (FALSE),
	m_bReadOnly (FALSE),
	m_bWantReturn (FALSE)
{
}

CEditEx::~CEditEx()
{
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	//{{AFX_MSG_MAP(CEditEx)
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnSetFocus)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Swallow the message if readonly
	if (m_bReadOnly) 
		return; 
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Swallow the message if readonly
	if (m_bReadOnly) 
		return; 
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}


// The overridden PreTranslateMessage() is to ascertain that certain key strokes do make it to 
// the edit control. The escape key and the enter key are normally pre-translated by the CDialog 
// or the CFormView object, we therefore specifically check for these and pass it on to the edit 
// control. 
//, The check for GetKeyState(VK_CONTROL) makes sure that key combinations such as 
// Ctrl-C, Ctrl-V and Ctrl-X get forwarded to the edit control. 
BOOL CEditEx::PreTranslateMessage(MSG* pMsg) 
{
	//trace("CEditInPlace::PreTranslateMessage\n");
//	if (m_bSwallowReturn && pMsg->message == WM_KEYDOWN)
	if (pMsg->message == WM_KEYDOWN)
	{
		if (
			(m_bWantReturn && (pMsg->wParam == VK_RETURN)) ||
//			::GetKeyState(VK_CONTROL) // bug: was supposed to check for < 0, not <> 0!!
			(::GetKeyState(VK_CONTROL) < 0)
			)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE; // DO NOT process further
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}


// Highlight text on gaining focus
void CEditEx::OnSetFocus() 
{
	if (m_bHighlightOnEntry)
		SetSel(0, -1);
}


