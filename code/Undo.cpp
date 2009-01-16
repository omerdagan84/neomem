
// CUndo



#include "precompiled.h"

#include "Undo.h"

#include "ConstantsDatabase.h"  // for classProperty


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CUndo::CUndo()
{
	m_pDoc = 0;
	m_pobj = 0;
	m_lngObjectID = 0;
	m_pdat = 0;
	m_lngPropertyID = 0;
}

CUndo::~CUndo()
{
	// Delete m_pdat since we own it
	if (m_pdat)
	{
		ASSERT_VALID(m_pdat);
		delete m_pdat;
	}
}




// Call this before deleting a property value so can restore it later.
// Returns True if can save the property value.
BOOL CUndo::SaveDeleteProperty(BObject *pobj, ULONG lngPropertyID)
{
	ASSERT_VALID(pobj);
	// First clear the current information, if any
	Clear();
	// Now see if there is actually any data to save
	BData* pdat = pobj->GetPropertyData(lngPropertyID); // get bdata
	if (pdat)
	{
		m_pDoc = pobj->m_pDoc; // save document
		m_pobj = pobj; // store object to restore property to also
		m_lngObjectID = pobj->GetObjectID(); // save object id
		m_lngPropertyID = lngPropertyID; // store property to restore to also
		m_pdat = pdat->CreateCopy(); // we own the copy now!!
		return TRUE;
	}
	return FALSE;
}



// Check if object, property, and property value are still valid
BOOL CUndo::IsValid()
{
	BOOL bValid = FALSE;
	if ((m_pobj != NULL) && (m_pdat != NULL) && (m_lngPropertyID))
	{
		BObject* pobjPropertyDef = m_pDoc->GetObject(m_lngPropertyID);
		if (
			(m_pobj == m_pDoc->GetObject(m_lngObjectID)) && // check pobj is still valid
			(m_pdat->IsValid(m_pDoc)) && // check pdat is still valid
			(pobjPropertyDef != NULL) &&  // check property is still valid
			(pobjPropertyDef->GetClassID() == classProperty) // "
			)
		{
			bValid = TRUE;
		}
	}
	return bValid;
}



// Restore the deleted property value and mark this undo object invalid
BOOL CUndo::Restore()
{
	if (IsValid())
	{
		// SetPropertyData returns true if it keeps pdat, or false if it doesn't
		if (m_pobj->SetPropertyData(m_lngPropertyID, m_pdat))
//			delete m_pdat; // if returned false, we must delete data
			m_pdat = 0; // bobject kept bdata, so we don't need to delete it
//		m_pobj = 0;
//		m_pdat = 0; // we no longer own it
//		m_lngPropertyID = 0;
		Clear();
		return TRUE;
	}
	return FALSE;
}



// Clear this undo object, deleting the bdata copy if we still own it
BOOL CUndo::Clear()
{
	m_pDoc = 0;
	m_pobj = 0;
	m_lngObjectID = 0;
//	m_pdat = 0;
	if (m_pdat)
		delete m_pdat;
	m_pdat = 0;
	m_lngPropertyID = 0;
	return TRUE;
}
