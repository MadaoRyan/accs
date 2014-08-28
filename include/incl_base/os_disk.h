/*
 *
 *
 *  for os disk.
 *
 *
 * FileName: os_disk.h 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

#ifndef _OS_DISK_H_20110426162134_
#define _OS_DISK_H_20110426162134_

/*------------------------ Include files ------------------------*/

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

#if 0
#pragma mark -
#pragma mark < Global functions declaration >
#endif
/*--------------------- Global function declaration -------------*/
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @function: diskUseRate
 *
 * @desc: get disk Use Rate
 *
 * @param psFilePath:File Path For Disk mount
 *
 * @return int: disk use rate
 *
 */
int diskUseRate(char *psFilePath);
    
#ifdef __cplusplus
}
#endif
/*--------------------- Global variable -------------------------*/

#endif /* _OS_DISK_H_20110426162134_ */
/*--------------------- End -------------------------------------*/
