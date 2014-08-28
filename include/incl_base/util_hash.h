/*
 *
 *
 * Hash Tables
 *
 *
 * FileName: util_hash.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_HASH_H_20101104140701_
#define _UTIL_HASH_H_20101104140701_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/
typedef struct T_HASH_TABLE T_HASH_TABLE;

typedef struct T_HASH_FUNCS {
	void	*(*fNew   )(struct T_HASH_FUNCS *, const char *, void *);
	void	 (*fDelete)(struct T_HASH_FUNCS *, const char *, void *);
	void	*pData;
} T_HASH_FUNCS;

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: hashNew
 *
 * @desc: ������ϣ��
 *
 * @param iSize: ��ʼ��ϣ���С
 *
 * @return T_HASH_TABLE *: ret!=NULL ��ϣ����; ret=NULL ʧ��
 *
 */
T_HASH_TABLE *hashNew(int iSize);

/**
 * @function: hashAdd
 *
 * @desc: ���ϣ���в���Ԫ��, ���Ѵ�����ִ��ʧ��
 *
 * @param ptHashTable: ��ϣ����
 * @param sKey: Ԫ�ؼ�
 * @param pValue: Ԫ��ֵ
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int hashAdd(T_HASH_TABLE *ptHashTable, const char *sKey, void *pValue);

/**
 * @function: hashSet
 *
 * @desc: ����Ԫ�ص�ֵ, ��������������
 *
 * @param ptHashTable: ��ϣ����
 * @param sKey: Ԫ�ؼ�
 * @param pValue: Ԫ��ֵ
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int hashSet(T_HASH_TABLE *ptHashTable, const char *sKey, void *pValue);

/**
 * @function: hashGet
 *
 * @desc: ȡ��Ԫ�ص�ֵ
 *
 * @param ptHashTable: ��ϣ����
 * @param sKey: Ԫ�ؼ�
 *
 * @return void *: ret!=NULL Ԫ��ֵ; ret=NULL ʧ��
 *
 */
void *hashGet(T_HASH_TABLE *ptHashTable, const char *sKey);

/**
 * @function: hashDel
 *
 * @desc: �ӹ�ϣ����ɾ��Ԫ��
 *
 * @param ptHashTable: ��ϣ����
 * @param sKey: Ԫ�ؼ�
 *
 * @return void *: ret!=NULL ��ɾ����Ԫ��ֵ; ret=NULL ʧ��
 *
 */
void *hashDel(T_HASH_TABLE *ptHashTable, const char *sKey);

/**
 * @function: hashFree
 *
 * @desc: �ͷŹ�ϣ��
 *
 * @param ptHashTable: ��ϣ����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int hashFree(T_HASH_TABLE *ptHashTable);

/**
 * @function: hashCount
 *
 * @desc: ȡ�ù�ϣ����Ԫ�ظ���
 *
 * @param ptHashTable: ��ϣ����
 *
 * @return int: Ԫ�ظ���
 *
 */
int hashCount(T_HASH_TABLE *ptHashTable);

/**
 * @function: hashIter
 *
 * @desc: ������ϣ��
 *
 * @param ptHashTable: ��ϣ����
 *
 * @return const char *: ret!=NULL Ԫ�ؼ�; ret=NULL ʧ��
 *
 */
const char *hashIter(T_HASH_TABLE *ptHashTable);

/**
 * @function: hashRewind
 *
 * @desc: ���ù�ϣ�����ָ��
 *
 * @param ptHashTable: ��ϣ����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int hashRewind(T_HASH_TABLE *ptHashTable);

/**
 * @function: hashSort
 *
 * @desc: �Թ�ϣ����Ԫ�ؽ�������
 *
 * @param ptHashTable: ��ϣ����
 * @param fCompare: �ȽϺ���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int hashSort(T_HASH_TABLE *ptHashTable, int (*fCompare)(void *, void *));

/**
 * @function: hashSetFuncs
 *
 * @desc: ����callback����
 *
 * @param ptHashTable: ��ϣ����
 * @param tFuncs: callback����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int hashSetFuncs(T_HASH_TABLE *ptHashTable, T_HASH_FUNCS tFuncs);

/**
 * @function: hashFuncString
 *
 * @desc: �ַ�������Ԫ��callback����
 *
 * @return T_HASH_FUNCS: callback����
 *
 */
T_HASH_FUNCS hashFuncString(void);

/**
 * @function: hashFuncStruct
 *
 * @desc: �ṹ������Ԫ��callback����
 *
 * @param iSize: �ṹ���С
 *
 * @return T_HASH_FUNCS: callback����
 *
 */
T_HASH_FUNCS hashFuncStruct(int iSize);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_HASH_H_20101104140701_*/
/*-----------------------------  End ------------------------------------*/
