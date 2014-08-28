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
 * @desc: ���������ڴ�
 *
 * @param iKey: ��
 * @param iSize: ������С
 * @param iFlg: ��־��
 * @param ptShmHdl: �����ڴ���
 *
 * @return int: 0:�ɹ�, <0:����
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
 * @desc: ӳ�乲���ڴ�
 *
 * @param ptShmHdl:�����ڴ���
 *
 * @return void*: NULL:ӳ��ʧ��,>0:�ڴ��׵�ַ
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
 * @desc: ȡ��ӳ��Ĺ����ڴ�
 *
 * @param pAddr: �����ڴ��׵�ַ
 *
 * @return int : 0:�ɹ�, <0:����
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
 * @desc: ɾ�������Ĺ����ڴ�
 *
 * @param ptShmHdl:�����ڴ���
 *
 * @return int: 0:�ɹ�, <0:����
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
