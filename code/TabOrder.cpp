
// CTabOrder



#include "precompiled.h"
#include "NeoMem.h"
#include "TabOrder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




CTabOrder::CTabOrder()
{
}

CTabOrder::~CTabOrder()
{
}


// Get the next ID
UINT CTabOrder::GetNextID(UINT nID)
{
	int nNew = 0;
	int nItems = GetSize();
	int nCurrent = FindID(nID);
	if (nCurrent == nItems - 1)
		nNew = 0;
	else
		nNew = nCurrent + 1;
	UINT nNewID = GetAt(nNew);
	return nNewID;
}


// Find the specified ID and return its index
int CTabOrder::FindID(UINT nID)
{
	int nItems = GetSize();
	for (int i = 0; i < nItems; i++)
	{
//		if (GetAt(i) == 0)
		if (GetAt(i) == nID)
			return i;
	}
	return 0;
}


// Get previous ID
UINT CTabOrder::GetPrevID(UINT nID)
{
	int nNew = 0;
	int nItems = GetSize();
	int nCurrent = FindID(nID);
	if (nCurrent == 0)
		nNew = nItems - 1;
	else
		nNew = nCurrent - 1;
	UINT nNewID = GetAt(nNew);
	return nNewID;
}




