
// CViewSearch
// The Search View allows the user to search across all objects in the file.
//-----------------------------------------------------------------------------------------------------------------

#pragma once

#include "ViewEx.h"


#include "EditEx.h"
#include "ListCtrlEx.h"
#include "StaticEx.h"
#include "TabOrder.h"
#include "ToolBarCtrlEx.h"
#include "XComboBoxEx.h"



class CViewSearch : public CViewEx
{
	DECLARE_DYNCREATE(CViewSearch)

// Construction
protected:
	CViewSearch();           // protected constructor used by dynamic creation
	virtual ~CViewSearch();

// Operations
public:
	virtual BOOL PressKey(UINT nVK);
	void SelectNextObject();
	void RecalculateLayout(int cx = 0, int cy = 0);
//	void SelectFirstTab();


// Attributes
private:
	CToolBarCtrlEx m_tbr;
	CListCtrlEx m_lvw; // Search results

	// Controls
	CStaticEx m_lblSearchIn;
	XComboBoxEx m_cboSearchIn; // Specify properties to search in
//	CStaticEx m_lblSearchFor;
	CEditEx m_txtSearchFor; // Search text

//	CButton m_chkMatchCase;
//	CButton m_chkWholeWord;
//	CStatic m_lblResults; // Number of items found

//	CString m_strLabel;
	int m_nSearchInLabelWidth; // width of m_lblSearchIn
//	int m_nSearchForLabelWidth; // width of m_lblSearchFor
	int m_nButtonWidth; // width of buttons in toolbar
	int m_nButtonHeight; // height of buttons in toolbar

	CString m_strFindText; // text to find
	ULONG m_lngPropertyID; // property to search in or 0 for all properties
	BOOL m_bMatchCase;
	BOOL m_bWholeWord;

	CTabOrder m_to; // tab order for controls

	// Popup menu
//	BObject* m_pobjPopup;
//	int m_nPopupItem;
//	enum eProperties {propNameDescRtf = -2, propAllProperties = -3};



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSearch)
	public:
	protected:
	virtual void OnDraw(CDC* pDC);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CViewSearch)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyReturn();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCmdEditFindNext();
	afx_msg void OnKeyTab();
	afx_msg void OnKeyShiftTab();
	afx_msg void OnObjOpen(); //ok
	afx_msg void OnObjEditInPlace(); //ok
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnUpdateObjMoveDown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjMoveUp(CCmdUI* pCmdUI);
	afx_msg void OnNavigateFollowLink();
	afx_msg void OnObjMoveTo();
	afx_msg void OnUpdateObjMoveTo(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnSelectSearch();
	afx_msg void OnBtnGo();
	afx_msg void OnNotifyClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotifyDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotifyItemChanged(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};


