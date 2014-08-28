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
 * @desc: 读取文件内容至内存
 *
 * @param sFile: 文件名称
 * @param sBuf: 目标内存区域
 * @param *piLen: in-目标内存区域大小 out-读取的文件大小
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int filRead(char *sFile, char *sBuf, int *piLen);

/**
 * @function: filWrite
 *
 * @desc: 写内存至文件
 *
 * @param sBuf: 内存区域
 * @param iLen: 目标内存区域大小
 * @param sFile: 目标文件名称
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int filWrite(char *sBuf, int iLen, char *sFile);

/**
 * @function: filDateDir
 *
 * @desc: 生成日期路径并建立该目录
 *
 * @param sPath: in-原始路径 out-日期路径
 *
 * @return char *: ret!=NULL 日期路径; ret=NULL 失败
 *
 */
char *filDateDir(char *sPath);

/**
 * @function: filDateDir2
 *
 * @desc: 生成日期路径并建立该目录(避免单一目录文件太多)
 *
 * @param sPath: in-原始路径 out-日期路径
 *
 * @return char *: ret!=NULL 日期路径; ret=NULL 失败
 *
 */
char *filDateDir2(char *sPath);

/**
 * @function: filGetSeqNext
 *
 * @desc: 在文件中读取序号
 *
 * @param sFileName: 文件路径
 * @param iMax: 序号最大值
 *
 * @return int: ret>=0 序号; ret<0 失败
 *
 */
int filGetSeqNext(char *sFileName, int iMax);

/**
 * @function: filGetLine
 *
 * @desc: 从文件中读取一行
 *
 * @param sBuf: 目标缓存
 * @param iLen: 目标缓存大小
 * @param fpFile: 打开的文件
 *
 * @return int:  ret>=0 读取到的长度; ret<0 失败
 *
 */
int filGetLine(char *sBuf, int iLen, FILE *fpFile);


/**
 * @function: filGetUniExt
 *
 * @desc: 生成唯一后缀
 *
 * @param psFileName: 文件名
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
