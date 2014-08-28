/*
 *
 *
 * OS Mmap Module
 *
 *
 * FileName: os_mmap.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_MMAP_H_20111130102322_
#define _OS_MMAP_H_20111130102322_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/
#define	OS_MMAP_MAX		8

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: mapRead
 *
 * @desc: ӳ���ļ��������ڴ�
 *
 * @param sFile: �ļ�����
 * @param piLen: �ļ�����
 *
 * @return int: ret!=NULL �ļ�����ָ��; ret=NULL ʧ��
 *
 */
char *mapRead(char *sFile, int *piLen);

/**
 * @function: mapFree
 *
 * @desc: �ͷ��ļ�����ָ��
 *
 * @param pMap: �ļ�����ָ��
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int mapFree(char *pMap);

/**
 * @function: mapFree
 *
 * @desc: �ͷ����д򿪵��ļ�����ָ��
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int mapFreeAll(void);

#ifdef __cplusplus
}
#endif

#endif /*_OS_MMAP_H_20111130102322_*/
/*-----------------------------  End ------------------------------------*/
