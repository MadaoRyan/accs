/*
 *
 *
 * Algorithm Functions
 *
 *
 * FileName: util_alg.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_ALG_H_20100121112643_
#define _UTIL_ALG_H_20100121112643_
/*--------------------------- Include files -----------------------------*/
#include	<stddef.h>

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/
typedef int(*F_COMPAR)(const void *, const void *);

typedef struct {
	char	*pKey;
	void	*pData;
} T_ITEM;

typedef struct T_TABLE {
	unsigned int	iUsed;
	T_ITEM			tItem;
} T_TABLE;

typedef struct T_HTABLE {
	T_TABLE			*ptTable;
	unsigned int	iSize;
	unsigned int	iFilled;
} T_HTABLE;

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: algSort
 *
 * @desc: ����
 *
 * @param pBase: ������ʼ��ַ
 * @param iNum: �������
 * @param iSize: ������ÿ����λ��С
 * @param fCompar: �ȽϺ���
 *
 */
void algSort(void *pBase, size_t iNum, size_t iSize, F_COMPAR fCompar);

/**
 * @function: algBSearch
 *
 * @desc: ���������ַ���
 *
 * @param pKey: ����Ŀ��
 * @param pBase: ������ʼ��ַ
 * @param iNum: �������
 * @param iSize: ������ÿ����λ��С
 * @param fCompar: �ȽϺ���
 *
 * @return void *: ret!=NULL �ɹ�; ret=NULL ʧ��
 *
 */
void *algBSearch(const void *pKey, const void *pBase, size_t iNum, size_t iSize,
                 F_COMPAR fCompar);

/**
 * @function: algLSearch
 *
 * @desc: ���������ԣ�
 *
 * @param pBase: ������ʼ��ַ
 * @param iNum: �������
 * @param iSize: ������ÿ����λ��С
 * @param fCompar: �ȽϺ���
 *
 * @return void *: ret!=NULL �ɹ�; ret=NULL ʧ��
 *
 */
void *algLSearch(const void *pKey, const void *pBase, size_t iNum, size_t iSize,
                 F_COMPAR fCompar);

/**
 * @function: algHCreate
 *
 * @desc: ������ϣ��
 *
 * @param iNum: ��ϣ�������ɵ�Ԫ�ظ���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int algHCreate(size_t iNum);

/**
 * @function: algHSearch
 *
 * @desc: �ڹ�ϣ���в���Ŀ��
 *
 * @param tItem: ����Ŀ��
 *
 * @return T_ITEM *: ret!=NULL �ɹ�; ret=NULL ʧ��
 *
 */
T_ITEM *algHSearch(T_ITEM tItem);

/**
 * @function: algHEnter
 *
 * @desc: ���ϣ���������
 *
 * @param tItem: �������ϣ���Ԫ��
 *
 * @return int: ret=1 �ؼ�; ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int algHEnter(T_ITEM tItem);

/**
 * @function: algHDestroy
 *
 * @desc: �ͷŹ�ϣ��
 *
 */
void algHDestroy(void);

/**
 * @function: algHCreateR
 *
 * @desc: ������ϣ��
 *
 * @param iNum: ��ϣ�������ɵ�Ԫ�ظ���
 * @param ptHtab: ��ϣ����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int algHCreateR(size_t iNum, T_HTABLE *ptHtab);

/**
 * @function: algHSearchR
 *
 * @desc: �ڹ�ϣ���в���Ŀ��
 *
 * @param tItem: ����Ŀ��
 * @param ptHtab: ��ϣ����
 *
 * @return T_ITEM *: ret!=NULL �ɹ�; ret=NULL ʧ��
 *
 */
T_ITEM *algHSearchR(T_ITEM tItem, T_HTABLE *ptHtab);

/**
 * @function: algHEnterR
 *
 * @desc: ���ϣ���������
 *
 * @param tItem: �������ϣ���Ԫ��
 * @param ptHtab: ��ϣ����
 *
 * @return int: ret=1 �ؼ�; ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int algHEnterR(T_ITEM tItem, T_HTABLE *ptHtab);

/**
 * @function: algHDestroyR
 *
 * @desc: �ͷŹ�ϣ��
 *
 * @param ptHtab: ��ϣ����
 *
 */
void algHDestroyR(T_HTABLE *ptHtab);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_ALG_H_20100121112643_*/
/*-----------------------------  End ------------------------------------*/
