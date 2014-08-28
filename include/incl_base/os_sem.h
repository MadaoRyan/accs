/*
 *
 * 
 * Semaphore functions.
 * 
 * 
 * FileName: os_sem.h
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_SEM_H_20100118172327_
#define _OS_SEM_H_20100118172327_
/*--------------------------- Include files -----------------------------*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/sem.h>

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: semCreate
 *
 * @desc: 创建信号灯
 *
 * @param iKey:,
 * @param iOption:
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int semCreate(int iKey, int iOption);

/**
 * @function: semP
 *
 * @desc: 执行P操作
 *
 * @param iSemId:
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int semP(int iSemId);

/**
 * @function: semV
 *
 * @desc: 执行V操作
 *
 * @param iSemId:
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int semV(int iSemId);

/**
 * @function: semDelete
 *
 * @desc: 删除信号量
 *
 * @param iSemId:
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int semDelete(int iSemId);

#ifdef __cplusplus
}
#endif

#endif /*_OS_SEM_H_20100118172327_*/
/*-----------------------------  End ------------------------------------*/
