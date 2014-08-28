/*
 *
 *
 * env module for env.
 *
 *
 * FileName: os_env.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_ENV_H_20110824150051_
#define _OS_ENV_H_20110824150051_
/*--------------------------- Include files -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: envParseStr
 *
 * @desc: �滻��������
 *
 * @param sOld: ԭ�ַ���
 * @param sNew: �滻���ַ���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int envParseStr(char *sOld, char *sNew);

#ifdef __cplusplus
}
#endif

#endif /*_OS_ENV_H_20110824150051_*/
/*-----------------------------  End ------------------------------------*/
