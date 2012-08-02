
// CHint
// This struct is used in transferring information about events between views
// (eg delete an object, rename an object, etc).
//-----------------------------------------------------------------------------

#pragma once


class BObject;
class BObjects;


struct CHint : public CObject {
	BObject* pobjObject;
	BObject* pobjParent; // not used?
	BObjects* paObjects;
	ULONG idProperty; // for property change notifications
	BObject* pobjTarget; // for move up and move down
	BOOL bAfter; // for move up and move down - true if move after target, false if move before
};



