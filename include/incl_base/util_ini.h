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
 * @desc: 从INI文件中读取字符串
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sDefault: 默认值
 * @param sOutBuf: 输出缓冲区
 * @param nInBufLen: 输出缓冲区长度
 * @param sFile: INI文件名
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int iniReadString(char *sSection, char *sKey, char *sDefault,
                  char *sOutBuf, size_t nInBufLen, char *sFile);

/**
 * @function: iniReadInt
 *
 * @desc: 从INI文件中读取数值
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sDefault: 默认值
 * @param sFile: INI文件名
 *
 * @return int: 键值
 *
 */
int iniReadInt(char *sSection, char *sKey, int nDefault, char *sFile);

/**
 * @function: iniWriteString
 *
 * @desc: 向INI文件中写入字符串
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sInBuf: 欲写入值
 * @param sFile: INI文件名
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int iniWriteString(char *sSection, char *sKey, char *sInBuf, char *sFile);

/**
 * @function: iniReadString2
 *
 * @desc: 从INI文件中读取字符串(支持字段重定向)
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sDefault: 默认值
 * @param sOutBuf: 输出缓冲区
 * @param nInBufLen: 输出缓冲区长度
 * @param sFile: INI文件名
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int iniReadString2(char *sSection, char *sKey, char *sDefault,
                   char *sOutBuf, size_t iInBufLen, char *sFile);

/**
 * @function: iniReadInt2
 *
 * @desc: 从INI文件中读取数值(支持字段重定向)
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sDefault: 默认值
 * @param sFile: INI文件名
 *
 * @return int: 键值
 *
 */
int iniReadInt2(char *sSection, char *sKey, int iDefault, char *sFile);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_INI_H_20100107182413_*/
/*-----------------------------  End ------------------------------------*/
