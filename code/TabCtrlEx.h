
// CTabCtrlEx
// Extension of CTabCtrl class, handles tab drag and drop.
// Date: February 2000
//-----------------------------------------------------------------------------------------------------------------


#pragma once


class CTabCtrlEx : public CTabCtrl
{
// Construction
public:
	CTabCtrlEx();
	virtual ~CTabCtrlEx();

// enums
public:
	enum DragPhase {dragNone, dragInitial, dragMove};

// Operations
public:
	void DrawRect(CRect& rErase, CRect& rDraw);


// Attributes
private:
	DragPhase m_nDragPhase; // phase of drag operation
	CPoint m_ptInitial; // initial point
	int m_nTabDrag; // which tab is being dragged (zero based)
	CRect m_rectTest; // rectangle used to test for drag start
	CRect m_rectDraw; // used in drawing dragged tab
	int m_nTargetTab;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
protected:
	//{{AFX_MSG(CTabCtrlEx)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};




