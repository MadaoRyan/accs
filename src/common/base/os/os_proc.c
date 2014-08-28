/*
 *
 *
 *  For App Process.
 *
 *
 * FileName: os_proc.c 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

/*------------------------ Include files ------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os_proc.h"

#if defined (__linux)

#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>

#elif defined (_AIX50)

#include <dirent.h>
#include <fcntl.h>
#include <sys/procfs.h>

#endif

#if 0
#pragma mark -
#pragma mark < Macro define >
#endif
/*--------------------- Macro define ----------------------------*/

#if 0
#pragma mark -
#pragma mark < Type define >
#endif
/*--------------------- Type define -----------------------------*/

/*--------------------- Local function declaration --------------*/

/*--------------------- Global variable -------------------------*/

#if 0
#pragma mark -
#pragma mark < Global functions >
#endif
/*--------------------- Global functions ------------------------*/
#if defined (_AIX50)

/**
 * prcGetProcNum
 * Get Proc Num of Process Name
 *
 * @param psProcName: Porcess Name
 *
 * @return >0  : ok
 *         <0  : err
 */
int prcGetProcNum(char *psProcName)
{
    /*
    int nProcNum = 0;
    pid_t nPid = 0;
    struct procsinfo tPrcInfo;
    
    while( getprocs(&tPrcInfo, sizeof(struct procsinfo), 0, 0, &nPid, 1) == 1 ) {
        if ( strcmp(psProcName, tPrcInfo.pi_comm) == 0 ) {
            ++nProcNum;
        }
    }
     */
    
    int nProcNum = 0;
    struct dirent *ptEnt = NULL;
    DIR *ptDir = NULL;
    
    if( ( ptDir = opendir("/proc") ) == NULL ) {
        return (-1);
    }
    
    int nNameLen = strlen(psProcName);
    while( ptEnt = readdir(ptDir) ) {
        if ( isdigit(ptEnt->d_name[0]) == 0 ) {
            continue;
        }
        
        char sBuf[32];
        sprintf(sBuf, "/proc/%s/psinfo", ptEnt->d_name);
        int nFd = open(sBuf, O_RDONLY, 0);
        if ( nFd == -1 ) {
            continue;
        }
        
        struct psinfo tPsinfo;
        read(nFd, &(tPsinfo), sizeof(tPsinfo));
        close(nFd);
        
        if ( tPsinfo.pr_psargs[nNameLen] == ' ' &&
             memcmp(psProcName, tPsinfo.pr_psargs, nNameLen) == 0 ) {
            ++nProcNum;
        }
    }
    closedir(ptDir);
    
    return ( nProcNum );
}

/**
 * prcGetProcName
 * Get Process Name By Process Id
 *
 * @param nPid: Porcess Id
 *
 * @return >0  : ok
 *         <0  : err
 */
int prcGetProcName(int nPid, char *psProcName, int nNameLen)
{
    /*
    int nRet;
    
    struct procsinfo tPrcInfo;
    nRet = getprocs(&tPrcInfo, sizeof(struct procsinfo), 0, 0, &nPid, 1);
    if ( nRet <= 0 ) {
        return (-1);
    }
    
    int nMaxLen = ( nNameLen > sizeof(tPrcInfo.pi_comm) ) ? sizeof(tPrcInfo.pi_comm) : nNameLen;
    memcpy(psProcName, tPrcInfo.pi_comm, nMaxLen);
    */
    
    int nRet;
    
    char sBuf[32];
    sprintf(sBuf, "/proc/%d/psinfo", nPid);
    int nFd = open(sBuf, O_RDONLY, 0);
    if ( nFd == -1 ) {
        return (-1);
    }
    
    struct psinfo tPsinfo;
    read(nFd, &(tPsinfo), sizeof(tPsinfo));
    close(nFd);
    
    char *p = strchr(tPsinfo.pr_psargs, ' ');
    int nLen = p - tPsinfo.pr_psargs;
    nLen = ( nLen > nNameLen ) ? nNameLen : nLen;
    memcpy(psProcName, tPsinfo.pr_psargs, nLen);
    
    return (0);
}

#elif defined (__linux)

/**
 * prcGetProcNum
 * Get Proc Num of Process Name
 *
 * @param psProcName: Porcess Name
 *
 * @return >0  : ok
 *         <0  : err
 */
int prcGetProcNum(char *psProcName)
{
    int nProcNum = 0;
    struct dirent *ptEnt = NULL;
    DIR *ptDir = NULL;
    
    if( ( ptDir = opendir("/proc") ) == NULL ) {
        return (-1);
    }
    
    int nNameLen = strlen(psProcName);
    nNameLen = (nNameLen>1024) ? 1024 : nNameLen;
    char sName[1024+1];
    while( ptEnt = readdir(ptDir) ) {
        if ( isdigit(ptEnt->d_name[0]) == 0 ) {
            continue;
        }
        
        char sBuf[32];
        sprintf(sBuf, "/proc/%s/cmdline", ptEnt->d_name);
        int nFd = open(sBuf, O_RDONLY, 0);
        if ( nFd == -1 ) {
            continue;
        }
        
        memset(sName, '\0', nNameLen + 1);
        read(nFd, sName, nNameLen);
        close(nFd);
        
        if ( memcmp(psProcName, sName, nNameLen + 1) == 0 ) {
            ++nProcNum;
        }
    }
    closedir(ptDir);
    
    return (nProcNum);
}

/**
 * prcGetProcName
 * Get Process Name By Process Id
 *
 * @param nPid: Porcess Id
 *
 * @return >0  : ok
 *         <0  : err
 */
int prcGetProcName(int nPid, char *psProcName, int nNameLen)
{
    int nRet;
        
    char sBuf[32];
    sprintf(sBuf, "/proc/%d/cmdline", nPid);
    int nFd = open(sBuf, O_RDONLY, 0);
    if ( nFd == -1 ) {
        return (-1);
    }
    
    read(nFd, psProcName, nNameLen);
    close(nFd);
    
    return ( 0 );
}

#else

/**
 * prcGetProcNum
 * Get Proc Num of Process Name
 *
 * @param psProcName: Porcess Name
 *
 * @return >0  : ok
 *         <0  : err
 */
int prcGetProcNum(char *psProcName)
{
    return (0);
}

/**
 * prcGetProcName
 * Get Process Name By Process Id
 *
 * @param nPid: Porcess Id
 *
 * @return >0  : ok
 *         <0  : err
 */
int prcGetProcName(int nPid, char *psProcName, int nNameLen)
{
    return (0);
}

#endif

#if 0
#pragma mark -
#pragma mark < Local functions >
#endif
/*--------------------- Local functions -------------------------*/

/*--------------------- End -------------------------------------*/
