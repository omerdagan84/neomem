
// BDataPersonName
// Encapsulates a person's name
//----------------------------------------------------------------------------------------------------


#include "precompiled.h"
#include "BDataPersonName.h"
#include "ConstantsDatabase.h"
#include "BDoc.h"

#include "NeoMem.h"
//x#include "DialogEditPersonName.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




IMPLEMENT_SERIAL(BDataPersonName, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataPersonName::BDataPersonName() :
	m_bCacheValid (FALSE)
{
}

BDataPersonName::~BDataPersonName()
{
}


// Set data via text parser
BOOL BDataPersonName::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	ASSERT_VALID(this);

	// Clear names first
	m_strTitle.Empty();
	m_strFirst.Empty();
	m_strMiddle.Empty();
	m_strLast.Empty();
	m_strNickname.Empty();
	m_strSuffix.Empty();

	// Parse the name into first, middle, last, etc.
	// Then set the valid flag so the full name will be regenerated on the next call to GetText
	//, define names in BNR notation so we can parse easily - 
	// do very simple parse for now - just first and last names
	// will eventually want to handle "BURNS, Brian", "John Jones Jr.", "Daniel Ivan Jordan Sigmund Hart III", 
	// "Dr. Martin Luther King, Jr.", "William Jefferson Clinton", "Mrs. Jane Doe", "Joe 'Bud' Fahey", "Sting", etc.
	//, if unsure how to parse, bring up dialog
	
	// First check for a comma
	int n = str.Find(',');
	if (n != -1)
	{
		m_strLast = str.Left(n);
		m_strFirst = str.Mid(n+1);
		m_strFirst.TrimLeft(); // remove any preceding spaces in first name
	}
	else
	{
		// No comma, so search for space
		int n = str.ReverseFind(' ');
		if (n == -1)
		{
			// No space in string so assign to last name
			m_strLast = str;
		}
		else
		{
			// Space found, so divy up into first and last names
			m_strFirst = str.Left(n);
			m_strLast = str.Mid(n+1);
		}
	}

	yTRACE("Parsed %s into %s and %s!\n", (LPCTSTR) str, (LPCTSTR) m_strFirst, (LPCTSTR) m_strLast);

	// Reset flag so will know to recalculate full name
	m_bCacheValid = FALSE;

	return TRUE;
}




// Get text representation of data into string
CString BDataPersonName::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDoc);

	// Check if name is already created and valid
	if (m_bCacheValid)
		return m_strText;

	// Clear cache
	m_strText.Empty();

	switch (theApp.m_nNameFormat)
	{
		case nfLastFirst:
			{
				// eg Hughes, Sir Ted F. "Crow" Jr.
				LPCTSTR pszGap = "";
				if (!m_strLast.IsEmpty()) { m_strText = m_strLast; pszGap = g_strCommaSpace; }
				if (!m_strTitle.IsEmpty()) { m_strText += pszGap + m_strTitle + g_strSpace; pszGap = "";}
				if (!m_strFirst.IsEmpty()) { m_strText += pszGap + m_strFirst + g_strSpace; pszGap = "";}
				if (!m_strMiddle.IsEmpty()) { m_strText += pszGap + m_strMiddle + g_strSpace; pszGap = "";}
				if (!m_strNickname.IsEmpty()) { m_strText += pszGap + g_strQuote + m_strNickname + g_strQuoteSpace; pszGap = "";}
				if (!m_strSuffix.IsEmpty()) { m_strText += pszGap + m_strSuffix; pszGap = "";}
				break;
			}
//		case nfLASTFirst:
//		case nfFirstLAST:
//			// eg Ted F. "Crow" HUGHES, Jr.
//			CString strLast = m_strLast;
//			strLast.MakeUpper();
//			m_strText = m_strFirst + strSpace + strLast;
//			break;
		case nfFirstLast:
		default:
			// eg Sir Ted F. "Crow" Hughes, Jr.
			if (!m_strTitle.IsEmpty()) m_strText = m_strTitle + g_strSpace;
			if (!m_strFirst.IsEmpty()) m_strText += m_strFirst + g_strSpace;
			if (!m_strMiddle.IsEmpty()) m_strText += m_strMiddle + g_strSpace;
			if (!m_strNickname.IsEmpty()) m_strText += g_strQuote + m_strNickname + g_strQuoteSpace;
			if (!m_strLast.IsEmpty()) m_strText += m_strLast;
			if (!m_strSuffix.IsEmpty()) m_strText += g_strCommaSpace + m_strSuffix;
			break;
	}

	// Remove any trailing spaces
	m_strText.TrimRight();

	// Set flag so we know not to calculate it again
	m_bCacheValid = TRUE;

	return m_strText;

/*
	//, if on parsing the string, we stored the lengths of each segment, then we would know the
	// exact string length to get here, and wouldn't have to check for overflow.
	// this would speed up the copying and use the minimum amount of memory necessary,
	// eliminating the need to call FreeExtra

	// will involve less copying if we use getbuffer, because then we'll be writing
	// directly into the cstring's memory, and it won't need to be copied on
	// assignment to the cstring
	const int nChars = 256;
//	TCHAR szBuffer[nChars];
	m_strText.Empty(); // empty string first so getbuffer won't have to copy any data
	LPTSTR pBuffer = m_strText.GetBuffer(nChars);
	for (int i = 0; i < nChars; i++)
	{
		int nCharsLeft = nChars - i;
		for (int j = 0; j < nCharsLeft; j++)
		{
		}
	}

	// make macro that will walk through each string and copy it to the buffer?
	if (m_lngFlags & flagFirst) m_strText = m_strFirst + strSpace;
	if (m_lngFlags & flagMiddle) m_strText += m_strMiddle + strSpace;
	if (m_lngFlags & flagNickname) m_strText += strQuote + m_strNickname + strQuoteSpace;
	if (m_lngFlags & flagLast) m_strText += m_strLast;
	if (m_lngFlags & flagSuffix) m_strText += strCommaSpace + m_strSuffix;
	for (i = 0; i < nChars; i++)
	{
		szBuffer[i] = m_strFirst[j];
	}

//	strcpy(lpBuffer, "abc");   // use the buffer directly
	lpPos = lpBuffer;

	// Last, First
	//, make sure it doesn't overwrite buffer
	nChars = m_strLast.GetLength();
	nCharsTotal += nChars;
	if (nCharsTotal > nCharsMax)
	_tcscpy(lpPos, m_strLast); lpPos += m_strLast.GetLength();
	_tcscpy(lpPos, lpszCommaSpace); lpPos += 2;
	_tcscpy(lpPos, m_strFirst); lpPos += m_strFirst.GetLength();

	int nNewLength = (long) lpPos - (long) lpBuffer;

	//, keep track of length of new string so when we call release buffer it won't have to scan the string.
	m_strText.ReleaseBuffer(nNewLength);
	// could free extra memory also but would add another deallocate and allocate...
//	m_strText.FreeExtra();

	return m_strText;
*/
}


// Edit the name in a dialog.
// Updates value and returns TRUE if user hit OK in dialog.
BOOL BDataPersonName::UIEditValue(BObject* pobj, BObject* pobjPropertyDef, CUI& ui)
{
	ASSERT_VALID(this);
//	ASSERT_VALID(pobj);
//	ASSERT_VALID(pobjPropertyDef);

//x
/*
	CDialogEditPersonName dlg;

	dlg.m_strTitle = m_strTitle;
	dlg.m_strFirst = m_strFirst;
	dlg.m_strMiddle = m_strMiddle;
	dlg.m_strNickname = m_strNickname;
	dlg.m_strLast = m_strLast;
	dlg.m_strSuffix = m_strSuffix;

	// Bring up simple dialog to edit name parts
	if (dlg.DoModal() == IDOK)
	{
		// Save new string values
		m_strTitle = dlg.m_strTitle;
		m_strFirst = dlg.m_strFirst;
		m_strMiddle = dlg.m_strMiddle;
		m_strNickname = dlg.m_strNickname;
		m_strLast = dlg.m_strLast;
		m_strSuffix = dlg.m_strSuffix;

		// Reset flag so will know to recalculate full name
		m_bCacheValid = FALSE;

		return TRUE;
	}
	return FALSE;
*/

	if (ui.EditName(m_strTitle, m_strFirst, m_strMiddle, m_strNickname, m_strLast, m_strSuffix)) {
		// Reset flag so will know to recalculate full name
		m_bCacheValid = FALSE;
		return TRUE;
	}
	return FALSE;
}



void BDataPersonName::Serialize(CArchive& ar)
{
	// Call the base class first
	BData::Serialize(ar);

	if (ar.IsStoring())
	{
		// Save to file
		ar << m_strTitle;
		ar << m_strFirst;
		ar << m_strMiddle;
		ar << m_strLast;
		ar << m_strNickname;
		ar << m_strSuffix;
	}
	else
	{
		ar >> m_strTitle;
		ar >> m_strFirst;
		ar >> m_strMiddle;
		ar >> m_strLast;
		ar >> m_strNickname;
		ar >> m_strSuffix;
	}
}




// Create a copy of this object
BData* BDataPersonName::CreateCopy()
{
	ASSERT_VALID(this);
	BDataPersonName* pdatCopy = new BDataPersonName();
	ASSERT_VALID(pdatCopy);
	pdatCopy->m_strTitle = m_strTitle;
	pdatCopy->m_strFirst = m_strFirst;
	pdatCopy->m_strLast = m_strLast;
	pdatCopy->m_strMiddle = m_strMiddle;
	pdatCopy->m_strNickname = m_strNickname;
	pdatCopy->m_strSuffix = m_strSuffix;
	return pdatCopy;
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataPersonName::FindReferences(BObject* pobjFind)
{
	return FALSE;
}


ULONG BDataPersonName::GetMemoryUsed(BOOL bRecursive)
{
	ASSERT_VALID(this);
	ULONG nBytes = sizeof(BDataPersonName);
	nBytes += m_strTitle.GetLength() * sizeof(TCHAR);
	nBytes += m_strFirst.GetLength() * sizeof(TCHAR);
	nBytes += m_strLast.GetLength() * sizeof(TCHAR);
	nBytes += m_strMiddle.GetLength() * sizeof(TCHAR);
	nBytes += m_strNickname.GetLength() * sizeof(TCHAR);
	nBytes += m_strSuffix.GetLength() * sizeof(TCHAR);
//,?	nBytes += m_strText.GetLength() * sizeof(TCHAR);
	return nBytes;
}



// Virtual function used to reset cache flag for this bdata object
void BDataPersonName::ResetData()
{
	m_bCacheValid = FALSE;
}



