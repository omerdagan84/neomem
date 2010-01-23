
// CHint
// This class is used in transferring information about events between views
// (eg delete an object, rename an object, etc).
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "BObject.h"



class CHint : public CObject {

public:
	CHint();
	virtual ~CHint();


	BObject* m_pobjObject;
	BObject* m_pobjParent;
	BObjects* m_paObjects;
	ULONG m_idProperty; // for property change notifications
	BObject* m_pobjTarget; // for move up and move down
	BOOL m_bAfter; // for move up and move down - true if move after target, false if move before

};



