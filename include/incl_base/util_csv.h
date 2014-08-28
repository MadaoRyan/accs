/*
 *
 *
 * CSV Operation Functions
 *
 *
 * FileName: util_csv.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_CSV_H_20100107154922_
#define _UTIL_CSV_H_20100107154922_

/*--------------------------- Include files -----------------------------*/
#include	<stdio.h>

/*--------------------------- Macro define ------------------------------*/
#ifndef	CSV_MAX_LINE
#define	CSV_MAX_LINE	4096
#endif

#ifndef	CSV_MAX_COL
#define	CSV_MAX_COL		64
#endif

/*---------------------------- Type define ------------------------------*/
typedef struct {
	FILE	*fpFile;				/* CSV文件指针 */
	char	sBuf[CSV_MAX_LINE];		/* 该行缓存 */
	int		nCnt;					/* 字段个数 */
	char	*psCol[CSV_MAX_COL];	/* 字段数组，例:psCol[0]为第一个字段 */
} T_CSV;

typedef struct {
	int		iCnt;					/* 字段个数 */
	char	*psCol[CSV_MAX_COL];	/* 字段数组，例:psCol[0]为第一个字段 */
} T_SPLIT_STR;

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: csvOpen
 *
 * @desc: 打开CSV文件
 *
 * @param sFileName: CSV文件名
 *
 * @return T_CSV *: ret!=NULL CSV句柄; ret=NULL 失败
 *
 */
T_CSV *csvOpen(char *sFileName);

/**
 * @function: csvReadRow
 *
 * @desc: 从CSV文件中读取一行
 *
 * @param ptCsv: CSV句柄
 * @param cSep: 字段分隔符
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvReadRow(T_CSV *ptCsv, char cSep);

/**
 * @function: csvReadRow2
 *
 * @desc: 从CSV文件中读取一行
 *
 * @param ptCsv: CSV句柄
 * @param sSep: 字段分隔符
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvReadRow2(T_CSV *ptCsv, const char *sSep);

/**
 * @function: csvReadRowSkip
 *
 * @desc: 从CSV文件中读取一行
 *
 * @param ptCsv: CSV句柄
 * @param cSep: 字段分隔符
 * @param cSkip: 注释字符
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvReadRowSkip(T_CSV *ptCsv, char cSep, char cSkip);

/**
 * @function: csvReadRowN
 *
 * @desc: 从CSV文件中读取一行
 *
 * @param ptCsv: CSV句柄
 * @param cSep: 字段分隔符
 * @param iCnt: 分割数量
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvReadRowN(T_CSV *ptCsv, char cSep, int iCnt);

/**
 * @function: csvReadRowN2
 *
 * @desc: 从CSV文件中读取一行
 *
 * @param ptCsv: CSV句柄
 * @param sSep: 字段分隔符
 * @param iCnt: 分割数量
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvReadRowN2(T_CSV *ptCsv, const char *sSep, int iCnt);

/**
 * @function: csvReadRowSkipN
 *
 * @desc: 从CSV文件中读取一行
 *
 * @param ptCsv: CSV句柄
 * @param cSep: 字段分隔符
 * @param cSkip: 注释字符
 * @param iCnt: 分割数量
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvReadRowSkipN(T_CSV *ptCsv, char cSep, char cSkip, int iCnt);

/**
 * @function: csvReadRowExt
 *
 * @desc: 从CSV文件中读取一行
 *
 * @param ptCsv: CSV句柄
 * @param cSep: 字段分隔符
 * @param cQuote: 引用字符
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvReadRowExt(T_CSV *ptCsv, char cSep, char cQuote);

/**
 * @function: csvReadRowExtR
 *
 * @desc: 从CSV文件中读取一行
 *
 * @param fpFile: 文件句柄
 * @param cSep: 字段分隔符
 * @param cQuote: 引用字符
 * @param piCnt: 列数
 * @param psCol: 列值
 * @param sBuf: 行缓冲区
 * @param iSize: 行缓冲区大小
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvReadRowExtR(FILE *fpFile, char cSep, char cQuote,
                   int *piCnt, char *psCol[], char *sBuf, int iSize);

/**
 * @function: csvClose
 *
 * @desc: 关闭CSV文件
 *
 * @param ptCsv: CSV句柄
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvClose(T_CSV *ptCsv);

/**
 * @function: csvSplitStr
 *
 * @desc: 分割字符串
 *
 * @param sStr: 欲分割的字符串(会修改该字符串内容)
 * @param cSep: 字段分隔符
 * @param ptSplitStr: 分割后的结构体
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvSplitStr(char *sStr, char cSep, T_SPLIT_STR *ptSplitStr);

/**
 * @function: csvSplitStr2
 *
 * @desc: 分割字符串
 *
 * @param sStr: 欲分割的字符串(会修改该字符串内容)
 * @param sSep: 字段分隔符
 * @param ptSplitStr: 分割后的结构体
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvSplitStr2(char *sStr, const char *sSep, T_SPLIT_STR *ptSplitStr);

/**
 * @function: csvSplitStrN
 *
 * @desc: 分割字符串
 *
 * @param sStr: 欲分割的字符串(会修改该字符串内容)
 * @param cSep: 字段分隔符
 * @param iCnt: 分割数量
 * @param ptSplitStr: 分割后的结构体
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvSplitStrN(char *sStr, char cSep, int iCnt, T_SPLIT_STR *ptSplitStr);

/**
 * @function: csvSplitStrN2
 *
 * @desc: 分割字符串
 *
 * @param sStr: 欲分割的字符串(会修改该字符串内容)
 * @param sSep: 字段分隔符
 * @param iCnt: 分割数量
 * @param ptSplitStr: 分割后的结构体
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvSplitStrN2(char *sStr, const char *sSep, int iCnt,
                  T_SPLIT_STR *ptSplitStr);

/**
 * @function: csvWrite
 *
 * @desc: 写CSV文件
 *
 * @param fpFile: 文件句柄
 * @param cSep: 字段分隔符
 * @param iCnt: 字段数量
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvWrite(FILE *fpFile, char cSep, int iCnt, ...);

/**
 * @function: csvWriteV
 *
 * @desc: 写CSV文件
 *
 * @param fpFile: 文件句柄
 * @param cSep: 字段分隔符
 * @param iCnt: 字段数量
 * @param psCol: 字段数组
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvWriteV(FILE *fpFile, char cSep, int iCnt, char *psCol[]);

/**
 * @function: csvWriteExt
 *
 * @desc: 写CSV文件
 *
 * @param fpFile: 目标文件
 * @param cSep: 字段分隔符
 * @param cQuote: 引用字符
 * @param iCnt: 字段数量
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvWriteExt(FILE *fpFile, char cSep, char cQuote, int iCnt, ...);

/**
 * @function: csvWriteExtV
 *
 * @desc: 写CSV文件
 *
 * @param fpFile: 目标文件
 * @param cSep: 字段分隔符
 * @param cQuote: 引用字符
 * @param iCnt: 字段数量
 * @param psCol: 字段数组
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int csvWriteExtV(FILE *fpFile, char cSep, char cQuote, int iCnt, char *psCol[]);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_CSV_H_20100107154922_*/
/*-----------------------------  End ------------------------------------*/
