
// XComboBoxEx
// Extension of MFC ComboBoxEx class
//-----------------------------------------------------------------------------------------------------------------


#pragma once


class BObject;
class BDataLink;


class XComboBoxEx : public CComboBoxEx
{

// Construction
public:
	XComboBoxEx();
	virtual ~XComboBoxEx();

// Operations
public:
	int AddNone(int nImage);
	int AddObject(BObject* pobj, BObject* pobjParent = 0);
	void AddObjects(BObject* pobjStart, ULONG lngExcludeFlags, BOOL bIncludeStart = FALSE, 
								BOOL bRecurse = FALSE, int nIndent = 0, BDataLink* pdatValidClasses = 0);
	void DeleteAllItems();
	BOOL DeleteItemData(LPARAM lParam);
	int FindItemData(DWORD dwData);
	int GetItemIndent(int nIndex);
	DWORD GetSelectedItemData();
	int SelectItemData(DWORD dwData, BOOL bSelectFirstIfNotFound = TRUE);
	void SelectNone();
	BOOL SelectAnything();
	void SetItemIcon(int nItem, int nImage);


// Attributes
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XComboBoxEx)
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(XComboBoxEx)
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



