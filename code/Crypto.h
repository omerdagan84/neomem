
// CCrypto
// Wrapper for Windows Crypto API
//-----------------------------------------------------------------------------------------------------------------

// Stream cipher algorithms encrypt data one bit at a time. A stream of plaintext bits 
// flows into the encryption process and a stream of encrypted ciphertext flows out. 
// At least, this is the way it works mathematically; in practice, data is always 
// encrypted in byte units.
// Stream ciphers are not generally considered as secure as block ciphers, although 
// their relative security will vary depending on the particular algorithm. On the 
// other hand, stream ciphers tend to execute faster in software. Ciphertext encrypted 
// with stream ciphers is always the same size as the original plaintext.
// * The only stream cipher provided with the Microsoft Base Cryptographic Provider 
// is the RC4 stream cipher. This stream cipher was developed by RSA Data Security, Inc. 
// The details of this algorithm have not been published.
// RC4 is a variable-key-length cipher. When used with CryptoAPI and the Microsoft 
// Base Cryptographic Provider, key lengths may be queried using CryptGetProvParam 
// with the PP_ENUMALGS_EX parameter value. 
// The key length can be set with the upper 16 bits of the dwFlags parameter 
// in calls to CryptGenKey and CryptDeriveKey.


#pragma once

//#include "precompiled.h"
#include <wincrypt.h>

#include "StringEx.h"

//#define MAXCHARS 16


class CCrypto  
{
// Construction
public:
	CCrypto();
	virtual ~CCrypto();

// Operations
public:
	void Init();
	void CleanUp();
	void MakeKey(CString& strPassword);

	CString GetSessionKeyHash(CString& strPassword);
	void Encrypt(BYTE* pbData, UINT nBytes);
	void Decrypt(BYTE* pbData, UINT nBytes);

//	void Xor(BYTE* pBuffer, UINT nBytes);
//	void EncryptPassword(CString& strIn, CString& strOut);
//	void DecryptPassword(CString& strIn, CString& strOut);
//	void EncryptStringList(CStringList& as);
//	void DecryptStringList(CStringList& as);

// Attributes
protected:
	void Error(); // Give an error message and throw an exception
	HCRYPTPROV m_hProvider; // Handle to Cryptographic Service Provider (CSP)
	HCRYPTKEY m_hSessionKey; // Handle to session key 
	DWORD m_dwProviderType; // Provider type to use
	ALG_ID m_nHashID; // Type of hash algorithm to use for password
	ALG_ID m_nEncryptionID; // Type of encryption to use
};




