/*
 *
 *
 *  Get Disk Info.
 *
 *
 * FileName: os_disk.c 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

/*------------------------ Include files ------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include "os_disk.h"

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
/**
 * diskUseRate
 * get disk Use Rate
 * statvfs for base all system
 *
 * @param psFilePath: File Path
 *
 * @return >0  : ok
 *         <0  : err
 */
int diskUseRate(char *psFilePath)
{
    int nRet;
    int nUseRate = 0;
    
    struct statvfs tFsStat;
    memset(&(tFsStat), '\0', sizeof(struct statvfs));
    nRet = statvfs(psFilePath, &(tFsStat));
    if ( nRet != 0 ){
        return (-1);
    }
    
    nUseRate = 100 - (tFsStat.f_bfree*100 / tFsStat.f_blocks);
    
    return (nUseRate);
}

#if 0
#pragma mark -
#pragma mark < Global functions >
#endif
/*--------------------- Global functions ------------------------*/

#if 0
#pragma mark -
#pragma mark < Local functions >
#endif
/*--------------------- Local functions -------------------------*/

/*--------------------- End -------------------------------------*/
