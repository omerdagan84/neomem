
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



class BDataDate : public BData {

	DECLARE_SERIAL(BDataDate)

public:
	BDataDate();
	BDataDate(LPCTSTR pszText);
	virtual ~BDataDate();

public:
	virtual BData* CreateCopy();
	virtual BOOL FindReferences(BObject* pobjFind);
	virtual ULONG GetMemoryUsed(BOOL bRecursive);
	virtual CString GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion=FALSE);
	virtual void Serialize(CArchive &ar);
	virtual BOOL SetBDataText(const CString& str, BObject* pobjPropertyDef = 0, BOOL bShowErrorMessage = TRUE);
	virtual BOOL UIEditValue(BObject* pobj, BObject* pobjPropertyDef);

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


