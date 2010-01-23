
// CCrypto


#include "precompiled.h"

#include "Crypto.h"

#include "NeoMem.h"
#include "StringEx.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CCrypto::CCrypto() :
	m_dwProviderType (0),
	m_hProvider (0),
	m_hSessionKey (0),
	m_nEncryptionID (0),
	m_nHashID (0)
{
}

CCrypto::~CCrypto()
{
	CleanUp();
}



void CCrypto::Init()
{
	ASSERT(m_hProvider == 0);

	// The CryptAcquireContext function is used to acquire a handle to a particular key 
	// container within a particular cryptographic service provider (CSP). This 
	// returned handle can then be used to make calls to the selected CSP.
	// This function performs two operations. It first attempts to find a CSP with 
	// the characteristics described in the dwProvType and pszProvider parameters. 
	// If the CSP is found, the function attempts to find a key container within the 
	// CSP matching the name specified by the pszContainer parameter.
	// Note: An application should not use the default key container to store private 
	// keys. When multiple applications use the same container, one application may 
	// change or destroy the keys that another application needs to have available. 
	// It is recommended that applications use key containers that are linked to the 
	// application. Doing so reduces the risk of other applications tampering with 
	// keys that are necessary for an application to function properly.
	// Some advanced applications might connect to more than one CSP at a time, but 
	// most applications generally use only a single CSP.

	// Specify Cryptographic Service Provider (CSP)
	m_dwProviderType = PROV_RSA_FULL; // encryption: rc2, rc4 ; hash: md5, sha
//	m_dwProviderType = PROV_FORTEZZA; // encryption: skipjack; hash: sha
//	m_dwProviderType = PROV_DH_SCHANNEL; // encryption: des, triple des; hash: md5, sha

	m_nEncryptionID = CALG_RC4; // Encryption algorithm to use on data
	m_nHashID = CALG_MD5; // Hash algorithm to use on password
//	LPCTSTR pszProvider = NULL; // bug: need to specify provider explicitly, as differences arose between 2000 and xp! 
	LPCTSTR pszProvider = MS_DEF_PROV;
	LPCTSTR pszContainer = "NeoMemKeyset"; // Specify key container
	DWORD dwFlags = 0; 

	// Try to open the neomem keyset
	// 'The best way to acquire a context is to try to open the container. If this 
	// attempt fails with "NTE_BAD_KEYSET", then create the container by using the 
	// CRYPT_NEWKEYSET flag.'
	if (!::CryptAcquireContext(&m_hProvider, pszContainer, pszProvider, m_dwProviderType, dwFlags)) 
	{
		// If couldn't open keyset then we need to create it
		DWORD dwError = GetLastError();
		if (dwError == NTE_BAD_KEYSET)
		{
			dwFlags = CRYPT_NEWKEYSET; 
			if (!::CryptAcquireContext(&m_hProvider, pszContainer, pszProvider, m_dwProviderType, dwFlags)) 
				Error();
		}
		else
			Error();
	}
}




void CCrypto::CleanUp()
{
	// Destroy the session key
	if (m_hSessionKey) 
		::CryptDestroyKey(m_hSessionKey);

	// Release the provider handle
	if (m_hProvider) 
		::CryptReleaseContext(m_hProvider, 0);

	m_hProvider = 0;
	m_hSessionKey = 0;
}



// Generate a session key from the specified password.
// Password must contain only ASCII characters.
// Eg someone used the euro character and had problems decrypting on win95.
// Code creates a hash of the password, then generates a key from the hash.
// password --> hash of password --> session key
void CCrypto::MakeKey(CString& strPassword)
{
	ASSERT(sizeof(TCHAR) == 1); //, not working with unicode yet
	ASSERT(m_hProvider);

	DWORD dwLength = strPassword.GetLength(); // number of characters. 
// 1.1	ASSERT(dwLength);  // bug - shouldn't have had this!

	// Make sure the password has only Ascii characters (ensures one byte per character).
	// (Other code pages might use two bytes per character for weird characters, so limit to standard Ascii).
	CStringEx strPasswordEx = strPassword;
	ASSERT(strPasswordEx.IsAscii());

	// Create an empty hash object.
	// The CryptCreateHash function initiates the hashing of a stream of data. It creates and 
	// returns to the calling application a handle to a CSP hash object. This handle is used in 
	// subsequent calls to CryptHashData and CryptHashSessionKey to hash session keys and 
	// other streams of data.
	HCRYPTHASH hHash;
	HCRYPTKEY hMACKey = 0; // Set to zero for nonkeyed algorithms (ie use for MAC (message authentication) algorithms)
	if (!::CryptCreateHash(m_hProvider, m_nHashID, hMACKey, 0, &hHash))
		Error();

	// Hash the password string.
	// is this where things are going wrong? hmm, i don't think so. it looks okay...
	const BYTE* pbBuffer = (const BYTE*) (LPCTSTR) strPassword;
	// The CryptHashData function adds data to the specified hash object. 
//	if (!::CryptHashData(hHash, (BYTE*) pszPassword, dwLength, 0)) 
	if (!::CryptHashData(hHash, pbBuffer, dwLength, 0)) 
		Error();

	// Create a session key based on the hash of the password.
	// The CryptDeriveKey function generates cryptographic session keys derived 
	// from a base data value. This function guarantees that when the same CSP 
	// and algorithms are used, the keys generated from the same base data are 
	// identical. The base data can be a password or any other user data.
	// This function is the same as CryptGenKey, except that the generated session 
	// keys are derived from base data instead of being random. CryptDeriveKey can 
	// only be used to generate session keys. 
	// A handle to the session key is returned in the phKey parameter. This handle 
	// can be used with any CryptoAPI function that requires a key handle.
	// When keys are generated for symmetric block ciphers, the key by default is 
	// set up in cipher block chaining (CBC) mode with an initialization vector 
	// of zero. This cipher mode provides a good default method for bulk-encrypting 
	// data. To change these parameters, use the CryptSetKeyParam function.
	// The CryptDeriveKey function completes the hash. After CryptDeriveKey has been 
	// called, no more data can be added to the hash. Additional calls to CryptHashData 
	// or CryptHashSessionKey fail. After the application is done with the hash, 
	// CryptDestroyHash must be called to destroy the hash object.

	// The key length can be set with the upper 16 bits of the dwFlags parameter 
	// in calls to CryptGenKey and CryptDeriveKey.
	// Thus, if a 128-bit RC4 session key is to be generated, the value 0x00800000 is 
	// combined with any other dwFlags predefined value with a bitwise OR operation.
	// Note: The 128-bit keys are only available with the Enhanced provider, which 
	// is only available in the US (plus requires Win2000 etc)
	// So we're stuck with 40-bits for now. 
//	DWORD dwFlags = 0x00800000; 
	DWORD dwFlags = 0; 
	if (!::CryptDeriveKey(m_hProvider, m_nEncryptionID, hHash, dwFlags, &m_hSessionKey)) 
		Error();
	
	// Destroy the hash object.
	// Better security is provided by destroying the hash object with CryptDestroyHash 
	// as soon as the hash value has been obtained.
	if (hHash)
		::CryptDestroyHash(hHash);

	// Now use m_hSessionKey as appropriate to encrypt or decrypt a message.
}


// Encrypt nbytes in specified buffer
void CCrypto::Encrypt(BYTE* pbData, UINT nBytes)
{
	ASSERT(m_hProvider);
	ASSERT(m_hSessionKey);

	// The CryptEncrypt function encrypts data. The algorithm used to encrypt the 
	// data is designated by the key held by the CSP module and is referenced by 
	// the hKey parameter.

	// pbData Pointer to a buffer holding the data to be encrypted. The encrypted 
	// data overwrites the data to be encrypted in this buffer. The size of this 
	// buffer is specified by dwBufLen. 
	// The number of bytes of data to be encrypted is specified by pdwDataLen. 
	// As a rule, if a stream cipher is used the ciphertext is the same size as 
	// the plaintext. 

	BOOL bFinal = FALSE; // for block ciphers
	DWORD dwDataLen = nBytes;
	DWORD dwBufLen = nBytes;
	if (!::CryptEncrypt(m_hSessionKey, NULL, bFinal, 0, pbData, &dwDataLen, dwBufLen))
		Error();

//	{
//		// Under some circumstances, Microsoft cryptographic service providers may not allow encryption 
//		// when used in France, in which case this function fails with the error code NTE_PERM. 
//		DWORD dwError = GetLastError();
//		if (dwError == NTE_PERM)
//		{
//		}
//		else Error();
//	}

}


// Decrypt nbytes in specified buffer
void CCrypto::Decrypt(BYTE* pbData, UINT nBytes)
{
	BOOL bFinal = FALSE; // Set to True for final block of data
	DWORD dwDataLen = nBytes;
	if (!::CryptDecrypt(m_hSessionKey, NULL, bFinal, 0, pbData, &dwDataLen))
		Error();
//	{
//		// Under some circumstances, Microsoft cryptographic service providers may not allow encryption 
//		// when used in France, in which case this function fails with the error code NTE_PERM. 
//		DWORD dwError = GetLastError();
//		if (dwError == NTE_PERM)
//		{
//		}
//		else Error();
//	}
}


/*
// Xor the given buffer with the password
void CCrypto::Xor(BYTE* pBuffer, UINT nBytes)
{
	ASSERT(pBuffer);
	ASSERT(m_pKey);
	ASSERT(m_nKeyLength);
	for (UINT i = 0; i < nBytes; i++)
	{
		BYTE ch = *(pBuffer+i);
		UINT j = i % m_nKeyLength; // cycle through characters in password
		ch ^= *(m_pKey+j); // xor with key
		*(pBuffer+i) = ch;
	}
}




// This will encrypt and encode a given password into a string of hexascii characters.
void CCrypto::EncryptPassword(CString &strIn, CString &strOut)
{
//	ASSERT(m_nKeyLength); // make sure key has been initialized
	if (m_nKeyLength == 0)
	{
		strOut = strIn;
		return;
	}

	ASSERT(strIn.GetLength() <= 16); // 16 char max

	CONST nBytes = 40;
	char buffer[nBytes];
	::ZeroMemory(buffer, nBytes);
	sprintf(buffer, "%s", (LPCTSTR) strIn); // unicode: make sure this will work

	UINT nChars = strIn.GetLength();
	Encrypt((BYTE*) buffer, nChars); // encrypt in place

	// Now convert the data bytes into hexascii characters
	char szHexascii[40];
	CharStr2HexStr((const unsigned char*) buffer, (char*) szHexascii, nChars);
	strOut = szHexascii;
}



// This will decode and decrypt a string of hexascii characters into an ascii string.
// String must be 32 characters in length (representing 16 bytes of data).
void CCrypto::DecryptPassword(CString& strIn, CString& strOut)
{
//	ASSERT(m_nKeyLength);

	// Handle blank password case
	if (strIn.IsEmpty())
	{
		strOut.Empty();
		return;
	}

	// First decode the hexascii into raw data.
	// Passwords are encoded as ASCII text (eg "E8B5..."), so need to convert it to a byte array.
	CONST nData = 40;
	BYTE pData[nData];
	::ZeroMemory(pData, nData);
	int nChars = strIn.GetLength();
	int nBytes = nChars / 2;
	HexStr2CharStr(strIn, pData, nBytes);

	// Now decrypt the raw data into a string
	Decrypt(pData, nBytes);
	strOut = pData;
}



void CCrypto::EncryptStringList(CStringList& as)
{
	ASSERT(m_nKeyLength);
}
void CCrypto::DecryptStringList(CStringList& as)
{
	ASSERT(m_nKeyLength);
}
*/




// Get hash of the session key generated by the given password.
// Returned as a hexascii string.
CString CCrypto::GetSessionKeyHash(CString &strPassword)
{
	// Create a session key based on the password
	MakeKey(strPassword);
	
	// Create an empty hash object
	HCRYPTHASH hHash;
	HCRYPTKEY hMACKey = 0; // Set to zero for nonkeyed algorithms 
							// (ie use for MAC (message authentication) algorithms)
	if (!::CryptCreateHash(m_hProvider, m_nHashID, hMACKey, 0, &hHash))
		Error();

	// Create a hash of the session key
	if (!::CryptHashSessionKey(hHash, m_hSessionKey, 0))
		Error();

	// Now get the hash data
	// Note: This routine will set dwDataLen to the actual amount of data written, 
	// (the number of bytes) which we must use for the hexascii routine below.
	BYTE pbData[20] = {0}; // 20 bytes
	DWORD dwDataLen = 20; // hash values are either 16 or 20 bytes
	if (!::CryptGetHashParam(hHash, HP_HASHVAL, pbData, &dwDataLen, 0))
		Error();
	
	// Now convert the data bytes into hexascii characters
	char szHexAscii[60];
	int nBytes = dwDataLen; // number of bytes of data to convert
	CharStr2HexStr((const unsigned char*) pbData, (char*) szHexAscii, nBytes);

	return szHexAscii;
}




void CCrypto::Error()
{
	CString str;
	DWORD dwError = GetLastError();
	str.Format("Error number %#x occurred while trying to encrypt/decrypt this file. "
		"If possible, try turning off encryption for this file (select File Security "
		"from the File menu).", 
		dwError);
//	theApp.ThrowException(str); // ie make error object and throw it. 
	CError e(FALSE);
	e.SetName(str);
	throw &e;
//	theApp.HandleError(e);
}


