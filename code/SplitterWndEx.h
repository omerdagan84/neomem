
// CSplitterWndEx
// This is an extension of the CSplitterWnd class - overrode it to make the borders of 
// the panes look better.
//-----------------------------------------------------------------------------------------------------------------



#pragma once


// WARNING: AfxImpl.h is subject to change in later
// versions of MFC and, as such, may break a project in the future.
// It includes the afxData structure with system colors etc.
#include <..\src\afximpl.h> 



class CSplitterWndEx : public CSplitterWnd  
{

// Construction
public:
	CSplitterWndEx();
	virtual ~CSplitterWndEx();


// enums
public:
	enum eSplitterModes {modeNormal, modeSplitterContents};

// Operations
public:
	void SetRowHeightPct(int nRow, int nHeightPct, BOOL bRecalcLayout);
	int GetRowHeightPct(int nRow);
	void SetRowHeight(int nRow, int nHeightPct, BOOL bRecalcLayout);
	int GetRowHeight(int nRow);
	BOOL SetPane(int row, int col, CView *pView);
	BOOL ClearPane(int row, int col);
	void OnDrawSplitter(CDC* pDC, ESplitType nType,	const CRect& rectArg);
//	BOOL CreateView(int row, int col, CRuntimeClass *pViewClass, SIZE sizeInit, CCreateContext *pContext);
	void SetMode(ULONG lngMode);

	void RecalcLayout();    // advanced overridable 
//	void StopTracking(BOOL bAccept); // advanced overridable

// Attributes
public:
	ULONG m_lngMode; // ie eSplitterModes


// Implementation
	//{{AFX_MSG(CSplitterWndEx)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



