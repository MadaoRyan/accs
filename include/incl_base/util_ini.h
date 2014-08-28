/*
 *
 *
 * INI Operation Functions
 *
 *
 * FileName: util_ini.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_INI_H_20100107182413_
#define _UTIL_INI_H_20100107182413_

/*--------------------------- Include files -----------------------------*/
#include	<stddef.h>

/*--------------------------- Macro define ------------------------------*/
#ifndef	INI_MAX_LINE
#define	INI_MAX_LINE	4096
#endif

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: iniReadString
 *
 * @desc: ��INI�ļ��ж�ȡ�ַ���
 *
 * @param sSection: ����
 * @param sKey: ����
 * @param sDefault: Ĭ��ֵ
 * @param sOutBuf: ���������
 * @param nInBufLen: �������������
 * @param sFile: INI�ļ���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int iniReadString(char *sSection, char *sKey, char *sDefault,
                  char *sOutBuf, size_t nInBufLen, char *sFile);

/**
 * @function: iniReadInt
 *
 * @desc: ��INI�ļ��ж�ȡ��ֵ
 *
 * @param sSection: ����
 * @param sKey: ����
 * @param sDefault: Ĭ��ֵ
 * @param sFile: INI�ļ���
 *
 * @return int: ��ֵ
 *
 */
int iniReadInt(char *sSection, char *sKey, int nDefault, char *sFile);

/**
 * @function: iniWriteString
 *
 * @desc: ��INI�ļ���д���ַ���
 *
 * @param sSection: ����
 * @param sKey: ����
 * @param sInBuf: ��д��ֵ
 * @param sFile: INI�ļ���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int iniWriteString(char *sSection, char *sKey, char *sInBuf, char *sFile);

/**
 * @function: iniReadString2
 *
 * @desc: ��INI�ļ��ж�ȡ�ַ���(֧���ֶ��ض���)
 *
 * @param sSection: ����
 * @param sKey: ����
 * @param sDefault: Ĭ��ֵ
 * @param sOutBuf: ���������
 * @param nInBufLen: �������������
 * @param sFile: INI�ļ���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int iniReadString2(char *sSection, char *sKey, char *sDefault,
                   char *sOutBuf, size_t iInBufLen, char *sFile);

/**
 * @function: iniReadInt2
 *
 * @desc: ��INI�ļ��ж�ȡ��ֵ(֧���ֶ��ض���)
 *
 * @param sSection: ����
 * @param sKey: ����
 * @param sDefault: Ĭ��ֵ
 * @param sFile: INI�ļ���
 *
 * @return int: ��ֵ
 *
 */
int iniReadInt2(char *sSection, char *sKey, int iDefault, char *sFile);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_INI_H_20100107182413_*/
/*-----------------------------  End ------------------------------------*/
