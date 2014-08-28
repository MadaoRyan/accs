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
 * @desc: ���ַ�������Base64����
 *
 * @param sInBuf: �����ַ���
 * @param nInBufLen: �����ַ�������
 * @param sOutBuf: ���������
 * @param pnOutBufLen: �������������
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int encEncodeBase64(unsigned char *sInBuf,  size_t  nInBufLen,
                    unsigned char *sOutBuf, size_t *pnOutBufLen);

/**
 * @function: encDecodeBase64
 *
 * @desc: ���ַ�������Base64����
 *
 * @param sInBuf: �����ַ���
 * @param nInBufLen: �����ַ�������
 * @param sOutBuf: ���������
 * @param pnOutBufLen: �������������
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int encDecodeBase64(unsigned char *sInBuf,  size_t  nInBufLen,
                    unsigned char *sOutBuf, size_t *pnOutBufLen);

/**
 * @function: encEncodeHexZero
 *
 * @desc: ���ַ�������HexZero����
 *
 * @param sInBuf: �����ַ���
 * @param nInBufLen: �����ַ�������
 * @param sOutBuf: ���������
 * @param pnOutBufLen: �������������
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int encEncodeHexZero(unsigned char *sInBuf,  size_t  nInBufLen,
                     unsigned char *sOutBuf, size_t *pnOutBufLen);

/**
 * @function: encDecodeHexZero
 *
 * @desc: ���ַ�������HexZero����
 *
 * @param sInBuf: �����ַ���
 * @param nInBufLen: �����ַ�������
 * @param sOutBuf: ���������
 * @param pnOutBufLen: �������������
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int encDecodeHexZero(unsigned char *sInBuf,  size_t  nInBufLen,
                     unsigned char *sOutBuf, size_t *pnOutBufLen);

/**
 * @function: encEncodeHex16
 *
 * @desc: ���ַ�������Hex16����
 *
 * @param sInBuf: �����ַ���
 * @param nInBufLen: �����ַ�������
 * @param sOutBuf: ���������
 * @param pnOutBufLen: �������������
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int encEncodeHex16(unsigned char *sInBuf,  size_t  nInBufLen,
                   unsigned char *sOutBuf, size_t *pnOutBufLen);

/**
 * @function: encDecodeHex16
 *
 * @desc: ���ַ�������Hex16����
 *
 * @param sInBuf: �����ַ���
 * @param nInBufLen: �����ַ�������
 * @param sOutBuf: ���������
 * @param pnOutBufLen: �������������
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int encDecodeHex16(unsigned char *sInBuf,  size_t  nInBufLen,
                   unsigned char *sOutBuf, size_t *pnOutBufLen);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_ENC_H_20100105103541_*/
/*-----------------------------  End ------------------------------------*/
