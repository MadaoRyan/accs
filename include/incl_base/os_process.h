/*
 *
 * 
 * os module process functions .
 * 
 * 
 * FileName: os_process.h
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_PROCESS_H_20100315144126_
#define _OS_PROCESS_H_20100315144126_
/*--------------------------- Include files -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*--------------------------- Macro define ------------------------------*/
#define VAR_OS_PROCESS_OPTION_DEFAULT  0x00000000   /*default*/
#define VAR_OS_PROCESS_OPTION_NODEFUCT 0x00000001   /*NO defuct process*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: procParseAgrs
 *
 * @desc: 分析参数
 *
 * @param psLine: 输入参数，该参数将被修改
 * @param psArgv[]: 参数指针数组
 *
 * @return int: >=0:参数个数 <0:出错
 *
 * @comment:
 * 将psLine以空格分隔转换为\0分隔，并用记录至psArgv
 *
 *
 * @sample:
 *
 *
 */
int procParseAgrs(char *psLine, char *psArgv[]);

/**
 * @function: procStartProcess
 *
 * @desc: 启动一个进程
 *
 * @param psProcName: 进程名
 * @param psBin: 可执行程序
 * @param psArgs: 参数
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int procStartProcess(char *psProcName, char *psBin, char *psArgs);

/**
 * @function: procStartProcessE
 *
 * @desc: 启动一个进程
 *
 * @param psProcName: 进程名
 * @param psBin: 可执行程序
 * @param psArgs: 参数
 * @param iOption: 选项
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int procStartProcessE(char *psProcName, char *psBin, char *psArgs, int iOption);

/**
 * @function: procFork
 *
 * @desc: 创建进程
 *
 * @param iOption: 选项
 *
 * @return int: 0:子进程 >0:父进程 <0:出错
 *
 * @comment:
 * @sample:
 *
 *
 */
int procFork(int iOption);

/**
 * @function: procSndSig2Proc
 *
 * @desc: 向进程发信号
 *
 * @param iPid: 进程Id
 * @param iSig: 信号
 *
 * @return int:  0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int procSndSig2Proc(int iPid, int iSig);

/**
 * @function: procRunAsDaemon
 *
 * @desc: 进程以Daemon方式运行
 *
 * @param psRoot: 工作路径
 * @param iMask: 掩码
 * @param iOptions: 选项
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int procRunAsDaemon(char *psRoot, int iMask, int iOptions);

/**
 * @function: procIsExist
 *
 * @desc: 确定一个进程是否存在
 *
 * @param iPid: 进程号
 *
 * @return int: >=0: 存在, <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int procIsExist(int iPid);

#ifdef __cplusplus
}
#endif

#endif /*_OS_PROCESS_H_20100315144126_*/
/*-----------------------------  End ------------------------------------*/
