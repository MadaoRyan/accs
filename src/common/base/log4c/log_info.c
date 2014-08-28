/*
 *
 *
 *  New Log Function.
 *
 *
 * FileName: log_info.c 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

/*------------------------ Include files ------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/stat.h>

#include "log_info.h"
#include "util.h"
#include "os_dll.h"
#include "log_plug.h"
#include "topcfg.h"
#include "os_lock.h"

#if 0
#pragma mark -
#pragma mark < Macro define >
#endif
/*--------------------- Macro define ----------------------------*/
#define LOG_CHECK(p,r) \
do { \
    if ( NULL == (p) ) {\
        logLog(#p" is NULL!!\n"); \
        return ((r)); \
    } \
} while (0);

#define LOGSTRCAT(a, b, i) \
do { \
    int iLen = strlen((b)); \
    memcpy((a), (b), iLen); \
    (i) += iLen;\
} while (0);

#define LOG_VERSION_NEW  2
#define LOG_VERSION_OLD  1

#define LOG_SECTION_NAME "log"
#define LOG_GLOBAL_SECTION_NAME "general"
#define LOG_GLOBAL_PREFIX "prefix"

#define LOG_LEVEL_NUMBER_DEFAULT 20
#define LOG_LEVEL_NAME_LEN       20
#define LOG_LEVEL_CFG_DEFAULT    "Default"
#define LOG_LEVEL_SPLIT          ","

#define LOG_LEVEL_LEVEL_KEY     "Level"
#define LOG_LEVEL_LEVEL_DEFAULT "0{DEBUG},1{E},2{W},3{N},4{D},5{T}"

#define LOG_LEVEl_DEBUG_STRING   0
#define LOG_LEVEL_ERROR          1
#define LOG_LEVEL_WARNING        2
#define LOG_LEVEL_NORMAL         3
#define LOG_LEVEL_DEBUG          4
#define LOG_LEVEL_TRACE          5

#define LOG_LEVEl_DEBUG_STRING_NAME "DEBUG_STRING"
#define LOG_LEVEL_ERROR_NAME        "ERROR"
#define LOG_LEVEL_WARNING_NAME      "WARNING"
#define LOG_LEVEL_DEBUG_NAME        "DEBUG"
#define LOG_LEVEL_NORMAL_NAME       "NORMAL"
#define LOG_LEVEL_TRACE_NAME        "TRACE"

#define LOG_LEVEL_LAYOUT_KEY      "Layout"
#define LOG_LEVEL_LAYOUT_SIMPLE   "SimpleLayout"
#define LOG_LEVEL_LAYOUT_TRACE    "TraceLayout"
#define LOG_LEVEL_LAYOUT_PATTERN  "PatternLayout"
#define LOG_LEVEL_PATTERN_KEY     "Layout.ConversionPattern"
#define LOG_LEVEL_PATTERN_DEFAUTL "[%d][%F:%L][%x][%p]%M{[%s]}%s{[%s]} %m"
#define LOG_LEVEL_TIME_DEFAULT    "%Y-%m-%d %H:%M:%S"
#define LOG_LEVEL_LAYOUT_DEFAULT  LOG_LEVEL_LAYOUT_SIMPLE

#define LOG_FORMAT_STRING     '%'
#define LOG_FORMAT_MESSAGE    'm'
#define LOG_FORMAT_FILE       'F'
#define LOG_FORMAT_LINE       'L'
#define LOG_FORMAT_FUNCTION   'M'
#define LOG_FORMAT_TIME       'd'
#define LOG_FORMAT_PROCESS_ID 'x'
#define LOG_FORMAT_LEVEL_NAME 'p'
#define LOG_FORMAT_RUN_TIME   'r'
#define LOG_FORMAT_SESSION    's'
#define LOG_FORMAT_GROUP      'G'
#define LOG_FORMAT_SECOND     'S'

#define LOG_LEVEL_APPENDER_FILE                "File"
#define LOG_LEVEL_APPENDER_CONSOLE             "Console"
#define LOG_LEVEL_APPENDER_ROLLING_FILE        "RollingFile"
#define LOG_LEVEL_APPENDER_BACKUP_ROLLING_FILE "BackupRollingFile"
#define LOG_LEVEL_APPENDER_DAILY_ROLLING_FILE  "DailyRollingFile"
#define LOG_LEVEL_APPENDER_USER_LOG            "UserLog"
#define LOG_LEVEL_APPENDER_DEFAULT             LOG_LEVEL_APPENDER_ROLLING_FILE

#define LOG_LEVEL_FILTER                       "Filter"
#define LOG_LEVEL_FILTER_FILE_NAME_DEFAULT     ""
#define LOG_FILTER_TYPE_NAME                   0
#define LOG_FILTER_TYPE_VALUE                  1
#define LOG_FILTER_TYPE_NUM                    2

#define LOG_LEVEL_FILE_KEY                     "File"
#define LOG_LEVEL_SIZE_KEY                     "Size"
#define LOG_LEVEL_TIME_PATTERN                 "TimePattern"
#define LOG_LEVEL_MAX_BACKUP_INDEX_KEY         "MaxBackupIndex"
#define LOG_LEVEL_SIZE_DEFAULT                 "50M"
#define LOG_LEVEL_FILE_DEFAULT                 "log/${APP}.log"
#define LOG_LEVEL_MAX_BACKUP_INDEX_DEFAULT     "1"

#define LOG_LEVEL_TIME_PATTERN_MONTHLY        "yyyy-MM"
#define LOG_LEVEL_TIME_PATTERN_DAILY          "yyyy-MM-dd"
#define LOG_LEVEL_TIME_PATTERN_HOUR           "yyyy-MM-dd-HH"
#define LOG_LEVEL_TIME_PATTERN_MINUTES        "yyyy-MM-dd-HH-mm"

#define LOG_LEVEL_TIME_PATTERN_MONTHLY_NUM     1
#define LOG_LEVEL_TIME_PATTERN_DAILY_NUM       3
#define LOG_LEVEL_TIME_PATTERN_HOUR_NUM        4
#define LOG_LEVEL_TIME_PATTERN_MINUTES_NUM     5

#define LOG_SESSION_KEY         "Session"
#define LOG_SESSION_STATUS_START 1
#define LOG_SESSION_STATUS_END   0

#define LOG_SESSION_FLAG_ON_NAME      "On"
#define LOG_SESSION_FLAG_OFF_NAME     "Off"
#define LOG_SESSION_FLAG_NOPRINT_NAME "NoPrint"

#define LOG_DEBUG_STRING_KEY "DebugString"
#define LOG_DEBUG_STRING_NEW "New"
#define LOG_DEBUG_STRING_OLD "Old"

#define LOG_DEBUG_PRINT_KEY          "DebugString.Print"
#define LOG_DEBUG_PRINT_NO_NAME      "No"
#define LOG_DEBUG_PRINT_ALL_NAME     "All"
#define LOG_DEBUG_PRINT_DEFAULT_NAME LOG_DEBUG_PRINT_NO_NAME
#define LOG_DEBUG_PRINT_NO           '1'
#define LOG_DEBUG_PRINT_ALL          '2'
#define LOG_DEBUG_PRINT_DEFAULT      LOG_DEBUG_PRINT_NO

#define LOG_ACCESS_CHECK_FILE_EXIST 0

#define LOG_BUF_LEN 256
#define LOG_MSG_BUF_LEN 2048

#define LOG_TRUE  1
#define LOG_FALSE 0

#define LOG_SIZE_KB 1024
#define LOG_SIZE_MB LOG_SIZE_KB*LOG_SIZE_KB
#define LOG_SIZE_GB LOG_SIZE_KB*LOG_SIZE_KB*LOG_SIZE_KB

#define LOG_PLUG_PATH_NAME "Path"
#define LOG_PLUG_FUNCTION_NAME "Init"
#define LOG_PLUG_FUCNTION_DEFAULT "logAppenderPlugInit"

#define LOG_TYPE_STRING 1
#define LOG_TYPE_VAR    2
#define LOG_TYPE_ENV    3

#define LOG_MINI_BUF 64

#define LOG_FORMAT_NO_FORMAT   '0'
#define LOG_FORMAT_NEED_FORMAT '1'

#if 0
#pragma mark -
#pragma mark < Type define >
#endif
/*--------------------- Type define -----------------------------*/
/* For Dispaly log struct */
typedef struct LogFormat {
    int (*fLogPrintf)(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
    void *ptOtherArgs;
    struct LogFormat *ptNext;
} LogFormat;

/* For Out Put Ways */
typedef struct LogAppender {
    clock_t tOld;
    FncAppenderPrintf fLogOutPut;
    FncAppenderSessionStart fLogSessionStart;
    FncAppenderSessionEnd fLogSessionEnd;
    FncAppenderFree fLogAppenderFree;
    FncAppenderCtl  fLogAppenderCtl;
    int nSessionFlag;
    int nSessionStatus;
    void *ptOtherArgs;
} LogAppender;

/* Single Info */
typedef struct LogCfg {
    char sCfglName[LOG_LEVEL_NAME_LEN];
    LogFormat *ptFormat;
    LogAppender *ptLogAppender;
    T_HASH_TABLE *ptFilterHastTable;
    struct LogCfg *ptNext;
} LogCfg;

/* Index of Log Config */
typedef struct LogCfgIdx {
    char sLevelName[LOG_LEVEL_NAME_LEN];
    LogCfg *ptLogCfg;
    struct LogCfgIdx *ptNext;
} LogCfgIdx;

/* All Info */
typedef struct LogAllCfg {
    int nNum;
    LogCfgIdx *ptLogCfgIdx[LOG_LEVEL_NUMBER_DEFAULT];
    LogCfg *ptLogCfgList;
    int (*fDebugString)(struct LogAllCfg *ptLogAllCfg, char *psFile, int nLine, const char *psFunction, char *psStr, char *psStrName, int nStrLen);
    char caDebug;
} LogAllCfg;

/* Just For File Appender */
typedef struct LogAppenderFile {
    char *psLogFileName;
    FILE *ptFp;
    dev_t lDev;
    ino_t lIno;
} LogAppenderFile;

/* For Rolling File Appender */
typedef struct LogAppenderRollingFile {
    char *psLogFileName;
    FILE *ptFp;
    dev_t lDev;
    ino_t lIno;
    int iBackType;
    unsigned long lSize;
} LogAppenderRollingFile;

/* For Back Up Rolling File Appender */
typedef struct LogAppenderBackupRollingFile {
    char *psLogFileName;
    FILE *ptFp;
    dev_t lDev;
    ino_t lIno;
    int nMaxBackIndex;
    int nNowBackIndex;
    unsigned long lSize;
} LogAppenderBackupRollingFile;

/* For Daily Rolliong File Appender */
typedef struct LogAppenderDailyRollingFile {
    char *psLogFileName;
    FILE *ptFp;
    dev_t lDev;
    ino_t lIno;
    int nBackUpType;
    time_t tLastBackUpTime;
    time_t tNextBackUpTime;
} LogAppenderDailyRollingFile;

typedef struct LogFormatArgs{
    char caName;
    char *psInfo;
    int  nLen;
} LogFormatArgs;

/* plug */
typedef struct UserAppenderPlug {
    H_DllFile ptDllObject;
    FncInitUserAppender fInitAppender;
} UserAppenderPlug;

typedef struct UserAppenderDefine {
    char *psName;
    UserAppenderPlug *ptUserAppenderPlug;
    struct UserAppenderDefine *ptNext;
} UserAppenderDefine;

/*--------------------- Local function declaration --------------*/
static char * GetToken(char *psBuf, char **ppsValue, int *pnValueLen, int *pnType);
static char * AddKey(char *psOldKey, char *psNewKey);
static int logIniItemRead(char *psSection, char *psProgram, char *psType, char *psItem, char *psDefault, char *psOutBuf, int nOutLen, char *psFile);
static LogFormat * logFormatInit(char *psKey, char *psFormat);
static int logStrNSplit(char **ppsStr, char *psLine, char *psSplitChar, int nNum);
static char * logStrNDup(char *psBuf, int nLen);
static int logLevelInit(LogAllCfg *ptLogAllCfg, char *psFile, char *psCfglName, char *psKey);
static char * logGetNextFormat(char *psBuf, LogFormatArgs  *ptLogFormatArgs);
static int LogAddStringFormat(LogFormat *ptLogFormat, char *psBuf, int nLen);
static int LogAddMessageFormat(LogFormat *ptLogFormat, char *psBuf, int nLen);
static int LogAddFileForamt(LogFormat *ptLogFormat, char *psBuf, int nLen);
static int LogAddlineFormat(LogFormat *ptLogFormat, char *psBuf, int nLen);
static int LogAddFunctionFormat(LogFormat *ptLogFormat, char *psBuf, int nLen);
static int LogAddTimeFormat(LogFormat *ptLogFormat, char *psBUf, int nLen);
static int LogAddProcessId(LogFormat *ptLogFormat, char *psBuf, int nLen);
static int LogAddLevelName(LogFormat *ptLogFormat, char *psBuf, int nLen);
static int LogAddRunTime(LogFormat *ptLogFormat, char *psBuf, int nLen);
static int LogAddSession(LogFormat *ptLogFormat, char *psBuf, int nLen);
static int LogStringPrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogMessagePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogFilePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogLinePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogFunctionPrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogTimePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogProcessIdPrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogLevelNamePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogRunTimePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogSessionPrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogSamplePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogTracePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogFreeFormat(LogFormat *ptLogFormat);
static int LogFreeCfg(LogAllCfg *ptLogAllCfg);
static int LogFreeAppender(LogAppender *ptLogAppender);
static int LogInfoVArgs(LogAllCfg *ptLogAllCfg, int nLevel, char *psGroup, char *psFile, int nLine, const char *psFunction, char *psLogStr, va_list tVaList);
static int LogInfoOutPutVArgs(LogAllCfg *ptLogAllCfg, int nLevel, char *psGroup, char *psFile, int nLine, const char *psFunction, char *psLogStr, va_list tVaList, char caFormatFlag);
static int LogMakeMessage(char *psBuf, int nLen, LogFormat *ptLogFormat, LogArgs *ptLogArgs);
static int LogOutPutArgs(LogAppender *ptLogAppender, char *psBuf, ... );
static int LogOutPut(LogArgs *ptLogArgs, LogAppender *ptLogAppender, char *psBuf, va_list tVaList);
static int logAddCfg(LogAllCfg *ptLogAllCfg, LogCfg *ptLogCfg, char *psFile, char *psPkg, char *psGroup);
static int logAddCfgLevel(LogAllCfg *ptLogAllCfg, LogCfg *ptLogCfg, char *psBuf);
static int logCfgLayout(LogCfg *ptLogCfg, char *psFile, char *psPkg, char *psGroup);
static int logCfgAppender(LogCfg *ptLogCfg, char *psFile, char *psPkg, char *psGroup);
static LogAppender * logGetUserDefineAppender(char *psAppenderName, char *psFile, char *psPkg, char *psGroup);
static int logGetSessionFlag(char *psFile, char *psPkg, char *psGroup);
static int logAddCfgToAllCfgLevelIndex(LogAllCfg *ptLogAllCfg, LogCfg *ptLogCfg, char *psIndex, char *psName);
static LogFormat * logGetSampleFormat(char *psFile, char *psKey);
static LogFormat * logGetTraceFormat(char *psFile, char *psKey);
static LogFormat * logGetPatternFormat(char *psFile, char *psPkg, char *psGroup);
static LogAppender * logGetFileAppender(char *psFile, char *psPkg, char *psGroup);
static LogAppender * logGetRollingFileAppender(char *psFile, char *psPkg, char *psGroup);
static LogAppender * logNewRollingFileAppender(char *psFileName, unsigned long lSize, int iBackType);
static LogAppender * logGetBackupRollingFileAppender(char *psFile, char *psPkg, char *psGroup);
static LogAppender * logNewBackupRollingFileAppender(char *psLogFileName, unsigned long lSize, int nBackupNum);
static LogAppender * logGetDailyRollingFileAppender(char *psFile, char *psPkg, char *psGroup);
static LogAppender * logGetConsoleAppender(char *psFile, char *psPkg, char *psGroup);
static int LogFileOutPut(LogArgs *ptLogArgs, void *ptArgs, char *psBuf, va_list tVaList);
static int LogFileSessionStart(void *ptArgs);
static int LogFileRollingFileSessionStart(void *ptArgs);
static int LogFileBackupRollingFileSessionStart(void *ptArgs);
static int LogFileDailyRollingFileSessionStart(void *ptArgs);
static int LogConsoleOutPut(LogArgs *ptLogArgs, void *ptArgs, char *psBuf, va_list taVaList);
static LogAllCfg * logDefaultInit();
static int logPrintfSessionStart(char *psKey);
static int logPrintfSessionEnd(LogAppender *ptLogAppender, char *psKey);
static int logDoSessionStart(LogAppender *ptLogAppender);
static int logDoSessionEnd(LogAppender *ptLogAppender);
static int logNewDebugString(LogAllCfg *ptLogAllCfg, char *psFile, int nLine, const char *psFunction, char *psStr, char *psStrName, int nStrLen);
static int logOldDebugString(LogAllCfg *ptLogAllCfg, char *psFile, int nLine, const char *psFunction, char *psStr, char *psStrName, int nStrLen);
static int logGetDebugString(LogAllCfg *ptLogAllCfg, char *psFile, char *psGroup);
static unsigned long logGetSizeNum(char *psFile, char *psPkg, char *psGroup);
static int logGetTimePattern(char *psFile, char *psPkg, char *psGroup, int *pnBackUpType);
static int logGetBackupNum(char *psFile, char *psPkg, char *psGroup);
static int logBackUpFile(char *psFile, unsigned long lSize, int iBackType);
static int logBackUpRollingFile(char *psFileName, unsigned long lSize, int nMaxIndex, int *pnNowIndex);
static int logGetBackUpFileName(char *psFile, int iBackType, char *psBackUpFile);
static int GetLastBackupFileName(char *psFile, int nMaxIndex, int nNowIndex, char *psBackUpFileName);
static int logGetLastBackUpTime(int nBackUpType, time_t *ptLastBakcUpTime);
static int logGetNextBackUpTime(int nBackUpType, time_t *ptNextBakcUpTime);
static int logCheckNextBackUpTime(LogAppenderDailyRollingFile *ptDailyFile);
static int logCheckLastBackUpTime(LogAppenderDailyRollingFile *ptDailyFile);
static FncInitUserAppender GetUserDefineAppenderInitFunction(char *psAppenderName, char *psFile);
static UserAppenderPlug * FindUserAppenderDefine(UserAppenderDefine * ptUserAppenderDefine, char *psAppenderName);
static UserAppenderDefine * AddUserAppenderDefine(UserAppenderDefine *ptUserAppenderDefine, char *psAppenderName, UserAppenderPlug *ptUserPlug);
static UserAppenderPlug * GetInitAppenderFromCfgFile(char *psAppenderName, char *psFile);
static UserAppenderPlug * NewUserAppenderPlug(char *psFile, char *psFunction);
static int UserAppenderPlugFree(UserAppenderPlug *ptUserAppenderPlug);
static int AddLogCfgIdx(LogAllCfg *ptLogAllCfg, int nIndex, char *psName, LogCfg *ptLogCfg);
static int FreeLogCfgIdx(LogCfgIdx *ptLogCfgIdx);
static int OldLogInfoVArgs(LogAllCfg *ptLogAllCfg, char *psFileName, int nLevel, char *psFile, int nLine, const char *psFunction, char *psLogStr, va_list tVaList);
static LogAppender * logGetTempAppender(char *psFileName);
static int logGetGlobalVar(char *psCfgFileName, char *psKey, char *psBuffer, int nNum);
static int DebugStringInfo(struct LogAllCfg *ptLogAllCfg, int nLevel, char *psFile, int nLine, const char *psFunction, char *psLogStr, ...) __LOGINFO_GNUC_EXT2__;
static int logLevelAction(LogCfgIdx *ptCfgInfo, char *psName, char *psValue);
static int logLevelSetLevelName(LogCfgIdx *ptCfgInfo, char *psValue);
static int logDefaultAction(char *psName, char *psValue);
static int logCfgFilter(LogCfg *ptLogCfg, char *psFile, char *psPkg, char *psGroup);
static int logCfgReplaceFile(char *psFileName, int nNum, char *psGroup, char *psPkg, char *psKey, char *psDefault, char *psCfgFile);
static int logCfgFilterGetDetailByFile(LogCfg *ptLogCfg, FILE *ptFp);
static int * logFilterGetList(char *psValue);
static int logAddGroupFormat(LogFormat *ptLogFormat, char *psBuf, int nLen);
static int logAddSecondFormat(LogFormat *ptLogFormat, char *psBuf, int nLen);
static int LogGroupPrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int LogSecondPrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs);
static int logIsFilter(LogCfg *ptLogCfg, char *psGroup, int nLevel);
static int LogFileOpen(struct stat *ptStatBuf, void *ptArgs);
static int LogFileClose(void *ptArgs);
static int LogFileReopen(void *ptArgs);
static char * locCfgGetFileName(char *psGroup, char *psPkg, char *psCfgFile);
static void LogFileFree(void *ptArgs);
static void logRollingFileFree(void *ptArgs);
static void logBackupRollingFileFree(void *ptArgs);
static void locDailyRollingFileFree(void *ptArgs);
static void locSetHex(char *psBuf, unsigned char caChar);
static void locMemCopy(char *psOutBuf, char *psInBuf, int iLen);
static int LogIntFormat(char *psBuffer, int iLen, int iNum);
static void _lockFile(FILE *ptFp);
static void _unlockFile(FILE *ptFp);
static void _MakeDir(char *psBuf);
static FILE * _FileOpen(char *psFileName, char *psType);

/*--------------------- Global variable -------------------------*/ 
static int gnInitFlag = LOG_FALSE;
static UserAppenderDefine *gptUserAppenderDefine = NULL;
static LogAllCfg *gptLogAllCfg = NULL;
static char gsSessionKey[LOG_BUF_LEN];
static int gnKeyNum = 0;
static int giIsFlush = 1;

/* For Old InterFace */
static int gnVersion = LOG_VERSION_NEW;
static unsigned long glFileSize = LOG_SIZE_MB * 30;
static char *gpsLogFileName = "";
static char *gpsAppName = NULL;
static int gnMode = LOG_LEVEL_TRACE;
static LogAppender gtOldAppender;
static LogAppenderRollingFile gtOldRollingFile;

/* Default Value */
static char * gpsLevelDefault = LOG_LEVEL_LEVEL_DEFAULT;

#if 0
#pragma mark -
#pragma mark < Global functions >
#endif
/*--------------------- Global functions ------------------------*/
/**
 * logCfgInit
 * log cfg init, save cfg into global
 *
 * @param psFile: Cfg File Name
 * @param psName: Fucntion Name
 *
 * @return >0  : ok
 *         <0  : err
 */
int logInit(char *psFile, char *psName)
{
    /* Check Arg */
    LOG_CHECK(psFile, -1);
    LOG_CHECK(psName, -1);
    
    /* Free gptLogAllCfg If has init */
    if ( gptLogAllCfg != NULL ) {
        LogFreeCfg(gptLogAllCfg);
        gptLogAllCfg = NULL;
    }
    
    /* Init */
    gptLogAllCfg = logCfgInit(psFile, psName);
    if ( NULL == gptLogAllCfg ) {
        return (-1);
    }
    
    gnInitFlag = LOG_TRUE;
    return (0);
}

/**
 * logInit
 * Init log config
 *
 * @param psFile: log config info
 *
 * @return >0  : ok
 *         <0  : err
 */
LogAllCfg * logCfgInit(char *psFile, char *psName)
{
    int nRet;
    int i;
    
    /* Check Args */
    LOG_CHECK(psFile, NULL);
    LOG_CHECK(psName, NULL);

    memset(gsSessionKey, '\0', sizeof(gsSessionKey));

    /* Init LogCfg List, Default num is LOG_LEVEL_NUMBER */
    LogAllCfg *ptLogAllCfg = (LogAllCfg *)malloc(sizeof(LogAllCfg));
    memset(ptLogAllCfg, '\0', sizeof(LogAllCfg));
    ptLogAllCfg->nNum = LOG_LEVEL_NUMBER_DEFAULT;
    ptLogAllCfg->ptLogCfgList = NULL;

    /* Get All Config Name List */
    char sCfgBuf[LOG_BUF_LEN];
    memset(sCfgBuf, '\0', sizeof(sCfgBuf));
    logIniItemRead(LOG_SECTION_NAME, NULL, NULL, LOG_SECTION_NAME, LOG_LEVEL_CFG_DEFAULT, sCfgBuf, LOG_BUF_LEN, psFile);
    logIniDefRead(psName, NULL, NULL, sCfgBuf, sCfgBuf, LOG_BUF_LEN, psFile);

    /* Split Config list String */
    char *ppsLevelList[LOG_LEVEL_NUMBER_DEFAULT];
    int nListNum = logStrNSplit(ppsLevelList, sCfgBuf, ",", LOG_LEVEL_NUMBER_DEFAULT);
    if ( nListNum <= 0 ) {
        logLog("Cfg[%s] File[%s] Str Get Config Name err!!\n", psName, psFile);
        goto ERR;
    }

    /* Init every Config */
    for ( i=0 ; i < nListNum; ++i) {
        if ( '\0' == ppsLevelList[i][0] ) {
            continue;
        }

        nRet = logLevelInit(ptLogAllCfg, psFile, ppsLevelList[i], psName);
        if ( nRet != 0 ) {
            logLog("Init Level [%s] err!!\n", ppsLevelList[i]);
            goto ERR;
        }

    }

    /* Get Log Debug String Mode */
    nRet = logGetDebugString(ptLogAllCfg, psFile, psName);
    if ( nRet != 0 ) {
        logLog("loggetDebugString err!!\n");
        goto ERR;
    }
    
    return (ptLogAllCfg);
    
ERR:
    LogFreeCfg(ptLogAllCfg);
    
    return (NULL);
}

/**
 * logEnd
 * Free All Log Info
 *
 * @param #: void
 *
 * @return >0  : ok
 *         <0  : err
 */
int logEnd(void)
{
    LogFreeCfg(gptLogAllCfg);
    gptLogAllCfg = NULL;
    gnInitFlag = LOG_FALSE;
    
    return (0);
}

/**
 * logCfgEnd
 * Free logAll Cfg
 *
 * @param ptLogAllCfg: void
 *
 * @return >0  : ok
 *         <0  : err
 */
int logCfgEnd(LogAllCfg *ptLogAllCfg)
{
    LogFreeCfg(ptLogAllCfg);
    
    return (0);
}

/**
 * logCtl
 * Ctl defualt cfg
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int logCtl(int nLevel, char *psName, char *psValue)
{
    return ( logCfgCtl(gptLogAllCfg, nLevel, psName, psValue) );
}

/**
 * logCfgCtl
 * Cfg Ctl All Info
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int logCfgCtl(LogAllCfg *ptLogAllCfg, int nLevel, char *psName, char *psValue)
{
    int nRet = 0;
    
    if ( LOG_DEFAULT == nLevel ) {
        return ( logDefaultAction(psName, psValue) );
    }

    if ( nLevel < 0 || nLevel >= ptLogAllCfg->nNum ) {
        logLog("nLevel[%d] is too big [%d]\n", nLevel, ptLogAllCfg->nNum);
        return (-1);
    }

    if ( NULL ==  ptLogAllCfg->ptLogCfgIdx[nLevel] ) {
        return (0);
    }
    
    logLevelAction(ptLogAllCfg->ptLogCfgIdx[nLevel], psName, psValue);
    
    return ( nRet );
}

/**
 * logInfo
 * printf log info
 * use global log info
 *
 * @param psLogFile: NOT USE
 * @param nLevel: Log Level
 * @param psLevelStr: NOT USE
 * @param psFile: source File Name
 * @param nLine: source File Line
 * @param psFunction: source Function Name
 * @param psLogStr: Log Message
 * @param ...: Log Message Args
 *
 * @return >0  : ok
 *         <0  : err
 */
int logInfo(char *psLogFile, int nLevel, char *psLevelStr, char *psFile, int nLine, const char *psFunction, char *psLogStr, ...)
{
    int nRet;
    
    /* if Not Init, Get Default Config */
    if ( LOG_FALSE == gnInitFlag ) {
        gnInitFlag = LOG_TRUE;
        gptLogAllCfg = logDefaultInit();
    }
    
	va_list tVaList;
	va_start(tVaList, psLogStr);
    if ( psLogFile != NULL ) {
        nRet= OldLogInfoVArgs(gptLogAllCfg, psLogFile, nLevel, psFile, nLine, psFunction, psLogStr, tVaList);
    } else { 
        nRet= LogInfoVArgs(gptLogAllCfg, nLevel, psLevelStr, psFile, nLine, psFunction, psLogStr, tVaList);
    }

    va_end(tVaList);
    
    return ( nRet );
}

/**
 * logInfo
 * printf log info
 * use global log info
 *
 * @param psLogFile: NOT USE
 * @param nLevel: Log Level
 * @param psLevelStr: NOT USE
 * @param psFile: source File Name
 * @param nLine: source File Line
 * @param psFunction: source Function Name
 * @param psLogStr: Log Message
 * @param ...: Log Message Args
 *
 * @return >0  : ok
 *         <0  : err
 */
int logInfoNoFront(char *psLogFile, int nLevel, char *psLevelStr, char *psFile, int nLine, const char *psFunction, char *psLogStr, ...)
{
    int nRet;

    /* if Not Init, Get Default Config */
    if ( LOG_FALSE == gnInitFlag ) {
        gnInitFlag = LOG_TRUE;
        gptLogAllCfg = logDefaultInit();
    }

	va_list tVaList;
	va_start(tVaList, psLogStr);
    nRet = LogInfoOutPutVArgs(gptLogAllCfg, nLevel, psLevelStr, psFile, nLine, psFunction, psLogStr, tVaList, LOG_FORMAT_NO_FORMAT);
    va_end(tVaList);

    return ( nRet );
}

/**
 * logVInfo
 * FOr Old Inter Face
 *
 * @param psLogFile: NOT USE
 * @param nLevel: Log Level
 * @param psLevelStr: NOT USE
 * @param psFile: source File Name
 * @param nLine: source File Line
 * @param psFunction: source Function Name
 * @param psLogStr: Log Message
 * @param ...: Log Message Args
 *
 * @return >0  : ok
 *         <0  : err
 */
int logVInfo(char *psLogFile, int nLevel, char *psLevelStr, char *psFile, int nLine, const char *psFunction, char *psLogStr, va_list ap)
{
    int nRet;
    
    /* if Not Init, Get Default Config */
    if ( LOG_FALSE == gnInitFlag ) {
        gnInitFlag = LOG_TRUE;
        gptLogAllCfg = logDefaultInit();
    }
    
    if ( psLogFile != NULL ) {
        nRet= OldLogInfoVArgs(gptLogAllCfg, psLogFile, nLevel, psFile, nLine, psFunction, psLogStr, ap);
    } else {
        nRet= LogInfoVArgs(gptLogAllCfg, nLevel, psLevelStr, psFile, nLine, psFunction, psLogStr, ap);
    }
    
    return (nRet);
}

/**
 * logVInfo
 * FOr Old Inter Face
 *
 * @param psLogFile: NOT USE
 * @param nLevel: Log Level
 * @param psLevelStr: NOT USE
 * @param psFile: source File Name
 * @param nLine: source File Line
 * @param psFunction: source Function Name
 * @param psLogStr: Log Message
 * @param ...: Log Message Args
 *
 * @return >0  : ok
 *         <0  : err
 */
int logVInfoNoFront(char *psLogFile, int nLevel, char *psLevelStr, char *psFile, int nLine, const char *psFunction, char *psLogStr, va_list ap)
{
    int nRet;

    /* if Not Init, Get Default Config */
    if ( LOG_FALSE == gnInitFlag ) {
        gnInitFlag = LOG_TRUE;
        gptLogAllCfg = logDefaultInit();
    }

    nRet= LogInfoOutPutVArgs(gptLogAllCfg, nLevel, psLevelStr, psFile, nLine, psFunction, psLogStr, ap, LOG_FORMAT_NO_FORMAT);

    return (nRet);
}

/**
 * logCfgInfo
 * Printf Log Message In Format
 *
 * @param ptLogCfg: Log Config Info
 * @param nLevel: Log Level
 * @param psFile: source File Name
 * @param nLine: source File Line
 * @param psFunction: source Function Name
 * @param psLogStr: Log Message
 * @param ...: Log Message Args
 *
 * @return >0  : ok
 *         <0  : err
 */
int logCfgInfo(LogAllCfg *ptLogAllCfg, int nLevel, char *psFile, int nLine, const char *psFunction, char *psLogStr, ...)
{
    int nRet;
	
    /* Check */
    LOG_CHECK(ptLogAllCfg, -1);
    
	va_list tVaList;
	va_start(tVaList, psLogStr);
    nRet= LogInfoVArgs(ptLogAllCfg, nLevel, NULL, psFile, nLine, psFunction, psLogStr, tVaList);
    va_end(tVaList);
    
    return ( nRet );
}

/**
 * logSession
 * log Session Use globle config
 * just invork logCfgSession
 *
 * @param psKey: Session Key
 *
 * @return >0  : ok
 *         <0  : err
 */
int logSession(char *psKey)
{
    /* if Not Init, Get Default Config */
    if ( LOG_FALSE == gnInitFlag ) {
        gnInitFlag = LOG_TRUE;
        gptLogAllCfg = logDefaultInit();
    }
    
    return ( logCfgSession(gptLogAllCfg, psKey) );
}

/**
 * logCfgSession
 * log Sesion Function
 * just invork the logCfgSessionEnd And logCfgSessionStart
 *
 * @param ptLogAllCfg: Log All Config Info
 * @param psKey: Session Key
 *
 * @return >0  : ok
 *         <0  : err
 */
int logCfgSession(LogAllCfg *ptLogAllCfg, char *psKey)
{
    int nRet;
    
    /* Check */
    LOG_CHECK(ptLogAllCfg, -1);
    
    nRet = logCfgSessionEnd(ptLogAllCfg, psKey);
    if ( nRet != 0 ) {
        return (nRet);
    }
    
    nRet = logCfgSessionStart(ptLogAllCfg, psKey);
    if ( nRet != 0 ) {
        return (nRet);
    }
    
    return (0);
}

/**
 * logSessionStart
 * Just Invork the logCfgSessionStart by default gptLogAllCfg
 *
 * @param psKey: Session Key
 *
 * @return >0  : ok
 *         <0  : err
 */
int logSessionStart(char *psKey)
{
    /* if Not Init, Get Default Config */
    if ( LOG_FALSE == gnInitFlag ) {
        gnInitFlag = LOG_TRUE;
        gptLogAllCfg = logDefaultInit();
    }

    return ( logCfgSessionStart(gptLogAllCfg, psKey) );
}

/**
 * logCfgSessionStart
 * Do session Start
 * Check Session Flag
 * Off : Do nothing
 * On : Do session start function; printf Session Head; 
 * Noprintf: Do session start function
 *
 * @param ptLogAllCfg: All Config
 * @param psKey: Session Key
 *
 * @return >0  : ok
 *         <0  : err
 */
int logCfgSessionStart(LogAllCfg *ptLogAllCfg, char *psKey)
{
    int nRet;
    
    /* Check Args */
    LOG_CHECK(ptLogAllCfg, -1);

    nRet = logPrintfSessionStart(psKey);
    if ( nRet != 0 ) {
        logLog("Printf Session Start Err!!\n");
    }

    /* Do All Appender Session Function */
    LogCfg *ptLogCfg = ptLogAllCfg->ptLogCfgList;
    for ( ; ptLogCfg!=NULL ; ptLogCfg=ptLogCfg->ptNext) {
        switch ( ptLogCfg->ptLogAppender->nSessionFlag ) {
            case LOG_SESSION_FLAG_OFF:
                break;
            
            case LOG_SESSION_FLAG_ON:
                nRet = logDoSessionStart(ptLogCfg->ptLogAppender);
                if ( nRet != 0 ) {
                    logLog("DoSessionStart Err!!\n");
                }

                char sBuf[LOG_BUF_LEN];
                memset(sBuf, '\0', sizeof(sBuf));
                time_t tSystime = time(NULL);
                struct tm *ptTm  = localtime(&tSystime);
                strftime(sBuf, LOG_BUF_LEN, "%Y/%m/%d %H:%M:%S", ptTm);
                LogOutPutArgs(ptLogCfg->ptLogAppender, "\n\n\n====SESSION START :[%s] TIME [%s]====\n", gsSessionKey, sBuf);
                break;
                
            case LOG_SESSION_FLAG_NOPRINT:
                nRet = logDoSessionStart(ptLogCfg->ptLogAppender);
                if ( nRet != 0 ) {
                    logLog("DoSessionStart Err!!\n");
                }
                break;
                
            default:
                logLog("Cfg [%s]undefined session flag[%d] !!\n",  ptLogCfg->sCfglName, ptLogCfg->ptLogAppender->nSessionFlag);
                /* Not Return, continue next session function */
                break;
        }
    }
    
    return (0);
}

/**
 * logSessionEnd
 * Just Invork the logCfgSessionEnd by gptLogAllCfg
 *
 * @param psKey: Session Key
 *
 * @return >0  : ok
 *         <0  : err
 */
int logSessionEnd(char *psKey)
{
    /* if Not Init, Get Default Config */
    if ( LOG_FALSE == gnInitFlag ) {
        gnInitFlag = LOG_TRUE;
        gptLogAllCfg = logDefaultInit();
    }
    
    return ( logCfgSessionEnd(gptLogAllCfg, psKey) );
}

/**
 * logCfgSessionEnd
 * Session End Funciton
 * Check Session Flag
 * Off : Do nothing
 * On : printf Session End; Do session End function;
 * Noprintf: Do session End function;
 *
 * @param ptLogAllCfg: All Log Cnfig Info
 * @param psKey: Session Key
 *
 * @return >0  : ok
 *         <0  : err
 */
int logCfgSessionEnd(LogAllCfg *ptLogAllCfg, char *psKey)
{
    int nRet;
    
    /* Check Args */
    LOG_CHECK(ptLogAllCfg, -1);
    
    /* Do All Appender Session Function */
    LogCfg *ptLogCfg = ptLogAllCfg->ptLogCfgList;
    for ( ; ptLogCfg!=NULL ; ptLogCfg=ptLogCfg->ptNext) {
        switch ( ptLogCfg->ptLogAppender->nSessionFlag ) {
            case LOG_SESSION_FLAG_OFF:
                break;
                
            case LOG_SESSION_FLAG_ON:
                nRet = logPrintfSessionEnd(ptLogCfg->ptLogAppender, psKey);
                if ( nRet != 0 ) {
                    logLog("Printf Session End Err!!\n");
                }
                
                nRet = logDoSessionEnd(ptLogCfg->ptLogAppender);
                if ( nRet != 0 ) {
                    logLog("DoSessionEnd Err!!\n");
                }
                break;
                
            case LOG_SESSION_FLAG_NOPRINT:
                nRet = logDoSessionEnd(ptLogCfg->ptLogAppender);
                if ( nRet != 0 ) {
                    logLog("DoSessionEnd Err!!\n");
                }
                break;
                
            default:
                logLog("[%s] undefined session flag[%d] !!\n", ptLogCfg->sCfglName, ptLogCfg->ptLogAppender->nSessionFlag);
                /* Do not return , continue do session */
                break;
        }
    }
    
    return (0);
}

/**
 * logDebugString
 * Juse invork the function logCfgDebugString
 *
 * @param psLogFile: Is Not Null, printf log to Log File
 * @param nFormat: NOT USE Just For Old interface
 * @param psFile: Source File name
 * @param nLine: Source File Num
 * @param psFunction: Source File Function
 * @param psStr: Source String
 * @param psStrName: Source String Name
 * @param nStrLen: Source String Line
 *
 * @return >0  : ok
 *         <0  : err
 */
int logDebugString(char *psLogFile, int nFormat, char *psFile, int nLine, const char *psFunction, char *psStr, char *psStrName, int nStrLen)
{
    /* if Not Init, Get Default Config */
    if ( LOG_FALSE == gnInitFlag ) {
        gnInitFlag = LOG_TRUE;
        gptLogAllCfg = logDefaultInit();
    }
    
    LogAppender *ptOrgAppender;
    if ( psLogFile != NULL ) {
        /* Back Org Appender */
        ptOrgAppender = gptLogAllCfg->ptLogCfgList->ptLogAppender;
        gptLogAllCfg->ptLogCfgList->ptLogAppender = logGetTempAppender(psLogFile);
    }
    
    int nRet = logCfgDebugString(gptLogAllCfg, psFile, nLine, psFunction, psStr, psStrName, nStrLen);
    
    if ( psLogFile != NULL ) {
        /* restore Org Appender */
        gptLogAllCfg->ptLogCfgList->ptLogAppender = ptOrgAppender;
    }
    
    return (nRet);
}

/**
 * logDebugString
 * Printf String Hex Mode
 *
 * @param ptLogAllCfg: All Log Info
 * @param psFile: Source File name
 * @param nLine: Source File Line
 * @param psFunction: Source File Fucntion Name
 * @param psStr: Debug String Buf
 * @param psStrName: Debug String Name
 * @param nStrLen: Debug String Buf Line
 *
 * @return >0  : ok
 *         <0  : err
 */
int logCfgDebugString(LogAllCfg *ptLogAllCfg, char *psFile, int nLine, const char *psFunction, char *psStr, char *psStrName, int nStrLen)
{
    int nRet;
    
    /* CHECK */
    LOG_CHECK(ptLogAllCfg, -1);
    LOG_CHECK(psStr, -1);
    
    /* Check Enable */
    if ( NULL == ptLogAllCfg->ptLogCfgIdx[LOG_LEVEl_DEBUG_STRING] ) {
        return (0);
    }
    
    /* Invroke the function */
    if ( ptLogAllCfg->fDebugString != NULL ) {
        if ( LOG_DEBUG_PRINT_NO == ptLogAllCfg->caDebug ) {
            logCfgInfo(ptLogAllCfg, LOG_LEVEl_DEBUG_STRING, psFile, nLine, psFunction, "Debug String [%s[%d]] ...", psStrName, nStrLen);
        }
        
        nRet = ptLogAllCfg->fDebugString(ptLogAllCfg, psFile, nLine, psFunction, psStr, psStrName, nStrLen);
        if ( nRet != 0 ) {
            logLog("invork Debug String Err!!\n");
            return (-1);
        }
    }
    
	return ( 0 );
}

#if 0
#pragma mark -
#pragma mark < plug >
#endif

int logInitAppnederSetSession(LogAppenderPoint ptAppender, int iSessionFlag)
{
    if (NULL == ptAppender) {
        return (-1);
    }

    ptAppender->nSessionFlag = iSessionFlag;

    return (0);
}

/**
 * logInitAppender
 * Add appender ctl
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int logInitAppenderAddCtl(LogAppenderPoint ptAppender, FncAppenderCtl fAppenderCtl)
{
    if ( NULL == ptAppender ) {
        return (-1);
    }
    
    ptAppender->fLogAppenderCtl = fAppenderCtl;
    return (0);
}

/**
 * logInitAppender
 * Init log Appender
 * Add Below Function To Appender
 *
 * @param ptAppender: Tager Appender
 * @param fAppenderPrintf: Printf Function
 * @param fAppenderSessionStart: Session Start Function
 * @param fAppenderSessionEnd: Session End Function
 * @param fAppenderFree: Free Fucntion
 * @param ptOtherArgs: Other Args Info
 *
 * @return >0  : ok
 *         <0  : err
 */
int logInitAppender(LogAppender *ptAppender, 
                    FncAppenderPrintf fAppenderPrintf,
                    FncAppenderSessionStart fAppenderSessionStart,
                    FncAppenderSessionEnd fAppenderSessionEnd,
                    FncAppenderFree fAppenderFree, 
                    void *ptOtherArgs)
{
    if ( NULL == ptAppender ) {
        return (-1);
    }
    
    ptAppender->fLogOutPut = fAppenderPrintf;
    ptAppender->fLogSessionStart = fAppenderSessionStart;
    ptAppender->fLogSessionEnd = fAppenderSessionEnd;
    ptAppender->fLogAppenderFree = fAppenderFree;
    ptAppender->ptOtherArgs = ptOtherArgs;

    return (0);
}

/**
 * logIniRead
 * log Info Get Ini info
 *
 * @param psKey: Key Name
 * @param psItem: Ini Item Name
 * @param psDefault: Default String Value
 * @param psOutBuf: return buf 
 * @param pnOutLen: return buf len
 *
 * @return >0  : ok
 *         <0  : err
 */
int logIniDefRead(char *psGroup, char *psPkg, char *psKey, char *psDefault, char *psOutBuf, int nOutLen, char *psFile)
{
    int nRet;
    
    /* Check Err */
    LOG_CHECK(psOutBuf, -1);
    
    /* Get Default Value form Ini */
    nRet = logIniItemRead(LOG_SECTION_NAME, NULL, psPkg, psKey, psDefault, psOutBuf, nOutLen, psFile);
    if ( nRet < 0 ) {
        return (0);
    }
    
    /* If No Group Not Need Next */
    if ( NULL == psGroup ) {
        return (0);
    }

    /* Get Rale Value Form Ini */
    nRet = logIniItemRead(LOG_SECTION_NAME, psGroup, psPkg, psKey, psOutBuf, psOutBuf, nOutLen, psFile);
    if ( nRet < 0 ) {
        return (0);
    }
    
    /* Get Secion Value */
    nRet = logIniItemRead(psGroup, LOG_SECTION_NAME, psPkg, psKey, psOutBuf, psOutBuf, nOutLen, psFile);
    if ( nRet < 0 ) {
        return (0);
    }
    
    return (0);
}

/**
 * logIniDefReadInt
 * read int init info
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
int logIniDefReadInt(char *psGroup, char *psPkg, char *psKey, int nDefault, char *psFile)
{
    char sDefault[LOG_MINI_BUF];
    sprintf(sDefault, "%d", nDefault);
    
    char sInt[LOG_MINI_BUF];
    memset(sInt, '\0', sizeof(sInt));
    logIniDefRead(psGroup, psPkg, psKey, sDefault, sInt, sizeof(sInt), psFile);
    
    return ( atoi(sInt) );
}

/**
 * logCfgGetFileName
 * Get FIle Name Full Path
 * Is File Is Not Current Path
 * Get prefix from generate
 *
 * @param psGroup: Group Name
 * @param psPkg: Pkg Name
 * @param sFileName: FIle Buffer
 * @param nNum: Buffer Num
 * @param psCfgFile: Config FIle Name
 *
 * @return >0  : ok
 *         <0  : err
 */
int logCfgGetFileName(char *psGroup, char *psPkg, char *sFileName, int nNum, char *psCfgFile)
{
    return ( logCfgReplaceFile(sFileName, nNum, psGroup, psPkg, LOG_LEVEL_FILE_KEY, LOG_LEVEL_FILE_DEFAULT, psCfgFile) );
}

#if 0
#pragma mark -
#pragma mark < Old InterFace >
#endif

/**
 * logSetFile
 * Old InterFace, Not Use Again
 * Set Log File Name
 *
 * @param psLogFileName: File Name
 *
 * @return >0  : ok
 *         <0  : err
 */
int logSetFile(char *psLogFileName)
{
    gnVersion = LOG_VERSION_OLD;
    gpsLogFileName = psLogFileName;
    
    return (0);
}

/**
 * logSetMode
 * Old Inter Face, Not Use Again
 * Set Log Show Mode
 *
 * @param pnMode: Mode Level
 *
 * @return >0  : ok
 *         <0  : err
 */
int logSetMode(int *pnMode)
{
    gnVersion = LOG_VERSION_OLD;
    gnMode = *pnMode;
    
    return (0);
}

/**
 * SetMaxFileSize
 * Old INterface, not used again
 * Set File max Size
 *
 * @param nMaxSize: Max Size
 *
 * @return >0  : ok
 *         <0  : err
 */
int SetMaxFileSize(int nMaxSize)
{
    gnVersion = LOG_VERSION_OLD;
    glFileSize = nMaxSize;
    
    return (0);
}

/**
 * logGetSeesionId
 * Old Interface, not use again
 * Get Session Key
 *
 * @param psVaule: Buf
 * @param nLen: Buf Size
 *
 * @return >0  : ok
 *         <0  : err
 */
int logGetSeesionId(char *psVaule, int nLen)
{
    snprintf(psVaule, nLen-1, "%s", gsSessionKey);
    
    return (0);
}

/**
 * logGetLogFile
 * Old Inter Face, Not Use Again
 * Get Log File Name
 *
 * @param psVaule: Buf 
 * @param nLen: Buf Len
 *
 * @return >0  : ok
 *         <0  : err
 */
int logGetLogFile(char *psVaule, int nLen)
{
    strncpy(psVaule, gpsLogFileName, nLen-1);
    
    return (0);
}

/**
 *  Set app name to log global
 *
 *  @param psName app name
 *
 *  @return 0
 */
int logSetAppName(char *psName)
{
    if (NULL != gpsAppName) {
        free(gpsAppName);
    }

    gpsAppName = strdup(psName);

    return (0);
}

char * logGetAppName(void)
{
    if (NULL == gpsAppName) {
        return "";
    }

    return gpsAppName;
}

/**
 * logSetFile
 * Set Log File Name
 * Old Interface Not Use
 * also set global config to file appender
 *
 * @param psLogFileName: File Name
 *
 * @return >0  : ok
 *         <0  : err
 */
int logSetConfig(char *psLogFileName, long nLogFileSize, int nLogMode)
{
    LOG_CHECK(psLogFileName, -1);
    
    /* if Not Init, Get Default Config */
    gnVersion = LOG_VERSION_OLD;
    if ( LOG_TRUE == gnInitFlag ) { 
        logEnd();
    }
    gnInitFlag = LOG_TRUE;
    
    /* For File Appender */
    LogFormat *ptLogFormat = (LogFormat *)malloc(sizeof(LogFormat));
    memset(ptLogFormat, '\0', sizeof(LogFormat));
    ptLogFormat->fLogPrintf  = LogSamplePrintf;
    ptLogFormat->ptOtherArgs = NULL;
    ptLogFormat->ptNext      = NULL;
    
    /* Appender Info */
    LogAppenderRollingFile *ptLogRollingFile = (LogAppenderRollingFile *)malloc(sizeof(LogAppenderRollingFile));
    memset(ptLogRollingFile, '\0', sizeof(LogAppenderRollingFile));
    ptLogRollingFile->psLogFileName = strdup(psLogFileName);
    ptLogRollingFile->lSize = nLogFileSize;
    ptLogRollingFile->iBackType = LOG_LEVEL_TIME_PATTERN_MINUTES_NUM;
    
    /* New Appender */
    LogAppender *ptLogAppender = (LogAppender *)malloc(sizeof(LogAppender));
    memset(ptLogAppender, '\0', sizeof(LogAppender));
    ptLogAppender->tOld = 0;
    ptLogAppender->fLogOutPut = LogFileOutPut;
    ptLogAppender->fLogSessionStart = LogFileRollingFileSessionStart;
    ptLogAppender->fLogSessionEnd = NULL;
    ptLogAppender->fLogAppenderFree = logRollingFileFree;
    ptLogAppender->nSessionFlag = LOG_SESSION_FLAG_ON;
    ptLogAppender->nSessionStatus = LOG_SESSION_STATUS_END;
    ptLogAppender->ptOtherArgs = (void *)ptLogRollingFile;
    
    /* Set Cfg */
    LogCfg *ptLogCfg = (LogCfg *)malloc(sizeof(LogCfg));
    memset(ptLogCfg, '\0', sizeof(LogCfg));
    ptLogCfg->ptFormat      = ptLogFormat;
    ptLogCfg->ptLogAppender = ptLogAppender;
    ptLogCfg->ptNext        = NULL;
    strcpy(ptLogCfg->sCfglName, LOG_LEVEL_CFG_DEFAULT);
    
    /* Set All Cfg */
    gptLogAllCfg = (LogAllCfg *)malloc(sizeof(LogAllCfg));
    memset(gptLogAllCfg, '\0', sizeof(LogAllCfg));
    gptLogAllCfg->nNum         = LOG_LEVEL_NUMBER_DEFAULT;
    gptLogAllCfg->ptLogCfgList = ptLogCfg;
    gptLogAllCfg->fDebugString = logNewDebugString;
    gptLogAllCfg->caDebug      = LOG_DEBUG_PRINT_DEFAULT;
    
    /* Set Log Level */
    if ( nLogMode < LOG_LEVEl_DEBUG_STRING ) {
        return (0);
    }
    AddLogCfgIdx(gptLogAllCfg, LOG_LEVEl_DEBUG_STRING, LOG_LEVEl_DEBUG_STRING_NAME, ptLogCfg);
    
    if ( nLogMode < LOG_LEVEL_ERROR ) {
        return (0);
    }
    AddLogCfgIdx(gptLogAllCfg, LOG_LEVEL_ERROR, LOG_LEVEL_ERROR_NAME, ptLogCfg);
    
    if ( nLogMode < LOG_LEVEL_WARNING ) {
        return (0);
    }
    AddLogCfgIdx(gptLogAllCfg, LOG_LEVEL_WARNING, LOG_LEVEL_WARNING_NAME, ptLogCfg);
    
    if ( nLogMode < LOG_LEVEL_DEBUG ) {
        return (0);
    }
    AddLogCfgIdx(gptLogAllCfg, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_NAME, ptLogCfg);
    
    if ( nLogMode < LOG_LEVEL_NORMAL ) {
        return (0);
    }
    AddLogCfgIdx(gptLogAllCfg, LOG_LEVEL_NORMAL, LOG_LEVEL_NORMAL_NAME, ptLogCfg);
    
    if ( nLogMode < LOG_LEVEL_TRACE ) {
        return (0);
    }
    AddLogCfgIdx(gptLogAllCfg, LOG_LEVEL_TRACE, LOG_LEVEL_TRACE_NAME, ptLogCfg);
    
    return ( 0 );
}

#if 0
#pragma mark -
#pragma mark < Local functions >
#endif
/*--------------------- Local functions -------------------------*/
/**
 * logVInfo
 * log printf Message
 * get all args info
 * Make the Format Message
 * Invorke the printf function
 *
 * @param ptLogCfg: Log Config Info
 * @param nLevel: Log Level
 * @param psFile: Log source File name
 * @param nLine: Log Source File Name
 * @param psFunction: Log Source File Fucntion
 * @param psLogStr: Log Message
 * @param tVaList: Log Message Args
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogInfoVArgs(LogAllCfg *ptLogAllCfg, int nLevel, char *psGroup, char *psFile, int nLine, const char *psFunction, char *psLogStr, va_list tVaList)
{
    return LogInfoOutPutVArgs(ptLogAllCfg, nLevel, psGroup, psFile, nLine, psFunction, psLogStr, tVaList, LOG_FORMAT_NEED_FORMAT);
}

static int LogInfoOutPutVArgs(LogAllCfg *ptLogAllCfg, int nLevel, char *psGroup, char *psFile, int nLine, const char *psFunction, char *psLogStr, va_list tVaList, char caFormatFlag)
{
    int nRet;

    /* TODO:Check Args */
    if ( nLevel < 0 || nLevel >= ptLogAllCfg->nNum ) {
        logLog("[%s:%d] nLevel[%d] is < 0 || >= %d!!\n", psFile, nLine, nLevel, ptLogAllCfg->nNum);
        return (-1);
    }

    if ( NULL == ptLogAllCfg->ptLogCfgIdx[nLevel] ) {
        return (0);
    }

    /* Get Args */
    LogArgs tLogArgs;
    tLogArgs.nLevel      = nLevel;
    tLogArgs.nLine       = nLine;
    tLogArgs.psFile      = psFile;
    tLogArgs.psFunction  = psFunction;
    tLogArgs.psLogStr    = psLogStr;
    tLogArgs.psGroupName = NULL;

    /* CHeck Group */
    if ( NULL != psGroup && LOG_GROUP_FLAG[0] == psGroup[0] ) {
        tLogArgs.psGroupName = psGroup + 1;
    }

    LogCfgIdx *p = ptLogAllCfg->ptLogCfgIdx[nLevel];
    for (; p!=NULL; p=p->ptNext) {
        tLogArgs.psLevelStr = p->sLevelName;

        if ( logIsFilter(p->ptLogCfg, tLogArgs.psGroupName, nLevel) ) {
            continue;
        }

        /* Make Message String */
        char sBuf[LOG_MSG_BUF_LEN+1];
        if (LOG_FORMAT_NEED_FORMAT == caFormatFlag) {
            nRet = LogMakeMessage(sBuf, LOG_MSG_BUF_LEN, p->ptLogCfg->ptFormat, &(tLogArgs));
            if ( nRet != 0 ) {
                return (-1);
            }
        } else {
            strcpy(sBuf, psLogStr);
        }

        /* Printf Message */
        va_list ap;
        va_copy(ap, tVaList);
        nRet = LogOutPut(&(tLogArgs), p->ptLogCfg->ptLogAppender, sBuf, ap);
        if ( nRet != 0 ) {
            logLog("Config[%s] printf err!!\n", p->ptLogCfg->sCfglName);
            return (-1);
        }
        va_end(ap);
    }

    return (0);
}

/**
 * OldLogInfoVArgs
 * For Old Function InterFace
 * Back up Old Appender, instell of Temp Appender
 *
 * @param ptLogCfg: Log Config Info
 * @param nLevel: Log Level
 * @param psFile: Log source File name
 * @param nLine: Log Source File Name
 * @param psFunction: Log Source File Fucntion
 * @param psLogStr: Log Message
 * @param tVaList: Log Message Args
 *
 * @return >0  : ok
 *         <0  : err
 */
static int OldLogInfoVArgs(LogAllCfg *ptLogAllCfg, char *psFileName, int nLevel, char *psFile, int nLine, const char *psFunction, char *psLogStr, va_list tVaList)
{
    /* TODO:Check Args */
    if ( nLevel < 0 || nLevel >= ptLogAllCfg->nNum ) {
        logLog("[%s:%d] nLevel[%d] is < 0 || >= %d!!\n", psFile, nLine, nLevel, ptLogAllCfg->nNum);
        return (-1);
    }
    
    if ( NULL == ptLogAllCfg->ptLogCfgIdx[nLevel] ) {
        return (0);
    }
    
    /* Back Org Appender */
    LogAppender *ptOrgAppender = ptLogAllCfg->ptLogCfgIdx[nLevel]->ptLogCfg->ptLogAppender;
    LogCfgIdx *ptLogCfgIdx = ptLogAllCfg->ptLogCfgIdx[nLevel]->ptNext;
    
    /* Set Num */
    ptLogAllCfg->ptLogCfgIdx[nLevel]->ptLogCfg->ptLogAppender = logGetTempAppender(psFileName);
    ptLogAllCfg->ptLogCfgIdx[nLevel]->ptNext = NULL;
    
    int nRet = LogInfoVArgs(ptLogAllCfg, nLevel, NULL, psFile, nLine, psFunction, psLogStr, tVaList);
    
    /* restore Org Appender */
    ptLogAllCfg->ptLogCfgIdx[nLevel]->ptLogCfg->ptLogAppender = ptOrgAppender;
    ptLogAllCfg->ptLogCfgIdx[nLevel]->ptNext = ptLogCfgIdx;
    return ( nRet );
}

/**
 * logGetTempAppender
 * Get Temp Appender Info
 *
 * @param psFileName: File Name
 *
 * @return >0  : ok
 *         NULL: err
 */
static LogAppender * logGetTempAppender(char *psFileName)
{
    /* Make Temp Appender */
    gtOldRollingFile.psLogFileName = psFileName;
    gtOldRollingFile.lSize = glFileSize;
    gtOldRollingFile.iBackType = LOG_LEVEL_TIME_PATTERN_MINUTES_NUM;

    if (NULL != gtOldRollingFile.ptFp) {
        fclose(gtOldRollingFile.ptFp);
    }
    gtOldRollingFile.ptFp = NULL;
    gtOldRollingFile.lIno = 0;
    gtOldRollingFile.lDev = 0;

    gtOldAppender.tOld             = 0;
    gtOldAppender.fLogOutPut       = LogFileOutPut;
    gtOldAppender.fLogSessionStart = LogFileRollingFileSessionStart;
    gtOldAppender.fLogSessionEnd   = NULL;
    gtOldAppender.fLogAppenderFree = NULL;
    gtOldAppender.nSessionFlag     = LOG_SESSION_FLAG_OFF;
    gtOldAppender.nSessionStatus   = LOG_SESSION_STATUS_END;
    gtOldAppender.ptOtherArgs      = (void *)(&gtOldRollingFile);
    
    return ( &(gtOldAppender) );
}

/**
 * LogMakeMessage
 * Make Format String
 * Check Buf Len if is zero then return
 *
 * @param psBuf: Message Buf   
 * @param nLen: Message Buf Len
 * @param ptLogFormat: Format Info
 * @param ptLogArgs: Log Args Info
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogMakeMessage(char *psBuf, int nLen, LogFormat *ptLogFormat, LogArgs *ptLogArgs)
{
    LogFormat *p = ptLogFormat;
    int nLeft = nLen;
    for (; p!=NULL; p=p->ptNext) {
        nLeft = p->fLogPrintf(psBuf + nLen - nLeft, nLeft, ptLogArgs, p->ptOtherArgs);
        if ( nLeft <= 0 ) {
            break;
        }
    }

    if (nLeft != 0) {
        psBuf[nLen - nLeft] = '\n';
        psBuf[nLen - nLeft + 1] = '\0';
    }
    
    return (0);
}

/**
 * logIsFilter
 * Check Log Is Filter
 * 
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logIsFilter(LogCfg *ptLogCfg, char *psGroup, int nLevel)
{
    if ( NULL == psGroup || NULL == ptLogCfg->ptFilterHastTable ) {
        return (0);
    }
    
    int *pnList = hashGet(ptLogCfg->ptFilterHastTable, psGroup);
    if ( NULL == pnList ) {
        return (0);
    }
    
    return ( pnList[nLevel] );
}

/**
 * LogOutPutArgs
 * Out Put By mutil args
 * change args to va_list
 * just invorke LogOutPut
 *
 * @param ptLogAppender: Aappender Info
 * @param psBuf: Message Buf
 * @param ...: Args
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogOutPutArgs(LogAppender *ptLogAppender, char *psBuf, ... )
{
    int nRet;
    
    va_list tVaList;
	va_start(tVaList, psBuf);
    nRet= LogOutPut(NULL, ptLogAppender, psBuf, tVaList);
    va_end(tVaList);
    
    return (nRet);
}

/**
 * LogOutPut
 * Do Out Put By Appender
 * if session is  End, Then Do Location Session
 *
 * @param psBuf: Message Buf
 * @param ptLogAppender: Log Appender Info
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogOutPut(LogArgs *ptLogArgs, LogAppender *ptLogAppender, char *psBuf, va_list tVaList)
{
    int nRet;
    time_t tNewTime = time(NULL);
    
    /* Do Session Start */
    if ( LOG_SESSION_STATUS_END == ptLogAppender->nSessionStatus && ptLogAppender->tOld != tNewTime ) {
        if ( ptLogAppender->fLogSessionStart != NULL) {
            nRet = ptLogAppender->fLogSessionStart(ptLogAppender->ptOtherArgs);
            if ( nRet != 0 ) {
                return (-1);
            }
        }
    }
    
    /* Out Put String */
    nRet = ptLogAppender->fLogOutPut(ptLogArgs, ptLogAppender->ptOtherArgs, psBuf, tVaList);
    if ( nRet != 0) {
        return (-1);
    }
    
    /* Do Session End */
    if ( LOG_SESSION_STATUS_END == ptLogAppender->nSessionStatus && ptLogAppender->tOld != tNewTime ) {
        if ( ptLogAppender->fLogSessionEnd != NULL ) {
            nRet = ptLogAppender->fLogSessionEnd(ptLogAppender->ptOtherArgs);
            if ( nRet != 0 ) {
                return (-1);
            }
        }
    }

    ptLogAppender->tOld = tNewTime;
    
    return (0);
}

/**
 * logLevelCtl
 * Level Info Change
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logLevelAction(LogCfgIdx *ptCfgInfo, char *psName, char *psValue)
{
    int nRet = 0;
    
    LogCfgIdx *p = ptCfgInfo;
    for (; p!=NULL; p=p->ptNext) {
        if ( NULL == p->ptLogCfg || NULL == p->ptLogCfg->ptLogAppender || NULL == p->ptLogCfg->ptLogAppender->fLogAppenderCtl ) {
            continue;
        }
        
        nRet = p->ptLogCfg->ptLogAppender->fLogAppenderCtl(p->ptLogCfg->ptLogAppender->ptOtherArgs, psName, psValue);
        if ( nRet != 0 ) {
            return ( nRet );
        }
    }
    
    if ( 0 == strcmp(psName, LOG_LEVEL_NAME) ) {
        return (logLevelSetLevelName(ptCfgInfo, psValue));
    }
    
    return ( nRet );
}

/**
 * logDefaultAction
 * Set Default Info
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logDefaultAction(char *psName, char *psValue)
{
    int nRet = 0;
    
    if ( 0 == strcmp(LOG_LEVEL, psName) ) {
        gpsLevelDefault = psValue;
        return ( nRet );
    }
    
    return  ( nRet );
}

/**
 * logLevelSetLevelName
 * Reset Level Name
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logLevelSetLevelName(LogCfgIdx *ptCfgInfo, char *psValue)
{
    strncpy(ptCfgInfo->sLevelName, psValue, sizeof(ptCfgInfo->sLevelName));
    
    return (0);
}

/**
 * LogFreeCfg
 * Log Free Config struct 
 *
 * @param ptLogCfg: Config sturct
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogFreeCfg(LogAllCfg *ptLogAllCfg)
{
    /* Check Null */
    if ( NULL == ptLogAllCfg ) {
        return (0);
    }
    
    /* Free Cfg Info */
    LogCfg *p = ptLogAllCfg->ptLogCfgList;
    while ( p != NULL ) {
        LogCfg *q = p->ptNext;
        LogFreeFormat( p->ptFormat );
        LogFreeAppender(p->ptLogAppender);
        free(p);
        p = q;
    }
    
    /* Free Index */
    int i = 0;
    int nNum = ptLogAllCfg->nNum;
    for (; i<nNum; ++i) {
        FreeLogCfgIdx(ptLogAllCfg->ptLogCfgIdx[i]);
    }
    
    /* Free Cfg List & self */
    free(ptLogAllCfg);
    
    return (0);
}

/**
 * LogFreeAppender
 * Log Free Appender's info
 * if Log Appender has fLogAppenderFree
 *   invork the fLogAppenderFree is free other info
 *
 * @param ptLogAppender: Appneder Info
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogFreeAppender(LogAppender *ptLogAppender)
{
    if ( NULL == ptLogAppender ) {
        return (0);
    }
    
    /* Free other info */
    if ( ptLogAppender->fLogAppenderFree != NULL ) {
        ptLogAppender->fLogAppenderFree(ptLogAppender->ptOtherArgs);
    }
    
    free(ptLogAppender);
    
    return (0);
}

/**
 * logLevelInit
 * Log Level Init
 * get Level name,level format, level type
 *
 * @param ptLogCfg: Cfg File
 * @param psLevelName: Level names
 * @param psKey: Key Name
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logLevelInit(LogAllCfg *ptLogAllCfg, char *psFile, char *psPkg, char *psGroup)
{
    int nRet;
    
    /* Init LogCfg */
    LogCfg *ptLogCfg = (LogCfg *)malloc(sizeof(LogCfg));
    memset(ptLogCfg, '\0', sizeof(LogCfg));
    strncpy(ptLogCfg->sCfglName, psPkg, sizeof(ptLogCfg->sCfglName)-1);
    
    /* Get Layout Config */
    nRet = logCfgLayout(ptLogCfg, psFile, psPkg, psGroup);
    if ( nRet != 0 ) {
        logLog("Cfg[%s.%s] logCfgLayout err!!\n", psGroup, psPkg);
        free(ptLogCfg);
        return (-1);
    }
    
    /* Get Appender */
    nRet = logCfgAppender(ptLogCfg, psFile, psPkg, psGroup);
    if ( nRet != 0 ) {
        logLog("Cfg[%s.%s] logCfgAppender err!!\n", psGroup, psPkg);
        free(ptLogCfg);
        return (-1);
    }
    
    /* Get Filter */
    nRet = logCfgFilter(ptLogCfg, psFile, psPkg, psGroup);
    if ( nRet != 0 ) {
        logLog("Cfg[%s.%s] logCfgFileter err[%d]\n", psGroup, psPkg, nRet);
        free(ptLogCfg);
        return (-1);
    }
    
    /* Add Cfg Info To All Cfg */
    nRet = logAddCfg(ptLogAllCfg, ptLogCfg, psFile, psPkg, psGroup);
    if ( nRet != 0 ) {
        logLog("Cfg[%s.%s] logAddCfg err!!\n", psGroup, psPkg);
        free(ptLogCfg);
        return (-1);
    }
    
    return (0);
}

/**
 * logAddCfg
 * get Level Info from cfg file
 * add cfg info to AllCfg Index and List
 *
 * @param ptLogAllCfg: All Config Info
 * @param ptLogCfg: Config Info
 * @param psFile: Cfg File
 * @param psKey: Key
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logAddCfg(LogAllCfg *ptLogAllCfg, LogCfg *ptLogCfg, char *psFile, char *psPkg, char *psGroup)
{
    int nRet;
    
    /* Get Level Config */
    char sBuf[LOG_BUF_LEN];
    memset(sBuf, '\0', sizeof(sBuf));
    logIniDefRead(psGroup, psPkg, LOG_LEVEL_LEVEL_KEY, gpsLevelDefault, sBuf, LOG_BUF_LEN, psFile);
        
    /* Split List */
    nRet = logAddCfgLevel(ptLogAllCfg, ptLogCfg, sBuf);
    if ( nRet != 0 ) {
        logLog("Cfg[%s.%s] File[%s] Add Level err!!\n", psPkg, LOG_LEVEL_LEVEL_KEY, psFile);
        return (-1);
    }
    
    /* Add Cfg To List Of All Config */
    LogCfg *p = ptLogAllCfg->ptLogCfgList;
    if ( NULL == p ) {
        ptLogAllCfg->ptLogCfgList = ptLogCfg;
    } else {
        for ( ; p->ptNext!=NULL ; p=p->ptNext)
            ;
        p->ptNext = ptLogCfg;
    }
    
    return (0);
}

/**
 * logAddCfgLevel
 * Add Cfg Level Info Err
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logAddCfgLevel(LogAllCfg *ptLogAllCfg, LogCfg *ptLogCfg, char *psBuf)
{
    int nRet = 0;
    
    /* Level is Null just return */
    if ( '\0' == psBuf[0] ) {
        return ( nRet );
    }
    
    /* Split List */
    char *ppsLevelList[LOG_LEVEL_NUMBER_DEFAULT];
    int nLen = logStrNSplit(ppsLevelList, psBuf, LOG_LEVEL_SPLIT, LOG_LEVEL_NUMBER_DEFAULT);
    if ( nLen < 0 ) {
        logLog("Split Buffer[%s] err!!\n", psBuf);
        return (-1);
    }
    
    int i = 0;
    for ( ; i<nLen; ++i) {
        
        /* Get Level Index And Name From Config */
        enum {
            LEVEL_INDEX,
            LEVEL_NAME,
            LEVEL_NULL,
            LEVEL_NUM
        };
        
        char *ppsNameList[LEVEL_NUM];
        nRet = logStrNSplit(ppsNameList, ppsLevelList[i], "{}", LEVEL_NUM);
        if ( nRet < 0 ) {
            logLog("Get Level Info[%s] err!!\n", ppsLevelList[i]);
            return (-1);
        }
        
        /* Add Level info To All Config */
        nRet = logAddCfgToAllCfgLevelIndex(ptLogAllCfg, ptLogCfg, ppsNameList[LEVEL_INDEX], ppsNameList[LEVEL_NAME]);
        if ( nRet != 0 ) {
            logLog("Add Level Info[%s] To AllCfg err!!\n", ppsLevelList[i]);
            return (-1);
        }
    }
    
    return ( nRet );
}

/**
 * logAddCfgToAllCfgLevelIndex
 * Add Cfg To AllCfg By Index
 * the index must be not init and legal
 * add index ,and , cfg
 *
 * @param ptLogAllCfg: All Config Index
 * @param ptLogCfg: Config
 * @param psIndex: index
 * @param psName: name
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logAddCfgToAllCfgLevelIndex(LogAllCfg *ptLogAllCfg, LogCfg *ptLogCfg, char *psIndex, char *psName)
{
    /* Check Index */
    int nIndex = atoi(psIndex);
    if ( nIndex < 0 || nIndex >= LOG_LEVEL_NUMBER_DEFAULT ) {
        logLog("level Index[%d] is < 0 or >= %d !!\n", nIndex, LOG_LEVEL_NUMBER_DEFAULT);
        return (-1);
    }
    
    /* Add To All Cfg */
    if ( psName != NULL && strlen(psName) > 0 ) {
        AddLogCfgIdx(ptLogAllCfg, nIndex, psName, ptLogCfg);
    } else {
        char sBuf[LOG_BUF_LEN];
        sprintf(sBuf, "%d", nIndex);
        AddLogCfgIdx(ptLogAllCfg, nIndex, sBuf, ptLogCfg);
    }
    
    return (0);
}

/**
 * logCfgLayout
 * Get Log Config Layout
 * and invork Layout Function To Get Layout Args
 *
 * @param ptLogCfg: Log Config sturct info
 * @param psFile: Cfg File
 * @param psKey: Key
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logCfgLayout(LogCfg *ptLogCfg, char *psFile, char *psPkg, char *psGroup)
{
    /* Get Layout Type */
    char sTempLayout[LOG_BUF_LEN];
    memset(sTempLayout, '\0', sizeof(sTempLayout));
    logIniDefRead(psGroup, psPkg, LOG_LEVEL_LAYOUT_KEY, LOG_LEVEL_LAYOUT_DEFAULT, sTempLayout, LOG_BUF_LEN, psFile);
    
    /* Get LayOut Args info */
    LogFormat *ptLogFormat = NULL;
    if ( 0 == strcmp(sTempLayout, LOG_LEVEL_LAYOUT_SIMPLE) ) {
        ptLogFormat = logGetSampleFormat(psFile, psPkg);
    } else if ( 0 == strcmp(sTempLayout, LOG_LEVEL_LAYOUT_PATTERN) ) {
        ptLogFormat = logGetPatternFormat(psFile, psPkg, psGroup);
    } else if ( 0 == strcmp(sTempLayout, LOG_LEVEL_LAYOUT_TRACE) ){
        ptLogFormat = logGetTraceFormat(psFile, psPkg);
    } else{
        logLog("Layout[%s] Not define!! Must Be List Below\n", sTempLayout);
        logLog("  SimpleLayout\n");
        logLog("  PatternLayout\n");
        return (-1);
    }
    
    if ( NULL == ptLogFormat ) {
        logLog("Layout[%s] Get Config err!!\n", sTempLayout);
        return (-1);
    }
    ptLogCfg->ptFormat = ptLogFormat;
    
    return (0);
}

/**
 * logGetSampleFormat
 * Sample Format is default Value
 *
 * @param psFile: Cfg File name
 * @param psKey: key
 *
 * @return >0  : ok
 *         <0  : err
 */
static LogFormat * logGetSampleFormat(char *psFile, char *psKey)
{
    LogFormat *ptLogFormat = (LogFormat *)malloc(sizeof(LogFormat));
    memset(ptLogFormat, '\0', sizeof(LogFormat));
    
    ptLogFormat->fLogPrintf = LogSamplePrintf;
    ptLogFormat->ptNext = NULL;
    ptLogFormat->ptOtherArgs = NULL;
    
    return ( ptLogFormat );
}

/**
 *  get trace format
 *
 *  @param psFile : ini file name
 *  @param psKey  : key name
 *
 *  @return format
 */
static LogFormat * logGetTraceFormat(char *psFile, char *psKey)
{

    LogFormat *ptLogFormat = (LogFormat *)malloc(sizeof(LogFormat));
    memset(ptLogFormat, '\0', sizeof(LogFormat));

    ptLogFormat->fLogPrintf = LogTracePrintf;
    ptLogFormat->ptNext = NULL;
    ptLogFormat->ptOtherArgs = NULL;

    return ( ptLogFormat );
}

/**
 * logGetPatternFormat
 * Get Pattern Args in the config file
 *
 * @param psFile: Cfg File name
 * @param psKey: key
 *
 * @return >0  : ok
 *         <0  : err
 */
static LogFormat * logGetPatternFormat(char *psFile, char *psPkg, char *psGroup)
{    
    /* Get Conversion Pattern */
    char sTempLayout[LOG_BUF_LEN];
    memset(sTempLayout, '\0', sizeof(sTempLayout));
    logIniDefRead(psGroup, psPkg, LOG_LEVEL_PATTERN_KEY, LOG_LEVEL_PATTERN_DEFAUTL, sTempLayout, LOG_BUF_LEN, psFile);

    return ( logFormatInit(psPkg, sTempLayout) );
}

/**
 * logCfgFilter
 * Get Log Fielter Info
 * 
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logCfgFilter(LogCfg *ptLogCfg, char *psFile, char *psPkg, char *psGroup)
{
    int nRet = 0;
    
    /* Get Appender Type */
    char sFilterFileName[LOG_MSG_BUF_LEN];
    memset(sFilterFileName, '\0', sizeof(sFilterFileName));
    logCfgReplaceFile(sFilterFileName, LOG_MSG_BUF_LEN, psGroup, psPkg, LOG_LEVEL_FILTER, LOG_LEVEL_FILTER_FILE_NAME_DEFAULT, psFile);
    
    /* Not Filter File Name */
    if ( '\0' == sFilterFileName[0] ) {
        return (0);
    }
    
    ptLogCfg->ptFilterHastTable = hashNew(0);
    if ( NULL == ptLogCfg->ptFilterHastTable ) {
        logLog("New Filter Hash Table Err");
        return (-1);
    }
    
    FILE *ptFp = fopen(sFilterFileName, "r+");
    if ( NULL == ptFp ) {
        logLog("Filter File[%s] open err[%s]", sFilterFileName, strerror(errno));
        return (-1);
    }
    
    nRet = logCfgFilterGetDetailByFile(ptLogCfg, ptFp);
    fclose(ptFp);
    
    if ( nRet != 0 ) {
        logLog("Get Filter File[%s] detail info err", sFilterFileName);
        return (-1);
    }
    
    return (0);
}

/**
 * logCfgFilterGetDetailByFile
 * Get Fileter Detail Info By File Point
 * 
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logCfgFilterGetDetailByFile(LogCfg *ptLogCfg, FILE *ptFp)
{
    int nRet = 0;
    
    char sBuffer[LOG_MSG_BUF_LEN];
    memset(sBuffer, '\0', sizeof(sBuffer));
    
    while ( NULL != fgets(sBuffer, LOG_MSG_BUF_LEN, ptFp) ) {
        int nLen = strlen(sBuffer);
        if ( sBuffer[nLen - 1] != '\n' ) {
            sBuffer[nLen -1] = '\0';
        }
        
        if ( '#' == sBuffer[0] ) {
            continue;
        }
        
        /* Split Info */
        char *ppsFilter[LOG_FILTER_TYPE_NUM];
        int nListNum = logStrNSplit(ppsFilter, sBuffer, "=", LOG_FILTER_TYPE_NUM);
        if ( nListNum <= 0 ) {
            logLog("Get Filter info [%s] err\n", sBuffer);
            continue;
        }
        
        int *pnFilterList = logFilterGetList(ppsFilter[LOG_FILTER_TYPE_VALUE]);
        nRet = hashAdd(ptLogCfg->ptFilterHastTable, strTrim(ppsFilter[LOG_FILTER_TYPE_NAME]), pnFilterList);
        if ( nRet != 0 ) {
            logLog("Filter Add Info [%s] err[%d]", ppsFilter[LOG_FILTER_TYPE_NAME], nRet);
            continue;
        }
    }
    
    return (0);
}

/**
 * logFilterGetList
 * Get Filter List Info
 * 
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int * logFilterGetList(char *psValue)
{
    int *pnFilterList = (int *)malloc(sizeof(int) * LOG_LEVEL_NUMBER_DEFAULT);
    memset(pnFilterList, '\0', sizeof(int) * LOG_LEVEL_NUMBER_DEFAULT);
    
    if ( '*' == psValue[0] ) {
        memset(pnFilterList, 1, sizeof(int) * LOG_LEVEL_NUMBER_DEFAULT);
        return ( pnFilterList );
    }
    
    /* Value is like '1,2,3,5' */
    char *ppsStringList[LOG_LEVEL_NUMBER_DEFAULT];
    int nListNum = logStrNSplit(ppsStringList, psValue, ",", LOG_LEVEL_NUMBER_DEFAULT);
    if ( nListNum < 0 ) {
        logLog("Filter Value [%s] parse err", psValue);
        return (NULL);
    }
    
    /* Set Filter Index into 1 */
    int i = 0;
    for (; i<nListNum; ++i) {
        int nIdx = atoi(ppsStringList[i]);
        if ( nIdx < 0 || nIdx >= LOG_LEVEL_NUMBER_DEFAULT ) {
            continue;
        }
        
        pnFilterList[nIdx] = 1;
    }
    
    return ( pnFilterList );
}

/**
 * logCfgAppender
 * Get Appender define info
 * invork the appender function to get args
 *
 * @param ptLogCfg: Log Config sturct info
 * @param psFile: Cfg File
 * @param psKey: Key
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logCfgAppender(LogCfg *ptLogCfg, char *psFile, char *psPkg, char *psGroup)
{
    /* Get Appender Type */
    char sTempAppendeer[LOG_BUF_LEN];
    memset(sTempAppendeer, '\0', sizeof(sTempAppendeer));
    logIniDefRead(psGroup, psPkg, NULL, LOG_LEVEL_APPENDER_DEFAULT, sTempAppendeer, LOG_BUF_LEN, psFile);
    
    /* Get LayOut Args info */
    LogAppender *ptLogAppender = NULL;
    if ( 0 == strcmp(sTempAppendeer, LOG_LEVEL_APPENDER_FILE) ) {
        ptLogAppender = logGetFileAppender(psFile, psPkg, psGroup);
    } else if ( 0 == strcmp(sTempAppendeer, LOG_LEVEL_APPENDER_CONSOLE) ) {
        ptLogAppender = logGetConsoleAppender(psFile, psPkg, psGroup);
    } else if ( 0 == strcmp(sTempAppendeer, LOG_LEVEL_APPENDER_ROLLING_FILE) ) {
        ptLogAppender = logGetRollingFileAppender(psFile, psPkg, psGroup);
    } else if ( 0 == strcmp(sTempAppendeer, LOG_LEVEL_APPENDER_BACKUP_ROLLING_FILE) ) {
        ptLogAppender = logGetBackupRollingFileAppender(psFile, psPkg, psGroup);
    } else if ( 0 == strcmp(sTempAppendeer, LOG_LEVEL_APPENDER_DAILY_ROLLING_FILE) ) {
        ptLogAppender = logGetDailyRollingFileAppender(psFile, psPkg, psGroup);
    } else {
        ptLogAppender = logGetUserDefineAppender(sTempAppendeer, psFile, psPkg, psGroup);
    }
    
    if ( NULL == ptLogAppender ) {
        logLog("Appender[%s] Get Config err!!\n", sTempAppendeer);
        logLog("  Must Be File, Console, RollingFile, DailyRollingFile or UserSelfDefind Appender\n");
        return (-1);
    }
    
    /* Add Session Info */
    ptLogAppender->tOld           = 0;
    ptLogAppender->nSessionStatus = LOG_SESSION_STATUS_END;
    if (0 == ptLogAppender->nSessionFlag) {
        ptLogAppender->nSessionFlag = logGetSessionFlag(psFile, psPkg, psGroup);
        if ( ptLogAppender->nSessionFlag <= 0 ) {
            logLog("GetSessionFlag err!!\n");
            return (-1);
        }
    }
    
    ptLogCfg->ptLogAppender = ptLogAppender;
    
    return (0);
}

/**
 * logGetUserDefineAppender
 * Get User Define Appender Function
 * Init User Appender
 *
 * @param psAppenderName: User Appender Name
 * @param psFile: Config File Name
 * @param psPkg: Config File Pkg name
 * @param psGroup: Confg File Group Name
 *
 * @return >0  : ok
 *         NULL: err
 */
static LogAppender * logGetUserDefineAppender(char *psAppenderName, char *psFile, char *psPkg, char *psGroup)
{
    int nRet;
    
    /* Get User Define Appender Init Function */
    FncInitUserAppender fInitAppender = GetUserDefineAppenderInitFunction(psAppenderName, psFile);
    if ( NULL == fInitAppender ) {
        logLog("Appender [%s] Not Define!!\n", psAppenderName);
        return ( NULL );
    }

    /* Init Appender Function */
    LogAppender *ptLogAppender = (LogAppender *)malloc(sizeof(LogAppender));
    nRet = fInitAppender(ptLogAppender, psFile, psPkg, psGroup);
    if ( nRet != 0 ) {
        logLog("Appender [%s] Init Err!!\n", psAppenderName);
        free(ptLogAppender);
        return ( NULL );
    }
    
    return ( ptLogAppender );
}

/**
 * logGetSessionFlag
 * Get Sission Flag from cofnig file
 * parse string to int value
 *
 * @param psFile: Config File Name
 * @param psKey: Key
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logGetSessionFlag(char *psFile, char *psPkg, char *psGroup)
{
    /* Get Flag */
    char sFlag[LOG_BUF_LEN];
    logIniDefRead(psGroup, psPkg, LOG_SESSION_KEY, LOG_SESSION_FLAG_ON_NAME, sFlag, LOG_BUF_LEN, psFile);
    
    /* Convet To Int */
    int nFlag;
    if ( 0 == strcmp(sFlag, LOG_SESSION_FLAG_ON_NAME) ) {
        nFlag = LOG_SESSION_FLAG_ON;
    } else if ( 0 == strcmp(sFlag, LOG_SESSION_FLAG_OFF_NAME) ) {
        nFlag = LOG_SESSION_FLAG_OFF;
    } else if ( 0 == strcmp(sFlag, LOG_SESSION_FLAG_NOPRINT_NAME) ) {
        nFlag = LOG_SESSION_FLAG_NOPRINT;
    } else {
        logLog("Get [%s.%s.%s] = [%s] values not define!!\n", psGroup, psPkg, LOG_SESSION_KEY, sFlag);
        logLog("  - Must Be [%s]/[%s]/[%s]\n", LOG_SESSION_FLAG_ON_NAME, LOG_SESSION_FLAG_OFF_NAME, LOG_SESSION_FLAG_NOPRINT_NAME);
        return (-1);
    }

    return (nFlag);
}

/**
 * logGetFileAppender
 * Get File Appender Args
 * set funcction to LogAppender
 *
 * @param psFile: Cfg File name
 * @param psKey: Key
 *
 * @return >0  : ok
 *         <0  : err
 */
static LogAppender * logGetFileAppender(char *psFile, char *psPkg, char *psGroup)
{
    /* Get File Name */
    LogAppenderFile *ptLogAppenderFile = (LogAppenderFile *)malloc(sizeof(LogAppenderFile));
    memset(ptLogAppenderFile, '\0', sizeof(LogAppenderFile));
    ptLogAppenderFile->psLogFileName = locCfgGetFileName(psGroup, psPkg, psFile);
    
    /* Init */
    LogAppender *ptLogAppender = (LogAppender *)malloc(sizeof(LogAppender));
    memset(ptLogAppender, '\0', sizeof(LogAppender));
    ptLogAppender->fLogOutPut       = LogFileOutPut;
    ptLogAppender->fLogSessionStart = LogFileSessionStart;
    ptLogAppender->fLogSessionEnd   = NULL;
    ptLogAppender->fLogAppenderFree = LogFileFree;
    ptLogAppender->ptOtherArgs      = (void *)ptLogAppenderFile;
    
    return ( ptLogAppender );
}

/**
 * LogFileFree
 */
static void LogFileFree(void *ptArgs)
{
    LogAppenderFile *ptLogFile = (LogAppenderFile *)(ptArgs);
    free(ptLogFile->psLogFileName);
    free(ptLogFile);
}

/**
 * LogFileOutPut
 * Out Put To File
 *
 * @param psBuf: Buf Info
 * @param ptArgs: Other Info
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogFileOutPut(LogArgs *ptLogArgs, void *ptArgs, char *psBuf, va_list tVaList)
{
    /* Printf Message To File Point */
    LogAppenderFile *ptLogFile = (LogAppenderFile *)(ptArgs);
    if ( NULL == ptLogFile->ptFp ) {
        LogFileSessionStart(ptArgs);
    }
    
    vfprintf(ptLogFile->ptFp, psBuf, tVaList);
    if ( 1 == giIsFlush) {
        fflush(ptLogFile->ptFp);
    }
    
    return (0);
}

/**
 * LogFileSessionStart
 * File Sesiion Start Acction
 * Open the File
 *
 * @param ptArgs: LogAppenderFile
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogFileSessionStart(void *ptArgs)
{
    int nRet = 0;
    struct stat tStatBuf;
    LogAppenderFile *ptLogFile = (LogAppenderFile *)(ptArgs);

    /* File not exist, reopen the file */
    memset(&(tStatBuf), '\0', sizeof(tStatBuf));
    nRet = stat(ptLogFile->psLogFileName, &tStatBuf);
    if (0 != nRet) {
        return LogFileReopen(ptArgs);
    }

    LogFileOpen(&tStatBuf, ptArgs);
    
    return (0);
}

/**
 * LogFileOpen
 */
static int LogFileOpen(struct stat *ptStatBuf, void *ptArgs)
{
    LogAppenderFile *ptLogFile = (LogAppenderFile *)(ptArgs);

    if (NULL != ptLogFile->ptFp) {
        if (ptStatBuf->st_dev == ptLogFile->lDev && ptStatBuf->st_ino == ptLogFile->lIno) {
            return 0;
        }

        LogFileClose(ptArgs);
    }

    ptLogFile->ptFp = _FileOpen(ptLogFile->psLogFileName, "a+");
    if ( NULL == ptLogFile->ptFp ) {
        logLog("File[%s] open err[%s]!! Now Write Log To stderr!!\n", ptLogFile->psLogFileName, strerror(errno));
        ptLogFile->ptFp = stderr;
        return 0;
    }
    ptLogFile->lDev = ptStatBuf->st_dev;
    ptLogFile->lIno = ptStatBuf->st_ino;

    return (0);
}

/**
 * LogFileClose
 */
static int LogFileClose(void *ptArgs)
{
    /* Close File */
    LogAppenderFile *ptLogFile = (LogAppenderFile *)(ptArgs);
    if ( ptLogFile->ptFp != NULL && ptLogFile->ptFp != stderr ) {
        fclose(ptLogFile->ptFp);
        ptLogFile->ptFp = NULL;
        ptLogFile->lIno = 0;
        ptLogFile->lDev = 0;
    }

    return (0);
}

/**
 * LogFileReopen
 */
static int LogFileReopen(void *ptArgs)
{
    LogAppenderFile *ptLogFile = (LogAppenderFile *)(ptArgs);
    struct stat tStatBuf;
    int nRet;

    if (ptLogFile->ptFp != NULL && stderr != ptLogFile->ptFp) {
        fclose(ptLogFile->ptFp);
    }

    ptLogFile->ptFp = _FileOpen(ptLogFile->psLogFileName, "a+");
    if ( NULL == ptLogFile->ptFp ) {
        logLog("File[%s] open err[%s]!! Now Write Log To stderr!!\n", ptLogFile->psLogFileName, strerror(errno));
        ptLogFile->ptFp = stderr;
        return (0);
    }

    /* File not exist, reopen the file */
    memset(&(tStatBuf), '\0', sizeof(tStatBuf));
    nRet = stat(ptLogFile->psLogFileName, &tStatBuf);
    if (0 != nRet) {
        return (-1);
    }
    ptLogFile->lDev = tStatBuf.st_dev;
    ptLogFile->lIno = tStatBuf.st_ino;

    return (0);
}

/**
 * logGetRollingFileAppender
 * Make Rolling File Appender 
 * Get File Name
 * Get Fiel Size
 * Get Time pattern
 *
 * @param psFile: config File Name
 * @param psKey: config Key
 *
 * @return >0  : ok
 *         NULL: err
 */
static LogAppender * logGetRollingFileAppender(char *psFile, char *psPkg, char *psGroup)
{
    int nRet;
    
    /* Get File Name */
    char sLogFileName[LOG_MSG_BUF_LEN];
    memset(sLogFileName, '\0', sizeof(sLogFileName));
    logCfgGetFileName(psGroup, psPkg, sLogFileName, sizeof(sLogFileName), psFile);
    
    /* Get Size */
    unsigned long lSize = logGetSizeNum(psFile, psPkg, psGroup);
    if ( lSize <= 0 ) {
        logLog("logGetSizeNum err!!\n");
        return (NULL);
    }

    /* Get Time Pattern */
    int iBackType = 0;
    nRet = logGetTimePattern(psFile, psPkg, psGroup, &iBackType);
    if ( nRet != 0 ) {
        logLog("logGetTimePattern err!!\n");
        return (NULL);
    }
    
    return ( logNewRollingFileAppender(sLogFileName, lSize, iBackType) );
}

/**
 * logNewRollingFileAppender
 * New Rolling File Appender
 * By Args
 *
 * @param psFileName: Log File Name
 * @param lSize: Log FIle Size
 * @param psTimePattern: Log Time Pattern
 *
 * @return >0  : ok
 *         NULL: err
 */
static LogAppender * logNewRollingFileAppender(char *psFileName, unsigned long lSize, int iBackType)
{
    LogAppenderRollingFile *ptLogRollingFile = (LogAppenderRollingFile *)malloc(sizeof(LogAppenderRollingFile));
    memset(ptLogRollingFile, '\0', sizeof(LogAppenderRollingFile));
    ptLogRollingFile->psLogFileName = strdup(psFileName);
    ptLogRollingFile->lSize = lSize;
    ptLogRollingFile->iBackType = iBackType;
    
    /* New Appender */
    LogAppender *ptLogAppender = (LogAppender *)malloc(sizeof(LogAppender));
    memset(ptLogAppender, '\0', sizeof(LogAppender));
    ptLogAppender->fLogOutPut       = LogFileOutPut;
    ptLogAppender->fLogSessionStart = LogFileRollingFileSessionStart;
    ptLogAppender->fLogSessionEnd   = NULL;
    ptLogAppender->fLogAppenderFree = logRollingFileFree;
    ptLogAppender->ptOtherArgs      = (void *)ptLogRollingFile;
    
    return (ptLogAppender);
}

/**
 * logRollingFileFree
 */
static void logRollingFileFree(void *ptArgs)
{
    LogAppenderRollingFile *ptLogRollingFile = (LogAppenderRollingFile *)ptArgs;
    free(ptLogRollingFile->psLogFileName);
    free(ptLogRollingFile);
}

/**
 * LogFileRollingFileSessionEnd
 * End Session Of Rolling FIle
 * Close File
 * if File is biger then size
 * then back up the file
 *
 * @param ptArgs: RollingFile Args
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogFileRollingFileSessionStart(void *ptArgs)
{
    int nRet;
    
    /* Check File Size */
    LogAppenderRollingFile *ptRollFile = (LogAppenderRollingFile *)ptArgs;
    struct stat tStatBuf;
    memset(&(tStatBuf), '\0', sizeof(tStatBuf));

    /* File not exist, reopen the file */
    nRet = stat(ptRollFile->psLogFileName, &tStatBuf);
    if (0 != nRet) {
        return LogFileReopen(ptArgs);
    }

    /* file size ok, check is file changed */
    if ( tStatBuf.st_size < ptRollFile->lSize ){
        return LogFileOpen(&tStatBuf, ptArgs);
    }

    /* file size too big, back up file */
    _lockFile(ptRollFile->ptFp);

    nRet = logBackUpFile(ptRollFile->psLogFileName, ptRollFile->lSize, ptRollFile->iBackType);
    if ( nRet != 0 ) {
        logLog("File[%s] bakc up err!!\n", ptRollFile->psLogFileName);
        /* back up file err, not return */
    }

    _unlockFile(ptRollFile->ptFp);

    return LogFileReopen(ptArgs);
}

/**
 * logGetBackupRollingFileAppender
 * Get Back Up Rolling File Info From Config FIle
 *
 * @param psFile: Config File
 * @param psPkg: Config Pkg Name
 * @param psGroup: Config Group Name
 *
 * @return >0  : ok
 *         NULL: err
 */
static LogAppender * logGetBackupRollingFileAppender(char *psFile, char *psPkg, char *psGroup)
{
    /* Get File Name */
    char sLogFileName[LOG_MSG_BUF_LEN];
    memset(sLogFileName, '\0', sizeof(sLogFileName));
    logCfgGetFileName(psGroup, psPkg, sLogFileName, sizeof(sLogFileName), psFile);

    /* Get Size */
    unsigned long lSize = logGetSizeNum(psFile, psPkg, psGroup);
    if ( lSize <= 0 ) {
        logLog("logGetSizeNum err!!\n");
        return (NULL);
    }

    /* Get Time Pattern */
    int nBackupNum = logGetBackupNum(psFile, psPkg, psGroup);
    if ( nBackupNum < 0 ) {
        logLog("logGetBackupNum err!!\n");
        return (NULL);
    }
    
    return ( logNewBackupRollingFileAppender(sLogFileName, lSize, nBackupNum) );
}

/**
 * logNewBackupRollingFileAppender
 * New BackUo Rolling FIle Appender
 *
 * @param psLogFileName: File Name
 * @param lSize: size
 * @param nBackupNum: BackUp Max Num
 *
 * @return >0  : ok
 *         NULL: err
 */
static LogAppender * logNewBackupRollingFileAppender(char *psLogFileName, unsigned long lSize, int nBackupNum)
{
    LogAppenderBackupRollingFile *ptLogBackupRollingFile = (LogAppenderBackupRollingFile *)malloc(sizeof(LogAppenderBackupRollingFile));
    memset(ptLogBackupRollingFile, '\0', sizeof(LogAppenderBackupRollingFile));
    ptLogBackupRollingFile->psLogFileName = strdup(psLogFileName);
    ptLogBackupRollingFile->lSize = lSize;
    ptLogBackupRollingFile->nMaxBackIndex = nBackupNum;
    ptLogBackupRollingFile->nNowBackIndex = -1;
    
    /* New Appender */
    LogAppender *ptLogAppender = (LogAppender *)malloc(sizeof(LogAppender));
    memset(ptLogAppender, '\0', sizeof(LogAppender));
    ptLogAppender->fLogOutPut       = LogFileOutPut;
    ptLogAppender->fLogSessionStart = LogFileBackupRollingFileSessionStart;
    ptLogAppender->fLogSessionEnd   = NULL;
    ptLogAppender->fLogAppenderFree = logBackupRollingFileFree;
    ptLogAppender->ptOtherArgs      = (void *)ptLogBackupRollingFile;
    
    return (ptLogAppender);
}

/**
 * logBackupRollingFileFree
 */
static void logBackupRollingFileFree(void *ptArgs)
{
    LogAppenderBackupRollingFile *ptLogBackupRollingFile = (LogAppenderBackupRollingFile *)ptArgs;
    free(ptLogBackupRollingFile->psLogFileName);
    free(ptLogBackupRollingFile);
}

/**
 * LogFileBackupRollingFileSessionStart
 * Bakc up Rolling File Appender
 * If File is bigger then size then bakcup
 *
 * @param ptArgs: LogAppenderBackupRollingFile
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogFileBackupRollingFileSessionStart(void *ptArgs)
{
    int nRet;

    LogAppenderBackupRollingFile *ptBackup = (LogAppenderBackupRollingFile *)ptArgs;
    struct stat tStatBuf;

    /* Check File Size */
    memset(&(tStatBuf), '\0', sizeof(tStatBuf));
    nRet = stat(ptBackup->psLogFileName, &tStatBuf);
    if (0 != nRet) {
        return LogFileReopen(ptArgs);
    }

    if ( tStatBuf.st_size < ptBackup->lSize ){
        return LogFileOpen(&tStatBuf, ptArgs);
    }

    _lockFile(ptBackup->ptFp);

    nRet = logBackUpRollingFile(ptBackup->psLogFileName, ptBackup->lSize, ptBackup->nMaxBackIndex, &(ptBackup->nNowBackIndex));
    if ( nRet != 0 ) {
        logLog("File[%s] bakc up err!!\n", ptBackup->psLogFileName);
        /* back up file err, not return */
    }

    _unlockFile(ptBackup->ptFp);

    return LogFileReopen(ptArgs);
}

/**
 * logGetDailyRollingFileAppender
 * Make Daily Rolling File Appender
 * Get Time Pattern, File Name
 *
 * @param psFile: Config File name
 * @param psKey: Configl key 
 *
 * @return >0  : ok
 *         NULL: err
 */
static LogAppender * logGetDailyRollingFileAppender(char *psFile, char *psPkg, char *psGroup)
{
    int nRet;
    
    /* Init */
    LogAppenderDailyRollingFile *ptLogDailyFile = (LogAppenderDailyRollingFile *)malloc(sizeof(LogAppenderDailyRollingFile));
    memset(ptLogDailyFile, '\0', sizeof(LogAppenderDailyRollingFile));
    
    /* Get File Name */
    ptLogDailyFile->psLogFileName = locCfgGetFileName(psGroup, psPkg, psFile);
    
    /* Get Time Pattern */
    char sTimePattern[LOG_MINI_BUF];
    memset(sTimePattern, '\0', sizeof(sTimePattern));
    nRet = logGetTimePattern(psFile, psPkg, psGroup, &(ptLogDailyFile->nBackUpType));
    if ( nRet != 0 ) {
        logLog("logGetDailyRollingFileAppender: logGetTimePattern err!!\n");
        free(ptLogDailyFile);
        return (NULL);
    }
    
    /* Get Back Up Time */
    nRet = logGetLastBackUpTime(ptLogDailyFile->nBackUpType, &(ptLogDailyFile->tLastBackUpTime));
    if ( nRet != 0 ) {
        logLog("logGetDailyRollingFileAppender: logGetLastBackUpTime err!!\n");
        free(ptLogDailyFile);
        return (NULL);
    }
    nRet = logGetNextBackUpTime(ptLogDailyFile->nBackUpType, &(ptLogDailyFile->tNextBackUpTime));
    if ( nRet != 0 ) {
        logLog("logGetDailyRollingFileAppender: logGetNextBackUpTime err!!\n");
        free(ptLogDailyFile);
        return (NULL);
    }
    
    /* New Appender */
    LogAppender *ptLogAppender = (LogAppender *)malloc(sizeof(LogAppender));
    memset(ptLogAppender, '\0', sizeof(LogAppender));
    ptLogAppender->fLogOutPut       = LogFileOutPut;
    ptLogAppender->fLogSessionStart = LogFileDailyRollingFileSessionStart;
    ptLogAppender->fLogSessionEnd   = NULL;
    ptLogAppender->fLogAppenderFree = locDailyRollingFileFree;
    ptLogAppender->ptOtherArgs      = (void *)ptLogDailyFile;
    
    return (ptLogAppender);
}

/**
 * locDailyRollingFileFree
 */
static void locDailyRollingFileFree(void *ptArgs)
{
    LogAppenderDailyRollingFile *ptLogDailyFile = (LogAppenderDailyRollingFile *)ptArgs;
    free(ptLogDailyFile->psLogFileName);
    free(ptLogDailyFile);
}

/**
 * LogFileDailyRollingFileSessionStart
 * When Session start
 * check system time, if time is bigger then next back up time
 *   then updaste back up time
 * check file create time, if time less then last back up time
 *   then back up
 * open file
 *
 * @param ptArgs: Daily File Args
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogFileDailyRollingFileSessionStart(void *ptArgs)
{
    int nRet;

    /* Check File Size */
    LogAppenderDailyRollingFile *ptDailyFile = (LogAppenderDailyRollingFile *)ptArgs;
    nRet = logCheckNextBackUpTime(ptDailyFile);
    if ( nRet != 0 ) {
        logLog("logCheckNextBackUpTime err!!\n");
        /* Not Return Err */
    }

    /* Check File Lats Modify Time */
    struct stat tStatBuf;
    memset(&(tStatBuf), '\0', sizeof(tStatBuf));
    nRet = stat(ptDailyFile->psLogFileName, &tStatBuf);
    if ( nRet != 0 ) {
        return LogFileReopen((void *)ptDailyFile);
    }

    /* Check File Lats Modify Time */
    if ( tStatBuf.st_mtime >= ptDailyFile->tLastBackUpTime ) {
        return LogFileOpen(&tStatBuf, ptArgs);
    }

    _lockFile(ptDailyFile->ptFp);

    nRet = logCheckLastBackUpTime(ptDailyFile);
    if ( nRet != 0 ) {
        logLog("logCheckLastBackUpTime err!!\n");
        /* Not Return Err */
    }

    _unlockFile(ptDailyFile->ptFp);
    
    return LogFileOpen(&tStatBuf, ptArgs);
}

/**
 * logCheckNextBackUpTime
 * Check Next Back Up Time
 * if Systime Time is Bigger then Next Back up Time
 * then update next Beck up Time
 *
 * @param ptDailyFile: Args of Daily File
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logCheckNextBackUpTime(LogAppenderDailyRollingFile *ptDailyFile)
{
    int nRet;
    
    /* Check System Time */
    time_t tSystime = time(NULL);
    if ( tSystime < ptDailyFile->tNextBackUpTime ) {
        return (0);
    }
    
    /* Update Time */
    ptDailyFile->tLastBackUpTime = ptDailyFile->tNextBackUpTime;
    nRet = logGetNextBackUpTime(ptDailyFile->nBackUpType, &(ptDailyFile->tNextBackUpTime));
    if ( nRet != 0 ) {
        logLog("logGetNextBackUpTime err!!\n");
        return (-1);
    }
    
    return (0);
}

/**
 * logCheckLastBackUpTime
 * Check File Creat Time is less thne Last Back Up Time
 * then back up the File
 *
 * @param ptDailyFile: Args of Daily File
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logCheckLastBackUpTime(LogAppenderDailyRollingFile *ptDailyFile)
{
    int nRet;

    /* Check File Lats Modify Time */
    struct stat tStatBuf;
    memset(&(tStatBuf), '\0', sizeof(tStatBuf));
    nRet = stat(ptDailyFile->psLogFileName, &tStatBuf);
    if ( nRet != 0 ) {
        return LogFileReopen((void *)ptDailyFile);
    }

    if ( tStatBuf.st_mtime >= ptDailyFile->tLastBackUpTime ) {
        return (0);
    }

    char sTime[LOG_BUF_LEN];
    memset(sTime, '\0', sizeof(sTime));
    struct tm *ptTm  = localtime(&tStatBuf.st_mtime);
    int iOffset = 0;
    do {
        iOffset += LogIntFormat(sTime+iOffset, 4, ptTm->tm_year + 1900);
        sTime[iOffset++] = '-';
        iOffset += LogIntFormat(sTime+iOffset, 2, ptTm->tm_mon + 1);
        if (LOG_LEVEL_TIME_PATTERN_MONTHLY_NUM == ptDailyFile->nBackUpType) {
            break;
        }
        sTime[iOffset++] = '-';

        iOffset += LogIntFormat(sTime+iOffset, 2, ptTm->tm_mday);
        if (LOG_LEVEL_TIME_PATTERN_DAILY_NUM == ptDailyFile->nBackUpType) {
            break;
        }
        sTime[iOffset++] = '-';

        iOffset += LogIntFormat(sTime+iOffset, 2, ptTm->tm_hour);
        if (LOG_LEVEL_TIME_PATTERN_HOUR_NUM == ptDailyFile->nBackUpType) {
            break;
        }
        sTime[iOffset++] = '-';

        iOffset += LogIntFormat(sTime+iOffset, 2, ptTm->tm_min);
    } while (0);
    
    char *psTemp = strrchr(ptDailyFile->psLogFileName, '.');
    if ( psTemp != NULL ) {
        *psTemp = '\0';
    }
    
    char sBackUpFile[LOG_BUF_LEN];
    sprintf(sBackUpFile, "%s.%s.log", ptDailyFile->psLogFileName, sTime);
    if ( psTemp != NULL ) {
        *psTemp = '.';
    }
    
    nRet = rename(ptDailyFile->psLogFileName, sBackUpFile);
    if ( nRet != 0 ) {
        logLog("Back up File[%s] To File[%s] Err[%s]!!\n", ptDailyFile->psLogFileName, sBackUpFile, strerror(errno));
        return (-1);
    }
    
    return (0);
}

/**
 * logGetConsoleAppender
 * Get Console Args
 * send consol to log Appenders
 *
 * @param psFile: config file name
 * @param psKey: key
 *
 * @return >0  : ok
 *         NULL: err
 */
static LogAppender * logGetConsoleAppender(char *psFile, char *psPkg, char *psGroup)
{
    LogAppender *ptLogAppender = (LogAppender *)malloc(sizeof(LogAppender));
    memset(ptLogAppender, '\0', sizeof(LogAppender));
    
    ptLogAppender->fLogOutPut       = LogConsoleOutPut;
    ptLogAppender->fLogSessionStart = NULL;
    ptLogAppender->fLogSessionEnd   = NULL;
    ptLogAppender->fLogAppenderFree = NULL;
    ptLogAppender->ptOtherArgs      = NULL;
    
    return ( ptLogAppender );
}

/**
 * LogConsoleOutPut
 * Printf Message To Console
 *
 * @param psBuf: Message buf
 * @param ptArgs: NOT USE
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogConsoleOutPut(LogArgs *ptLogArgs, void *ptArgs, char *psBuf, va_list taVaList)
{
    vfprintf(stderr, psBuf, taVaList);
    
    return (0);
}

/**
 * logFormatInit
 * log Format Info Init
 * get format by format type
 *
 * @param psFormat: format string
 *
 * @return >0  : ok
 *         NULL: err
 */
static LogFormat * logFormatInit(char *psKey, char *psFormat)
{
    int nRet;
    LogFormat *ptLogFormat = NULL;
    LogFormatArgs tLogFormatArgs;
    
    char *psTemp = psFormat;
    LogFormat *p = NULL;
    while ( (psTemp = logGetNextFormat(psTemp, &(tLogFormatArgs))) != NULL ) {
        /* New Next Log Format */
        if ( NULL == p ) {
            ptLogFormat = p = (LogFormat *)malloc(sizeof(LogFormat));
            memset(p, '\0', sizeof(LogFormat));
        } else {
            p->ptNext = (LogFormat *)malloc(sizeof(LogFormat));
            p = p->ptNext;
            memset(p, '\0', sizeof(LogFormat));
        }
        
        /* Add Format */
        switch ( tLogFormatArgs.caName ) {
            case LOG_FORMAT_STRING:
                nRet = LogAddStringFormat(p, tLogFormatArgs.psInfo, tLogFormatArgs.nLen);
                break;
                
            case LOG_FORMAT_MESSAGE:
                nRet = LogAddMessageFormat(p, tLogFormatArgs.psInfo, tLogFormatArgs.nLen);
                break;
                
            case LOG_FORMAT_FILE:
                nRet = LogAddFileForamt(p, tLogFormatArgs.psInfo, tLogFormatArgs.nLen);
                break;
                
            case LOG_FORMAT_LINE:
                nRet = LogAddlineFormat(p, tLogFormatArgs.psInfo, tLogFormatArgs.nLen);
                break;
                
            case LOG_FORMAT_GROUP:
                nRet = logAddGroupFormat(p, tLogFormatArgs.psInfo, tLogFormatArgs.nLen);
                break;

            case LOG_FORMAT_SECOND:
                nRet = logAddSecondFormat(p, tLogFormatArgs.psInfo, tLogFormatArgs.nLen);
                break;
                
            case LOG_FORMAT_FUNCTION:
                nRet = LogAddFunctionFormat(p, tLogFormatArgs.psInfo, tLogFormatArgs.nLen);
                break;
                
            case LOG_FORMAT_TIME:
                nRet = LogAddTimeFormat(p, tLogFormatArgs.psInfo, tLogFormatArgs.nLen);
                break;
                
            case LOG_FORMAT_PROCESS_ID:
                nRet = LogAddProcessId(p, tLogFormatArgs.psInfo, tLogFormatArgs.nLen);
                break;
                
            case LOG_FORMAT_LEVEL_NAME:
                nRet = LogAddLevelName(p, tLogFormatArgs.psInfo, tLogFormatArgs.nLen);
                break;
                
            case LOG_FORMAT_RUN_TIME:
                nRet = LogAddRunTime(p, tLogFormatArgs.psInfo, tLogFormatArgs.nLen);
                break;
                
            case LOG_FORMAT_SESSION:
                nRet = LogAddSession(p, tLogFormatArgs.psInfo, tLogFormatArgs.nLen);
                break;
                
            default:
                logLog("Format Type[%%%c] not define !!\n", tLogFormatArgs.caName);
                LogFreeFormat(ptLogFormat);
                return (NULL);
                break;
        }
        
        if ( nRet != 0 ) {
            logLog("Add Format[%%%c] Err!!\n", tLogFormatArgs.caName);
            LogFreeFormat(ptLogFormat);
            return (NULL);
        }
    }
    
    return (ptLogFormat);
}

/**
 * LogAddStringFormat
 * Log Add String Printf Info
 *
 * @param psBuf: Args Buf
 * @param nLen: Args Buf len
 *
 * @return >0  : ok
 *         NULL: err
 */
static int LogAddStringFormat(LogFormat *ptLogFormat, char *psBuf, int nLen)
{
    ptLogFormat->fLogPrintf = LogStringPrintf;
    ptLogFormat->ptOtherArgs = logStrNDup(psBuf, nLen);
    ptLogFormat->ptNext = NULL;
    
    return ( 0 );
}

/**
 * LogStringPrintf
 * Log Printf String Info
 *
 * @param psBuf: Message Buffer
 * @param nLen: Buffer Len
 * @param ptLogArgs: Log Used Args
 * @param ptArgs: Other Info
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogStringPrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    int nOffset = snprintf(psBuf, nLen, "%s", (char *)ptArgs);
    return ( nLen - nOffset );
}

/**
 * LogAddMessageFormat
 * Log Add message Format fucntion 
 *
 * @param psBuf: Not Ues
 * @param nLen: Not Use
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogAddMessageFormat(LogFormat *ptLogFormat, char *psBuf, int nLen)
{
    ptLogFormat->fLogPrintf = LogMessagePrintf;
    if ( NULL == psBuf ) {
        ptLogFormat->ptOtherArgs = logStrNDup("%s", 2);
    } else {
        ptLogFormat->ptOtherArgs = logStrNDup(psBuf, nLen);
    }
    ptLogFormat->ptNext = NULL;
    
    return (0);
}

/**
 * LogAddFileForamt
 * Add Information Foramt Printf Tyoe Funciton
 *
 * @param ptLogFormat: Format Args
 * @param psBuf: NOT USE
 * @param nLen: NOT USe
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogAddFileForamt(LogFormat *ptLogFormat, char *psBuf, int nLen)
{
    ptLogFormat->fLogPrintf = LogFilePrintf;
    if ( NULL == psBuf ) {
        ptLogFormat->ptOtherArgs = logStrNDup("%s", 2);
    } else {
        ptLogFormat->ptOtherArgs = logStrNDup(psBuf, nLen);
    }
    ptLogFormat->ptNext = NULL;
    
    return (0);
}

/**
 * LogAddlineFormat
 * Add Line Format To Format Printf
 *
 * @param ptLogFormat: Format Struct 
 * @param psBuf: Info Buf  
 * @param nLen: Buf Len
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogAddlineFormat(LogFormat *ptLogFormat, char *psBuf, int nLen)
{
    ptLogFormat->fLogPrintf = LogLinePrintf;
    if ( NULL == psBuf ) {
        ptLogFormat->ptOtherArgs = logStrNDup("%d", 2);
    } else {
        ptLogFormat->ptOtherArgs = logStrNDup(psBuf, nLen);
    }
    ptLogFormat->ptNext = NULL;
    
    return (0);
}

/**
 * logAddGroupFormat
 * Add Group Format Info
 * 
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logAddGroupFormat(LogFormat *ptLogFormat, char *psBuf, int nLen)
{
    ptLogFormat->fLogPrintf = LogGroupPrintf;
    if ( NULL == psBuf ) {
        ptLogFormat->ptOtherArgs = logStrNDup("%s", 2);
    } else {
        ptLogFormat->ptOtherArgs = logStrNDup(psBuf, nLen);
    }
    ptLogFormat->ptNext = NULL;
    
    return (0);
}

/**
 * logAddSecondFormat
 * Add Group Format Info
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logAddSecondFormat(LogFormat *ptLogFormat, char *psBuf, int nLen)
{
    ptLogFormat->fLogPrintf = LogSecondPrintf;
    if ( NULL == psBuf ) {
        ptLogFormat->ptOtherArgs = logStrNDup("%06d", 4);
    } else {
        ptLogFormat->ptOtherArgs = logStrNDup(psBuf, nLen);
    }
    ptLogFormat->ptNext = NULL;

    return (0);
}

/**
 * LogAddFunctionFormat
 * Log Add Function Format Way to Info
 *
 * @param ptLogFormat: Format Args
 * @param psBuf: NOT USE
 * @param nLen: NOT USe
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogAddFunctionFormat(LogFormat *ptLogFormat, char *psBuf, int nLen)
{
    ptLogFormat->fLogPrintf = LogFunctionPrintf;
    if ( NULL == psBuf ) {
        ptLogFormat->ptOtherArgs = logStrNDup("%s", 2);
    } else {
        ptLogFormat->ptOtherArgs = logStrNDup(psBuf, nLen);
    }
    ptLogFormat->ptNext = NULL;
    
    return (0);
}

/**
 * LogAddTimeFormat
 * Log Add Time Format info
 *
 * @param ptLogFormat: Format Args
 * @param psBuf: NOT USE
 * @param nLen: NOT USe
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogAddTimeFormat(LogFormat *ptLogFormat, char *psBuf, int nLen)
{
    ptLogFormat->fLogPrintf = LogTimePrintf;
    if ( NULL == psBuf ) {
        ptLogFormat->ptOtherArgs = logStrNDup(LOG_LEVEL_TIME_DEFAULT, sizeof(LOG_LEVEL_TIME_DEFAULT));
    } else {
        ptLogFormat->ptOtherArgs = logStrNDup(psBuf, nLen);
    }
    ptLogFormat->ptNext = NULL;
    
    return (0);
}

/**
 * LogAddProcessId
 * Add Pricess Pritnf Fucntion
 *
 * @param ptLogFormat: Format Args
 * @param psBuf: NOT USE
 * @param nLen: NOT USe
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogAddProcessId(LogFormat *ptLogFormat, char *psBuf, int nLen)
{
    ptLogFormat->fLogPrintf = LogProcessIdPrintf;
    if ( NULL == psBuf ) {
        ptLogFormat->ptOtherArgs = logStrNDup("%d", 2);
    } else {
        ptLogFormat->ptOtherArgs = logStrNDup(psBuf, nLen);
    }
    ptLogFormat->ptNext = NULL;
    
    return (0);
}

/**
 * LogAddLevelName
 * Log Add Level Name
 *
 * @param ptLogFormat: Format Args
 * @param psBuf: NOT USE
 * @param nLen: NOT USe
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogAddLevelName(LogFormat *ptLogFormat, char *psBuf, int nLen)
{
    ptLogFormat->fLogPrintf = LogLevelNamePrintf;
    if ( NULL == psBuf ) {
        ptLogFormat->ptOtherArgs = logStrNDup("%s", 2);
    } else {
        ptLogFormat->ptOtherArgs = logStrNDup(psBuf, nLen);
    }
    ptLogFormat->ptNext = NULL;
    
    return (0);
}

/**
 * LogAddLevelName
 * Log Add Run Time Printf function
 *
 * @param ptLogFormat: Format Args
 * @param psBuf: NOT USE
 * @param nLen: NOT USe
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogAddRunTime(LogFormat *ptLogFormat, char *psBuf, int nLen)
{
    ptLogFormat->fLogPrintf = LogRunTimePrintf;
    if ( NULL == psBuf ) {
        ptLogFormat->ptOtherArgs = logStrNDup("%d.%06d", 7);
    } else {
        ptLogFormat->ptOtherArgs = logStrNDup(psBuf, nLen);
    }
    ptLogFormat->ptNext = NULL;
    
    return (0);
}

/**
 * LogAddSession
 * Log Add seison Printf Function
 *
 * @param ptLogFormat: Format Args
 * @param psBuf: NOT USE
 * @param nLen: NOT USE
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogAddSession(LogFormat *ptLogFormat, char *psBuf, int nLen)
{
    ptLogFormat->fLogPrintf = LogSessionPrintf;
    if ( NULL == psBuf ) {
        ptLogFormat->ptOtherArgs = logStrNDup("%s", 2);
    } else {
        ptLogFormat->ptOtherArgs = logStrNDup(psBuf, nLen);
    }
    ptLogFormat->ptNext = NULL;
    
    return (0);
}

/**
 * LogMessagePrintf
 * Log Message printf
 *
 * @param psBuf: Message Buf
 * @param nLen: Messasge Buf Len
 * @param ptLogArgs: Log Args Info
 * @param ptArgs: Not Use
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogMessagePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    int nOffset = snprintf(psBuf, nLen, (char *)ptArgs, ptLogArgs->psLogStr);
    return ( nLen - nOffset );
}

/**
 * LogFilePrintf
 * Log File Printf
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
static int LogFilePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    /* Get Base name */
    char *psFile = strrchr(ptLogArgs->psFile, '/');
    if ( NULL == psFile ) {
        psFile = ptLogArgs->psFile;
    } else {
        ++psFile;
    }
    
    int nOffset = snprintf(psBuf, nLen, (char *)ptArgs, psFile);
    return ( nLen - nOffset );
}

/**
 * LogLinePrintf
 * Log Printf Line Num
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
static int LogLinePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    int nOffset = snprintf(psBuf, nLen, (char *)ptArgs, ptLogArgs->nLine);
    return ( nLen - nOffset );
}

/**
 * LogFunctionPrintf
 * Printf Function Name
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
static int LogFunctionPrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    int nOffset = snprintf(psBuf, nLen, (char *)ptArgs, ptLogArgs->psFunction);
    return ( nLen - nOffset );
}

/**
 * LogGroupPrintf
 * Printf Gourp Name
 * 
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int LogGroupPrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    if ( NULL == ptLogArgs->psGroupName ) {
        return (nLen);
    }
    
    int nOffset = snprintf(psBuf, nLen, (char *)ptArgs, ptLogArgs->psGroupName);
    return ( nLen - nOffset );
}

/**
 * LogSecondPrintf
 * Printf Gourp Name
 *
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int LogSecondPrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    struct timeval tvTime;
    gettimeofday(&tvTime, NULL);

    int nOffset = snprintf(psBuf, nLen, (char *)ptArgs, tvTime.tv_usec);
    return ( nLen - nOffset );
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

    int nOffset = strftime(psBuf, nLen, (char *)ptArgs, ptTm);
    return ( nLen - nOffset );
}

/**
 * LogProcessIdPrintf
 * Printf Process Id
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
static int LogProcessIdPrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    int nOffset = snprintf(psBuf, nLen, (char *)ptArgs, getpid());
    return ( nLen - nOffset );
}

/**
 * LogLevelNamePrintf
 * Log Printf Level Name
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
static int LogLevelNamePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    int nOffset = snprintf(psBuf, nLen, (char *)ptArgs, ptLogArgs->psLevelStr);
    return ( nLen - nOffset );
}

/**
 * LogRunTimePrintf
 * Log Printf Runint Time
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
static int LogRunTimePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    static struct timeval tTime;

    dtmEndTimer(&tTime);
    int nOffset = snprintf(psBuf, nLen, (char *)ptArgs, tTime.tv_sec, tTime.tv_usec);
    dtmBeginTimer(&tTime);

    return ( nLen - nOffset );
}
            
/**
 * LogSessionPrintf
 * Printf Session If Session is Start
 * Check If gpsLastSessionKey != NULL And gnKeyNum != 0 
 * then printf
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
static int LogSessionPrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    if ('\0' == gsSessionKey[0]) {
        return nLen;
    }
    
    int nOffset = snprintf(psBuf, nLen, (char *)ptArgs, gsSessionKey);
    
    return ( nLen - nOffset );
}

/**
 *  int to string
 *
 *  @param psBuffer : buffer
 *  @param iLen     : buffer len
 *  @param iNum     : int
 *
 *  @return len
 */
static int LogIntFormat(char *psBuffer, int iLen, int iNum)
{
    int i = 0;
    
    if (iLen < 0) {
        i = 13;
    } else {
        i = iLen - 1;
    }

    for (; i>=0 && 0 != iNum; i--,iNum /= 10) {
        psBuffer[i] = '0' + (iNum % 10);
    }

    if (i < 0) {
        return iLen;
    }

    if (iLen > 0) {
        memset(psBuffer, '0', i+1);
        return iLen;
    }

    iLen = 13 - i;
    if ( 0 == iLen) {
        psBuffer[0] = '0';
        return 1;
    }

    memmove(psBuffer, psBuffer+i+1, iLen);

    return iLen;
}

/**
 * LogSamplePrintf
 * Log Printf In Default Way
 *
 * @param psBuf: Message Buf
 * @param nLen: Buf Len
 * @param ptLogArgs: Log Args
 * @param ptArgs: Other Args
 * @param tVaList: string List
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogSamplePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    int nOffset = 0;

    /* Get Base File */
    char *psFile = strrchr(ptLogArgs->psFile, '/');
    if ( NULL == psFile ) {
        psFile = ptLogArgs->psFile;
    } else {
        ++psFile;
    }

    time_t tTime = time(NULL);
    struct tm *ptTm  = localtime(&tTime);
    psBuf[nOffset++] = '[';
    nOffset += LogIntFormat(psBuf+nOffset, 2, ptTm->tm_mon + 1);
    psBuf[nOffset++] = '-';
    nOffset += LogIntFormat(psBuf+nOffset, 2, ptTm->tm_mday);
    psBuf[nOffset++] = ' ';
    nOffset += LogIntFormat(psBuf+nOffset, 2, ptTm->tm_hour);
    psBuf[nOffset++] = ':';
    nOffset += LogIntFormat(psBuf+nOffset, 2, ptTm->tm_min);
    psBuf[nOffset++] = ':';
    nOffset += LogIntFormat(psBuf+nOffset, 2, ptTm->tm_sec);
    psBuf[nOffset++] = ']';

    psBuf[nOffset++] = '[';
    LOGSTRCAT(psBuf+nOffset, ptLogArgs->psLevelStr, nOffset);
    psBuf[nOffset++] = ']';
    psBuf[nOffset++] = '[';
    nOffset += LogIntFormat(psBuf+nOffset, -1, getpid());
    psBuf[nOffset++] = ']';
    psBuf[nOffset++] = '[';
    LOGSTRCAT(psBuf+nOffset, psFile, nOffset);
    psBuf[nOffset++] = ':';
    nOffset += LogIntFormat(psBuf+nOffset, -1, ptLogArgs->nLine);
    psBuf[nOffset++] = ']';
    if ( NULL != ptLogArgs->psFunction ) {
        psBuf[nOffset++] = '[';
        LOGSTRCAT(psBuf+nOffset, (char *)ptLogArgs->psFunction, nOffset);
        psBuf[nOffset++] = ']';
    }

    psBuf[nOffset++] = ' ';
    LOGSTRCAT(psBuf+nOffset, ptLogArgs->psLogStr, nOffset);
    psBuf[nOffset] = '\0';

    return ( nLen - nOffset );
}

/**
 *  printf log info and time trace info
 *
 *  @param psBuf     : buffer for log
 *  @param nLen      : buffer len
 *  @param ptLogArgs : log args
 *  @param ptArgs    : format args
 *
 *  @return 0
 */
static int LogTracePrintf(char *psBuf, int nLen, LogArgs *ptLogArgs, void *ptArgs)
{
    int nOffset = 0;
    static struct timeval tTime;

    /* printf Base File */
    char *psFile = strrchr(ptLogArgs->psFile, '/');
    if ( NULL == psFile ) {
        psFile = ptLogArgs->psFile;
    } else {
        ++psFile;
    }

    /* printf last time run time */
    dtmEndTimer(&tTime);
    psBuf[nOffset++] = '[';
    nOffset += LogIntFormat(psBuf+nOffset, -1, tTime.tv_sec);
    psBuf[nOffset++] = '.';
    nOffset += LogIntFormat(psBuf+nOffset, 6, tTime.tv_usec);
    psBuf[nOffset++] = ']';
    dtmBeginTimer(&tTime);

    /* printf Time */
    time_t tSystime = time(NULL);
    struct tm *ptTm  = localtime(&tSystime);
    psBuf[nOffset++] = '[';
    nOffset += LogIntFormat(psBuf+nOffset, 2, ptTm->tm_mon + 1);
    psBuf[nOffset++] = '-';
    nOffset += LogIntFormat(psBuf+nOffset, 2, ptTm->tm_mday);
    psBuf[nOffset++] = ' ';
    nOffset += LogIntFormat(psBuf+nOffset, 2, ptTm->tm_hour);
    psBuf[nOffset++] = ':';
    nOffset += LogIntFormat(psBuf+nOffset, 2, ptTm->tm_min);
    psBuf[nOffset++] = ':';
    nOffset += LogIntFormat(psBuf+nOffset, 2, ptTm->tm_sec);
    psBuf[nOffset++] = '.';
    nOffset += LogIntFormat(psBuf+nOffset, 6, tTime.tv_usec);
    psBuf[nOffset++] = ']';

    /* printf log level name */
    psBuf[nOffset++] = '[';
    LOGSTRCAT(psBuf+nOffset, ptLogArgs->psLevelStr, nOffset);
    psBuf[nOffset++] = ']';

    /* printf pid */
    psBuf[nOffset++] = '[';
    nOffset += LogIntFormat(psBuf+nOffset, -1, getpid());
    psBuf[nOffset++] = ']';
    
    psBuf[nOffset++] = '[';
    LOGSTRCAT(psBuf+nOffset, psFile, nOffset);
    psBuf[nOffset++] = ':';
    nOffset += LogIntFormat(psBuf+nOffset, -1, ptLogArgs->nLine);
    psBuf[nOffset++] = ']';

    /* printf function */
    if ( NULL != ptLogArgs->psFunction ) {
        psBuf[nOffset++] = '[';
        LOGSTRCAT(psBuf+nOffset, (char *)ptLogArgs->psFunction, nOffset);
        psBuf[nOffset++] = ']';
    }

    psBuf[nOffset++] = ' ';
    /* printf format string */
    LOGSTRCAT(psBuf+nOffset, ptLogArgs->psLogStr, nOffset);
    psBuf[nOffset] = '\0';

    return ( nLen - nOffset );
}

/**
 * logPrintfSessionStart
 * printf sesion name,start time
 *
 * @param ptLogAppender: Log Appender Way
 * @param psKey: Session Key Name
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logPrintfSessionStart(char *psKey)
{
    /* Get Time */
    char sBuf[LOG_BUF_LEN];
    memset(sBuf, '\0', sizeof(sBuf));
    time_t tSystime = time(NULL);
    struct tm *ptTm  = localtime(&tSystime);
    strftime(sBuf, LOG_BUF_LEN, "%Y%m%d%H%M%S", ptTm);
    
    /* Keep Session Key for next use */
    if ( NULL == psKey ) {
        sprintf(gsSessionKey, "%s%08d%06d", sBuf, getpid(), ++gnKeyNum);
        if (gnKeyNum >= 999999) {
            gnKeyNum = 0;
        }
    } else {
        strncpy(gsSessionKey, psKey, sizeof(gsSessionKey)-1);
    }

    return (0);
}

/**
 * logPrintfSessionEnd
 * printf session Name , End Time
 *
 * @param ptLogAppender: Log Appender Info
 * @param psKey: Session Key name
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logPrintfSessionEnd(LogAppender *ptLogAppender, char *psKey)
{
    /* Get Time */
    char sBuf[LOG_BUF_LEN];
    memset(sBuf, '\0', sizeof(sBuf));
    time_t tSystime = time(NULL);
    struct tm *ptTm  = localtime(&tSystime);
    strftime(sBuf, LOG_BUF_LEN, "%Y/%m/%d %H:%M:%S", ptTm);
    
    /* Check Key */
    if ( NULL == psKey ) {
        psKey = gsSessionKey;
    }
    
    /* printf Session Header */
    if ('\0' == gsSessionKey[0]) {
            /*init no log*/
    } else {
        LogOutPutArgs(ptLogAppender,"====SESSION END :[%s] TIME [%s]====\n", psKey, sBuf);
    }
    
    return (0);
}

/**
 * logDoSessionStart
 * Check Session Status
 * Start: End Session; start Session
 * End: start Session
 *
 * @param ptLogAppender: Log Appender Info
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logDoSessionStart(LogAppender *ptLogAppender)
{
    int nRet;
    
    /* Make sure session is end */
    if ( LOG_SESSION_STATUS_START == ptLogAppender->nSessionStatus ) {
        nRet = logDoSessionEnd(ptLogAppender);
        if ( nRet != 0 ) {
            logLog("Session End Err!!\n");
            return (-1);
        }
    }
    
    /* Do session start funciton */
    if ( ptLogAppender->fLogSessionStart != NULL ) {
        nRet = ptLogAppender->fLogSessionStart(ptLogAppender->ptOtherArgs);
        if ( nRet != 0 ) {
            logLog("Session Start err!!\n");
            return (-1);
        }
    }
    ptLogAppender->nSessionStatus = LOG_SESSION_STATUS_START;
    
    return (0);
}

/**
 * logDoSessionEnd
 * Check Session Status
 * Start: End Session:
 * End: Do thing
 *
 * @param ptLogAppender: Log Appender Info
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logDoSessionEnd(LogAppender *ptLogAppender)
{
    int nRet;
    
    /* If Session is End do Nothing */
    if ( LOG_SESSION_STATUS_END == ptLogAppender->nSessionStatus ) {
        return (0);
    }
    
    /* Do Session End Function */
    if ( ptLogAppender->fLogSessionEnd != NULL ) {
        nRet = ptLogAppender->fLogSessionEnd(ptLogAppender->ptOtherArgs);
        if ( nRet != 0 ) {
            logLog("Session End Err!!\n");
            return (-1);
        }
    }
    ptLogAppender->nSessionStatus = LOG_SESSION_STATUS_END;
    
    return (0);
}

/**
 * logGetDebugString
 * Get Debug String Mode From File
 * Sed DebugString Fucntion to LogAllCfg
 *
 * @param ptLogAllCfg: Log Al Config
 * @param psFile: Source File
 * @param psKey: Source Key
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logGetDebugString(LogAllCfg *ptLogAllCfg, char *psFile, char *psGroup)
{
    /* Get Debug String Fucntion Name */
    char sTemp[LOG_BUF_LEN];
    logIniDefRead(psGroup, LOG_DEBUG_STRING_KEY, NULL, LOG_DEBUG_STRING_NEW, sTemp, LOG_BUF_LEN, psFile);
    
    /* Set Fucntion To ptLogAllCfg */
    if ( 0 == strcmp(sTemp, LOG_DEBUG_STRING_NEW) ) {
        ptLogAllCfg->fDebugString = logNewDebugString;
    } else if ( 0 == strcmp(sTemp, LOG_DEBUG_STRING_OLD) ){
        ptLogAllCfg->fDebugString = logOldDebugString;
    } else {
        logLog("Key[%s.%s] values[%s] not define!! must be [New][Old] !!\n", psGroup, LOG_DEBUG_STRING_KEY, sTemp);
        return (-1);
    }
    
    /* Get Debug String print format */
    char sFlag[LOG_BUF_LEN];
    logIniDefRead(psGroup, LOG_DEBUG_PRINT_KEY, NULL, LOG_DEBUG_PRINT_DEFAULT_NAME, sFlag, LOG_BUF_LEN, psFile);
    
    if ( 0 == strcmp(sFlag, LOG_DEBUG_PRINT_NO_NAME) ) {
        ptLogAllCfg->caDebug = LOG_DEBUG_PRINT_NO;
    } else if ( 0 == strcmp(sFlag, LOG_DEBUG_PRINT_ALL_NAME) ) {
        ptLogAllCfg->caDebug = LOG_DEBUG_PRINT_ALL;
    } else {
        logLog("Key[%s.%s] values[%s] not define!! must be [No][All] !!\n", psGroup, LOG_DEBUG_PRINT_KEY, sFlag);
        return (-1);
    }
    
    return (0);
}

/**
 * logNewDebugString
 * New Debug String Way
 * Printf Line 20 Word
 * Every Word has Two Line
 *
 * @param ptLogAllCfg: Log All Info
 * @param psFile: File Name
 * @param nLine: File Line
 * @param psFunction: Function Name
 * @param psStr: String
 * @param psStrName: String Name
 * @param nStrLen: String Len
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logNewDebugString(LogAllCfg *ptLogAllCfg, char *psFile, int nLine, const char *psFunction, char *psStr, char *psStrName, int nStrLen)
{
    /* set file not flush */
    giIsFlush = 0;

    /* Printf Head */
    DebugStringInfo(ptLogAllCfg, LOG_LEVEl_DEBUG_STRING, psFile, nLine, psFunction, 
               "==================[NAME:%-20.20s][LEN:%08d]====================\n", psStrName, nStrLen);
    
#define DEBUG_STRING_NUM_LEN  8
#define DEBUG_STRING_LINE_NUM 20
    
    /* Printf Body */
    char sHex[LOG_BUF_LEN];
    char sOrg[LOG_BUF_LEN];
    char sLine[LOG_BUF_LEN];
    int i = 0;
	int j = 0;
    int nChinese = LOG_FALSE;
    sLine[0] = ' ';
	for ( ; i < nStrLen; ++i ){
        /* Get Offset */
        int nOffset = j*3+(j>(DEBUG_STRING_LINE_NUM/2-1));
        
        /* Printf Hex */
        /* sprintf( sHex+nOffset,  "%02X ", (unsigned char)psStr[i]); */
        locSetHex(sHex+nOffset, (unsigned char)psStr[i]);
		
        /* Printf Org */
        if ( LOG_TRUE == nChinese ) {
            locMemCopy( sOrg+nOffset, "   ", 3);
            nChinese = LOG_FALSE;
        } else if ( (unsigned char)psStr[i] >= 0x81 &&
             (unsigned char)psStr[i] <= 0xFE &&
             i + 1 < nStrLen                 && 
             (unsigned char)psStr[i+1] >= 0x40 &&
             (unsigned char)psStr[i+1] <= 0xFE &&
             (unsigned char)psStr[i+1] != 0x7F
            ) {
            /* Chinese */
            locMemCopy( sOrg+nOffset, &(psStr[i]), 2);
            locMemCopy( sOrg+nOffset+2, " ", 1);
            nChinese = LOG_TRUE;
        } else if ( isprint( psStr[i] ) ){
            sOrg[nOffset] = psStr[i];
            sOrg[nOffset+1] = ' ';
            sOrg[nOffset+2] = ' ';
            sOrg[nOffset+3] = '\0';
		} else if ( '\0' == psStr[i] ) {
            locMemCopy( sOrg+nOffset, "\\0 ", 3);
        } else if ( '\n' == psStr[i] ) {
            locMemCopy( sOrg+nOffset, "\\n ", 3);
        } else if ( '\r' == psStr[i] ) {
            locMemCopy( sOrg+nOffset, "\\r " , 3);
        } else if ( '\t' == psStr[i]) {
            locMemCopy( sOrg+nOffset, "\\t ", 3);
        } else {
			locMemCopy( sOrg+nOffset, ".. ", 3);
		}
        
        /* Printf Line */
        if ( DEBUG_STRING_LINE_NUM/2-1 == j || DEBUG_STRING_LINE_NUM-1 == j) {
            if ( LOG_TRUE == nChinese ) {
                locMemCopy( sLine+nOffset+1, "----" , 4);
            } else {
                locMemCopy( sLine+nOffset+1, "--  " , 4);
            }
        } else {
            if ( LOG_TRUE == nChinese ) {
                locMemCopy( sLine+nOffset+1, "---" , 3);
            } else {
                locMemCopy( sLine+nOffset+1, "-- " , 3);
            }
        }

        if ( DEBUG_STRING_LINE_NUM/2-1 == j ) {
            sHex[DEBUG_STRING_LINE_NUM/2*3] = ' ';
            sHex[DEBUG_STRING_LINE_NUM/2*3+1] = '\0';
            sOrg[DEBUG_STRING_LINE_NUM/2*3] = ' ';
            sOrg[DEBUG_STRING_LINE_NUM/2*3+1] = '\0';
        }

        /* Change Offset */
		if ( DEBUG_STRING_LINE_NUM == ++j || i + 1 >= nStrLen) {
			DebugStringInfo(ptLogAllCfg, LOG_LEVEl_DEBUG_STRING, psFile, nLine, psFunction, 
                       "%0*d:%-*s:%0*d\n", DEBUG_STRING_NUM_LEN, i+1-j, DEBUG_STRING_LINE_NUM*3+1, sHex, DEBUG_STRING_NUM_LEN, i);
            DebugStringInfo(ptLogAllCfg, LOG_LEVEl_DEBUG_STRING, psFile, nLine, psFunction, 
                       "%0*d:%-*s:%0*d\n", DEBUG_STRING_NUM_LEN, i+1-j, DEBUG_STRING_LINE_NUM*3+1, sOrg, DEBUG_STRING_NUM_LEN, i);
            DebugStringInfo(ptLogAllCfg, LOG_LEVEl_DEBUG_STRING, psFile, nLine, psFunction, 
                       "--------%-*s--------\n", (DEBUG_STRING_LINE_NUM+1)*3, sLine);
			j=0;
            
            /* Printf Frist */
            if ( LOG_TRUE == nChinese ) {
                sLine[0] = '-';
            } else {
                sLine[0] = ' ';
            }

		}
   	}
    
    /* Printf End , set file to flush*/
    giIsFlush = 1;
    DebugStringInfo(ptLogAllCfg, LOG_LEVEl_DEBUG_STRING, psFile, nLine, psFunction, 
               "===============================================================================\n");
    
#undef DEBUG_STRING_NUM_LEN
#undef DEBUG_STRING_LINE_NUM
    
    return (0);
}

/**
 * memoy
 */
static void locMemCopy(char *psOutBuf, char *psInBuf, int iLen)
{
    memcpy(psOutBuf, psInBuf, iLen);
    psOutBuf[iLen] = '\0';
}

/**
 *  set hex
 */
static void locSetHex(char *psBuf, unsigned char caChar)
{
    unsigned int i = 0;

    i = caChar / 16;
    if (i < 10) {
        psBuf[0] = '0' + i;
    } else {
        psBuf[0] = 'A' + i - 10;
    }

    i = caChar % 16;
    if (i < 10) {
        psBuf[1] = '0' + i;
    } else {
        psBuf[1] = 'A' + i - 10;
    }

    psBuf[2] = ' ';
    psBuf[3] = '\0';
}

/**
 * logOldDebugString
 * Print Debug String in old Way
 *
 * @param ptLogAllCfg: Log All Info
 * @param psFile: File Name
 * @param nLine: File Line
 * @param psFunction: Function Name
 * @param psStr: String
 * @param psStrName: String Name
 * @param nStrLen: String Len
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logOldDebugString(LogAllCfg *ptLogAllCfg, char *psFile, int nLine, const char *psFunction, char *psStr, char *psStrName, int nStrLen)
{
    /* set file not flush */
    giIsFlush = 0;

    /* Printf Head */
    DebugStringInfo(ptLogAllCfg, LOG_LEVEl_DEBUG_STRING, psFile, nLine, psFunction, 
               "-------------------------------[NAME:%-20.20s][LEN:%08d]-------------------------------\n", psStrName, nStrLen);
    
#define DEBUG_STRING_NUM_LEN  8
#define DEBUG_STRING_LINE_NUM 20
    
    /* Printf Body */
    char sHex[LOG_BUF_LEN];
    char sOrg[LOG_BUF_LEN];
    int i = 0;
	int j = 0;
	
	for (; i<nStrLen; ++i){
		sprintf( &sHex[j*3+(j>(DEBUG_STRING_LINE_NUM/2-1))],  "%02X ", (unsigned char)psStr[i]);
		if ( isprint( psStr[i] ) ){
			sOrg[j+(j>(DEBUG_STRING_LINE_NUM/2-1))] = psStr[i];
		} else {
			sOrg[j+(j>(DEBUG_STRING_LINE_NUM/2-1))] = '.';
		}
		
		j++;
		if ( DEBUG_STRING_LINE_NUM == j ) {
            sHex[DEBUG_STRING_LINE_NUM/2*3] = ' ';
            sOrg[DEBUG_STRING_LINE_NUM/2] = ' ';
            sOrg[DEBUG_STRING_LINE_NUM+1] = '\0';
			DebugStringInfo(ptLogAllCfg, LOG_LEVEl_DEBUG_STRING, psFile, nLine, psFunction, 
                       "%0*d: %s %s :%0*d\n", DEBUG_STRING_NUM_LEN, i+1-j, sHex, sOrg, DEBUG_STRING_NUM_LEN, i);
			j=0;
		}
   	}
    
   	if ( j ) {
        sHex[DEBUG_STRING_LINE_NUM/2*3] = ' ';
        sOrg[DEBUG_STRING_LINE_NUM/2] = ' ';
        sOrg[j+(j>(DEBUG_STRING_LINE_NUM/2-1))] = '\0';
		DebugStringInfo(ptLogAllCfg, LOG_LEVEl_DEBUG_STRING, psFile, nLine, psFunction, 
                   "%0*d: %-*s %-*s :%0*d\n", DEBUG_STRING_NUM_LEN, i-j, DEBUG_STRING_LINE_NUM*3+1, sHex, DEBUG_STRING_LINE_NUM+1, sOrg, DEBUG_STRING_NUM_LEN, i);
   	}
    
    /* Printf End , set file to flush*/
    giIsFlush = 1;
    DebugStringInfo(ptLogAllCfg, LOG_LEVEl_DEBUG_STRING, psFile, nLine, psFunction, 
               "-------------------------------------------------------------------------------------------------------\n");
    
#undef DEBUG_STRING_NUM_LEN
#undef DEBUG_STRING_LINE_NUM
    
    return (0);
}

/**
 * DebugStringInfo
 * Debug String INfo
 *
 * @param ptLogAllCfg: All
 * @param nLevel: Level
 * @param psFile: File
 * @param nLine: Line
 * @param psFunction: Function
 * @param psLogStr: psLogStr
 * @param ...: args
 *
 * @return >0  : ok
 *         <0  : err
 */
static int DebugStringInfo(struct LogAllCfg *ptLogAllCfg, int nLevel, char *psFile, int nLine, const char *psFunction, char *psLogStr, ...)
{
    int nRet = 0;
    va_list tVaList;
	va_start(tVaList, psLogStr);
    
    if ( LOG_DEBUG_PRINT_ALL == ptLogAllCfg->caDebug ) {
        nRet = LogInfoVArgs(ptLogAllCfg, nLevel, NULL, psFile, nLine, psFunction, psLogStr, tVaList);
    } else {
        LogCfgIdx *p = ptLogAllCfg->ptLogCfgIdx[nLevel];
        for (; p!=NULL; p=p->ptNext) {
            va_list ap;
            va_copy(ap, tVaList);
            nRet = LogOutPut(NULL, p->ptLogCfg->ptLogAppender, psLogStr, ap);
            va_end(ap);
            if ( nRet != 0 ) {
                logLog("Config[%s] printf err!!\n", p->ptLogCfg->sCfglName);
                break;
            }
        }
    }
    
    va_end(tVaList);
    
    return ( nRet );
}

/**
 * logGetSizeNum
 * Get File Max Size
 * Get Init String define form config file
 * convert string to int size
 * size is byte, but string can be 'k/m/g'
 *
 * @param psFile: config file
 * @param psKey: config key
 *
 * @return >0  : ok
 *         <0  : err
 */
static unsigned long logGetSizeNum(char *psFile, char *psPkg, char *psGroup)
{
    /* Get Size String Form ini file */
    char sSize[LOG_BUF_LEN];
    logIniDefRead(psGroup, psPkg, LOG_LEVEL_SIZE_KEY, LOG_LEVEL_SIZE_DEFAULT, sSize, LOG_BUF_LEN, psFile);
    
    /* Get Org Size */
    unsigned long lSize = atol(sSize);
    if ( lSize <= 0 ) {
        logLog("Config[%s.%s.%s] Value[%s] err!! Must Be Num!!\n", psGroup, psPkg, LOG_LEVEL_SIZE_KEY, sSize);
        return (0);
    }
    
    /* Get Real Size */
    int i = 0;
    for ( ; sSize[i] != '\0'; ++i) {
        if ( ( sSize[i] < '0' || sSize[i] > '9' ) && sSize[i] != ' ' ){
            break;
        }
    }
    switch ( sSize[i] ) {
        case 'k':
        case 'K':
            lSize *= LOG_SIZE_KB;
            break;

        case 'm':
        case 'M':
            lSize *= LOG_SIZE_MB;
            break;

        case 'g':
        case 'G':
            lSize *= LOG_SIZE_GB;
            break;
            
        case '\0':
            /* Byte */
            break;

        default:
            logLog("Config[%s.%s.%s] Value[%s] Err!! Must be [k,m,g]!!\n", psGroup, psPkg, LOG_LEVEL_SIZE_KEY, sSize);
            return (0);
            break;
    }
    
    if ( lSize < LOG_SIZE_KB * 50 ) {
        logLog("Warning: FileSize[%ld] is too small, auto set to [%d] !!\n", lSize, LOG_SIZE_KB * 50);
        lSize = LOG_SIZE_MB;
    }
    
    return (lSize);
}

/**
 * GetToken
 * Get Token 
 *
 * @param psBuf: Buf 
 * @param ppsValue: Return Value
 * @param pnValueLen: Value Len
 * @param pnType: Type
 *
 * @return >0  : ok
 *         NULL: err
 */
static char * GetToken(char *psBuf, char **ppsValue, int *pnValueLen, int *pnType)
{
    /* End */
    if ( '\0' == psBuf[0] ) {
        return ( NULL );
    }
    
    /* Begin */
    int i;
    char caTag;
    if ( '$' == psBuf[0] && '{' == psBuf[1] ) {
        *ppsValue = psBuf + 2;
        *pnType = LOG_TYPE_VAR;
        caTag = '}';
        i = 2;
    } else if ( '$' == psBuf[0] && '(' == psBuf[1] ) {
        *ppsValue = psBuf + 2;
        *pnType = LOG_TYPE_ENV;
        caTag = ')';
        i = 2;
    } else {
        *ppsValue = psBuf;
        *pnType = LOG_TYPE_STRING;
        caTag = '$';
        i = 0;
    }
    
    /* scanf tag */
    for ( ; psBuf[i]!='\0'; ++i) {
        if ( psBuf[i] == caTag ) {
            break;
        }
    }
    
    /* Set Len */
    *pnValueLen = i;
    if ( LOG_TYPE_VAR == *pnType ) {
        *pnValueLen = i - 2;
        if ( psBuf[i] != '\0' ) {
            psBuf[i] = '\0';
        }
        ++i;
    }
    
    return ( psBuf + i );
}

/**
 * logGetGlobalVar
 * Get Globale Value
 *
 * @param psCfgFileName: Config FIle name
 * @param psKey: Global Key Name
 * @param psBuffer: Buffer
 * @param nNum: Buffer Size
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logGetGlobalVar(char *psCfgFileName, char *psKey, char *psBuffer, int nNum)
{
    /* Temp File Name */
    int nRet = cfgGetStrL(LOG_GLOBAL_SECTION_NAME, psKey, NULL, psBuffer, nNum, psCfgFileName);
    if ( nRet < 0 ) {
        logLog("Get general Item[%s] From File[%s] Err[%d]!!\n", psKey, psCfgFileName, nRet);
        return (-1);
    }
    
    return ( strlen(psBuffer) );
}

/**
 * logGetBackupNum
 * Get Back up Num
 * If Value is not num, set to defautl
 *
 * @param psFile: Config File name
 * @param psPkg: Pkg Name
 * @param psGroup: Group Name
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logGetBackupNum(char *psFile, char *psPkg, char *psGroup)
{
    /* Get Size String Form ini file */
    char sBackUpNum[LOG_BUF_LEN];
    logIniDefRead(psGroup, psPkg, LOG_LEVEL_MAX_BACKUP_INDEX_KEY, LOG_LEVEL_MAX_BACKUP_INDEX_DEFAULT, sBackUpNum, LOG_BUF_LEN, psFile);
    
    /* Convert To Num */
    int nMaxIndex = atoi(sBackUpNum);
    if ( 0 == nMaxIndex && sBackUpNum[0] != '0' ) {
        logLog("Get Max BackUpIndex Err[%s] set to default[%s]!!\n", sBackUpNum, LOG_LEVEL_MAX_BACKUP_INDEX_DEFAULT);
        nMaxIndex = 1;
    }
    
    return (nMaxIndex);
}

/**
 * logGetTimePattern
 * Get Time Pattern
 * Get Time String Form Config File
 * Must be folling string
 * yyyy-MM: every Month
 * yyyy-ww: every Weeker
 * yyyy-MM-dd: every Day
 * yyyy-MM-dd-a: every Day twice (may be not do this time)
 * yyyy-MM-dd-HH: every hour
 * yyyy-MM-dd-HH-mm: every minter
 * convert string to time printf format
 *
 * @param psFile: config File
 * @param psKey: config key
 *
 * @return >0  : ok
 *         NULL: err
 */
static int logGetTimePattern(char *psFile, char *psPkg, char *psGroup, int *pnBackUpType)
{
    /* Get Time Pattern */
    char sTimePattern[LOG_BUF_LEN];
    logIniDefRead(psGroup, psPkg, LOG_LEVEL_TIME_PATTERN, LOG_LEVEL_TIME_PATTERN_DAILY, sTimePattern, LOG_BUF_LEN, psFile);
    
    /* Get Time pattern */
    int nBackUpType = 0;
    if ( 0 == strcmp(sTimePattern, LOG_LEVEL_TIME_PATTERN_MONTHLY) ) {
        nBackUpType = LOG_LEVEL_TIME_PATTERN_MONTHLY_NUM;
    } else if ( 0 == strcmp(sTimePattern, LOG_LEVEL_TIME_PATTERN_DAILY) ) {
        nBackUpType = LOG_LEVEL_TIME_PATTERN_DAILY_NUM;
    } else if ( 0 == strcmp(sTimePattern, LOG_LEVEL_TIME_PATTERN_HOUR) ) {
        nBackUpType = LOG_LEVEL_TIME_PATTERN_HOUR_NUM;
    } else if ( 0 == strcmp(sTimePattern, LOG_LEVEL_TIME_PATTERN_MINUTES) ) {
        nBackUpType = LOG_LEVEL_TIME_PATTERN_MINUTES_NUM;
    } else {
        logLog("Config[%s.%s.%s] Values[%s] is err!! Must Be blow list\n", psGroup, psPkg, LOG_LEVEL_TIME_PATTERN, sTimePattern);
        logLog(" yyyy-MM: every Month\n");
        logLog(" yyyy-MM-dd: every Day\n");
        logLog(" yyyy-MM-dd-HH: every hour\n");
        logLog(" yyyy-MM-dd-HH-mm: every minter\n");
        logLog(" yyyy-MM-dd-HH-mm-ss: every second\n");
        return (-1);
    }
    
    /* Set Return back up time */
    if ( pnBackUpType != NULL ) {
        *pnBackUpType = nBackUpType;
    }
    
    return (0);
}

/**
 * logGetLastBackUpTime
 * Get Last Back Up Time From System Time
 *
 * @param nBackUpType: Back Up Type
 * @param ptLastBakcUpTime: return value LastbackUpTime
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logGetLastBackUpTime(int nBackUpType, time_t *ptLastBakcUpTime)
{
    /* Get SysTime */
    time_t tSystime = time(NULL);
    struct tm *ptTm  = localtime(&tSystime);
    
    /* Get Min Back Up Time */
    switch ( nBackUpType ) {
        case LOG_LEVEL_TIME_PATTERN_MONTHLY_NUM:
            ptTm->tm_mday  = 1;
            ptTm->tm_hour  = 0;
            ptTm->tm_min   = 0;
            ptTm->tm_sec   = 0;
            break;
            
        case LOG_LEVEL_TIME_PATTERN_DAILY_NUM:
            ptTm->tm_hour  = 0;
            ptTm->tm_min   = 0;
            ptTm->tm_sec   = 0;
            break;
        
        case LOG_LEVEL_TIME_PATTERN_HOUR_NUM:
            ptTm->tm_min   = 0;
            ptTm->tm_sec   = 0;
            break;

        case LOG_LEVEL_TIME_PATTERN_MINUTES_NUM:
            ptTm->tm_sec   = 0;
            break;
            
        default:
            logLog("Back Up Time Type[%d] err!!\n", nBackUpType);
            return (-1);
            break;
    }
    
    *ptLastBakcUpTime = mktime(ptTm);
    
    return (0);
}

/**
 * logGetNextBackUpTime
 * Get Next Back Up Time From System Time
 *
 * @param nBackUpType: Back Up Type
 * @param ptLastBakcUpTime: return value LastbackUpTime
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logGetNextBackUpTime(int nBackUpType, time_t *ptNextBakcUpTime)
{
    time_t tSystime = time(NULL);
    struct tm *ptTm  = localtime(&tSystime);
    
    /* Get Min Back Up Time */
    switch ( nBackUpType ) {
        case LOG_LEVEL_TIME_PATTERN_MONTHLY_NUM:
            ptTm->tm_mon   += 1;
            ptTm->tm_mday  = 1;
            ptTm->tm_hour  = 0;
            ptTm->tm_min   = 0;
            ptTm->tm_sec   = 0;
            break;
            
        case LOG_LEVEL_TIME_PATTERN_DAILY_NUM:
            ptTm->tm_mday  += 1;
            ptTm->tm_hour  = 0;
            ptTm->tm_min   = 0;
            ptTm->tm_sec   = 0;
            break;
            
        case LOG_LEVEL_TIME_PATTERN_HOUR_NUM:
            ptTm->tm_hour  += 1;
            ptTm->tm_min   = 0;
            ptTm->tm_sec   = 0;
            break;
            
        case LOG_LEVEL_TIME_PATTERN_MINUTES_NUM:
            ptTm->tm_min   += 1;
            ptTm->tm_sec   = 0;
            break;
            
        default:
            logLog("Back Up Time Type[%d] err!!\n", nBackUpType);
            return (-1);
            break;
    }
    
    *ptNextBakcUpTime = mktime(ptTm);
    
    return (0);
}

/**
 * logGetNextFormat
 * log Get Next Format Info
 * split by char '%'
 *
 * @param psBuf: format buf len
 * @param pnOffset: offset 
 *
 * @return >0  : ok
 *         NULL: err
 */
static char * logGetNextFormat(char *psBuf, LogFormatArgs  *ptLogFormatArgs)
{
    /* Check Args */
    if ( NULL == psBuf || '\0' == *psBuf ) {
        return (NULL);
    }
    
    /* Args */
    if ( '%' == psBuf[0] ) {
        /* No {...} */
        if ( psBuf[2] != '{' ) {
            ptLogFormatArgs->caName = psBuf[1];
            ptLogFormatArgs->psInfo = NULL;
            ptLogFormatArgs->nLen   = 0;
            
            return (psBuf+2);
        }
        
        /* Has {...} */
        char *p = strchr(psBuf, '}');
        if ( NULL == p ) {
            logLog("%s:[%s] err!! Missing '}'\n", LOG_LEVEL_PATTERN_KEY, psBuf);
            return (NULL);
        }
        *p = '\0';
        ptLogFormatArgs->caName = psBuf[1];
        ptLogFormatArgs->psInfo = psBuf + 3;
        ptLogFormatArgs->nLen   = strlen(ptLogFormatArgs->psInfo);
        return (p+1);
    }
    
    /* String */
    char *p = strchr(psBuf, '%');
    ptLogFormatArgs->caName = LOG_FORMAT_STRING;
    ptLogFormatArgs->psInfo = psBuf;
    
    if ( NULL == p ) {
        ptLogFormatArgs->nLen = strlen(psBuf);
    } else {
        ptLogFormatArgs->nLen = p - psBuf;
    }
    
    return ( psBuf + ptLogFormatArgs->nLen );
}

/**
 * logBackUpFile
 * bakc Up File By TimePattern
 * if back up file is exites
 * make file name -1,-2,...
 *
 * @param psFile: Back File Name
 * @param psTimePatterh: Time Pattern
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logBackUpFile(char *psFile, unsigned long lSize, int iBackType)
{
    int nRet;
    struct stat tStatBuf;

    /* check again */
    nRet = stat(psFile, &tStatBuf);
    if (0 != nRet) {
        return 0;
    }

    if (tStatBuf.st_size < lSize) {
        return 0;
    }
    
    /* Get File Name */
    char *psType = strrchr(psFile, '.');
    if ( psType != NULL ) {
        *psType = '\0';
    }
    
    /* Check Back Up Is OK */
    char sBackUpFile[LOG_BUF_LEN];
    memset(sBackUpFile, '\0', sizeof(sBackUpFile));
    nRet = logGetBackUpFileName(psFile, iBackType, sBackUpFile);
    
    /* reconver the file name */
    if ( psType != NULL ) {
        *psType = '.';
    }
    
    if ( nRet != 0 ) {
        logLog("logGetBackUpFileName err!! File[%s] timepattern[%d]!!\n", psFile, iBackType);
        return (-1);
    }
    
    /* back Up file */
    nRet = rename(psFile, sBackUpFile);
    if ( nRet != 0 ) {
        logLog("Back up File[%s] To File[%s] Err[%s]!!\n", psFile, sBackUpFile, strerror(errno));
        return (-1);
    }
    
    return (0);
}

/**
 * logBackUpRollingFile
 * Back Up Rolliong File
 * if Max Index is 0 not bakcup
 * get last backup file name
 * back up to last backup file
 *
 * @param psFileName: log File Name
 * @param nMaxIndex: Max Index
 * @param nNowIndex: now Index
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logBackUpRollingFile(char *psFileName, unsigned long lSize, int nMaxIndex, int *pnNowIndex)
{
    int iRet = 0;

    /* Not Back Up */
    if ( 0 == nMaxIndex ) {
        return 0;
    }

    /* Check again */
    struct stat tStatBuf;
    memset(&(tStatBuf), '\0', sizeof(tStatBuf));
    iRet = stat(psFileName, &tStatBuf);
    if (0 != iRet) {
        return 0;
    }

    if ( tStatBuf.st_size < lSize ){
        return 0;
    }

    /* Check Back Up Is OK */
    char sBackUpFile[LOG_MSG_BUF_LEN];
    memset(sBackUpFile, '\0', sizeof(sBackUpFile));
    *pnNowIndex = GetLastBackupFileName(psFileName, nMaxIndex, *pnNowIndex, sBackUpFile);
    
    /* back Up file */
    iRet = rename(psFileName, sBackUpFile);
    if ( iRet != 0 ) {
        logLog("Back up File[%s] To File[%s] Err[%s]!!\n", psFileName, sBackUpFile, strerror(errno));
        return (-1);
    }
    
    return (0);
}

/**
 * logGetBackUpFileName
 * Make back uo file name by time
 * make tiem pattern after file name
 * if file is exist make num after file
 *
 * @param psFile: File Org name
 * @param psTimePattern: Time Pattern
 * @param psBackUpFile: return back up file name
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logGetBackUpFileName(char *psFile, int iBackType, char *psBackUpFile)
{
    int nRet;
    
    /* Get Time Pattern */
    time_t tSystime = time(NULL);
    struct tm *ptTm  = localtime(&tSystime);
    int iOffset = 0;
    LOGSTRCAT(psBackUpFile, psFile, iOffset);
    psBackUpFile[iOffset++] = '.';

    do {
        iOffset += LogIntFormat(psBackUpFile+iOffset, 4, ptTm->tm_year + 1900);
        psBackUpFile[iOffset++] = '-';
        iOffset += LogIntFormat(psBackUpFile+iOffset, 2, ptTm->tm_mon + 1);
        if (LOG_LEVEL_TIME_PATTERN_MONTHLY_NUM == iBackType) {
            break;
        }
        psBackUpFile[iOffset++] = '-';

        iOffset += LogIntFormat(psBackUpFile+iOffset, 2, ptTm->tm_mday);
        if (LOG_LEVEL_TIME_PATTERN_DAILY_NUM == iBackType) {
            break;
        }
        psBackUpFile[iOffset++] = '-';

        iOffset += LogIntFormat(psBackUpFile+iOffset, 2, ptTm->tm_hour);
        if (LOG_LEVEL_TIME_PATTERN_HOUR_NUM == iBackType) {
            break;
        }
        psBackUpFile[iOffset++] = '-';

        iOffset += LogIntFormat(psBackUpFile+iOffset, 2, ptTm->tm_min);
    } while (0);
    
    int i = 0;
    for (; ; ++i) {
        /* Make Back Up file Name */
        if ( 0 == i ) {
            strcpy(psBackUpFile+iOffset, ".log");
        } else {
            sprintf(psBackUpFile+iOffset, ".%d.log", i);
        }
        
        /* check back file exist */
        nRet = access(psBackUpFile, LOG_ACCESS_CHECK_FILE_EXIST);
        if ( nRet != 0 ) {
            /* file is not exist */
            break;
        }
    }
    
    return (0);
}

/**
 * GetLastBackupFileName
 * Get Last Back Up File Name
 * If Now Index is not -1 then add 1 is bakcup file Name
 * else get Last Backup File Name
 * get min modify file file
 *
 * @param psFile: File name, no file type(.log)
 * @param nMaxIndex: Max Index
 * @param nNowIndex: Now Index
 * @param psBackUpFileName: return Back Up File Name
 *
 * @return >0  : ok
 *         <0  : err
 */
static int GetLastBackupFileName(char *psFile, int nMaxIndex, int nNowIndex, char *psBackUpFileName)
{
    int nRet;
    
    /* Has Index */
    if ( nNowIndex != -1 ) {
        int nBackUpIndex = nNowIndex + 1;
        if ( nBackUpIndex >= nMaxIndex ) {
            nBackUpIndex = 1;
        }
        sprintf(psBackUpFileName, "%s.%d", psFile, nBackUpIndex);
        return (nBackUpIndex);
    }
    
    /* Get Index */
    int i = 1;
    time_t tLastBackUpTime = 0;
    for ( ; i<=nMaxIndex; ++i) {
        /* Get File Info */
        sprintf(psBackUpFileName, "%s.%d", psFile, i);
        struct stat tStatBuf;
        memset(&(tStatBuf), '\0', sizeof(tStatBuf));
        nRet = stat(psBackUpFileName, &tStatBuf);
        if ( nRet != 0 ) {
            /* File Not Exit */
            return (i);
        }
        
        /* This File is Old Then Last One */
        if ( tStatBuf.st_mtime < tLastBackUpTime ) {
            return (i);
        }
        
        /* Restore Last File */
        tLastBackUpTime = tStatBuf.st_mtime;
    }
    
    /* Frist File Is Last File */
    sprintf(psBackUpFileName, "%s.1", psFile);
    return (1);
}

/**
 * GetUserDefineAppenderInitFunction
 * Get User Appender Init Function
 * Get User Appneder Form List
 * If Not Get From Config File
 *
 * @param psAppenderName: Appender Name
 * @param psFile: Config File
 *
 * @return >0  : ok
 *         NULL: err
 */
static FncInitUserAppender GetUserDefineAppenderInitFunction(char *psAppenderName, char *psFile)
{
    /* Get From List */
    UserAppenderPlug *ptUserAppenderPlug = FindUserAppenderDefine(gptUserAppenderDefine, psAppenderName);
    if ( ptUserAppenderPlug != NULL ) {
        return ( ptUserAppenderPlug->fInitAppender );
    }
    
    /* Init From Config File */
    ptUserAppenderPlug = GetInitAppenderFromCfgFile(psAppenderName, psFile);
    if ( NULL == ptUserAppenderPlug ) {
        logLog("Appender[%s] Not Define!! Please Define It Frist \n", psAppenderName);
        return ( NULL );
    }
    
    /* Add To List */
    gptUserAppenderDefine = AddUserAppenderDefine(gptUserAppenderDefine, psAppenderName, ptUserAppenderPlug);
    if ( NULL == gptUserAppenderDefine ) {
        logLog("AddUserAppenderDefine Add appender[%s] err!!\n", psAppenderName);
        UserAppenderPlugFree(ptUserAppenderPlug);
        return (NULL);
    }
    
    return ( ptUserAppenderPlug->fInitAppender );
}

/**
 * GetInitAppenderFromCfgFile
 * Get Appender File Name And Fucntion name
 * Invok the NewUserAppenderPlug New Plug
 *
 * @param psAppenderName: Appender Name
 * @param psFile: Config File Name
 *
 * @return >0  : ok
 *         NULL: err
 */
static UserAppenderPlug * GetInitAppenderFromCfgFile(char *psAppenderName, char *psFile)
{
    int nRet;
    
    char sDefaultPath[LOG_MSG_BUF_LEN];
    sprintf(sDefaultPath, "liblog_plug_%s.so", psAppenderName);
    
    /* Get File Path */
    char sFilePath[LOG_MSG_BUF_LEN];
    nRet = logIniDefRead(NULL, psAppenderName, LOG_PLUG_PATH_NAME, sDefaultPath, sFilePath, LOG_MSG_BUF_LEN, psFile);
    if ( nRet < 0 ) {
        logLog("User Appender[%s] Path[%s.%s] Get Err [%d]!!\n", psAppenderName, psAppenderName, LOG_PLUG_PATH_NAME, nRet);
        return (NULL);
    }
    
    /* Get Fucntion Name */
    char sFunction[LOG_BUF_LEN];
    nRet = logIniDefRead(NULL, psAppenderName, LOG_PLUG_FUNCTION_NAME, LOG_PLUG_FUCNTION_DEFAULT, sFunction, LOG_BUF_LEN, psFile);
    if ( nRet < 0 ) {
        logLog("User Appender[%s] Function[%s.%s] Get Err[%d]", psAppenderName, psAppenderName, LOG_PLUG_FUNCTION_NAME, nRet);
        return (NULL);
    }
    
    return ( NewUserAppenderPlug(sFilePath, sFunction) );
}

/**
 * NewUserAppenderPlug
 * Init Plug
 * Get Dll Object
 * Get Function
 *
 * @param psFile: Dll Path
 * @param psFunction: Function Name
 *
 * @return >0  : ok
 *         NULL: err
 */
static UserAppenderPlug * NewUserAppenderPlug(char *psFile, char *psFunction)
{
    H_DllFile pObject = dllOpen(psFile);
    if ( NULL == pObject ) {
        logLog("Path[%s] Err:[%s]!!\n", psFile, dllErrStr());
        return (NULL);
    }
    
    FncInitUserAppender fInit = (FncInitUserAppender)dllGetFun(pObject, psFunction);
    if ( NULL == fInit ) {
        logLog("Fucntion [%s] Err:[%s]!!\n", psFunction, dllErrStr());
        dllClose(pObject);
        return (NULL);
    }
    
    UserAppenderPlug *p = (UserAppenderPlug *)malloc(sizeof(UserAppenderPlug));
    memset(p, '\0', sizeof(UserAppenderPlug));
    p->ptDllObject = pObject;
    p->fInitAppender = fInit;
    
    return (p);
}

/**
 * UserAppenderPlugFree
 * Fre User Appender Plug
 * CLose Dll Object
 *
 * @param ptUserAppenderPlug: UserAppederPlug Info
 *
 * @return >0  : ok
 *         <0  : err
 */
static int UserAppenderPlugFree(UserAppenderPlug *ptUserAppenderPlug)
{
    dllClose(ptUserAppenderPlug->ptDllObject);
    free(ptUserAppenderPlug);
    
    return (0);
}

/**
 * FindUserAppenderDefine
 * Find UserAppnederPlug By Name
 *
 * @param UserAppenderDefine: List Header
 * @param psAppenderName: Appender Name
 *
 * @return >0  : ok
 *         NULL: err
 */
static UserAppenderPlug * FindUserAppenderDefine(UserAppenderDefine * ptUserAppenderDefine, char *psAppenderName)
{
    if ( NULL == ptUserAppenderDefine || NULL == psAppenderName ) {
        return (NULL);
    }
    
    UserAppenderDefine *p = ptUserAppenderDefine;
    for ( ; p!=NULL; p=p->ptNext ) {
        if ( 0 == strcmp(psAppenderName, p->psName) ) {
            return ( p->ptUserAppenderPlug );
        }
    }
    
    return (NULL);
}

/**
 * AddUserAppenderDefine
 * Add UserAppenderPlug To List
 * user AddUserAppenderDefine of Plug
 * Add AddUserAppenderDefine to List Header
 *
 * @param ptUserAppenderDefine: List Header
 * @param psAppenderName: AppenderPlug Name
 * @param ptUserPlug: AppenderPlug Info
 *
 * @return >0  : ok
 *         NULL: err
 */
static UserAppenderDefine * AddUserAppenderDefine(UserAppenderDefine *ptUserAppenderDefine, char *psAppenderName, UserAppenderPlug *ptUserPlug)
{
    UserAppenderDefine * p = (UserAppenderDefine *)malloc(sizeof(UserAppenderDefine));
    memset(p, '\0', sizeof(UserAppenderDefine));
    p->psName             = strdup(psAppenderName);
    p->ptUserAppenderPlug = ptUserPlug;
    p->ptNext             = ptUserAppenderDefine;
    
    return ( p );
}

/**
 * AddLogCfgIdx
 * Add LogCfg To Index
 * If Index Is Not Init Then Init
 * If Index Has Init then Add Next To Index
 *
 * @param ptLogAllCfg: Log Config
 * @param nIndex: Index
 * @param psName: Config Name
 * @param ptLogCfg: Config Info
 *
 * @return >0  : ok
 *         <0  : err
 */
static int AddLogCfgIdx(LogAllCfg *ptLogAllCfg, int nIndex, char *psName, LogCfg *ptLogCfg)
{
    /* Check */
    if ( nIndex < 0 || nIndex >= ptLogAllCfg->nNum ) {
        logLog("Log Level Err [%d] Max is [%d]!!\n", nIndex, ptLogAllCfg->nNum);
        return (-1);
    }
    
    /* Add Muti Printf */
    LogCfgIdx *p = (LogCfgIdx *)malloc(sizeof(LogCfgIdx));
    memset(p, '\0', sizeof(LogCfgIdx));
    p->ptLogCfg = ptLogCfg;
    strncpy(p->sLevelName, psName, sizeof(p->sLevelName)-1);
    if (NULL == ptLogAllCfg->ptLogCfgIdx[nIndex]) {
        ptLogAllCfg->ptLogCfgIdx[nIndex] = p;
    } else {
        p->ptNext = ptLogAllCfg->ptLogCfgIdx[nIndex]->ptNext;
        ptLogAllCfg->ptLogCfgIdx[nIndex]->ptNext = p;
    }
    
    return (0);
}

/**
 * FreeLogCfgIdx
 * Free LogCfgIdx
 *
 * @param ptLogCfgIdx: Taget Free LogCfgIdx
 *
 * @return >0  : ok
 *         <0  : err
 */
static int FreeLogCfgIdx(LogCfgIdx *ptLogCfgIdx)
{
    LogCfgIdx *p = ptLogCfgIdx;
    while ( p!=NULL ) {
        LogCfgIdx *q = p->ptNext;
        free(p);
        p = q;
    }
    
    return (0);
}

/**
 * logDefaultInit
 * Init Log All Config In Default Way
 *
 * @param #: void
 *
 * @return >0  : ok
 *         NULL: err
 */
static LogAllCfg * logDefaultInit()
{
    /* For Old InterFace */
    if ( LOG_VERSION_OLD == gnVersion ) {
        logSetConfig(gpsLogFileName, glFileSize, gnMode);
        return (gptLogAllCfg);
    }
    
    LogFormat *ptLogFormat = (LogFormat *)malloc(sizeof(LogFormat));
    memset(ptLogFormat, '\0', sizeof(LogFormat));
    ptLogFormat->fLogPrintf  = LogSamplePrintf;
    ptLogFormat->ptOtherArgs = NULL;
    ptLogFormat->ptNext      = NULL;
    
    LogAppender *ptLogAppender = (LogAppender *)malloc(sizeof(LogAppender));
    memset(ptLogAppender, '\0', sizeof(LogAppender));
    ptLogAppender->tOld             = 0;
    ptLogAppender->fLogOutPut       = LogConsoleOutPut;
    ptLogAppender->fLogSessionStart = NULL;
    ptLogAppender->fLogSessionEnd   = NULL;
    ptLogAppender->nSessionFlag     = LOG_SESSION_FLAG_ON;
    ptLogAppender->nSessionStatus   = LOG_SESSION_STATUS_END;
    ptLogAppender->ptOtherArgs      = NULL;
    
    LogCfg *ptLogCfg = (LogCfg *)malloc(sizeof(LogCfg));
    memset(ptLogCfg, '\0', sizeof(LogCfg));
    ptLogCfg->ptFormat      = ptLogFormat;
    ptLogCfg->ptLogAppender = ptLogAppender;
    ptLogCfg->ptNext        = NULL;
    strcpy(ptLogCfg->sCfglName, LOG_LEVEL_CFG_DEFAULT);
    
    LogAllCfg *ptLogAllCfg = (LogAllCfg *)malloc(sizeof(LogAllCfg));
    memset(ptLogAllCfg, '\0', sizeof(LogAllCfg));
    ptLogAllCfg->nNum         = LOG_LEVEL_NUMBER_DEFAULT;
    ptLogAllCfg->ptLogCfgList = ptLogCfg;
    ptLogAllCfg->fDebugString = logNewDebugString;
    ptLogAllCfg->caDebug      = LOG_DEBUG_PRINT_DEFAULT;
    
    char sBuf[LOG_BUF_LEN];
    strncpy(sBuf, gpsLevelDefault, sizeof(sBuf)-1);
    logAddCfgLevel(ptLogAllCfg, ptLogCfg, sBuf);
    
    return (ptLogAllCfg);
}

/**
 * LogFreeFormat
 * Free Log Format Struct
 *
 * @param ptLogFormat: Format Struct
 *
 * @return >0  : ok
 *         <0  : err
 */
static int LogFreeFormat(LogFormat *ptLogFormat)
{
    LogFormat *p = ptLogFormat;
    while ( p!=NULL ) {
        LogFormat * q = p->ptNext;
        free(p->ptOtherArgs);
        free(p);
        p = q;
    }
    
    return (0);
}

/**
 * cvtStrNSplit
 * split string to short string
 * split by splitchar, the max num is nNum
 *
 * @param ppsStr: short string list
 * @param psLine: source string
 * @param psSplitChar: slplus char(can be many)
 * @param nNum: max num of split
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logStrNSplit(char **ppsStr, char *psLine, char *psSplitChar, int nNum)
{
    char *p;
    int n = 1;
    int ntemp;
    
    if ( NULL == psLine || NULL == ppsStr || NULL == psSplitChar){
        return (-1);
    }
    
    *ppsStr++ = p = psLine;
    while ( (p=strpbrk(p, psSplitChar)) && (n<nNum) ) {
        n++;
        *p++ = '\0';
        *ppsStr++ = p;
    }
    
    for ( ntemp = n; ntemp<nNum ; ++ntemp ){
        *ppsStr++ = "";
    }
    
    return (n);
}

/**
 * logStrNDup
 * Dump String To Malloc Buf
 * Need Free Outside
 *
 * @param psBuf: Message Buf
 * @param nLen: Message buf Len
 *
 * @return >0  : ok
 *         NULL: err
 */
static char * logStrNDup(char *psBuf, int nLen)
{
    if ( NULL == psBuf || nLen <= 0 ) {
        return (NULL);
    }
    
    char *psTemp = malloc(nLen+1);
    memset(psTemp, '\0', nLen+1);
    
    memcpy(psTemp, psBuf, nLen);
    
    return (psTemp);
}

/**
 * logIniItemRead
 * log Init config file read item
 * make Key by prg
 *
 * @param psSection: Sesion File
 * @param psPrg: Program Name
 * @param psType: Type Name
 * @param psItem: Item Name
 * @param psDefault: Default 
 * @param psOutBuf: Out Buf
 * @param nOutLen: Out Buf Len
 * @param psFile: Cfg File Name
 *
 * @return >0  : ok
 *         <0  : err
 */
static int logIniItemRead(char *psSection, char *psProgram, char *psType, char *psItem, char *psDefault, char *psOutBuf, int nOutLen, char *psFile)
{
    char sItemKey[LOG_BUF_LEN];
    memset(sItemKey, '\0', sizeof(sItemKey));
    AddKey(sItemKey, psProgram);
    AddKey(sItemKey, psType);
    AddKey(sItemKey, psItem);

    int nRet = cfgGetStrL(psSection, sItemKey, psDefault, psOutBuf, nOutLen, psFile);
    if ( nRet < 0 ) {
        logLog("Get Key[%s] Secion[%s] From File[%s] Err[%d]!!\n", sItemKey, psSection, psFile, nRet);
        logLog("Set Key[%s] To Defaulg Value[%s]\n", sItemKey, psOutBuf);
        return (0);
    }
    
    return ( nRet );
}

/**
 * AddKey
 * Add Key To Old Key
 * If Old Key is Blank Then Copy
 * Elds Add "."New Key to End Of Old Key
 *
 * @param psOldKey: Old Key Value
 * @param psNewKey: New Key Value
 *
 * @return >0  : ok
 *         NULL: err
 */
static char * AddKey(char *psOldKey, char *psNewKey)
{
    if ( NULL == psNewKey ) {
        return ( psOldKey );
    }
    
    if ( '\0' == psOldKey[0] ) {
        strcpy(psOldKey, psNewKey);
    } else {
        strcat(psOldKey, ".");
        strcat(psOldKey, psNewKey);
    }
    
    return (psOldKey);
}

/**
 * logCfgReplaceFile
 * replace file name
 * 
 * @return FUNC_OK  : ok
 *         FUNC_ERR : err
 */
static int logCfgReplaceFile(char *psFileName, int nNum, char *psGroup, char *psPkg, char *psKey, char *psDefault, char *psCfgFile)
{
    int nRet = 0;

    /* Get Tmp File Name */
    char sTmpFile[LOG_MSG_BUF_LEN];
    memset(sTmpFile, '\0', sizeof(sTmpFile));
    logIniDefRead(psGroup, psPkg, psKey, psDefault, sTmpFile, LOG_MSG_BUF_LEN, psCfgFile);
    
    /* File Name Is NULL */
    if ( '\0' == sTmpFile[0] ) {
        psFileName[0] = '\0';
        return (0);
    }
    
    /* File Path Is Ok */
    int nUsed = 0;
    if ( sTmpFile[0] != '/' ) {
        int iLen = logGetGlobalVar(psCfgFile, LOG_GLOBAL_PREFIX, psFileName, nNum);
        if ( iLen > 0 ) {
            psFileName[iLen] = '/';
            nUsed = iLen + 1;
        }

        nRet = access(psFileName, LOG_ACCESS_CHECK_FILE_EXIST);
        if (nRet != 0) {
            psFileName[0] = '\0';
            return (0);
        }
    }
    
    /* Get All Full Path */
    char * p = sTmpFile;
    while ( 1 ) {
        char *psBuf = NULL;
        int nBufLen = 0;
        int nType;
        
        p = GetToken(p, &(psBuf), &(nBufLen), &(nType));
        if ( NULL == p ) {
            break;
        }
        
        switch ( nType ) {
            case LOG_TYPE_VAR:
                nBufLen = 0;
                if ( 0 == strcmp(psBuf, "SECTION") ) {
                    strcpy(psFileName+nUsed, psGroup);
                    nBufLen = strlen(psGroup);
                } else if ( 0 == strcmp(psBuf, "PID") ) {
                    nBufLen = sprintf(psFileName+nUsed, "%d", getpid());
                } else if ( 0 == strcmp(psBuf, "APP") && NULL != gpsAppName ) {
                    nBufLen = sprintf(psFileName+nUsed, "%s", gpsAppName);
                } else {
                    char *psEnvString = getenv(psBuf);
                    if ( NULL != psEnvString ) {
                        strcpy(psFileName+nUsed, psEnvString);
                        nBufLen = strlen(psEnvString);
                    }
                }
                break;
                
            case LOG_TYPE_ENV:
                break;

            case LOG_TYPE_STRING:
                memcpy(psFileName+nUsed, psBuf, nBufLen);
                break;
                
            default:
                continue;
                break;
        }
        
        nUsed += nBufLen;
    }
    
    return (0);
}

/**
 * locCfgGetFileName
 *
 * @return >0  : ok
 *         <0  : err
 */
static char * locCfgGetFileName(char *psGroup, char *psPkg, char *psCfgFile)
{
    char sFileName[LOG_MSG_BUF_LEN];
    memset(sFileName, '\0', sizeof(sFileName));

    logCfgReplaceFile(sFileName, LOG_MSG_BUF_LEN, psGroup, psPkg, LOG_LEVEL_FILE_KEY, LOG_LEVEL_FILE_DEFAULT, psCfgFile);

    return strdup(sFileName);
}

static void _lockFile(FILE *ptFp)
{
    int iRet = 0;

    if (NULL == ptFp || stderr == ptFp) {
        return;
    }

    do {
		iRet = lockFileWriteW(fileno(ptFp), 0, SEEK_SET, 0);
		if (iRet < 0) {
			if (errno == EINTR) {
				continue;
			}

            break;
		}
		break;
	} while (1);
}

static void _unlockFile(FILE *ptFp)
{
    if (NULL == ptFp || stderr == ptFp) {
        return;
    }

    lockFileUnLock(fileno(ptFp), 0, SEEK_SET, 0);
}

static FILE * _FileOpen(char *psFileName, char *psType)
{
    FILE *ptFp = NULL;

    ptFp = fopen(psFileName, psType);
    if (NULL != ptFp) {
        return ptFp;
    }

    if (ENOENT != errno) {
        return NULL;
    }

    _MakeDir(psFileName);

    return fopen(psFileName, psType);
}

static void _MakeDir(char *psBuf)
{
    int iRet = 0;
    char sBuf[LOG_MSG_BUF_LEN];

    char *p = strrchr(psBuf, '/');
    if (NULL == p) {
        return;
    }

    memset(sBuf, '\0', sizeof(sBuf));
    memcpy(sBuf, psBuf, (int)(p - psBuf));

    iRet = access(sBuf, LOG_ACCESS_CHECK_FILE_EXIST);
    if (0 == iRet) {
        return;
    }

    _MakeDir(sBuf);
    mkdir(sBuf, S_IRWXU | S_IRWXG | S_IRWXO);

    return;
}

/*--------------------- End -------------------------------------*/
