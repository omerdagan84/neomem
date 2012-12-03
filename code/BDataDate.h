
// BDataDate
// This class stores date/time data.
//-----------------------------------------------------------------------------------------------------------------



#pragma once


#include "BData.h"



// Structure for m_bitsFlags variable.
// we use this to cram a lot of information about the date into 32 bits.
//, make private
struct sDateFlags {
	UINT Type : 4;
	UINT Relationship : 3;
	UINT Modifiers : 10;
	UINT Season : 15;
};


//----------------------------


class BDataDate : public BData {

	DECLARE_SERIAL(BDataDate)

public:
	BDataDate();
	BDataDate(LPCTSTR pszText);
	virtual ~BDataDate();

public:

	// These had been constants in resource.h, but on separating ui from db,
	// needed to move these here. 
	// Must stay in sync with strings in s_dates.
	typedef enum {
		IDS_DATE_EMPTY, // need this to take zeroth place so stringex::getnexttokenid works properly
		IDS_DATE_AFTER,
		IDS_DATE_BEFORE,
		IDS_DATE_CIRCA,
		IDS_DATE_JANUARY,
		IDS_DATE_FEBRUARY,
		IDS_DATE_MARCH,
		IDS_DATE_APRIL,
		IDS_DATE_MAY,
		IDS_DATE_JUNE,
		IDS_DATE_JULY,
		IDS_DATE_AUGUST,
		IDS_DATE_SEPTEMBER,
		IDS_DATE_OCTOBER,
		IDS_DATE_NOVEMBER,
		IDS_DATE_DECEMBER,
		IDS_DATE_SPRINGBREAK,
		IDS_DATE_SUMMER,
		IDS_DATE_AUTUMN,
		IDS_DATE_WINTER,
		IDS_DATE_NEWYEARSEVE,
		IDS_DATE_NEWYEARSDAY,
		IDS_DATE_EASTER,
		IDS_DATE_CHRISTMAS,
		IDS_DATE_THANKSGIVING,
		IDS_DATE_SPRING,
		IDS_DATE_HALLOWEEN 
	} e_dates; // enum dates

	static char* s_dates[]; // string dates


public:
	virtual BData* CreateCopy();
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual CString GetBDataText(BDoc* pDoc=NULL, ULONG lngPropertyID=0, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);

	virtual BOOL UIEditValue(CUI& ui, BObject* pobj = NULL, BObject* pobjPropertyDef = NULL);

public:
	BOOL operator< (const BDataDate& that) { return (m_odt < that.m_odt); };
	BOOL operator== (const BDataDate& that) { return (m_odt == that.m_odt); };

	BOOL AreFlagsSet() { return 
		(m_bitsFlags.Modifiers != 0 && m_bitsFlags.Modifiers != flagUseTime) || 
		((m_bitsFlags.Type != 0) && (m_bitsFlags.Type != flagBlank)); };
	BOOL IsStringDate() { return (m_bitsFlags.Type == BDataDate::flagString); };
	BOOL IsValid() { return (m_odt.GetStatus() == COleDateTime::valid); };
	BOOL UseTime() { return (m_bitsFlags.Modifiers && flagUseTime); };

	void ResetFlags(BOOL bUseTime) { 
						m_bitsFlags.Type = 0;
						m_bitsFlags.Relationship = 0;
						m_bitsFlags.Modifiers = bUseTime ? flagUseTime : 0;
						m_bitsFlags.Season = 0; };

	COleDateTime& GetDate();
	void SetDate(COleDateTime& odt);
	void SetDate(int Year, int Month, int Day, int Hour, int Min, int Sec, 
		int nType = 0, int nRelationship = 0, int nModifiers = 0, int nSeason = 0);

	static COleDateTime NewDate(int Year, int Month, int Day, int Hour, int Min, int Sec);

public: 
	// Type: Only one type allowed. 4 bits = 16 values
	enum eDateTypes {flagYear = 1, flagMonthYear, flagSeasonYear, flagString, flagUnknown, flagBlank};

	// Relationship: Only one type allowed. 3 bits = 8 values
	enum eDateRelationships {flagRange = 1, flagPlusOrMinus, flagAnd, flagOr};

	// Modifiers: may be combined. 10 bits = 10 values 
	// Note: Want to leave room for more modifiers in string table, so make usetime, plural and uncertain last.
	enum eDateModifiers {flagAfter = 1, flagBefore = 2, flagCirca = 4, 
										flagUseTime = 128, flagPlural = 256, flagUncertain = 512}; // (2^10 is 1024)

	// Seasons: Only one season allowed. 15 bits = 32768 values
	// Note: Season enums must match order of strings in string table!
	// Had to switch spring break and spring because parsing "spring" was chopping off "spring" from
	// "spring break" - longer entries must come first!
	enum eDateSeasons {flagSpringBreak = 1, flagSummer, flagAutumn, flagWinter, flagNewYearsEve,
										flagNewYearsDay, flagEaster, flagChristmas, flagThanksgiving,
										flagSpring, flagHalloween};


private:
	// COleDateTime objects can be used to represent dates between January 1, 100, and 
	// December 31, 9999. COleDateTime objects are floating point values, with an approximate 
	// resolution of 1 millisecond
	// The month-calendar control is based on the Gregorian calendar, which was introduced 
	// in 1753. It will not calculate dates that are consistent with the Julian calendar that 
	// was in use prior to 1753. 
	COleDateTime m_odt; // January 1, 100 to December 31, 9999

	sDateFlags m_bitsFlags;

	//, we could use our own format like this...
	// Note: Float stores 3.4E–38 to 3.4E+38, 6 – 7 significant digits
//	float m_sngYear; // Offset from fictional year 0 A.D., truncate to integer portion
//	long m_lngSeconds; // Number of seconds into the year (0 to 31,536,000)

	// Note: CTime is too small to use - 1970 to 2036 or something
	// SYSTEMTIME structure provides the year since 0 AD—in a word-sized member. 
	// Windows does not support dates prior to 1601. See FILETIME for details. 

};


