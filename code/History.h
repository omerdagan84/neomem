
// CHistory
// Stores a round-robin history list of objects the user has visited, 
// implemented by taking the modulus of positions to fit into a limited 
// size array.
//-------------------------------------------------------------------------



#pragma once


class CHistory : public CObArray  
{
// Construction
public:
	CHistory();
	virtual ~CHistory();

// Operations
public:
	int GetForwardPositions();
	int GetBackPositions();
	BOOL IsBackValid();
	BOOL IsForwardValid();
	BOOL SetSize(int nPositions);
	BOOL SetCurrent(CObject* pobj);
	CObject* GoBack();
	CObject* GoForward();

// Attributes
private:
	int m_nPositions; // number of positions in array
	int m_nStart; // max back position
	int m_nPos; // current position
	int m_nStop; // max forward position
};

