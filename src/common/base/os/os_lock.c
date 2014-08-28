/*
 *
 *
 * lock functions.
 *
 *
 * FileName: os_lock.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */

/*--------------------------- Include files -----------------------------*/
#include "os_lock.h"

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/
/**
 * lockFileReg
 *
 * @param fd: �ļ�������
 * @param iCmd: ���� F_SETLK�����ȴ���, F_SETLKW���ȴ���
 * @param iType: ������ F_RDLCK, F_WRLCK, F_UNLCK
 * @param iOffset: ƫ����
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: ����
 *
 * @return 0:�ɹ�
 *        <0:ʧ��
 *
 */
int	lockFileReg(int fd, int iCmd, int iType,
				off_t iOffset, int iWhence, off_t iLen);

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
/**
 * lockFileReg
 *
 * @param fd: �ļ�������
 * @param iCmd: ���� F_SETLK�����ȴ���, F_SETLKW���ȴ���
 * @param iType: ������ F_RDLCK, F_WRLCK, F_UNLCK
 * @param iOffset: ƫ����
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: ����
 *
 * @return 0:�ɹ�
 *        <0:ʧ��
 *
 */
int
lockFileReg(int fd, int iCmd, int iType, off_t iOffset, int iWhence, off_t iLen)
{
	struct flock	tLock;

	tLock.l_type = iType;		/* F_RDLCK, F_WRLCK, F_UNLCK */
	tLock.l_start = iOffset;	/* byte offset, relative to l_whence */
	tLock.l_whence = iWhence;	/* SEEK_SET, SEEK_CUR, SEEK_END */
	tLock.l_len = iLen;			/* #bytes (0 means to EOF) */

	return (fcntl(fd, iCmd, &tLock));
}

/**
 * lockFileRead
 * ���ļ��Ӷ��������ȴ���ֱ�ӷ��أ�
 *
 * @param fd: �ļ�������
 * @param iOffset: ƫ����
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: ����
 *
 * @return 0:�ɹ�
 *        <0:ʧ��
 *
 */
int
lockFileRead(int fd, off_t iOffset, int iWhence, off_t iLen)
{
	return (lockFileReg(fd, F_SETLK, F_RDLCK, iOffset, iWhence, iLen));
}

/**
 * lockFileReadW
 * ���ļ��Ӷ������ȴ�ֱ�������ɹ���
 *
 * @param fd: �ļ�������
 * @param iOffset: ƫ����
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: ����
 *
 * @return 0:�ɹ�
 *        <0:ʧ��
 *
 */
int
lockFileReadW(int fd, off_t iOffset, int iWhence, off_t iLen)
{
	return (lockFileReg(fd, F_SETLKW, F_RDLCK, iOffset, iWhence, iLen));
}

/**
 * lockFileWrite
 * ���ļ���д�������ȴ���ֱ�ӷ��أ�
 *
 * @param fd: �ļ�������
 * @param iOffset: ƫ����
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: ����
 *
 * @return 0:�ɹ�
 *        <0:ʧ��
 *
 */
int
lockFileWrite(int fd, off_t iOffset, int iWhence, off_t iLen)
{
	return (lockFileReg(fd, F_SETLK, F_WRLCK, iOffset, iWhence, iLen));
}

/**
 * lockFileWriteW
 * ���ļ���д�����ȴ�ֱ�������ɹ���
 *
 * @param fd: �ļ�������
 * @param iOffset: ƫ����
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: ����
 *
 * @return 0:�ɹ�
 *        <0:ʧ��
 *
 */
int
lockFileWriteW(int fd, off_t iOffset, int iWhence, off_t iLen)
{
	return (lockFileReg(fd, F_SETLKW, F_WRLCK, iOffset, iWhence, iLen));
}

/**
 * lockFileUnLock
 * ���ļ��������
 *
 * @param fd: �ļ�������
 * @param iOffset: ƫ����
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: ����
 *
 * @return 0:�ɹ�
 *        <0:ʧ��
 *
 */
int
lockFileUnLock(int fd, off_t iOffset, int iWhence, off_t iLen)
{
	return (lockFileReg(fd, F_SETLK, F_UNLCK, iOffset, iWhence, iLen));
}

/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

