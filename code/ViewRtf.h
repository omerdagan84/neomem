
// CViewRtf
// This is the Text View, and contains a toolbar and a rich edit view 
// derived from CRichEditView.
// Date: January 2000
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "ViewEx.h"
#include "ToolBarRtf.h"

class CRichEditView2;
class CRichEditCtrlEx;
class CFontEx;


class CViewRtf : public CViewEx
{
	DECLARE_DYNCREATE(CViewRtf)

// Construction
protected:
	CViewRtf();           // protected constructor used by dynamic creation
	virtual ~CViewRtf();


// Operations
public:
	void SetZoom();
	void GotoStart();
	void SaveOleObjects();
//	int GetHyperlinkFromPos(CPoint* pt);
	virtual BOOL DoPrint(BOOL bPreview);
	void AdjustWindow();
	void SetDefaultColor();
//	void PrintPreview();
	void ChangeFont(CString& strFontName);
	void ChangeSize(int nSize);
	long FindNext(CString& strFindText, BOOL bMatchCase, BOOL bWholeWord); // used by viewsearch,
		// or could get through m_pviewrtf!
	void SetDefaultFont(CFontEx& font);
protected:
//	void ColorHyperlinks();
	void SetFormat(CHARRANGE& cr, CHARFORMAT& cf);
//	void GetNewBackcolor();
//	void GetNewForecolor();


// Attributes
public:
//	CMiniFrameWnd m_wndMini; // delete
//	CWnd m_wnd; //delete

	// Pointer to CRichEditView2 object, which contains the actual Richedit control.
	// Public so framechild can access it to get plain text for export.
	CRichEditView2* m_pViewRtf; 

	// Pointer to the actual Richedit control
	CRichEditCtrlEx* m_prtf; 
protected:
	CToolBarRtf m_tbrRtf;
	CString m_strFindText;
	BOOL m_bMatchCase;
	BOOL m_bWholeWord;
//	COLORREF m_clrForecolor;
//	COLORREF m_clrBackcolor;

	CHARRANGE m_chrgMouseDown; // Remembers charrange between MouseDown and MouseUp events
	BOOL m_bZoom;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewRtf)
	public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnDraw(CDC* pDC);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult); // (goes inside afx block above)
	BOOL OnPreparePrinting(CPrintInfo* pInfo);
	void OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/);
	void OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/);
//	void OnFilePrint();
//	void OnFilePrintPreview();

// Implementation


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewRtf)
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedSel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedClip(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedFind(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatBold(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatItalic(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatUnderline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatStrikethrough(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatCenter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatRight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatBullet(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatFont(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatSize(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCmdEditCut();
	afx_msg void OnCmdEditSpike();
	afx_msg void OnCmdEditCopy();
	afx_msg void OnCmdEditPasteFormatted();
	afx_msg void OnCmdEditUndo();
	afx_msg void OnCmdEditRedo();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnCmdEditInsertLine();
	afx_msg void OnCmdEditDeleteLine();
	afx_msg void OnCmdEditFindNext();
	afx_msg void OnUpdateEditFindNext(CCmdUI* pCmdUI);
	afx_msg void OnCmdEditFind();
	afx_msg void OnCmdEditReplace();
	afx_msg void OnCmdEditShiftLeft();
	afx_msg void OnCmdEditShiftRight();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCmdEditSelectAll();
	afx_msg void OnCmdEditPastePlain();
	afx_msg void OnCmdEditClear();
	afx_msg void OnTest();
	afx_msg void OnCmdEditInsertObject();
	afx_msg void OnFormatBackcolor();
	afx_msg void OnFormatForecolor();
	afx_msg void OnEditInsertLink();
	afx_msg void OnEditClearFormat();
	afx_msg void OnEditMoveTo();
	afx_msg void OnEditSortSelectedText();
	afx_msg void OnEditInsertSymbol();
	afx_msg void OnEditInsertDate();
	afx_msg void OnFormatFont();
	afx_msg void OnViewZoom();
	afx_msg void OnUpdateViewZoom(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnColorButtonClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelEndOK(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFormatApplyBackcolor();
	afx_msg void OnFormatApplyForecolor();
	afx_msg void OnRtfLink(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFormatBold();
	afx_msg void OnFormatItalic();
	afx_msg void OnFormatUnderline();
	afx_msg void OnFormatStrikethrough();
	afx_msg void OnFormatLeft();
	afx_msg void OnFormatCenter();
	afx_msg void OnFormatRight();
	afx_msg void OnFormatBullet();
//	afx_msg void OnToolbarDropDown(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnUpdateEdit(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

};



