
// BDataDate


#include "precompiled.h"

#include "BDataDate.h"
#include "ConstantsDatabase.h"
#include "NeoDoc.h"


#include "NeoMem.h"
#include "DialogEditDate.h"
#include "DialogEditString.h"
#include "StringEx.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




IMPLEMENT_SERIAL(BDataDate, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataDate::BDataDate()
{
//	m_odt.SetStatus(COleDateTime::null);

	m_lngFlags.Type = 0;
	m_lngFlags.Relationship = 0;
	m_lngFlags.Modifiers = 0;
	m_lngFlags.Season = 0;

	m_odt = COleDateTime::GetCurrentTime();
}

BDataDate::~BDataDate()
{
}



// Set date via text parser
// Check modifiers first (abt, bef, ca)
// Then check for month year - otherwise date parsing will just convert "aug 2000" to 8/1/2000
// Tokens must be stored in order longest to shortest for parsing to work right
// First token will be the output format - not used for input!
// Bug: Modified m_lngFlags directly even though date in string might have been invalid. Caught in code.
BOOL BDataDate::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	// Copy the string so we can modify it
	CStringEx strCopy = str;
	strCopy.TrimLeft();
	strCopy.TrimRight();
	strCopy.MakeLower(); // faster to just do this once rather than use case-insensitive comparison functions
	CString strCopyBackup; // for monthyear processing

	// Check for blank string
	//, ideally delete this property value but for now just set blank
	if (strCopy.IsEmpty())
	{
		m_lngFlags.Type = flagBlank;
		m_strText.Empty();
		return TRUE;
	}

	// Initialize new flags struct
	sDateFlags lngNewFlags;
	lngNewFlags.Type = 0;
	lngNewFlags.Relationship = 0;
	lngNewFlags.Modifiers = 0;
	lngNewFlags.Season = 0;

	// Check for modifiers (after, before, circa)
	UINT nModifierID = strCopy.GetNextTokenID(IDS_DATE_AFTER, IDS_DATE_CIRCA, TRUE);
	if (nModifierID)
	{
		ULONG nModifiers[] = {flagAfter, flagBefore, flagCirca};
		lngNewFlags.Modifiers = nModifiers[nModifierID - IDS_DATE_AFTER];
	}

	// Check for question mark at end (uncertain)
	if (strCopy.Right(1) == "?")
	{
		// Remove question mark and set modifier flag
		strCopy = strCopy.Left(strCopy.GetLength() - 1);
		lngNewFlags.Modifiers |= flagUncertain;
	}

	// Check for 's at end (plural), e.g. "1800's", "1970s"
//	if (strCopy.Right(2) == "'s")
//	{
//		// Remove 's and set modifier flag
//		strCopy = strCopy.Left(strCopy.GetLength() - 2);
//		lngNewFlags.Modifiers |= flagPlural;
//	}

	// Middle of year
	int nMonth = 7;
	int nDay = 1;

	// Check for seasons and holidays
	UINT nSeasonID = strCopy.GetNextTokenID(IDS_DATE_SPRINGBREAK, IDS_DATE_HALLOWEEN, TRUE);
	if (nSeasonID)
	{
		lngNewFlags.Type = flagSeasonYear;
		int nSeason = nSeasonID - IDS_DATE_SPRINGBREAK + 1;
		lngNewFlags.Season = nSeason;
		// set date here! this is why would be better to store holidays as bobjects (eventually)
		// ie could have date properties with them!
//		enum eDateSeasons {flagSpringBreak = 1, flagSummer, flagAutumn, flagWinter, flagNewYearsEve,
//										flagNewYearsDay, flagEaster, flagChristmas, flagThanksgiving,
//										flagSpring, flagHalloween};
		// store array of dates - need month and day
		BYTE nDates[] = {4,12,  7,15,  10,15,  1,15,  12,31,  1,1,  4,20,  12,25,  11, 25,  4,15,  10,31};
		nMonth = nDates[ (nSeason - 1) * 2 ];
		nDay = nDates[ (nSeason - 1) * 2 + 1];
		// Note: We'll be expecting a year to follow at this point
	}
	else
	{
		// Check for month year
		// Save backup of string in case user entered "jan 15 1988" or somesuch
		strCopyBackup = strCopy; 
		UINT nMonthID = strCopy.GetNextTokenID(IDS_DATE_JANUARY, IDS_DATE_DECEMBER, TRUE);
		if (nMonthID)
		{
			lngNewFlags.Type = flagMonthYear;
			nMonth = nMonthID - IDS_DATE_JANUARY + 1;
			nDay = 15;
		}
	}

	// If we can convert date to an integer between 100 and 9999, then it's year only.
	// The function stops reading the input string at the first character that it cannot recognize 
	// as part of a number. This character may be the null character ('\0') terminating the string.
	ULONG lngYear = atol(strCopy);
	if (lngYear >= 100 && lngYear <= 9999)
	{
		// Check if there's anything after the number, eg "1991 and 1998" should fail!
		CString strYear;
		strYear.Format("%d", lngYear);
		strCopy.TrimLeft(); // make sure no spaces in front of number
		strCopy.TrimRight(); // or after it
		// Accept the year if that's all there is in the string
		if (strCopy == strYear)
		{		
			// Set flags, but don't overwrite type if set to flagSeasonYear or flagMonthYear, for instance.
			if (lngNewFlags.Type == 0)
				lngNewFlags.Type = flagYear;
			m_lngFlags = lngNewFlags;

			// Set date - if flagYear will be middle of year
			m_odt.SetDateTime(lngYear, nMonth, nDay, 0, 0, 0);
			return TRUE;
		}
	}

	// Restore string to state before removing the month
	if (lngNewFlags.Type == flagMonthYear)
	{
		lngNewFlags.Type = 0; // also clear flag since monthyear format test failed
		strCopy = strCopyBackup;
	}

	// Check if it's a normal date
	if (lngNewFlags.Type != flagSeasonYear)
	{
		// Use COleDateTime's parser to convert string to date.
		// Use a temporary object to avoid overwriting our good data in case it fails.
		COleDateTime dt; 
//		if (dt.ParseDateTime(strCopy, VAR_DATEVALUEONLY))
		if (dt.ParseDateTime(strCopy, LOCALE_NOUSEROVERRIDE))
		{
			// Set flags and save new date
			lngNewFlags.Type = 0; // normal date
			if (dt.GetHour() == 0 && dt.GetMinute() == 0 && dt.GetSecond() == 0)
			{
				// Just set date
				m_odt.SetDate(dt.GetYear(), dt.GetMonth(), dt.GetDay());
			}
			else
			{
				// Since time is != midnight, user speficied a time, so set flag and save time also
				lngNewFlags.Modifiers = flagUseTime;
				m_odt = dt;
			}
			m_lngFlags = lngNewFlags;
			return TRUE;
		}
	}

	// See if user wants to store the 'date' as a string
	if (bShowErrorMessage)
	{
//		AfxMessageBox("Invalid date. Please try again.");
		CString strMsg;
		strMsg.Format(_T("The text you entered '%s' could not be parsed as a date. Do you want to store it as a string?"), (LPCTSTR) str); 
		if (IDNO == AfxMessageBox(strMsg, MB_ICONQUESTION + MB_YESNO))
		{
			return FALSE;
		}
	}

	// Set flags
	lngNewFlags.Type = flagString;
	m_lngFlags = lngNewFlags;

	// Store 'date' as string
	m_strText = str;

	return TRUE;
}



// Get text representation of date into string, eg "January 15, 1988".
LPCTSTR BDataDate::GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	// Return string if string flag is set
	if (m_lngFlags.Type == flagString)
	{
		return m_strText;
	}

	// Return blank string if date is blank (null)
	if (m_lngFlags.Type == flagBlank)
	{
		return m_strText; // ""
	}

	// Get date, ignoring time portion
//	m_strText = m_odt.Format(VAR_DATEVALUEONLY);
	//. use global format specified in options dialog
	//, could have propdef override the format? eg to store time instead of date?
	if (m_odt.GetStatus() != COleDateTime::valid)
	{
		// BUG:: Used GetCurrentTime incorrectly here - assumed that it was a method used
		// to fill the object with current time. Instead, it's a static function, kinda bizarre.
		// As a result, m_odt remained invalid. 
//		m_odt.GetCurrentTime();
		m_odt = COleDateTime::GetCurrentTime();
	}

	if (bMachineVersion)
	{
		m_strText = m_odt.Format("%Y-%m-%d %H:%M:%S");
	}
	else
	{
		// Handle our custom formats, like year only, approximate dates, etc.
		switch (m_lngFlags.Type)
		{
			case flagYear:
				m_strText.Format("%d", m_odt.GetYear());
				break;

			case flagMonthYear:
				m_strText = m_odt.Format("%B %Y"); // Month Year
				break;

			case flagSeasonYear:
				{
					//, make method in cstringex
					CStringEx strTokens;
					strTokens.LoadString(IDS_DATE_SPRINGBREAK + m_lngFlags.Season - 1);
					TCHAR pszDelimiters[] = _T(",");
					LPTSTR pszTokens = strTokens.GetBuffer(0);
					TCHAR* pszToken = _tcstok(pszTokens, pszDelimiters); 
					ASSERT(pszToken); // there should always be at least one token
					m_strText.Format("%s %d", pszToken, m_odt.GetYear());
					strTokens.ReleaseBuffer();
					break;
				}
			case flagUnknown:
				m_strText = "Unknown";
				break;
			case 0: // Normal date
			default:
				// Format date as string.
				// Note: GetDateFormat only works back to January 1, 1601.
	//			SYSTEMTIME systime;
	//			if (m_odt.GetAsSystemTime(systime))
	//			{
	//				// Use short or long date based on global option.
	//				DWORD dwFlags = (theApp.m_nDateFormat == 0) ? DATE_SHORTDATE : DATE_LONGDATE;
	//				const int nChars = 50;
	//				LPTSTR pszBuffer = m_strText.GetBuffer(nChars);
	//				::GetDateFormat(NULL, dwFlags, &systime, NULL, pszBuffer, nChars);
	//				m_strText.ReleaseBuffer();
	//			}
	//			else
				if (m_lngFlags.Modifiers && flagUseTime)
				{
					// Want to display time portion, so format the whole date/time object.
					// Oddly enough, this seems to actually use the Windows system settings for the short date format. 
					m_strText = m_odt.Format();
				}
				else
				{
					// If don't want to display time portion, just use the ymd portion of date for format
					COleDateTime dt;
					dt.SetDate(m_odt.GetYear(), m_odt.GetMonth(), m_odt.GetDay());
					// Oddly enough, this seems to actually use the Windows system settings for the short date format. 
					m_strText = dt.Format(); // format using system settings
				}
				break;
		}

		// Add modifiers
		UINT nModifiers = m_lngFlags.Modifiers;
		if (nModifiers & flagAfter)
			m_strText = _T("After ") + m_strText;
		else if (nModifiers & flagBefore)
			m_strText = _T("Before ") + m_strText;
		else if (nModifiers & flagCirca)
			m_strText = _T("Ca. ") + m_strText;

		if (nModifiers & flagUncertain)
			m_strText = m_strText + _T("?");
		
	//	ULONG lngRelationship = m_lngFlags & flagsRelationship;
	//	switch (lngRelationship)
	//	{
	//	}
	}

	return m_strText;
}



void BDataDate::Serialize(CArchive &ar)
{
	// Call base class first
	BData::Serialize(ar);

	if (ar.IsStoring())
	{
		// Cast our flags struct to a long and write it
		ULONG lng = *((ULONG*) (&m_lngFlags));
		ar << lng;

		if (m_lngFlags.Type == flagString)
			ar << m_strText;
		else
			ar << m_odt;		
	}
	else // loading
	{
//		int nFileVersion = ar.GetObjectSchema();

		// Read in a long then cast it into our struct
		ULONG lng;
		ar >> lng;
		*((ULONG*) (&m_lngFlags)) = lng;
		if (m_lngFlags.Type == flagString)
			ar >> m_strText;
		else
			ar >> m_odt;
	}
}



// Bring up dialog to enter date/time
// Updates value and returns TRUE if user hit OK in dialog
BOOL BDataDate::EditValue(BObject* pobj, BObject* pobjPropertyDef)
{
	// Check assumptions
	ASSERT_VALID(this);
	ASSERT_VALID(pobj);
	ASSERT_VALID(pobjPropertyDef);
//	ASSERT_VALID(m_pobj); // not always true

	// If storing a string bring up edit string dialog
	if (m_lngFlags.Type == flagString)
	{
		CDialogEditString dlg;
		dlg.m_strValue = m_strText;
		if (dlg.DoModal() == IDOK)
		{
			// Save new string value
			m_strText = dlg.m_strValue;
			return TRUE;
		}
		return FALSE;
	}

	// Get current date/time if date is empty or invalid
	if (m_odt.GetStatus() != COleDateTime::valid)
	{
		m_odt = COleDateTime::GetCurrentTime();
	}

	// Check if year is before 1753
	if (m_odt.GetYear() < 1753)
	{
//		AfxMessageBox("Note: The calendar control does not handle dates before September 1, 1752, due to the change from the Julian to the Gregorian calendar. You can still use the date dialog, but you won't be able to select dates prior to the changeover.");
		if (IDNO == AfxMessageBox("Warning: The calendar control does not handle dates before \n"
													"September 1, 1752 due to the change from the Julian to the Gregorian \n"
													"calendar. You can still use the date dialog but you won't be able to select \n"
													"dates prior to this date. Continue?", MB_YESNO + MB_ICONQUESTION))
			return FALSE;
	}
	// Check if any flags are set
	else if (
		(m_lngFlags.Modifiers != 0 && m_lngFlags.Modifiers != flagUseTime) || 
		((m_lngFlags.Type != 0) && (m_lngFlags.Type != flagBlank))
		)
	{
		if (IDNO == AfxMessageBox("Warning: By selecting a new value with the Edit Date dialog you will lose \n"
													"any current modifiers (e.g. After, Before, Year Only, Year Month, etc.). Continue?", MB_YESNO + MB_ICONQUESTION))
			return FALSE;
	}

	CDialogEditDate dlg;
	dlg.m_dtDate = m_odt;
	dlg.m_dtTime = m_odt;
//	dlg.m_bUseTime = FALSE; // default is to just enter date
	dlg.m_bUseTime = m_lngFlags.Modifiers && flagUseTime;
	if (dlg.DoModal() == IDOK)
	{
		// Save new date and time
		COleDateTime& dtDate = dlg.m_dtDate;
		COleDateTime& dtTime = dlg.m_dtTime;
//		m_odt.SetTime(dtTime.GetHour(), dtTime.GetMinute(), dtTime.GetSecond());
		int nRet = m_odt.SetDateTime(dtDate.GetYear(), dtDate.GetMonth(), dtDate.GetDay(),
									dtTime.GetHour(), dtTime.GetMinute(), dtTime.GetSecond());
		if (nRet == 0) 
		{
			// Set flags?
			m_lngFlags.Type = 0;
			m_lngFlags.Relationship = 0;
//			m_lngFlags.Modifiers = 0;
			m_lngFlags.Modifiers = dlg.m_bUseTime ? flagUseTime : 0;
			m_lngFlags.Season = 0;

			// Note: BObject's EditValue will set document modified flag and update all views
			return TRUE;
		}
		AfxMessageBox("Error in setting date. Please try again.", MB_ICONINFORMATION);
	}
	return FALSE;
}



// Create a copy of this object
BData* BDataDate::CreateCopy()
{
	BDataDate* pdatCopy = new BDataDate();
	pdatCopy->m_odt = m_odt;
	pdatCopy->m_lngFlags = m_lngFlags; // bug: forgot this!
	pdatCopy->m_strText = m_strText;
	return (BData*) pdatCopy;
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataDate::FindReferences(BObject* pobjFind)
{
	return FALSE;
}



ULONG BDataDate::GetMemoryUsed(BOOL bRecursive)
{
	ULONG nBytes = sizeof(BDataDate);
	return nBytes;
}


