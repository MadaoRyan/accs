/*
 *
 *
 * rtcore context.
 *
 *
 * FileName: core_ctx.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _CORE_CTX_H_20100523142205_
#define _CORE_CTX_H_20100523142205_
/*--------------------------- Include files -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "log_info.h"
/*--------------------------- Macro define ------------------------------*/
#ifdef  __GNUC__
#define __LOGINFO_GNUC_EXT__ __attribute__((format (printf, 7, 8)))
#else
#define __LOGINFO_GNUC_EXT__
#endif

/*长度定义*/
#define DLEN_RTCORE_CTX_DIR_MAX 1024
#define DLEN_RTCORE_CTX_ARGC_MAX 1024
#define DLEN_RTCORE_CTX_PROCNAME_MAX 256
#define DLEN_RTCORE_CTX_CFGOBJ_MAX 256

/*进程选项*/
#define OPTION_RTCORE_CTX_DAEMON_MASK 0x00000001
#define OPTION_RTCORE_CTX_UMASK_MASK 0x00000002

#define OPTION_RTCORE_CTX_RSTDOUT_MASK 0x00000001
#define OPTION_RTCORE_CTX_RSTDERR_MASK 0x00000002

/*---------------------------- Type define ------------------------------*/
/*日志函数*/
typedef int (*PFN_RTCORE_LOGPRINT)(char *, int , char *, char *, int , const char *, char *, ...) __LOGINFO_GNUC_EXT__;

typedef struct {
    int iArgc;                                      /*参数个数*/
    int iOptc;                                      /*选项参数个数*/
    struct {
        char chKey;                                 /*选项，如果为0或空格表示该参数是普通参数*/
        char *psValue;                              /*参数值*/
    } tArg[DLEN_RTCORE_CTX_ARGC_MAX];               /*经过分析后的参数*/
} T_RTCORE_ARGV;

typedef struct {

    /*----------------------------------*/
    /*进程信息*/
    /*----------------------------------*/
    struct {                                        /*原始启动参数*/
        int argc;                                   /*参数个数*/
        char **argv;                                /*参数*/
    } tCmdArgs;

    T_RTCORE_ARGV tRTCoreArgv;                      /*经过分析后的参数*/

    unsigned int iProcOpt;                          /*进程选项*/

    /*重定向*/
    unsigned int iStdRediretOpt;                    /*重定向选项*/
    char sStdOut[DLEN_RTCORE_CTX_DIR_MAX];          /*标准输出重定向*/
    char sStdErr[DLEN_RTCORE_CTX_DIR_MAX];          /*标准出错重定向*/

    /*进程信息*/
    char sProcName[DLEN_RTCORE_CTX_PROCNAME_MAX];   /*进程名称*/
    char sExe[DLEN_RTCORE_CTX_DIR_MAX];             /*可执行程序*/

    /*以Deamon方式运行时有效*/
    int iMask;                                      /*掩码*/
    char sCwd[DLEN_RTCORE_CTX_DIR_MAX];             /*当前工作路径*/
    char sRootDir[DLEN_RTCORE_CTX_DIR_MAX];         /*需要chroot时设置此目录*/

    PFN_RTCORE_LOGPRINT pfnLogPrint;                /*默认日志打印函数*/

    /*----------------------------------*/
    /*框架辅助参数,参数用途自行定义*/
    /*----------------------------------*/
    int  iSvrid;                                    /*进程编号*/
    int  iRunLevel;                                 /*运行级别*/
    char sLogFile[DLEN_RTCORE_CTX_DIR_MAX];         /*日志文件*/

    /*配置默认参数*/
    int  iCfgMode;                                  /*配置模式*/
    char sCfgSection[DLEN_RTCORE_CTX_CFGOBJ_MAX];   /*配置区*/
    char sCfgObject[DLEN_RTCORE_CTX_CFGOBJ_MAX];    /*配置对象*/
    char sCfgFile[DLEN_RTCORE_CTX_DIR_MAX];         /*配置文件名称*/
    void *pCfgData;                                 /*配置数据在内存中时使用*/
    char sAppHome[DLEN_RTCORE_CTX_DIR_MAX];         /*应用主目录*/

    /*----------------------------------*/
    /*扩展*/
    /*----------------------------------*/
    void *pUserData;                                /*用户数据,供扩展*/

} T_RTCORE_CONTEXT;
/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: RTCoreGetCtx
 *
 * @desc: 获得当前进程的核心数据区
 *
 * @return T_RTCORE_CONTEXT *: ret!=NULL 核心数据区; ret=NULL 失败
 *
 */
T_RTCORE_CONTEXT * RTCoreGetCtx(void);

#ifdef __cplusplus
}
#endif

#endif /*_CORE_CTX_H_20100523142205_*/
/*-----------------------------  End ------------------------------------*/
