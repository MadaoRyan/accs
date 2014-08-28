/*
 *
 *
 * file system functions.
 *
 *
 * FileName: os_fs.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include "os_fs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "util_dtm.h"
#include "util_str.h"

/*--------------------------- Macro define ------------------------------*/
#define _DLEN_TINY_BUF 64
#define _DLEN_MINI_BUF 256
#define _DLEN_LARGE_BUF 1024
#define _DLEN_HUGE_BUF 10240

#define	OS_PATH_MAX	4096
#define	OS_DIR_MAX	4096

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/
static char	f_sLast[14+1];

/*-------------------------  Global functions ---------------------------*/
long fsGetFileSize(char *psFileName)
{
    int iRet = 0;
    struct stat tStatBuf;

    memset(&(tStatBuf), 0, sizeof(tStatBuf));

    iRet = stat(psFileName, &tStatBuf);

    if(iRet < 0) {
        return iRet;
    }

    return (long)tStatBuf.st_size;
}

char * fsGetNewFile(char *sPrefix, char *sFile)
{
	DIR		*ptDir;
	char	sPath[OS_PATH_MAX], sPath2[OS_PATH_MAX];
	int		iSum = 0;

	dtmGetCurTime(sPath);
	if (memcmp(f_sLast, sPath, 8) != 0) {
		strcpy(f_sLast, sPath);
		strPathJoin(sPrefix, sPath);
		mkdir(sPath, S_IRWXU | S_IRWXG | S_IRWXO);
		strPathJoin(sPath, sFile);
		return sFile;
	}

	strcpy(sPath2, f_sLast);
	strPathJoin(sPrefix, sPath2);

	ptDir = opendir(sPath2);
	while (ptDir && readdir(ptDir) && iSum < OS_DIR_MAX) iSum++;
	if (iSum >= OS_DIR_MAX) {
		strcpy(f_sLast, sPath);
		strPathJoin(sPrefix, sPath);
		mkdir(sPath, S_IRWXU | S_IRWXG | S_IRWXO);
		strPathJoin(sPath, sFile);
		closedir(ptDir);
		return sFile;
	}

	closedir(ptDir);
	strPathJoin(sPath2, sFile);
	return sFile;
}

int fsReadData(char *psFile, int iPos, char *psBuf, int iLen)
{
    int iRet = 0;
    FILE *fpIn = NULL;
    
    fpIn =  fopen(psFile, "rb");
    if(NULL == fpIn) {
        return -1;
    }
    
    if(iPos > 0) {
        iRet = fseek(fpIn, iPos, SEEK_SET);
    }
    
    iRet = fread(psBuf, sizeof(char), iLen, fpIn);
    if(iRet < 0) {
        fclose(fpIn);
        return -3;
    }
    
    fclose(fpIn);
    
    return iRet;
}

int fsWriteData(char *psFile, int iPos, char *psBuf, int iLen)
{
    int iRet = 0;
    FILE *fpOut = NULL;
    char sCrtOpt[_DLEN_TINY_BUF];
    
    /*init variable*/
    memset(sCrtOpt, 0, sizeof(sCrtOpt));
    
    if(iPos == 0) {
        strcpy(sCrtOpt, "wb");
    } else if(iPos > 0) {
        strcpy(sCrtOpt, "r+b");
    } else {
        strcpy(sCrtOpt, "ab");
    }
    
    /*process*/
    
    fpOut =  fopen(psFile, sCrtOpt);
    if(NULL == fpOut) {
        return -2;
    }

    if(iPos > 0) {
        iRet = fseek(fpOut, iPos, SEEK_SET);
    }
    
    iRet = fwrite(psBuf, sizeof(char), iLen, fpOut);
    if(iRet < 0) {
        fclose(fpOut);
        return -3;
    }
    
    fclose(fpOut);
    
    return iRet;
}

int fsJoinFile(char *psDest, int iPos, char *psSrc, int iInPos,  int iLen)
{
    int iRet = 0;
    FILE *fpIn = NULL;
    FILE *fpOut = NULL;
    char sCrtOpt[_DLEN_TINY_BUF];
    char sTmpData[_DLEN_LARGE_BUF];
    int iReadLen = 0;
    int iTotal = 0;
    
    
    /*init variable*/
    memset(sCrtOpt, 0, sizeof(sCrtOpt));
    
    if(iPos == 0) {
        strcpy(sCrtOpt, "wb");
    } else if(iPos > 0) {
        strcpy(sCrtOpt, "r+b");
    } else {
        strcpy(sCrtOpt, "ab");
    }
    
    /*process*/
    
    fpIn =  fopen(psSrc, "rb");
    if(NULL == fpIn) {
        return -1;
    }
    
    if(iInPos > 0) {
        iRet = fseek(fpIn, iInPos, SEEK_SET);
    }
    
    fpOut =  fopen(psDest, sCrtOpt);
    if(NULL == fpOut) {
        fclose(fpIn);
        return -2;
    }

    if(iPos > 0) {
        iRet = fseek(fpOut, iPos, SEEK_SET);
    }

    for(;;) {
        memset(sTmpData, 0, sizeof(sTmpData));
        
        if(iLen > 0) {
            iReadLen = iLen - iTotal;
            if(iReadLen > sizeof(sTmpData)) {
                iReadLen = sizeof(sTmpData);
            }
        } else {
            iReadLen = sizeof(sTmpData);
        }
        
        iRet = fread(sTmpData, sizeof(char), iReadLen, fpIn);
        if(iRet < 0) {
            fclose(fpIn);
            fclose(fpOut);
            return -3;
        }
        
        if(iRet == 0) {
            break;
        }
        
        iTotal += iReadLen;
        
        iRet = fwrite(sTmpData, sizeof(char),iReadLen, fpOut);
        if(iRet < 0) {
            fclose(fpIn);
            fclose(fpOut);
            return -3;
        }
    }

    fclose(fpIn);
    fclose(fpOut);
    
    
    return iTotal;
}
/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

