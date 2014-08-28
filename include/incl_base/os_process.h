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
 * @desc: ��������
 *
 * @param psLine: ����������ò��������޸�
 * @param psArgv[]: ����ָ������
 *
 * @return int: >=0:�������� <0:����
 *
 * @comment:
 * ��psLine�Կո�ָ�ת��Ϊ\0�ָ������ü�¼��psArgv
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
 * @desc: ����һ������
 *
 * @param psProcName: ������
 * @param psBin: ��ִ�г���
 * @param psArgs: ����
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ����һ������
 *
 * @param psProcName: ������
 * @param psBin: ��ִ�г���
 * @param psArgs: ����
 * @param iOption: ѡ��
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ��������
 *
 * @param iOption: ѡ��
 *
 * @return int: 0:�ӽ��� >0:������ <0:����
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
 * @desc: ����̷��ź�
 *
 * @param iPid: ����Id
 * @param iSig: �ź�
 *
 * @return int:  0:�ɹ� <0:����
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
 * @desc: ������Daemon��ʽ����
 *
 * @param psRoot: ����·��
 * @param iMask: ����
 * @param iOptions: ѡ��
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ȷ��һ�������Ƿ����
 *
 * @param iPid: ���̺�
 *
 * @return int: >=0: ����, <0:����
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
