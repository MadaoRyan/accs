/*
 *
 *
 * Encode and Decode Functions
 *
 *
 * FileName: util_enc.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */

/*--------------------------- Include files -----------------------------*/
#include	"util_enc.h"

#include	<stdio.h>
#include	<string.h>

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/
static void	encToBase64(unsigned char sInBuf[3], size_t nInBufLen,
						unsigned char sOutBuf[4]);

static void	encFromBase64(unsigned char sInBuf[4],
						  unsigned char sOutBuf[3], size_t *pnOutBufLen);

/*-------------------------  Global variable ----------------------------*/
static const unsigned char	f_sBase64Code[] =
	{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

/*-------------------------  Global functions ---------------------------*/
/**
 * 对字符串进行Base64编码。
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 *
 * @return 0-成功，1-输出缓冲区太小
 *
 */
int
encEncodeBase64(unsigned char *sInBuf,  size_t  nInBufLen,
				unsigned char *sOutBuf, size_t *pnOutBufLen)
{
	int i;

	if (*pnOutBufLen < (nInBufLen + 2) / 3 * 4)
		return -1;

	for (i = 0; i < nInBufLen; i += 3)
		encToBase64(&sInBuf[i], nInBufLen >= i + 3 ? 3 : nInBufLen % 3,
			&sOutBuf[i / 3 * 4]);
	*pnOutBufLen = i / 3 * 4;

	return 0;
}

/**
 * 对字符串进行Base64解码。
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 *
 * @return 0-成功，1-输出缓冲区太小
 *
 */
int
encDecodeBase64(unsigned char *sInBuf,  size_t  nInBufLen,
				unsigned char *sOutBuf, size_t *pnOutBufLen)
{
	int				i;
	size_t			iLen;
	unsigned char	sBuf[4];

	if (*pnOutBufLen < (nInBufLen + 3) / 4 * 3)
		return -1;

	sBuf[3] = '\0';
	sOutBuf[0] = '\0';

	*pnOutBufLen = 0;

	for (i = 0; i < nInBufLen; i += 4)
	{
		while (sInBuf[i] == '\r' || sInBuf[i] == '\n') {
			i++;
		}
		if (i >= nInBufLen) {
			break;
		}
		encFromBase64(sInBuf + i, sBuf, &iLen);

		memcpy(sOutBuf + (*pnOutBufLen), sBuf, iLen);
		*pnOutBufLen += iLen;
	}

	return 0;
}

/**
 * 对字符串进行HexZero编码。
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 *
 * @return 0-成功，1-输出缓冲区太小
 *
 */
int
encEncodeHexZero(unsigned char *sInBuf,  size_t  nInBufLen,
				 unsigned char *sOutBuf, size_t *pnOutBufLen)
{
	int	i,j;

	int	nSum = 0;
	for (i = 0; i < nInBufLen; i++)
		if (sInBuf[i] == '^' || sInBuf[i] == '\0') nSum++;
	if (*pnOutBufLen < nInBufLen + nSum)
		return -1;

	i = 0; j = 0;
	while (i < nInBufLen)
	{
		if (sInBuf[i] == (unsigned char)0x00)
		{
			sOutBuf[j] = 0x5E;
			j++;
			sOutBuf[j] = 0x31;
		}
		else if (sInBuf[i] == (unsigned char)0x5E)
		{
			sOutBuf[j] = 0x5E;
			j++;
			sOutBuf[j] = 0x32;
		}
		else
			sOutBuf[j] = sInBuf[i];

		i++; j++;
	}

	*pnOutBufLen = j;

	return 0;
}

/**
 * 对字符串进行HexZero解码。
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 *
 * @return 0-成功，1-输出缓冲区太小
 *
 */
int
encDecodeHexZero(unsigned char *sInBuf,  size_t  nInBufLen,
				 unsigned char *sOutBuf, size_t *pnOutBufLen)
{
	int	i,j;

	int	nSum = 0;
	for (i = 0; i < nInBufLen; i++)
		if (sInBuf[i] == '^') nSum++;
	if (*pnOutBufLen < nInBufLen - nSum)
		return -1;

	i = 0; j = 0;
	while (i < nInBufLen)
	{
		if ((sInBuf[i] == (unsigned char)0x5E)
			&& (sInBuf[i+1] == (unsigned char)0x31))
		{
			sOutBuf[j] = 0x00;
			i++;
		}
		else if ((sInBuf[i] == (unsigned char)0x5E)
			&& (sInBuf[i+1] == (unsigned char)0x32))
		{
			sOutBuf[j] = 0x5E;
			i++;
		}
		else
			sOutBuf[j] = sInBuf[i];

		i++; j++;
	}

	*pnOutBufLen = j;

	return 0;
}

/**
 * 对字符串进行Hex16编码。
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 *
 * @return 0-成功，1-输出缓冲区太小
 *
 */
int
encEncodeHex16(unsigned char *sInBuf,  size_t  nInBufLen,
			   unsigned char *sOutBuf, size_t *pnOutBufLen)
{
	int	i;

	if (*pnOutBufLen < nInBufLen * 2)
		return -1;

	for (i = 0; i < nInBufLen; i++) {
		sprintf((char *)sOutBuf, "%02X", *sInBuf++ & 0xFF);
		sOutBuf += 2;
	}

	*pnOutBufLen = nInBufLen * 2;

	return 0;
}

/**
 * 对字符串进行Hex16解码。
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 *
 * @return 0-成功，1-输出缓冲区太小
 *
 */
int
encDecodeHex16(unsigned char *sInBuf,  size_t  nInBufLen,
			   unsigned char *sOutBuf, size_t *pnOutBufLen)
{
	int	i, nChar;

	if (*pnOutBufLen < nInBufLen / 2 || nInBufLen % 2 != 0)
		return -1;

	for (i = 0; i < nInBufLen / 2; i++) {
		sscanf((char *)sInBuf, "%2X", &nChar);
		sInBuf += 2;
		*sOutBuf++ = nChar;
	}

	*pnOutBufLen = nInBufLen / 2;

	return 0;
}

/* 用于测试 */
/*
#include	<ctype.h>
#include	<stdlib.h>

void
testBase64(char *sBuf)
{
	unsigned char	sInBuf[100+1] = {0, };
	size_t			nInBufLenLen = strlen(sBuf);
	unsigned char	sOutbuf[150+1] = {0, };
	size_t			nOutBufLenLen = 150;

	printf("---------------------------------------------------------------\n");

	memcpy(sInBuf, sBuf, nInBufLenLen >= 100 ? 100 : nInBufLenLen);
	printf("  Data: [%s]\n", sInBuf);

	encEncodeBase64(sInBuf, nInBufLenLen, sOutbuf, &nOutBufLenLen);
	printf("Encode: [%s]\n", sOutbuf);

	memset(sInBuf, 0, sizeof(sInBuf));
	nInBufLenLen = 100;
	encDecodeBase64(sOutbuf, nOutBufLenLen, sInBuf, &nInBufLenLen);
	printf("Decode: [%s]\n", sInBuf);
}

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
testHexZero(char *sBuf, size_t nBufLen)
{
	unsigned char	sInBuf[100] = {0, };
	size_t			nInBufLenLen = nBufLen;
	unsigned char	sOutbuf[200] = {0, };
	size_t			nOutBufLenLen = 200;

	printf("---------------------------------------------------------------\n");

	memcpy(sInBuf, sBuf, nInBufLenLen >= 100 ? 100 : nInBufLenLen);
	printbuf("  Data", (char *)sInBuf, nInBufLenLen >= 100 ? 100 : nInBufLenLen);

	encEncodeHexZero(sInBuf, nInBufLenLen, sOutbuf, &nOutBufLenLen);
	printbuf("Encode", (char *)sOutbuf, nOutBufLenLen);

	memset(sInBuf, 0, sizeof(sInBuf));
	nInBufLenLen = 100;
	encDecodeHexZero(sOutbuf, nOutBufLenLen, sInBuf, &nInBufLenLen);
	printbuf("Decode", (char *)sInBuf, nInBufLenLen);
}

void
testHex16(char *sBuf)
{
	unsigned char	sInBuf[100] = {0, };
	size_t			nInBufLenLen = strlen(sBuf);
	unsigned char	sOutbuf[200] = {0, };
	size_t			nOutBufLenLen = 200;

	printf("---------------------------------------------------------------\n");

	memcpy(sInBuf, sBuf, nInBufLenLen >= 100 ? 100 : nInBufLenLen);
	printbuf("  Data", (char *)sInBuf, nInBufLenLen >= 100 ? 100 : nInBufLenLen);

	encEncodeHex16(sInBuf, nInBufLenLen, sOutbuf, &nOutBufLenLen);
	printbuf("Encode", (char *)sOutbuf, nOutBufLenLen);

	memset(sInBuf, 0, sizeof(sInBuf));
	nInBufLenLen = 100;
	encDecodeHex16(sOutbuf, nOutBufLenLen, sInBuf, &nInBufLenLen);
	printbuf("Decode", (char *)sInBuf, nInBufLenLen);
}
*/

/*-------------------------  Local functions ----------------------------*/
/**
 * 对3个字节的字符串进行Base64编码。
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 *
 * @return 无
 *
 */
static void
encToBase64(unsigned char sInBuf[3], size_t nInBufLen,
			unsigned char sOutBuf[4])
{
	sOutBuf[0] = f_sBase64Code[sInBuf[0] >> 2];
	if (nInBufLen == 1)
	{
		sOutBuf[1] = f_sBase64Code[sInBuf[0] << 4 & 0x30];
		sOutBuf[2] = '=';
		sOutBuf[3] = '=';
		return;
	}
	sOutBuf[1] = f_sBase64Code[(sInBuf[0] << 4 & 0x30) | (sInBuf[1] >> 4)];
	if (nInBufLen == 2)
	{
		sOutBuf[2] = f_sBase64Code[sInBuf[1] << 2 & 0x3C];
		sOutBuf[3] = '=';
		return;
	}
	sOutBuf[2] = f_sBase64Code[(sInBuf[1] << 2 & 0x3C) | (sInBuf[2] >> 6)];
	sOutBuf[3] = f_sBase64Code[sInBuf[2] & 0x3F];

	return;
}

/**
 * 对4个字节的字符串进行Base64解码。
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 *
 * @return 无
 *
 */
static void
encFromBase64(unsigned char sInBuf[4],
			  unsigned char sOutBuf[3], size_t *pnOutBufLen)
{
	unsigned char	sBuf[4];
	int				i;

	for (i = 0; i < 4; i++)
	{
		if (sInBuf[i] == '=')
			sBuf[i] = '\0';
		else
			sBuf[i] = (unsigned char*)strchr((char*)f_sBase64Code, sInBuf[i])
					- f_sBase64Code;
	}

	sOutBuf[0] = (sBuf[0] << 2) | ((sBuf[1] & 0x30) >> 4);
	sOutBuf[1] = (sBuf[1] & 0x0f) << 4 | ((sBuf[2] & 0x3c) >> 2);
	sOutBuf[2] = (sBuf[2] << 6) | (sBuf[3] & 0x3f);

	*pnOutBufLen = 3;
	if (sInBuf[2] == '=') (*pnOutBufLen)--;
	if (sInBuf[3] == '=') (*pnOutBufLen)--;

	return;
}

/*-----------------------------  End ------------------------------------*/

