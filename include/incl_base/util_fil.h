/*
 *
 *
 * File Handle Functions
 *
 *
 * FileName: util_fil.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_FIL_H_20100105103547_
#define _UTIL_FIL_H_20100105103547_

/*--------------------------- Include files -----------------------------*/
#include	<stdio.h>

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: filRead
 *
 * @desc: ��ȡ�ļ��������ڴ�
 *
 * @param sFile: �ļ�����
 * @param sBuf: Ŀ���ڴ�����
 * @param *piLen: in-Ŀ���ڴ������С out-��ȡ���ļ���С
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int filRead(char *sFile, char *sBuf, int *piLen);

/**
 * @function: filWrite
 *
 * @desc: д�ڴ����ļ�
 *
 * @param sBuf: �ڴ�����
 * @param iLen: Ŀ���ڴ������С
 * @param sFile: Ŀ���ļ�����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int filWrite(char *sBuf, int iLen, char *sFile);

/**
 * @function: filDateDir
 *
 * @desc: ��������·����������Ŀ¼
 *
 * @param sPath: in-ԭʼ·�� out-����·��
 *
 * @return char *: ret!=NULL ����·��; ret=NULL ʧ��
 *
 */
char *filDateDir(char *sPath);

/**
 * @function: filDateDir2
 *
 * @desc: ��������·����������Ŀ¼(���ⵥһĿ¼�ļ�̫��)
 *
 * @param sPath: in-ԭʼ·�� out-����·��
 *
 * @return char *: ret!=NULL ����·��; ret=NULL ʧ��
 *
 */
char *filDateDir2(char *sPath);

/**
 * @function: filGetSeqNext
 *
 * @desc: ���ļ��ж�ȡ���
 *
 * @param sFileName: �ļ�·��
 * @param iMax: ������ֵ
 *
 * @return int: ret>=0 ���; ret<0 ʧ��
 *
 */
int filGetSeqNext(char *sFileName, int iMax);

/**
 * @function: filGetLine
 *
 * @desc: ���ļ��ж�ȡһ��
 *
 * @param sBuf: Ŀ�껺��
 * @param iLen: Ŀ�껺���С
 * @param fpFile: �򿪵��ļ�
 *
 * @return int:  ret>=0 ��ȡ���ĳ���; ret<0 ʧ��
 *
 */
int filGetLine(char *sBuf, int iLen, FILE *fpFile);


/**
 * @function: filGetUniExt
 *
 * @desc: ����Ψһ��׺
 *
 * @param psFileName: �ļ���
 *
 * @return int:  ret:0
 *
 */
int filGetUniExt(char *psFileName);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_FIL_H_20100105103547_*/
/*-----------------------------  End ------------------------------------*/
