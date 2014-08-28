/*
 *
 *
 * Encrypt and Decrypt Functions
 *
 *
 * FileName: util_des.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */

/*--------------------------- Include files -----------------------------*/
#include	"util_des.h"

#include	<string.h>

#include	"desalgo/desalgo.h"

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/
int	desEncryptDES(unsigned char *sInBuf,  size_t  nInBufLen,
				  unsigned char *sOutBuf, size_t *pnOutBufLen,
				  unsigned char sKey[8]);

int	desDecryptDES(unsigned char *sInBuf,  size_t  nInBufLen,
				  unsigned char *sOutBuf, size_t *pnOutBufLen,
				  unsigned char sKey[8]);

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
/**
 * 对字符串进行DES加密。
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 * @param sKey: 加密密钥（8位）
 *
 * @return 0-成功，1-输出缓冲区太小
 *
 */
int
desEncryptDES(unsigned char *sInBuf,  size_t  nInBufLen,
			  unsigned char *sOutBuf, size_t *pnOutBufLen,
			  unsigned char sKey[8])
{
	DES_KS			desKS;
	unsigned char	sBuf[8];

	if (*pnOutBufLen < (nInBufLen / 8 + (nInBufLen % 8 == 0 ? 0 : 1)) * 8)
		return -1;

	__deskey(desKS, sKey, DES_ENCRYPT);

	*pnOutBufLen = 0;
	while (nInBufLen > 0) {
		if (nInBufLen >= 8)
			memcpy(sBuf, sInBuf + *pnOutBufLen, 8);
		else {
			memcpy(sBuf, sInBuf + *pnOutBufLen, nInBufLen);
			memset(sBuf + nInBufLen, 0, 8 - nInBufLen);
		}

		__des(desKS, sBuf);

		memcpy(sOutBuf + *pnOutBufLen, sBuf, 8);
		*pnOutBufLen += 8;
		nInBufLen = nInBufLen > 8 ? nInBufLen - 8 : 0;
	}

	return 0;
}

/**
 * 对字符串进行DES解密。
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 * @param sKey: 加密密钥（8位）
 *
 * @return 0-成功，1-输出缓冲区太小
 *
 */
int
desDecryptDES(unsigned char *sInBuf,  size_t  nInBufLen,
			  unsigned char *sOutBuf, size_t *pnOutBufLen,
			  unsigned char sKey[8])
{
	DES_KS			desKS;
	unsigned char	sBuf[8];

	if (*pnOutBufLen < (nInBufLen / 8 + (nInBufLen % 8 == 0 ? 0 : 1)) * 8)
		return -1;

	__deskey(desKS, sKey, DES_DECRYPT);

	*pnOutBufLen = 0;
	while (nInBufLen > 0) {
		if (nInBufLen >= 8)
			memcpy(sBuf, sInBuf + *pnOutBufLen, 8);
		else {
			memcpy(sBuf, sInBuf + *pnOutBufLen, nInBufLen);
			memset(sBuf + nInBufLen, 0, 8 - nInBufLen);
		}

		__des(desKS, sBuf);

		memcpy(sOutBuf + *pnOutBufLen, sBuf, 8);
		*pnOutBufLen += 8;
		nInBufLen = nInBufLen > 8 ? nInBufLen - 8 : 0;
	}

	return 0;
}

/**
 * 对字符串进行3DES加密。
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 * @param sKey: 加密密钥（24位）
 *
 * @return 0-成功，1-输出缓冲区太小
 *
 */
int
desEncrypt3DES(unsigned char *sInBuf,  size_t  nInBufLen,
			   unsigned char *sOutBuf, size_t *pnOutBufLen,
			   unsigned char sKey[24])
{
	DES3_KS			desKS;
	unsigned char	sBuf[8];

	if (*pnOutBufLen < (nInBufLen / 8 + (nInBufLen % 8 == 0 ? 0 : 1)) * 8)
		return -1;

	__des3key(desKS, sKey, DES_ENCRYPT);

	*pnOutBufLen = 0;
	while (nInBufLen > 0) {
		if (nInBufLen >= 8)
			memcpy(sBuf, sInBuf + *pnOutBufLen, 8);
		else {
			memcpy(sBuf, sInBuf + *pnOutBufLen, nInBufLen);
			memset(sBuf + nInBufLen, 0, 8 - nInBufLen);
		}

		__des3(desKS, sBuf);

		memcpy(sOutBuf + *pnOutBufLen, sBuf, 8);
		*pnOutBufLen += 8;
		nInBufLen = nInBufLen > 8 ? nInBufLen - 8 : 0;
	}

	return 0;
}

/**
 * 对字符串进行3DES解密。
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 * @param sKey: 加密密钥（24位）
 *
 * @return 0-成功，1-输出缓冲区太小
 *
 */
int
desDecrypt3DES(unsigned char *sInBuf,  size_t  nInBufLen,
			   unsigned char *sOutBuf, size_t *pnOutBufLen,
			   unsigned char sKey[24])
{
	DES3_KS			desKS;
	unsigned char	sBuf[8];

	if (*pnOutBufLen < (nInBufLen / 8 + (nInBufLen % 8 == 0 ? 0 : 1)) * 8)
		return -1;

	__des3key(desKS, sKey, DES_DECRYPT);

	*pnOutBufLen = 0;
	while (nInBufLen > 0) {
		if (nInBufLen >= 8)
			memcpy(sBuf, sInBuf + *pnOutBufLen, 8);
		else {
			memcpy(sBuf, sInBuf + *pnOutBufLen, nInBufLen);
			memset(sBuf + nInBufLen, 0, 8 - nInBufLen);
		}

		__des3(desKS, sBuf);

		memcpy(sOutBuf + *pnOutBufLen, sBuf, 8);
		*pnOutBufLen += 8;
		nInBufLen = nInBufLen > 8 ? nInBufLen - 8 : 0;
	}

	return 0;
}

/* 用于测试 */
/*
#include	<ctype.h>
#include	<stdio.h>
#include	<stdlib.h>

void
printbuf(char *sHead, char *sBuf, size_t nBufLen)
{
	char	sTmpBuf[200+1] = {0, };
	int		i;

	memcpy(sTmpBuf, sBuf, nBufLen >= 200 ? 200 : nBufLen);
	for (i = 0; i < (nBufLen >= 200 ? 200 : nBufLen); i++)
		if (!isprint(sTmpBuf[i])) sTmpBuf[i] = '.';
	printf("%s: [%s]\t\t", sHead, sTmpBuf);
	for (i = 0; i < (nBufLen >= 100 ? 100 : nBufLen); i++)
		printf("[%02X]", sBuf[i] & 0xFF);
	printf("\n");
}

void
testDES(char *sKey, char *sBuf)
{
	unsigned char	sInBuf[100] = {0, };
	size_t			nInBufLenLen = strlen(sBuf);
	unsigned char	sOutbuf[100] = {0, };
	size_t			nOutBufLenLen = 100;
	unsigned char	sKeyBuf[8] = {0, };

	printf("---------------------------------------------------------------\n");
	memcpy(sKeyBuf, sKey, sizeof(sKeyBuf));
	printbuf("    Key", (char *)sKeyBuf, 8);

	memcpy(sInBuf, sBuf, nInBufLenLen >= 100 ? 100 : nInBufLenLen);
	printbuf("   Data", (char *)sInBuf, nInBufLenLen >= 100 ? 100 : nInBufLenLen);

	desEncryptDES(sInBuf, nInBufLenLen, sOutbuf, &nOutBufLenLen, sKeyBuf);
	printbuf("Encrypt", (char *)sOutbuf, nOutBufLenLen);

	memset(sInBuf, 0, sizeof(sInBuf));
	nInBufLenLen = 100;
	desDecryptDES(sOutbuf, nOutBufLenLen, sInBuf, &nInBufLenLen, sKeyBuf);
	printbuf("Decrypt", (char *)sInBuf, nInBufLenLen);
}

void
test3DES(char *sKey, char *sBuf)
{
	unsigned char	sInBuf[100] = {0, };
	size_t			nInBufLenLen = strlen(sBuf);
	unsigned char	sOutbuf[100] = {0, };
	size_t			nOutBufLenLen = 100;
	unsigned char	sKeyBuf[24] = {0, };

	printf("---------------------------------------------------------------\n");
	memcpy(sKeyBuf, sKey, sizeof(sKeyBuf));
	printbuf("    Key", (char *)sKeyBuf, 24);

	memcpy(sInBuf, sBuf, nInBufLenLen >= 100 ? 100 : nInBufLenLen);
	printbuf("   Data", (char *)sInBuf, nInBufLenLen >= 100 ? 100 : nInBufLenLen);

	desEncrypt3DES(sInBuf, nInBufLenLen, sOutbuf, &nOutBufLenLen, sKeyBuf);
	printbuf("Encrypt", (char *)sOutbuf, nOutBufLenLen);

	memset(sInBuf, 0, sizeof(sInBuf));
	nInBufLenLen = 100;
	desDecrypt3DES(sOutbuf, nOutBufLenLen, sInBuf, &nInBufLenLen, sKeyBuf);
	printbuf("Decrypt", (char *)sInBuf, nInBufLenLen);
}

int
main(void)
{
	printf("---------------------------------------------------------------\n");
	printf("testDES\n");

	testDES("12345678", "1234");
	testDES("12345678", "12345678");
	testDES("87654321", "123456789012");
	testDES("87654321", "1234567890123456");

	printf("---------------------------------------------------------------\n");
	printf("test3DES\n");

	test3DES("123456789012345678901234", "1234");
	test3DES("123456789012345678901234", "12345678");
	test3DES("432109876543210987654321", "123456789012");
	test3DES("432109876543210987654321", "1234567890123456");

	exit(EXIT_SUCCESS);
}
*/

/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

