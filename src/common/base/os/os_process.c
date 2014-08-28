/*
 *
 *
 * os module process functions .
 *
 *
 * FileName: os_process.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include<dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

#include "os_process.h"

/*--------------------------- Macro define ------------------------------*/
#define _DLEN_TINY_BUF 64
#define _DLEN_MINI_BUF 256
#define _DLEN_LARGE_BUF 1024
#define _DLEN_HUGE_BUF 10240
/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
int procParseAgrs(char *psLine, char *psArgv[])
{
    int i = 0;
    int iCnt = 0;
    int iStat = 0;


    for(i=0; psLine[i] != '\0'; i++) {
        switch(iStat) {
        case 0:
            {  
                if(psLine[i] == '\'') {
                    iStat = 2;
                    psLine[i] = '\0';
                    continue;
                } else if(psLine[i] != ' ') {
                    iStat = 1;
                    psArgv[iCnt] = psLine+i;
                    iCnt++;
                    continue;
                } else {
                    psLine[i] = '\0';
                }
            }
            break;
        case 1:
            {
                if(psLine[i] == ' ') {
                    psLine[i] = '\0';
                    iStat = 0;
                }

                continue;
            }
            break;
        case 2:
            {
                iStat = 3;
                psArgv[iCnt] = psLine+i;
                iCnt++;
                if(psLine[i] == '\'') {
                    iStat = 0;
                    psLine[i] = '\0';
                } 
                
                continue;
            }
            break;
        case 3:
            {
                if(psLine[i] == '\'') {
                    iStat = 0;
                    psLine[i] = '\0';
                    continue;
                } 
            }
            break;
        default:
            break;
        }
        
    }

    psArgv[iCnt] = NULL;

    return iCnt;
}

int procStartProcess(char *psProcName, char *psBin, char *psArgs)
{
    return procStartProcessE(psProcName, psBin, psArgs, 0);
}

int procStartProcessE(char *psProcName, char *psBin, char *psArgs, int iOption)
{
    int   iPid = 0;
    char  sTmp[_DLEN_HUGE_BUF];
    char *argv[100];
    int iArgc = 1;
    int iRet = 0;

    iPid = procFork(iOption);
    if(iPid < 0) {
        return iPid;
    }
    
    if(iPid > 0) { /*¸¸½ø³Ì*/
        return iPid;
    }

    strncpy(sTmp, psArgs, sizeof(sTmp)-1);
    argv[0] = psProcName;
    iArgc = procParseAgrs(sTmp, argv+1);
    iArgc += 1;
    argv[iArgc] = NULL;
    iRet = execvp(psBin, argv);
    fprintf(stderr, "execvp error[%d:%s]\n", errno, strerror(errno));

    /*exec fail, child proc exit.*/
    _exit(0);

    return 0;
}

int procFork(int iOption)
{
    int iPid = 0;
    
    iPid = fork();
    if(iPid == -1) {
        fprintf(stderr, "fork error[%d:%s]\n", errno, strerror(errno));
        return iPid;
    }
    
    if(iOption & VAR_OS_PROCESS_OPTION_NODEFUCT) {
        if(iPid > 0) { /*parent process*/
            while(1) {
                if (waitpid(iPid, NULL, 0) != iPid) {
                    if(errno == EINTR) {
                        continue;
                    }
                    
                    fprintf(stderr, "Waitpid error[%d:%s]", errno, strerror(errno));
                }
                
                break;
            }

            return iPid;
        }

        iPid = fork();
        if(iPid == -1) {
            fprintf(stderr, "fork error[%d:%s]\n", errno, strerror(errno));
            _exit(1);
        }

        if(iPid > 0) {
            _exit(0);
        }
    } 
    
    return iPid;
}

int procSndSig2Proc(int iPid, int iSig)
{
    int iRet = 0;
    if(iPid <= 1) {
        return -1;
    }
    
    if(procIsExist(iPid) <= 0) {
        return -1;
    }

    iRet = kill(iPid, iSig);
    if(iRet != 0) {
        fprintf(stderr, "kill[%d][%d] error[%d:%s]\n",
            iPid, iSig, errno, strerror(errno));
        return -1;
    }

    return 0;
}

int procRunAsDaemon(char *psRoot, int iMask, int iOptions)
{
    pid_t pid;
    int   ret;

    if ((pid = fork()) != 0) {
        _exit(0);    /* parent terminates */
    }
    /* 1st child continues */

    setsid();    /* become session leader */

    signal(SIGHUP, SIG_IGN);
    if ((pid = fork()) != 0) {
        _exit(0);    /* 1st child terminates */
    }
    /* 2nd child continues */

    ret = chdir(psRoot);    /* change working directory */

    umask(iMask);      /* clear our file mode creation mask */

    return 0;
}

int procIsExist(int iPid)
{
    if ( 0 != kill(iPid, 0) ){
        return -1;
    }

    return iPid;
}
/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

