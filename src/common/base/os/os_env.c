/*
 *
 * 
 * env module for env.
 * 
 * 
 * FileName: os_env.c
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include "os_env.h"

/*--------------------------- Macro define ------------------------------*/
#define _DLEN_TINY_BUF 64
#define _DLEN_MINI_BUF 256
#define _DLEN_LARGE_BUF 1024
#define _DLEN_HUGE_BUF 10240
/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
int envParseStr(char *psOld, char *psNew)
{
    int i = 0;
    int j = 0;
    char sVarName[_DLEN_LARGE_BUF];
    int k = 0;
    char *psEnv = NULL;
    
    memset(sVarName, 0, sizeof(sVarName));
    
    for(i=0; psOld[i]!='\0'; i++) {
        if(psOld[i] == '$' && psOld[i+1] != '\0' && psOld[i+1] == '(') {
            i += 2;
            memset(sVarName, 0, sizeof(sVarName));
            for(k=0; psOld[i]!='\0' && psOld[i]!=')'; i++, k++) {
                sVarName[k] = psOld[i];
            }
            
            if(psOld[i] == '\0') {
                return -1;
            }
            
            psEnv = getenv(sVarName);
            if(NULL == psEnv) {
                return -2;
            }
            
            strcat(psNew, psEnv);
            j += strlen(psEnv);
            continue;
        }
        
        psNew[j] = psOld[i];
        j++;
    }
    
    return 0;
}
/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

