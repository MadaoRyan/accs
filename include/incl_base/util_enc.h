/*
 *
 *
 * Encode and Decode Functions
 *
 *
 * FileName: util_enc.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_ENC_H_20100105103541_
#define _UTIL_ENC_H_20100105103541_

/*--------------------------- Include files -----------------------------*/
#include	<stddef.h>

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: encEncodeBase64
 *
 * @desc: 对字符串进行Base64编码
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int encEncodeBase64(unsigned char *sInBuf,  size_t  nInBufLen,
                    unsigned char *sOutBuf, size_t *pnOutBufLen);

/**
 * @function: encDecodeBase64
 *
 * @desc: 对字符串进行Base64解码
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int encDecodeBase64(unsigned char *sInBuf,  size_t  nInBufLen,
                    unsigned char *sOutBuf, size_t *pnOutBufLen);

/**
 * @function: encEncodeHexZero
 *
 * @desc: 对字符串进行HexZero编码
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int encEncodeHexZero(unsigned char *sInBuf,  size_t  nInBufLen,
                     unsigned char *sOutBuf, size_t *pnOutBufLen);

/**
 * @function: encDecodeHexZero
 *
 * @desc: 对字符串进行HexZero解码
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int encDecodeHexZero(unsigned char *sInBuf,  size_t  nInBufLen,
                     unsigned char *sOutBuf, size_t *pnOutBufLen);

/**
 * @function: encEncodeHex16
 *
 * @desc: 对字符串进行Hex16编码
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int encEncodeHex16(unsigned char *sInBuf,  size_t  nInBufLen,
                   unsigned char *sOutBuf, size_t *pnOutBufLen);

/**
 * @function: encDecodeHex16
 *
 * @desc: 对字符串进行Hex16解码
 *
 * @param sInBuf: 输入字符串
 * @param nInBufLen: 输入字符串长度
 * @param sOutBuf: 输出缓冲区
 * @param pnOutBufLen: 输出缓冲区长度
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int encDecodeHex16(unsigned char *sInBuf,  size_t  nInBufLen,
                   unsigned char *sOutBuf, size_t *pnOutBufLen);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_ENC_H_20100105103541_*/
/*-----------------------------  End ------------------------------------*/
