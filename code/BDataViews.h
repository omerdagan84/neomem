
// BDataViews
// This class stores information describing what tabs and views are 
// available for a class or an object. Eg classPaper just has viewText 
// available, classFolder has viewText and viewContents.
//-----------------------------------------------------------------------------------------------------------------

// Note: Each view is specified in a ViewInfo object, and an array of 
// these objects specifies the entire tab/view arrangement. Each tab is 
// separated by a ViewInfo object with 0 for all values.
// The last ViewInfo object in the array should be all zeros also.


#pragma once


#include "BData.h"

class BObject;
class CNeoDoc;


// ViewInfo
// This is a helper class that stores information used in the array.
class ViewInfo
{
public:
	ViewInfo();
public:
	ULONG m_lngViewID;
	ULONG m_lngViewHeight;
};


typedef CArray<ViewInfo, ViewInfo&> CViewInfoArray;


class BDataViews : public BData  
{
	DECLARE_SERIAL(BDataViews)

public:
	BDataViews();
	BDataViews(BDataViews* pdat); // copy constructor
	virtual ~BDataViews();

public:
	virtual BData* CreateCopy();
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual LPCTSTR GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	virtual BOOL ReplaceReferences(BObject* pobjFind, BObject* pobjNew = 0);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);
//	virtual BOOL UIEditValue(BObject* pobj, BObject* pobjPropertyDef);

public:
	int GetViewHeight(int nTab, int nView);
	BOOL SetViewHeight(int nTab, int nView, int nHeight);
	BOOL MoveViewUp(ULONG lngViewID);
	int GetMaxIndex();
	BOOL RemoveView(ULONG lngViewID);
	int GetTabCount();
	int GetTabIndex(int nTab);
	BOOL GetTabName(int nTab, CString& strName, CNeoDoc* pDoc);
	int GetViewCount(int nTab);
	ViewInfo& GetViewInfo(int nIndex);
	int GetViewTab(ULONG lngViewID);
	int InsertView(int nTab, int nView, ULONG lngViewID, ULONG lngViewHeight);

//private:
	CViewInfoArray m_avi; // Array of ViewInfo structures

};



