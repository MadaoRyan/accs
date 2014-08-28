/*
 *
 *
 * lock functions.
 *
 *
 * FileName: os_lock.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_LOCK_H_20100118172157_
#define _OS_LOCK_H_20100118172157_

/*--------------------------- Include files -----------------------------*/
#include <fcntl.h>

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: lockFileRead
 *
 * @desc: ���ļ��Ӷ��������ȴ���ֱ�ӷ��أ�
 *
 * @param fd: �ļ�������
 * @param iOffset: ƫ����
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: ����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int	lockFileRead(int fd, off_t iOffset, int iWhence, off_t iLen);

/**
 * @function: lockFileReadW
 *
 * @desc: ���ļ��Ӷ������ȴ�ֱ�������ɹ���
 *
 * @param fd: �ļ�������
 * @param iOffset: ƫ����
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: ����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int	lockFileReadW(int fd, off_t iOffset, int iWhence, off_t iLen);

/**
 * @function: lockFileWrite
 *
 * @desc: ���ļ���д�������ȴ���ֱ�ӷ��أ�
 *
 * @param fd: �ļ�������
 * @param iOffset: ƫ����
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: ����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int	lockFileWrite(int fd, off_t iOffset, int iWhence, off_t iLen);

/**
 * @function: lockFileWriteW
 *
 * @desc: ���ļ���д�����ȴ�ֱ�������ɹ���
 *
 * @param fd: �ļ�������
 * @param iOffset: ƫ����
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: ����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int	lockFileWriteW(int fd, off_t iOffset, int iWhence, off_t iLen);

/**
 * @function: lockFileUnLock
 *
 * @desc: ���ļ��������
 *
 * @param fd: �ļ�������
 * @param iOffset: ƫ����
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: ����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int	lockFileUnLock(int fd, off_t iOffset, int iWhence, off_t iLen);

#ifdef __cplusplus
}
#endif

#endif /*_OS_LOCK_H_20100118172157_*/
/*-----------------------------  End ------------------------------------*/
