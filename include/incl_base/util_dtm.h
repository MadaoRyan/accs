/*
 *
 *
 * Date and Time Functions
 *
 *
 * FileName: util_dtm.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_DTM_H_20100105103319_
#define _UTIL_DTM_H_20100105103319_

/*--------------------------- Include files -----------------------------*/
#include	<sys/time.h>
#include	<time.h>

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/
typedef struct {
	struct timeval	tvSave;		/* 每次时间 */
	int				iCnt;		/* 计时次数 */
	struct timeval	tvTotal;	/* 累计时间 */
} T_TIMER;

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: dtmDate2Tt
 *
 * @desc: YYYYMMDD格式日期转换为time_t格式时间
 *
 * @param sDate: 输入的日期
 *
 * @return time_t: time_t格式时间
 *
 */
time_t dtmDate2Tt(char sDate[9]);

/**
 * @function: dtmTime2Tt
 *
 * @desc: YYYYMMDDHH24MISS格式日期时间转换为time_t格式时间
 *
 * @param sTime: 输入的时间
 *
 * @return time_t: time_t格式时间
 *
 */
time_t dtmTime2Tt(char sTime[15]);

/**
 * @function: dtmTt2Date
 *
 * @desc: time_t格式时间转换为YYYYMMDD格式日期
 *
 * @param ttTime: time_t格式时间
 * @param sDate: 输出缓存区
 *
 * @return char *: 输出缓存区
 *
 */
char *dtmTt2Date(time_t ttTime, char sDate[9]);

/**
 * @function: dtmTt2Time
 *
 * @desc: time_t格式时间转换为YYYYMMDDHH24MISS格式日期时间
 *
 * @param ttTime: time_t格式时间
 * @param sTime: 输出缓存区
 *
 * @return char *: 输出缓存区
 *
 */
char *dtmTt2Time(time_t ttTime, char sTime[15]);

/**
 * @function: dtmGetCurDate
 *
 * @desc: 获取当前日期
 *
 * @param sDate: 输出缓存区
 *
 * @return char *: 输出缓存区
 *
 */
char *dtmGetCurDate(char sDate[9]);

/**
 * @function: dtmGetCurTime
 *
 * @desc: 获取当前日期时间
 *
 * @param sTime: 输出缓存区
 *
 * @return char *: 输出缓存区
 *
 */
char *dtmGetCurTime(char sTime[15]);

/**
 * @function: dtmAddDays
 *
 * @desc: 计算日期+天数(可负)
 *
 * @param sDate: 输入日期，计算结果也会保存在此
 * @param nDayCnt: 天数
 *
 * @return char *: 结果日期
 *
 */
char *dtmAddDays(char sDate[9], int nDayCnt);

/**
 * @function: dtmAddSeconds
 *
 * @desc: 计算日期时间+秒数(可负)
 *
 * @param sTime: 输入日期时间，计算结果也会保存在此
 * @param nSecondCnt: 秒数
 *
 * @return char *: 结果日期时间
 *
 */
char *dtmAddSeconds(char sTime[15], int nSecondCnt);

/**
 * @function: dtmDaysBetween
 *
 * @desc: 计算两日期间隔天数(sDate1<sDate2时结果为负数)
 *
 * @param sDate1: 输入日期1
 * @param sDate2: 输入日期2
 *
 * @return int: 间隔天数
 *
 */
int dtmDaysBetween(char sDate1[9], char sDate2[9]);

/**
 * @function: dtmSecondsBetween
 *
 * @desc: 计算两日期时间间隔秒数(sTime1<sTime2)
 *
 * @param sTime1: 输入日期时间1
 * @param sTime2: 输入日期时间2
 *
 * @return int: 间隔秒数
 *
 */
int dtmSecondsBetween(char sTime1[15], char sTime2[15]);

/**
 * @function: dtmFmtTime
 *
 * @desc: 获得格式化日期时间
 *
 * @param sBuf: 输出缓存区
 * @param nBufLen: 输出缓存区长度
 * @param sFmt: 时间格式
 * @param ttTime: time_t格式时间
 *
 * @return char *: 输出缓存区
 *
 */
char *dtmFmtTime(char *sBuf, size_t nBufLen, char *sFmt, time_t ttTime);

/**
 * @function: dtmGetCurFmtTime
 *
 * @desc: 获得当前格式化日期时间
 *
 * @param sBuf: 输出缓存区
 * @param nBufLen: 输出缓存区长度
 * @param sFmt: 时间格式
 *
 * @return char *: 输出缓存区
 *
 */
char *dtmGetCurFmtTime(char *sBuf, size_t nBufLen, char *sFmt);

/**
 * @function: dtmStdTime
 *
 * @desc: 获得标准格式日期时间
 *
 * @param sBuf: 输出缓存区(最少应大于等于38)
 *
 * @return char *: 输出缓存区
 *
 */
char *dtmStdTime(char *sBuf);

/**
 * @function: dtmBeginTimer
 *
 * @desc: 计时开始
 *
 * @param ptvBegin: 开始的时间
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int dtmBeginTimer(struct timeval *ptvBegin);

/**
 * @function: dtmEndTimer
 *
 * @desc: 计时停止
 *
 * @param ptvBegin: 间隔的时间
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int dtmEndTimer(struct timeval *ptvBegin);

/**
 * @function: dtmBeginTimer2
 *
 * @desc: 计时开始
 *
 * @param ptTimer: Timer控制结构
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int dtmBeginTimer2(T_TIMER *ptTimer);

/**
 * @function: dtmEndTimer2
 *
 * @desc: 计时停止
 *
 * @param ptTimer: Timer控制结构
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int dtmEndTimer2(T_TIMER *ptTimer);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_DTM_H_20100105103319_*/
/*-----------------------------  End ------------------------------------*/
