
// CHint


#include "precompiled.h"
#include "NeoMem.h"
#include "Hint.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CHint::CHint()
{
	m_pobjObject = 0;
	m_pobjParent = 0;
	m_paObjects = 0;
	m_lngPropertyID = 0;
}

CHint::~CHint()
{
}






