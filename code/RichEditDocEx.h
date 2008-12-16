
// CRichEditDocEx



#pragma once



class CRichEditDocEx : public CRichEditDoc  
{
public:
	CRichEditDocEx();
	virtual ~CRichEditDocEx();


	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo = NULL) const;

};




