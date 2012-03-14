// CColourPopup
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage whatsoever.
// It's free - so you get what you pay for.

// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// Updated 30 May 1998 to allow any number of colours, and to
//                     make the appearance closer to Office 97. 
//                     Also added "Default" text area.         (CJM)
//
//         13 June 1998 Fixed change of focus bug (CJM)
//         30 June 1998 Fixed bug caused by focus bug fix (D'oh!!)
//                      Solution suggested by Paul Wilkerson.
//
// ColourPopup is a helper class for the colour picker control
// CColourPicker. Check out the header file or the accompanying 
// HTML doc file for details.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 



//` just ignore the keyboard mode thing



#pragma once 

// CColourPopup messages
#define CPN_SELCHANGE        WM_USER + 1001        // Colour Picker Selection change
#define CPN_DROPDOWN         WM_USER + 1002        // Colour Picker drop down
#define CPN_CLOSEUP          WM_USER + 1003        // Colour Picker close up
#define CPN_SELENDOK         WM_USER + 1004        // Colour Picker end OK
#define CPN_SELENDCANCEL     WM_USER + 1005        // Colour Picker end (cancelled)

// forward declaration
class CColourPicker;

// To hold the colours and their names
typedef struct 
{
	COLORREF crColour;
	TCHAR *szName;
} ColourTableEntry;



/////////////////////////////////////////////////////////////////////////////
// CColourPopup window

class CColourPopup : public CWnd
{
// Construction
public:
    CColourPopup();
    CColourPopup(CPoint p, COLORREF crColour, CWnd* pParentWnd,
                 LPCTSTR szDefaultText = NULL, LPCTSTR szCustomText = NULL,
				 BOOL bKeyboardMode = FALSE);
    virtual ~CColourPopup();

// Operations
public:
    void Initialise();
    BOOL Create(CPoint p, COLORREF crColour, CWnd* pParentWnd, 
                LPCTSTR szDefaultText = NULL, LPCTSTR szCustomText = NULL);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColourPopup)
    public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

// Implementation
public:
    BOOL GetCellRect(int nIndex, const LPRECT& rect);
    void FindCellFromColour(COLORREF crColour);
    void SetWindowSize();
    void CreateToolTips();
    void ChangeSelection(int nIndex);
    void EndSelection(int nMessage);
    void DrawCell(CDC* pDC, int nIndex);

    COLORREF GetColour(int nIndex)              { return m_crColours[nIndex].crColour; }
    LPCTSTR GetColourName(int nIndex)           { return m_crColours[nIndex].szName; }
    int  GetIndex(int row, int col) const;
    int  GetRow(int nIndex) const;
    int  GetColumn(int nIndex) const;

// Attributes
protected:
    static ColourTableEntry m_crColours[];
    int            m_nNumColours;
    int            m_nNumColumns, m_nNumRows;
    int            m_nBoxSize, m_nMargin;
    int            m_nCurrentSel;
    int            m_nChosenColourSel;
    CString        m_strDefaultText;
    CString        m_strCustomText;
    CRect          m_CustomTextRect, m_DefaultTextRect, m_WindowRect;
    CFont          m_Font;
    CPalette       m_Palette;
    COLORREF       m_crInitialColour, m_crColour;
    CToolTipCtrl   m_ToolTip;
    CWnd*          m_pParent;

    BOOL           m_bChildWindowVisible;
	
	BOOL m_bKeyboardMode; //` added this

    // Generated message map functions
protected:
    //{{AFX_MSG(CColourPopup)
    afx_msg void OnNcDestroy();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};




