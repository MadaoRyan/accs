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
	FILE	*fpFile;				/* CSV�ļ�ָ�� */
	char	sBuf[CSV_MAX_LINE];		/* ���л��� */
	int		nCnt;					/* �ֶθ��� */
	char	*psCol[CSV_MAX_COL];	/* �ֶ����飬��:psCol[0]Ϊ��һ���ֶ� */
} T_CSV;

typedef struct {
	int		iCnt;					/* �ֶθ��� */
	char	*psCol[CSV_MAX_COL];	/* �ֶ����飬��:psCol[0]Ϊ��һ���ֶ� */
} T_SPLIT_STR;

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: csvOpen
 *
 * @desc: ��CSV�ļ�
 *
 * @param sFileName: CSV�ļ���
 *
 * @return T_CSV *: ret!=NULL CSV���; ret=NULL ʧ��
 *
 */
T_CSV *csvOpen(char *sFileName);

/**
 * @function: csvReadRow
 *
 * @desc: ��CSV�ļ��ж�ȡһ��
 *
 * @param ptCsv: CSV���
 * @param cSep: �ֶηָ���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvReadRow(T_CSV *ptCsv, char cSep);

/**
 * @function: csvReadRow2
 *
 * @desc: ��CSV�ļ��ж�ȡһ��
 *
 * @param ptCsv: CSV���
 * @param sSep: �ֶηָ���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvReadRow2(T_CSV *ptCsv, const char *sSep);

/**
 * @function: csvReadRowSkip
 *
 * @desc: ��CSV�ļ��ж�ȡһ��
 *
 * @param ptCsv: CSV���
 * @param cSep: �ֶηָ���
 * @param cSkip: ע���ַ�
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvReadRowSkip(T_CSV *ptCsv, char cSep, char cSkip);

/**
 * @function: csvReadRowN
 *
 * @desc: ��CSV�ļ��ж�ȡһ��
 *
 * @param ptCsv: CSV���
 * @param cSep: �ֶηָ���
 * @param iCnt: �ָ�����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvReadRowN(T_CSV *ptCsv, char cSep, int iCnt);

/**
 * @function: csvReadRowN2
 *
 * @desc: ��CSV�ļ��ж�ȡһ��
 *
 * @param ptCsv: CSV���
 * @param sSep: �ֶηָ���
 * @param iCnt: �ָ�����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvReadRowN2(T_CSV *ptCsv, const char *sSep, int iCnt);

/**
 * @function: csvReadRowSkipN
 *
 * @desc: ��CSV�ļ��ж�ȡһ��
 *
 * @param ptCsv: CSV���
 * @param cSep: �ֶηָ���
 * @param cSkip: ע���ַ�
 * @param iCnt: �ָ�����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvReadRowSkipN(T_CSV *ptCsv, char cSep, char cSkip, int iCnt);

/**
 * @function: csvReadRowExt
 *
 * @desc: ��CSV�ļ��ж�ȡһ��
 *
 * @param ptCsv: CSV���
 * @param cSep: �ֶηָ���
 * @param cQuote: �����ַ�
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvReadRowExt(T_CSV *ptCsv, char cSep, char cQuote);

/**
 * @function: csvReadRowExtR
 *
 * @desc: ��CSV�ļ��ж�ȡһ��
 *
 * @param fpFile: �ļ����
 * @param cSep: �ֶηָ���
 * @param cQuote: �����ַ�
 * @param piCnt: ����
 * @param psCol: ��ֵ
 * @param sBuf: �л�����
 * @param iSize: �л�������С
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvReadRowExtR(FILE *fpFile, char cSep, char cQuote,
                   int *piCnt, char *psCol[], char *sBuf, int iSize);

/**
 * @function: csvClose
 *
 * @desc: �ر�CSV�ļ�
 *
 * @param ptCsv: CSV���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvClose(T_CSV *ptCsv);

/**
 * @function: csvSplitStr
 *
 * @desc: �ָ��ַ���
 *
 * @param sStr: ���ָ���ַ���(���޸ĸ��ַ�������)
 * @param cSep: �ֶηָ���
 * @param ptSplitStr: �ָ��Ľṹ��
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvSplitStr(char *sStr, char cSep, T_SPLIT_STR *ptSplitStr);

/**
 * @function: csvSplitStr2
 *
 * @desc: �ָ��ַ���
 *
 * @param sStr: ���ָ���ַ���(���޸ĸ��ַ�������)
 * @param sSep: �ֶηָ���
 * @param ptSplitStr: �ָ��Ľṹ��
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvSplitStr2(char *sStr, const char *sSep, T_SPLIT_STR *ptSplitStr);

/**
 * @function: csvSplitStrN
 *
 * @desc: �ָ��ַ���
 *
 * @param sStr: ���ָ���ַ���(���޸ĸ��ַ�������)
 * @param cSep: �ֶηָ���
 * @param iCnt: �ָ�����
 * @param ptSplitStr: �ָ��Ľṹ��
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvSplitStrN(char *sStr, char cSep, int iCnt, T_SPLIT_STR *ptSplitStr);

/**
 * @function: csvSplitStrN2
 *
 * @desc: �ָ��ַ���
 *
 * @param sStr: ���ָ���ַ���(���޸ĸ��ַ�������)
 * @param sSep: �ֶηָ���
 * @param iCnt: �ָ�����
 * @param ptSplitStr: �ָ��Ľṹ��
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvSplitStrN2(char *sStr, const char *sSep, int iCnt,
                  T_SPLIT_STR *ptSplitStr);

/**
 * @function: csvWrite
 *
 * @desc: дCSV�ļ�
 *
 * @param fpFile: �ļ����
 * @param cSep: �ֶηָ���
 * @param iCnt: �ֶ�����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvWrite(FILE *fpFile, char cSep, int iCnt, ...);

/**
 * @function: csvWriteV
 *
 * @desc: дCSV�ļ�
 *
 * @param fpFile: �ļ����
 * @param cSep: �ֶηָ���
 * @param iCnt: �ֶ�����
 * @param psCol: �ֶ�����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvWriteV(FILE *fpFile, char cSep, int iCnt, char *psCol[]);

/**
 * @function: csvWriteExt
 *
 * @desc: дCSV�ļ�
 *
 * @param fpFile: Ŀ���ļ�
 * @param cSep: �ֶηָ���
 * @param cQuote: �����ַ�
 * @param iCnt: �ֶ�����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvWriteExt(FILE *fpFile, char cSep, char cQuote, int iCnt, ...);

/**
 * @function: csvWriteExtV
 *
 * @desc: дCSV�ļ�
 *
 * @param fpFile: Ŀ���ļ�
 * @param cSep: �ֶηָ���
 * @param cQuote: �����ַ�
 * @param iCnt: �ֶ�����
 * @param psCol: �ֶ�����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int csvWriteExtV(FILE *fpFile, char cSep, char cQuote, int iCnt, char *psCol[]);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_CSV_H_20100107154922_*/
/*-----------------------------  End ------------------------------------*/
