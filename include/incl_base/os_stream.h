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
 * @desc: ������д�������ֽ�
 *
 * @param pBuf: ���뻺��
 * @param iLen: д���ֽ���
 * @param iFileNo: �ļ�������
 *
 * @return int : >:д���ֽ���,<=0:����
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
 * @desc: ���ֽ����ж��������ֽ�
 *
 * @param pBuf: ����
 * @param iLen: �����С
 * @param piReadLen: �����ֽ���
 * @param iFileNo: �ļ�������
 *
 * @return int : >0:�����ֽ���, =0:�ļ���β, <0:����
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
 * @desc: ������д��һ��
 *
 * @param pBuf: �ַ�����
 * @param iLen: д�볤��
 * @param iFileNo: �ļ�������
 *
 * @return int : >0:д���ֽ�,<=0:����
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
 * @desc: �����ж���һ��
 *
 * @param pBuf: ����
 * @param iLen: �����С
 * @param piReadLen: �����ֽ�
 * @param iFileNo: �ļ�������
 *
 * @return int : >0:�����ֽ���, =0:�ļ���β, <0:����
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
 * @desc: ������д������λ
 *
 * @param pBuf: д�뻺��
 * @param iBitCnt: д��λ��
 * @param iFileNo: �ļ�������
 *
 * @return int : >0:д��λ�� <=0:����
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
 * @desc: �����ж�������λ
 *
 * @param pBuf: ����
 * @param iBitCnt: �����С��n bit
 * @param piReadBitCnt: ����λ��
 * @param iFileNo: �ļ�������
 *
 * @return int :  >0:����λ�� =0:�ļ����� <0:����
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
 * @desc: �ӱ�׼�����ж�ȡһ�У�֧�����½�ѡ����ʷ����
 *
 * @param pBuf: ����
 * @param iLen: �����С
 *
 * @return char * : NULL:����
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
