
// CCryptoFile


#include "precompiled.h"

#include "CryptoFile.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNAMIC(CCryptoFile, CFile)


// Constructor
CCryptoFile::CCryptoFile(int nBufSize /* = 4096 */) 
	: CFile()
{
	m_pCrypto = 0;
	m_bCryptoOn = FALSE;
	m_pBufEncryptStart = 0;
	m_pBufEncryptStop = 0;
}


// Constructor
CCryptoFile::CCryptoFile(LPCTSTR lpszFileName, UINT nOpenFlags, int nBufSize) 
	: CFile(lpszFileName, nOpenFlags)
{
	m_pCrypto = 0;
	m_bCryptoOn = FALSE;
	m_pBufEncryptStart = 0;
	m_pBufEncryptStop = 0;
}


// Destructor
CCryptoFile::~CCryptoFile()
{
}




void CCryptoFile::Flush()
{
	CFile::Flush();
}

void CCryptoFile::Close()
{
	CFile::Close();
}


//-----------------------------------------------------------------------------------------------------------------

// Note: You may not use CFile operations to alter the state of the file until you have closed the archive. 
// Any such operation will damage the integrity of the archive. You may access the position of the 
// file pointer at any time during serialization by obtaining the archive’s file object from the GetFile 
// member function and then using the CFile::GetPosition function. You should call CArchive::Flush 
// before obtaining the position of the file pointer.



// Turn encryption/decryption on
void CCryptoFile::SetCryptoOn(CCrypto* pCrypto, BOOL bReading, BYTE* pBufCur, BYTE* pBufMax)
{
	ASSERT(pCrypto);

	// Store pointer to crypto object
	m_pCrypto = pCrypto;

	// If in read mode need to decrypt the remains of the buffer so CArchive will get decrypted data!
	if (bReading)
	{
		UINT nBytesRemaining = (UINT) (pBufMax - pBufCur); // bytes remaining in buffer
		m_pCrypto->Decrypt(pBufCur, nBytesRemaining);
	}
	else
	{
		// In write mode we need to remember the current position for write routine
		m_pBufEncryptStart = pBufCur;
	}
	m_bCryptoOn = TRUE;
}



// Turn encryption/decryption off
void CCryptoFile::SetCryptoOff(BYTE* pBufCur)
{
	// Remember current position so we know where to stop encrypting or decrypting
	m_pBufEncryptStop = pBufCur;
	// if in read mode now we need to reverse the decryption from the current position to the end of the buffer!
	// but this isn't really necessary cause we always encrypt to the end of the file
	m_bCryptoOn = FALSE;
}



// Read data into the buffer, decrypting if necessary
UINT CCryptoFile::Read(void* lpBuf, UINT nCount)
{
	if (m_bCryptoOn)
	{
		ASSERT(m_pCrypto);

		// Read data into the buffer
		UINT nBytes = CFile::Read(lpBuf, nCount);

		// Decrypt the data in the buffer 
		m_pCrypto->Decrypt((BYTE*) lpBuf, nCount);

		return nBytes;
	}
	else
		return CFile::Read(lpBuf, nCount);
}


// Write data from the buffer, encrypting if necessary
void CCryptoFile::Write(const void* lpBuf, UINT nCount)
{
	if (m_bCryptoOn)
	{
		ASSERT(m_pCrypto);

		if (nCount) // strange, but sometimes nCount is zero
		{
			// Figure out what part of the buffer we need to encrypt
			BYTE* pStart = (m_pBufEncryptStart == 0) ? (BYTE*) lpBuf : m_pBufEncryptStart;
			BYTE* pStop = (m_pBufEncryptStop == 0) ? ((BYTE*)lpBuf+nCount) : m_pBufEncryptStop;
			UINT nBytes = (UINT) (pStop - pStart);

			// Encrypt the data in the buffer (might just be part of it)
			m_pCrypto->Encrypt(pStart, nBytes); 

			// Write the entire buffer
			CFile::Write(lpBuf, nCount); 

			// Now clear the start and stop positions so next time we'll encrypt the entire buffer
			m_pBufEncryptStart = 0;
			m_pBufEncryptStop = 0;
		}
	}
	else
		CFile::Write(lpBuf, nCount); 
}



