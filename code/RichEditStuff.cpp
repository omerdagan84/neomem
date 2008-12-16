
#include "precompiled.h"

#include "richeditstuff.h"



/*

CReObject::CReObject()
{
	cbStruct = sizeof(REOBJECT);
	memset(&cbStruct+1, 0, sizeof(*this)-sizeof(cbStruct));
}

CReObject::CReObject(CRichEditCntrItem* pItem)
{
	ASSERT(pItem != NULL);
	cbStruct = sizeof(REOBJECT);

	pItem->GetClassID(&clsid);
	poleobj = pItem->m_lpObject;
	pstg = pItem->m_lpStorage;
	polesite = pItem->m_lpClientSite;
	ASSERT(poleobj != NULL);
	ASSERT(pstg != NULL);
	ASSERT(polesite != NULL);
	poleobj->AddRef();
	pstg->AddRef();
	polesite->AddRef();

	sizel.cx = sizel.cy = 0; // let richedit determine initial size
	dvaspect = pItem->GetDrawAspect();
	dwFlags = REO_RESIZABLE;
	dwUser = 0;
}

CReObject::~CReObject()
{
	if (poleobj != NULL)
		poleobj->Release();
	if (pstg != NULL)
		pstg->Release();
	if (polesite != NULL)
		polesite->Release();
}




//` from oleinit.cpp
OLE_DATA _oleData;
OLE_DATA::OLE_DATA()
{
	// OLE 1.0 Clipboard formats
	cfNative = ::RegisterClipboardFormat(_T("Native"));
	cfOwnerLink = ::RegisterClipboardFormat(_T("OwnerLink"));
	cfObjectLink = ::RegisterClipboardFormat(_T("ObjectLink"));
	// OLE 2.0 Clipboard formats
	cfEmbeddedObject = ::RegisterClipboardFormat(_T("Embedded Object"));
	cfEmbedSource = ::RegisterClipboardFormat(_T("Embed Source"));
	cfLinkSource = ::RegisterClipboardFormat(_T("Link Source"));
	cfObjectDescriptor = ::RegisterClipboardFormat(_T("Object Descriptor"));
	cfLinkSourceDescriptor = ::RegisterClipboardFormat(_T("Link Source Descriptor"));
	cfFileName = ::RegisterClipboardFormat(_T("FileName"));
	cfFileNameW = ::RegisterClipboardFormat(_T("FileNameW"));
	cfRichTextFormat = ::RegisterClipboardFormat(_T("Rich Text Format"));
	cfRichTextAndObjects = ::RegisterClipboardFormat(_T("RichEdit Text and Objects"));
}



*/

