
// CCryptoArchive


#include "precompiled.h"

#include "CryptoArchive.h"
#include "CryptoFile.h"


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





/*
UINT CCryptoArchive::GetBufferRemaining()
{
	UINT nBytes = (UINT) (m_lpBufMax - m_lpBufCur);
	return nBytes;
}
*/




//#include "zlib.h"


/*
CString signature = _TEXT("fng");

CCryptoArchive::CCryptoArchiveCFile* pFile, UINT nMode, int nBufSize, void* lpBuf, CString Key, BOOL bCompress)
			:CArchive(pFile, nMode, nBufSize, lpBuf)
{
	m_strKey = Key;
	m_bCompress = bCompress;
	m_pFileTmp = m_pFile;

	// Always compress file if encrypting file
	if (!Key.IsEmpty())
		m_bCompress = TRUE;

	if (m_bCompress)
	{
		if (IsLoading())
		{
			// Decompress file
		    int iComprErr;
			CString Contents, Compress;
			_TCHAR sign[4];
			DWORD dwFileSize = m_pFileTmp -> GetLength();
			LPSTR szContents = Contents.GetBuffer(dwFileSize);
			DWORD dwComprSize;

			m_pBuf = (BYTE *)new char[dwFileSize];
			m_pFile = new CMemFile(m_pBuf, dwFileSize, 1024);

			m_pFileTmp -> SeekToBegin();

			ArcHeader ahead;
			m_pFileTmp -> Read(&ahead, sizeof(ahead));

			sign[0] = ahead.chSignature[0];
			sign[1] = ahead.chSignature[1];
			sign[2] = ahead.chSignature[2];
			sign[3] = _TEXT('\0');

			if (signature == sign)
			{
				dwFileSize = m_pFileTmp -> ReadHuge(szContents, dwFileSize);
				if (ahead.uchFlag == 3)
				{
					ULONG crc = adler32(0L, Z_NULL, 0);
					Crypto(szContents, dwFileSize, m_strKey);
					crc = adler32(crc, (const unsigned char*)szContents, dwFileSize);
					if (crc != ahead.ulCRC)
					{
						AfxMessageBox("Password incorrect!!!");
						Abort();
						AfxThrowUserException();
					}
				}

				dwComprSize = ahead.dwOriginalSize;
				LPSTR szCompress = Compress.GetBuffer(dwComprSize);
				iComprErr = uncompress((LPBYTE)szCompress, &dwComprSize, (LPBYTE)szContents, dwFileSize);
				ASSERT(iComprErr == Z_OK);

				m_pFile -> WriteHuge(szCompress, dwComprSize);
			}
			else
			{
				m_pFileTmp -> SeekToBegin();
				dwFileSize = m_pFileTmp -> ReadHuge(szContents, dwFileSize);
				m_pFile -> WriteHuge(szContents, dwFileSize);
			}
			m_pFile -> SeekToBegin();
		}
		else
		{
			m_pBuf = (BYTE *)new char[1024];
			m_pFile = new CMemFile(m_pBuf, 1024, 1024);
		}
	}
}


CCryptoArchive::~CCryptoArchive)
{
	if (m_pFile != NULL && !(m_nMode & bNoFlushOnDelete))
		Close();

	Abort();    // abort completely shuts down the archive
}


void CCryptoArchive::Abort()
{
	// If compression is on, clean up buffers
	if (m_bCompress && m_pFile != NULL)
	{
		m_pBuf = (BYTE *)(((CMemFile*)m_pFile)->Detach());
		delete m_pBuf;
		m_pFile->Close();
		delete m_pFile;
	}

	CArchive::Abort();
}


void CCryptoArchive::Close()
{
	ASSERT_VALID(m_pFile);
	Flush();

	if (m_bCompress)
	{
		if (IsStoring())
		{
		    int iComprErr;
			CString Compress;
			DWORD dwFileSize = m_pFile -> GetLength();
			DWORD dwComprSize;

			m_pBuf = (BYTE *)(((CMemFile*)m_pFile)->Detach());
			dwComprSize = (DWORD)(dwFileSize * 1.1) + 12;
			LPSTR szCompress = Compress.GetBuffer(dwComprSize);
			iComprErr = compress((LPBYTE)szCompress, &dwComprSize, m_pBuf, dwFileSize);
			ASSERT(iComprErr == Z_OK);

			ULONG crc = adler32(0L, Z_NULL, 0);
			crc = adler32(crc, (const unsigned char*)szCompress, dwComprSize);
			Crypto(szCompress, dwComprSize, m_strKey);

			ArcHeader ahead;
			ahead.chSignature[0] = _TEXT('f');
			ahead.chSignature[1] = _TEXT('n');
			ahead.chSignature[2] = _TEXT('g');
			ahead.dwOriginalSize = dwFileSize;
			ahead.uchFlag = m_strKey.IsEmpty() ? 1 : 3;
			ahead.ulCRC = crc;

			m_pFileTmp -> Write(&ahead, sizeof(ahead));
			m_pFileTmp -> WriteHuge(szCompress, dwComprSize);
		}
		else
		{
			m_pBuf = (BYTE *)(((CMemFile*)m_pFile)->Detach());
		}
		delete m_pBuf;
		m_pFile->Close();
		delete m_pFile;
	}
	m_pFile = NULL;
}



bool CCryptoArchive::Crypto(LPSTR lpstrBuffer, ULONG ulLen, CString strKey)
{
	if (strKey.IsEmpty())
		return FALSE;
	
	int nLenKey = strKey.GetLength();

	for (ULONG ii=0; ii<ulLen; ii++)
	{
		for (int jj=0; jj<nLenKey; jj++)
		{
			lpstrBuffer[ii] ^= strKey[jj];
		}
	}
	return TRUE;
}

*/
