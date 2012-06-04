
// BDataIcon


#include "precompiled.h"
#include "BDataIcon.h"
#include "ConstantsDatabase.h"

#include "NeoMem.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



// These structures are from the article "Icons in Win32"

typedef struct
{
	BYTE        bWidth;          // Width, in pixels, of the image
	BYTE        bHeight;         // Height, in pixels, of the image
	BYTE        bColorCount;     // Number of colors in image (0 if >=8bpp)
	BYTE        bReserved;       // Reserved ( must be 0)
	WORD        wPlanes;         // Color Planes
	WORD        wBitCount;       // Bits per pixel
	DWORD       dwBytesInRes;    // How many bytes in this resource?
	DWORD       dwImageOffset;   // Where in the file is this image?
} ICONDIRENTRY, *LPICONDIRENTRY;

typedef struct
{
	WORD           idReserved;   // Reserved (must be 0)
	WORD           idType;       // Resource Type (1 for icons)
	WORD           idCount;      // How many images?
	ICONDIRENTRY   idEntries[1]; // An entry for each image (idCount of 'em)
} ICONDIR, *LPICONDIR;

typedef struct
{
	BITMAPINFOHEADER   icHeader;      // DIB header
	RGBQUAD         icColors[1];   // Color table
	BYTE            icXOR[1];      // DIB bits for XOR mask
	BYTE            icAND[1];      // DIB bits for AND mask
} ICONIMAGE, *LPICONIMAGE;






IMPLEMENT_SERIAL(BDataIcon, BData, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number


// Construction/Destruction
//------------------------------------------------------------------------------------------------------

BDataIcon::BDataIcon() :
	m_pData (0),
	m_nBytes (0)
{
}

// Delete any memory allocated on the heap that we own.
BDataIcon::~BDataIcon()
{
	if (m_pData)
		delete m_pData;
}


// Set data via text parser
BOOL BDataIcon::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */)
{
	return FALSE;
}



// Get text representation of data into string
LPCTSTR BDataIcon::GetBDataText(CNeoDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion)
{
	// Get number of images in file
	//, could also say format of images (size, color depth)
	if (VerifyData())
	{
		ICONDIR* pIconDir = (ICONDIR*) m_pData;
		WORD wCount = pIconDir->idCount;
		m_strText.Format("[Icon Data - %d bytes, %d image(s)]", m_nBytes, wCount);
	}
	else
	{
		m_strText = "No data";
	}
	return m_strText;
}



ULONG BDataIcon::GetMemoryUsed(BOOL bRecursive)
{
	ULONG nBytes = sizeof(BDataIcon);
	nBytes += m_nBytes;
	return nBytes;
}



// Verify that the data is a valid icon image
BOOL BDataIcon::VerifyData()
{
	// Verify ICONDIR structure
	ICONDIR* pIconDir = (ICONDIR*) m_pData;
	if (pIconDir == 0) return FALSE;
	if (pIconDir->idReserved != 0) return FALSE; // reserved (must be 0)
	if (pIconDir->idType != 1) return FALSE; // resource type (1=icon)
	if (pIconDir->idCount > 50) return FALSE; // number of images
	return TRUE;
}


// GetIcon finds the nearest icon in size and creates it using CreateIconFromResourceEx.
// x and y are the desired size in pixels.
// For now assumes x = y
// Returns a handle to the icon or 0 if failed.
HICON BDataIcon::GetIcon(int x, int y)
{
	HICON hIcon = 0;
	ICONIMAGE* pIconImage = 0;
	ICONDIRENTRY* pIconDirEntry = 0;
	DWORD nBytes = 0;

	// Verify data
	if (VerifyData() == FALSE)
		return 0;

	// ICONDIR is the first structure in the icon data
	ICONDIR* pIconDir = (ICONDIR*) m_pData;
		
	// Get number of images in file
	WORD wCount = pIconDir->idCount;
	ASSERT(wCount);
	
	DWORD dwAddress;

	// Loop through and analyze each image to find best fit
	for (int i = 0; i < wCount; i++)
	{
		// Calculate location of IconDirEntry for this image
		dwAddress = (DWORD) pIconDir + 3 * sizeof(WORD) + i * sizeof(ICONDIRENTRY);
		pIconDirEntry = (ICONDIRENTRY*) dwAddress;

		// Get pointer to ICONIMAGE structure and size of icon data
		dwAddress = (DWORD) pIconDir + pIconDirEntry->dwImageOffset;
		pIconImage = (ICONIMAGE*) dwAddress;
		nBytes = pIconDirEntry->dwBytesInRes;

		// Test size of image - if a match then exit the for loop
		// (assume height is same as width)
		if (x == pIconDirEntry->bWidth)
			break;
	}

	// If icon data found create icon from it
	// bug: icon data could be bad, in which case this api routine crashes badly. 
	// 1.1d added try and catch here (note: the TRY/CATCH macros didn't catch this exception!)
	if (pIconImage)
	{
		try 
		{
			hIcon = ::CreateIconFromResourceEx((UCHAR*) pIconImage, nBytes, TRUE, 0x0030000, x, y, LR_DEFAULTCOLOR);
		}
		catch(...)
		{
			hIcon = 0;
		}
	}

	//, either put this here or let UI handle it
//	if (hIcon == NULL)
//	{
//		AfxMessageBox("Error creating icon!");
//		DWORD dwError = ::GetLastError();
//		FormatMessage
//	}

	return hIcon;
}


// Open the specified .ico file and import its contents into this bdata object.
// m_pData will point to the memory block if successful.
// Returns True if successful.
BOOL BDataIcon::LoadFile(LPCTSTR szFileName)
{
	CFile f;
	CFileException fe;
	BOOL bOK = f.Open(szFileName, CFile::modeRead, &fe);
	if (bOK)
	{
		m_nBytes = f.GetLength();

		// Allocate chunk of memory and read entire file into it
		m_pData = new unsigned char[m_nBytes];
		ASSERT(m_pData);
		UINT nActual = f.Read(m_pData, m_nBytes);
		ASSERT (nActual == m_nBytes);

		// Verify that data is in Icon format
		bOK = VerifyData();
	}
	else
	{
		// If there are problems, the Open call can return a CFileException object in its last 
		// parameter, as shown here. You can call the AfxThrowFileException function if you 
		// require more detailed error reporting.
		fe.ReportError();
	}
	return bOK;
}



void BDataIcon::Serialize(CArchive &ar)
{
	// Call base class first
	BData::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_nBytes;
		ar.Write(m_pData, m_nBytes); //, may throw cfileexception
	}
	else
	{
		ar >> m_nBytes;
		// allocate m_nBytes bytes at m_pData
		m_pData = new unsigned char [m_nBytes];
		if (m_pData)
		{
			UINT nRead = ar.Read(m_pData, m_nBytes); //, may throw cfileexception
			ASSERT (nRead == m_nBytes); // Verify that right number of bytes was read
		}
	}
}





// Returns True if can find any references to the specified object in this bdata
BOOL BDataIcon::FindReferences(BObject* pobjFind)
{
	return FALSE;
}

