/*
 *
 *
 *  Get Os Memory info.
 *
 *
 * FileName: os_memory.c 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

/*------------------------ Include files ------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "os_memory.h"

#if defined (_AIX50)
#include <libperfstat.h>
#elif defined (__linux)
#include <fcntl.h>
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
#if defined (__linux)
static unsigned long GetMemInfo(char *psBuffer, char *psName);
#endif

/*--------------------- Global variable -------------------------*/

#if 0
#pragma mark -
#pragma mark < Global functions >
#endif
/*--------------------- Global functions ------------------------*/
#if defined (_AIX50)

/**
 * memUseRate
 * For Aix 5.0, Get Memory Info
 *
 * @param #: void
 *
 * @return >0  : ok
 *         <0  : err
 */
int memUseRate(void)
{
    int nRet;
    int nUseRete = 0;
    
    perfstat_memory_total_t tMemoryTotal;
    memset(&(tMemoryTotal), '\0', sizeof(perfstat_memory_total_t));
    nRet = perfstat_memory_total(NULL, &(tMemoryTotal), sizeof(perfstat_memory_total_t), 1);
    if ( nRet == -1 ) { 
        return (-1);
    }
    
    nUseRete = 100 - (tMemoryTotal.real_inuse * 100 / tMemoryTotal.real_total);
    
    return ( nUseRete );
}

#elif defined (__linux)

/**
 * memUseRate
 * Get Linux Memory Info from /proc/meminfo
 *
 * @param #: void
 *
 * @return >0  : ok
 *         <0  : err
 */
int memUseRate(void)
{
    int nUseRete = 0;
    
    char sBuffer[4096+1];
    memset(sBuffer, '\0', sizeof(sBuffer));
    int nFd = open("/proc/meminfo", O_RDONLY);
    int nLen = read(nFd, sBuffer, sizeof(sBuffer)-1);
    close(nFd);
    sBuffer[nLen] = '\0';
    
    unsigned long ulTotal = GetMemInfo(sBuffer, "MemTotal");
    unsigned long ulFree = GetMemInfo(sBuffer, "MemFree");
    nUseRete = 100 - ( ulFree * 100 / ulTotal );
    
    return ( nUseRete );
}

#else

/**
 * memUseRate
 * Just For OTher System
 * No return
 *
 * @param #: void
 *
 * @return >0  : ok
 *         <0  : err
 */
int memUseRate(void)
{
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
 * GetMemInfo
 * Get Memory Info by Name
 *
 * @param psBuffer: Buffer
 * @param psName: Name
 *
 * @return >0  : ok
 *         NULL: err
 */
static unsigned long GetMemInfo(char *psBuffer, char *psName)
{
    char *p = strstr(psBuffer, psName);
    if ( NULL == p ) {
        return (0);
    }
    
    p += strlen(psName) + 1;
    
    return ( strtoul(p, NULL, 10) );
}

#endif

/*--------------------- End -------------------------------------*/
