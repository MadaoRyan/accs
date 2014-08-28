/*
 *
 *
 *  log plug Header define.
 *
 *
 * FileName: log_plug.h 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

#ifndef _LOG_PLUG_H_20101122134518_
#define _LOG_PLUG_H_20101122134518_

/*------------------------ Include files ------------------------*/

#if 0
#pragma mark -
#pragma mark < Macro define >
#endif
/*--------------------- Macro define ----------------------------*/
#define logLog \
printf("[%s:%d] Log Err:", __FILE__, __LINE__);printf

#if 0
#pragma mark -
#pragma mark < Type define >
#endif
/*--------------------- Type define -----------------------------*/
typedef struct LogArgs {
    char *psGroupName;
    int nLevel;
    char *psLevelStr;
    char *psFile;
    int nLine;
    const char *psFunction;
    char *psLogStr;
} LogArgs;

typedef struct LogAppender *LogAppenderPoint;

/* Init Fucntion */
typedef int (*FncInitUserAppender)(LogAppenderPoint ptLogAppender, char *psFile, char *psPkg, char *psGroup);

/* appender plug */
typedef int (*FncAppenderPrintf)(LogArgs *ptLogArgs, void *ptOtherArgs, char *psBuf, va_list tVaList);
typedef int (*FncAppenderSessionStart)(void *ptOtherArgs);
typedef int (*FncAppenderSessionEnd)(void *ptOtherArgs);
typedef void (*FncAppenderFree)(void *ptOtherArgs);
typedef int (*FncAppenderCtl)(void *ptOtherArgs, char *psName, char *psValue);

#if 0
#pragma mark -
#pragma mark < Global functions declaration >
#endif
/*--------------------- Global function declaration -------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: logInitAppender
 *
 * @desc: 
 *
 * @param ptAppender: 
 * @param fAppenderPrintf: 
 * @param fAppenderSessionStart: 
 * @param fAppenderSessionEnd: 
 * @param fAppenderFree: 
 * @param ptOtherArgs: 
 *
 * @return  int : 
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int logInitAppender(LogAppenderPoint ptAppender, 
                        FncAppenderPrintf fAppenderPrintf,
                        FncAppenderSessionStart fAppenderSessionStart,
                        FncAppenderSessionEnd fAppenderSessionEnd,
                        FncAppenderFree fAppenderFree, 
                        void *ptOtherArgs);

/**
 * @function: logInitAppender
 *
 * @desc: 
 *
 * @param ptAppender: 
 * @param fAppenderCtl: 
 *
 * @return  int : 
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int logInitAppenderAddCtl(LogAppenderPoint ptAppender, FncAppenderCtl fAppenderCtl);

int logInitAppnederSetSession(LogAppenderPoint ptAppender, int iSessionFlag);
    
/**
 * @function: logIniDefRead
 *
 * @desc: 
 *
 * @param psGroup:,
 * @param psFile:,
 * @param psPkg:,
 * @param psDefault:,
 * @param psOutBuf:,
 * @param nOutLen:,
 * @param psKey:
 *
 * @return int: 
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int logIniDefRead(char *psGroup, char *psPkg, char *psKey, char *psDefault, 
                      char *psOutBuf, int nOutLen, char *psFile);

/**
 * @function: logIniDefReadInt
 *
 * @desc: 
 *
 * @param psGroup:,
 * @param psFile:,
 * @param psPkg:,
 * @param psDefault:,
 * @param psOutBuf:,
 * @param nOutLen:,
 * @param psKey:
 *
 * @return int: 
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int logIniDefReadInt(char *psGroup, char *psPkg, char *psKey, int nDefault, char *psFile);
    
/**
 * @function: logCfgGetFileName
 *
 * @desc: 
 *
 * @param psGroup:,
 * @param psPkg:,
 * @param sFileName:,
 * @param nNum:,
 * @param psCfgFile:,
 *
 * @return int: 
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int logCfgGetFileName(char *psGroup, char *psPkg, char *sFileName, int nNum, char *psCfgFile);

    
#ifdef __cplusplus
}
#endif
/*--------------------- Global variable -------------------------*/

#endif /* _LOG_PLUG_H_20101122134518_ */
/*--------------------- End -------------------------------------*/
