
// CCryptoFile
// Extension of CFile class to allow for encryption/decryption of data.
//-----------------------------------------------------------------------------------------------------------------

// Must use a stream cipher so that the encrypted data is the same size as the original data.
// So we're using RC4 40-bit for now. 

// Currently allows you to turn encryption on/off only once - more than that and it'll lose
// track of which parts of the buffer to encrypt.


// CNeoDoc   <->   CCryptoArchive   <->   CCryptoFile    <->   buffer   <->   disk

// what is the buffer size of the cfile??? we need our cfileex's to match it
// 4096 bytes is default, specified in carchive creation
//. need to assert buffer sizes are same, or pass archive buf size to cfileex creator



#include "Crypto.h"


#pragma once


class CCryptoFile : public CFile
{
	DECLARE_DYNAMIC(CCryptoFile)

// Construction
public:
	CCryptoFile(int nBufSize = 4096);
	CCryptoFile(LPCTSTR lpszFileName, UINT nOpenFlags, int nBufSize=4096);
	virtual ~CCryptoFile();

// Operations
public:
	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write( const void* lpBuf, UINT nCount );
	void SetCryptoOn(CCrypto* pCrypto, BOOL bReading, BYTE* pBufCur, BYTE* pBufMax);
	void SetCryptoOff(BYTE* pBufCur);
	virtual void Flush();
	virtual void Close();

// Attributes
protected:
	CCrypto* m_pCrypto; // pointer to crypto object
	BOOL m_bCryptoOn; // flag for encryption/decryption
	BYTE* m_pBufEncryptStart; // point at which to start encrypting the buffer
	BYTE* m_pBufEncryptStop; // point at which to stop encrypting the buffer
};



