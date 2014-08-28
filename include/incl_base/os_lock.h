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
 * @desc: 对文件加读锁（不等待，直接返回）
 *
 * @param fd: 文件描述符
 * @param iOffset: 偏移量
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: 长度
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int	lockFileRead(int fd, off_t iOffset, int iWhence, off_t iLen);

/**
 * @function: lockFileReadW
 *
 * @desc: 对文件加读锁（等待直至加锁成功）
 *
 * @param fd: 文件描述符
 * @param iOffset: 偏移量
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: 长度
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int	lockFileReadW(int fd, off_t iOffset, int iWhence, off_t iLen);

/**
 * @function: lockFileWrite
 *
 * @desc: 对文件加写锁（不等待，直接返回）
 *
 * @param fd: 文件描述符
 * @param iOffset: 偏移量
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: 长度
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int	lockFileWrite(int fd, off_t iOffset, int iWhence, off_t iLen);

/**
 * @function: lockFileWriteW
 *
 * @desc: 对文件加写锁（等待直至加锁成功）
 *
 * @param fd: 文件描述符
 * @param iOffset: 偏移量
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: 长度
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int	lockFileWriteW(int fd, off_t iOffset, int iWhence, off_t iLen);

/**
 * @function: lockFileUnLock
 *
 * @desc: 对文件解除锁定
 *
 * @param fd: 文件描述符
 * @param iOffset: 偏移量
 * @param iWhence: SEEK_SET, SEEK_CUR, SEEK_END
 * @param iLen: 长度
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int	lockFileUnLock(int fd, off_t iOffset, int iWhence, off_t iLen);

#ifdef __cplusplus
}
#endif

#endif /*_OS_LOCK_H_20100118172157_*/
/*-----------------------------  End ------------------------------------*/
