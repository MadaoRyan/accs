/*
 *
 * 
 * share memory functions.
 * 
 * 
 * FileName: os_shm.c
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include "os_shm.h"

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
/**
 * 创建共享内存
 */
int shmOpen(int iKey, int iSize, int iFlg, T_SHM_HANDLE *ptShmHdl)
{
    int iRet = 0;
#ifdef _WIN32
    return -1;
#else    
    iRet = shmget (iKey, iSize, iFlg);
    if(iRet < 0) {
        return -1;
    }
    
    ptShmHdl->iShareMem = iRet;
#endif    
    return 0;
}

/**
 * 映射共享内存
 */
void * shmMap(T_SHM_HANDLE *ptShmHdl)
{
    void *ptr;

#ifdef _WIN32
    return 0;
#else     
    ptr = shmat(ptShmHdl->iShareMem, 0, 0);
#endif       
    return ptr;
}

/**
 * 取消映射的共享内存
 */
int shmUnMap(void * pAddr)
{
    int iRet = 0;
#ifdef _WIN32
    return -1;
#else     
    iRet = shmdt (pAddr);
#endif    
    return iRet;
}

/**
 * 删除创建的共享内存
 */
int shmRemove(T_SHM_HANDLE *ptShmHdl)
{
    int iRet = 0;
#ifdef _WIN32
    return -1;
#else     
    iRet = shmctl(ptShmHdl->iShareMem,IPC_RMID,0);
#endif     
    return iRet;
}
/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

