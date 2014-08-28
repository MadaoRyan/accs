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
	struct timeval	tvSave;		/* ÿ��ʱ�� */
	int				iCnt;		/* ��ʱ���� */
	struct timeval	tvTotal;	/* �ۼ�ʱ�� */
} T_TIMER;

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: dtmDate2Tt
 *
 * @desc: YYYYMMDD��ʽ����ת��Ϊtime_t��ʽʱ��
 *
 * @param sDate: ���������
 *
 * @return time_t: time_t��ʽʱ��
 *
 */
time_t dtmDate2Tt(char sDate[9]);

/**
 * @function: dtmTime2Tt
 *
 * @desc: YYYYMMDDHH24MISS��ʽ����ʱ��ת��Ϊtime_t��ʽʱ��
 *
 * @param sTime: �����ʱ��
 *
 * @return time_t: time_t��ʽʱ��
 *
 */
time_t dtmTime2Tt(char sTime[15]);

/**
 * @function: dtmTt2Date
 *
 * @desc: time_t��ʽʱ��ת��ΪYYYYMMDD��ʽ����
 *
 * @param ttTime: time_t��ʽʱ��
 * @param sDate: ���������
 *
 * @return char *: ���������
 *
 */
char *dtmTt2Date(time_t ttTime, char sDate[9]);

/**
 * @function: dtmTt2Time
 *
 * @desc: time_t��ʽʱ��ת��ΪYYYYMMDDHH24MISS��ʽ����ʱ��
 *
 * @param ttTime: time_t��ʽʱ��
 * @param sTime: ���������
 *
 * @return char *: ���������
 *
 */
char *dtmTt2Time(time_t ttTime, char sTime[15]);

/**
 * @function: dtmGetCurDate
 *
 * @desc: ��ȡ��ǰ����
 *
 * @param sDate: ���������
 *
 * @return char *: ���������
 *
 */
char *dtmGetCurDate(char sDate[9]);

/**
 * @function: dtmGetCurTime
 *
 * @desc: ��ȡ��ǰ����ʱ��
 *
 * @param sTime: ���������
 *
 * @return char *: ���������
 *
 */
char *dtmGetCurTime(char sTime[15]);

/**
 * @function: dtmAddDays
 *
 * @desc: ��������+����(�ɸ�)
 *
 * @param sDate: �������ڣ�������Ҳ�ᱣ���ڴ�
 * @param nDayCnt: ����
 *
 * @return char *: �������
 *
 */
char *dtmAddDays(char sDate[9], int nDayCnt);

/**
 * @function: dtmAddSeconds
 *
 * @desc: ��������ʱ��+����(�ɸ�)
 *
 * @param sTime: ��������ʱ�䣬������Ҳ�ᱣ���ڴ�
 * @param nSecondCnt: ����
 *
 * @return char *: �������ʱ��
 *
 */
char *dtmAddSeconds(char sTime[15], int nSecondCnt);

/**
 * @function: dtmDaysBetween
 *
 * @desc: ���������ڼ������(sDate1<sDate2ʱ���Ϊ����)
 *
 * @param sDate1: ��������1
 * @param sDate2: ��������2
 *
 * @return int: �������
 *
 */
int dtmDaysBetween(char sDate1[9], char sDate2[9]);

/**
 * @function: dtmSecondsBetween
 *
 * @desc: ����������ʱ��������(sTime1<sTime2)
 *
 * @param sTime1: ��������ʱ��1
 * @param sTime2: ��������ʱ��2
 *
 * @return int: �������
 *
 */
int dtmSecondsBetween(char sTime1[15], char sTime2[15]);

/**
 * @function: dtmFmtTime
 *
 * @desc: ��ø�ʽ������ʱ��
 *
 * @param sBuf: ���������
 * @param nBufLen: �������������
 * @param sFmt: ʱ���ʽ
 * @param ttTime: time_t��ʽʱ��
 *
 * @return char *: ���������
 *
 */
char *dtmFmtTime(char *sBuf, size_t nBufLen, char *sFmt, time_t ttTime);

/**
 * @function: dtmGetCurFmtTime
 *
 * @desc: ��õ�ǰ��ʽ������ʱ��
 *
 * @param sBuf: ���������
 * @param nBufLen: �������������
 * @param sFmt: ʱ���ʽ
 *
 * @return char *: ���������
 *
 */
char *dtmGetCurFmtTime(char *sBuf, size_t nBufLen, char *sFmt);

/**
 * @function: dtmStdTime
 *
 * @desc: ��ñ�׼��ʽ����ʱ��
 *
 * @param sBuf: ���������(����Ӧ���ڵ���38)
 *
 * @return char *: ���������
 *
 */
char *dtmStdTime(char *sBuf);

/**
 * @function: dtmBeginTimer
 *
 * @desc: ��ʱ��ʼ
 *
 * @param ptvBegin: ��ʼ��ʱ��
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int dtmBeginTimer(struct timeval *ptvBegin);

/**
 * @function: dtmEndTimer
 *
 * @desc: ��ʱֹͣ
 *
 * @param ptvBegin: �����ʱ��
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int dtmEndTimer(struct timeval *ptvBegin);

/**
 * @function: dtmBeginTimer2
 *
 * @desc: ��ʱ��ʼ
 *
 * @param ptTimer: Timer���ƽṹ
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int dtmBeginTimer2(T_TIMER *ptTimer);

/**
 * @function: dtmEndTimer2
 *
 * @desc: ��ʱֹͣ
 *
 * @param ptTimer: Timer���ƽṹ
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int dtmEndTimer2(T_TIMER *ptTimer);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_DTM_H_20100105103319_*/
/*-----------------------------  End ------------------------------------*/
