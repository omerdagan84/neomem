
// CEditInPlace



#include "precompiled.h"
#include "NeoMem.h"
#include "EditInPlace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CEditInPlace, CEdit)
	//{{AFX_MSG_MAP(CEditInPlace)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CEditInPlace::CEditInPlace(CListCtrlEx* plvw, int iItem, int iSubItem, CString sInitText) :
	m_sInitText(sInitText),
	m_bEscape (FALSE),
	m_iItem (iItem),
	m_iSubItem (iSubItem),
	m_plvw (plvw)
{
}

CEditInPlace::~CEditInPlace()
{
	xTRACE("CEditInPlace Destructor reached\n");
}


// The OnCreate() function creates the edit control and initializes it with the proper values. 
int CEditInPlace::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	xTRACE("CEditInPlace::OnCreate\n");

	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Set the proper font
	CFont* font = GetParent()->GetFont();
	SetFont(font);

	SetWindowText(m_sInitText);

	SetFocus();
//	SetActiveWindow();

	SetSel(0, -1); // select all text

	return 0;
}





// Message Handlers

// The overridden PreTranslateMessage() is to ascertain that certain key strokes do make it to 
// the edit control. The escape key and the enter key are normally pre-translated by the CDialog 
// or the CFormView object, we therefore specifically check for these and pass it on to the edit 
// control. The check for GetKeyState(VK_CONTROL) makes sure that key combinations such as 
// Ctrl-C, Ctrl-V and Ctrl-X get forwarded to the edit control. 
BOOL CEditInPlace::PreTranslateMessage(MSG* pMsg) 
{
	xTRACE("CEditInPlace::PreTranslateMessage\n");
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN
				|| pMsg->wParam == VK_DELETE
				|| pMsg->wParam == VK_ESCAPE
				|| GetKeyState(VK_CONTROL)
				)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE; // DO NOT process further
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}


// OnKillFocus() sends of the LVN_ENDLABELEDIT notification and destroys the edit control. The 
// notification is sent to the parent of the list view control and not to the list view control itself. 
// When sending the notification, the m_bEscape member variable is used to determine whether to 
// send a NULL string. 
void CEditInPlace::OnKillFocus(CWnd* pNewWnd) 
{
	xTRACE("CEditInPlace::OnKillFocus\n");

	// Call base class
	CEdit::OnKillFocus(pNewWnd);

	CString str;
	GetWindowText(str);

	// Send LVN_ENDLABELEDIT notification to parent of ListView ctrl
	NMLVDISPINFO nmdi;
	nmdi.hdr.hwndFrom = GetParent()->m_hWnd;
	nmdi.hdr.idFrom = GetDlgCtrlID();
	nmdi.hdr.code = LVN_ENDLABELEDIT;

	nmdi.item.mask = LVIF_TEXT;
	nmdi.item.iItem = m_iItem;
	nmdi.item.iSubItem = m_iSubItem;
	nmdi.item.pszText = m_bEscape ? NULL : LPTSTR((LPCTSTR)str); // Note cast removing const!!
	nmdi.item.cchTextMax = str.GetLength();

	xTRACE("   OnKillFocus: send LVN_ENDLABELEDIT to parent\n");
	GetParent()->GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&nmdi);

	xTRACE("  OnKillFocus: call DestroyWindow\n");
	DestroyWindow();

	xTRACE("  OnKillFocus: exit onkillfocus\n");
}


// The OnNcDestroy() function is the appropriate place to destroy the C++ object. 
void CEditInPlace::OnNcDestroy() 
{
	xTRACE("CEditInPlace::OnNcDestroy\n");
	
	// Call base class
	xTRACE("  ond: call CEdit::OnNcDestroy\n");
	CEdit::OnNcDestroy();

	// Delete this CEditInPlace object
	xTRACE("  ond: delete this\n");
	delete this;

	xTRACE("  ond: exit onncdestroy\n");
}



// The OnChar() function ends the edit if the Escape or the Enter key is pressed. It does this 
// by setting focus to the list view control which force the OnKillFocus() of the edit control to be called. 
// For any other character, the OnChar() function lets the base class function take care of it before 
// it tries to determine if the control needs to be resized. The function first gets the extent of the 
// new string using the proper font and then compares it to the current dimension of the edit control. 
// If the string is too long to fit within the edit control, it resizes the edit control after checking the 
// parent window (the list view control) to determine if there is space for the edit control to grow. 
void CEditInPlace::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    BOOL bShift = GetKeyState(VK_SHIFT) < 0;

	// Get current column order
	int nOrder = m_plvw->IndexToOrder(m_iSubItem);

    switch (nChar)
    {
		case VK_ESCAPE:
		{
			if (nChar == VK_ESCAPE)
				m_bEscape = TRUE;
			GetParent()->SetFocus();
			return;
		}

		case VK_RETURN:
		{
//			// Goto next row if possible
//			if (m_iItem < m_plvw->GetItemCount() - 1)
//				m_plvw->SelectCellOrder(m_iItem + 1, nOrder);

//			// Goto next col if possible
			int nCols = m_plvw->GetColumnCount();
			int nRows = m_plvw->GetItemCount();
			if (nOrder < nCols - 1)
			{
				m_plvw->SelectCellOrder(m_iItem, nOrder + 1);
			}
			else if (m_iItem < nRows - 1)
				m_plvw->SelectCellOrder(m_iItem + 1, 0);

			GetParent()->SetFocus();
			return;
		}

		case VK_TAB:
		{
			int nCols = m_plvw->GetColumnCount();
			int nRows = m_plvw->GetItemCount();

			if (!bShift)
			{
				if (nOrder < nCols - 1)
				{
					m_plvw->SelectCellOrder(m_iItem, nOrder + 1);
				}
				else if (m_iItem < nRows - 1)
					m_plvw->SelectCellOrder(m_iItem + 1, 0);
			}
			else
			{
				if (nOrder > 0)
					m_plvw->SelectCellOrder(m_iItem, nOrder - 1);
				else if (m_iItem > 0)
					m_plvw->SelectCellOrder(m_iItem - 1, nCols - 1);
			}
			GetParent()->SetFocus();
			return;
		}
    }

	// Call base class
    CEdit::OnChar(nChar, nRepCnt, nFlags);

    // Resize edit control if needed

    // Get text extent
    CString strText;

    GetWindowText(strText);
    CWindowDC dc(this);
    CFont *pFont = GetParent()->GetFont();
    CFont *pFontDC = dc.SelectObject(pFont);
    CSize sz = dc.GetTextExtent(strText);
    dc.SelectObject(pFontDC);
    sz.cx += 5;			   	// add some extra buffer

    // Get client rect
    CRect r, rParent;
    GetClientRect(&r);
    GetParent()->GetClientRect(&rParent);

    // Transform rect to parent coordinates
    ClientToScreen(&r);
    GetParent()->ScreenToClient(&r);

    // Check whether control needs to be resized and whether there is space to grow
    if (sz.cx > r.Width())
    {
		if (sz.cx + r.left < rParent.right )
			r.right = r.left + sz.cx;
		else
			r.right = rParent.right;
		MoveWindow(&r);
    }
}






void CEditInPlace::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Up and down are in the OnKeyDown so that the user can hold down the arrow
	// keys to scroll through the entries.
	BOOL bControl = GetKeyState(VK_CONTROL) < 0;
	switch (nChar)
	{
		case VK_UP:
		{
			if (m_iItem > 0)
				m_plvw->SelectCell(m_iItem - 1, m_iSubItem);
			GetParent()->SetFocus();
			return;
		}

		case VK_DOWN:
		{
			int nItems = m_plvw->GetItemCount();
			if (m_iItem < nItems - 1)
				m_plvw->SelectCell(m_iItem + 1, m_iSubItem);
			GetParent()->SetFocus();
			return;
		}

		// Ctrl+Home - Goto the top of the current column
		case VK_HOME:
		{
			if (!bControl)
				break;
			m_plvw->SelectCell(0, m_iSubItem);
			GetParent()->SetFocus();
			return;
		}

		// Ctrl+End - Goto the bottom of the current column
		case VK_END:
		{
			if (!bControl)
				break;
			int nItems = m_plvw->GetItemCount();
			m_plvw->SelectCell(nItems - 1, m_iSubItem);
			GetParent()->SetFocus();
			return;
		}
	}

	// Call base class
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}



void CEditInPlace::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    switch (nChar)
    {
		case VK_NEXT: // Page Down
		{
			int nCount = m_plvw->GetItemCount();
			int nNewItem = m_iItem + m_plvw->GetCountPerPage();
			if (nCount > nNewItem)
				m_plvw->SelectCell(nNewItem, m_iSubItem);
			else
				m_plvw->SelectCell(nCount - 1, m_iSubItem);
			GetParent()->SetFocus();
			return;
		}

		case VK_PRIOR: // Page Up
		{
			int nNewItem = m_iItem - m_plvw->GetCountPerPage();
			if (nNewItem > 0)
				m_plvw->SelectCell(nNewItem, m_iSubItem);
			else
				m_plvw->SelectCell(0, m_iSubItem);
			GetParent()->SetFocus();
			return;
		}
    }
    
	// Call base class
    CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}





// Normally, Windows handles all arrow-key and TAB-key input to a CWnd control. By overriding 
// OnGetDlgCode, a CWnd control can choose a particular type of input to process itself. 
UINT CEditInPlace::OnGetDlgCode() 
{
    return CEdit::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;
}



