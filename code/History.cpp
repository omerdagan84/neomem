
// CHistory


#include "precompiled.h"
#include "NeoMem.h"
#include "History.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CHistory::CHistory()
{
	m_nStart = 0;
	m_nStop = -1;
	m_nPos = -1;
	m_nPositions = 0;
}

CHistory::~CHistory()
{
}


BOOL CHistory::SetSize(int nPositions)
{
	m_nPositions = nPositions;
	CObArray::SetSize(nPositions);
	return TRUE;
}



BOOL CHistory::IsForwardValid()
{
	return (GetForwardPositions() > 0);
}

BOOL CHistory::IsBackValid()
{
	return (GetBackPositions() > 0);
}


// Set current position to the specified object
BOOL CHistory::SetCurrent(CObject* pobj)
{
	m_nPos++;
	SetAt(m_nPos % m_nPositions, pobj);
	m_nStop = m_nPos; // save stop position (max forward)
	
	// Move start position up if we're exceeding the number of positions in array
	if (m_nPos - m_nStart >= m_nPositions)
		m_nStart++;

	return TRUE;
}


// How many back positions are available?
int CHistory::GetBackPositions()
{
	return (m_nPos - m_nStart);
}

// How many forward positions are available?
int CHistory::GetForwardPositions()
{
	return (m_nStop - m_nPos);
}


// Get previous object in history list
CObject* CHistory::GoBack()
{
	CObject* pobj = 0;
	if (GetBackPositions() > 0)
	{
		m_nPos--;
		pobj = GetAt(m_nPos % m_nPositions);
	}
	return pobj;
}


// Get next object in history list
CObject* CHistory::GoForward()
{
	CObject* pobj = 0;
	if (GetForwardPositions() > 0)
	{
		m_nPos++;
		pobj = GetAt(m_nPos % m_nPositions);
	}
	return pobj;
}


