/*
 *
 *
 *  userlog proc.
 *
 *
 * FileName: userlog.c 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

/*------------------------ Include files ------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include <stdarg.h>

#include "util.h"
#include "userlog.h"

#if 0
#pragma mark -
#pragma mark < Macro define >
#endif
/*--------------------- Macro define ----------------------------*/
#define MSG_BUF_LEN 2048
#define ERR_DEF_SPLIT '^'

#if 0
#pragma mark -
#pragma mark < Type define >
#endif
/*--------------------- Type define -----------------------------*/
/* Fro User Logger */
typedef struct UserLogOneMsg {
    int nErrno;
    int nLen;
    int nOffset;
} T_UserLogOneMsg;

typedef struct UserLog {
    int nMaxLevel;
    int nNowLevel;
    int nStartLevel;
    char *psBuffer;
    int  nOffset;
    int  nLeft;
    int  nMaxBuf;
    T_UserLogOneMsg *ptMsgList;
    T_CTN *ptErrMsgList;
} T_UserLog;

typedef  struct UserErrMsg {
    int nErrno;
    T_SPLIT_STR tErrString;
} T_UserErrMsg;

/*--------------------- Local function declaration --------------*/
static int locInitDir(T_CTN *ptErrCtn, char *psPath, char *psLanguage);
static int locInitOneDir(T_CTN *ptErrCtn, char *psPath, char *psName);
static int locInitOneFile(T_CTN *ptErrCtn, char *psFileName);
static int locReadLine(FILE *ptFp, char *psBuf, int nBufLen);
static int locAddErrMsg(T_CTN *ptErrCtn, T_UserErrMsg *ptErrMsg, T_SPLIT_STR *ptSplitStr);
static T_UserLogOneMsg * locGetLastOneMsg(T_UserLog *ptUserLog);
static int locVsnprintf(char *psBuffer, int nMaxLen, char *psString, va_list tVaList);
static int locFreeBuf(T_UserLog *ptUserLog, int nMaxBuf);
static int locSaveErrMsg(T_CTN *ptErrCtn, T_UserErrMsg *ptErrMsg);
static int locFreeSplitStr(T_SPLIT_STR *ptSplitStr);
static char * locGetMsgBuffer(int *pnMaxBufferLen);

/*--------------------- Global variable -------------------------*/
static char *gpsMsgBuffer = NULL;
static int gnMaxMsgBuffer = 0;

#if 0
#pragma mark -
#pragma mark < Global functions >
#endif
/*--------------------- Global functions ------------------------*/
/**
 * usrLogInit
 * Init user Log Obj
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
TP_UserLog usrLogInit(int nMaxLevel, int nMaxBuffer)
{
    T_UserLog *ptUserLog = (T_UserLog*)malloc(sizeof(T_UserLog));
    memset(ptUserLog, '\0', sizeof(T_UserLog));
    
    /* New Table */
    ptUserLog->ptErrMsgList = ctnNew(sizeof(T_UserErrMsg), ctnComparInt);
    if ( NULL == ptUserLog->ptErrMsgList ) {
    	free(ptUserLog);
        return (NULL);
    }
    
    /* get Max Level */
    ptUserLog->nMaxLevel = nMaxLevel;
    ptUserLog->ptMsgList = (T_UserLogOneMsg*)malloc(sizeof(T_UserLogOneMsg)*ptUserLog->nMaxLevel);
    memset(ptUserLog->ptMsgList, '\0', sizeof(T_UserLogOneMsg)*ptUserLog->nMaxLevel);
    
    /* init Buffer */
    ptUserLog->nMaxBuf = nMaxBuffer;
    ptUserLog->psBuffer = (char*)malloc(ptUserLog->nMaxBuf+1);
    memset(ptUserLog->psBuffer, '\0', ptUserLog->nMaxBuf+1);
    
    /* Init Other Info */
    ptUserLog->nNowLevel   = 0;
    ptUserLog->nStartLevel = 0;
    ptUserLog->nOffset     = 0;
    ptUserLog->nLeft       = ptUserLog->nMaxBuf;
    
    return ( ptUserLog );
}

/**
 * usrLogLoadFile
 * Add User Log Info
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int usrLogLoadFile(TP_UserLog ptUserLog, char *psLanguage, char *psDir)
{
    int nRet = 0;
    
    if ( NULL == ptUserLog ) {
        return  (-1);
    }
    
    /* Get Info File */
    nRet = locInitDir(ptUserLog->ptErrMsgList, psDir, psLanguage);
    if ( nRet != 0 ) {
        return ( nRet );
    }
    
    /* sort All Info */
    ctnSort(ptUserLog->ptErrMsgList);
    
    return (0);
}

/**
 * userLogFree
 * Free User Log
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int usrLogFree(TP_UserLog ptUserLog)
{
    if ( NULL == ptUserLog ) {
        return ( 0 );
    }
    
    if ( NULL != ptUserLog->ptErrMsgList ) {
        int nMax = ctnCount(ptUserLog->ptErrMsgList);
        int i = 0;
        for (; i<nMax; ++i) {
            T_UserErrMsg *ptErrMsg = ctnGet(ptUserLog->ptErrMsgList, i);
            locFreeSplitStr(&(ptErrMsg->tErrString));
        }
        
        ctnFree(ptUserLog->ptErrMsgList);
    }
    
    free(ptUserLog->ptMsgList);
    free(ptUserLog->psBuffer);
    free(ptUserLog);
    
    return ( 0 );
}

/**
 * usrLogInput
 * Inoout err string
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int usrLogInput(TP_UserLog ptUserLog, int nErrno, char *psBuffer, ...)
{
    int nRet = 0;
    
    va_list tVaList;
	va_start(tVaList, psBuffer);
    nRet = usrLogInputByList(ptUserLog, nErrno, psBuffer, tVaList);
    va_end(tVaList);
    
    return ( nRet );
}

/**
 * usrLogInputByList
 * Inout err string
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int usrLogInputByList(TP_UserLog ptUserLog, int nErrno, char *psBuffer, va_list tVaList)
{
    int nRet = 0;
    
    /* Check Level */
    T_UserLogOneMsg *ptMsg = locGetLastOneMsg(ptUserLog);
    ptMsg->nErrno = nErrno;
    
    /* output to buffer */
    ptMsg->nOffset = ptUserLog->nOffset;
    int nLen = locVsnprintf(ptUserLog->psBuffer+ptMsg->nOffset, ptUserLog->nLeft, psBuffer, tVaList);
    if ( nLen > ptUserLog->nLeft ) {
        ptMsg->nOffset = locFreeBuf(ptUserLog, nLen + 1);
        if ( ptMsg->nOffset < 0 ) {
            nLen = -1;
        } else {
            nLen = locVsnprintf(ptUserLog->psBuffer+ptMsg->nOffset, ptUserLog->nLeft, psBuffer, tVaList);
        }
    }

    ptMsg->nLen = nLen + 1;
    ptUserLog->nOffset += ptMsg->nLen;
    ptUserLog->nLeft -= ptMsg->nLen;
    
    return ( nRet );
}

/**
 * logUserLogClean
 * Clean Log
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int usrLogClean(TP_UserLog ptUserLog)
{
    int nRet = 0;
    
    ptUserLog->nLeft = ptUserLog->nMaxBuf;
    ptUserLog->nNowLevel = 0;
    ptUserLog->nOffset = 0;
    ptUserLog->nStartLevel = 0;
    memset(ptUserLog->psBuffer, '\0', sizeof(ptUserLog->nMaxBuf));
    memset(ptUserLog->ptMsgList, '\0', sizeof(T_UserLogOneMsg)*ptUserLog->nMaxLevel);
    
    return ( nRet );
}

/**
 * usrLogGetOutPutNum
 * Get Out Put Msg Num
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int usrLogGetOutPutNum(TP_UserLog ptUserLog)
{
    return ( ptUserLog->nNowLevel > ptUserLog->nMaxLevel ? ptUserLog->nMaxLevel : ptUserLog->nNowLevel );
}

/**
 * usrLogOutPut
 * Out Put One Info
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int usrLogOutPut(TP_UserLog ptUserLog, FILE *ptFp, int nIndex)
{
    int nRet = 0;
    
    int nMax = ptUserLog->nNowLevel > ptUserLog->nMaxLevel ? ptUserLog->nMaxLevel : ptUserLog->nNowLevel;
    if ( nIndex >= nMax ) {
        return (LOG_USER_OUT_PUT_END);
    }
    
    int nLevel = (nIndex + ptUserLog->nStartLevel) % ptUserLog->nMaxLevel;
    T_UserLogOneMsg *ptMsg = &(ptUserLog->ptMsgList[nLevel]);
    
    /* Make Buffer */
    char *psBuffer = ptUserLog->psBuffer+ptMsg->nOffset;
    if ( ptMsg->nLen <= 0 ) {
        psBuffer = "";
    }
    
    /* Err Info */
    T_SPLIT_STR tSplitStr;
    memset(&(tSplitStr), '\0', sizeof(T_SPLIT_STR));
    nRet = csvSplitStr(psBuffer, ERR_DEF_SPLIT, &(tSplitStr));
    if ( nRet != 0 ) {
        return ( LOG_USER_FORMAT_ERROR );
    }
    
    /* Get Err Info */
    T_SPLIT_STR *ptErrMsg = NULL;
    T_UserErrMsg *ptUsrErrMsg = ctnSearch(ptUserLog->ptErrMsgList, (void *)&ptMsg->nErrno);
    if ( NULL != ptUsrErrMsg ) {
        ptErrMsg = &(ptUsrErrMsg->tErrString);
    }
    
    int i = 0;
    for (; 1; ++i) {
        if ( (NULL == ptErrMsg || i >= ptErrMsg->iCnt) && i >= tSplitStr.iCnt ) {
            break;
        }
        
        if ( (NULL != ptErrMsg) && (i < ptErrMsg->iCnt) ) {
            fprintf(ptFp, "%s", ptErrMsg->psCol[i]);
        }
        
        if ( i < tSplitStr.iCnt ) {
            fprintf(ptFp, "%s", tSplitStr.psCol[i]);
        }
    }
    
    return ( nRet );
}

/**
 * usrLogMemOutPutOne
 * Out Put Info to Memory immediately
 * 
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int usrLogMemOutPutOne(TP_UserLog ptUserLog, char *psBuffer, int nMaxBufferLen, int nErrno, char *psString, ... )
{
    int nRet = 0;
    
    va_list tVaList;
	va_start(tVaList, psString);
    nRet = usrLogMemOutPutOneByList(ptUserLog, psBuffer, nMaxBufferLen, nErrno, psString, tVaList);
    va_end(tVaList);
    
    return ( nRet );
}

/**
 * usrLogMemOutPutOneByList
 * Out Put Info To Memory immedately by arg list
 * 
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int usrLogMemOutPutOneByList(TP_UserLog ptUserLog, char *psBuffer, int nMaxBufferLen, int nErrno, char *psString, va_list tVaList)
{
    int nRet = 0;
    
    /* Copy String Info */
    int nRealLen = nMaxBufferLen;
    char *sMsgBuffer = locGetMsgBuffer(&(nRealLen));
    vsnprintf(sMsgBuffer, nRealLen, psString, tVaList);
    
    /* Err Info */
    T_SPLIT_STR tSplitStr;
    memset(&(tSplitStr), '\0', sizeof(T_SPLIT_STR));
    nRet = csvSplitStr(sMsgBuffer, ERR_DEF_SPLIT, &(tSplitStr));
    if ( nRet != 0 ) {
        return ( LOG_USER_FORMAT_ERROR );
    }
    
    /* Get Err Info */
    int nOffset = 0;
    T_SPLIT_STR *ptErrMsg = NULL;
    T_UserErrMsg *ptUsrErrMsg = ctnSearch(ptUserLog->ptErrMsgList, (void *)&nErrno);
    if ( NULL != ptUsrErrMsg ) {
        ptErrMsg = &(ptUsrErrMsg->tErrString);
    }
    
    int i = 0;
    for (; 1; ++i) {
        if ( (NULL == ptErrMsg || i >= ptErrMsg->iCnt) && i >= tSplitStr.iCnt ) {
            break;
        }
        
        if ( (NULL != ptErrMsg) && (i < ptErrMsg->iCnt) ) {
            nOffset += snprintf(psBuffer + nOffset, nMaxBufferLen - nOffset, "%s", ptErrMsg->psCol[i]);
            if ( nOffset >= nMaxBufferLen ) {
                break;
            }
        }
        
        if ( i < tSplitStr.iCnt ) {
            nOffset += snprintf(psBuffer + nOffset, nMaxBufferLen - nOffset, "%s", tSplitStr.psCol[i]);
            if ( nOffset >= nMaxBufferLen ) {
                break;
            }
        }
    }
    
    return ( nRet );
}

#if 0
#pragma mark -
#pragma mark < Local functions >
#endif
/*--------------------- Local functions -------------------------*/
/**
 * locFreeSplitStr
 * Free Split Str Info
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int locFreeSplitStr(T_SPLIT_STR *ptSplitStr)
{
    int i = 0;
    for (; i<ptSplitStr->iCnt; ++i) {
        free(ptSplitStr->psCol[i]);
    }
    
    return ( 0 );
}

/**
 * logUserLogAppenderInitDir
 * read all dir
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int locInitDir(T_CTN *ptErrCtn, char *psPath, char *psLanguage)
{
    int nRet = 0;
    
    nRet = locInitOneDir(ptErrCtn, psPath, psLanguage);
    if ( nRet != 0 ) {
        return ( nRet );
    }
    
    nRet = locInitOneDir(ptErrCtn, psPath, "default");
    if ( nRet != 0 ) {
        return ( nRet );
    }
    
    return ( nRet );
}

/**
 * logUserLogAppenderInitOneDir
 * Read One DIr
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int locInitOneDir(T_CTN *ptErrCtn, char *psPath, char *psName)
{
    int nRet = 0;
    
    char sDir[MSG_BUF_LEN];
    memset(sDir, '\0', sizeof(sDir));
    sprintf(sDir, "%s/%s", psPath, psName);
    DIR * ptDir = opendir(sDir);
    if ( NULL == ptDir ) {
        return ( 0 );
    }
    
    /* Read dir */
    struct dirent * ptDirent = NULL;
    while ( (ptDirent = readdir(ptDir)) != NULL ) {
        char *psType = strrchr(ptDirent->d_name, '.');
        if ( NULL == psType ) {
            continue;
        }
        
        if ( strcmp(psType+1, "msg") != 0 ) {
            continue;
        }
        
        char sFile[MSG_BUF_LEN];
        sprintf(sFile, "%s/%s", sDir, ptDirent->d_name);
        nRet = locInitOneFile(ptErrCtn, sFile);
        if ( nRet != 0 ) {
            closedir(ptDir);
            return ( nRet );
        }
    }
    closedir(ptDir);
    
    return ( nRet );
}

/**
 * logUserLogAppenderInitFile
 * Init Log Aooender File
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int locInitOneFile(T_CTN *ptErrCtn, char *psFileName)
{
    int nRet = 0;
    
    /* Read File */
    FILE *ptFile = fopen(psFileName, "r");
    if ( NULL == ptFile ) {
        return ( LOG_USER_FILE_NO_FIND );
    }
    
    char sBuffer[MSG_BUF_LEN+1];
    T_UserErrMsg tUser;
    memset(&(tUser), '\0', sizeof(T_UserErrMsg));
    while ( 0 == locReadLine(ptFile, sBuffer, MSG_BUF_LEN) ) {
        /* New User Info */
        if ( sBuffer[0] == '#' ) {
            continue;
        }
        
        /* Get Err String Info */
        T_SPLIT_STR tSplitStr;
        memset(&(tSplitStr), '\0', sizeof(T_SPLIT_STR));
        nRet = csvSplitStr(sBuffer, '%', &(tSplitStr));
        if ( nRet != 0 ) {
            fclose(ptFile);
            return ( LOG_USER_FORMAT_ERROR );
        }
        
        /* Check Err */
        int i = 1;
        int j = 1;
        int nMax = tSplitStr.iCnt;
        for (; j<nMax; ++j) {
            if ( tSplitStr.psCol[j][0] == 's' ||
                tSplitStr.psCol[j][0] == 'd') {
                tSplitStr.psCol[i++] = tSplitStr.psCol[j] + 1;
            } else {
                --tSplitStr.iCnt;
                char *p = --tSplitStr.psCol[j];
                *p = '$';
            }
        }
        
        /* Add To CTN */
        nRet = locAddErrMsg(ptErrCtn, &(tUser), &(tSplitStr));
        if ( nRet != 0 ) {
            fclose(ptFile);
            return ( nRet );
        }
    }
    fclose(ptFile);
    
    /* Save Last One */
    nRet = locSaveErrMsg(ptErrCtn, &(tUser));
    if ( nRet != 0 ) {
        return ( nRet );
    }
    
    return ( nRet );
}

/**
 * logReadLine
 * Read Line
 * Trim Lift
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int locReadLine(FILE *ptFp, char *psBuf, int nBufLen)
{
    memset(psBuf, '\0', nBufLen);
    if ( fgets(psBuf, nBufLen, ptFp) == NULL ){
        return (1);
    }
    
    if ( psBuf[nBufLen-1] != '\0' ) {
        return ( LOG_USER_LINE_TOO_LONG );
    }
    
    return (0);
}

/**
 * logUserLogAppenderAddErrMsg
 * Add ErrMsg To CTN
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int locAddErrMsg(T_CTN *ptErrCtn, T_UserErrMsg *ptErrMsg, T_SPLIT_STR *ptSplitStr)
{
    int nRet = 0;
    
    if ( 0 == isblank(ptSplitStr->psCol[0][0]) && ptErrMsg->nErrno > 0 ) {
        nRet = locSaveErrMsg(ptErrCtn, ptErrMsg);
        if ( nRet != 0 ) {
            return ( nRet );
        }
        memset(ptErrMsg, '\0', sizeof(T_UserErrMsg));
    }
    
    /* Add Errno */
    if ( ptErrMsg->nErrno <= 0 ) {
        ptErrMsg->nErrno = atoi(ptSplitStr->psCol[0]);
    }
    
    int i = 0;
    for (; i<ptSplitStr->iCnt; ++i) {
        ptErrMsg->tErrString.psCol[ptErrMsg->tErrString.iCnt++] = strdup(ptSplitStr->psCol[i]);
        
        if ( ptErrMsg->tErrString.iCnt >= sizeof(ptErrMsg->tErrString.psCol)/sizeof(char*) ) {
            return ( LOG_USER_LINE_TOO_MORE );
        }
    }
    
    return ( nRet );
}

/**
 * locSaveErrMsg
 * Save Err Msg
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int locSaveErrMsg(T_CTN *ptErrCtn, T_UserErrMsg *ptErrMsg)
{
    int nRet = 0;
    
    if ( ptErrMsg->nErrno <= 0 ) {
        return (0);
    }
    
    /* Check Err Msg Has exist */
    void *p = ctnSearchL(ptErrCtn, (void *)ptErrMsg, NULL);
    if ( NULL != p ) {
        return ( 0 );
    }
    
    /* Add */
    nRet = ctnAdd(ptErrCtn, (void *)ptErrMsg);
    if ( nRet != 0 ) {
        return ( LOG_USER_ADD_ERROR );
    }
    
    return  (0);
}

/**
 * logUserGetLastOneMsg
 * Get Last One Msg
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static T_UserLogOneMsg * locGetLastOneMsg(T_UserLog *ptUserLog)
{
    int nLevel = ptUserLog->nNowLevel++ % ptUserLog->nMaxLevel;
    if ( ptUserLog->nNowLevel < ptUserLog->nMaxLevel ) {
        return ( &(ptUserLog->ptMsgList[nLevel]) );
    }
    
    /* Clean level */
    ptUserLog->nStartLevel = ptUserLog->nNowLevel % ptUserLog->nMaxLevel;
    T_UserLogOneMsg *ptMsg = &(ptUserLog->ptMsgList[nLevel]);
    ptMsg->nLen    = 0;
    ptMsg->nOffset = 0;
    ptMsg->nErrno  = 0;
    
    return ( ptMsg );
}

/**
 * logVsnprintf
 * my vsnprintf
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int locVsnprintf(char *psBuffer, int nMaxLen, char *psString, va_list tVaList)
{
    va_list ap;
    va_copy(ap, tVaList);
    int nLen = vsnprintf(psBuffer, nMaxLen, psString, ap);
    va_end(ap);
    
    return ( nLen );
}

/**
 * logUserFreeBuf
 * Free Buffer
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int locFreeBuf(T_UserLog *ptUserLog, int nMaxBuf)
{
    /* Check Max Buffer */
    if ( nMaxBuf > ptUserLog->nMaxBuf ) {
        return (-1);
    }
    
    /* set offset */
    int nOffset = ptUserLog->nOffset;
    int nLen    = ptUserLog->nLeft;
    
    /* Get Free Buffer */
    int i = 0;
    int nMax = ptUserLog->nNowLevel > ptUserLog->nMaxLevel ? ptUserLog->nMaxLevel : ptUserLog->nNowLevel;
    for (; i<nMax; i++) {
        int nLevel = (i + ptUserLog->nStartLevel) % ptUserLog->nMaxLevel;
        T_UserLogOneMsg *ptMsg = &(ptUserLog->ptMsgList[nLevel]);
        if ( ptMsg->nLen <= 0 ) {
            continue;
        }
        
        /* Check Offset */
        if ( ptMsg->nOffset < nOffset ) {
            if ( ptUserLog->nMaxBuf - ptUserLog->nOffset > nMaxBuf ) {
                nLen = ptUserLog->nMaxBuf - ptUserLog->nOffset;
                break;
            }
            
            nOffset = 0;
            nLen    = ptMsg->nOffset;
            if ( nLen > nMaxBuf ) {
                break;
            }
        }
        
        /* Add nLen */
        nLen += ptMsg->nLen;
        
        /* Clean Msg */
        if ( ptMsg->nErrno <= 0 && ptMsg->nLen > 0 ) {
            ptMsg->nErrno = atoi(ptUserLog->psBuffer + ptMsg->nOffset);
        }
        ptMsg->nLen = 0;
        ptMsg->nOffset = -1;
        
        if ( nOffset == 0 && nLen == ptUserLog->nOffset ) {
            nLen = ptUserLog->nMaxBuf;
            break;
        }
        
        /* Check Len */
        if ( nLen >= nMaxBuf ) {
            break;
        }
    }
    
    /* CheckLen */
    if ( nLen < nMaxBuf ) {
        return (-1);
    }
    
    ptUserLog->nOffset = nOffset;
    ptUserLog->nLeft   = nLen;
    
    return ( nOffset );
}

/**
 * locGetMsgBuffer
 * get Static Msg Buffer 
 * 
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static char * locGetMsgBuffer(int *pnMaxBufferLen)
{
    if ( gnMaxMsgBuffer >= *pnMaxBufferLen ) {
        *pnMaxBufferLen = gnMaxMsgBuffer;
        return ( gpsMsgBuffer );
    }
    
    free(gpsMsgBuffer);
    gnMaxMsgBuffer = *pnMaxBufferLen;
    gpsMsgBuffer = (char *)malloc(gnMaxMsgBuffer);
    
    return ( gpsMsgBuffer );
}

/*--------------------- End -------------------------------------*/
