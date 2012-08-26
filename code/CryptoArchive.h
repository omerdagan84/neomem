
// CCryptoArchive
// Simple extension to the CArchive class so we can get access to protected data members.
// Wound up making some routines to pass the info to the associated CCryptoFile class. 


#pragma once

#include "Crypto.h"


class CCryptoArchive : public CArchive
{
public:
	CCryptoArchive(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL);
	void SetCryptoOn(CCrypto* pCrypto);
	void SetCryptoOff();
};


