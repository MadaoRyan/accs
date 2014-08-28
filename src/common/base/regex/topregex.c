/*
 *
 *
 * Regular Expressions Support
 *
 *
 * FileName: topregex.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include	"topregex.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"tre/tre.h"

/*--------------------------- Macro define ------------------------------*/
#define	_TINY_BUF_LEN	32
#define	_SMALL_BUF_LEN	256
#define	_LARGE_BUF_LEN	1024
#define	_HUGE_BUF_LEN	10240

#define	setnull(x)		memset(&(x), 0, sizeof(x))
#define	max(x,y)		((x) > (y) ? (x) : (y))
#define	min(x,y)		((x) < (y) ? (x) : (y))

/*---------------------------- Type define ------------------------------*/
struct T_REGEXP {
	regex_t		tRegex;
	regmatch_t	atMatch[1];
};

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/
static char	f_sErrStr[_LARGE_BUF_LEN];

/*-------------------------  Global functions ---------------------------*/
T_REGEXP *
regCompile(const char *sReg, int iFlags)
{
	T_REGEXP	*ptRegExp, *ptTmp;
	int			iRet, iOpt = REG_EXTENDED;

	ptRegExp = malloc(sizeof(T_REGEXP));
	if (!ptRegExp) {
		strcpy(f_sErrStr, "out of memory");
		return NULL;
	}

	if (iFlags & REG_FLAG_ICASE) {
		iOpt |= REG_ICASE;
	}

	if (iFlags & REG_FLAG_NEWLINE) {
		iOpt |= REG_NEWLINE;
	}

	iRet = tre_regcomp(&ptRegExp->tRegex, sReg, iOpt);
	if (iRet != 0) {
		tre_regerror(iRet, &ptRegExp->tRegex, f_sErrStr, sizeof(f_sErrStr));
		free(ptRegExp);
		return NULL;
	}

	if (ptRegExp->tRegex.re_nsub > 0) {
		ptTmp = realloc(ptRegExp, sizeof(T_REGEXP) +
		                sizeof(regmatch_t) * ptRegExp->tRegex.re_nsub);
		if (!ptTmp) {
			strcpy(f_sErrStr, "out of memory");
			free(ptRegExp);
			return NULL;
		}
		ptRegExp = ptTmp;
	}

	return ptRegExp;
}

int
regFree(T_REGEXP *ptRegExp)
{
	if (!ptRegExp) {
		tre_regfree(&ptRegExp->tRegex);
		free(ptRegExp);
	}

	return 0;
}

int
regExec(T_REGEXP *ptRegExp, const char *sStr)
{
	int	iRet;

	iRet = tre_regexec(&ptRegExp->tRegex, sStr,
	                   ptRegExp->tRegex.re_nsub + 1, ptRegExp->atMatch, 0);
	if (iRet != 0) {
		tre_regerror(iRet, &ptRegExp->tRegex, f_sErrStr, sizeof(f_sErrStr));
		return -1;
	}

	return 0;
}

int
regSubCount(T_REGEXP *ptRegExp)
{
	return ptRegExp->tRegex.re_nsub + 1;
}

int
regSub(T_REGEXP *ptRegExp, int iIndex, int *piLen)
{
	if (iIndex < 0 || iIndex > ptRegExp->tRegex.re_nsub) {
		return -1;
	}

	if (piLen) {
		*piLen = ptRegExp->atMatch[iIndex].rm_eo -
		         ptRegExp->atMatch[iIndex].rm_so;
	}

	return ptRegExp->atMatch[iIndex].rm_so;
}

char *
regSubString(T_REGEXP *ptRegExp, int iIndex, const char *sStr, int *piLen)
{
	int	iRet;

	iRet = regSub(ptRegExp, iIndex, piLen);
	if (iRet < 0) {
		return NULL;
	}

	return ((char *)sStr + iRet);
}

int
regMatch(const char *sReg, const char *sStr)
{
	regex_t	tRegex;
	int		iRet;

	iRet = tre_regcomp(&tRegex, sReg, REG_EXTENDED | REG_NOSUB);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return -2;
	}

	iRet = tre_regexec(&tRegex, sStr, 0, NULL, 0);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return -1;
	}
    tre_regfree(&tRegex);
	return 0;
}

int
regMatchI(const char *sReg, const char *sStr)
{
	regex_t	tRegex;
	int		iRet;

	iRet = tre_regcomp(&tRegex, sReg, REG_EXTENDED | REG_NOSUB | REG_ICASE);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return -2;
	}

	iRet = tre_regexec(&tRegex, sStr, 0, NULL, 0);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return -1;
	}
    tre_regfree(&tRegex);
	return 0;
}

int
regMatchN(const char *sReg, const char *sStr, int iSize)
{
	regex_t	tRegex;
	int		iRet;

	iRet = tre_regcomp(&tRegex, sReg, REG_EXTENDED | REG_NOSUB | REG_NEWLINE);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return -2;
	}

	iRet = tre_regnexec(&tRegex, sStr, iSize, 0, NULL, 0);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return -1;
	}
    tre_regfree(&tRegex);
	return 0;
}

int
regMatchNI(const char *sReg, const char *sStr, int iSize)
{
	regex_t	tRegex;
	int		iRet;

	iRet = tre_regcomp(&tRegex, sReg,
	                   REG_EXTENDED | REG_NOSUB | REG_ICASE | REG_NEWLINE);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return -2;
	}

	iRet = tre_regnexec(&tRegex, sStr, iSize, 0, NULL, 0);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return -1;
	}
    tre_regfree(&tRegex);
	return 0;
}

char *
regSearch(const char *sReg, const char *sStr, int *piLen)
{
	regex_t		tRegex;
	regmatch_t	tMatch;
	int			iRet;

	iRet = tre_regcomp(&tRegex, sReg, REG_EXTENDED);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return NULL;
	}

	iRet = tre_regexec(&tRegex, sStr, 1, &tMatch, 0);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return NULL;
	}

	if (piLen) {
		*piLen = tMatch.rm_eo - tMatch.rm_so;
	}
    tre_regfree(&tRegex);
	return ((char *)sStr + tMatch.rm_so);
}

char *
regSearchI(const char *sReg, const char *sStr, int *piLen)
{
	regex_t		tRegex;
	regmatch_t	tMatch;
	int			iRet;

	iRet = tre_regcomp(&tRegex, sReg, REG_EXTENDED | REG_ICASE);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return NULL;
	}

	iRet = tre_regexec(&tRegex, sStr, 1, &tMatch, 0);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return NULL;
	}

	if (piLen) {
		*piLen = tMatch.rm_eo - tMatch.rm_so;
	}
    tre_regfree(&tRegex);
	return ((char *)sStr + tMatch.rm_so);
}

char *
regSearchN(const char *sReg, const char *sStr, int iSize, int *piLen)
{
	regex_t		tRegex;
	regmatch_t	tMatch;
	int			iRet;

	iRet = tre_regcomp(&tRegex, sReg, REG_EXTENDED | REG_NEWLINE);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return NULL;
	}

	iRet = tre_regnexec(&tRegex, sStr, iSize, 1, &tMatch, 0);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return NULL;
	}

	if (piLen) {
		*piLen = tMatch.rm_eo - tMatch.rm_so;
	}
    tre_regfree(&tRegex);
	return ((char *)sStr + tMatch.rm_so);
}

char *
regSearchNI(const char *sReg, const char *sStr, int iSize, int *piLen)
{
	regex_t		tRegex;
	regmatch_t	tMatch;
	int			iRet;

	iRet = tre_regcomp(&tRegex, sReg, REG_EXTENDED | REG_NEWLINE | REG_ICASE);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return NULL;
	}

	iRet = tre_regnexec(&tRegex, sStr, iSize, 1, &tMatch, 0);
	if (iRet != 0) {
		tre_regerror(iRet, &tRegex, f_sErrStr, sizeof(f_sErrStr));
		return NULL;
	}

	if (piLen) {
		*piLen = tMatch.rm_eo - tMatch.rm_so;
	}
    tre_regfree(&tRegex);
	return ((char *)sStr + tMatch.rm_so);
}

const char *
regErrStr(void)
{
	return f_sErrStr;
}

/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

