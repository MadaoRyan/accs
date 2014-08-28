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

#include	"glb_def.h"

/*--------------------------- Macro define ------------------------------*/
#define	_TINY_BUF_LEN	32
#define	_SMALL_BUF_LEN	256
#define	_LARGE_BUF_LEN	1024
#define	_HUGE_BUF_LEN	10240

#define	setnull(x)		memset(&(x), 0, sizeof(x))
#define	max(x,y)		((x) > (y) ? (x) : (y))
#define	min(x,y)		((x) < (y) ? (x) : (y))

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/
int
cryWrapEncrypt(int iMethod, char *sBuf, int iBufLen)
{
    return cryWrapEncryptKey(iMethod, sBuf, iBufLen, (char *)GLB_KEY);
}

int
cryWrapEncryptKey(int iMethod, char *sBuf, int iBufLen, char *sKey)
{
	char	*pMid;
	int		iMidLen, iRet;

	if (iMethod == CRY_METHOD_NONE) {
		return strlen(sBuf);
	}

	iMidLen = strlen(sBuf) + GLB_KEY_LEN;
	pMid = malloc(iMidLen);
	if (!pMid) {
		return -1;
	}

	switch (iMethod) {
		case CRY_METHOD_3DES: {
			iRet = cryDes3UtilEncrypt(sBuf, strlen(sBuf), pMid, &iMidLen,
			                          sKey);
			if (iRet < 0) {
				free(pMid);
				return -4;
			}
			break;
		}
		case CRY_METHOD_AES: {
			iRet = cryAesEncrypt(sKey, GLB_KEY_LEN,
			                     sBuf, strlen(sBuf), pMid, &iMidLen);
			if (iRet < 0) {
				free(pMid);
				return -5;
			}
			break;
		}
		default: {
			free(pMid);
			return -3;
		}
	}

	iRet = cryBase64Encode(pMid, iMidLen, sBuf, &iBufLen);
	if (iRet < 0) {
		free(pMid);
		return -2;
	}

	free(pMid);
	return iBufLen;
}

int
cryWrapDecrypt(int iMethod, char *sBuf, int iBufLen)
{
    return cryWrapDecryptKey(iMethod, sBuf, iBufLen, (char *)GLB_KEY);
}

int
cryWrapDecryptKey(int iMethod, char *sBuf, int iBufLen, char *sKey)
{
	char	*pMid;
	int		iMidLen, iRet;

	if (iMethod == CRY_METHOD_NONE) {
		return strlen(sBuf);
	}

	iMidLen = strlen(sBuf) / 4 * 3;
	pMid = malloc(iMidLen);
	if (!pMid) {
		return -1;
	}

	iRet = cryBase64Decode(sBuf, -1, pMid, &iMidLen);
	if (iRet < 0) {
		free(pMid);
		return -2;
	}

	iBufLen -= 1;
	switch (iMethod) {
		case CRY_METHOD_3DES: {
			iRet = cryDes3UtilDecrypt(pMid, iMidLen, sBuf, &iBufLen,
			                          sKey);
			if (iRet < 0) {
				free(pMid);
				return -4;
			}
			break;
		}
		case CRY_METHOD_AES: {
			iRet = cryAesDecrypt(sKey, GLB_KEY_LEN,
			                     pMid, iMidLen, sBuf, &iBufLen);
			if (iRet < 0) {
				free(pMid);
				return -5;
			}
			break;
		}
		default: {
			free(pMid);
			return -3;
		}
	}

	sBuf[iBufLen] = '\0';
	free(pMid);
	return iBufLen;
}

/*-------------------------  Global functions ---------------------------*/

/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

