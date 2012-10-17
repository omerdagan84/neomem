
// CRichEditDocEx


#include "precompiled.h"
#include "neomem.h"
#include "RichEditDocEx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CRichEditDocEx::CRichEditDocEx()
{
}

CRichEditDocEx::~CRichEditDocEx()
{
	//trace("CRichEditDocEx::Destructor\n");
}



CRichEditCntrItem* CRichEditDocEx::CreateClientItem(REOBJECT* preo) const
{
	return CRichEditDoc::CreateClientItem(preo);
}
