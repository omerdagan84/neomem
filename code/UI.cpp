

#include "precompiled.h"
#include "UI.h"

#include "Brooklyn.h"

//, #include "Dialogs.h"  ?
#include "DialogEditDate.h"
#include "DialogEditString.h"



CUI::CUI() {
}


CUI::~CUI() {
}





//----------------------------------


// The code in the case stmts used to be spread out through the BData classes,
// but in separating the db from the ui needed to remove it here.


BOOL CUI::EditDate(BObject* pobj, OBJID idProperty) {
	ASSERT_VALID(pobj);
	ASSERT(idProperty);

	BDataDate* pdat = DYNAMIC_DOWNCAST(BDataDate, pobj->GetPropertyData(idProperty, TRUE));

	// If storing a string bring up edit string dialog
//x	if (m_bitsFlags.Type == flagString)
	if (pdat->IsStringDate()) {
		CDialogEditString dlg;
//x		dlg.m_strValue = m_strText;
		dlg.m_strValue = pdat->GetBDataText(NULL, 0); //, params not used or needed - fix
		if (dlg.DoModal() == IDOK)
		{
			// Save new string value
//x			m_strText = dlg.m_strValue;
			pdat->SetBDataText(dlg.m_strValue); //, this is where you can set flag for showing error/interactive dialog or not. what should we do with it? 
			// This also sets the document modified flag and updates views.
			pobj->SetPropertyData(idProperty, pdat);
			return TRUE;
		}
		return FALSE;
	}

	// Get current date/time if date is empty or invalid
//x	if (m_odt.GetStatus() != COleDateTime::valid) {
//		m_odt = COleDateTime::GetCurrentTime();
//	}

	if (!pdat->IsValid())
		pdat->SetDate(COleDateTime::GetCurrentTime());


	COleDateTime odt = pdat->GetDate();

	// Check if year is before 1753
	if (odt.GetYear() < 1753) {
		if (IDNO == AfxMessageBox("Warning: The calendar control does not handle dates before "
									"September 1, 1752 due to the change from the Julian to the Gregorian "
									"calendar. You can still use the date dialog but you won't be able to select "
									"dates prior to this date. \n"
									"Continue?", MB_YESNO + MB_ICONQUESTION))
			return FALSE;
	}
	// Check if any flags are set
//x	else if (
//		(m_bitsFlags.Modifiers != 0 && m_bitsFlags.Modifiers != flagUseTime) || 
//		((m_bitsFlags.Type != 0) && (m_bitsFlags.Type != flagBlank))
//		)
	else if (pdat->AreFlagsSet())
	{
		if (IDNO == AfxMessageBox("Warning: By selecting a new value with the Edit Date dialog you will lose "
								"any current modifiers (e.g. After, Before, Year Only, Year Month, etc.). \n"
								"Continue?", MB_YESNO + MB_ICONQUESTION))
			return FALSE;
	}


	CDialogEditDate dlg;
	dlg.m_dtDate = odt;
	dlg.m_dtTime = odt;
//x	dlg.m_bUseTime = FALSE; // default is to just enter date
//x	dlg.m_bUseTime = m_bitsFlags.Modifiers && flagUseTime;
	dlg.m_bUseTime = pdat->UseTime();
	if (dlg.DoModal() == IDOK)
	{
		// Save new date and time
		COleDateTime& odtDate = dlg.m_dtDate;
		COleDateTime& odtTime = dlg.m_dtTime;
		int nRet = odt.SetDateTime(odtDate.GetYear(), odtDate.GetMonth(), odtDate.GetDay(),
									odtTime.GetHour(), odtTime.GetMinute(), odtTime.GetSecond());
		if (nRet == 0) 
		{
			pdat->SetDate(odt);

			// Set flags
			pdat->ResetFlags(dlg.m_bUseTime);
//x			m_bitsFlags.Type = 0;
//			m_bitsFlags.Relationship = 0;
//			m_bitsFlags.Modifiers = dlg.m_bUseTime ? flagUseTime : 0;
//			m_bitsFlags.Season = 0;


			// This also sets the document modified flag and updates views.
			pobj->SetPropertyData(idProperty, pdat);

			// Note: BObject's UIEditValue will set document modified flag and update all views
			return TRUE;
		}
		AfxMessageBox("Error in setting date. Please try again.", MB_ICONINFORMATION);
	}
	return FALSE;
}





// Bring up dialog to edit string value.
// Updates value and returns TRUE if user hit OK in dialog.
BOOL CUI::EditEmail(BObject* pobj, OBJID idProperty) {

	CDialogEditString dlg;
//x	dlg.m_strValue = m_strText;
	dlg.m_strValue = pobj->GetPropertyString(idProperty); //,   ,TRUE!
	if (dlg.DoModal() == IDOK)
	{
		// Save new string value
//x		m_strText = dlg.m_strValue;
		pobj->SetPropertyString(idProperty, dlg.m_strValue);
		return TRUE;
	}
	return FALSE;
}



BOOL CUI::EditString(BObject* pobj, OBJID idProperty) {
	//, will want to say the object we're editing and the property name
	// Object: System
	// Property: Description
	//, for most string properties, we should use cdialogeditname
	// for description, etc. use the bigger one.
	// for now leave like this

	CDialogEditString dlg;
	dlg.m_strValue = pobj->GetPropertyString(idProperty); //,   ,TRUE!
	if (dlg.DoModal() == IDOK)
	{
		// Save new string value
		// This also sets the document modified flag and updates views.
		pobj->SetPropertyString(idProperty, dlg.m_strValue);
		return TRUE;
	}
	return FALSE;
}



//----------------


// Edit object's property value in a dialog
BOOL CUI::EditValue(BObject* pobj, OBJID idProperty) {
	ASSERT_VALID(pobj);
	ASSERT(idProperty);

	CWaitCursor cw;

	BDoc* pdoc = pobj->GetDoc();
	BObject* pobjPropdef = pdoc->GetObject(idProperty);
	
	// Check if property is read-only
	if (pobjPropdef->GetFlag(flagPropValueReadOnly)) {
		AfxMessageBox("This property value is read-only and cannot be changed.", MB_ICONINFORMATION);
		return FALSE;
	}


	OBJID idProptype = pobjPropdef->GetPropertyLink(propPropertyType);

	switch (idProptype) {
		case proptypeDate:
			return EditDate(pobj, idProperty);
			break;
		case proptypeEmail:
			return EditEmail(pobj, idProperty);
			break;
		case proptypeString:
			return EditString(pobj, idProperty);
			break;
		default:
			AfxMessageBox("missing case stmt");
			ASSERT(0);
			return FALSE;
	}

	return FALSE;
}

