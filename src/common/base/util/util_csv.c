/*
 *
 *
 * CSV Operation Functions
 *
 *
 * FileName: util_csv.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include	"util_csv.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	<stdarg.h>

#include	"util_fil.h"

/*--------------------------- Macro define ------------------------------*/
#define	csvSubmitChar(p,c)                                                    \
do {                                                                          \
	if ((p)->iIndex >= (p)->iSize) {                                          \
		return -11;                                                           \
	}                                                                         \
	(p)->sBuf[(p)->iIndex++] = (c);                                           \
} while (0)

#define	csvSubmitField(p)                                                     \
do {                                                                          \
	if ((p)->iIndex >= (p)->iSize) {                                          \
		return -11;                                                           \
	}                                                                         \
	if (*(p)->piCnt >= (p)->iMax) {                                           \
		return -12;                                                           \
	}                                                                         \
	(p)->sBuf[(p)->iIndex++ - (p)->iSpace] = '\0';                            \
	(p)->psCol[(*(p)->piCnt)++] = (p)->sBuf + (p)->iHead;                     \
	(p)->iHead = (p)->iIndex;                                                 \
	(p)->iSpace = 0;                                                          \
	(p)->eStatus = eFieldBegin;                                               \
} while (0)

#define	csvSubmitRow(p)                                                       \
do {                                                                          \
	if (ptCtx->eStatus != eRowBegin) {                                        \
		csvSubmitField(ptCtx);                                                \
	}                                                                         \
	(p)->eStatus = eRowBegin;                                                 \
	return (p)->eStatus;                                                      \
} while (0)

/*---------------------------- Type define ------------------------------*/
typedef enum {
	eRowBegin,
	eFieldBegin,
	eNormalField,
	eQuoteField,
	eQuote,
	eSpace,
} E_CSV_STATUS;

typedef struct {
	E_CSV_STATUS	eStatus;

	char	*sBuf;
	int		iIndex;
	int		iHead;
	int		iSpace;
	int		iSize;

	int		*piCnt;
	char	**psCol;
	int		iMax;

} T_CSV_SPLIT_CTX;

/*---------------------- Local function declaration ---------------------*/
static char *csvTrimCrLf(char *sBuf);
static int csvSplitN(char *sStr, const char *sSep, int *piCnt, char *psCol[]);

static int csvParseInit(T_CSV_SPLIT_CTX *ptCtx,
                        char *sBuf, int iSize, int *piCnt, char *psCol[]);
static int csvParseLine(T_CSV_SPLIT_CTX *ptCtx,
                        char *sStr, char cSep, char cQuote);

static int csvWriteOne(FILE *fpFile, char cSep, char *sBuf);
static int csvWriteOneExt(FILE *fpFile, char cQuote, char *sBuf);

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
/**
 * csvOpen
 *
 * @param sFileName: CSV文件名
 *
 * @return 成功CSV句柄 失败返回NULL
 *
 */
T_CSV *
csvOpen(char *sFileName)
{
	T_CSV	*ptCsv;

	if ((ptCsv = (T_CSV *) malloc(sizeof(T_CSV))) == NULL)
		return (NULL);

	memset(ptCsv, 0, sizeof(T_CSV));

	if (strcmp(sFileName, "-") == 0) {
		ptCsv->fpFile = stdin;

	} else if ((ptCsv->fpFile = fopen(sFileName, "r")) == NULL) {
		free(ptCsv);
		return (NULL);
	}

	return (ptCsv);
}

/**
 * csvReadRow
 *
 * @param ptCsv: CSV句柄
 * @param cSep, sSep: 字段分隔符
 * @param iCnt: 分割数量
 * @param cSkip: 注释字符，以该字符开头的行将被忽略
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
csvReadRow(T_CSV *ptCsv, char cSep)
{
	int	iRet = csvReadRowN(ptCsv, cSep, CSV_MAX_COL);
	return iRet > 0 ? -11 : iRet;
}

int
csvReadRow2(T_CSV *ptCsv, const char *sSep)
{
	int	iRet = csvReadRowN2(ptCsv, sSep, CSV_MAX_COL);
	return iRet > 0 ? -11 : iRet;
}

int
csvReadRowSkip(T_CSV *ptCsv, char cSep, char cSkip)
{
	int	iRet = csvReadRowSkipN(ptCsv, cSep, cSkip, CSV_MAX_COL);
	return iRet > 0 ? -11 : iRet;
}

int
csvReadRowN(T_CSV *ptCsv, char cSep, int iCnt)
{
	char	sSep[1+1] = {cSep};
	return csvReadRowN2(ptCsv, sSep, iCnt);
}

int
csvReadRowN2(T_CSV *ptCsv, const char *sSep, int iCnt)
{
	int	iRet;

	iRet = filGetLine(ptCsv->sBuf, sizeof(ptCsv->sBuf), ptCsv->fpFile);
	if (iRet == 0) {
		return -1;
	} else if (iRet < 0) {
		return -2;
	}

	csvTrimCrLf(ptCsv->sBuf);

	iRet = csvSplitStrN2(ptCsv->sBuf, sSep, iCnt, (T_SPLIT_STR *)&ptCsv->nCnt);
	if (iRet < 0) {
		return -3;
	}

	return iRet;
}

int
csvReadRowSkipN(T_CSV *ptCsv, char cSep, char cSkip, int iCnt)
{
	int	iRet;

	if (cSep == cSkip) {
		return -4;
	}

	do {
		iRet = filGetLine(ptCsv->sBuf, sizeof(ptCsv->sBuf), ptCsv->fpFile);
		if (iRet == 0) {
			return -1;
		} else if (iRet < 0) {
			return -2;
		}
	} while(ptCsv->sBuf[0] == cSkip);

	csvTrimCrLf(ptCsv->sBuf);

	iRet = csvSplitStrN(ptCsv->sBuf, cSep, iCnt, (T_SPLIT_STR *)&ptCsv->nCnt);
	if (iRet < 0) {
		return -3;
	}

	return 0;
}

int
csvReadRowExt(T_CSV *ptCsv, char cSep, char cQuote)
{
	ptCsv->nCnt = CSV_MAX_COL;
	memset(ptCsv->psCol, 0, sizeof(ptCsv->psCol));

	return csvReadRowExtR(ptCsv->fpFile, cSep, cQuote,
	                      &ptCsv->nCnt, ptCsv->psCol,
	                      ptCsv->sBuf, sizeof(ptCsv->sBuf));
}

int
csvReadRowExtR(FILE *fpFile, char cSep, char cQuote,
               int *piCnt, char *psCol[], char *sBuf, int iSize)
{
	T_CSV_SPLIT_CTX	tCtx;
	char			sLine[CSV_MAX_LINE];
	int				iRet;

	iRet = csvParseInit(&tCtx, sBuf, iSize, piCnt, psCol);
	if (iRet < 0) {
		return -5;
	}

	do {
		iRet = filGetLine(sLine, sizeof(sLine), fpFile);
		if (iRet == 0) {
			return tCtx.eStatus == eRowBegin ? -1 : -4;
		} else if (iRet < -1) {
			return -2;
		} else if (iRet > 0) {
			if (sLine[iRet - 1] != '\n') {
				sLine[iRet] = '\n';
			}
		}

		iRet = csvParseLine(&tCtx, sLine, cSep, cQuote);
		if (iRet < 0) {
			return -3;
		}
	} while (iRet > 0);

	return 0;
}

/**
 * csvClose
 *
 * @param ptCsv: CSV句柄
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
csvClose(T_CSV *ptCsv)
{
	if (ptCsv) {
		if (ptCsv->fpFile != stdin)
			fclose(ptCsv->fpFile);
		free(ptCsv);
	}

	return (0);
}

/**
 * csvSplitStr
 *
 * @param sStr: 欲分割的字符串（会修改该字符串内容）
 * @param cSep, sSep: 字段分隔符
 * @param iCnt: 分割数量
 * @param ptSplitStr: 分割后的结构体
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
csvSplitStr(char *sStr, char cSep, T_SPLIT_STR *ptSplitStr)
{
	int	iRet = csvSplitStrN(sStr, cSep, CSV_MAX_COL, ptSplitStr);
	return iRet > 0 ? -11 : iRet;
}

int
csvSplitStr2(char *sStr, const char *sSep, T_SPLIT_STR *ptSplitStr)
{
	int	iRet = csvSplitStrN2(sStr, sSep, CSV_MAX_COL, ptSplitStr);
	return iRet > 0 ? -11 : iRet;
}

int
csvSplitStrN(char *sStr, char cSep, int iCnt, T_SPLIT_STR *ptSplitStr)
{
	char	sSep[1+1] = {cSep};
	return csvSplitStrN2(sStr, sSep, iCnt, ptSplitStr);
}

int
csvSplitStrN2(char *sStr, const char *sSep, int iCnt, T_SPLIT_STR *ptSplitStr)
{
	if (iCnt > CSV_MAX_COL) {
		return -1;
	}

	ptSplitStr->iCnt = iCnt;
	memset(ptSplitStr->psCol, 0, sizeof(char *) * CSV_MAX_COL);

	return csvSplitN(sStr, sSep, &ptSplitStr->iCnt, ptSplitStr->psCol);
}

/**
 * csvWrite
 *
 * @param fpFile: 目标文件
 * @param cSep: 字段分隔符
 * @param cQuote: 引用字符
 * @param iCnt: 字段数量
 * @param psCol: 字段数组
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
csvWrite(FILE *fpFile, char cSep, int iCnt, ...)
{
	va_list	vaList;
	int		i;

	va_start(vaList, iCnt);

	for (i = 0; i < iCnt; i++) {
		if (i > 0) {
			if (fputc(cSep, fpFile) == EOF) {
				va_end(vaList);
				return -1;
			}
		}

		if (csvWriteOne(fpFile, cSep, va_arg(vaList, char *)) < 0) {
			va_end(vaList);
			return -2;
		}
	}

	if (fputc('\n', fpFile) == EOF) {
		va_end(vaList);
		return -3;
	}

	va_end(vaList);

	return 0;
}

int
csvWriteV(FILE *fpFile, char cSep, int iCnt, char *psCol[])
{
	int	i;

	for (i = 0; i < iCnt; i++) {
		if (i > 0) {
			if (fputc(cSep, fpFile) == EOF) {
				return -1;
			}
		}

		if (csvWriteOne(fpFile, cSep, psCol[i]) < 0) {
			return -2;
		}
	}

	if (fputc('\n', fpFile) == EOF) {
		return -3;
	}

	return 0;
}

int
csvWriteExt(FILE *fpFile, char cSep, char cQuote, int iCnt, ...)
{
	va_list	vaList;
	int		i;

	va_start(vaList, iCnt);

	for (i = 0; i < iCnt; i++) {
		if (i > 0) {
			if (fputc(cSep, fpFile) == EOF) {
				va_end(vaList);
				return -1;
			}
		}

		if (csvWriteOneExt(fpFile, cQuote, va_arg(vaList, char *)) < 0) {
			va_end(vaList);
			return -2;
		}
	}

	if (fputc('\n', fpFile) == EOF) {
		va_end(vaList);
		return -3;
	}

	va_end(vaList);

	return 0;
}

int
csvWriteExtV(FILE *fpFile, char cSep, char cQuote, int iCnt, char *psCol[])
{
	int	i;

	for (i = 0; i < iCnt; i++) {
		if (i > 0) {
			if (fputc(cSep, fpFile) == EOF) {
				return -1;
			}
		}

		if (csvWriteOneExt(fpFile, cQuote, psCol[i]) < 0) {
			return -2;
		}
	}

	if (fputc('\n', fpFile) == EOF) {
		return -3;
	}

	return 0;
}

/*-------------------------  Local functions ----------------------------*/
static char *
csvTrimCrLf(char *sBuf)
{
	char	*pCur;

	pCur = sBuf + strlen(sBuf);
	while (pCur-- >= sBuf) {
		if (*pCur != '\r' && *pCur != '\n')
			break;
	}
	*(pCur + 1) = '\0';

	return sBuf;
}

static int
csvSplitN(char *sStr, const char *sSep, int *piCnt, char *psCol[])
{
	char	*pCur;
	int		iFlag, iMax = *piCnt, iLen = strlen(sSep);

	*piCnt = 0;
	if (iMax <= 0) return 0;

	pCur = sStr - 1;
	iFlag = 1;

	while (pCur < sStr || *pCur != '\0') {
		if (strncmp(pCur, sSep, iLen) == 0) {
			if (pCur >= sStr) {
				*pCur = '\0';
				pCur += iLen - 1;
				iFlag = 1;
			}
		}

		if (iFlag) {
			psCol[(*piCnt)++] = pCur + 1;
			if (*piCnt >= iMax) return 1;
			iFlag = 0;
		}

		pCur++;
	}

	return 0;
}

static int
csvParseInit(T_CSV_SPLIT_CTX *ptCtx,
             char *sBuf, int iSize, int *piCnt, char *psCol[])
{
	memset(ptCtx, 0, sizeof(T_CSV_SPLIT_CTX));

	ptCtx->eStatus = eRowBegin;
	ptCtx->sBuf    = sBuf     ;
	ptCtx->iSize   = iSize    ;
	ptCtx->piCnt   = piCnt    ;
	ptCtx->psCol   = psCol    ;
	ptCtx->iMax    = *piCnt   ;

	*ptCtx->piCnt = 0;

	return 0;
}

static int
csvParseLine(T_CSV_SPLIT_CTX *ptCtx, char *sStr, char cSep, char cQuote)
{
	char	*pCur = sStr - 1;

	if (cSep == cQuote) {
		return -1;
	}

	while (*++pCur != '\0') {
		switch (ptCtx->eStatus) {
			case eRowBegin:
			case eFieldBegin: {
				if (*pCur == '\r' || *pCur == '\n') {
					csvSubmitRow(ptCtx);

				} else if (*pCur == cSep) {
					csvSubmitField(ptCtx);

				} else if (*pCur == cQuote) {
					ptCtx->eStatus = eQuoteField;

				} else {
					ptCtx->eStatus = eNormalField;
					csvSubmitChar(ptCtx, *pCur);
				}
				break;
			}
			case eNormalField: {
				if (*pCur == '\r' || *pCur == '\n') {
					csvSubmitRow(ptCtx);

				} else if (*pCur == cSep) {
					csvSubmitField(ptCtx);

				} else {
					csvSubmitChar(ptCtx, *pCur);
				}
				break;
			}
			case eQuoteField: {
				if (*pCur == cQuote) {
					ptCtx->eStatus = eQuote;

				} else {
					csvSubmitChar(ptCtx, *pCur);
				}
				break;
			}
			case eQuote: {
				if (*pCur == '\r' || *pCur == '\n') {
					csvSubmitRow(ptCtx);

				} else if (*pCur == cSep) {
					csvSubmitField(ptCtx);

				} else if (*pCur == cQuote) {
					ptCtx->eStatus = eQuoteField;
					csvSubmitChar(ptCtx, *pCur);

				} else if (*pCur == ' ' || *pCur == '\t') {
					ptCtx->eStatus = eSpace;
					ptCtx->iSpace++;
					csvSubmitChar(ptCtx, *pCur);

				} else {
					ptCtx->eStatus = eNormalField;
					csvSubmitChar(ptCtx, *pCur);
				}
				break;
			}
			case eSpace: {
				if (*pCur == '\r' || *pCur == '\n') {
					csvSubmitRow(ptCtx);

				} else if (*pCur == cSep) {
					csvSubmitField(ptCtx);

				} else if (*pCur == ' ' || *pCur == '\t') {
					ptCtx->iSpace++;
					csvSubmitChar(ptCtx, *pCur);

				} else {
					ptCtx->iSpace = 0;
					ptCtx->eStatus = eNormalField;
					csvSubmitChar(ptCtx, *pCur);
				}
				break;
			}
			default: {
				return -2;
			}
		}
	}

	return ptCtx->eStatus;
}

static int
csvWriteOne(FILE *fpFile, char cSep, char *sBuf)
{
	char	*pCur = sBuf - 1;

	while (*++pCur != '\0') {
		if (*pCur == cSep || *pCur == '\r' || *pCur == '\n') {
			return -1;
		}

		if (fputc((unsigned char)*pCur, fpFile) == EOF) {
			return -2;
		}
	}

	return 0;
}

static int
csvWriteOneExt(FILE *fpFile, char cQuote, char *sBuf)
{
	char	*pCur = sBuf - 1;

	if (fputc('"', fpFile) == EOF) {
		return -1;
	}

	while (*++pCur != '\0') {
		if (fputc((unsigned char)*pCur, fpFile) == EOF) {
			return -2;
		}

		if (*pCur == cQuote) {
			if (fputc((unsigned char)*pCur, fpFile) == EOF) {
				return -3;
			}
		}
	}

	if (fputc('"', fpFile) == EOF) {
		return -4;
	}

	return 0;
}

/*-----------------------------  End ------------------------------------*/
