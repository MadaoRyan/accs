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
 * @desc: 排序
 *
 * @param pBase: 数组起始地址
 * @param iNum: 数组个数
 * @param iSize: 数组中每个单位大小
 * @param fCompar: 比较函数
 *
 */
void algSort(void *pBase, size_t iNum, size_t iSize, F_COMPAR fCompar);

/**
 * @function: algBSearch
 *
 * @desc: 搜索（二分法）
 *
 * @param pKey: 查找目标
 * @param pBase: 数组起始地址
 * @param iNum: 数组个数
 * @param iSize: 数组中每个单位大小
 * @param fCompar: 比较函数
 *
 * @return void *: ret!=NULL 成功; ret=NULL 失败
 *
 */
void *algBSearch(const void *pKey, const void *pBase, size_t iNum, size_t iSize,
                 F_COMPAR fCompar);

/**
 * @function: algLSearch
 *
 * @desc: 搜索（线性）
 *
 * @param pBase: 数组起始地址
 * @param iNum: 数组个数
 * @param iSize: 数组中每个单位大小
 * @param fCompar: 比较函数
 *
 * @return void *: ret!=NULL 成功; ret=NULL 失败
 *
 */
void *algLSearch(const void *pKey, const void *pBase, size_t iNum, size_t iSize,
                 F_COMPAR fCompar);

/**
 * @function: algHCreate
 *
 * @desc: 创建哈希表
 *
 * @param iNum: 哈希表能容纳的元素个数
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int algHCreate(size_t iNum);

/**
 * @function: algHSearch
 *
 * @desc: 在哈希表中查找目标
 *
 * @param tItem: 查找目标
 *
 * @return T_ITEM *: ret!=NULL 成功; ret=NULL 失败
 *
 */
T_ITEM *algHSearch(T_ITEM tItem);

/**
 * @function: algHEnter
 *
 * @desc: 向哈希表插入数据
 *
 * @param tItem: 欲插入哈希表的元素
 *
 * @return int: ret=1 重键; ret=0 成功; ret<0 失败
 *
 */
int algHEnter(T_ITEM tItem);

/**
 * @function: algHDestroy
 *
 * @desc: 释放哈希表
 *
 */
void algHDestroy(void);

/**
 * @function: algHCreateR
 *
 * @desc: 创建哈希表
 *
 * @param iNum: 哈希表能容纳的元素个数
 * @param ptHtab: 哈希表句柄
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int algHCreateR(size_t iNum, T_HTABLE *ptHtab);

/**
 * @function: algHSearchR
 *
 * @desc: 在哈希表中查找目标
 *
 * @param tItem: 查找目标
 * @param ptHtab: 哈希表句柄
 *
 * @return T_ITEM *: ret!=NULL 成功; ret=NULL 失败
 *
 */
T_ITEM *algHSearchR(T_ITEM tItem, T_HTABLE *ptHtab);

/**
 * @function: algHEnterR
 *
 * @desc: 向哈希表插入数据
 *
 * @param tItem: 欲插入哈希表的元素
 * @param ptHtab: 哈希表句柄
 *
 * @return int: ret=1 重键; ret=0 成功; ret<0 失败
 *
 */
int algHEnterR(T_ITEM tItem, T_HTABLE *ptHtab);

/**
 * @function: algHDestroyR
 *
 * @desc: 释放哈希表
 *
 * @param ptHtab: 哈希表句柄
 *
 */
void algHDestroyR(T_HTABLE *ptHtab);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_ALG_H_20100121112643_*/
/*-----------------------------  End ------------------------------------*/
