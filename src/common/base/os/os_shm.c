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
 * ���������ڴ�
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
 * ӳ�乲���ڴ�
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
 * ȡ��ӳ��Ĺ����ڴ�
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
 * ɾ�������Ĺ����ڴ�
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

