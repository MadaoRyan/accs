/*
 *
 *
 * String Functions
 *
 *
 * FileName: util_str.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */

/*--------------------------- Include files -----------------------------*/
#include	"util_str.h"

#include	<ctype.h>
#include	<errno.h>
#include	<stdarg.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

/*--------------------------- Macro define ------------------------------*/
#define	STR_BUFLEN	32	/* �����ڲ����泤�� */
#define	min(x,y)		((x) < (y) ? (x) : (y))

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/
static int	strIsNull(char cChar);

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
/**
 * strRightTrim
 * ȥ���ַ����ұߵĿո�
 *
 * @param sStr: Ҫ�������ַ���
 *
 * @return ������ɵ��ַ���
 *
 */
char *
strRightTrim(char *sStr)
{
    if(sStr==NULL) return sStr;
	char	*p = sStr + strlen(sStr) - 1;

	while (p >= sStr)
		if (*p == ' ' || *p == '\t' /*|| *p == '\r' || *p == '\n'*/)
			p--;
		else
			break;

	*(p + 1) = 0;

	return sStr;
}

/**
 * strLeftTrim
 * ȥ���ַ�����ߵĿո�
 *
 * @param sStr: Ҫ�������ַ���
 *
 * @return ������ɵ��ַ���
 *
 */
char *
strLeftTrim(char *sStr)
{
    if(sStr==NULL) return sStr;
	char	*p = sStr;

	while (*p)
		if (*p == ' ' || *p == '\t' /*|| *p == '\r' || *p == '\n'*/)
			p++;
		else
			break;

	if (p > sStr)
		memmove (sStr, p, strlen(p)+1);

	return sStr;
}

/**
 * strTrim
 * ȥ���ַ����������ߵĿո�
 *
 * @param sStr: Ҫ�������ַ���
 *
 * @return ������ɵ��ַ���
 *
 */
char *
strTrim(char *sStr)
{
	return (strLeftTrim(strRightTrim(sStr)));
}

/**
 * strInt2Str
 * ������ֵת�����ַ�����
 *
 * @param nNum: ��ת��������ֵ
 * @param sBuf: ���������
 * @param nBufLen: �����������
 *
 * @return ����sBuf
 *
 */
char *
strInt2Str(int nNum, char *sBuf, size_t nBufLen)
{
	char	sTmp[STR_BUFLEN];
	int		nLen = nBufLen;

	sprintf(sTmp, "%0*d", nLen, nNum);
	memcpy(sBuf, sTmp, nBufLen);

	return (sBuf);
}

/**
 * strStr2Int
 * ���ַ���ת��������ֵ��
 *
 * @param sBuf: �����ַ���
 * @param nBufLen: �����ַ�����
 *
 * @return ת���ɵ�����ֵ
 *
 */
int
strStr2Int(char *sBuf, size_t nBufLen)
{
	char	sTmp[STR_BUFLEN];

	memset(sTmp, '\0', sizeof(sTmp));
	memcpy(sTmp, sBuf, min(nBufLen, sizeof(sTmp) - 1));

	return (atoi(sTmp));
}

/**
 * strLong2Str
 * ������ֵת�����ַ�����
 *
 * @param nNum: ��ת��������ֵ
 * @param sBuf: ���������
 * @param nBufLen: �����������
 *
 * @return ����sBuf
 *
 */
char *
strLong2Str(long nNum, char *sBuf, size_t nBufLen)
{
	char	sTmp[STR_BUFLEN];
	int		nLen = nBufLen;

	sprintf(sTmp, "%0*ld", nLen, nNum);
	memcpy(sBuf, sTmp, nBufLen);

	return (sBuf);
}

/**
 * strStr2Long
 * ���ַ���ת��������ֵ��
 *
 * @param sBuf: �����ַ���
 * @param nBufLen: �����ַ�����
 *
 * @return ת���ɵ�����ֵ
 *
 */
long
strStr2Long(char *sBuf, size_t nBufLen)
{
	char	sTmp[STR_BUFLEN];

	memset(sTmp, '\0', sizeof(sTmp));
	memcpy(sTmp, sBuf, min(nBufLen, sizeof(sTmp) - 1));

	return (atol(sTmp));
}

/**
 * strLL2Str
 * ������ֵת�����ַ�����
 *
 * @param nNum: ��ת��������ֵ
 * @param sBuf: ���������
 * @param nBufLen: �����������
 *
 * @return ����sBuf
 *
 */
char *
strLL2Str(long long nNum, char *sBuf, size_t nBufLen)
{
	char	sTmp[STR_BUFLEN];
	int		nLen = nBufLen;

	sprintf(sTmp, "%0*lld", nLen, nNum);
	memcpy(sBuf, sTmp, nBufLen);

	return (sBuf);
}

/**
 * strStr2LL
 * ���ַ���ת��������ֵ��
 *
 * @param sBuf: �����ַ���
 * @param nBufLen: �����ַ�����
 *
 * @return ת���ɵ�����ֵ
 *
 */
long long
strStr2LL(char *sBuf, size_t nBufLen)
{
	char	sTmp[STR_BUFLEN];

	memset(sTmp, '\0', sizeof(sTmp));
	memcpy(sTmp, sBuf, min(nBufLen, sizeof(sTmp) - 1));

	return (atoll(sTmp));
}

/**
 * strDbl2Str
 * ������ֵת�����ַ�����
 *
 * @param dNum: ��ת���ĸ���ֵ
 * @param sBuf: ���������
 * @param nBufLen: �����������
 * @param nDot: С��λ��
 *
 * (16.0 , 5, 0) -> 00016
 * (16.12, 5, 2) -> 01612
 *
 * @return ����sBuf
 *
 */
char *
strDbl2Str(double dNum, char *sBuf, size_t nBufLen, int nDot)
{
	char	sTmp[STR_BUFLEN];
	int		nLen = nBufLen, i;

	if (nDot > 0)
		for (i = 0; i < nDot; i++)
			dNum *= 10;
	else
		for (i = 0; i > nDot; i--)
			dNum /= 10;

	sprintf(sTmp, "%0*.0f", nLen, dNum);
	memcpy(sBuf, sTmp, nBufLen);

	return (sBuf);
}

/**
 * strDbl2StrRaw
 * ������ֵת�����ַ�����
 *
 * @param dNum: ��ת���ĸ���ֵ
 * @param sBuf: ���������
 * @param nBufLen: �����������
 * @param nDot: С��λ��
 *
 * (16.0, 0) -> "16"
 * (16.12, 2) -> "0.16"
 * (1600, 2) -> "16.00"
 *
 * @return ����sBuf
 *
 */
char *
strDbl2StrRaw(double dNum, char *sBuf, size_t nBufLen, int nDot)
{
	char	sTmp[STR_BUFLEN];
	int		i;

	if (nDot > 0)
		for (i = 0; i < nDot; i++)
			dNum /= 10;
	else
		for (i = 0; i > nDot; i--)
			dNum *= 10;

	i = sprintf(sTmp, "%.*f", nDot, dNum);
	memcpy(sBuf, sTmp, i < nBufLen ? i : nBufLen);

	return (sBuf);
}

/**
 * strStr2Dbl
 * ���ַ���ת���ɸ���ֵ��
 *
 * @param sBuf: ���������
 * @param nBufLen: �����������
 * @param nDot: С��λ��
 *
 * @return ת���ɵĸ���ֵ
 *
 */
double
strStr2Dbl(char *sBuf, size_t nBufLen, int nDot)
{
	char	sTmp[STR_BUFLEN];
	double	dTmp;
	int		i;

	memset(sTmp, '\0', sizeof(sTmp));
	memcpy(sTmp, sBuf, nBufLen);

	dTmp = atof(sTmp);
	if (nDot > 0)
		for (i = 0; i < nDot; i++)
			dTmp /= 10;
	else
		for (i = 0; i > nDot; i--)
			dTmp *= 10;

	return (dTmp);
}

/**
 * strStr2DblRaw
 * ���ַ���ת���ɸ���ֵ��
 *
 * @param sBuf: ���������
 * @param nBufLen: �����������
 *
 * ("16.0" , 0) -> 16.0
 * ("16.12", 2) -> 1612.0
 * ("1600", -2) -> 16.0
 *
 * @return ת���ɵĸ���ֵ
 *
 */
double
strStr2DblRaw(char *sBuf, size_t nBufLen, int nDot)
{
	char	sTmp[STR_BUFLEN];
	double	dNum;
	int		i;

	memset(sTmp, '\0', sizeof(sTmp));
	memcpy(sTmp, sBuf, nBufLen);

	dNum = atof(sTmp);
	if (nDot > 0)
		for (i = 0; i < nDot; i++)
			dNum *= 10;
	else
		for (i = 0; i > nDot; i--)
			dNum /= 10;

	return dNum;
}

/**
 * strMem2Str
 * �����ַ���
 *
 * @param sDest: Ŀ�껺����
 * @param iDestSize: Ŀ�껺������С
 * @param sSrc: Դ����
 * @param iSrcLen: Դ���泤��
 *
 * @return ������ɵ�·��
 *
 */
char *
strMem2Str(char *sDest, int iDestSize, char *sSrc, int iSrcLen)
{
	if (iSrcLen < 0) iSrcLen = strlen(sSrc);
	int iLen = min(iDestSize - 1, iSrcLen);
	if (iLen >= 0) {
		memcpy(sDest, sSrc, iLen);
		sDest[iLen] = '\0';
	}
	return sDest;
}

/**
 * strToLower
 * ���ַ����еĴ�д��ĸת��ΪСд��
 *
 * @param sStr: Ҫ�������ַ���
 *
 * @return ������ɵ��ַ���
 *
 */
char *
strToLower(char *sStr)
{
	unsigned char	*p = (unsigned char *)sStr;

	while (*p != '\0') {
		*p = tolower(*p);
		p++;
	}

	return (sStr);
}

/**
 * strToUpper
 * ���ַ����е�Сд��ĸת��Ϊ��д��
 *
 * @param sStr: Ҫ�������ַ���
 *
 * @return ������ɵ��ַ���
 *
 */
char *
strToUpper(char *sStr)
{
	unsigned char	*p = (unsigned char *)sStr;

	while (*p != '\0') {
		*p = toupper(*p);
		p++;
	}

	return (sStr);
}

/**
 * strFirstUpper
 * ���ַ�������λת��Ϊ��д������ת��ΪСд��
 *
 * @param sStr: Ҫ�������ַ���
 *
 * @return ������ɵ��ַ���
 *
 */
char *
strFirstUpper(char *sStr)
{
	strToLower(sStr);
	sStr[0] = toupper(sStr[0]);

	return (sStr);
}

/**
 * strStrChr
 * ��sBuf��ʼ��[��]����sBuf��
 * ����pIsMatchʱ���ظõ�ַ��������pIsEndʱ����NULL��
 * ������(pIsMatch && pIsEnd)ΪTRUE����ú���ʧЧ��δ֪�����
 *
 * @param sBuf: ����������
 * @param pIsMatch: ƥ�亯����ƥ��ɹ�ʱ���ط�0��ƥ��ʧ��ʱ����0��
 *                  ��ΪNULL���򷵻�sBuf��
 * @param pIsEnd: ��βƥ�亯��������βʱ���ط�0��δ����βʱ����0��
 *                ��ΪNULL����Ĭ��Ϊ����'\0'ʱ������
 *
 * @return >0:�ɹ�
 *       NULL:ʧ��
 *
 */
char *
strStrChr(char *sBuf, int (*pIsMatch)(char), int (*pIsEnd)(char))
{
	char	*p = sBuf;

	if (pIsMatch == NULL) return sBuf;
	if (pIsEnd   == NULL) pIsEnd = strIsNull;

	while (!pIsEnd(*p)) {
		if (pIsMatch(*p))
			break;
		p++;
	}

	if (pIsEnd(*p))
		return NULL;

	return p;
}

/**
 * strStrNStr
 * ���ַ����в���Ŀ���ַ���
 *
 * @param sBuf: ԭ�ַ���
 * @param iLen: ԭ�ַ�����
 * @param sTarget: Ŀ���ַ���
 *
 * @return �������
 *
 */
char *
strStrNStr(char *sBuf, int iLen, char *sTarget)
{
	int	iTarget, i;

	if (iLen < 0) return strstr(sBuf, sTarget);

	iTarget = strlen(sTarget);
	iLen -= iTarget;

	for (i = 0; i < iLen; i++) {
		if (memcmp(sBuf + i, sTarget, iTarget) == 0) {
			return sBuf + i;
		}
	}

	return NULL;
}

/**
 * strDirName
 * ��ȡ·���е�Ŀ¼����
 *
 * @param sPath: Ҫ������·��
 *
 * @return ������ɵ�·��
 *
 */
char *
strDirName(char *sPath)
{
	static char	sDot[] = ".";
	int			iFlag = 0;
	char		*p;

	if (sPath == NULL) return sDot;

	if ((p = strrchr(sPath, '/')) == NULL) {
		if (strlen(sPath) > 1) {
			sPath[0] = '.';
			sPath[1] = '\0';
			return sPath;

		} else
			return sDot;
	}

	if (*(p + 1) == '\0') iFlag = 1;
	while (p > sPath && *(p - 1) == '/') p--;

	if (p == sPath) {
		*(p + 1) = '\0';
		return sPath;
	}

	*p = '\0';

	if (iFlag) {
		if ((p = strrchr(sPath, '/')) == NULL) {
			sPath[0] = '.';
			sPath[1] = '\0';
			return sPath;
		}

		while (p > sPath && *(p - 1) == '/') p--;

		if (p == sPath) {
			*(p + 1) = '\0';
			return sPath;
		}

		*p = '\0';
	}

	return sPath;
}

/**
 * strBaseName
 * ��ȡ·���е�Ŀ¼����
 *
 * @param sPath: Ҫ������·��
 *
 * @return ������ɵ�·��
 *
 */
char *
strBaseName(char *sPath)
{
	char	*p = strrchr(sPath, '/');

	return p ? p + 1 : sPath;
}

/**
 * strPathJoin
 * ����·������ǰ׺��
 *
 * @param sPrefix: ·��ǰ׺
 * @param sPath: Ҫ������·��
 *
 * @return sPath
 *
 */
char *
strPathJoin(char *sPrefix, char *sPath)
{
	int	iLen;

	if (sPrefix[0] != '\0') {
		if (sPath[0] == '\0') {
			strcpy(sPath, sPrefix);

		} else if (sPath[0] != '/') {
			iLen = strlen(sPrefix);
			if (sPrefix[iLen - 1] == '/') {
				iLen--;
			}
			memmove(sPath + iLen + 1, sPath, strlen(sPath) + 1);
			memcpy(sPath, sPrefix, iLen);
			sPath[iLen] = '/';
		}
	}

	return sPath;
}

/**
 * strPathJoin2
 * ����·�������׺��
 *
 * @param sPath: Ҫ������·��
 * @param sAppend: ·����׺
 *
 * @return sPath
 *
 */
char *
strPathJoin2(char *sPath, char *sAppend)
{
	int	iLen;

	if (sAppend[0] == '/') {
		strcpy(sPath, sAppend);

	} else {
		iLen = strlen(sPath);
		if (iLen > 0 && sPath[iLen - 1] != '/') {
			sPath[iLen] = '/';
			sPath[iLen + 1] = '\0';
		}

		strcat(sPath, sAppend);
	}

	return sPath;
}

/**
 * strReplaceDir
 * �滻Ŀ¼����
 *
 * @param sPath: Ҫ������·��
 * @param sDir: �µ�Ŀ¼·��
 *
 * @return sPath
 *
 */
char *
strReplaceDir(char *sPath, char *sDir)
{
	return (strPathJoin(sDir, strBaseName(sPath)));
}

/**
 * strLength
 * ��ȡ�ַ�������
 *
 * @param sStr: �ַ���
 * @param iMax: ��󳤶�
 *
 * @return ʵ�ʳ���
 *
 */
int
strLength(const char *sStr, int iMax)
{
	int	i;

	if (iMax < 0) {
		return strlen(sStr);
	}

	for (i = 0; i < iMax; i++) {
		if (sStr[i] == '\0') break;
	}
	return i;
}

/**
 * strErrno
 * ��ʾ��ǰerrno��Ӧ�Ĵ�����Ϣ
 *
 * @return ������Ϣ
 *
 */
char *
strErrno(void)
{
	return strerror(errno);
}

/**
 * strAprintf
 * ��ӡ���·�����ڴ�����
 *
 * @return !=0:�ɹ�
 *        NULL:ʧ��
 *
 */
char *
strAprintf(const char *sFmt, ...)
{
	va_list	vaArgs;
	int		iSize;
	char	*pBuf;

	va_start(vaArgs, sFmt);
	iSize = vsnprintf(NULL, 0, sFmt, vaArgs);
	va_end(vaArgs);

	pBuf = malloc(iSize + 1);
	if (!pBuf) {
		return NULL;
	}

	va_start(vaArgs, sFmt);
	iSize = vsprintf(pBuf, sFmt, vaArgs);
	va_end(vaArgs);

	return pBuf;
}

/**
 * strEnv
 * ��û������������������򷵻ؿ��ַ���
 *
 * @return ��������
 *
 */
const char *
strEnv(const char *sEnv)
{
	const char	*pEnv = getenv(sEnv);
	return pEnv ? pEnv : "";
}

/* ���ڲ��� */
/*
int
main(void)
{
	char	a[] = "1234567890";
	char	b[] = "bbbbbbbbbb";
	char	c[] = "cccccccccc";
	char	d[] = " \t\r\nabcd \t\r\n";
	char	e[] = " \t\r\nabcd \t\r\n";
	char	f[] = " \t\r\nabcd \t\r\n";
	char	g[] = "abcd1234ABCD";
	char	h[] = "abcd1234ABCD";
	char	i[] = "abcd1234ABCD";

	printf("a=[%d]\n", strStr2Int(a, 5));
	printf("b=[%s]\n", strInt2Str(123, b, 5));
	printf("a=[%f]\n", strStr2Dbl(a, 5, 2));
	printf("a=[%f]\n", strStr2Dbl(a, 5, 0));
	printf("a=[%f]\n", strStr2Dbl(a, 5, -2));
	printf("c=[%s]\n", strDbl2Str(123.456, c, 5, 2));
	printf("c=[%s]\n", strDbl2Str(123.456, c, 5, 0));
	printf("c=[%s]\n", strDbl2Str(123.456, c, 5, -2));
	printf("d=[%s]\n", strRightTrim(d));
	printf("e=[%s]\n", strLeftTrim(e));
	printf("f=[%s]\n", strTrim(f));
	printf("g=[%s]\n", strToLower(g));
	printf("h=[%s]\n", strToUpper(h));

	int		isA(char c) { return (c == 'A'); }
	int		is4(char c) { return (c == '4'); }
	int		isA4(char c) { return (c == 'A' || c == '4'); }

	printf("strStrChr=[%.1s]\n", strStrChr(i, NULL, NULL));
	printf("strStrChr=[%.1s]\n", strStrChr(i, isA, NULL));
	printf("strStrChr=[%.1s]\n", strStrChr(i, isA, is4));
	printf("strStrChr=[%.1s]\n", strStrChr(i, isA4, NULL));
	printf("strStrChr=[%.1s]\n", strStrChr(i, isA4, isA));

	char	j1[] = "/usr/lib/";
	char	j2[] = "/usr/lib" ;
	char	j3[] = "/usr/"    ;
	char	j4[] = "/usr"     ;
	char	j5[] = "usr/"     ;
	char	j6[] = "usr"      ;
	char	j7[] = "/"        ;
	char	j8[] = "."        ;
	char	j9[] = ".."       ;

	printf("j1[%s] --> ", j1);
	printf("dirname(j1)[%s]\n", strDirName(j1));
	printf("j2[%s] --> ", j2);
	printf("dirname(j2)[%s]\n", strDirName(j2));
	printf("j3[%s] --> ", j3);
	printf("dirnamej(03)[%s]\n", strDirName(j3));
	printf("j4[%s] --> ", j4);
	printf("dirname(j4)[%s]\n", strDirName(j4));
	printf("j5[%s] --> ", j5);
	printf("dirname(j5)[%s]\n", strDirName(j5));
	printf("j6[%s] --> ", j6);
	printf("dirname(j6)[%s]\n", strDirName(j6));
	printf("j7[%s] --> ", j7);
	printf("dirname(j7)[%s]\n", strDirName(j7));
	printf("j8[%s] --> ", j8);
	printf("dirname(j8)[%s]\n", strDirName(j8));
	printf("j9[%s] --> ", j9);
	printf("dirname(j9)[%s]\n", strDirName(j9));

	exit(EXIT_SUCCESS);
}
*/

/*-------------------------  Local functions ----------------------------*/
static int
strIsNull(char cChar)
{
	return (cChar == '\0');
}

/*-----------------------------  End ------------------------------------*/

