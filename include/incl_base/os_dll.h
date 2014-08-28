/*
 *
 *
 * dynamic library link functions.
 *
 *
 * FileName: os_dll.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_DLL_H_20100121133647_
#define _OS_DLL_H_20100121133647_
/*--------------------------- Include files -----------------------------*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>

#include <unistd.h>
/*--------------------------- Macro define ------------------------------*/
#define DLEN_DLM_FUN_ARGV_NUM_MAX 8

#define VAR_OS_DLL_FUN_ARG_TYPE_POINTER 0
#define VAR_OS_DLL_FUN_ARG_TYPE_LONG 1
#define VAR_OS_DLL_FUN_ARG_TYPE_INT 2
#define VAR_OS_DLL_FUN_ARG_TYPE_SHORT 3
#define VAR_OS_DLL_FUN_ARG_TYPE_CHAR 4

#define VAR_OS_DLL_FUN_RET_TYPE_VOID 0

/*error code*/
#define ERR_DLL_OK 0
#define ERR_DLL_BASE (-6000)
#define ERR_DLL_PARAM (ERR_DLL_BASE-1)
#define ERR_DLL_INIT (ERR_DLL_BASE-2)
#define ERR_DLL_NOTFOUND (ERR_DLL_BASE-3)
#define ERR_DLL_NOSPACE (ERR_DLL_BASE-4)
#define ERR_DLL_SYSTEM (ERR_DLL_BASE-5)
#define ERR_DLL_LOADLIB (ERR_DLL_BASE-6)
#define ERR_DLL_UNKNOW (ERR_DLL_BASE-100)

/*---------------------------- Type define ------------------------------*/
typedef void* H_DllFile;

typedef struct {
    int iArgc;
    int iArgVT[DLEN_DLM_FUN_ARGV_NUM_MAX];
    char* psArgV[DLEN_DLM_FUN_ARGV_NUM_MAX];
    int iRetType;
} T_OS_DLL_FUN_ARG;
/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: dllOpen
 *
 * @desc: ���ض�̬��
 *
 * @param _sDllFile: ��̬���ļ���
 *
 * @return H_DllFile: ret!=NULL ��̬����; ret=NULL ʧ��
 *
 */
H_DllFile dllOpen(char *_sDllFile);

/**
 * @function: dllGetFun
 *
 * @desc: ��ȡ��̬���к�����ַ
 *
 * @param _hDll: ��̬����
 * @param _sFunName: ��������
 *
 * @return void *: ret!=NULL ����ָ��; ret=NULL ʧ��
 *
 */
void* dllGetFun(H_DllFile _hDll, char *_sFunName);

/**
 * @function: dllClose
 *
 * @desc: �رն�̬���ÿ�
 *
 * @param _hDll: ��̬����
 *
 */
void  dllClose(H_DllFile _hDll);

/**
 * @function: dllErrStr
 *
 * @desc: ��̬���ӿ���ô���
 *
 * @return char *: �����ַ���ָ��
 *
 */
char* dllErrStr(void);


/**
 * @function: dllExecFun
 *
 * @desc: ��ִ̬�к���
 *
 * @param pfnFun: ����ָ��
 * @param ptArg: ��������
 * @param iOptions: ����ѡ��
 *
 * @return int: ����ִ�н��
 *
 */
int dllExecFun(void *pfnFun, T_OS_DLL_FUN_ARG *ptArg, int iOptions);

#ifdef __cplusplus
}
#endif

#endif /*_OS_DLL_H_20100121133647_*/
/*-----------------------------  End ------------------------------------*/
