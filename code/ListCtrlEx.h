
// CListCtrlEx
// This class extends the CListCtrl class to handle BObjects.
// Date: February 2000
//-----------------------------------------------------------------------------------------------------------------

// currently knows about bobjects and columns etc. make it pure ui. 

// IMPORTANT: Need to set m_pDoc because various routines need it!

// To have a dummy new row, you must set m_bDummyRow = TRUE and assign
// the index of the new row icon to m_nNewIconIndex.

// To show disabled items, say
//	m_lvw.m_bDisabledItems = TRUE; // Special disabled items mode (draws some items in 
//  gray and won't let user change their checkbox)


//, listviews can have state columns - eg could indicate if an item had children
//, or could indicate that with an overlay


//-----------------------------------------------------------------------------------------------------------------



#pragma once


class BDataColumns;
class BDataLink;
class BObjects;
class CNeoDoc;
#include "BData.h"
#include "BObject.h"


// Custom Messages
// Control notifications
// This includes WM_COMMAND notification messages from controls and other child 
// windows to their parent windows. For example, an edit control sends its parent a 
// WM_COMMAND message containing the EN_CHANGE control-notification code when 
// the user has taken an action that may have altered text in the edit control. 
// The window’s handler for the message responds to the notification message in some 
// appropriate way, such as retrieving the text in the control.
#define CM_COLUMNS_CHANGED		WM_APP + 0
#define CM_ITEM_SELECTED		WM_APP + 1




class CListCtrlEx : public CListCtrl {

public:

	// Construction
	CListCtrlEx();
	virtual ~CListCtrlEx();

	// Enums
	enum eRowConstants {keyDummyRow = 0}; 

	// these must be shifted 8 bits to the left to fit in the LVIS_OVERLAYMASK bits
	enum eItemStates {stateNone = 0, stateDisabled = 1 << 8}; // used with GetItemState(LVIS_STATEIMAGEMASK)

	// Mode this listview is in (combination of several flags):
	// SelectCell allows individual cells to be selected and edited.
	// DisabledItems will disable a row if its item data is 0.
	// MultiSelect displays checkboxes to each item.
	// DummyRow includes a dummy new row at end of listview.
	// ExpandLastColumn will expand the last column on resize to fill the listview.
	// ExpandAllColumns will keep columns proportionally sized on resize.
	// DisableFirstColumn prevents user from editing first column (eg in properties view).
	enum eMode {
		modeNormalListView = 1, 
		modeSelectCell = 2, // mode used by contents view - allows for multiselect
		modeFullRowSelect = 4, // 
//		modeDisabledItems = 8, // mode used by properties list in class wizard
		modeCheckboxes = 32, // mode used by edit links dialog - check off multiple items
	};

	// Focus state currently in (fsNone, fsFocus, fsHighlight, fsInactive)
	enum eFocusStates {fsNone = 0, fsFocus = 1, fsHighlight = 2, fsInactive = 3};


	// Operations
	void AddDummyRow();
	void AddObjects(BObject* pobjStart, ULONG lngExcludeFlags, BOOL bIncludeStart = FALSE, 
								BOOL bRecurse = FALSE, int nIndent = 0, BOOL bClearFirst = FALSE, 
								BOOL bHierarchy = FALSE);
	int AddObject(BObject *pobj, BObject *pobjParent = NULL);
	void ClearSelections();
	void ClearSortMemory();
	BOOL CopyCurrentCellToClipboard();
	BOOL CopyToClipboard(BOOL bIncludeHeaders);
	BOOL CutCurrentCellToClipboard();
	void DeleteColumnProperty(BObject* pobjPropertyDef);
	BOOL DeleteItemData(LPARAM lParam);
	BOOL DeleteSelectedItem();
	CEdit* EditCurrentCell(LPCTSTR pszEditText = 0);
	CEdit* EditSubItem(int nItem, int nCol, LPCTSTR pszEditText = 0);
	BOOL EnsureCellVisible(int nRow, int nCol);
	int FindColumn(ULONG lngPropertyID);
	int FindItemData(LPARAM lParam);
	void FollowLink(CView* pViewToRestore = NULL);
	BData* GetCellBData(int nRow = -1, int nCol = -1);
	BOOL GetCellRect(int nRow, int nColIndex, CRect &r, int nArea = LVIR_BOUNDS);
	int GetColumnCount();
	virtual ULONG GetColumnPropertyID(int iCol = -1);
	CString GetColumnText(int nColOrder);
	int GetColumnWidths(int nCols = 0);
	int GetItemIndent(int nIndex);
	CString GetItemTextOP(int nItem, int nSubItem);
	int GetSelectedItemsArray(CObArray& aObjects);
	int GetSelectedItem();
	int GetSelectedItems(BDataLink* pdatLink);
	LPARAM GetSelectedItemData();
	ULONG GetSelectedObjectID();
	int HitTestEx(CPoint &pt, int* pnCol) const;
	int IndexToOrder(int nColIndex);
	int InsertColumnAsk(OBJID idProperty = 0, int nCol = -1, BObject* pobjParent = NULL);
	int InitializeColumns(BDataColumns* pdatColumns, BOOL bExpandLastColumn = FALSE, BOOL bSaveChangesAutomatically = FALSE);
	void InvalidateCurrentCell();
	int MoveItemToSibling(int nSource, int nTarget, BOOL bAfter);
	void NotifyParentOfChanges();
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	int OrderToIndex(int nColPosition);
	BOOL PasteClipboardToCurrentCell();
	BOOL PressKey(UINT nVK);
	void PrintPage(CDC *pDC, CPrintInfo *pInfo);
	void RecalculateLayout();
	BOOL RemoveColumn(int nCol = -1);
	void RemoveDummyRow();
	BOOL SaveColumnInfo(BObject* pobj);
	BOOL SaveColumnOrder(BDataColumns* pdatColumns);
	void SetSelectedItemsState(UINT nStateMask, UINT nState);
	BOOL SelectAnything();
	void SelectCell(int nRow, int nCol, BOOL bEnsureVisible = TRUE);
	void SelectCellOrder(int nRow, int nColOrder, BOOL bEnsureVisible = TRUE);
	void SelectItem(int nItem);
	int SelectItemData(LPARAM lParam);
	void SelectNone();
	void SetCheckboxes(BOOL bCheckboxes);
	void SetFullRowSelect(BOOL bOn);
	BOOL SetSelectedItems(BDataLink* pdatLink);
	void SortByColumn(int nCol, int iDir = 0);
	void SortByProperty(ULONG lngPropertyID = 0, int iDir = 0);
	BOOL UpdateColumn(int nCol, BObject* pobjPropertyDef);
	void UpdateEdit(CCmdUI* pCmdUI);



	// Attributes
public:
	CNeoDoc* m_pDoc;
	
	// Colors obtained in GetColors()
	COLORREF m_clrDisabledItems;

	ULONG m_nMode; // an eMode

	BOOL m_bDummyRow;
	BOOL m_bExpandLastColumn;
	BOOL m_bExpandAllColumns;
	BOOL m_bDisableFirstColumn;
	BOOL m_bDisabledItems; // some items disabled - check stateimage
	BOOL m_bAutoColumns; // if true the control will handle inserting removing columns, popup etc

	eFocusStates m_nFocusState; 

	// Columns
	// Note: The listview never owns a bdata object - it always points to someone else's!
	BDataColumns* m_pdatColumns; // Pointer to column arrangement info
	BOOL m_bColumnsChanged; // Flag indicating if user changed column order or widths, etc.
	BOOL m_bSaveChangesAutomatically; // If True, listview will save any changes directly to m_pdatColumns as they are made.

	// These are public so views can access the current cell
	int m_nRow; // Currently selected row
	int m_nCol; // Currently selected column
	int m_nNewIconIndex; // Index of the New row icon (triangle)
	ULONG m_lngSortPropertyID; // Property being sorted on
	int m_iSortDirection; // Direction of sort (1=ascending, -1=descending)

private:
	
	BOOL m_bMouseDown; // Used in drag drop
	CPoint m_ptDragStart; // Start point for drag drop - user must move a certain distance from this to register as drag start.
	int m_nTargetColumn; // Target column to use for commands. 

	// Printing
	CUIntArray m_anColPos;
	CUIntArray m_anColWidth;
	CUIntArray m_anRowLines;
	UINT m_nPrintLinesTotal;
	UINT m_nPrintLinesPerPage;


	// Implementation
private: 
	void EnableAllProperties(); // clear flagDisabled for all properties
	void DisableVisibleProperties(); // set flagDisabled for properties that are already visible in this view


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult); // don't delete this!
	//}}AFX_VIRTUAL


	// Message map functions
	protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotifyDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotifyDeleteAllItems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNotifyItemChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnColumnSortAscending();
	afx_msg void OnColumnSortDescending();
	afx_msg void OnColumnEditProperty();
	afx_msg void OnColumnDelete();
	afx_msg void OnColumnInsert();
	afx_msg void OnColumnSortClear();
	afx_msg void OnUpdateColumnSortClear(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg BOOL OnHeaderItemChanged(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHeaderEndDrag(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnLvwColumnsChanged(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};






