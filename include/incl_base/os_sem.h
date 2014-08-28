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
 * @desc: �����źŵ�
 *
 * @param iKey:,
 * @param iOption:
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ִ��P����
 *
 * @param iSemId:
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ִ��V����
 *
 * @param iSemId:
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ɾ���ź���
 *
 * @param iSemId:
 *
 * @return int: 0:�ɹ� <0:����
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
