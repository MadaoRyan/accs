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
 * @desc: ���ַ�������3DES����
 *
 * @param sInBuf: �����ַ���
 * @param nInBufLen: �����ַ�������
 * @param sOutBuf: ���������
 * @param pnOutBufLen: �������������
 * @param sKey: ������Կ(24λ)
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int desEncrypt3DES(unsigned char *sInBuf,  size_t  nInBufLen,
                   unsigned char *sOutBuf, size_t *pnOutBufLen,
                   unsigned char sKey[24]);

/**
 * @function: desDecrypt3DES
 *
 * @desc: ���ַ�������3DES����
 *
 * @param sInBuf: �����ַ���
 * @param nInBufLen: �����ַ�������
 * @param sOutBuf: ���������
 * @param pnOutBufLen: �������������
 * @param sKey: ������Կ(24λ)
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
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
