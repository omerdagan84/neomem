
// BDataFlags


#include "precompiled.h"
#include "BDataFlags.h"
#include "ConstantsDatabase.h"
#include "NeoDoc.h"
#include "BObject.h"

#include "NeoMem.h"
#include "DialogEditLink.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif





IMPLEMENT_SERIAL(BDataFlags, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataFlags::BDataFlags()
{
	m_lngFlags = 0;
}

BDataFlags::~BDataFlags()
{
}


// Set data via text parser
BOOL BDataFlags::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
/*
	//, look up object and set to closest match
	// search through objects, starting at pobjStart and recursing downwards
	// could do a fuzzy search
	// if more than one match found, bring up a dialog letting user choose one
	// if no match found, show message box saying that, return false
	//, exclude system objects?
	BObjects aResults;
	int nObjects = pobjStart->m_pDoc->Search(pobjStart, propName, str, aResults);
	if (nObjects == 0)
	{
		CString strMsg;
		strMsg.Format("No objects found to match %s.", (LPCTSTR) str);
		AfxMessageBox(strMsg);
		return FALSE;
	}
	else if (nObjects > 1)
	{
		//, bring up dialog to let user choose one of the found objects
		AfxMessageBox("more than one found - bring up dialog");
		return FALSE;
//		return TRUE;
	}
	else // nObjects == 1
	{
		m_pobj = (BObject*) aResults.GetAt(0);
		ASSERT_VALID(m_pobj);
//		BObject* pobj = (BObject*) aResults.GetAt(0);
//		ASSERT_VALID(pobj);
//		m_lngID = pobj->GetObjectID();
		return TRUE;
	}
*/

	// For now, just convert text to integer.
	// If the first character is '0' and the second character is 'x' or 'X', the string is 
	// interpreted as a hexadecimal integer. If the first character is '1' through '9', 
	// the string is interpreted as a decimal integer. 
//	m_lngFlags = atol(str);
//	TCHAR* pStopChar;
//	m_lngFlags = wcstoul(str, &pStopChar, 10);
	char* pStopChar;
	m_lngFlags = strtoul(str, &pStopChar, 10);
	return TRUE;
}



// Get text representation of data into string
//, this could look for values in a system folder containing flag value objects
//LPCTSTR BDataFlags::GetText()
LPCTSTR BDataFlags::GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	//, Show numbers if in admin, for now
	if (theApp.m_bAdmin)
//		m_strText.Format("%u (%x): ", m_lngFlags, m_lngFlags);
//		m_strText.Format("0x%x: ", m_lngFlags);
		m_strText.Format("%d: ", m_lngFlags);
	else
		m_strText.Empty();

	if (m_lngFlags & flagSystem) m_strText += "System, ";
	if (m_lngFlags & flagAdminOnly) m_strText += "Admin Only, ";
	if (m_lngFlags & flagNoDelete) m_strText += "No Delete, ";
	if (m_lngFlags & flagNoModify) m_strText += "No Modify, ";
	if (m_lngFlags & flagNoAutosort) m_strText += "Autosort Off, ";
	if (m_lngFlags & flagNoModifyClass) m_strText += "No Modify Class, ";
	if (m_lngFlags & flagPropValueReadOnly) m_strText += "Read-Only, ";

	// Remove last comma and space
	if (!m_strText.IsEmpty())
		m_strText = m_strText.Left(m_strText.GetLength() - 2);

	return m_strText;
}



void BDataFlags::Serialize(CArchive &ar)
{
	// Call base class first
	BData::Serialize(ar);

	// serialize the data
	if (ar.IsStoring())
	{
		ar << m_lngFlags;
	}
	else
	{
//		int nFileVersion = ar.GetObjectSchema();
		ar >> m_lngFlags;
	}

}





// Bring up dialog to select a new object to refer to for this link
// root of dialog is based on property def
// eg Author propertydef could point to the People folder, or Authors folder
// eg Class propertydef should point to the Classes folder
// Links will be similar, but will have multiselect
BOOL BDataFlags::UIEditValue(BObject* pobj, BObject* pobjPropertyDef)
{
/*
	ASSERT_VALID(this);
	ASSERT_VALID(pobj);
	ASSERT_VALID(pobjPropertyDef);
	ASSERT_VALID(m_pobj);

//	AfxMessageBox("bdata link edit value (bring up dialog here)");
//	return FALSE;

	CDialogEditLink dlg;
	dlg.m_nHelpID = IDD_EDIT_FLAG;
//	dlg.m_pobjStart = ...
//	dlg.m_pobjSelected = m_pobj;
//	if (dlg.DoModal() == IDOK)
	ULONG lngClassID = m_pobj->GetObjectID();
	if (dlg.DoModalParameters(folderClasses, lngClassID, 0, 0, TRUE) == IDOK)
	{
		// Save new link
		m_pobj = dlg.m_pobjSelected;

		// Set document modified flag
		CNeoDoc* pDoc = pobj->m_pDoc;
		if (pDoc)
			pDoc->SetModifiedFlag(TRUE);

		// but how would that change get written back to the actual m_lngClassID variable?
		// the UIEditValue could handle that - ie check to see if the propertydef is a pseudo property
		// if so, do any special handling required

		// Handle pseudo properties here
		ULONG lngPropertyID = pobjPropertyDef->GetObjectID();
		switch (lngPropertyID)
		{
			case propClassName:
				// pobj->m_lngClassID = m_pobj->GetObjectID();
				pobj->SetClassID(m_pobj->GetObjectID());
				break;
			// really shouldn't let user modify this like this...
//			case propLocation:
		}

		return TRUE;
	}
*/
	return FALSE;
}




BData* BDataFlags::CreateCopy()
{
	BDataFlags* pdatCopy = new BDataFlags;
	pdatCopy->m_lngFlags = m_lngFlags;
	return (BData*) pdatCopy;
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataFlags::FindReferences(BObject* pobjFind)
{
	return FALSE;
}



ULONG BDataFlags::GetMemoryUsed(BOOL bRecursive)
{
	ULONG nBytes = sizeof(BDataFlags);
	return nBytes;
}

