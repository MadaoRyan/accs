/*
 *
 * 
 * command line interface.
 * 
 * 
 * FileName: os_cli.c
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "glb_os.h"
#include "os_cli.h"

/*--------------------------- Macro define ------------------------------*/
#define _DLEN_TINY_BUF 64
#define _DLEN_MINI_BUF 256
#define _DLEN_LARGE_BUF 1024
#define _DLEN_HUGE_BUF 10240
/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
int cliExecCmd(char *psCmd, char *psOutPut, int *piOutLen, int iOption)
{
    int iRet = 0;
    int f_des[2];
    char sTmp[_DLEN_MINI_BUF];
    int iPid;
    int iLen = 0;
    
    memset(sTmp, 0, sizeof(sTmp));
    
    if((NULL == psOutPut) && (*piOutLen > 0)) {
        return ERR_GLB_OS_PARAM;
    }
    
    if(pipe(f_des)==-1) {
        return ERR_GLB_OS_OTHER;
    }
    
    iPid = fork();
    if(iPid == -1) {
        return ERR_GLB_OS_OTHER;
    }
    
    if(iPid > 0) {
        close(f_des[1]);
        
        while(1) {
            iRet = read(f_des[0], psOutPut+iLen, *piOutLen - iLen);
            if(iRet <= 0) {
                break;
            }
            
            iLen += iRet;
            if(iLen == *piOutLen) {
                while(read(f_des[0], sTmp, sizeof(sTmp)) > 0) {};/*¶ªÆúÊ£Óà²¿·Ö*/
            }
            
        }
        
        *piOutLen = iLen;
        close(f_des[0]);
        
        if (waitpid(iPid, NULL, 0) != iPid) {
            fprintf(stderr, "Waitpid error[%d:%s]", errno, strerror(errno));
        }
        
        return 0;
    }
    
    /*child process*/
    dup2(f_des[1],STDOUT_FILENO);
    close(f_des[0]);
    close(f_des[1]);
    iRet = system(psCmd);
    _exit(iRet);
    
    return 0;
}
/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

