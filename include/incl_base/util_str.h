/*
 *
 *
 * String Functions
 *
 *
 * FileName: util_str.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_STR_H_20100105103547_
#define _UTIL_STR_H_20100105103547_

/*--------------------------- Include files -----------------------------*/
#include	<stddef.h>

/*--------------------------- Macro define ------------------------------*/
#ifdef  __GNUC__
#define __PRINTF__(f,s) __attribute__((format(printf, (f), (s))))
#else
#define __PRINTF__(f,s)
#endif

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: strRightTrim
 *
 * @desc: 去除字符串右边的空格
 *
 * @param sStr: 要操作的字符串
 *
 * @return char *: 操作完成的字符串
 *
 */
char *strRightTrim(char *sStr);

/**
 * @function: strLeftTrim
 *
 * @desc: 去除字符串左边的空格
 *
 * @param sStr: 要操作的字符串
 *
 * @return char *: 操作完成的字符串
 *
 */
char *strLeftTrim(char *sStr);

/**
 * @function: strTrim
 *
 * @desc: 去除字符串左右两边的空格
 *
 * @param sStr: 要操作的字符串
 *
 * @return char *: 操作完成的字符串
 *
 */
char *strTrim(char *sStr);

/**
 * @function: strInt2Str
 *
 * @desc: 将整型值转换成字符串
 *
 * @param nNum: 欲转换的整型值
 * @param sBuf: 输出缓存区
 * @param nBufLen: 输出缓存区长
 *
 * @return char *: 输出缓存区
 *
 */
char *strInt2Str(int nNum, char *sBuf, size_t nBufLen);

/**
 * @function: strStr2Int
 *
 * @desc: 将字符串转换成整型值
 *
 * @param sBuf: 输入字符串
 * @param nBufLen: 输入字符串长
 *
 * @return int: 转换成的整型值
 *
 */
int strStr2Int(char *sBuf, size_t nBufLen);

/**
 * @function: strLong2Str
 *
 * @desc: 将整型值转换成字符串
 *
 * @param nNum: 欲转换的整型值
 * @param sBuf: 输出缓存区
 * @param nBufLen: 输出缓存区长
 *
 * @return char *: 输出缓存区
 *
 */
char *strLong2Str(long nNum, char *sBuf, size_t nBufLen);

/**
 * @function: strStr2Long
 *
 * @desc: 将字符串转换成整型值
 *
 * @param sBuf: 输入字符串
 * @param nBufLen: 输入字符串长
 *
 * @return long: 转换成的整型值
 *
 */
long strStr2Long(char *sBuf, size_t nBufLen);

/**
 * @function: strLL2Str
 *
 * @desc: 将整型值转换成字符串
 *
 * @param nNum: 欲转换的整型值
 * @param sBuf: 输出缓存区
 * @param nBufLen: 输出缓存区长
 *
 * @return char *: 输出缓存区
 *
 */
char *strLL2Str(long long nNum, char *sBuf, size_t nBufLen);

/**
 * @function: strStr2LL
 *
 * @desc: 将字符串转换成整型值
 *
 * @param sBuf: 输入字符串
 * @param nBufLen: 输入字符串长
 *
 * @return long long: 转换成的整型值
 *
 */
long long strStr2LL(char *sBuf, size_t nBufLen);

/**
 * @function: strDbl2Str
 *
 * @desc: 将浮点值转换成字符串
 *
 * @param dNum: 欲转换的浮点值
 * @param sBuf: 输出缓存区
 * @param nBufLen: 输出缓存区长
 * @param nDot: 小数位数
 *
 * @return char *: 输出缓存区
 *
 * @comment:
 * (16.0 , 5, 0) -> 00016
 * (16.12, 5, 2) -> 01612
 *
 */
char *strDbl2Str(double dNum, char *sBuf, size_t nBufLen, int nDot);

/**
 * @function: strDbl2StrRaw
 *
 * @desc: 将浮点值转换成字符串
 *
 * @param dNum: 欲转换的浮点值
 * @param sBuf: 输出缓存区
 * @param nBufLen: 输出缓存区长
 * @param nDot: 小数位数
 *
 * @return char *: 输出缓存区
 *
 * @comment:
 * (16.0, 0) -> "16"
 * (16.12, 2) -> "0.16"
 * (1600, 2) -> "16.00"
 *
 */
char *strDbl2StrRaw(double dNum, char *sBuf, size_t nBufLen, int nDot);

/**
 * @function: strStr2Dbl
 *
 * @desc: 将字符串转换成浮点值
 *
 * @param sBuf: 输出缓存区
 * @param nBufLen: 输出缓存区长
 * @param nDot: 小数位数
 *
 * @return double: 转换成的浮点值
 *
 */
double strStr2Dbl(char *sBuf, size_t nBufLen, int nDot);

/**
 * @function: strStr2DblRaw
 *
 * @desc: 将字符串转换成浮点值
 *
 * @param sBuf: 输出缓存区
 * @param nBufLen: 输出缓存区长
 * @param nDot: 小数位数
 *
 * @return double: 转换成的浮点值
 *
 * @comment:
 * ("16.0" , 0) -> 16.0
 * ("16.12", 2) -> 1612.0
 * ("1600", -2) -> 16.0
 *
 */
double strStr2DblRaw(char *sBuf, size_t nBufLen, int nDot);

/**
 * @function: strMem2Str
 *
 * @desc: 复制字符串
 *
 * @param sDest: 目标缓存区
 * @param iDestSize: 目标缓存区大小
 * @param sSrc: 源缓存
 * @param iSrcLen: 源缓存长度
 *
 * @return char *: 目标缓存区
 *
 */
char *strMem2Str(char *sDest, int iDestSize, char *sSrc, int iSrcLen);

/**
 * @function: strToLower
 *
 * @desc: 将字符串中的大写字母转换为小写
 *
 * @param sStr: 要操作的字符串
 *
 * @return char *: 操作完成的字符串
 *
 */
char *strToLower(char *sStr);

/**
 * @function: strToUpper
 *
 * @desc: 将字符串中的小写字母转换为大写
 *
 * @param sStr:要操作的字符串
 *
 * @return char *: 操作完成的字符串
 *
 */
char *strToUpper(char *sStr);

/**
 * @function: strFirstUpper
 *
 * @desc: 将字符串的首位转换为大写, 其余转换为小写
 *
 * @param sStr:要操作的字符串
 *
 * @return char *: 操作完成的字符串
 *
 */
char *strFirstUpper(char *sStr);

/**
 * @function: strStrChr
 *
 * @desc: 从sBuf开始向[后]搜索sBuf
 *
 * @param sBuf: 搜索缓冲区
 * @param pIsMatch: 匹配函数
 * @param pIsEnd: 结尾匹配函数
 *
 * @return char *: ret!=NULL 成功; ret=NULL 失败
 *
 * @comment:
 *     匹配函数: 匹配成功时返回非0, 匹配失败时返回0
 *               若为NULL, 则返回sBuf
 * 结尾匹配函数: 到结尾时返回非0, 未到结尾时返回0
 *               若为NULL, 则默认为遇到'\0'时结束
 *
 */
char *strStrChr(char *sBuf, int (*pIsMatch)(char), int (*pIsEnd)(char));

/**
 * @function: strStrNStr
 *
 * @desc: 在字符串中查找目标字符串
 *
 * @param sBuf: 原字符串
 * @param iLen: 原字符串长
 * @param sTarget: 目标字符串
 *
 * @return char *: ret!=NULL 成功; ret=NULL 失败
 *
 */
char *strStrNStr(char *sBuf, int iLen, char *sTarget);

/**
 * @function: strDirName
 *
 * @desc: 获取路径中的目录名
 *
 * @param sPath: 要操作的路径
 *
 * @return char *: 操作完成的路径
 *
 */
char *strDirName(char *sPath);

/**
 * @function: strBaseName
 *
 * @desc: 获取路径中的目录名
 *
 * @param sPath:要操作的路径
 *
 * @return char *: 操作完成的路径
 *
 */
char *strBaseName(char *sPath);

/**
 * @function: strPathJoin
 *
 * @desc: 连接路径名与前缀
 *
 * @param sPrefix: 路径前缀
 * @param sPath: 要操作的路径
 *
 * @return char *: 操作完成的路径
 *
 */
char *strPathJoin(char *sPrefix, char *sPath);

/**
 * @function: strPathJoin2
 *
 * @desc: 连接路径名与后缀
 *
 * @param sPath: 要操作的路径
 * @param sAppend: 路径后缀
 *
 * @return char *: 操作完成的路径
 *
 */
char *strPathJoin2(char *sPath, char *sAppend);

/**
 * @function: strReplaceDir
 *
 * @desc: 替换目录名
 *
 * @param sPath: 要操作的路径
 * @param sDir: 新的目录路径
 *
 * @return char *: 操作完成的路径
 *
 */
char *strReplaceDir(char *sPath, char *sDir);

/**
 * @function: strLength
 *
 * @desc: 获取字符串长度
 *
 * @param sStr: 字符串
 * @param iMax: 最大长度
 *
 * @return int: 实际长度
 *
 */
int strLength(const char *sStr, int iMax);

/**
 * @function: strErrno
 *
 * @desc: 显示当前errno对应的错误信息
 *
 * @return char *: 错误信息
 *
 */
char *strErrno(void);

/**
 * @function: strAprintf
 *
 * @desc: 打印至新分配的内存区域
 *
 * @param sFmt: 格式信息
 *
 * @return char *: ret!=NULL 成功; ret=NULL 失败
 *
 */
char *strAprintf(const char *sFmt, ...) __PRINTF__(1,2);

/**
 * @function: strEnv
 *
 * @desc: 获得环境变量，若该环境变量不存在则返回空字符串
 *
 * @param sEnv: 环境变量名称
 *
 * @return const char *: 环境变量
 *
 */
const char *strEnv(const char *sEnv);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_STR_H_20100105103547_*/
/*-----------------------------  End ------------------------------------*/
