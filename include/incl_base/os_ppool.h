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
#define ERR_OS_PPOOL_OK 0                                           /*�ɹ�*/
            
#define ERR_OS_PPOOL_FAIL_BASE - 500                                /*ʧ��*/
#define ERR_OS_PPOOL_PARAM     (ERR_OS_PPOOL_FAIL_BASE - 1)         /*��������*/
#define ERR_OS_PPOOL_NOSPACE   (ERR_OS_PPOOL_FAIL_BASE - 2)         /*�ռ䲻��*/
#define ERR_OS_PPOOL_NOTFOUND  (ERR_OS_PPOOL_FAIL_BASE - 3)         /*δ�ҵ�*/
#define ERR_OS_PPOOL_EXISTS    (ERR_OS_PPOOL_FAIL_BASE - 4)         /*�Ѵ���*/
#define ERR_OS_PPOOL_INIT      (ERR_OS_PPOOL_FAIL_BASE - 5)         /*��ʼ��*/

#define ERR_OS_PPOOL_OTHER     (ERR_OS_PPOOL_FAIL_BASE - 99)         /*��������*/

/*options*/
#define OPT_OS_PPOOL_BLOCK 0x00000001                                /*����ģʽ*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: pplInit
 *
 * @desc: ��ʼ�����̳�
 *
 * @param iMax: ��������
 * @param iOptions: ѡ��
 *
 * @return int: ret>=0 �ɹ�; ret<0 ʧ��
 *
 */
int pplInit(int iMax, int iOptions);

/**
 * @function: pplFork
 *
 * @desc: �������
 *
 * @param iOptions: ѡ��
 *
 * @return int: ret>0 ���̺�,������; ret==0 �ӽ���; ret<0 ʧ��
 *
 */
int pplFork(int iOptions);

/**
 * @function: pplKill
 *
 * @desc: ����̷����ź�
 *
 * @param iPid: ���̺ţ�0-���н���
 * @param iSignal: �ź�
 *
 * @return int: ret>=0 ���ͽ�����; ret<0 ʧ��
 *
 */
int pplKill(int iPid, int iSignal);

/**
 * @function: pplCountAlive
 *
 * @desc: ������̳��л���̸���
 *
 * @return long: ret>=0 ������; ret<0 ����
 *
 */
int pplCountAlive(void);

/**
 * @function: pplUninit
 *
 * @desc: �ͷŽ��̳���Դ
 *
 * @param iOptions: ѡ��
 *
 * @return long: ret>=0 �رս�����; ret<0 ����
 *
 */
int pplUninit(int iOptions);

/**
 *  ��ȡ��ǰ�������
 *
 *  @return �������
 */
int pplGetIndex();

#ifdef __cplusplus
}
#endif

#endif /*_OS_PPOOL_H_20111221162117_*/
/*-----------------------------  End ------------------------------------*/
