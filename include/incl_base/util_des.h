/*
 *
 *
 * Encrypt and Decrypt Functions
 *
 *
 * FileName: util_des.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_DES_H_20100105103527_
#define _UTIL_DES_H_20100105103527_

/*--------------------------- Include files -----------------------------*/
#include	<stddef.h>

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: desEncrypt3DES
 *
 * @desc: 对字符串进行3DES加密
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 * @param sKey: 加密密钥(24位)
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int desEncrypt3DES(unsigned char *sInBuf,  size_t  nInBufLen,
                   unsigned char *sOutBuf, size_t *pnOutBufLen,
                   unsigned char sKey[24]);

/**
 * @function: desDecrypt3DES
 *
 * @desc: 对字符串进行3DES解密
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 * @param sKey: 加密密钥(24位)
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int	desDecrypt3DES(unsigned char *sInBuf,  size_t  nInBufLen,
                   unsigned char *sOutBuf, size_t *pnOutBufLen,
                   unsigned char sKey[24]);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_DES_H_20100105103527_*/
/*-----------------------------  End ------------------------------------*/
