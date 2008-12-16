
// CUndo
// This class stores an array of command objects which describe actions the user takes
// and allows the user to undo them in order.
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "BObject.h"
#include "BData.h"
#include "NeoDoc.h"


class CUndo  
{
public:
	CUndo();
	virtual ~CUndo();

public:
	BOOL Clear();
	BOOL IsValid();
	BOOL Restore();
	BOOL SaveDeleteProperty(BObject* pobj, ULONG lngPropertyID);

private:
	CNeoDoc* m_pDoc;
	BObject* m_pobj;
	ULONG m_lngObjectID;
	ULONG m_lngPropertyID;
	BData* m_pdat;
};


