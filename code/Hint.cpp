
// CHint


#include "precompiled.h"
#include "NeoMem.h"
#include "Hint.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CHint::CHint() :
	m_bAfter (FALSE),
	m_idProperty (0),
	m_paObjects (NULL),
	m_pobjObject (NULL),
	m_pobjParent (NULL),
	m_pobjTarget (NULL)
{
}

CHint::~CHint() {
}






