/*
 *
 *
 * container handle functions
 *
 *
 * FileName: util_ctn.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _MEMCTL_H_20101012103404_
#define _MEMCTL_H_20101012103404_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/
typedef struct T_CTN T_CTN;
typedef int (*F_CTN_COMPAR)(const void *, const void *);

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: ctnNew
 *
 * @desc: ��������
 *
 * @param iSize: ��λ��С
 * @param fCompar: �ȽϺ���
 *
 * @return T_CTN *: �������
 *
 */
T_CTN *ctnNew(int iSize, F_CTN_COMPAR fCompar);

/**
 * @function: ctnReAlloc
 *
 * @desc: Ϊ�����������ռ�
 *
 * @param ptCtn: �������
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int ctnReAlloc(T_CTN *ptCtn);

/**
 * @function: ctnFree
 *
 * @desc: �ͷ�����
 *
 * @param ptCtn: �������
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int ctnFree(T_CTN *ptCtn);

/**
 * @function: ctnSetSorted
 *
 * @desc: ������������״̬
 *
 * @param ptCtn: �������
 * @param iSorted: ����״̬
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int ctnSetSorted(T_CTN *ptCtn, int iSorted);

/**
 * @function: ctnSetCompar
 *
 * @desc: ���ñȽϺ���
 *
 * @param ptCtn: �������
 * @param fCompar: �ȽϺ���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int ctnSetCompar(T_CTN *ptCtn, F_CTN_COMPAR fCompar);

/**
 * @function: ctnAdd
 *
 * @desc: ������������Ԫ��
 *
 * @param ptCtn: �������
 * @param pData: Ԫ��
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int ctnAdd(T_CTN *ptCtn, void *pData);

/**
 * @function: ctnGet
 *
 * @desc: ȡ��Ԫ��
 *
 * @param ptCtn: �������
 * @param iIdx: Ԫ�����
 *
 * @return void *: ret!=NULL Ԫ�ص�ַ; ret=NULL ʧ��
 *
 */
void *ctnGet(T_CTN *ptCtn, int iIdx);

/**
 * @function: ctnIdx
 *
 * @desc: ȡ��Ԫ�����
 *
 * @param ptCtn: �������
 * @param pData: Ԫ��
 *
 * @return int: ret>=0 Ԫ�����; ret<0 ʧ��
 *
 */
int ctnIdx(T_CTN *ptCtn, void *pData);

/**
 * @function: ctnCount
 *
 * @desc: ȡ��������ǰԪ�ظ���
 *
 * @param ptCtn: �������
 *
 * @return int: ret>=0 Ԫ�ظ���; ret<0 ʧ��
 *
 */
int ctnCount(T_CTN *ptCtn);

/**
 * @function: ctnSort
 *
 * @desc: ��������Ԫ�ؽ�������
 *
 * @param ptCtn: �������
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int ctnSort(T_CTN *ptCtn);

/**
 * @function: ctnSearch
 *
 * @desc: ������������Ԫ��
 *
 * @param ptCtn: �������
 * @param pKey: Ŀ��Ԫ��
 *
 * @return void *: ret!=NULL �ɹ�; ret=NULL ʧ��
 *
 */
void *ctnSearch(T_CTN *ptCtn, const void *pKey);

/**
 * @function: ctnSearchL
 *
 * @desc: ������������Ԫ��(����)
 *
 * @param ptCtn: �������
 * @param pKey: Ŀ��Ԫ��
 * @param pCur: ��ǰ������λ��
 *
 * @return void *: ret!=NULL �ɹ�; ret=NULL ʧ��
 *
 */
void *ctnSearchL(T_CTN *ptCtn, const void *pKey, const void *pCur);

/**
 * @function: ctnSearchM
 *
 * @desc: ������������Ԫ��(����)
 *
 * @param ptCtn: �������
 * @param pKey: Ŀ��Ԫ��
 * @param pCur: ��ǰ������λ��
 *
 * @return void *: ret!=NULL �ɹ�; ret=NULL ʧ��
 *
 */
void *ctnSearchM(T_CTN *ptCtn, const void *pKey, const void *pCur);

/**
 * @function: ctnSearchIdx
 *
 * @desc:
 *
 * @param ptCtn: �������
 * @param pKey:
 *
 * @return int: ret=0 Ŀ�����; ret<0 ʧ��
 *
 */
int ctnSearchIdx(T_CTN *ptCtn, const void *pKey);

/**
 * @function: ctnSearchIdxL
 *
 * @desc: ������������Ԫ��(����)
 *
 * @param ptCtn: �������
 * @param pKey: Ŀ��Ԫ��
 * @param iCur: ��ǰ������λ��
 *
 * @return int: ret=0 Ŀ�����; ret<0 ʧ��
 *
 */
int ctnSearchIdxL(T_CTN *ptCtn, const void *pKey, const int iCur);

/**
 * @function: ctnSearchIdxM
 *
 * @desc: ������������Ԫ��(����)
 *
 * @param ptCtn: �������
 * @param pKey: Ŀ��Ԫ��
 * @param iCur: ��ǰ������λ��
 *
 * @return int: ret=0 Ŀ�����; ret<0 ʧ��
 *
 */
int ctnSearchIdxM(T_CTN *ptCtn, const void *pKey, const int iCur);

/**
 * @function: ctnComparInt
 *
 * @desc: �Ƚ�int�ͱ���
 *
 * @param p1: ����1
 * @param p2: ����2
 *
 * @return int: ret>0 p1>p2; ret=0 p1=p2; ret<0 p1<p2
 *
 */
int ctnComparInt(const void *p1, const void *p2);

#ifdef __cplusplus
}
#endif

#endif /*_MEMCTL_H_20101012103404_*/
/*-----------------------------  End ------------------------------------*/
