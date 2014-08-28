/*
 *
 *
 * db operator for db2.
 *
 *
 * FileName: dbs_db2oper.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "top_dbs.h"
#include "dbs_x.h"
/*--------------------------- Macro define ------------------------------*/
#define _TINY_BUF_LEN   32
#define _MINI_BUF_LEN   256
#define _LARGE_BUF_LEN  1024
#define _HUGE_BUF_LEN   10240

#define _DBS_FUNS_MAX   21

#define _               logMsg

#define _DBS_FUNS(_FUNNUM) \
    dbsOperInit##_FUNNUM,          \
    dbsOperSetNumberLen##_FUNNUM,  \
    dbsPrepare##_FUNNUM,           \
    dbsBindCount##_FUNNUM,         \
    dbsBindName##_FUNNUM,          \
    dbsGetBind##_FUNNUM,           \
    dbsBindNull##_FUNNUM,          \
    dbsBindString##_FUNNUM,        \
    dbsBindArray##_FUNNUM,         \
    dbsBindSepValues##_FUNNUM,     \
    dbsOpen##_FUNNUM,              \
    dbsColumnCount##_FUNNUM,       \
    dbsColumnName##_FUNNUM,        \
    dbsColumnLength##_FUNNUM,      \
    dbsColumnString##_FUNNUM,      \
    dbsColumnChar##_FUNNUM,        \
    dbsColumnStrings##_FUNNUM,     \
    dbsColumnChars##_FUNNUM,       \
    dbsColumnBuffer##_FUNNUM,      \
    dbsFetch##_FUNNUM,             \
    dbsGetField##_FUNNUM,     \
    dbsGetInd##_FUNNUM,            \
    dbsClose##_FUNNUM


/*---------------------------- Type define ------------------------------*/
typedef struct {
    int (*pfnDbsOperInit)(int , int );
    int (*pfnDbsOperSetNumberLen)(int);
    int (*pfnDbsPrepare)(const char *);
    int (*pfnDbsBindCount)(void);
    char * (*pfnDbsBindName)(int , char *);
    char * (*pfnDbsGetBind)(int, char *, int *);
    int (*pfnDbsBindNull)(int);
    int (*pfnDbsBindString)(int , const char *, int );
    int (*pfnDbsBindArray)(char *asArray[], int iCnt);
    int (*pfnDbsBindSepValues)(char , const char *, int );
    int (*pfnDbsOpen)(void);
    int (*pfnDbsColumnCount)(void);
    char * (*pfnDbsColumnName)(int , char *);
    int (*pfnDbsColumnLength)(int);
    int (*pfnDbsColumnString)(int , char *, int );
    int (*pfnDbsColumnChar)(int , char *, int );
    int (*pfnDbsColumnStrings)(char *, int );
    int (*pfnDbsColumnChars)(char *, int );
    int (*pfnDbsColumnBuffer)(void);
    int (*pfnDbsFetch)(void);
    const char * (*pfnDbsGetField)(int , int *);
    int (*pfnDbsGetInd)(int);
    int (*pfnDbsClose)(void);
} T_DBS_FUNS;

/*---------------------- Local function declaration ---------------------*/
static int Init(int iDbsNum);

/*-------------------------  Global variable ----------------------------*/
static T_DBS_FUNS f_tFuns[_DBS_FUNS_MAX] = {  \
      {_DBS_FUNS(0)}  \
    , {_DBS_FUNS(1)}  \
    , {_DBS_FUNS(2)}  \
    , {_DBS_FUNS(3)}  \
    , {_DBS_FUNS(4)}  \
    , {_DBS_FUNS(5)}  \
    , {_DBS_FUNS(6)}  \
    , {_DBS_FUNS(7)}  \
    , {_DBS_FUNS(8)}  \
    , {_DBS_FUNS(9)}  \
    , {_DBS_FUNS(10)}  \
    , {_DBS_FUNS(11)}  \
    , {_DBS_FUNS(12)}  \
    , {_DBS_FUNS(13)}  \
    , {_DBS_FUNS(14)}  \
    , {_DBS_FUNS(15)}  \
    , {_DBS_FUNS(16)}  \
    , {_DBS_FUNS(17)}  \
    , {_DBS_FUNS(18)}  \
    , {_DBS_FUNS(19)}  \
    , {_DBS_FUNS(20)}  \
};

static int f_iDbsFlgs[_DBS_FUNS_MAX] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static int f_iDbsInitFlgs[_DBS_FUNS_MAX] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static int f_iMaxVarNum = 128;
static int f_iMaxNameLen = 128;

static char f_sCfgFile[_LARGE_BUF_LEN] = "";
static char f_sCfgSection[_MINI_BUF_LEN] = "";
/*-------------------------  Global functions ---------------------------*/
int dbsSetCfgFile(char *psCfgFile, char *psCfgSection)
{
    return ERR_TOP_DBS_OK;
}

int dbsOperInit(int iMaxVarNum, int iMaxNameLen)
{
    int iRet = 0;

    f_iMaxVarNum = iMaxVarNum;
    f_iMaxNameLen = iMaxNameLen;

    iRet = Init(0);
    if(iRet < 0) {
        return ERR_TOP_DBS_INIT;
    }

    return ERR_TOP_DBS_OK;
}


int dbsPrepare(const char *sSql)
{
    int iRet;

    iRet = Init(0);
    if(iRet < 0) {
        return ERR_TOP_DBS_INIT;
    }

    return f_tFuns[0].pfnDbsPrepare(sSql);
}

int dbsBindCount(void)
{
    return f_tFuns[0].pfnDbsBindCount();
}

char * dbsBindName(int iCol, char *sName)
{
    return f_tFuns[0].pfnDbsBindName(iCol, sName);
}

int dbsBindNull(int iCol)
{
    return f_tFuns[0].pfnDbsBindNull(iCol);
}

int dbsBindString(int iCol, const char *sStr, int iLen)
{
    return f_tFuns[0].pfnDbsBindString(iCol, sStr, iLen);
}

int dbsBindArray(char *asArray[], int iCnt)
{
    return f_tFuns[0].pfnDbsBindArray(asArray, iCnt);
}

int dbsBindSepValues(char sSep, const char *sBuf, int iLen)
{
    return f_tFuns[0].pfnDbsBindSepValues(sSep, sBuf, iLen);
}

int dbsOpen(void)
{
    return f_tFuns[0].pfnDbsOpen();
}

int dbsColumnCount(void)
{
    return f_tFuns[0].pfnDbsColumnCount();
}

char * dbsColumnName(int iCol, char *sName)
{
    return f_tFuns[0].pfnDbsColumnName(iCol, sName);
}

int dbsColumnLength(int iCol)
{
    return f_tFuns[0].pfnDbsColumnLength(iCol);
}

int dbsColumnString(int iCol, char *sBuf, int iSize)
{
    return f_tFuns[0].pfnDbsColumnString(iCol, sBuf, iSize);
}

int dbsColumnChar(int iCol, char *sBuf, int iSize)
{
    return f_tFuns[0].pfnDbsColumnChar(iCol, sBuf, iSize);
}

int dbsColumnStrings(char *sBuf, int iSize)
{
    return f_tFuns[0].pfnDbsColumnStrings(sBuf, iSize);
}

int dbsColumnChars(char *sBuf, int iSize)
{
    return f_tFuns[0].pfnDbsColumnChars(sBuf, iSize);
}

int dbsColumnBuffer(void)
{
    return f_tFuns[0].pfnDbsColumnBuffer();
}

int dbsFetch(void)
{
    return f_tFuns[0].pfnDbsFetch();
}

const char * dbsGetField(int iCol, int *piLen)
{
    return f_tFuns[0].pfnDbsGetField(iCol, piLen);
}

int dbsGetInd(int iCol)
{
    return f_tFuns[0].pfnDbsGetInd(iCol);
}

int dbsClose(void)
{
    return f_tFuns[0].pfnDbsClose();
}

int dbsOperSetNumberLen(int nLen)
{
    return f_tFuns[0].pfnDbsOperSetNumberLen(nLen);
}

/*---------------------------------------------*/
/*ÔöÇ¿°æAPI*/
/*---------------------------------------------*/
int dbsPrepareX(const char *sSql, T_TOP_DBS_STMT *phStmt)
{
    int iRet = 0;
    int i = 1;

    *phStmt = -1;

    if(NULL == phStmt) {
        return ERR_TOP_DBS_PARAM;
    }

    for(i=1; i<_DBS_FUNS_MAX; i++) {
        if(f_iDbsFlgs[i] == 0) {
            break;
        }
    }

    if(i >= _DBS_FUNS_MAX) {
        return ERR_TOP_DBS_SPACE;
    }

    iRet = Init(i);
    if(iRet < 0) {
        return ERR_TOP_DBS_INIT;
    }

    iRet = f_tFuns[i].pfnDbsPrepare(sSql);
    if(iRet == 0) {
        f_iDbsFlgs[i] = 1;
        *phStmt = i;
    }

    return iRet;
}

int dbsBindCountX(T_TOP_DBS_STMT hStmt)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsBindCount();
}

char * dbsBindNameX(T_TOP_DBS_STMT hStmt, int iCol, char *sName)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return NULL;
    }

    return f_tFuns[hStmt].pfnDbsBindName(iCol, sName);
}

char * dbsGetBindX(T_TOP_DBS_STMT hStmt, int iCol, char *sName, int *piLen)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return NULL;
    }

    return f_tFuns[hStmt].pfnDbsGetBind(iCol, sName, piLen);
}

int dbsBindNullX(T_TOP_DBS_STMT hStmt, int iCol)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsBindNull(iCol);
}

int dbsBindStringX(T_TOP_DBS_STMT hStmt, int iCol, const char *sStr, int iLen)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsBindString(iCol, sStr, iLen);
}

int dbsBindArrayX(T_TOP_DBS_STMT hStmt, char *asArray[], int iCnt)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsBindArray(asArray, iCnt);
}

int dbsBindSepValuesX(T_TOP_DBS_STMT hStmt, char sSep, const char *sBuf, int iLen)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsBindSepValues(sSep, sBuf, iLen);
}

int dbsOpenX(T_TOP_DBS_STMT hStmt)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsOpen();
}

int dbsColumnCountX(T_TOP_DBS_STMT hStmt)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsColumnCount();
}

char * dbsColumnNameX(T_TOP_DBS_STMT hStmt, int iCol, char *sName)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return NULL;
    }

    return f_tFuns[hStmt].pfnDbsColumnName(iCol, sName);
}

int dbsColumnLengthX(T_TOP_DBS_STMT hStmt, int iCol)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsColumnLength(iCol);
}

int dbsColumnStringX(T_TOP_DBS_STMT hStmt, int iCol, char *sBuf, int iSize)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsColumnString(iCol, sBuf, iSize);
}

int dbsColumnCharX(T_TOP_DBS_STMT hStmt, int iCol, char *sBuf, int iSize)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsColumnChar(iCol, sBuf, iSize);
}

int dbsColumnStringsX(T_TOP_DBS_STMT hStmt, char *sBuf, int iSize)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsColumnStrings(sBuf, iSize);
}

int dbsColumnCharsX(T_TOP_DBS_STMT hStmt, char *sBuf, int iSize)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsColumnChars(sBuf, iSize);
}

int dbsColumnBufferX(T_TOP_DBS_STMT hStmt)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsColumnBuffer();
}

int dbsFetchX(T_TOP_DBS_STMT hStmt)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsFetch();
}

int dbsOperSetNumberLenX(T_TOP_DBS_STMT hStmt, int nLen)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsOperSetNumberLen(nLen);
}

const char * dbsGetFieldX(T_TOP_DBS_STMT hStmt, int iCol, int *piLen)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return NULL;
    }

    return f_tFuns[hStmt].pfnDbsGetField(iCol, piLen);
}

int  dbsGetIndX(T_TOP_DBS_STMT hStmt, int iCol)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    return f_tFuns[hStmt].pfnDbsGetInd(iCol);
}

int dbsCloseX(T_TOP_DBS_STMT hStmt)
{
    if(hStmt <= 0 || hStmt >= _DBS_FUNS_MAX || f_iDbsInitFlgs[hStmt] == 0) {
        return ERR_TOP_DBS_HANDLE;
    }

    f_iDbsFlgs[hStmt] = 0;
    return f_tFuns[hStmt].pfnDbsClose();
}

int   dbsExecStmtX(char *psSql, char * psBindVars[], int iBindCnt, char * psColVars[], int iColCnt)
{
    return 0;
}

int dbsCloseAll(void)
{
    int i, iRet;

    for (i = 0; i < _DBS_FUNS_MAX; i++) {
        if (f_iDbsFlgs[i]) {
            _(LOG_ERR, "dbs T_TOP_DBS_STMT[%d] not closed!!!", i);
            iRet = dbsCloseX(i);
            if (iRet != 0) return iRet;
        }
    }

    return 0;
}

/*-------------------------  Local functions ----------------------------*/

int Init(int iDbsNum)
{
    int iRet = 0;

    if(f_iDbsInitFlgs[iDbsNum]) {
        return 0;
    }

    iRet = f_tFuns[iDbsNum].pfnDbsOperInit(f_iMaxVarNum, f_iMaxNameLen);

    f_iDbsInitFlgs[iDbsNum] = 1;

    return iRet;
}
/*-----------------------------  End ------------------------------------*/

