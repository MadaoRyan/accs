/*
 *
 * 
 * Semaphore functions.
 * 
 * 
 * FileName: os_sem.c
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include "os_sem.h"

/*--------------------------- Macro define ------------------------------*/
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#elif defined(__APPLE__)

#else
/* according to X/OPEN we have to define it ourselves */
union semun {
    int val;                  /* value for SETVAL */
    struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array;    /* array for GETALL, SETALL */
    /* Linux specific part: */
    struct seminfo *__buf;    /* buffer for IPC_INFO */
};
#endif

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
int semCreate(int iKey, int iOption)
{
    int iSemId = 0;
    union semun arg; 
    
    iSemId = semget(iKey, 1, IPC_CREAT|IPC_EXCL|0666);
    if(iSemId < 0) {
        return -1;
    } else {
        arg.val = 0;
        
        if(semctl(iSemId,0,SETVAL,arg) == -1) {
            return -1;
        } else {
            return 0;
        }
    }
    
    return 0;
}

int semP(int iSemId)
{
    struct sembuf tSemopVar;
    
    tSemopVar.sem_num = 0;
    tSemopVar.sem_op = -1;
    tSemopVar.sem_flg = SEM_UNDO;
    
    if(semop(iSemId, &tSemopVar, 1) == -1) {
        return -1;
    }
    
    return 0;
}

int semV(int iSemId)
{
    struct sembuf tSemopVar;
    
    tSemopVar.sem_num = 0;
    tSemopVar.sem_op = 1;
    tSemopVar.sem_flg = SEM_UNDO;
    
    if(semop(iSemId, &tSemopVar, 1) == -1) {
        return -1;
    }
    
    return 0;
}

int semDelete(int iSemId)
{
    if(semctl(iSemId, 0, IPC_RMID) == -1) {
        return -1;
    } 
    
    return 0;
}
/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

