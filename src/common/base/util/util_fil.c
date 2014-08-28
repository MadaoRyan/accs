/*
 *
 *
 * File Handle Functions
 *
 *
 * FileName: util_fil.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */

/*--------------------------- Include files -----------------------------*/
#include	"util_fil.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include    <unistd.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<sys/types.h>

#include	"os_lock.h"
#include	"util_dtm.h"
#include	"util_str.h"

/*--------------------------- Macro define ------------------------------*/
#define _DLEN_TINY_BUF 64
#define _DLEN_MINI_BUF 256
#define _DLEN_LARGE_BUF 1024
#define _DLEN_HUGE_BUF 10240

#define	FILE_MAX_COUNT	10000

/*---------------------------- Type define ------------------------------*/
static char	f_sLast[32];
static int	f_iFileMax = FILE_MAX_COUNT;

static int f_iSSN = 0;
/*---------------------- Local function declaration ---------------------*/
static int filValidDir(char *sPath, char *pEnd);

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
/**
 * filRead
 * 读取文件内容至内存。
 *
 * @param sFile: 文件名称
 * @param sBuf: 目标内存区域
 * @param *piLen: in-目标内存区域大小 out-读取的文件大小
 *
 * @return 0: 成功
 *        <0: 失败
 *
 */
int
filRead(char *sFile, char *sBuf, int *piLen)
{
	FILE	*fp;
	int		iRet;

	fp = fopen(sFile, "r");
	if (fp == NULL) {
		return -1;
	}

	iRet = fread(sBuf, sizeof(char), *piLen, fp);
	if (iRet < *piLen && ferror(fp)) {
		fclose(fp);
		return -2;
	}

	fclose(fp);
	*piLen = iRet;

	return 0;
}

/**
 * filWrite
 * 写内存至文件。
 *
 * @param sBuf: 内存区域
 * @param iLen: 目标内存区域大小
 * @param sFile: 目标文件名称
 *
 * @return 0: 成功
 *        <0: 失败
 *
 */
int
filWrite(char *sBuf, int iLen, char *sFile)
{
	FILE	*fp;
	int		iRet;

	fp = fopen(sFile, "w");
	if (fp == NULL) {
		return -1;
	}

	iRet = fwrite(sBuf, sizeof(char), iLen, fp);
	if (iRet < iLen) {
		fclose(fp);
		return -2;
	}

	fclose(fp);

	return 0;
}

/**
 * filDateDir
 * 生成带日期路径，并建立该目录
 * 例：in-"/tmp/" out-"/tmp/20100105/"
 *
 * @param sPath: in-原始路径 out-拼上日期的路径
 *
 * @return sPath: 成功
 *          NULL: 失败
 *
 */
char *
filDateDir(char *sPath)
{
	char	sDate[8+1];

	dtmGetCurDate(sDate);
	strPathJoin2(sPath, sDate);

	mkdir(sPath, S_IRWXU | S_IRWXG | S_IRWXO);

	return sPath;
}

char *
filDateDir2(char *sPath)
{
	char	sDate[8+1], *pEnd;

	dtmGetCurDate(sDate);

	if (memcmp(f_sLast, sDate, 8) == 0) {
		strPathJoin2(sPath, f_sLast);
		pEnd = sPath + strlen(sPath) - (strlen(f_sLast) - 8);
	} else {
		strPathJoin2(sPath, sDate);
		pEnd = sPath + strlen(sPath);
	}

	if (filValidDir(sPath, pEnd) < 0) {
		return NULL;
	}

	strcpy(f_sLast, pEnd - 8);
	return sPath;
}

/**
 * filGetSeqNext
 * 在文件中读取序号
 *
 * @param sFileName: 文件路径
 * @param iMax: 序号最大值
 *
 * @return >=0: 序号
 *          <0: 失败
 *
 */
int
filGetSeqNext(char *sFileName, int iMax)
{
	FILE	*fp;
	int		iExist = 1, iSeq, iRet;

	fp = fopen(sFileName, "r+");
	if (fp == NULL && errno == ENOENT) {
		fp = fopen(sFileName, "w");
		iExist = 0;
	}

	if (fp == NULL) {
		return -1;
	}

	do {
		iRet = lockFileWriteW(fileno(fp), 0, SEEK_SET, 0);
		if (iRet < 0) {
			if (errno == EINTR) {
				continue;	
			}
			fclose(fp);
			return -2;
		}
		break;
	} while (1);

	iSeq = -1;

	if (iExist) {
		iRet = fscanf(fp, "%d", &iSeq);
		if (iRet < 0 && ferror(fp)) {
			fclose(fp);
			return -3;
		}

		iRet = fseek(fp, 0, SEEK_SET);
		if (iRet < 0) {
			fclose(fp);
			return -4;
		}
	}

	iSeq = iSeq + 1 >= iMax ? 0 : iSeq + 1;
	iRet = fprintf(fp, "%d\n", iSeq);
	if (iRet < 0) {
		fclose(fp);
		return -5;
	}

	iRet = lockFileUnLock(fileno(fp), 0, SEEK_SET, 0);
	if (iRet < 0) {
		fclose(fp);
		return -6;
	}

	fclose(fp);

	return iSeq;
}

int
filGetLine(char *sBuf, int iLen, FILE *fpFile)
{
	char	*pRet;

	pRet = fgets(sBuf, iLen, fpFile);
	if (!pRet) {
		if (feof(fpFile)) {
			sBuf[0] = '\0';
			return 0;
		} else {
			return -2;
		}
	}
	iLen = strlen(sBuf);
	if (sBuf[iLen - 1] != '\n' && !feof(fpFile)) {
		return -1;
	}

	return iLen;
}

int filGetUniExt(char *psFileName)
{
    time_t ttTime;
    char sDateTime[_DLEN_TINY_BUF];
    struct timeval tval;
    struct tm *tmTime;
    
    memset(sDateTime, 0, sizeof(sDateTime));
    memset(&tval, 0, sizeof(tval));
    memset(&ttTime, 0, sizeof(ttTime));
    
    ttTime = time(NULL);

    tmTime = localtime(&ttTime);
    
    gettimeofday(&tval, NULL);
    
    sprintf(sDateTime, "%4d%02d%02d%02d%02d%02d%06ld", tmTime->tm_year + 1900,
        tmTime->tm_mon + 1, tmTime->tm_mday, tmTime->tm_hour,
        tmTime->tm_min, tmTime->tm_sec, (long)tval.tv_usec);
    sprintf(psFileName + strlen(psFileName), "%d.%s.%08d", getpid(), sDateTime, f_iSSN);
    
    f_iSSN += 1;
    if(f_iSSN > 99999999) {
        f_iSSN = 0;
    }
    
    return 0;
}

/*-------------------------  Local functions ----------------------------*/
static int
filValidDir(char *sPath, char *pEnd)
{
	char	sSeq[4096];
	int		iSum, i;

	i = *pEnd ? atoi(pEnd + 1) : 0;
	while (1) {
		mkdir(sPath, S_IRWXU | S_IRWXG | S_IRWXO);

		snprintf(sSeq, sizeof(sSeq), "%s/.dir.seq", sPath);
		iSum = filGetSeqNext(sSeq, 99999999);
		if (iSum < f_iFileMax) break;

		sprintf(pEnd, ".%d", ++i);
	}

	return 0;
}

/*-----------------------------  End ------------------------------------*/
