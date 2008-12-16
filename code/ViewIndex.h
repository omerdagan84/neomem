
// CViewIndex
// This view displays an alphabetical or categorized index of all objects in file.
// Date: May 2000
//-----------------------------------------------------------------------------------------------------------------



#pragma once



#include "ListCtrlEx.h"
#include "ToolBarCtrlEx.h"
#include "ViewEx.h"
class BObject;
#include "xcomboboxex.h"


class CViewIndex : public CViewEx
{
	DECLARE_DYNCREATE(CViewIndex)

// Construction
protected:
	CViewIndex();
	virtual ~CViewIndex();

// Operations
public:
	void FilterItems();
	void ReloadItems();
	virtual BOOL PressKey(UINT nVK);

// Implementation
protected:
	void AddChildrenToList(BObject* pobjStart);
	void RecalculateLayout(int cx = 0, int cy = 0);

// Attributes
private:
	CListCtrlEx m_lvw;
	CToolBarCtrlEx m_tbr;
	CStatic m_lbl;
	BOOL m_bLoaded;
	XComboBoxEx m_cboClass; // filter by class
	ULONG m_lngClassID;

	// Popup menu
//	BObject* m_pobjPopup;
//	int m_nPopupItem;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewIndex)
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewIndex)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnObjEditInPlace(); //ok
	afx_msg void OnObjOpen(); //ok
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnUpdateObjMoveDown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjMoveUp(CCmdUI* pCmdUI);
	afx_msg void OnNavigateFollowLink();
	afx_msg void OnObjMoveTo();
	afx_msg void OnUpdateObjMoveTo(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnNotifyClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotifyItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotifyDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeClass();
	DECLARE_MESSAGE_MAP()
};



