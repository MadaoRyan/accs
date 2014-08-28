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

/*���ȶ���*/
#define DLEN_RTCORE_CTX_DIR_MAX 1024
#define DLEN_RTCORE_CTX_ARGC_MAX 1024
#define DLEN_RTCORE_CTX_PROCNAME_MAX 256
#define DLEN_RTCORE_CTX_CFGOBJ_MAX 256

/*����ѡ��*/
#define OPTION_RTCORE_CTX_DAEMON_MASK 0x00000001
#define OPTION_RTCORE_CTX_UMASK_MASK 0x00000002

#define OPTION_RTCORE_CTX_RSTDOUT_MASK 0x00000001
#define OPTION_RTCORE_CTX_RSTDERR_MASK 0x00000002

/*---------------------------- Type define ------------------------------*/
/*��־����*/
typedef int (*PFN_RTCORE_LOGPRINT)(char *, int , char *, char *, int , const char *, char *, ...) __LOGINFO_GNUC_EXT__;

typedef struct {
    int iArgc;                                      /*��������*/
    int iOptc;                                      /*ѡ���������*/
    struct {
        char chKey;                                 /*ѡ����Ϊ0��ո��ʾ�ò�������ͨ����*/
        char *psValue;                              /*����ֵ*/
    } tArg[DLEN_RTCORE_CTX_ARGC_MAX];               /*����������Ĳ���*/
} T_RTCORE_ARGV;

typedef struct {

    /*----------------------------------*/
    /*������Ϣ*/
    /*----------------------------------*/
    struct {                                        /*ԭʼ��������*/
        int argc;                                   /*��������*/
        char **argv;                                /*����*/
    } tCmdArgs;

    T_RTCORE_ARGV tRTCoreArgv;                      /*����������Ĳ���*/

    unsigned int iProcOpt;                          /*����ѡ��*/

    /*�ض���*/
    unsigned int iStdRediretOpt;                    /*�ض���ѡ��*/
    char sStdOut[DLEN_RTCORE_CTX_DIR_MAX];          /*��׼����ض���*/
    char sStdErr[DLEN_RTCORE_CTX_DIR_MAX];          /*��׼�����ض���*/

    /*������Ϣ*/
    char sProcName[DLEN_RTCORE_CTX_PROCNAME_MAX];   /*��������*/
    char sExe[DLEN_RTCORE_CTX_DIR_MAX];             /*��ִ�г���*/

    /*��Deamon��ʽ����ʱ��Ч*/
    int iMask;                                      /*����*/
    char sCwd[DLEN_RTCORE_CTX_DIR_MAX];             /*��ǰ����·��*/
    char sRootDir[DLEN_RTCORE_CTX_DIR_MAX];         /*��Ҫchrootʱ���ô�Ŀ¼*/

    PFN_RTCORE_LOGPRINT pfnLogPrint;                /*Ĭ����־��ӡ����*/

    /*----------------------------------*/
    /*��ܸ�������,������;���ж���*/
    /*----------------------------------*/
    int  iSvrid;                                    /*���̱��*/
    int  iRunLevel;                                 /*���м���*/
    char sLogFile[DLEN_RTCORE_CTX_DIR_MAX];         /*��־�ļ�*/

    /*����Ĭ�ϲ���*/
    int  iCfgMode;                                  /*����ģʽ*/
    char sCfgSection[DLEN_RTCORE_CTX_CFGOBJ_MAX];   /*������*/
    char sCfgObject[DLEN_RTCORE_CTX_CFGOBJ_MAX];    /*���ö���*/
    char sCfgFile[DLEN_RTCORE_CTX_DIR_MAX];         /*�����ļ�����*/
    void *pCfgData;                                 /*�����������ڴ���ʱʹ��*/
    char sAppHome[DLEN_RTCORE_CTX_DIR_MAX];         /*Ӧ����Ŀ¼*/

    /*----------------------------------*/
    /*��չ*/
    /*----------------------------------*/
    void *pUserData;                                /*�û�����,����չ*/

} T_RTCORE_CONTEXT;
/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: RTCoreGetCtx
 *
 * @desc: ��õ�ǰ���̵ĺ���������
 *
 * @return T_RTCORE_CONTEXT *: ret!=NULL ����������; ret=NULL ʧ��
 *
 */
T_RTCORE_CONTEXT * RTCoreGetCtx(void);

#ifdef __cplusplus
}
#endif

#endif /*_CORE_CTX_H_20100523142205_*/
/*-----------------------------  End ------------------------------------*/
