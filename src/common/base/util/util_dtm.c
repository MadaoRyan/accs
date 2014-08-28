/*
 *
 *
 * Date and Time Functions
 *
 *
 * FileName: util_dtm.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */

/*--------------------------- Include files -----------------------------*/
#include	"util_dtm.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

/*--------------------------- Macro define ------------------------------*/
#define	DTM_DAYSECONDS	(60 * 60 * 24)

/*---------------------------- Type define ------------------------------*/
typedef struct timeval timeval_t;

/*---------------------- Local function declaration ---------------------*/
static timeval_t *dtmTimeValAdd(timeval_t *ptvA, timeval_t *ptvB);
static timeval_t *dtmTimeValSub(timeval_t *ptvA, timeval_t *ptvB);

/*-------------------------  Global variable ----------------------------*/
static const char *f_sDays[] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
};

static const char *f_sMonths[] = {
	 "Jan", "Feb", "Mar", "Apr", "May", "Jun",
	 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};

/*-------------------------  Global functions ---------------------------*/
/**
 * YYYYMMDD格式日期转换为time_t格式时间
 *
 * @param sDate: 输入的日期
 *
 * @return time_t格式时间
 *
 */
time_t
dtmDate2Tt(char sDate[9])
{
	struct tm	tmTime;

	memset(&tmTime, 0, sizeof(tmTime));

	sscanf(sDate, "%4d%2d%2d", &tmTime.tm_year, &tmTime.tm_mon,
		&tmTime.tm_mday);

	tmTime.tm_year -= 1900;
	tmTime.tm_mon -= 1;

	return (mktime(&tmTime));
}

/**
 * YYYYMMDDHH24MISS格式日期时间转换为time_t格式时间
 *
 * @param sTime: 输入的日期
 *
 * @return time_t格式时间
 *
 */
time_t
dtmTime2Tt(char sTime[15])
{
	struct tm	tmTime;

	memset(&tmTime, 0, sizeof(tmTime));

	sscanf(sTime, "%4d%2d%2d%2d%2d%2d", &tmTime.tm_year,
		&tmTime.tm_mon, &tmTime.tm_mday, &tmTime.tm_hour,
		&tmTime.tm_min, &tmTime.tm_sec);

	tmTime.tm_year -= 1900;
	tmTime.tm_mon -= 1;

	return (mktime(&tmTime));
}

/**
 * time_t格式时间转换为YYYYMMDD格式日期
 *
 * @param ttTime: time_t格式时间
 * @param sTime: 输出缓存区
 *
 * @return sTime
 *
 */
char *
dtmTt2Date(time_t ttTime, char sDate[9])
{
	struct tm	*tmTime;

	tzset();
	tmTime = localtime(&ttTime);

	sprintf(sDate, "%4d%02d%02d", tmTime->tm_year + 1900,
		tmTime->tm_mon + 1, tmTime->tm_mday);

	return (sDate);
}

/**
 * time_t格式时间转换为YYYYMMDDHH24MISS格式日期时间
 *
 * @param ttTime: time_t格式时间
 * @param sTime: 输出缓存区
 *
 * @return sTime
 *
 */
char *
dtmTt2Time(time_t ttTime, char sTime[15])
{
	struct tm	*tmTime;

	tzset();
	tmTime = localtime(&ttTime);

	sprintf(sTime, "%4d%02d%02d%02d%02d%02d", tmTime->tm_year + 1900,
		tmTime->tm_mon + 1, tmTime->tm_mday, tmTime->tm_hour,
		tmTime->tm_min, tmTime->tm_sec);

	return (sTime);
}

/**
 * 获取当前日期。
 * 日期格式为YYYYMMDD，sDate长度至少为9。
 *
 * @param sDate: 输出缓存区
 * @return sDate
 *
 */
char *
dtmGetCurDate(char sDate[9])
{
	return (dtmTt2Date(time(NULL), sDate));
}

/**
 * 获取当前日期时间。
 * 日期时间格式为YYYYMMDDHH24MISS，sTime长度至少为15。
 *
 * @param sTime: 输出缓存区
 *
 * @return sTime
 *
 */
char *
dtmGetCurTime(char sTime[15])
{
	return (dtmTt2Time(time(NULL), sTime));
}

/**
 * 计算日期+天数。
 * 日期格式为YYYYMMDD，天数可为负数。
 *
 * @param sDate: 输入日期，计算结果也会保存在此
 * @param nDayCnt: 天数
 *
 * @return sDate
 *
 */
char *
dtmAddDays(char sDate[9], int nDayCnt)
{
	return (dtmTt2Date(dtmDate2Tt(sDate) + DTM_DAYSECONDS * nDayCnt, sDate));
}

/**
 * 计算日期时间+秒数。
 * 日期时间格式为YYYYMMDD，秒数可为负数。
 *
 * @param sTime: 输入日期时间，计算结果也会保存在此
 * @param nSecondCnt: 秒数
 *
 * @return sTime
 *
 */
char *
dtmAddSeconds(char sTime[15], int nSecondCnt)
{
	return (dtmTt2Time(dtmTime2Tt(sTime) + nSecondCnt, sTime));
}

/**
 * 计算两日期间隔天数。
 * 日期格式为YYYYMMDD；
 * 若sDate1 > sDate2则返回值大于0，若sDate1 < sDate2则返回值小于0。
 *
 * @param sDate1: 输入日期1
 * @param sDate2: 输入日期2
 *
 * @return 间隔天数
 *
 */
int
dtmDaysBetween(char sDate1[9], char sDate2[9])
{
	return ((dtmDate2Tt(sDate1) - dtmDate2Tt(sDate2)) / DTM_DAYSECONDS);
}

/**
 * 计算两日期时间间隔秒数。
 * 日期时间格式为YYYYMMDDHH24MISS；
 * 若sTime1 > sTime2则返回值大于0，若sTime1 < sTime2则返回值小于0。
 *
 * @param sTime1: 输入日期时间1
 * @param sTime2: 输入日期时间2
 *
 * @return 间隔秒数
 *
 */
int
dtmSecondsBetween(char sTime1[15], char sTime2[15])
{
	return (dtmTime2Tt(sTime1) - dtmTime2Tt(sTime2));
}

/**
 * 获得格式化日期时间。
 *
 * @param sBuf: 输出缓存区
 * @param nBufLen: 输出缓存区长度
 * @param sFmt: 时间格式
 * @param ttTime: time_t格式时间
 *
 * @return sBuf
 *
 */
char *
dtmFmtTime(char *sBuf, size_t nBufLen, char *sFmt, time_t ttTime)
{
	memset(sBuf, 0, nBufLen);

	tzset();
	if (sFmt == NULL)
		strftime(sBuf, nBufLen, "%Y-%m-%d %A %H:%M:%S", localtime(&ttTime));
	else
		strftime(sBuf, nBufLen, sFmt, localtime(&ttTime));

	return (sBuf);
}

/**
 * 获得当前格式化日期时间。
 *
 * @param sBuf: 输出缓存区
 * @param nBufLen: 输出缓存区长度
 * @param sFmt: 时间格式
 *
 * @return sBuf
 *
 */
char *
dtmGetCurFmtTime(char *sBuf, size_t nBufLen, char *sFmt)
{
	return (dtmFmtTime(sBuf, nBufLen, sFmt, time(NULL)));
}

/**
 * 获得标准格式日期时间。
 *
 * @param sBuf: 输出缓存区(sizeof(sBuf) > 37+1)
 *
 * @return sBuf
 *
 */
char *
dtmStdTime(char *sBuf)
{
	time_t		ttTime = time(NULL);
	struct tm	*tmTime;

	tzset();
	tmTime = localtime(&ttTime);

	sprintf(sBuf, "%s, %02d %s %d %02d:%02d:%02d %c%02ld%02ld (%s)",
	        f_sDays[tmTime->tm_wday], tmTime->tm_mday,
	        f_sMonths[tmTime->tm_mon], tmTime->tm_year + 1900, tmTime->tm_hour,
	        tmTime->tm_min, tmTime->tm_sec, timezone > 0 ? '-' : '+',
	        labs(timezone) / 60 / 60, labs(timezone) / 60 % 60, tzname[0]);

	return sBuf;
}

/**
 * 计时开始。
 *
 * @param ptvBegin: 开始的时间
 *
 * @return 0-成功，<0失败
 *
 */
int
dtmBeginTimer(struct timeval *ptvBegin)
{
	if (ptvBegin == NULL) {
		return -1;
	}

	if (gettimeofday(ptvBegin, NULL) < 0) {
		return -2;
	}

	return 0;
}

/**
 * 计时停止。
 *
 * @param ptvBegin: 间隔的时间
 *
 * @return 0-成功，<0失败
 *
 */
int
dtmEndTimer(struct timeval *ptvBegin)
{
	timeval_t	tvEnd;

	if (ptvBegin == NULL) {
		return -1;
	}

	if (gettimeofday(&tvEnd, NULL) < 0) {
		return -2;
	}

	dtmTimeValSub(ptvBegin, &tvEnd);

	return 0;
}

/**
 * 计时开始。
 *
 * @param ptTimer: Timer控制结构
 *
 * @return 0-成功，<0失败
 *
 */
int
dtmBeginTimer2(T_TIMER *ptTimer)
{
	if (ptTimer == NULL) {
		return -1;
	}

	if (gettimeofday(&ptTimer->tvSave, NULL) < 0) {
		return -2;
	}

	return 0;
}

/**
 * 计时停止。
 *
 * @param ptTimer: Timer控制结构
 *
 * @return 0-成功，<0失败
 *
 */
int
dtmEndTimer2(T_TIMER *ptTimer)
{
	timeval_t	tvEnd;

	if (ptTimer == NULL) {
		return -1;
	}

	if (gettimeofday(&tvEnd, NULL) < 0) {
		return -2;
	}

	ptTimer->iCnt++;
	dtmTimeValSub(&ptTimer->tvSave, &tvEnd);
	dtmTimeValAdd(&ptTimer->tvTotal, &ptTimer->tvSave);

	return 0;
}

/* 用于测试 */
/*
#include	<stdlib.h>
#include	<unistd.h>

int
main(void)
{
	char			sDate1[8+1], sDate2[8+1];
	char			sTime1[14+1], sTime2[14+1];
	char			sBuf[40] = {0, };
	timeval_t	tvTmp;

	printf("tzname[0]=[%s]\ttzname[1]=[%s]\ntimezone=[%ld]\tdaylight=[%d]\n",
		tzname[0], tzname[1], timezone, daylight);
	printf("sDate1=[%s]\n", dtmGetCurDate(sDate1));
	printf("sTime1=[%s]\n", dtmGetCurTime(sTime1));
	strcpy(sDate2, sDate1);
	strcpy(sTime2, sTime1);
	printf("sDate2=sDate1+10=[%s]\n", dtmAddDays(sDate2, 10));
	printf("sTime2=sTime1-1000=[%s]\n", dtmAddSeconds(sTime2, -1000));
	printf("sDate2-sDate1=[%d]\n", dtmDaysBetween(sDate1, sDate2));
	printf("sTime2-sTime1=[%d]\n", dtmSecondsBetween(sTime1, sTime2));
	printf("sBuf=[%s]\n", dtmGetCurFmtTime(sBuf, 40, NULL));
	dtmBeginTimer(&tvTmp);
	sleep(1);
	dtmEndTimer(&tvTmp);
	printf("nSec=[%ld]\tnUSec=[%ld]\n", tvTmp.tv_sec, tvTmp.tv_usec);

	exit(EXIT_SUCCESS);
}
*/

/*-------------------------  Local functions ----------------------------*/
/**
 * dtmTimeValAdd
 *
 * @param ptvA: 加数A
 * @param ptvB: 加数B
 *
 * @return ptvA: 和
 *
 */
static timeval_t *
dtmTimeValAdd(timeval_t *ptvA, timeval_t *ptvB)
{
	ptvA->tv_sec  += ptvB->tv_sec;
	ptvA->tv_usec += ptvB->tv_usec;

	while (ptvA->tv_usec >= 1000000) {
		ptvA->tv_sec  += 1;
		ptvA->tv_usec -= 1000000;
	}

	return ptvA;
}

/**
 * dtmTimeValSub
 *
 * @param ptvA: 减数A(小数)
 * @param ptvB: 减数B(大数)
 *
 * @return ptvA: 差的绝对值
 *
 */
static timeval_t *
dtmTimeValSub(timeval_t *ptvA, timeval_t *ptvB)
{
	timeval_t	*ptvTmp;
	int			iUSec;

	if ((ptvA->tv_sec >  ptvB->tv_sec) ||
		(ptvA->tv_sec == ptvB->tv_sec  && ptvA->tv_usec > ptvB->tv_usec)) {
		ptvTmp = ptvA;
		ptvA = ptvB;
		ptvB = ptvTmp;
	}

	ptvA->tv_sec = ptvB->tv_sec - ptvA->tv_sec;
	iUSec = (int) ptvB->tv_usec - (int) ptvA->tv_usec;
	while (iUSec < 0) {
		iUSec += 1000000;
		ptvA->tv_sec -= 1;
	}
	ptvA->tv_usec = iUSec;

	return ptvA;
}
/*-----------------------------  End ------------------------------------*/
