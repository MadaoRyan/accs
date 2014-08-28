/*
 *
 * 
 * dynamic library link functions.
 * 
 * 
 * FileName: os_dll.c
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include "dlfcn.h"
#include "os_dll.h"

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/
typedef int (*PFN_DLL_EXEC_FUN_TYPE0)(void);
typedef int (*PFN_DLL_EXEC_FUN_TYPE1)(void *);
typedef int (*PFN_DLL_EXEC_FUN_TYPE2)(void *, void *);
typedef int (*PFN_DLL_EXEC_FUN_TYPE3)(void *, void *, void *);
typedef int (*PFN_DLL_EXEC_FUN_TYPE4)(void *, void *, void *, void *);
typedef int (*PFN_DLL_EXEC_FUN_TYPE5)(void *, void *, void *, void *, void *);
typedef int (*PFN_DLL_EXEC_FUN_TYPE6)(void *, void *, void *, void *, void *, void *);
typedef int (*PFN_DLL_EXEC_FUN_TYPE7)(void *, void *, void *, void *, void *, void *, void *);
typedef int (*PFN_DLL_EXEC_FUN_TYPE8)(void *, void *, void *, void *, void *, void *, void *, void *);
/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
H_DllFile dllOpen(char *_sDllFile)
{
    H_DllFile hDll;
    
    hDll = dlopen(_sDllFile, RTLD_NOW);
    
    return hDll;
}

void *dllGetFun(H_DllFile _hDll, char *_sFunName)
{
    void *fun;

    fun = dlsym(_hDll, _sFunName);
    
    return fun;
}

void dllClose(H_DllFile _hDll)
{
    dlclose (_hDll); 
}

char *dllErrStr(void)
{
    return dlerror();
}

int dllExecFun(void *pfnFun, T_OS_DLL_FUN_ARG *ptArg, int iOptions)
{
    int iRet = 0;
    PFN_DLL_EXEC_FUN_TYPE0 pfnFun0 = NULL;
    PFN_DLL_EXEC_FUN_TYPE1 pfnFun1 = NULL;
    PFN_DLL_EXEC_FUN_TYPE2 pfnFun2 = NULL;
    PFN_DLL_EXEC_FUN_TYPE3 pfnFun3 = NULL;
    PFN_DLL_EXEC_FUN_TYPE4 pfnFun4 = NULL;
    PFN_DLL_EXEC_FUN_TYPE5 pfnFun5 = NULL;
    PFN_DLL_EXEC_FUN_TYPE6 pfnFun6 = NULL;
    PFN_DLL_EXEC_FUN_TYPE7 pfnFun7 = NULL;
    PFN_DLL_EXEC_FUN_TYPE8 pfnFun8 = NULL;

    switch(ptArg->iArgc) {
        case 0:
            {
                pfnFun0 = (PFN_DLL_EXEC_FUN_TYPE0)pfnFun;
                iRet = pfnFun0();
            }
            break;
        case 1:
            {
                pfnFun1 = (PFN_DLL_EXEC_FUN_TYPE1)pfnFun;
                iRet = pfnFun1(ptArg->psArgV[0]);
            }
            break;
        case 2:
            {
                pfnFun2 = (PFN_DLL_EXEC_FUN_TYPE2)pfnFun;
                iRet = pfnFun2(ptArg->psArgV[0], ptArg->psArgV[1]);
            }
            break;
        case 3:
            {
                pfnFun3 = (PFN_DLL_EXEC_FUN_TYPE3)pfnFun;
                iRet = pfnFun3(ptArg->psArgV[0], ptArg->psArgV[1], ptArg->psArgV[2]);
            }
            break;
        case 4:
            {
                pfnFun4 = (PFN_DLL_EXEC_FUN_TYPE4)pfnFun;
                iRet = pfnFun4(ptArg->psArgV[0], ptArg->psArgV[1], ptArg->psArgV[2], ptArg->psArgV[3]);
                
            }
            break;
        case 5:
            {
                pfnFun5 = (PFN_DLL_EXEC_FUN_TYPE5)pfnFun;
                iRet = pfnFun5(ptArg->psArgV[0], ptArg->psArgV[1], ptArg->psArgV[2], ptArg->psArgV[3], ptArg->psArgV[4]);
                
            }
            break;
        case 6:
            {
                pfnFun6 = (PFN_DLL_EXEC_FUN_TYPE6)pfnFun;
                iRet = pfnFun6(ptArg->psArgV[0], ptArg->psArgV[1], ptArg->psArgV[2], ptArg->psArgV[3], ptArg->psArgV[4], ptArg->psArgV[5]);
                
            }
            break;
        case 7:
            {
                pfnFun7 = (PFN_DLL_EXEC_FUN_TYPE7)pfnFun;
                iRet = pfnFun7(ptArg->psArgV[0], ptArg->psArgV[1], ptArg->psArgV[2], ptArg->psArgV[3], ptArg->psArgV[4], ptArg->psArgV[5], ptArg->psArgV[6]);
                
            }
            break;  
        case 8:
            {
                pfnFun8 = (PFN_DLL_EXEC_FUN_TYPE8)pfnFun;
                iRet = pfnFun8(ptArg->psArgV[0], ptArg->psArgV[1], ptArg->psArgV[2], ptArg->psArgV[3], ptArg->psArgV[4], ptArg->psArgV[5], ptArg->psArgV[6], ptArg->psArgV[7]);
                
            }
            break; 
        default:
            {
                return ERR_DLL_PARAM;
            }
    }
    
    return iRet;
}
/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

