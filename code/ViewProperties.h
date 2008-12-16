
// CViewProperties
// Encapsulates a listview that displays properties for the selected object.
// Date: January 2000
//-----------------------------------------------------------------------------------------------------------------


#pragma once


#include "ViewEx.h"
#include "ToolBarCtrlEx.h"	// Added by ClassView
#include "ListCtrlEx.h"	// Added by ClassView
#include "Undo.h"

#include "BObject.h"


class CViewProperties : public CViewEx
{
	DECLARE_DYNCREATE(CViewProperties)

// Construction
protected:
	CViewProperties();           // protected constructor used by dynamic creation
	virtual ~CViewProperties();

// Operations
public:
	ULONG FindNext(CString& strFindText, BOOL bMatchCase, BOOL bWholeWord, ULONG lngPropertyID = 0);
	ULONG GetSelectedPropertyID();
	void SelectPropertyValue(ULONG lngPropertyID);
	BOOL DoPrint(BOOL bPreview);

// Attributes
private:
	CListCtrlEx m_lvw;
	CToolBarCtrlEx m_tbr;
	BObject* m_pobjCurrent;
//	CUndo m_objUndo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewProperties)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
//	void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewProperties)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnObjEditInPlace(); //ok
	afx_msg void OnObjEditInDialog(); //ok
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
//	afx_msg void OnCmdEditDelete();
	afx_msg void OnCmdEditCopyGrid();
	afx_msg void OnUpdateEdit(CCmdUI* pCmdUI);
	afx_msg void OnCmdEditPastePlain();
	afx_msg void OnCmdEditPasteFormatted();
	afx_msg void OnCmdEditCut();
	afx_msg void OnCmdEditCopy();
	afx_msg void OnCmdEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnCmdEditClear();
	afx_msg void OnCmdAddProperty();
	afx_msg void OnCmdDeleteProperty();
	//}}AFX_MSG
	afx_msg void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotifyRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupBDataCommand(UINT nCommandID);
	DECLARE_MESSAGE_MAP()
};




