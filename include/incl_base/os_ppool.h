/*
 *
 * 
 * process pool
 * 
 * 
 * FileName: os_ppool.h
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_PPOOL_H_20111221162117_
#define _OS_PPOOL_H_20111221162117_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/
/*Ret Code*/
#define ERR_OS_PPOOL_OK 0                                           /*成功*/
            
#define ERR_OS_PPOOL_FAIL_BASE - 500                                /*失败*/
#define ERR_OS_PPOOL_PARAM     (ERR_OS_PPOOL_FAIL_BASE - 1)         /*参数错误*/
#define ERR_OS_PPOOL_NOSPACE   (ERR_OS_PPOOL_FAIL_BASE - 2)         /*空间不足*/
#define ERR_OS_PPOOL_NOTFOUND  (ERR_OS_PPOOL_FAIL_BASE - 3)         /*未找到*/
#define ERR_OS_PPOOL_EXISTS    (ERR_OS_PPOOL_FAIL_BASE - 4)         /*已存在*/
#define ERR_OS_PPOOL_INIT      (ERR_OS_PPOOL_FAIL_BASE - 5)         /*初始化*/

#define ERR_OS_PPOOL_OTHER     (ERR_OS_PPOOL_FAIL_BASE - 99)         /*参数错误*/

/*options*/
#define OPT_OS_PPOOL_BLOCK 0x00000001                                /*阻塞模式*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: pplInit
 *
 * @desc: 初始化进程池
 *
 * @param iMax: 最大进程数
 * @param iOptions: 选项
 *
 * @return int: ret>=0 成功; ret<0 失败
 *
 */
int pplInit(int iMax, int iOptions);

/**
 * @function: pplFork
 *
 * @desc: 申请进程
 *
 * @param iOptions: 选项
 *
 * @return int: ret>0 进程号,父进程; ret==0 子进程; ret<0 失败
 *
 */
int pplFork(int iOptions);

/**
 * @function: pplKill
 *
 * @desc: 向进程发送信号
 *
 * @param iPid: 进程号，0-所有进程
 * @param iSignal: 信号
 *
 * @return int: ret>=0 发送进程数; ret<0 失败
 *
 */
int pplKill(int iPid, int iSignal);

/**
 * @function: pplCountAlive
 *
 * @desc: 计算进程池中活动进程个数
 *
 * @return long: ret>=0 进程数; ret<0 出错
 *
 */
int pplCountAlive(void);

/**
 * @function: pplUninit
 *
 * @desc: 释放进程池资源
 *
 * @param iOptions: 选项
 *
 * @return long: ret>=0 关闭进程数; ret<0 出错
 *
 */
int pplUninit(int iOptions);

/**
 *  获取当前进程序号
 *
 *  @return 进程序号
 */
int pplGetIndex();

#ifdef __cplusplus
}
#endif

#endif /*_OS_PPOOL_H_20111221162117_*/
/*-----------------------------  End ------------------------------------*/
