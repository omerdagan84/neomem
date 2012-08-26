
// CCryptoArchive


#include "precompiled.h"

#include "CryptoArchive.h"
#include "CryptoFile.h"


CCryptoArchive::CCryptoArchive(CFile* pFile, UINT nMode, int nBufSize, void* lpBuf) :
	CArchive(pFile, nMode, nBufSize, lpBuf)
{
}


// Start encrypting or decrypting data to/from the archive, using the specified cryptography object. 
void CCryptoArchive::SetCryptoOn(CCrypto* pCrypto)
{
	// Get pointer to the CCryptoFile associated with the archive.
	// This is actually a CCryptoFile instead of a CFile because we overrode various routines and classes to do so.
	CCryptoFile* pFile = STATIC_DOWNCAST(CCryptoFile, GetFile()); 
	ASSERT_VALID(pFile);

	BOOL bReading = IsLoading();

	// Tell associated CCryptoFile object to turn encryption/decryption on
	pFile->SetCryptoOn(pCrypto, bReading, m_lpBufCur, m_lpBufMax);
}


// Stop encrypting or decrypting data to/from the archive. 
void CCryptoArchive::SetCryptoOff()
{
	// Get pointer to the CCryptoFile associated with the archive.
	// This is actually a CCryptoFile instead of a CFile because we overrode various routines and classes to do so.
	CCryptoFile* pFile = STATIC_DOWNCAST(CCryptoFile, GetFile()); 
	ASSERT_VALID(pFile);

	// Tell associated CCryptoFile object to turn off encryption/decryption
	pFile->SetCryptoOff(m_lpBufCur);
}



