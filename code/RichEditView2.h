
// CRichEditView2
// This is basically MFC's CRichEditView with OLE object handling disabled,
// and some additional functions added for NeoMem.
// Also uses CRichEditCtrlEx, which allows us to use RichEdit 2.0+, 
// and adds some other methods.
//-----------------------------------------------------------------------------------------------------------------



#pragma once

#include "richeditctrlex.h"

class CNeoDoc;
class _AFX_RICHEDIT_STATE;  // private to implementation


class CRichEditView2 : public CCtrlView
{
	DECLARE_DYNCREATE(CRichEditView2)

// Construction
public:
	CRichEditView2();
	~CRichEditView2();

// enums
public:
	enum WordWrapType {WrapNone = 0, WrapToWindow = 1, WrapToTargetDevice = 2};


// Operations
public:
	CRect GetMargins() const;
	CRect GetPageRect() const;
	CSize GetPaperSize() const;
	CRect GetPrintRect() const;
	int GetPrintWidth() const;

	void SetMargins(const CRect& rectMargin);
	void SetPaperSize(CSize sizePaper);

	// Formatting
	CHARFORMAT2& GetCharFormatSelection2(); //`
	PARAFORMAT& GetParaFormatSelection();
	void SetCharFormat2(CHARFORMAT2 cf2); //`
	BOOL SetParaFormat(PARAFORMAT& pf);
	CRichEditCntrItem* GetSelectedItem() const;
	CRichEditCntrItem* GetInPlaceActiveItem() const;

	// CEdit control access
//`	CRichEditCtrl& GetRichEditCtrl() const;
	CRichEditCtrlEx& GetRichEditCtrlEx() const; //`
	CRichEditDoc* GetDocument() const;

	CNeoDoc* GetDocument(); //`
	BOOL FindNext();

	// other attributes
	long GetTextLength() const;
	static BOOL AFX_CDECL IsRichEditFormat(CLIPFORMAT cf);
	BOOL CanPaste() const;

	void AdjustDialogPosition(CDialog* pDlg);
	HRESULT InsertItem(CRichEditCntrItem* pItem);
	void InsertFileAsObject(LPCTSTR lpszFileName);
	BOOL FindText(LPCTSTR lpszFind, BOOL bCase = TRUE, BOOL bWord = TRUE);
	BOOL FindTextSimple(LPCTSTR lpszFind, BOOL bCase = TRUE, BOOL bWord = TRUE);
	long PrintInsideRect(CDC* pDC, RECT& rectLayout, long nIndexStart, long nIndexStop, BOOL bOutput);
	long PrintPage(CDC* pDC, long nIndexStart, long nIndexStop);
	void DoPaste(COleDataObject& dataobj, CLIPFORMAT cf, HMETAFILEPICT hMetaPict);

	void SetFindText(CString& str, BOOL bMatchCase, BOOL bWholeWord); //` added this

	void TextNotFound(LPCTSTR lpszFind);
	BOOL FindText(_AFX_RICHEDIT_STATE* pEditState);
	BOOL FindTextSimple(_AFX_RICHEDIT_STATE* pEditState);
	long FindAndSelect(DWORD dwFlags, FINDTEXTEX& ft);
	void Stream(CArchive& ar, BOOL bSelection);
	HRESULT GetWindowContext(LPOLEINPLACEFRAME* lplpFrame, LPOLEINPLACEUIWINDOW* lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo);
	HRESULT ShowContainerUI(BOOL b);
	static DWORD CALLBACK EditStreamCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);


// Attributes
public:
	int m_nWordWrap;
	int m_nBulletIndent;

//	float m_fScale; // delete
//	CWnd* m_pwndOldParent; //test
//	CWnd m_wnd; // test

	//` added this to make up for missing MFC global variable
	_AFX_RICHEDIT_STATE* _afxRichEditState;

	LPRICHEDITOLE m_lpRichEditOle;
	CDC m_dcTarget;
	long m_lInitialSearchPos;
	UINT m_nPasteType;
	BOOL m_bFirstSearch;

	static AFX_DATA ULONG lMaxSize; // maximum number of characters supported

public:
	long m_nPrintStartChar;
	long m_nPrintStopChar; 
	int m_nHeaderHeightTwips; 
	int m_nFooterHeightTwips; 
	

private:
	CRect m_rectMargin; // twips
	CSize m_sizePaper; // twips
	CRect m_rectPrintableArea; // twips
	CDWordArray m_aPageStart;    // array of starting pages
	PARAFORMAT m_paraformat;
	CHARFORMAT2 m_charformat2; //`
	BOOL m_bSyncCharFormat;
	BOOL m_bSyncParaFormat;

// Overrideables
public:
//	void OnCharEffect(DWORD dwMask, DWORD dwEffect);
	void OnUpdateCharEffect(CCmdUI* pCmdUI, DWORD dwMask, DWORD dwEffect) ;
	void OnParaAlign(WORD wAlign);
	void OnUpdateParaAlign(CCmdUI* pCmdUI, WORD wAlign);
	void ShowPopup(CPoint point, WORD seltyp);
protected:
	virtual BOOL IsSelected(const CObject* pDocItem) const;
	virtual void OnInitialUpdate();
	virtual void OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord);
	virtual void OnReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord, LPCTSTR lpszReplace);
	virtual void OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace, BOOL bCase, BOOL bWord);
	virtual void OnTextNotFound(LPCTSTR lpszFind);
	virtual void OnPrinterChanged(const CDC& dcPrinter);
	virtual void WrapChanged();

	//{{AFX_VIRTUAL(CRichEditView2)
	public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Advanced
	virtual BOOL OnPasteNativeObject(LPSTORAGE lpStg);
	virtual HMENU GetContextMenu(WORD, LPOLEOBJECT, CHARRANGE* );
	virtual HRESULT GetClipboardData(CHARRANGE* lpchrg, DWORD dwReco, LPDATAOBJECT lpRichDataObj, LPDATAOBJECT* lplpdataobj);
	virtual HRESULT QueryAcceptData(LPDATAOBJECT, CLIPFORMAT*, DWORD, BOOL, HGLOBAL);
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
//	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// construction
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Printing support
	BOOL PaginateTo(CDC* pDC, CPrintInfo* pInfo);
//	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
//	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
//	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo = NULL);
//	void DrawHeaderFooter(CDC* pDC);
//	void DrawMargins(CDC* pDC, CPrintInfo* pInfo);
//	void PrintPreview();

	// Find & replace support
	void OnEditFindReplace(BOOL bFindOnly);
	BOOL SameAsSelected(LPCTSTR lpszCompare, BOOL bCase, BOOL bWord);


	//{{AFX_MSG(CRichEditView2)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnFormatFont();
	afx_msg void OnEditOpenHyperlink();
	afx_msg void OnEditReplace();
	afx_msg void OnEditFind();
	//}}AFX_MSG
	afx_msg LRESULT OnFindReplaceCmd(WPARAM, LPARAM lParam);
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFilePrintPreview();

//	afx_msg void OnEditCut();
//	afx_msg void OnEditCopy();
//	afx_msg void OnEditPasteFormatted();
//	afx_msg void OnEditClear();
//	afx_msg void OnEditUndo();
//	afx_msg void OnEditSelectAll();
//	afx_msg void OnEditRepeat();

// ole
//	afx_msg void OnEditProperties();
//	afx_msg void OnInsertObject();
//	afx_msg void OnEditPasteSpecial();
//	afx_msg void OnEditInsertObject();

	// special overrides for implementation

//	afx_msg void OnUpdateNeedSel(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateNeedClip(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateNeedText(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateNeedFind(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()


// Interface Map
public:
	BEGIN_INTERFACE_PART(RichEditOleCallback, IRichEditOleCallback)
		INIT_INTERFACE_PART(CRichEditView, RichEditOleCallback)
		STDMETHOD(GetNewStorage) (LPSTORAGE*);
		STDMETHOD(GetInPlaceContext) (LPOLEINPLACEFRAME*,
									  LPOLEINPLACEUIWINDOW*,
									  LPOLEINPLACEFRAMEINFO);
		STDMETHOD(ShowContainerUI) (BOOL);
		STDMETHOD(QueryInsertObject) (LPCLSID, LPSTORAGE, LONG);
		STDMETHOD(DeleteObject) (LPOLEOBJECT);
		STDMETHOD(QueryAcceptData) (LPDATAOBJECT, CLIPFORMAT*, DWORD,BOOL, HGLOBAL);
		STDMETHOD(ContextSensitiveHelp) (BOOL);
		STDMETHOD(GetClipboardData) (CHARRANGE*, DWORD, LPDATAOBJECT*);
		STDMETHOD(GetDragDropEffect) (BOOL, DWORD, LPDWORD);
		STDMETHOD(GetContextMenu) (WORD, LPOLEOBJECT, CHARRANGE*, HMENU*);
	END_INTERFACE_PART(RichEditOleCallback)

	DECLARE_INTERFACE_MAP()
};





#ifndef _DEBUG  // debug version is in RichEditView2.cpp
//. ole
/*
inline CRichEditDoc* CRichEditView2::GetDocument() const
{ 
	ASSERT(0);
	return 0;
//	return &theApp.m_redoc; 
}
*/
inline CNeoDoc* CRichEditView2::GetDocument()
   { return (CNeoDoc*)m_pDocument; }
#endif




// from afxrich.inl:

// Inlines for AFXRICH.H
//`#ifdef _AFXRICH_INLINE
#ifndef _AFXRICH_INLINE
#define _AFXRICH_INLINE inline
#endif


// CRichEditView
// This is MFC code, but how can you cast from CRichEditView to CRichEditCtrl?
// they both derive from CWnd
// CWnd has about 10 member variables - HWND is the first. 99% functions though.
// CRichEditCtrl has NO member variables.
// CRichEditView adds 10 vars
// CCtrlView adds 2 vars
// CView adds 2 vars
// so CRichEditView has like 14 vars, CRichEditCtrl has NONE. 
// is that why this is safe? like casting to a base class
//`_AFXRICH_INLINE CRichEditCtrl& CRichEditView::GetRichEditCtrl() const
//`	{ return *(CRichEditCtrl*)this; }
_AFXRICH_INLINE CRichEditCtrlEx& CRichEditView2::GetRichEditCtrlEx() const
{ return *(CRichEditCtrlEx*)this; } 



/*int _AFXRICH_INLINE CRichEditView2::GetPrintWidth() const
	{ return m_sizePaper.cx - m_rectMargin.left - m_rectMargin.right;}

CRect _AFXRICH_INLINE CRichEditView2::GetPrintRect() const
//	{ return CRect(m_rectMargin.left, m_rectMargin.top, m_sizePaper.cx - m_rectMargin.right, m_sizePaper.cy - m_rectMargin.bottom); }
	{ 
		return CRect(m_rectMargin.left, m_rectMargin.top + m_nHeaderHeightTwips, 
							m_sizePaper.cx - m_rectMargin.right, m_sizePaper.cy - m_rectMargin.bottom - m_nFooterHeightTwips); 
	}
CRect _AFXRICH_INLINE CRichEditView2::GetPageRect() const
	{ return CRect(CPoint(0,0), m_sizePaper); }

*/


void _AFXRICH_INLINE CRichEditView2::SetPaperSize(CSize sizePaper)
	{ m_sizePaper = sizePaper; }

CSize _AFXRICH_INLINE CRichEditView2::GetPaperSize() const
	{ return m_sizePaper; }

void _AFXRICH_INLINE CRichEditView2::SetMargins(const CRect& rectMargin)
	{ m_rectMargin = rectMargin; }

CRect _AFXRICH_INLINE CRichEditView2::GetMargins() const
	{ return m_rectMargin; }

_AFXRICH_INLINE long CRichEditView2::GetTextLength() const
	{ return GetRichEditCtrlEx().GetTextLength(); }

/*
_AFXRICH_INLINE CRichEditDoc* CRichEditView2::GetDocument() const
{
	ASSERT(m_pDocument != NULL);
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRichEditDoc)));
	return (CRichEditDoc*)m_pDocument;
}
*/
//`#endif //_AFXRICH_INLINE

/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
// _AFX_EDIT_STATE

class _AFX_EDIT_STATE : public CNoTrackObject
{
public:
//	_AFX_EDIT_STATE();
//	virtual ~_AFX_EDIT_STATE();

	CFindReplaceDialog* pFindReplaceDlg; // find or replace dialog
	BOOL bFindOnly; // Is pFindReplace the find or replace?
	CString strFind;    // last find string
	CString strReplace; // last replace string
	BOOL bCase; // TRUE==case sensitive, FALSE==not
	int bNext;  // TRUE==search down, FALSE== search up
	BOOL bWord; // TRUE==match whole word, FALSE==not
};

//#undef AFX_DATA
//#define AFX_DATA AFX_CORE_DATA

class _AFX_RICHEDIT_STATE : public _AFX_EDIT_STATE
{
public:
	HINSTANCE m_hInstRichEdit;      // handle to RICHED32.DLL
//	virtual ~_AFX_RICHEDIT_STATE();
};




//AFX_STATIC const UINT _afxMsgFindReplace2 = ::RegisterWindowMessage(FINDMSGSTRING);


