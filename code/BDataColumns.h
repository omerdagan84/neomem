
// BDataColumns
// This class stores information about the columns displayed in the Contents View,
// (Properties, widths, alignments, etc.)
//-----------------------------------------------------------------------------------------------------------------


#pragma once

#include "BData.h"


class BObject;


class ColumnInfo
{
public:
	ColumnInfo();
//	LPCTSTR GetName(CNeoDoc* pDoc);
public:
	ULONG m_lngPropertyID;
	BObject* m_pobjPropertyDef; // Note: This is not serialized, just the ID is!
	int m_nColWidth;
	int m_nColAlignment;
//	int m_nColOrder;
//	int m_nColFormat;
//	int m_nColGroup;
//	int m_nColSort;
//	CString m_strColHeader;
//	CString m_strColFilter;
};


typedef CArray<ColumnInfo, ColumnInfo&> CColumnInfoArray;

class BDataColumns : public BData  
{
	DECLARE_SERIAL(BDataColumns)

public:
	BDataColumns();
	BDataColumns(BDataColumns* pdat); // copy constructor
	virtual ~BDataColumns();

public:
	virtual BData* CreateCopy();
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual LPCTSTR GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	virtual BOOL IsValid(CNeoDoc* pDoc);
	virtual BOOL ReplaceReferences(BObject* pobjFind, BObject* pobjNew = 0);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

public:
	void RemoveColumns();
	int GetColumnCount();
	int GetColumnIndex(ULONG lngPropertyID);
	int GetColumnIndex(BObject* pobjPropertyDef);
	ColumnInfo& GetColumnInfo(int nCol);
	LPCTSTR GetColumnName(int nCol, CNeoDoc* pDoc);
	LPINT GetColumnOrder();
	BObject* GetPropertyDef(int nCol, CNeoDoc* pDoc);
	ULONG GetPropertyID(int nCol);
	ColumnInfo& InsertColumn(ULONG lngPropertyID, CNeoDoc* pDoc, int nWidth = 0, int nCol = -1);
	BOOL RemoveColumn(int nCol);
	void SetColumnOrder(LPINT anOrder, int nColumns);
	BOOL SetColumnWidth(int nCol, int nWidth);

private:
	// These are all serialized
	int m_nColumns; // Number of columns in view
	CColumnInfoArray m_aci; // Array of ColumnInfo structures
	CUIntArray m_anOrder; // Array of column indices indicating column order
public:
	// These are not serialized
	BOOL m_bModified;
};


