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
 * @param fd: 文件描述符
 * @param iCmd: 命令 F_SETLK（不等待）, F_SETLKW（等待）
 * @param iType: 锁类型 F_RDLCK, F_WRLCK, F_UNLCK
 * @param iOffset: 偏移量
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: 长度
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int	lockFileReg(int fd, int iCmd, int iType,
				off_t iOffset, int iWhence, off_t iLen);

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
/**
 * lockFileReg
 *
 * @param fd: 文件描述符
 * @param iCmd: 命令 F_SETLK（不等待）, F_SETLKW（等待）
 * @param iType: 锁类型 F_RDLCK, F_WRLCK, F_UNLCK
 * @param iOffset: 偏移量
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: 长度
 *
 * @return 0:成功
 *        <0:失败
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
 * 对文件加读锁（不等待，直接返回）
 *
 * @param fd: 文件描述符
 * @param iOffset: 偏移量
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: 长度
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
lockFileRead(int fd, off_t iOffset, int iWhence, off_t iLen)
{
	return (lockFileReg(fd, F_SETLK, F_RDLCK, iOffset, iWhence, iLen));
}

/**
 * lockFileReadW
 * 对文件加读锁（等待直至加锁成功）
 *
 * @param fd: 文件描述符
 * @param iOffset: 偏移量
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: 长度
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
lockFileReadW(int fd, off_t iOffset, int iWhence, off_t iLen)
{
	return (lockFileReg(fd, F_SETLKW, F_RDLCK, iOffset, iWhence, iLen));
}

/**
 * lockFileWrite
 * 对文件加写锁（不等待，直接返回）
 *
 * @param fd: 文件描述符
 * @param iOffset: 偏移量
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: 长度
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
lockFileWrite(int fd, off_t iOffset, int iWhence, off_t iLen)
{
	return (lockFileReg(fd, F_SETLK, F_WRLCK, iOffset, iWhence, iLen));
}

/**
 * lockFileWriteW
 * 对文件加写锁（等待直至加锁成功）
 *
 * @param fd: 文件描述符
 * @param iOffset: 偏移量
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: 长度
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
lockFileWriteW(int fd, off_t iOffset, int iWhence, off_t iLen)
{
	return (lockFileReg(fd, F_SETLKW, F_WRLCK, iOffset, iWhence, iLen));
}

/**
 * lockFileUnLock
 * 对文件解除锁定
 *
 * @param fd: 文件描述符
 * @param iOffset: 偏移量
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: 长度
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
lockFileUnLock(int fd, off_t iOffset, int iWhence, off_t iLen)
{
	return (lockFileReg(fd, F_SETLK, F_UNLCK, iOffset, iWhence, iLen));
}

/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

