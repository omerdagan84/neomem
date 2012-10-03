

#include "precompiled.h"
#include "UI.h"

#include "Brooklyn.h"

//#include "Dialogs.h"
#include "DialogEditString.h"



CUI::CUI() {
}


CUI::~CUI() {
}




// The code in the case stmts used to be spread out through the BData classes,
// but in separating the db from the ui needed to remove it here.

BOOL CUI::EditValue(BObject* pobj, OBJID lngPropertyID) {

//	BDoc& doc = *pobj->GetDoc();
//	BObject& objPropdef = doc.GetObject(lngPropertyID);
//	OBJID idProptype = objPropdef.GetPropertyLink(propPropertyType);
	BDoc* pdoc = pobj->GetDoc();
	BObject* pobjPropdef = pdoc->GetObject(lngPropertyID);
	OBJID idProptype = pobjPropdef->GetPropertyLink(propPropertyType);
	switch (idProptype) {
		case proptypeString:
			{
				//, will want to say the object we're editing and the property name
				// Object: System
				// Property: Description
				//, for most string properties, we should use cdialogeditname
				// for description, etc. use the bigger one.
				// for now leave like this

				CDialogEditString dlg;
				dlg.m_strValue = pobj->GetPropertyString(lngPropertyID);
				if (dlg.DoModal() == IDOK)
				{
					// Save new string value
					pobj->SetPropertyString(lngPropertyID, dlg.m_strValue);
					return TRUE;
				}
			}
			break;
//		case proptypeDate:
//			break;
		default:
			AfxMessageBox("missing case stmt");
			ASSERT(0);
			return FALSE;
	}

	return FALSE;
}

