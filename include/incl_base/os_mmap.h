/*
 *
 *
 * OS Mmap Module
 *
 *
 * FileName: os_mmap.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_MMAP_H_20111130102322_
#define _OS_MMAP_H_20111130102322_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/
#define	OS_MMAP_MAX		8

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: mapRead
 *
 * @desc: 映射文件内容至内存
 *
 * @param sFile: 文件名称
 * @param piLen: 文件长度
 *
 * @return int: ret!=NULL 文件内容指针; ret=NULL 失败
 *
 */
char *mapRead(char *sFile, int *piLen);

/**
 * @function: mapFree
 *
 * @desc: 释放文件内容指针
 *
 * @param pMap: 文件内容指针
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int mapFree(char *pMap);

/**
 * @function: mapFree
 *
 * @desc: 释放所有打开的文件内容指针
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int mapFreeAll(void);

#ifdef __cplusplus
}
#endif

#endif /*_OS_MMAP_H_20111130102322_*/
/*-----------------------------  End ------------------------------------*/
