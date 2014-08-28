/*
 *
 * 
 * stream operate functions.
 * 
 * 
 * FileName: os_stream.h
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_STREAM_H_20100209101555_
#define _OS_STREAM_H_20100209101555_
/*--------------------------- Include files -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/file.h>
/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: streamWriteNByte
 *
 * @desc: 向流中写入若干字节
 *
 * @param pBuf: 输入缓存
 * @param iLen: 写入字节数
 * @param iFileNo: 文件描述符
 *
 * @return int : >:写入字节数,<=0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int streamWriteNByte(void *pBuf, int iLen, int iFileNo);

/**
 * @function: streamReadNByte
 *
 * @desc: 从字节流中读出若干字节
 *
 * @param pBuf: 缓存
 * @param iLen: 缓存大小
 * @param piReadLen: 读出字节数
 * @param iFileNo: 文件描述符
 *
 * @return int : >0:读出字节数, =0:文件结尾, <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int streamReadNByte(void *pBuf, int iLen, int *piReadLen, int iFileNo);


/**
 * @function: streamWriteLine
 *
 * @desc: 向流中写入一行
 *
 * @param pBuf: 字符缓存
 * @param iLen: 写入长度
 * @param iFileNo: 文件描述符
 *
 * @return int : >0:写入字节,<=0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int streamWriteLine(void *pBuf, int iLen, int iFileNo);

/**
 * @function: streamReadLine
 *
 * @desc: 从流中读入一行
 *
 * @param pBuf: 缓存
 * @param iLen: 缓存大小
 * @param piReadLen: 读入字节
 * @param iFileNo: 文件描述符
 *
 * @return int : >0:读出字节数, =0:文件结尾, <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int streamReadLine(void *pBuf, int iLen, int *piReadLen, int iFileNo);

/**
 * @function: streamWriteNbit
 *
 * @desc: 向流中写入若干位
 *
 * @param pBuf: 写入缓存
 * @param iBitCnt: 写入位数
 * @param iFileNo: 文件描述符
 *
 * @return int : >0:写入位数 <=0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int streamWriteNbit(void *pBuf, int iBitCnt, int iFileNo);

/**
 * @function: streamReadNbit
 *
 * @desc: 从流中读出若干位
 *
 * @param pBuf: 缓存
 * @param iBitCnt: 缓存大小，n bit
 * @param piReadBitCnt: 读出位数
 * @param iFileNo: 文件描述符
 *
 * @return int :  >0:读出位数 =0:文件结束 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int streamReadNbit(void *pBuf, int iBitCnt, int *piReadBitCnt, int iFileNo);

/**
 * @function: streamGets
 *
 * @desc: 从标准输入中读取一行，支持上下建选择历史命令
 *
 * @param pBuf: 缓存
 * @param iLen: 缓存大小
 *
 * @return char * : NULL:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
char * streamGets(char *psBuf, int iLen);

#ifdef __cplusplus
}
#endif

#endif /*_OS_STREAM_H_20100209101555_*/
/*-----------------------------  End ------------------------------------*/
