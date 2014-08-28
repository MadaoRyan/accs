/*
 *
 *
 *  Get Cpu Info.
 *
 *
 * FileName: os_cpu.c 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

/*------------------------ Include files ------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "os_cpu.h"

#if defined (_AIX50)
#include <libperfstat.h>
#elif defined (__linux)
#include <fcntl.h>
#define BUFFSIZE (64*1024)
#endif

#if 0
#pragma mark -
#pragma mark < Macro define >
#endif
/*--------------------- Macro define ----------------------------*/
#define CPU_INIT     '1'
#define CPU_NOT_INIT '0'

#if 0
#pragma mark -
#pragma mark < Type define >
#endif
/*--------------------- Type define -----------------------------*/
#if defined (__linux)
enum {
    CPU_USE,
    CPU_NIC,
    CPU_SYS,
    CPU_IDL,
    CPU_IOW,
    CPU_XXX,
    CPU_YYY,
    CPU_ZZZ,
    CPU_NUMBER
};
#endif

/*--------------------- Local function declaration --------------*/
#if defined (__linux)
static int GetCpuInfo(unsigned long long tCpuStatus[CPU_NUMBER]);
#endif

/*--------------------- Global variable -------------------------*/
static char caFlag = CPU_NOT_INIT;

#if defined (_AIX50)
perfstat_cpu_total_t gtOldCpuTotal;
#elif defined (__linux)
static unsigned long long gtOldCpuStates[CPU_NUMBER];
static unsigned long long gtNewCpuStates[CPU_NUMBER];
#endif

#if 0
#pragma mark -
#pragma mark < Global functions >
#endif
/*--------------------- Global functions ------------------------*/
#if defined (_AIX50)
/**
 * cpuUseRate
 * Get Cpu Use Rate For Aix
 * Maybe just for Aix 5, Get Cpu Info By Api
 *
 * @param #: void
 *
 * @return >0  : ok
 *         <0  : err
 */
int cpuUseRate(void)
{
    int nRet;
    int nUseRate = 0;
    
    /* Get the system cpu information, actually is the average information */
    perfstat_cpu_total_t tCpuTotal;
    memset(&(tCpuTotal), '\0', sizeof(perfstat_cpu_total_t));
    nRet = perfstat_cpu_total(NULL, &(tCpuTotal), sizeof(perfstat_cpu_total_t), 1);
    if (nRet == -1) {
        return (-1);
    }
    
    if ( CPU_NOT_INIT == caFlag ) {
        caFlag = CPU_INIT;
        memset(&(gtOldCpuTotal), '\0', sizeof(perfstat_cpu_total_t));
    }
    
    u_longlong_t nTotal = 0;    
    nTotal += tCpuTotal.user - gtOldCpuTotal.user;
    nTotal += tCpuTotal.sys - gtOldCpuTotal.sys;
    nTotal += tCpuTotal.idle - gtOldCpuTotal.idle;
    nTotal += tCpuTotal.wait - gtOldCpuTotal.wait;
    
    if ( 0 == nTotal ) {
        nTotal = 1;
    }
    u_longlong_t nHalfTotal =  nTotal/21;
    
    nUseRate = 100 - (((tCpuTotal.idle - gtOldCpuTotal.idle) * 100 + nHalfTotal)/ nTotal);
    memcpy(&(gtOldCpuTotal), &(tCpuTotal), sizeof(perfstat_cpu_total_t));
    
    return ( nUseRate );
}

#elif defined (__linux)

/**
 * cpuUseRate
 * Get Cpu Use Rate In Linux
 * get Cpu Info Bh File /proc/stat
 *
 * @param #: void
 *
 * @return >0  : ok
 *         <0  : err
 */
int cpuUseRate(void)
{
    static int nDebt = 0;
    
    /* Init */
    if ( CPU_NOT_INIT == caFlag ) {
        caFlag = CPU_INIT;
        memset(&(gtOldCpuStates), '\0', sizeof(gtOldCpuStates));
    }
    
    /* Get Cpu Info */
    if ( GetCpuInfo(gtNewCpuStates) < 0 ) {
        return (-1);
    }
    
    /* Count All Cpu Info */
    int nUseRate = 0;    
    unsigned long long lTotal = 0;
    lTotal += gtNewCpuStates[CPU_USE] - gtOldCpuStates[CPU_USE];
    lTotal += gtNewCpuStates[CPU_NIC] - gtOldCpuStates[CPU_NIC];
    lTotal += gtNewCpuStates[CPU_SYS] - gtOldCpuStates[CPU_SYS];
    lTotal += gtNewCpuStates[CPU_IOW] - gtOldCpuStates[CPU_IOW];
    lTotal += gtNewCpuStates[CPU_XXX] - gtOldCpuStates[CPU_XXX];
    lTotal += gtNewCpuStates[CPU_YYY] - gtOldCpuStates[CPU_YYY];
    lTotal += gtNewCpuStates[CPU_ZZZ] - gtOldCpuStates[CPU_ZZZ];
    
    /* Count Cpu idle Info */
    unsigned long long lIdle = gtNewCpuStates[CPU_IDL] - gtOldCpuStates[CPU_IDL];
    if ( nDebt != 0 ) {
        nDebt = (int)lIdle + nDebt;
        nDebt = 0;
    }
    
    if( (int)lIdle < 0 ){
        nDebt = (int)lIdle;
        lIdle = 0;
    }
    
    /* Count Cpu Rate */
    lTotal += lIdle;
    if ( lTotal < 0 ) {
        lTotal = 1;
    }
    
    unsigned long long nHalfTotal = lTotal/2UL;
    nUseRate = 100 - ((lIdle * 100 + nHalfTotal)/ lTotal);
    if ( nUseRate < 0 ) {
        nUseRate = 0;
    }
    
    memcpy(&(gtOldCpuStates), &(gtNewCpuStates), sizeof(gtOldCpuStates));
    
    return (nUseRate);
}

#else

/**
 * cpuUseRate
 * For Other System, just Return 0
 *
 * @param #: void
 *
 * @return >0  : ok
 *         <0  : err
 */
int cpuUseRate(void)
{
    caFlag = CPU_INIT;
    return (0);
}

#endif

#if 0
#pragma mark -
#pragma mark < Local functions >
#endif
/*--------------------- Local functions -------------------------*/
#if defined (__linux)
/**
 * GetCpuInfo
 * Get Cpu Info
 *
 * @param #: void
 *
 * @return >0  : ok
 *         <0  : err
 */
static int GetCpuInfo(unsigned long long tCpuStatus[CPU_NUMBER])
{
    static int fd = 0;
    
    /* Init All Info */
    memset(tCpuStatus, '\0', sizeof(unsigned long long)*CPU_NUMBER);
    
    /* Get all Info */
    if ( fd ) {
        lseek(fd, 0L, SEEK_SET);
    }else{
        fd = open("/proc/stat", O_RDONLY, 0);
        if ( fd == -1 ) {
            return (-1);
        }
    }
    
    char sBuffer[BUFFSIZE-1];
    memset(sBuffer, '\0', sizeof(sBuffer));
    read(fd, sBuffer, sizeof(sBuffer));
    
    /* Get Cpu Info */
    char  *b = strstr(sBuffer, "cpu ");
    if ( b != NULL ) {
        sscanf(b,  "cpu  %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu", 
               &(tCpuStatus[CPU_USE]), 
               &(tCpuStatus[CPU_NIC]),
               &(tCpuStatus[CPU_SYS]),
               &(tCpuStatus[CPU_IDL]),
               &(tCpuStatus[CPU_IOW]),
               &(tCpuStatus[CPU_XXX]),
               &(tCpuStatus[CPU_YYY]),
               &(tCpuStatus[CPU_ZZZ]));
    }
    
    return (0);
}

#endif

/*--------------------- End -------------------------------------*/
