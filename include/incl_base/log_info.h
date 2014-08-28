/*
 *
 *
 *  Log Head File.
 *
 *
 * FileName: log_info.h 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

#ifndef _LOG_INFO_H_20101020180818_
#define _LOG_INFO_H_20101020180818_

/*------------------------ Include files ------------------------*/
#include <stdarg.h>

#if 0
#pragma mark -
#pragma mark < Macro define >
#endif
/*--------------------- Macro define ----------------------------*/
#define LOG_GROUP_FLAG "$"
#define LOG_PRINT_LEN_CTRL 1024
#ifdef  __GNUC__
#define __LOGINFO_GNUC_EXT__ __attribute__((format (printf, 7, 8)))
#define __LOGINFO_GNUC_EXT2__ __attribute__((format (printf, 6, 7)))
#else
#define __LOGINFO_GNUC_EXT__
#define __LOGINFO_GNUC_EXT2__
#endif

/* FUNCTION */
#if !defined(__GNUC__) && !defined(__xlc__) && !defined(__xlC__)
#define __FUNCTION__ NULL
#endif

/* Log Normal Args */
#ifdef  LOG_GROUP_NAME
#define F_LOG_INFO(LEVEL) NULL, (LEVEL), LOG_GROUP_FLAG LOG_GROUP_NAME, __FILE__, __LINE__, __FUNCTION__
#else   
#define F_LOG_INFO(LEVEL) NULL, (LEVEL), NULL, __FILE__, __LINE__, __FUNCTION__
#endif
/* Log Err Name */
#define LOG_ERROR        1
#define LOG_WARNING      2
#define LOG_NORMAL       3
#define LOG_DEBUG        4
#define LOG_TRACE        5
#define LOG_DBS_TRACE    6
#define LOG_BIZ_WARNING  7
#define LOG_SYS_WARNING  8
#define LOG_SYS_ERROR    9
#define LOG_USER        10
#define LOG_PACKAGE     19

/* Log Level Args */
#ifndef   _TL_LOG_
#define LOG_ERR      F_LOG_INFO( LOG_ERROR )
#define LOG_WRN      F_LOG_INFO( LOG_WARNING )
#define LOG_NOR      F_LOG_INFO( LOG_NORMAL )
#define LOG_BUG      F_LOG_INFO( LOG_DEBUG )
#define LOG_TRC      F_LOG_INFO( LOG_TRACE )
#define LOG_DBS      F_LOG_INFO( LOG_DBS_TRACE )
#define LOG_BIZ_WRN  F_LOG_INFO( LOG_BIZ_WARNING )
#define LOG_SYS_WRN  F_LOG_INFO( LOG_SYS_WARNING )
#define LOG_SYS_ERR  F_LOG_INFO( LOG_SYS_ERROR )
#define LOG_USR      F_LOG_INFO( LOG_USER )
#define LOG_C11      F_LOG_INFO( 11 )
#define LOG_C12      F_LOG_INFO( 12 )
#define LOG_C13      F_LOG_INFO( 13 )
#define LOG_C14      F_LOG_INFO( 14 )
#define LOG_C15      F_LOG_INFO( 15 )
#define LOG_PKG      F_LOG_INFO( LOG_PACKAGE )
#endif

/* easy way to use logDebugString */
#define logDebug(s, n) logDebugString(NULL, 0, __FILE__, __LINE__, __FUNCTION__, (s), (#s), (n))
#define logCfgDebug(c, s, n) logCfgDebugString((c), __FILE__, __LINE__, __FUNCTION__, (s), (#s), (n))

/* session flag */
#define LOG_SESSION_FLAG_ON      1
#define LOG_SESSION_FLAG_OFF     2
#define LOG_SESSION_FLAG_NOPRINT 3

/* Ctl */
#define LOG_DEFAULT      -1
#define LOG_LEVEL       "LOG_LEVEL"
#define LOG_LEVEL_NAME  "LOG_LEVEL_NAME"
#define LOG_USER_CLEAN  "LOG_USER_CLEAN"
#define LOG_USER_OUTPUT "LOG_USER_OUTPUT"

#if 0
#pragma mark -
#pragma mark < Type define >
#endif
/*--------------------- Type define -----------------------------*/
struct LogAllCfg;

#if 0
#pragma mark -
#pragma mark < Global functions declaration >
#endif
/*--------------------- Global function declaration -------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: logCfgInit
 *
 * @desc: Init log config
 *
 * @param psFile:log config File Name,
 * @param psName:log section Name
 *
 * @return structLogAllCfg*: LogAllCfg Info
 *
 *
 */
    struct LogAllCfg * logCfgInit(char *psFile, char *psName);

/**
 * @function: logInit
 *
 * @desc: log cfg init, save cfg into global
 *
 * @param psFile:Cfg File Name,
 * @param psName:Cfg section Name
 *
 * @return int: 0 - ok ; <0 - err
 *
 *
 */
    int logInit(char *psFile, char *psName);

/**
 * @function: logEnd
 *
 * @desc: clean all log used memory
 *
 * @return int: 0 - ok ; <0 - err
 *
 *
 */
    int logEnd(void);

/**
 * @function: logCfgEnd
 *
 * @desc: Free logAll Cfg
 *
 * @param ptLogAllCfg:Log All Cfg Info
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
    int logCfgEnd(struct LogAllCfg *ptLogAllCfg);
    
    /* LogInfo */

/**
 * @function: logInfo
 *
 * @desc: printf log info,use global log info
 *
 * @param psLogFile:If Not Null, Printf to File,
 * @param nLevel:Log Level,
 * @param psLevelStr:NOT USE,
 * @param nLine:source File Line,
 * @param psLogStr:Log Message,
 * @param ...:Log Message Args,
 * @param psFunction:source Function Name,
 * @param psFile:source File Name
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logInfo(char *psLogFile, int nLevel, char *psLevelStr, char *psFile, int nLine, const char *psFunction, char *psLogStr, ...) __LOGINFO_GNUC_EXT__;

/**
 * @function: logInfoNoFront
 *
 * @desc: printf log info,use global log info, but has not front inform
 *
 * @param psLogFile:If Not Null, Printf to File,
 * @param nLevel:Log Level,
 * @param psLevelStr:NOT USE,
 * @param nLine:source File Line,
 * @param psLogStr:Log Message,
 * @param ...:Log Message Args,
 * @param psFunction:source Function Name,
 * @param psFile:source File Name
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logInfoNoFront(char *psLogFile, int nLevel, char *psLevelStr, char *psFile, int nLine, const char *psFunction, char *psLogStr, ...)__LOGINFO_GNUC_EXT__;

/**
 * @function: logVInfo
 *
 * @desc: log printf Message
 *
 * @param psLogFile:If Not Null, printf Message To Log File,
 * @param psLogStr:Log Message,
 * @param ap:Log Message Args,
 * @param psFunction:source Function Name,
 * @param nLevel:Log Level,
 * @param psLevelStr:NOT USE,
 * @param psFile:source File Name,
 * @param nLine:source File Line
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logVInfo(char *psLogFile, int nLevel, char *psLevelStr, char *psFile, int nLine, const char *psFunction, char *psLogStr, va_list ap);

/**
 * @function: logVInfoNoFront
 *
 * @desc: log printf Message, but no front inform
 *
 * @param psLogFile:If Not Null, printf Message To Log File,
 * @param psLogStr:Log Message,
 * @param ap:Log Message Args,
 * @param psFunction:source Function Name,
 * @param nLevel:Log Level,
 * @param psLevelStr:NOT USE,
 * @param psFile:source File Name,
 * @param nLine:source File Line
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logVInfoNoFront(char *psLogFile, int nLevel, char *psLevelStr, char *psFile, int nLine, const char *psFunction, char *psLogStr, va_list ap);

/**
 * @function: logCfgInfo
 *
 * @desc: Printf Log Message In Format By LogAllCfg
 *
 * @param ptLogAllCfg:Log Config Info,
 * @param psFunction:source Function Name,
 * @param ...:Log Message Args,
 * @param psLogStr:Log Message,
 * @param nLine:source File Line,
 * @param nLevel:Log Level,
 * @param psFile:source File Name
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logCfgInfo(struct LogAllCfg *ptLogAllCfg, int nLevel, char *psFile, int nLine, const char *psFunction, char *psLogStr, ...) __LOGINFO_GNUC_EXT2__;
    
/**
 * @function: logSession
 *
 * @desc: log Sesion Function by Default
 *
 * @param psKey:Session Key
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logSession(char *psKey);

/**
 * @function: logCfgSession
 *
 * @desc: log Sesion Function by LogAllCfg
 *
 * @param ptLogAllCfg:Log All Config Info,
 * @param psKey:Session Key
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logCfgSession(struct LogAllCfg *ptLogAllCfg, char *psKey);

/**
 * @function: logSessionStart
 *
 * @desc: Do session Start by Default
 *
 * @param psKey:Session Key
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logSessionStart(char *psKey);

/**
 * @function: logCfgSessionStart
 *
 * @desc: Do session Start by LogAllCfg
 *
 * @param ptLogAllCfg:All Session Config,
 * @param psKey:Session Key
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logCfgSessionStart(struct LogAllCfg *ptLogAllCfg, char *psKey);

/**
 * @function: logSessionEnd
 *
 * @desc: Session End Funciton by Default Way
 *
 * @param psKey:Session Key
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logSessionEnd(char *psKey);

/**
 * @function: logCfgSessionEnd
 *
 * @desc: Session End Funciton by LogAllCfg
 *
 * @param ptLogAllCfg:All Log Cnfig Info,
 * @param psKey:Session Key
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logCfgSessionEnd(struct LogAllCfg *ptLogAllCfg, char *psKey);
    
/**
 * @function: logDebugString
 *
 * @desc: Printf String Buffer In Hex Mode by default way
 *
 * @param psLogFile:Is Not Null, printf log to Log File,
 * @param psStrName:Source String Name,
 * @param nStrLen:Source String Line,
 * @param psStr:Source String,
 * @param nFormat:NOT USE Just For Old interface,
 * @param psFile:Source File name,
 * @param psFunction:Source File Function,
 * @param nLine:Source File Num
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logDebugString(char *psLogFile, int nFormat, char *psFile, int nLine, const char *psFunction, char *psStr, char *psStrName, int nStrLen);

/**
 * @function: logCfgDebugString
 *
 * @desc: Printf String Buffer In Hex Mode by LogAllCfg
 *
 * @param ptLogAllCfg:All Log Configer Info,
 * @param psFile:Source File name,
 * @param psStr:Debug String Buf,
 * @param psFunction:Source File Fucntion Name,
 * @param nLine:Source File Line,
 * @param psStrName:Debug String Name,
 * @param nStrLen:Debug String Buf Line
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logCfgDebugString(struct LogAllCfg *ptLogAllCfg, char *psFile, int nLine, const char *psFunction, char *psStr, char *psStrName, int nStrLen);

/**
 * @function: logSetFile
 *
 * @desc: Set Log File Name,Old InterFace, Not Use Again
 *
 * @param psLogFileName:Set Log File Name
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logSetFile(char *psLogFileName);

/**
 * @function: logSetMode
 *
 * @desc: Set Log Show Level,Old Inter Face, Not Use Again
 *
 * @param pnMode:Set Log Show Level
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logSetMode(int *pnMode);

/**
 * @function: SetMaxFileSize
 *
 * @desc: Set File max Size,Old INterface, not used again
 *
 * @param nMaxSize:Set File Max Size
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int SetMaxFileSize(int nMaxSize);

/**
 * @function: logGetSeesionId
 *
 * @desc: Get Session Key,Old Interface, not use again
 *
 * @param psVaule:return session id buffer,
 * @param nLen:return buffer max len
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logGetSeesionId(char *psVaule, int nLen);

/**
 * @function: logGetLogFile
 *
 * @desc: Get Log File Name,Old Inter Face, Not Use Again
 *
 * @param psVaule:retrun file name buffer,
 * @param nLen:return buffer max len
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logGetLogFile(char *psVaule, int nLen);

/**
 *  Set app name to log global
 *
 *  @param psName app name
 *
 *  @return 0
 */
int logSetAppName(char *psName);

/**
 *  get app name from log global
 *
 *  @param psName app name
 *
 *  @return 0
 */
char * logGetAppName(void);

/**
 * @function: logSetConfig
 *
 * @desc: Set Log File Name/max file len/show level,Old Interface Not Use
 *
 * @param psLogFileName:Set File Name,
 * @param nLogMode:Set Log dispaly level,
 * @param nLogFileSize:Set File Max Size
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int logSetConfig(char *psLogFileName, long nLogFileSize, int nLogMode);
    
    /**
     * @function: logCfgCtl
     *
     * @desc: set log all info
     *
     * @param ptLogAllCfg:config handle,
     * @param nLevel:Setting level,
     * @param psName:Setting name
     * @param psValue:setting value
     *
     * @return int: 0 - ok ; <0 - err
     *
     */
    int logCfgCtl(struct LogAllCfg *ptLogAllCfg, int nLevel, char *psName, char *psValue);
    
    /**
     * @function: logCtl
     *
     * @desc: set log all info
     *
     * @param nLevel:Setting level,
     * @param psName:Setting name
     * @param psValue:setting value
     *
     * @return int: 0 - ok ; <0 - err
     *
     */
    int logCtl(int nLevel, char *psName, char *psValue);
    
    /**
     * @function: logUserLogEnd
     *
     * @desc: esay way to use User Log, it just call UserLog Out Put in the end
     * 
     * @param psLogFile:If Not Null, Printf to File,
     * @param nLevel:Log Level,
     * @param psLevelStr:NOT USE,
     * @param nLine:source File Line,
     * @param psLogStr:Log Message,
     * @param ...:Log Message Args,
     * @param psFunction:source Function Name,
     * @param psFile:source File Name
     *
     * @return int: 0 - ok ; <0 - err
     */
    int logUserLogEnd(char *psLogFile, int nLevel, char *psLevelStr, char *psFile, int nLine, const char *psFunction, char *psLogStr, ...)__LOGINFO_GNUC_EXT__;
    
#ifdef __cplusplus
}
#endif
/*--------------------- Global variable -------------------------*/

#endif /* _LOG_INFO_H_20101020180818_ */
/*--------------------- End -------------------------------------*/
