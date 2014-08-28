/*
 *
 *
 * iconv wrapper functions
 *
 *
 * FileName: util_conv.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_CONV_H_20111209102216_
#define _UTIL_CONV_H_20111209102216_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/
#define	CONV_OPEN_MAX	8

#define	CONV_CODE_GB	"GB18030"
#define	CONV_CODE_UTF8	"UTF-8"

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: convOpen
 *
 * @desc: 打开编码转换文件
 *
 * @param sFrom: 源编码
 * @param sTo: 目标编码
 *
 * @return int: ret>=0 编码转换句柄; ret<0 失败
 *
 */
int convOpen(char *sFrom, char *sTo);

/**
 * @function: convClose
 *
 * @desc: 关闭编码转换文件
 *
 * @param iConv: 编码转换句柄
 *
 * @return int: ret>=0 成功; ret<0 失败
 *
 */
int convClose(int iConv);

/**
 * @function: convCloseAll
 *
 * @desc: 关闭所有已打开的编码转换文件
 *
 * @return int: ret>=0 成功; ret<0 失败
 *
 */
int convCloseAll(void);

/**
 * @function: convTransform
 *
 * @desc: 编码转换
 *
 * @param iConv: 编码转换句柄
 * @param sIn: 输入缓存
 * @param iInLen: 输入缓存长度
 * @param sOut: 输出缓存
 * @param iOutLen: 输出缓存长度
 *
 * @return int: ret>=0 输出长度; ret<0 失败
 *
 */
int convTrans(int iConv, char *sIn, int iInLen, char *sOut, int iOutLen);

/**
 * @function: convTransform
 *
 * @desc: 编码转换
 *
 * @param sFrom: 源编码
 * @param sTo: 目标编码
 * @param sIn: 输入缓存
 * @param iInLen: 输入缓存长度
 * @param sOut: 输出缓存
 * @param iOutLen: 输出缓存长度
 *
 * @return int: ret>=0 输出长度; ret<0 失败
 *
 */
int convTransform(char *sFrom, char *sTo,
                  char *sIn, int iInLen, char *sOut, int iOutLen);

/**
 * @function: convFileTransform
 *
 * @desc: 文件编码转换
 *
 * @param sFrom: 源编码
 * @param sTo: 目标编码
 * @param sFileIn: 输入文件名
 * @param sFileOut: 输出文件名
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int convFileTransform(char *sFrom, char *sTo,
                      char *sFileIn, char *sFileOut);

/**
 * @function: convGBtoUTF8
 *
 * @desc: 从GB18030编码转换至UTF-8编码
 *
 * @param sIn: 输入缓存
 * @param iInLen: 输入缓存长度
 * @param sOut: 输出缓存
 * @param iOutLen: 输出缓存长度
 *
 * @return int: ret>=0 输出长度; ret<0 失败
 *
 */
int convGBtoUTF8(char *sIn, int iInLen, char *sOut, int iOutLen);

/**
 * @function: convUTF8toGB
 *
 * @desc: 从UTF-8编码转换至GB18030编码
 *
 * @param sIn: 输入缓存
 * @param iInLen: 输入缓存长度
 * @param sOut: 输出缓存
 * @param iOutLen: 输出缓存长度
 *
 * @return int: ret>=0 输出长度; ret<0 失败
 *
 */
int convUTF8toGB(char *sIn, int iInLen, char *sOut, int iOutLen);

/**
 * @function: convTransformS
 *
 * @desc: 编码转换(该版本会输出'\0')
 *
 * @param sFrom: 源编码
 * @param sTo: 目标编码
 * @param sIn: 输入缓存
 * @param iInLen: 输入缓存长度
 * @param sOut: 输出缓存
 * @param iOutLen: 输出缓存长度
 *
 * @return int: ret>=0 输出长度; ret<0 失败
 *
 */
int convTransformS(char *sFrom, char *sTo,
                   char *sIn, int iInLen, char *sOut, int iOutLen);

/**
 * @function: convGBtoUTF8s
 *
 * @desc: 从GB18030编码转换至UTF-8编码(该版本会输出'\0')
 *
 * @param sIn: 输入缓存
 * @param iInLen: 输入缓存长度
 * @param sOut: 输出缓存
 * @param iOutLen: 输出缓存长度
 *
 * @return int: ret>=0 输出长度; ret<0 失败
 *
 */
int convGBtoUTF8s(char *sIn, int iInLen, char *sOut, int iOutLen);

/**
 * @function: convUTF8toGBs
 *
 * @desc: 从UTF-8编码转换至GB18030编码(该版本会输出'\0')
 *
 * @param sIn: 输入缓存
 * @param iInLen: 输入缓存长度
 * @param sOut: 输出缓存
 * @param iOutLen: 输出缓存长度
 *
 * @return int: ret>=0 输出长度; ret<0 失败
 *
 */
int convUTF8toGBs(char *sIn, int iInLen, char *sOut, int iOutLen);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_CONV_H_20111209102216_*/
/*-----------------------------  End ------------------------------------*/
