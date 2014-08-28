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
 * @desc: 创建哈希表
 *
 * @param iSize: 初始哈希表大小
 *
 * @return T_HASH_TABLE *: ret!=NULL 哈希表句柄; ret=NULL 失败
 *
 */
T_HASH_TABLE *hashNew(int iSize);

/**
 * @function: hashAdd
 *
 * @desc: 向哈希表中插入元素, 若已存在则执行失败
 *
 * @param ptHashTable: 哈希表句柄
 * @param sKey: 元素键
 * @param pValue: 元素值
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int hashAdd(T_HASH_TABLE *ptHashTable, const char *sKey, void *pValue);

/**
 * @function: hashSet
 *
 * @desc: 设置元素的值, 若不存在则新增
 *
 * @param ptHashTable: 哈希表句柄
 * @param sKey: 元素键
 * @param pValue: 元素值
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int hashSet(T_HASH_TABLE *ptHashTable, const char *sKey, void *pValue);

/**
 * @function: hashGet
 *
 * @desc: 取得元素的值
 *
 * @param ptHashTable: 哈希表句柄
 * @param sKey: 元素键
 *
 * @return void *: ret!=NULL 元素值; ret=NULL 失败
 *
 */
void *hashGet(T_HASH_TABLE *ptHashTable, const char *sKey);

/**
 * @function: hashDel
 *
 * @desc: 从哈希表中删除元素
 *
 * @param ptHashTable: 哈希表句柄
 * @param sKey: 元素键
 *
 * @return void *: ret!=NULL 被删除的元素值; ret=NULL 失败
 *
 */
void *hashDel(T_HASH_TABLE *ptHashTable, const char *sKey);

/**
 * @function: hashFree
 *
 * @desc: 释放哈希表
 *
 * @param ptHashTable: 哈希表句柄
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int hashFree(T_HASH_TABLE *ptHashTable);

/**
 * @function: hashCount
 *
 * @desc: 取得哈希表中元素个数
 *
 * @param ptHashTable: 哈希表句柄
 *
 * @return int: 元素个数
 *
 */
int hashCount(T_HASH_TABLE *ptHashTable);

/**
 * @function: hashIter
 *
 * @desc: 遍历哈希表
 *
 * @param ptHashTable: 哈希表句柄
 *
 * @return const char *: ret!=NULL 元素键; ret=NULL 失败
 *
 */
const char *hashIter(T_HASH_TABLE *ptHashTable);

/**
 * @function: hashRewind
 *
 * @desc: 重置哈希表遍历指针
 *
 * @param ptHashTable: 哈希表句柄
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int hashRewind(T_HASH_TABLE *ptHashTable);

/**
 * @function: hashSort
 *
 * @desc: 对哈希表内元素进行排序
 *
 * @param ptHashTable: 哈希表句柄
 * @param fCompare: 比较函数
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int hashSort(T_HASH_TABLE *ptHashTable, int (*fCompare)(void *, void *));

/**
 * @function: hashSetFuncs
 *
 * @desc: 设置callback函数
 *
 * @param ptHashTable: 哈希表句柄
 * @param tFuncs: callback函数
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int hashSetFuncs(T_HASH_TABLE *ptHashTable, T_HASH_FUNCS tFuncs);

/**
 * @function: hashFuncString
 *
 * @desc: 字符串类型元素callback函数
 *
 * @return T_HASH_FUNCS: callback函数
 *
 */
T_HASH_FUNCS hashFuncString(void);

/**
 * @function: hashFuncStruct
 *
 * @desc: 结构体类型元素callback函数
 *
 * @param iSize: 结构体大小
 *
 * @return T_HASH_FUNCS: callback函数
 *
 */
T_HASH_FUNCS hashFuncStruct(int iSize);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_HASH_H_20101104140701_*/
/*-----------------------------  End ------------------------------------*/
