/*
 *
 *
 * Crypto Algorithm
 *
 *
 * FileName: topcry.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include	"topcry.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"polarssl/base64.h"

#include	"polarssl/aes.h"
#include	"polarssl/des.h"

#include	"polarssl/md5.h"
#include	"polarssl/sha1.h"
#include	"polarssl/sha2.h"
#include	"polarssl/sha4.h"

/*--------------------------- Macro define ------------------------------*/
#define	min(x,y)		((x) < (y) ? (x) : (y))

#define	CRY_BASE64_LINE	64

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
char *
cryHex(const char *sInBuf, int iInLen, char *sOutBuf)
{
	int	iOutLen;

	if (iInLen < 0) iInLen = strlen(sInBuf);
	iOutLen = iInLen * 2 + 1;

	cryHexEncode(sInBuf, iInLen, sOutBuf, &iOutLen);
	/*sOutBuf[iOutLen] = '\0';*/

	return sOutBuf;
}

int
cryUnHex(const char *sInBuf, char *sOutBuf)
{
	int	iOutLen = strlen(sInBuf) / 2;

	if (cryHexDecode(sInBuf, strlen(sInBuf), sOutBuf, &iOutLen) < 0) {
		return -1;
	}

	return iOutLen;
}

char *
cryBase64(const char *sInBuf, int iInLen, char *sOutBuf)
{
	size_t	iOutLen;

	if (iInLen < 0) iInLen = strlen(sInBuf);
	iOutLen = CRY_BASE64_LEN(iInLen);

	base64_encode((unsigned char *)sOutBuf, &iOutLen,
	              (const unsigned char *)sInBuf, iInLen);
	/*sOutBuf[iOutLen] = '\0';*/

	return sOutBuf;
}

int
cryUnBase64(const char *sInBuf, char *sOutBuf)
{
	size_t	iInLen = strlen(sInBuf);
	size_t	iOutLen = iInLen / 4 * 3;

	if (base64_decode((unsigned char *)sOutBuf, &iOutLen,
	                  (const unsigned char *)sInBuf, iInLen) != 0) {
		return -1;
	}

	return iOutLen;
}

char *
cryBase64A(const char *sInBuf, int iInLen)
{
	char	*sOutBuf;
	size_t	iOutLen;

	if (iInLen < 0) iInLen = strlen(sInBuf);
	iOutLen = CRY_BASE64_LEN(iInLen);

	sOutBuf = malloc(iOutLen);
	if (!sOutBuf) return NULL;

	base64_encode((unsigned char *)sOutBuf, &iOutLen,
	              (const unsigned char *)sInBuf, iInLen);
	/*sOutBuf[iOutLen] = '\0';*/

	return sOutBuf;
}

char *
cryBase64AN(const char *sInBuf, int iInLen)
{
	char	*sOutBuf;
	int		iOutLen;

	if (iInLen < 0) iInLen = strlen(sInBuf);
	iOutLen = CRY_BASE64_LEN(iInLen) - 1;
	iOutLen += iOutLen / CRY_BASE64_LINE + 1;

	sOutBuf = malloc(iOutLen);
	if (!sOutBuf) return NULL;

	cryBase64EncodeN(sInBuf, iInLen, sOutBuf, &iOutLen);

	return sOutBuf;
}

char *
cryUnBase64A(const char *sInBuf, int iInLen, int *piOutLen)
{
	char	*sOutBuf;
	size_t	iOutLen;

	if (iInLen < 0) iInLen = strlen(sInBuf);
	iOutLen = iInLen / 4 * 3 + 1;

	sOutBuf = malloc(iOutLen);
	if (!sOutBuf) return NULL;
	memset(sOutBuf, 0, iOutLen);

	if (base64_decode((unsigned char *)sOutBuf, &iOutLen,
	                  (const unsigned char *)sInBuf, iInLen) != 0) {
		return NULL;
	}

	if (piOutLen) *piOutLen = iOutLen;
	return sOutBuf;
}

int
cryHexEncode(const char *sInBuf, int iInLen, char *sOutBuf, int *piOutLen)
{
	const unsigned char	*sIn = (const unsigned char *)sInBuf;
	unsigned char		*sOut = (unsigned char *)sOutBuf;
	unsigned char		cLow, cHigh;

	if (iInLen < 0) iInLen = strlen(sInBuf);

	if (*piOutLen < iInLen * 2 + 1) {
		*piOutLen = iInLen * 2 + 1;
		return -1;
	}

	*piOutLen = iInLen * 2;

	while (iInLen != 0) {
		cHigh = (*sIn) / 16;
		cLow = (*sIn) % 16;

		if (cHigh < 10) {
			*sOut++ = '0' + cHigh;
		} else {
			*sOut++ = 'A' + cHigh - 10;
		}

		if (cLow < 10) {
			*sOut++ = '0' + cLow;
		} else {
			*sOut++ = 'A' + cLow - 10;
		}

		++sIn;
		iInLen--;
	}
	*sOut = '\0';

	return 0;
}

int
cryHexDecode(const char *sInBuf, int iInLen, char *sOutBuf, int *piOutLen)
{
	unsigned char	*sOut = (unsigned char *)sOutBuf;
	unsigned char	cHigh, cLow;

	if (iInLen < 0) iInLen = strlen(sInBuf);

	if (*piOutLen < iInLen / 2) {
		*piOutLen = iInLen / 2;
		return -1;
	}

	*piOutLen = iInLen / 2;

	if (iInLen % 2) {
		return -2;
	}

	while (*sInBuf != 0) {
		cHigh = *sInBuf++;
		if (cHigh >= '0' && cHigh <= '9') {
			cHigh -= '0';
		} else if (cHigh >= 'a' && cHigh <= 'f') {
			cHigh -= 'a' - 10;
		} else if (cHigh >= 'A' && cHigh <= 'F') {
			cHigh -= 'A' - 10;
		} else {
			return -3;
		}

		cLow = *sInBuf++;
		if (cLow >= '0' && cLow <= '9') {
			cLow -= '0';
		} else if (cLow >= 'a' && cLow <= 'f') {
			cLow -= 'a' - 10;
		} else if (cLow >= 'A' && cLow <= 'F') {
			cLow -= 'A' - 10;
		} else {
			return -4;
		}

		*sOut++ = (cHigh << 4) | cLow;
	}

	return 0;
}

int
cryBase64Encode(const char *sInBuf, int iInLen, char *sOutBuf, int *piOutLen)
{
	size_t	iOutLen = *piOutLen;

	if (iInLen < 0) iInLen = strlen(sInBuf);
	if (base64_encode((unsigned char *)sOutBuf, &iOutLen,
	                  (const unsigned char *)sInBuf, iInLen) != 0) {
		*piOutLen = iOutLen;
		return -1;
	}

	*piOutLen = iOutLen;
	return 0;
}

int
cryBase64EncodeN(const char *sInBuf, int iInLen, char *sOutBuf, int *piOutLen)
{
	size_t	iLen;

	if (iInLen < 0) iInLen = strlen(sInBuf);

	iLen = CRY_BASE64_LEN(iInLen) - 1;
	if (*piOutLen < iLen + iLen / CRY_BASE64_LINE + 1) {
		return -1;
	}

	*piOutLen = 0;
	while (iInLen > 0) {
		iLen = CRY_BASE64_LINE + 1;
		base64_encode((unsigned char *)sOutBuf, &iLen,
		              (const unsigned char *)sInBuf,
		              min(iInLen, CRY_BASE64_LINE / 4 * 3));
		sOutBuf[iLen++] = '\n';
		sOutBuf[iLen] = '\0';

		sOutBuf += iLen;
		*piOutLen += iLen;
		sInBuf += CRY_BASE64_LINE / 4 * 3;
		iInLen -= CRY_BASE64_LINE / 4 * 3;
	}

	return 0;
}

int
cryBase64Decode(const char *sInBuf, int iInLen, char *sOutBuf, int *piOutLen)
{
	size_t	iOutLen = *piOutLen;

	if (iInLen < 0) iInLen = strlen(sInBuf);
	if (base64_decode((unsigned char *)sOutBuf, &iOutLen,
	                  (const unsigned char *)sInBuf, iInLen) != 0) {
		*piOutLen = iOutLen;
		return -1;
	}

	*piOutLen = iOutLen;
	return 0;
}

int
cryDes3UtilEncrypt(const char *sInBuf, int iInLen, char *sOutBuf, int *piOutLen,
                   const char sKey[24])
{
	des3_context	tCtx;

	if (*piOutLen < (iInLen / 8 + (iInLen % 8 == 0 ? 0 : 1)) * 8)
		return -1;

	des3_set3key_enc(&tCtx, (unsigned char *)sKey);

	*piOutLen = 0;
	while (iInLen > 0) {
		if (iInLen >= 8)
			des3_crypt_ecb(&tCtx, (unsigned char *)sInBuf + *piOutLen,
			                      (unsigned char *)sOutBuf + *piOutLen);

		else {
			memcpy(sOutBuf + *piOutLen, sInBuf + *piOutLen, iInLen);
			memset(sOutBuf + *piOutLen + iInLen, 0, 8 - iInLen);
			des3_crypt_ecb(&tCtx, (unsigned char *)sOutBuf + *piOutLen,
			                      (unsigned char *)sOutBuf + *piOutLen);
		}

		*piOutLen += 8;
		iInLen = iInLen > 8 ? iInLen - 8 : 0;
	}

	return 0;
}

int
cryDes3UtilDecrypt(const char *sInBuf, int iInLen, char *sOutBuf, int *piOutLen,
                   const char sKey[24])
{
	des3_context	tCtx;

	if (*piOutLen < (iInLen / 8 + (iInLen % 8 == 0 ? 0 : 1)) * 8)
		return -1;

	des3_set3key_dec(&tCtx, (unsigned char *)sKey);

	*piOutLen = 0;
	while (iInLen > 0) {
		if (iInLen >= 8)
			des3_crypt_ecb(&tCtx, (unsigned char *)sInBuf + *piOutLen,
			                      (unsigned char *)sOutBuf + *piOutLen);

		else {
			memcpy(sOutBuf + *piOutLen, sInBuf + *piOutLen, iInLen);
			memset(sOutBuf + *piOutLen + iInLen, 0, 8 - iInLen);
			des3_crypt_ecb(&tCtx, (unsigned char *)sOutBuf + *piOutLen,
			                      (unsigned char *)sOutBuf + *piOutLen);
		}

		*piOutLen += 8;
		iInLen = iInLen > 8 ? iInLen - 8 : 0;
	}

	return 0;
}

int
cryDes3Encrypt(const char *sKey, int iKeyLen,
               const char *sInBuf, int iInLen, char *sOutBuf, int *piOutLen)
{
	unsigned char	sHash[32];
	unsigned char	*pIV = sHash + 24;
	des3_context	tCtx;
	int				iLast;

	iLast = iInLen % 8;

	if (iLast) {
		iInLen -= iLast;
		if (*piOutLen < iInLen + 8) {
			return -1;
		}
		*piOutLen = iInLen + 8;
	} else {
		if (*piOutLen < iInLen) {
			return -2;
		}
		*piOutLen = iInLen;
	}

	sha2((const unsigned char *)sKey, iKeyLen, sHash, 0);

	des3_set3key_enc(&tCtx, sHash);

	if (des3_crypt_cbc(&tCtx, DES_ENCRYPT, iInLen, pIV,
		               (const unsigned char *)sInBuf,
		               (unsigned char *)sOutBuf) != 0) {
		return -3;
	}

	if (iLast) {
		memcpy(sOutBuf + iInLen, sInBuf + iInLen, iLast);
		memset(sOutBuf + iInLen + iLast, 0, 8 - iLast);

		if (des3_crypt_cbc(&tCtx, DES_ENCRYPT, 8, pIV,
			               (const unsigned char *)sOutBuf + iInLen,
			               (unsigned char *)sOutBuf + iInLen) != 0) {
			return -4;
		}
	}

	return 0;
}

int
cryDes3Decrypt(const char *sKey, int iKeyLen,
               const char *sInBuf, int iInLen, char *sOutBuf, int *piOutLen)
{
	unsigned char	sHash[32];
	unsigned char	*pIV = sHash + 24;
	des3_context	tCtx;

	if (iInLen % 8 || *piOutLen < iInLen) {
		return -1;
	}

	*piOutLen = iInLen;

	sha2((const unsigned char *)sKey, iKeyLen, sHash, 0);

	des3_set3key_dec(&tCtx, sHash);

	if (des3_crypt_cbc(&tCtx, DES_DECRYPT, iInLen, pIV,
		               (const unsigned char *)sInBuf,
		               (unsigned char *)sOutBuf) != 0) {
		return -2;
	}

	return 0;
}

int
cryAesEncrypt(const char *sKey, int iKeyLen,
              const char *sInBuf, int iInLen, char *sOutBuf, int *piOutLen)
{
	unsigned char	sHash[48];
	unsigned char	*pIV = sHash + 32;
	aes_context		tCtx;
	int				iLast;

	iLast = iInLen % 16;

	if (iLast) {
		iInLen -= iLast;
		if (*piOutLen < iInLen + 16) {
			return -1;
		}
		*piOutLen = iInLen + 16;
	} else {
		if (*piOutLen < iInLen) {
			return -2;
		}
		*piOutLen = iInLen;
	}

	sha4((const unsigned char *)sKey, iKeyLen, sHash, 1);

	aes_setkey_enc(&tCtx, sHash, 256);

	if (aes_crypt_cbc(&tCtx, DES_ENCRYPT, iInLen, pIV,
		              (const unsigned char *)sInBuf,
		              (unsigned char *)sOutBuf) != 0) {
		return -3;
	}

	if (iLast) {
		memcpy(sOutBuf + iInLen, sInBuf + iInLen, iLast);
		memset(sOutBuf + iInLen + iLast, 0, 16 - iLast);

		if (aes_crypt_cbc(&tCtx, DES_ENCRYPT, 16, pIV,
			              (const unsigned char *)sOutBuf + iInLen,
			              (unsigned char *)sOutBuf + iInLen) != 0) {
			return -4;
		}
	}

	return 0;
}

int
cryAesDecrypt(const char *sKey, int iKeyLen,
                const char *sInBuf, int iInLen,
                     char *sOutBuf, int *piOutLen)
{
	unsigned char	sHash[48];
	unsigned char	*pIV = sHash + 32;
	aes_context		tCtx;

	if (iInLen % 16 || *piOutLen < iInLen) {
		return -1;
	}

	*piOutLen = iInLen;

	sha4((const unsigned char *)sKey, iKeyLen, sHash, 1);

	aes_setkey_dec(&tCtx, sHash, 256);

	if (aes_crypt_cbc(&tCtx, DES_DECRYPT, iInLen, pIV,
		              (const unsigned char *)sInBuf,
		              (unsigned char *)sOutBuf) != 0) {
		return -2;
	}

	return 0;
}

char *
cryMd5(const char *sInBuf, int iInLen, char sOutBuf[16])
{
	md5((const unsigned char *)sInBuf, iInLen, (unsigned char *)sOutBuf);

	return sOutBuf;
}

char *
cryMd5File(const char *sFile, char sOutBuf[16])
{
	if (md5_file(sFile, (unsigned char *)sOutBuf) != 0) {
		return NULL;
	}

	return sOutBuf;
}

char *
crySha1(const char *sInBuf, int iInLen, char sOutBuf[20])
{
	sha1((const unsigned char *)sInBuf, iInLen, (unsigned char *)sOutBuf);

	return sOutBuf;
}

char *
crySha1File(const char *sFile, char sOutBuf[20])
{
	if (sha1_file(sFile, (unsigned char *)sOutBuf) != 0) {
		return NULL;
	}

	return sOutBuf;
}

/*------------------------- Self Test ----------------------------*/
int
cryHexSelfTest(int iVerbose)
{
	const char	sOrig[64] = {
	    0x24, 0x48, 0x6E, 0x56, 0x87, 0x62, 0x5A, 0xBD,
	    0xBF, 0x17, 0xD9, 0xA2, 0xC4, 0x17, 0x1A, 0x01,
	    0x94, 0xED, 0x8F, 0x1E, 0x11, 0xB3, 0xD7, 0x09,
	    0x0C, 0xB6, 0xE9, 0x10, 0x6F, 0x22, 0xEE, 0x13,
	    0xCA, 0xB3, 0x07, 0x05, 0x76, 0xC9, 0xFA, 0x31,
	    0x6C, 0x08, 0x34, 0xFF, 0x8D, 0xC2, 0x6C, 0x38,
	    0x00, 0x43, 0xE9, 0x54, 0x97, 0xAF, 0x50, 0x4B,
	    0xD1, 0x41, 0xBA, 0x95, 0x31, 0x5A, 0x0B, 0x97};

	const char	sHex[] =
	    "24486E5687625ABDBF17D9A2C4171A01"
	    "94ED8F1E11B3D7090CB6E9106F22EE13"
	    "CAB3070576C9FA316C0834FF8DC26C38"
	    "0043E95497AF504BD141BA95315A0B97";

	int		iLen;
	char	sBuffer[128+1];

	if (iVerbose) printf("  Hex encoding test: ");

	iLen = sizeof(sBuffer);
	if (cryHexEncode(sOrig, sizeof(sOrig), sBuffer, &iLen) < 0 ||
		memcmp(sHex, sBuffer, 128) != 0 ) {
		if (iVerbose != 0) printf( "failed\n" );
		return -1;
	}

	if (iVerbose != 0) printf( "passed\n  Hex decoding test: " );

	iLen = sizeof(sBuffer);
	if (cryHexDecode(sHex, strlen(sHex), sBuffer, &iLen) < 0 ||
		memcmp(sOrig, sBuffer, 64) != 0 ) {
		if (iVerbose != 0) printf( "failed\n" );
		return -2;
	}

	if (iVerbose != 0) printf( "passed\n\n" );

	return 0;
}

int
cryBase64SelfTest(int iVerbose)
{
	if (base64_self_test(iVerbose) != 0) {
		return -1;
	}

	return 0;
}

int
cryDesSelfTest(int iVerbose)
{
	if (des_self_test(iVerbose) != 0) {
		return -1;
	}

	return 0;
}

int
cryAesSelfTest(int iVerbose)
{
	if (aes_self_test(iVerbose) != 0) {
		return -1;
	}

	return 0;
}

int
cryMdxSelfTest(int iVerbose)
{
	if (md5_self_test(iVerbose) != 0) {
		return -1;
	}

	return 0;
}

int
cryShaxSelfTest(int iVerbose)
{
	if (sha1_self_test(iVerbose) != 0) {
		return -1;
	}

	if (sha2_self_test(iVerbose) != 0) {
		return -2;
	}

	if (sha4_self_test(iVerbose) != 0) {
		return -3;
	}

	return 0;
}

/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

