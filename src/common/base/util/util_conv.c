/*
 *
 *
 * iconv wrapper functions
 *
 *
 * FileName: util_conv.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include	"util_conv.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	<iconv.h>
#include	<errno.h>

/*--------------------------- Macro define ------------------------------*/
#define	_TINY_BUF_LEN	32
#define	_MINI_BUF_LEN	256
#define	_LARGE_BUF_LEN	1024
#define	_HUGE_BUF_LEN	10240

#define	setnull(t)		memset(&(t), 0, sizeof(t))
#define	setpnull(p)		memset(p, 0, sizeof(*(p)))
#define	max(x,y)		((x) > (y) ? (x) : (y))
#define	min(x,y)		((x) < (y) ? (x) : (y))
#define	arrayof(a)		(sizeof(a) / sizeof(*(a)))

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/
static int locConvSkipWindowsUTF8Head(FILE *fpIn);

/*-------------------------  Global variable ----------------------------*/
static struct {
	int		iUse;
	char	*sFrom;
	char	*sTo;
	iconv_t	iConv;
} f_tConv[CONV_OPEN_MAX];

/*-------------------------  Global functions ---------------------------*/
int
convOpen(char *sFrom, char *sTo)
{
	int	iConv = -1, i;

	for (i = 0; i < arrayof(f_tConv); i++) {
		if (f_tConv[i].iUse) {
			if (strcmp(f_tConv[i].sFrom, sFrom) == 0 &&
				strcmp(f_tConv[i].sTo, sTo) == 0) {
				return i;
			}

		} else {
			if (iConv < 0) iConv = i;
		}
	}

	if (iConv < 0) return -1;

	f_tConv[iConv].sFrom = strdup(sFrom);
	f_tConv[iConv].sTo = strdup(sTo);
	if (!f_tConv[iConv].sFrom || !f_tConv[iConv].sTo) {
		free(f_tConv[iConv].sFrom);
		free(f_tConv[iConv].sTo);
		return -2;
	}

	f_tConv[iConv].iConv = iconv_open(sTo, sFrom);
	if (f_tConv[iConv].iConv == (iconv_t)-1) {
		free(f_tConv[iConv].sFrom);
		free(f_tConv[iConv].sTo);
		return -3;
	}

	f_tConv[iConv].iUse = 1;

	return iConv;
}

int
convClose(int iConv)
{
	if (iConv < 0 || iConv >= arrayof(f_tConv)) return -1;

	if (f_tConv[iConv].iUse) {
		free(f_tConv[iConv].sFrom);
		free(f_tConv[iConv].sTo);
		iconv_close(f_tConv[iConv].iConv);
		f_tConv[iConv].iUse = 0;
	}

	return 0;
}

int
convCloseAll(void)
{
	int	i;

	for (i = 0; i < arrayof(f_tConv); i++) {
		if (f_tConv[i].iUse) convClose(i);
	}

	return 0;
}

int
convTrans(int iConv, char *sIn, int iInLen, char *sOut, int iOutLen)
{
	size_t	iIn, iOut, iRet;

	if (iConv < 0 || iConv >= arrayof(f_tConv) || !f_tConv[iConv].iUse)
		return -1;

	iRet = iconv(f_tConv[iConv].iConv, NULL, NULL, NULL, NULL);
	if (iRet == (size_t)-1) return -4;

	iIn = iInLen < 0 ? strlen(sIn) : iInLen;
	iOut = iOutLen;

	iRet = iconv(f_tConv[iConv].iConv, &sIn, &iIn, &sOut, &iOut);
	if (iRet == (size_t)-1) return -5;

	return iOutLen - iOut;
}

int
convTransform(char *sFrom, char *sTo,
			  char *sIn, int iInLen, char *sOut, int iOutLen)
{
	int	iConv;

	if ((iConv = convOpen(sFrom, sTo)) < 0) return iConv;

	return convTrans(iConv, sIn, iInLen, sOut, iOutLen);
}

int
convFileTransform(char *sFrom, char *sTo, char *sFileIn, char *sFileOut)
{
	FILE	*fpIn, *fpOut;
	size_t	iIn, iOut;
	char	sIn[4], sOut[4];
	char	*pIn, *pOut;
	int		iConv, iRet;

	iConv = convOpen(sFrom, sTo);
	if (iConv < 0) return iConv;

	iRet = iconv(f_tConv[iConv].iConv, NULL, NULL, NULL, NULL);
	if (iRet == (size_t)-1) return -4;

	fpIn = fopen(sFileIn, "r");
	if (!fpIn) {
		return -6;
	}

	fpOut = fopen(sFileOut, "w");
	if (!fpOut) {
		fclose(fpIn);
		return -7;
	}
    
    iRet = locConvSkipWindowsUTF8Head(fpIn);
    if ( iRet != 0 ) {
        fclose(fpOut); fclose(fpIn);
        return -11;
    }

    iIn = 0;
	while (!feof(fpIn)) {
		pIn = sIn;
		iRet = fread(pIn + iIn, 1, sizeof(sIn) - iIn, fpIn);
		if (iRet < 0) {
			fclose(fpOut); fclose(fpIn);
			return -8;
		}
		iIn += iRet;

		pOut = sOut;
		iOut = sizeof(sOut);
		iRet = iconv(f_tConv[iConv].iConv, &pIn, &iIn, &pOut, &iOut);
		if (iRet == (size_t)-1 && errno != E2BIG && errno != EINVAL) {
			fclose(fpOut); fclose(fpIn);
			return -5;
		}

		iRet = fwrite(sOut, 1, sizeof(sOut) - iOut, fpOut);
		if (iRet < 0) {
			fclose(fpOut); fclose(fpIn);
			return -9;
		}

		if (iIn > 0) memmove(sIn, pIn, iIn);
	}

	fclose(fpOut); fclose(fpIn);

	if (iIn > 0) return -10;

	return 0;
}

int
convGBtoUTF8(char *sIn, int iInLen, char *sOut, int iOutLen)
{
	return convTransform(CONV_CODE_GB, CONV_CODE_UTF8,
						 sIn, iInLen, sOut, iOutLen);
}

int
convUTF8toGB(char *sIn, int iInLen, char *sOut, int iOutLen)
{
	return convTransform(CONV_CODE_UTF8, CONV_CODE_GB,
						 sIn, iInLen, sOut, iOutLen);
}

int
convTransformS(char *sFrom, char *sTo,
			   char *sIn, int iInLen, char *sOut, int iOutLen)
{
	int	iRet;

	iRet = convTransform(sFrom, sTo, sIn, iInLen, sOut, iOutLen - 1);
	if (iRet < 0) return iRet;

	sOut[iRet] = '\0';

	return iRet;
}

int
convGBtoUTF8s(char *sIn, int iInLen, char *sOut, int iOutLen)
{
	return convTransformS(CONV_CODE_GB, CONV_CODE_UTF8,
						  sIn, iInLen, sOut, iOutLen);
}

int
convUTF8toGBs(char *sIn, int iInLen, char *sOut, int iOutLen)
{
	return convTransformS(CONV_CODE_UTF8, CONV_CODE_GB,
						  sIn, iInLen, sOut, iOutLen);
}

/*-------------------------  Local functions ----------------------------*/
static int 
locConvSkipWindowsUTF8Head(FILE *fpIn)
{
    int iRet;

    char sCheck[3 + 1];
    memset(sCheck, '\0', sizeof(sCheck));

    /* Goto File Head */
    iRet = fseek(fpIn, 0, SEEK_SET);
    if ( iRet != 0 ) {
        return -1;
    }

    iRet = fread(sCheck, 1, 3, fpIn);
    if (iRet < 0) {
        return -2;
    }

    /* Skip File Head */
    if ( 0xEF == (unsigned char)sCheck[0] && 
         0xBB == (unsigned char)sCheck[1] && 
         0xBF == (unsigned char)sCheck[2] ) {
        return 0;
    }

    /* Reset File to Head */
    iRet = fseek(fpIn, 0, SEEK_SET);
    if ( iRet != 0 ) {
        return -1;
    }

    return 0;
}

/*-----------------------------  End ------------------------------------*/
