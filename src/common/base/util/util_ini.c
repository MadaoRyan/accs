/*
 *
 *
 * INI Operation Functions
 *
 *
 * FileName: util_ini.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */

/*--------------------------- Include files -----------------------------*/
#include	"util_ini.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>

#include	"util_fil.h"
#include	"util_str.h"

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/
typedef enum {
	eNull,
	eSection,
	eKeyValue
} E_INI_TYPE;

/*---------------------- Local function declaration ---------------------*/
static int			iniIsLeft(char cChar);
static int			iniIsRight(char cChar);
static int			iniIsEqual(char cChar);
static int			iniIsNull(char cChar);
static int			iniIsEnd(char cChar);
static E_INI_TYPE	iniGetType(char *sLine);
static int			iniGetSection(FILE *fp, char *sSection);
static int			iniGetKeyValue(FILE *fp, char *sKey,
								   char *sValue, int iValueLen);

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
/**
 * iniReadString
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sDefault: 默认值
 * @param sOutBuf: 输出缓冲区
 * @param nInBufLen: 输出缓冲区长度
 * @param sFile: INI文件名
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
iniReadString(char *sSection, char *sKey, char *sDefault,
			  char *sOutBuf, size_t iInBufLen, char *sFile)
{
	int		iRet, iGoBack = 0;
	FILE	*fp = NULL;

	fp = fopen(sFile, "r");
	if (fp == NULL) {
		if (sDefault != NULL) {
			strncpy(sOutBuf, sDefault, iInBufLen - 1);
			sOutBuf[iInBufLen - 1] = '\0';
		}
		iGoBack = -1;
		goto back;
	}

	iRet = iniGetSection(fp, sSection);
	if (iRet < 0) {
		if (sDefault != NULL) {
			strncpy(sOutBuf, sDefault, iInBufLen - 1);
			sOutBuf[iInBufLen - 1] = '\0';
			iGoBack = -iRet + 10;
		} else {
			iGoBack = -2;
		}
		goto back;
	}

	iRet = iniGetKeyValue(fp, sKey, sOutBuf, iInBufLen);
	if (iRet < 0) {
		if (sDefault != NULL) {
			strncpy(sOutBuf, sDefault, iInBufLen - 1);
			sOutBuf[iInBufLen - 1] = '\0';
			iGoBack = -iRet + 20;
		} else {
			iGoBack = -3;
		}
		goto back;
	}

back:
	if (fp != NULL)
		fclose(fp);
	return iGoBack;
}

/**
 * iniReadInt
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sDefault: 默认值
 * @param sFile: INI文件名
 *
 * @return 键值
 *
 */
int
iniReadInt(char *sSection, char *sKey, int iDefault, char *sFile)
{
	char	sBuf[INI_MAX_LINE];

	return (iniReadString(sSection, sKey, "", sBuf, INI_MAX_LINE, sFile) == 0
			? atoi(sBuf) : iDefault);
}

/**
 * iniReadString2
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sDefault: 默认值
 * @param sOutBuf: 输出缓冲区
 * @param nInBufLen: 输出缓冲区长度
 * @param sFile: INI文件名
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
iniReadString2(char *sSection, char *sKey, char *sDefault,
			   char *sOutBuf, size_t iInBufLen, char *sFile)
{
	int		iRet, iRet2, iFlag = 0;
	char	sKeyFile[INI_MAX_LINE];
	char	sKeySection[INI_MAX_LINE];
	char	sOthFile[INI_MAX_LINE];
	char	sOthSection[INI_MAX_LINE];

	iRet = iniReadString(sSection, sKey, sDefault, sOutBuf, iInBufLen, sFile);
	if (iRet != 0) {
		sprintf(sKeyFile, "%s::file", sKey);
		iRet2 = iniReadString(sSection, sKeyFile, sFile,
							  sOthFile, sizeof(sOthFile), sFile);
		if (iRet2 < 0) {
			return -1;
		} else if (iRet2 == 0) {
			iFlag = 1;
		}

		sprintf(sKeySection, "%s::section", sKey);
		iRet2 = iniReadString(sSection, sKeySection, sSection,
							  sOthSection, sizeof(sOthSection), sFile);
		if (iRet2 < 0) {
			return -2;
		} else if (iRet2 == 0) {
			iFlag = 1;
		}

		if (iFlag) {
			iRet2 = iniReadString(sOthSection, sKey, NULL, sOutBuf, iInBufLen,
								  sOthFile);
			if (iRet2 < 0) {
				if (iRet < 0) {
					return -3;
				}
			} else {
				if (iRet < 0) {
					iRet = 0;
				}
			}
		}
	}

	return iRet;
}

/**
 * iniReadInt2
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sDefault: 默认值
 * @param sFile: INI文件名
 *
 * @return 键值
 *
 */
int
iniReadInt2(char *sSection, char *sKey, int iDefault, char *sFile)
{
	char	sBuf[INI_MAX_LINE];

	return (iniReadString2(sSection, sKey, NULL, sBuf, INI_MAX_LINE, sFile) == 0
			? atoi(sBuf) : iDefault);
}

/**
 * iniWriteString
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sInBuf: 欲写入值
 * @param sFile: INI文件名
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
iniWriteString(char *sSection, char *sKey, char *sInBuf, char *sFile)
{
	int		iRet, iGoBack = 0;
	FILE	*fp = NULL;
	char	sSectionBuf[INI_MAX_LINE];
	char	sKeyBuf[INI_MAX_LINE];
	char	sValueBuf[INI_MAX_LINE];

	FILE	*fpTmp = NULL;
	char	sTmpFile[INI_MAX_LINE];
	long	iPos;
	int		cChar, iLen;

	fp = fopen(sFile, "r");
	if (fp == NULL) {
		iGoBack = -1;
		goto back;
	}

	iRet = iniGetSection(fp, sSection);
	if (iRet < 0) {
		if (iRet == -1) {	/* 未找到段名时 */
			fclose(fp);
			fp = NULL;

			fp = fopen(sFile, "a");
			if (fp == NULL) {
				iGoBack = -2;
				goto back;
			}

			strncpy(sSectionBuf, sSection, INI_MAX_LINE - 1);
			sSectionBuf[INI_MAX_LINE - 1] = '\0';
			strToLower(strTrim(sSectionBuf));

			strncpy(sKeyBuf, sKey, INI_MAX_LINE - 1);
			sKeyBuf[INI_MAX_LINE - 1] = '\0';
			strToLower(strTrim(sKeyBuf));

			strncpy(sValueBuf, sInBuf, INI_MAX_LINE - 1);
			sValueBuf[INI_MAX_LINE - 1] = '\0';
			strTrim(sValueBuf);

			fprintf(fp, "[%s]\n%s=%s\n", sSectionBuf, sKeyBuf, sValueBuf);

			iGoBack = 0;
			goto back;

		} else {
			iGoBack = iRet - 20;
			goto back;
		}
	}

	iRet = iniGetKeyValue(fp, sKey, NULL, 0);
	if (iRet < 0) {
		if (iRet == -1) {	/* 未找到键名时 */
			sprintf(sTmpFile, "%s.%d", sFile, getpid());

			fpTmp = fopen(sTmpFile, "w");
			if (fpTmp == NULL) {
				iGoBack = -3;
				goto back;
			}

			iPos = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			iLen = 0;
			while ((cChar = fgetc(fp)) != EOF) {
				iLen++;
				if (fputc(cChar, fpTmp) == EOF) {
					iGoBack = -4;
					goto back;
				}
				if (iLen >= iPos)
					break;
			}
			if (ferror(fp)) {
				iGoBack = -5;
				goto back;
			}

			strncpy(sKeyBuf, sKey, INI_MAX_LINE - 1);
			sKeyBuf[INI_MAX_LINE - 1] = '\0';
			strToLower(strTrim(sKeyBuf));

			strncpy(sValueBuf, sInBuf, INI_MAX_LINE - 1);
			sValueBuf[INI_MAX_LINE - 1] = '\0';
			strTrim(sValueBuf);

			fprintf(fpTmp, "%s=%s\n", sKeyBuf, sValueBuf);

			while ((cChar = fgetc(fp)) != EOF) {
				if (fputc(cChar, fpTmp) == EOF) {
					iGoBack = -6;
					goto back;
				}
			}
			if (ferror(fp)) {
				iGoBack = -7;
				goto back;
			}

			iRet = rename(sTmpFile, sFile);
			if (iRet < 0) {
				iGoBack = -8;
				goto back;
			}

		} else {
			iGoBack = iRet - 30;
			goto back;
		}

	} else {	/* 找到键名时 */
		sprintf(sTmpFile, "%s.%d", sFile, getpid());

		fpTmp = fopen(sTmpFile, "w");
		if (fpTmp == NULL) {
			iGoBack = -9;
			goto back;
		}

		iPos = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		iLen = 0;
		while ((cChar = fgetc(fp)) != EOF) {
			iLen++;
			if (fputc(cChar, fpTmp) == EOF) {
				iGoBack = -10;
				goto back;
			}
			if (iLen >= iPos)
				break;
		}
		if (ferror(fp)) {
			iGoBack = -11;
			goto back;
		}

		strncpy(sKeyBuf, sKey, INI_MAX_LINE - 1);
		sKeyBuf[INI_MAX_LINE - 1] = '\0';
		strToLower(strTrim(sKeyBuf));

		strncpy(sValueBuf, sInBuf, INI_MAX_LINE - 1);
		sValueBuf[INI_MAX_LINE - 1] = '\0';
		strTrim(sValueBuf);

		fprintf(fpTmp, "%s=%s\n", sKeyBuf, sValueBuf);

		fseek(fp, iRet, SEEK_CUR);
		while ((cChar = fgetc(fp)) != EOF) {
			if (fputc(cChar, fpTmp) == EOF) {
				iGoBack = -12;
				goto back;
			}
		}
		if (ferror(fp)) {
			iGoBack = -13;
			goto back;
		}

		iRet = rename(sTmpFile, sFile);
		if (iRet < 0) {
			iGoBack = -14;
			goto back;
		}
	}

back:
	if (fp != NULL)
		fclose(fp);
	if (fpTmp != NULL)
		fclose(fpTmp);
	return iGoBack;
}

/*-------------------------  Local functions ----------------------------*/
static int
iniIsLeft(char cChar)
{
	return (cChar == '[');
}

static int
iniIsRight(char cChar)
{
	return (cChar == ']');
}

static int
iniIsEqual(char cChar)
{
	return (cChar == '=');
}

static int
iniIsNull(char cChar)
{
	return (cChar == '\0' || cChar == '\r' || cChar == '\n' || cChar == ';'
			|| cChar == '#');
}

static int
iniIsEnd(char cChar)
{
	return (cChar == '\0' || cChar == '\r' || cChar == '\n');
}

static E_INI_TYPE
iniGetType(char *sLine)
{
	strLeftTrim(sLine);

	if (iniIsLeft(sLine[0]))
		return eSection;

	else if (iniIsNull(sLine[0]))
		return eNull;

	else
		return eKeyValue;
}

static int
iniGetSection(FILE *fp, char *sSection)
{
	char	sLine[INI_MAX_LINE];
	char	sBuf[INI_MAX_LINE];
	int		iLineLen;
	int		iType;
	char	*p;

	while (1) {
		iLineLen = filGetLine(sLine, sizeof(sLine), fp);
		if (iLineLen == 0) {
			return -1;
		} else if (iLineLen < 0) {
			return -2;
		}

		iType = iniGetType(sLine);
		if (iType == eSection) {
			p = strStrChr(sLine + 1, iniIsRight, iniIsEnd);
			if (p == NULL)
				return -3;
			*p = '\0';
			strToLower(strTrim(sLine + 1));

			strncpy(sBuf, sSection, INI_MAX_LINE - 1);
			sBuf[INI_MAX_LINE - 1] = '\0';
			strToLower(strTrim(sBuf));

			if (strcmp(sLine + 1, sBuf) == 0)
				return iLineLen;
		}
	}
}

static int
iniGetKeyValue(FILE *fp, char *sKey, char *sValue, int iValueLen)
{
	char	sLine[INI_MAX_LINE];
	int	    iLineLen;
	int		iType;
	char	sBuf[INI_MAX_LINE];
	char	sBuf2[INI_MAX_LINE];
	char	*p, *p2;

    if (NULL == sValue) {
        return -4;
    }

	while (1) {
		iLineLen = filGetLine(sLine, sizeof(sLine), fp);
		if (iLineLen == 0) {
			return -1;
		} else if (iLineLen < 0) {
			return -2;
		}

		iType = iniGetType(sLine);
		if (iType == eKeyValue) {
			strcpy(sBuf, sLine);
			p = strStrChr(sBuf, iniIsEqual, iniIsEnd);
			if (p == NULL)
				return -3;
			*p = '\0';
			strToLower(strRightTrim(sBuf));

			strncpy(sBuf2, sKey, INI_MAX_LINE - 1);
			sBuf2[INI_MAX_LINE - 1] = '\0';
			strToLower(strTrim(sBuf2));

			if (strcmp(sBuf, sBuf2) == 0) {
				if (sValue == NULL && iValueLen == 0) {
					fseek(fp, -iLineLen, SEEK_CUR);

				} else {
					p++;
					p2 = strStrChr(p, iniIsEnd, NULL);
					if (p2 != NULL)
						*p2 = '\0';
					strTrim(p);
					strncpy(sValue, p, iValueLen - 1);
					sValue[iValueLen - 1] = '\0';
				}
				return iLineLen;
			}

		} else if (iType == eSection) {
			if (sValue == NULL || iValueLen == 0)
				fseek(fp, -iLineLen, SEEK_CUR);
			return -1;
		}
	}
}

/*-----------------------------  End ------------------------------------*/

