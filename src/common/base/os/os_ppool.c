/*
 *
 * 
 * process pool
 * 
 * 
 * FileName: os_ppool.c
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#include "os_process.h"
#include "os_ppool.h"

/*--------------------------- Macro define ------------------------------*/
#define _DLEN_TINY_BUF 64
#define _DLEN_MINI_BUF 256
#define _DLEN_LARGE_BUF 1024
#define _DLEN_HUGE_BUF 10240

#define DLEN_OS_PPOOL_MAX 1024
/*---------------------------- Type define ------------------------------*/
typedef struct {
    int iNum;
    int iTail;
    int iSpace;
    
    int aiPidList[DLEN_OS_PPOOL_MAX];
} T_OS_PPOOL;   

/*---------------------- Local function declaration ---------------------*/
static void WaitChild();
static void OnSIGCHLD(int iSignal);
/*-------------------------  Global variable ----------------------------*/
static int f_iInit = 0;
static int f_iIsChild = 0;
static int f_iMaxProc = DLEN_OS_PPOOL_MAX;
static int f_iIndex = 0;

static T_OS_PPOOL f_tPool;
/*-------------------------  Global functions ---------------------------*/
/**
 * @desc: 初始化进程池
 *
 */
int pplInit(int iMax, int iOptions)
{
    if(iMax < 1 || iMax >= DLEN_OS_PPOOL_MAX) {
        return ERR_OS_PPOOL_PARAM;
    }
    
    if(f_iInit) {
        return ERR_OS_PPOOL_INIT;
    }
    
    memset(&f_tPool, 0, sizeof(f_tPool));
    
    f_iMaxProc = iMax;
    if (sigset(SIGCHLD, OnSIGCHLD) == SIG_ERR) {
        return ERR_OS_PPOOL_OTHER;
    }
    
    f_iInit = 1;
    
    return ERR_OS_PPOOL_OK;
}


/**
 * @desc: 申请进程
 *
 */
int pplFork(int iOptions)
{
    int iPid = 0;
    int i = 0;
    
    if(!f_iInit) {
        return ERR_OS_PPOOL_INIT;
    }
    
    while(f_tPool.iNum >= f_iMaxProc) {
        if(!(iOptions & OPT_OS_PPOOL_BLOCK)) { /*非阻塞模式*/
            return ERR_OS_PPOOL_NOSPACE;
        }
        
        /*阻塞模式*/
        WaitChild();
    }

    /* find free index */
    if(f_tPool.iSpace > 0) {
        for(i=0; i<f_tPool.iTail; i++) {
            if(f_tPool.aiPidList[i] == 0) {
                f_iIndex = i;
                f_tPool.iSpace -= 1;
                break;
            }
        }
    } else {
        f_iIndex = f_tPool.iTail;
        f_tPool.iTail += 1;
    }
    
    iPid = fork();
    if(iPid < 0) {
        return ERR_OS_PPOOL_OTHER;
    }
    
    if(iPid == 0) {/*Child*/
        f_iIsChild = 1;
        return iPid;
    }
    
    f_tPool.iNum += 1;
    f_tPool.aiPidList[f_iIndex] = iPid;
    
    return iPid;
}


/**
 * @desc: 向进程发送信号
 *
 */
int pplKill(int iPid, int iSignal)
{
    int iRet = 0;
    int i = 0;
    int iSndCnt = 0;
    
    if(!f_iInit) {
        return ERR_OS_PPOOL_INIT;
    }
    
    if(f_iIsChild) {
        return ERR_OS_PPOOL_PARAM;
    }
    
    if(iPid > 0) {
        iRet = procSndSig2Proc(iPid, iSignal);
        if(iRet < 0) {
            return iRet;
        }
        
        return 1;
    }
    
    for(i=0; i<f_tPool.iTail; i++) {
        if(f_tPool.aiPidList[i] > 0) {
            iRet = procSndSig2Proc(f_tPool.aiPidList[i], iSignal);
            if(iRet < 0) {
                return iRet;
            }
            
            iSndCnt += 1;
        }
    }
        
    return iSndCnt;
}

/**
 * @desc: 计算进程池中活动进程个数
 *
 */
int pplCountAlive(void)
{
    if(!f_iInit) {
        return ERR_OS_PPOOL_INIT;
    }
    
    if(f_iIsChild) {
        return ERR_OS_PPOOL_PARAM;
    }
    
    return f_tPool.iNum;
}

/**
 * @desc: 释放进程池资源
 *
 */
int pplUninit(int iOptions)
{
    if(!f_iInit) {
        return ERR_OS_PPOOL_INIT;
    }
    
    if(f_iIsChild) {
        return ERR_OS_PPOOL_OK;
    }
    
    pplKill(0, SIGKILL);
    
    return ERR_OS_PPOOL_OK;
}

/**
 *  获取当前进程序号
 *
 *  @return 进程序号
 */
int pplGetIndex()
{
    return f_iIndex;
}

/*-------------------------  Local functions ----------------------------*/
void WaitChild()
{
    int iPid = 0;
    int i = 0;
    int iStatus = 0;
    
    if( (iPid = waitpid(-1,&iStatus,0)) > 0) {
        for(i=0; i<f_tPool.iTail; i++) {
            if(iPid == f_tPool.aiPidList[i]) {
                f_tPool.iNum -= 1;
                f_tPool.aiPidList[i] = 0;
                f_tPool.iSpace += 1;
            }
        }
    }
    
    return ;
}

void OnSIGCHLD(int iSignal)
{
    int iPid = 0;
    int i = 0;
    int iStatus = 0;
    
    while( (iPid  = waitpid(-1,&iStatus,WNOHANG)) > 0) {
        for(i=0; i<f_tPool.iTail; i++) {
            if(iPid == f_tPool.aiPidList[i]) {
                f_tPool.iNum -= 1;
                f_tPool.aiPidList[i] = 0;
                f_tPool.iSpace += 1;
            }
        }
    }
    
    return ;
}
/*-----------------------------  End ------------------------------------*/

