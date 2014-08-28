/*
 *
 * 
 * share memory functions.
 * 
 * 
 * FileName: os_shm.h
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_SHM_H_20100118172109_
#define _OS_SHM_H_20100118172109_
/*--------------------------- Include files -----------------------------*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/shm.h>
/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/
typedef struct {
#ifdef _WIN32
    HANDLE hShareMem;
#else
    int iShareMem;
#endif
} T_SHM_HANDLE;

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: shmOpen
 *
 * @desc: 创建共享内存
 *
 * @param iKey: 键
 * @param iSize: 创建大小
 * @param iFlg: 标志项
 * @param ptShmHdl: 共享内存句柄
 *
 * @return int: 0:成功, <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int shmOpen(int iKey, int iSize, int iFlg, T_SHM_HANDLE *ptShmHdl);

/**
 * @function: shmMap
 *
 * @desc: 映射共享内存
 *
 * @param ptShmHdl:共享内存句柄
 *
 * @return void*: NULL:映射失败,>0:内存首地址
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
void * shmMap(T_SHM_HANDLE *ptShmHdl);

/**
 * @function: shmUnMap
 *
 * @desc: 取消映射的共享内存
 *
 * @param pAddr: 共享内存首地址
 *
 * @return int : 0:成功, <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int shmUnMap(void *pAddr);

/**
 * @function: shmRemove
 *
 * @desc: 删除创建的共享内存
 *
 * @param ptShmHdl:共享内存句柄
 *
 * @return int: 0:成功, <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int shmRemove(T_SHM_HANDLE *ptShmHdl);

#ifdef __cplusplus
}
#endif

#endif /*_OS_SHM_H_20100118172109_*/
/*-----------------------------  End ------------------------------------*/
