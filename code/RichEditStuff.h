
// some stuff from mfc needed to get things to compile


#pragma once




//` from where?
class CReObject : public _reobject
{
public:
	CReObject();
	CReObject(CRichEditCntrItem* pItem);
	~CReObject();
};





// OLE data (like AUX_DATA)
//` from oleimpl2.h
struct OLE_DATA
{
	// OLE 1.0 clipboard formats
	UINT    cfNative, cfOwnerLink, cfObjectLink;
	// OLE 2.0 clipboard formats
	UINT    cfEmbeddedObject, cfEmbedSource, cfLinkSource;
	UINT    cfObjectDescriptor, cfLinkSourceDescriptor;
	UINT    cfFileName, cfFileNameW;
	//RichEdit formats
	UINT    cfRichTextFormat;
	UINT    cfRichTextAndObjects;
	OLE_DATA();
};
extern OLE_DATA _oleData;








