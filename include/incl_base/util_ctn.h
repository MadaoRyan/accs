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
 * @desc: 创建容器
 *
 * @param iSize: 单位大小
 * @param fCompar: 比较函数
 *
 * @return T_CTN *: 容器句柄
 *
 */
T_CTN *ctnNew(int iSize, F_CTN_COMPAR fCompar);

/**
 * @function: ctnReAlloc
 *
 * @desc: 为容器分配更大空间
 *
 * @param ptCtn: 容器句柄
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int ctnReAlloc(T_CTN *ptCtn);

/**
 * @function: ctnFree
 *
 * @desc: 释放容器
 *
 * @param ptCtn: 容器句柄
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int ctnFree(T_CTN *ptCtn);

/**
 * @function: ctnSetSorted
 *
 * @desc: 设置容器排序状态
 *
 * @param ptCtn: 容器句柄
 * @param iSorted: 排序状态
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int ctnSetSorted(T_CTN *ptCtn, int iSorted);

/**
 * @function: ctnSetCompar
 *
 * @desc: 设置比较函数
 *
 * @param ptCtn: 容器句柄
 * @param fCompar: 比较函数
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int ctnSetCompar(T_CTN *ptCtn, F_CTN_COMPAR fCompar);

/**
 * @function: ctnAdd
 *
 * @desc: 向容器中增加元素
 *
 * @param ptCtn: 容器句柄
 * @param pData: 元素
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int ctnAdd(T_CTN *ptCtn, void *pData);

/**
 * @function: ctnGet
 *
 * @desc: 取得元素
 *
 * @param ptCtn: 容器句柄
 * @param iIdx: 元素序号
 *
 * @return void *: ret!=NULL 元素地址; ret=NULL 失败
 *
 */
void *ctnGet(T_CTN *ptCtn, int iIdx);

/**
 * @function: ctnIdx
 *
 * @desc: 取得元素序号
 *
 * @param ptCtn: 容器句柄
 * @param pData: 元素
 *
 * @return int: ret>=0 元素序号; ret<0 失败
 *
 */
int ctnIdx(T_CTN *ptCtn, void *pData);

/**
 * @function: ctnCount
 *
 * @desc: 取得容器当前元素个数
 *
 * @param ptCtn: 容器句柄
 *
 * @return int: ret>=0 元素个数; ret<0 失败
 *
 */
int ctnCount(T_CTN *ptCtn);

/**
 * @function: ctnSort
 *
 * @desc: 对容器中元素进行排序
 *
 * @param ptCtn: 容器句柄
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int ctnSort(T_CTN *ptCtn);

/**
 * @function: ctnSearch
 *
 * @desc: 在容器中搜索元素
 *
 * @param ptCtn: 容器句柄
 * @param pKey: 目标元素
 *
 * @return void *: ret!=NULL 成功; ret=NULL 失败
 *
 */
void *ctnSearch(T_CTN *ptCtn, const void *pKey);

/**
 * @function: ctnSearchL
 *
 * @desc: 在容器中搜索元素(线性)
 *
 * @param ptCtn: 容器句柄
 * @param pKey: 目标元素
 * @param pCur: 当前搜索的位置
 *
 * @return void *: ret!=NULL 成功; ret=NULL 失败
 *
 */
void *ctnSearchL(T_CTN *ptCtn, const void *pKey, const void *pCur);

/**
 * @function: ctnSearchM
 *
 * @desc: 在容器中搜索元素(多数)
 *
 * @param ptCtn: 容器句柄
 * @param pKey: 目标元素
 * @param pCur: 当前搜索的位置
 *
 * @return void *: ret!=NULL 成功; ret=NULL 失败
 *
 */
void *ctnSearchM(T_CTN *ptCtn, const void *pKey, const void *pCur);

/**
 * @function: ctnSearchIdx
 *
 * @desc:
 *
 * @param ptCtn: 容器句柄
 * @param pKey:
 *
 * @return int: ret=0 目标序号; ret<0 失败
 *
 */
int ctnSearchIdx(T_CTN *ptCtn, const void *pKey);

/**
 * @function: ctnSearchIdxL
 *
 * @desc: 在容器中搜索元素(线性)
 *
 * @param ptCtn: 容器句柄
 * @param pKey: 目标元素
 * @param iCur: 当前搜索的位置
 *
 * @return int: ret=0 目标序号; ret<0 失败
 *
 */
int ctnSearchIdxL(T_CTN *ptCtn, const void *pKey, const int iCur);

/**
 * @function: ctnSearchIdxM
 *
 * @desc: 在容器中搜索元素(多数)
 *
 * @param ptCtn: 容器句柄
 * @param pKey: 目标元素
 * @param iCur: 当前搜索的位置
 *
 * @return int: ret=0 目标序号; ret<0 失败
 *
 */
int ctnSearchIdxM(T_CTN *ptCtn, const void *pKey, const int iCur);

/**
 * @function: ctnComparInt
 *
 * @desc: 比较int型变量
 *
 * @param p1: 变量1
 * @param p2: 变量2
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
