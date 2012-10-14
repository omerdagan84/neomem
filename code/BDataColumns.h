
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
public:
	ULONG GetPropertyID() { return m_lngPropertyID; };
	int GetColWidth() { return m_nColWidth; };

private:
	ULONG m_lngPropertyID;
	BObject* m_pobjPropertyDef; // Note: This is not serialized, just the ID is!
	int m_nColWidth;
	int m_nColAlignment;

friend class BDataColumns;

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
	virtual CString GetBDataText(BDoc* pDoc=NULL, ULONG lngPropertyID=0, BOOL bMachineVersion=FALSE);
	virtual BOOL IsValid(BDoc* pDoc);
	virtual BOOL ReplaceReferences(BObject* pobjFind, BObject* pobjNew = 0);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

public:
	const int GetColumnCount();
	int GetColumnIndex(ULONG lngPropertyID);
	int GetColumnIndex(BObject* pobjPropertyDef);
	ColumnInfo& GetColumnInfo(int nCol);
//x	LPCTSTR GetColumnName(int nCol, BDoc* pDoc);
	CString GetColumnName(int nCol, BDoc* pDoc);
	LPINT GetColumnOrder();
	int GetColumnWidth(int nCol);
	BObject* GetPropertyDef(int nCol, BDoc* pDoc);
	ULONG GetPropertyID(int nCol);
	ColumnInfo& InsertColumn(ULONG lngPropertyID, BDoc* pDoc, int nWidth = 0, int nCol = -1);
	BOOL RemoveColumn(int nCol);
	void RemoveColumns();
	void SetColumnOrder(LPINT anOrder, int nColumns);
	BOOL SetColumnWidth(int nCol, int nWidth);

private:
	// These are all serialized
	int m_nColumns; // Number of columns in view
	CColumnInfoArray m_aci; // Array of ColumnInfo structures
	CUIntArray m_anOrder; // Array of column indices indicating column order
};


