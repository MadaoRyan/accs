/*
 *
 *
 *  User Log.
 *
 *
 * FileName: log_plug_userlog.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

/*------------------------ Include files ------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "userlog.h"
#include "log_info.h"
#include "log_plug.h"
#include "util.h"
#include "log_plug_userlog.h"
#include "os_proc.h"

#if 0
#pragma mark -
#pragma mark < Macro define >
#endif
/*--------------------- Macro define ----------------------------*/
#define LOG_MSG_BUF_LEN                        2048
#define LOG_BUF_LEN                            256
#define LOG_MINI_BUF                           64

#define LOG_LEVEL_FILE_KEY                     "File"
#define LOG_LEVEL_LANGUAGE                     "Language"
#define LOG_LEVEL_LANGUAGE_DEFAULT             "zh_CN"
#define LOG_LEVEL_MAX_LEVEL                    "MaxLevel"
#define LOG_LEVEL_MAX_LEVEL_DEFAULT            10
#define LOG_LEVEL_MAX_BUF                      "MaxBuf"
#define LOG_LEVEL_MAX_BUF_DEFAULT              4096
#define LOG_LEVEL_ERR_DEF                      "ErrDef"
#define LOG_LEVEL_ERR_DEF_SPLIT                '^'

#define LOG_LEVEL_TIME_DEFAULT    "%Y-%m-%d %H:%M:%S"

#if 0
#pragma mark -
#pragma mark < Type define >
#endif
/*--------------------- Type define -----------------------------*/
typedef struct LogAddpenderUserLog{
    TP_UserLog ptUserLog;
    char sLogFileName[LOG_MSG_BUF_LEN];
} LogAddpenderUserLog;

/*--------------------- Local function declaration --------------*/
static int logUserLogAppenderInit(char *psFile, char *psPkg, char *psGroup, char *psLanguage, TP_UserLog ptUserLog);
static int logUserLogOutPrintf(LogArgs *ptLogArgs, void *ptOtherArgs, char *psBuf, va_list tVaList);
static void logAppenderFreeUserLog(void *ptOtherArgs);
static int logAppenderCtlUserLog(void *ptOtherArgs, char *psName, char *psValue);
static int logUserLogOutPut(LogAddpenderUserLog *ptUserLog);
static int LogTimePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);

/*--------------------- Global variable -------------------------*/

#if 0
#pragma mark -
#pragma mark < Global functions >
#endif
/*--------------------- Global functions ------------------------*/
/**
 * logAppenderPlugInit
 * Init Plug
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int logAppenderPlugInit(LogAppenderPoint ptLogAppender, char *psFile, char *psPkg, char *psGroup)
{
    int nRet = 0;
    
    /* Get File Name */
    LogAddpenderUserLog *ptUserLog = (LogAddpenderUserLog *)malloc(sizeof(LogAddpenderUserLog));
    memset(ptUserLog, '\0', sizeof(LogAddpenderUserLog));
    logCfgGetFileName(psGroup, psPkg, ptUserLog->sLogFileName, sizeof(ptUserLog->sLogFileName), psFile);
    
    /* Get User Log Info */
    int nMaxLen = logIniDefReadInt(psGroup, psPkg, LOG_LEVEL_MAX_LEVEL, LOG_LEVEL_MAX_LEVEL_DEFAULT, psFile);
    int nMaxBuf = logIniDefReadInt(psGroup, psPkg, LOG_LEVEL_MAX_BUF, LOG_LEVEL_MAX_BUF_DEFAULT, psFile);
    ptUserLog->ptUserLog = usrLogInit(nMaxLen, nMaxBuf);
    if ( NULL == ptUserLog->ptUserLog ) {
        logLog("User Log Init Err");
        return ( -1 );
    }
    
    /* Get Language Fix */
    char sLanguage[32];
    memset(sLanguage, '\0', sizeof(sLanguage));
    logIniDefRead(psGroup, psPkg, LOG_LEVEL_LANGUAGE, LOG_LEVEL_LANGUAGE_DEFAULT, sLanguage, sizeof(sLanguage), psFile);
    
    /* Get Info File */
    nRet = logUserLogAppenderInit(psFile, psPkg, psGroup, sLanguage, ptUserLog->ptUserLog);
    if ( nRet != 0 ) {
        logLog("Get File Info err!!\n");
        free(ptUserLog);
        return (-1);
    }

    /* New Appender */
    logInitAppender(ptLogAppender, logUserLogOutPrintf, NULL, NULL, logAppenderFreeUserLog, (void *)ptUserLog);
    logInitAppenderAddCtl(ptLogAppender, logAppenderCtlUserLog);
    logInitAppnederSetSession(ptLogAppender, LOG_SESSION_FLAG_OFF);
    
    return ( 0 );
}

#if 0
#pragma mark -
#pragma mark < Local functions >
#endif
/*--------------------- Local functions -------------------------*/
/**
 * logUserLogAppenderInit
 * Get All File Info
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logUserLogAppenderInit(char *psFile, char *psPkg, char *psGroup, char *psLanguage, TP_UserLog ptUserLog)
{
    int nRet = 0;
    
    int i = 1;
    while ( 1 ) {
        char sTempKey[LOG_BUF_LEN];
        sprintf(sTempKey, "%s.%d", LOG_LEVEL_ERR_DEF, i++);
        
        /* Get Temp File Name */
        char sFileName[LOG_BUF_LEN];
        memset(sFileName, '\0', sizeof(sFileName));
        logIniDefRead(psGroup, psPkg, sTempKey, "", sFileName, sizeof(sFileName), psFile);
        if ( '\0' == sFileName[0] ) {
            break;
        }
        
        /* Get Err File Info */
        nRet = usrLogLoadFile(ptUserLog, psLanguage, sFileName);
        if ( nRet != 0 ) {
            logLog("Init Err Dir[%s] err", sFileName);
            return ( -1 );
        }
    }
    
    return ( nRet );
}

/**
 * logUserLogOutPrintf
 * Printf Err Message Into 
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logUserLogOutPrintf(LogArgs *ptLogArgs, void *ptOtherArgs, char *psBuf, va_list tVaList)
{
    int nRet = 0;
    
    /* Check Level */
    LogAddpenderUserLog *ptUserLog = (LogAddpenderUserLog *)ptOtherArgs;
    int nErrno = atoi(ptLogArgs->psLogStr);
    char *psStr = strchr(ptLogArgs->psLogStr, '^');
    if ( NULL == psStr ) {
        return ( nRet );
    }
    ++psStr;
    
    /* Add Log Info */
    nRet = usrLogInputByList(ptUserLog->ptUserLog, nErrno, psStr, tVaList);
    if ( nRet != 0 ) {
        logLog("Inout UsrLog Err[%d]\n", nRet);
        return ( nRet );
    }
    
    return ( nRet );
}

/**
 * logAppenderFreeUserLog
 * Free User Log Info
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static void logAppenderFreeUserLog(void *ptOtherArgs)
{
    LogAddpenderUserLog *ptUserLog = (LogAddpenderUserLog *)ptOtherArgs;
    usrLogFree(ptUserLog->ptUserLog);
    free(ptUserLog);
    
    return;
}

/**
 * logAppenderCtlUserLog
 * Ctl User Log
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logAppenderCtlUserLog(void *ptOtherArgs, char *psName, char *psValue)
{
    /* Check */
    LogAddpenderUserLog *ptUserLog = (LogAddpenderUserLog *)ptOtherArgs;
    if ( strcmp(psName, LOG_USER_CLEAN) == 0 ) {
        return ( usrLogClean(ptUserLog->ptUserLog) );
    } else if ( strcmp(psName, LOG_USER_OUTPUT) == 0 ) {
        return ( logUserLogOutPut(ptUserLog) );
    }
    
    return (0);
}

/**
 * logUserLogOutPut
 * Session end Printf Info
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logUserLogOutPut(LogAddpenderUserLog *ptUserLog)
{
    int nRet = 0;
    
    /* Check Buffer */
    int nMax = usrLogGetOutPutNum(ptUserLog->ptUserLog);
    if ( nMax <= 0 ) {        
        return (0);
    }
    
    /* Open File */
    FILE *ptFp = fopen(ptUserLog->sLogFileName, "a+");
    if ( ptFp == NULL ) {
        logLog("File[%s] open err[%s]!! Now Write Log To stderr!!\n", ptUserLog->sLogFileName, strerror(errno));
        ptFp = stderr;
    }
    
    /* Print Err Log */
    fprintf(ptFp, "-------------------------------------------------------------------------------\n");
    
    char sBuffer[LOG_MINI_BUF];
    LogTimePrintf(sBuffer, sizeof(sBuffer), NULL, LOG_LEVEL_TIME_DEFAULT);
    fprintf(ptFp, "TIME[%s]  ", sBuffer);
    fprintf(ptFp, "PID[%d]  ", getpid());

    char *psAppName = logGetAppName();
    char sName[LOG_MINI_BUF];
    if (NULL == psAppName || '\0' == psAppName[0]) {
        memset(sName, '\0', sizeof(sName));
        prcGetProcName(getpid(), sName, sizeof(sName));
        psAppName = sName;
    }
    fprintf(ptFp, "PROGRAM[%s]  ", psAppName);
    
    char sSession[LOG_MINI_BUF];
    memset(sSession, '\0', sizeof(sSession));
    logGetSeesionId(sSession, sizeof(sSession));
    fprintf(ptFp, "SESSION[%s]  ", sSession);
    fprintf(ptFp, "\n\n");
    
    int nIndex = 0;
    for (; nIndex<nMax; nIndex++) {
        nRet = usrLogOutPut(ptUserLog->ptUserLog, ptFp, nIndex);   
        fprintf(ptFp, "\n");
    }
    
    /* Close File */
    fprintf(ptFp, "\n\n");
    if ( ptFp != NULL && ptFp != stderr ) {
        fclose(ptFp);
    }
    
    usrLogClean(ptUserLog->ptUserLog);
    
    return ( nRet );
}

/**
 * LogTimePrintf
 * Printf Time Info
 *
 * @param psBuf: Out Message Buf
 * @param nLen: Message Buf Len
 * @param ptLogArgs: Log Args Info
 * @param ptArgs: Ohter Args Not USE
 * @param tVaList: Printf Args
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogTimePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    time_t tSystime = time(NULL);
    struct tm *ptTm  = localtime(&tSystime);
    
    int nOffset = (int)strftime(psBuf, nLen, (char *)ptArgs, ptTm);
    return ( nLen - nOffset );
}


/*--------------------- End -------------------------------------*/
