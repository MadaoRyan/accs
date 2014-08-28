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
 * @desc: 替换环境变量
 *
 * @param sOld: 原字符串
 * @param sNew: 替换后字符串
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int envParseStr(char *sOld, char *sNew);

#ifdef __cplusplus
}
#endif

#endif /*_OS_ENV_H_20110824150051_*/
/*-----------------------------  End ------------------------------------*/
