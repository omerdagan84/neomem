
// BDataNumber


#include "precompiled.h"
#include "BDataNumber.h"
#include "ConstantsDatabase.h"

#include "NeoMem.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




IMPLEMENT_SERIAL(BDataNumber, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataNumber::BDataNumber() :
	m_dblValue (0),
	m_lngUnitID (0)
{
}

BDataNumber::~BDataNumber()
{
}


// Set data via text parser.
//, Parse the units also, if included in text. may be before and/or after the value, eg "$35/hr".
BOOL BDataNumber::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjPropertyDef);

	OBJID idPropertyType = pobjPropertyDef->GetPropertyLink(propPropertyType);
	if (idPropertyType == 0)
		idPropertyType = proptypeNumber;

	switch (idPropertyType)
	{
		case proptypeCurrency:
			{
				//, For now, use COleCurrency object to parse the string
				COleCurrency cy;
				if (cy.ParseCurrency(str))
				{
					// Convert currency to double
					VarR8FromCy(cy, &m_dblValue);
					return TRUE;
				}
				else
				{
					if (bShowErrorMessage)
						AfxMessageBox("Invalid currency. Please try again.");
					return FALSE;
				}
				break;
			}
		case proptypeTimeInterval:
			{
				// For now, parse using COleDateTime object
				//, Eventually, handle things like "5 months", "17 years", etc.
				COleDateTime dt;
				if (dt.ParseDateTime(str, VAR_TIMEVALUEONLY))
				{
					// Get total number of seconds
					m_dblValue = dt.GetHour() * 3600 + dt.GetMinute() * 60 + dt.GetSecond();
					return TRUE;
				}
				else
				{
					if (bShowErrorMessage)
						AfxMessageBox("Invalid time interval. Please try again.");
					return FALSE;
				}
				break;
			}
		default:
			// Just treat as a normal number
			m_dblValue = atof(str);
			m_lngUnitID = 0; // for now
			break;
	}
	
	return TRUE;
}



// Get text representation of data into string
LPCTSTR BDataNumber::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	ASSERT_VALID(this);
//	ASSERT_VALID(pDoc);

	// also might want to handle exponents, eg. for planetary information
	// convert distances to AU's?
	// different objects or categories/classes of objects might want different default units
	// could also specify how many decimal places to show
	//,  %2.10f   2 digits to left and 10 to right of decimal point
	//, unit objects could have prefix and postfix abbreviations that we could print out here instead of name:
	// so objCurrency could have .prefix = "$" and .postfix = ""
	// objFeet could have .prefix = "" and .postfix = "ft"
	// could use these in parsing the text as well
	//, handle commas for thousands separators, also periods if european, etc
//	if (m_lngUnitID)
//		m_strText.Format("%f %s", m_sngValue, (LPCTSTR) m_pobjUnit->GetPropertyString(propName));
//	else
//		m_strText.Format("%f", m_sngValue);

	const int nChars = 50;
	TCHAR szPlain[nChars];
	TCHAR szFormatted[nChars];

	if (bMachineVersion)
	{
		_stprintf_s(szFormatted, nChars, "%.15g", m_dblValue);
		// Now convert formatted string to a CString
		m_strText = szFormatted;
	}
	else
	{
		switch (m_lngUnitID)
		{
			case unitCurrency:
				// Convert floating point number to a plain string, eg "1313.4291"
				//, set format/number of digits here from propertydef
				// Bug: Used wsprintf here but it doesn't handle reals
	//			_stprintf(szPlain, "%f");
	//			_stprintf(szPlain, "%10.2f", m_dblValue);
				_stprintf_s(szPlain, nChars, "%.2f", m_dblValue); 
	//			wsprintf(szPlain, "%.2f", m_dblValue); // doesn't handle reals!
				// Convert plain string to formatted string, eg "$1,313.43"
				::GetCurrencyFormat(LOCALE_USER_DEFAULT, 0, szPlain, NULL, szFormatted, nChars);
				// Now convert formatted string to a CString
				m_strText = szFormatted;
				break;

	/*
			case unitSeconds:
				{
					// Convert number of seconds to time, eg "4:35"
	//				::GetTimeFormat(LOCALE_USER_DEFAULT, 0, ..."h':'m':'s");
					// Note: time_t is a long, representing number of seconds, so just convert dbl to long
	//				CTimeSpan ts((time_t) m_dblValue);
	//				m_strText = ts.Format("%h:%M:%S");
					time_t tt = (time_t) m_dblValue;
					COleDateTime dt(tt);
					CString strFormat;
	//				dt.Format("%#H:%M:%S");		
					if (tt < 60)
	//					m_strText.Format("%d seconds", tt);
						strFormat = "%#S";
					else if (tt < 3600)
						strFormat = "%#M:%S";
					else
						strFormat = "#H:%M:%S";
					dt.Format(strFormat);
					break;
				}
	*/

			default:
				// Convert floating point number to a plain string, eg "1,313.4291"
				//, set format/number of digits here from propertydef
	//			_stprintf(szPlain, "%f", m_dblValue);
				//, I'm sure this function is nice, but it uses fixed decimals
				// with g_nf, it works well for integers, but not decimals
	//			::GetNumberFormat(LOCALE_USER_DEFAULT, 0, szPlain, NULL, szFormatted, nChars);
	//			::GetNumberFormat(0, 0, szPlain, &g_nf, szFormatted, nChars);

	//			_stprintf(szFormatted, "%g", m_dblValue);
				// Doubles have 15 digits of precision!
				_stprintf_s(szFormatted, nChars, "%.15g", m_dblValue);
	//			wsprintf(szFormatted, "%.15g", m_dblValue);
				// Now convert formatted string to a CString
				m_strText = szFormatted;
				break;
		}
	}

	return m_strText;
}



void BDataNumber::Serialize(CArchive &ar)
{
	// Call base class first
	BData::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_dblValue;
		ar << m_lngUnitID;
	}
	else
	{
		ar >> m_dblValue;
		ar >> m_lngUnitID;
	}
}




// Create a copy of this bdata object
BData* BDataNumber::CreateCopy()
{
	ASSERT_VALID(this);
	BDataNumber* pdat = new BDataNumber;
	ASSERT_VALID(pdat);
	pdat->m_dblValue = m_dblValue;
	pdat->m_lngUnitID = m_lngUnitID;
	return pdat;
}


//, this will bring up a calculator dialog to let them
// enter amount and select units from combo.
// could have date also?
// for now bring up windows calculator
// (same as bdatalong)
BOOL BDataNumber::UIEditValue(BObject* pobj, BObject* pobjPropertyDef)
{
	CWaitCursor wc;
	HINSTANCE h = ::ShellExecute(NULL, "open", "Calc.exe", NULL, NULL, SW_SHOWNORMAL);
	Library::HandleShellExecuteError(h);
	return FALSE;
}



ULONG BDataNumber::GetMemoryUsed(BOOL bRecursive)
{
	ASSERT_VALID(this);
	ULONG nBytes = sizeof(BDataNumber);
	return nBytes;
}




// Returns True if can find any references to the specified object in this bdata
BOOL BDataNumber::FindReferences(BObject* pobjFind)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjFind);
	if (m_lngUnitID == pobjFind->GetObjectID())
		return TRUE;
	return FALSE;
}


// Remove any references to the specified object, or replace with new one if specified.
BOOL BDataNumber::ReplaceReferences(BObject* pobjFind, BObject* pobjNew /* = 0 */)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pobjFind);
	BOOL bFound = FALSE;
	if (m_lngUnitID == pobjFind->GetObjectID())
	{
		if (pobjNew)
			m_lngUnitID = pobjNew->GetObjectID();
		else
			m_lngUnitID = 0; //, for now
		// Set flag
		bFound = TRUE;
	}
	return bFound;
}


