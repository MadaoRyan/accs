/*
 *
 *
 * topframe dbs interface.
 *
 *
 * FileName: top_dbs.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "os_dll.h"
#include "top_dbs.h"
#include "log_info.h"

/*--------------------------- Macro define ------------------------------*/
#define _DLEN_TINY_BUF 64
#define _DLEN_MINI_BUF 256
#define _DLEN_LARGE_BUF 1024
#define _DLEN_HUGE_BUF 10240


#define _DBS_LIB_ORA "libdbs_oracle.so"
#define _DBS_LIB_DB2 "libdbs_db2.so"
#define _DBS_LIB_MYSQL "libdbs_mysql.so"
#define _DBS_LIB_SYBASE "libdbs_sybase.so"
#define _DBS_LIB_INFORMIX "libdbs_informix.so"

#define _DBS_FUN_LOAD(_FUN, _FUNNAME) \
    do {                \
        _FUN = dllGetFun(f_hDbsLib, _FUNNAME); \
        if(NULL == _FUN) {    \
            logMsg(LOG_ERR, "get lib[%s] fun[%s] error[%s]", sDbsLib, _FUNNAME, dllErrStr());  \
            return ERR_TOP_DBS_INIT; \
        }   \
    } while(0)

#define _DBS_FUN_LOAD_WRN(_FUN, _FUNNAME) \
    do {                \
        _FUN = dllGetFun(f_hDbsLib, _FUNNAME); \
        if(NULL == _FUN) {    \
            logMsg(LOG_WRN, "get lib[%s] fun[%s] error[%s]", sDbsLib, _FUNNAME, dllErrStr());  \
        }   \
    } while(0)

#define _DBS_RETURN(iRet) \
do { \
    if ( iRet != ERR_TOP_DBS_OK && iRet != ERR_TOP_DBS_NOTFOUND && NULL != f_pfnDbsErrCall) { \
        char sBuf[_DLEN_TINY_BUF]; \
        sprintf(sBuf, "%d|%d", getpid(), iRet); \
        f_pfnDbsErrCall(sBuf); \
    } \
 \
    return (iRet); \
}while(0);

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/
static int Init(void);
static int CheckDBConnected(void);

/*-------------------------  Global variable ----------------------------*/
static int f_iIsInit = 0;
static int f_iDbType = 0;
static int f_iChkFlag = 0;
static H_DllFile f_hDbsLib = NULL;

static PFN_TOP_DBS_CALLBACK f_pfnConnMissCallBack = NULL;
static PFN_TOP_DBS_ERR_CALL f_pfnDbsErrCall = NULL;

static T_TOP_DBS_STMT f_hStmt;

/*---------------itfs --------------*/
int (*f_pfnDbsSetCfgFile)(char *, char *) = NULL;
int (*f_pfnDbsConnect)(char *) = NULL;
int (*f_pfnDbsBegin)(void) = NULL;
int (*f_pfnDbsCommit)(void) = NULL;
int (*f_pfnDbsRollback)(void) = NULL;
int (*f_pfnDbsDisconnect)(void) = NULL;
int (*f_pfnDbsSqlCode)(void) = NULL;
int (*f_pfnDbsSqlcaSave)(void) = NULL;
int (*f_pfnDbsSqlcaRestore)(void) = NULL;
char * (*f_pfnDbsGetErrInfo)(char *, int ) = NULL;
char * (*f_pfnDbsErrStr)(void) = NULL;
char * (*f_pfnDbsGetLastSql)(char *, int ) = NULL;
char * (*f_pfnDbsLastSql)(void) = NULL;
int (*f_pfnDbsExecute)(const char *) = NULL;

int (*f_pfnDbsOperInit)(int , int ) = NULL;
int (*f_pfnDbsOperSetNumberLen)(int) = NULL;
int (*f_pfnDbsPrepare)(const char *) = NULL;
int (*f_pfnDbsBindCount)(void) = NULL;
char * (*f_pfnDbsBindName)(int , char *) = NULL;
int (*f_pfnDbsBindNull)(int) = NULL;
int (*f_pfnDbsBindString)(int , const char *, int ) = NULL;
int (*f_pfnDbsBindArray)(char *asArray[], int iCnt) = NULL;
int (*f_pfnDbsBindSepValues)(char , const char *, int ) = NULL;
int (*f_pfnDbsOpen)(void) = NULL;
int (*f_pfnDbsColumnCount)(void) = NULL;
char * (*f_pfnDbsColumnName)(int , char *) = NULL;
int (*f_pfnDbsColumnLength)(int) = NULL;
int (*f_pfnDbsColumnString)(int , char *, int ) = NULL;
int (*f_pfnDbsColumnChar)(int , char *, int ) = NULL;
int (*f_pfnDbsColumnStrings)(char *, int ) = NULL;
int (*f_pfnDbsColumnChars)(char *, int ) = NULL;
int (*f_pfnDbsColumnBuffer)(void) = NULL;
int (*f_pfnDbsFetch)(void) = NULL;
const char * (*f_pfnDbsGetField)(int , int *) = NULL;
int (*f_pfnDbsGetInd)(int) = NULL;
int (*f_pfnDbsClose)(void) = NULL;


/*增强版API*/
int (*f_pfnDbsPrepareX)(const char *, T_TOP_DBS_STMT *phStmt) = NULL;
int (*f_pfnDbsBindCountX)(T_TOP_DBS_STMT ) = NULL;
char * (*f_pfnDbsBindNameX)(T_TOP_DBS_STMT , int , char *) = NULL;
char * (*f_pfnDbsGetBindX)(T_TOP_DBS_STMT, int , char *, int *) = NULL;
int (*f_pfnDbsBindNullX)(T_TOP_DBS_STMT, int) = NULL;
int (*f_pfnDbsBindStringX)(T_TOP_DBS_STMT, int , const char *, int ) = NULL;
int (*f_pfnDbsBindArrayX)(T_TOP_DBS_STMT, char *asArray[], int iCnt) = NULL;
int (*f_pfnDbsBindSepValuesX)(T_TOP_DBS_STMT, char , const char *, int ) = NULL;
int (*f_pfnDbsOpenX)(T_TOP_DBS_STMT) = NULL;
int (*f_pfnDbsColumnCountX)(T_TOP_DBS_STMT) = NULL;
char * (*f_pfnDbsColumnNameX)(T_TOP_DBS_STMT, int , char *) = NULL;
int (*f_pfnDbsColumnLengthX)(T_TOP_DBS_STMT, int) = NULL;
int (*f_pfnDbsColumnStringX)(T_TOP_DBS_STMT, int , char *, int ) = NULL;
int (*f_pfnDbsColumnCharX)(T_TOP_DBS_STMT, int , char *, int ) = NULL;
int (*f_pfnDbsColumnStringsX)(T_TOP_DBS_STMT, char *, int ) = NULL;
int (*f_pfnDbsColumnCharsX)(T_TOP_DBS_STMT, char *, int ) = NULL;
int (*f_pfnDbsColumnBufferX)(T_TOP_DBS_STMT) = NULL;
int (*f_pfnDbsFetchX)(T_TOP_DBS_STMT) = NULL;
int (*f_pfnDbsOperSetNumberLenX)(T_TOP_DBS_STMT, int) = NULL;
const char * (*f_pfnDbsGetFieldX)(T_TOP_DBS_STMT, int , int *) = NULL;
int (*f_pfnDbsGetIndX)(T_TOP_DBS_STMT, int) = NULL;
int (*f_pfnDbsCloseX)(T_TOP_DBS_STMT) = NULL;

int (*f_pfnDbsPreparePageX)(const char *, T_TOP_DBS_STMT *, T_DBS_PAGE *) = NULL;
int (*f_pfnDbsFetchPageX)(T_TOP_DBS_STMT, T_DBS_PAGE *) = NULL;
char *(*f_pfnDbsCountSql)(const char *sSql, char *sCountSql, int iSize) = NULL;
int (*f_pfnDbsCloseAll)(void) = NULL;

int (*f_pfnDbsSetOption)(char *sOption, char *sValue);

/*-------------------------  Global functions ---------------------------*/
PFN_TOP_DBS_ERR_CALL dbsSetErrCall(PFN_TOP_DBS_ERR_CALL fFnc)
{
    PFN_TOP_DBS_ERR_CALL f_tmp = f_pfnDbsErrCall;
    f_pfnDbsErrCall = fFnc;

    return f_tmp;
}

int dbsSetCheckBefore(int iChkFlag)
{
    f_iChkFlag = iChkFlag;
    return 0;
}


int dbsSetDbType(int iDbType)
{
    int iRet = 0;

    f_iDbType = iDbType;

    iRet = Init();

    return iRet;
}

int dbsGetDbType( )
{
    return f_iDbType;
}

int dbsSetCfgFile(char *psCfgFile, char *psCfgSection)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsSetCfgFile) {
       return ERR_TOP_DBS_UNKNOW;
    }


    return f_pfnDbsSetCfgFile(psCfgFile, psCfgSection);
}


int dbsConnect(char *sConnStr)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsConnect) {
       return ERR_TOP_DBS_UNKNOW;
    }

	iRet = f_pfnDbsConnect(sConnStr);
	if (iRet != ERR_TOP_DBS_OK) {
		logMsg(LOG_TRC, RC_TOP_DBS_CONN "%s^%d^%s",
		        sConnStr, f_iDbType, dbsErrStr());
        _DBS_RETURN(iRet);
	}

    return 0;
}

int dbsBegin(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsBegin) {
       return ERR_TOP_DBS_UNKNOW;
    }

	CheckDBConnected();

    return f_pfnDbsBegin();

}

int dbsCommit(void)
{
    int iRet = 0;
    char * psArgv[1] = {NULL};

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsCommit) {
       return ERR_TOP_DBS_UNKNOW;
    }

    iRet = f_pfnDbsCommit();
    if((ERR_TOP_DBS_DISCONNECT == iRet) && (NULL != f_pfnConnMissCallBack)) {
        f_pfnConnMissCallBack(0, psArgv);
    }
    
    return iRet;

}

int dbsRollback(void)
{
    int iRet = 0;
    char * psArgv[1] = {NULL};
    
    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsRollback) {
       return ERR_TOP_DBS_UNKNOW;
    }
    
    iRet = f_pfnDbsRollback();
    if((ERR_TOP_DBS_DISCONNECT == iRet) && (NULL != f_pfnConnMissCallBack)) {
        f_pfnConnMissCallBack(0, psArgv);
    }

    return iRet;
}


int dbsDisconnect(void)
{
    if(NULL == f_pfnDbsDisconnect) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsDisconnect();
}


int dbsSqlCode(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsSqlCode) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsSqlCode();
}


int dbsSqlcaSave(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsSqlcaSave) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsSqlcaSave();
}


int dbsSqlcaRestore(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsSqlcaRestore) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsSqlcaRestore();
}


char *dbsGetErrInfo(char *sBuf, int iBufLen)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return NULL;
    }

    if(NULL == f_pfnDbsGetErrInfo) {
       return NULL;
    }

    return f_pfnDbsGetErrInfo(sBuf, iBufLen);
}


char *dbsErrStr(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return NULL;
    }

    if(NULL == f_pfnDbsErrStr) {
       return NULL;
    }

    return f_pfnDbsErrStr();
}


char *dbsGetLastSql(char *sBuf, int iBufLen)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return NULL;
    }

    if(NULL == f_pfnDbsGetLastSql) {
       return NULL;
    }

    return f_pfnDbsGetLastSql(sBuf, iBufLen);
}


char *dbsLastSql(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return NULL;
    }

    if(NULL == f_pfnDbsLastSql) {
       return NULL;
    }

    return f_pfnDbsLastSql();
}


int dbsExecute(const char *sSql)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsExecute) {
       return ERR_TOP_DBS_UNKNOW;
    }

	CheckDBConnected();

	iRet = f_pfnDbsExecute( sSql );
    _DBS_RETURN(iRet);
}


int dbsOperInit(int iMaxVarNum, int iMaxNameLen)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsOperInit) {
       return ERR_TOP_DBS_UNKNOW;
    }

	CheckDBConnected();

    return f_pfnDbsOperInit(iMaxVarNum, iMaxNameLen);
}

int dbsOperSetNumberLen(int nLen)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsOperSetNumberLen) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsOperSetNumberLen(nLen);
}

int dbsPrepare(const char *sSql)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsPrepare) {
       return ERR_TOP_DBS_UNKNOW;
    }

	CheckDBConnected();

	iRet = f_pfnDbsPrepare( sSql );
    _DBS_RETURN(iRet);
}

int dbsBindCount(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsBindCount) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsBindCount();
}

char *dbsBindName(int iCol, char *sName)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return NULL;
    }

    if(NULL == f_pfnDbsBindName) {
       return NULL;
    }

    return f_pfnDbsBindName(iCol, sName);
}

int dbsBindNull(int iCol)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsBindNull) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsBindNull(iCol);
}

int dbsBindString(int iCol, const char *sStr, int iLen)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsBindString) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsBindString(iCol, sStr, iLen);
}

int dbsBindArray(char *asArray[], int iCnt)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsBindArray) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsBindArray(asArray, iCnt);
}

int dbsBindSepValues(char sSep, const char *sBuf, int iLen)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsBindSepValues) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsBindSepValues(sSep, sBuf, iLen);
}

int dbsOpen(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsOpen) {
       return ERR_TOP_DBS_UNKNOW;
    }

	CheckDBConnected();

    iRet = f_pfnDbsOpen();
    _DBS_RETURN(iRet);
}

int dbsColumnCount(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnCount) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnCount();
}

char *dbsColumnName(int iCol, char *sName)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return NULL;
    }

    if(NULL == f_pfnDbsColumnName) {
       return NULL;
    }

    return f_pfnDbsColumnName(iCol, sName);
}

int dbsColumnLength(int iCol)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnLength) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnLength(iCol);
}

int dbsColumnString(int iCol, char *sBuf, int iSize)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnString) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnString(iCol, sBuf, iSize);
}

int dbsColumnChar(int iCol, char *sBuf, int iSize)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnChar) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnChar(iCol, sBuf, iSize);
}

int dbsColumnStrings(char *sBuf, int iSize)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnStrings) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnStrings(sBuf, iSize);
}

int dbsColumnChars(char *sBuf, int iSize)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnChars) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnChars(sBuf, iSize);
}


int dbsColumnBuffer(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnBuffer) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnBuffer();
}

int  dbsFetch(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsFetch) {
       return ERR_TOP_DBS_UNKNOW;
    }

    iRet = f_pfnDbsFetch();
    _DBS_RETURN(iRet);
}

const char *dbsGetField(int iCol, int *piLen)
{
    int iRet = 0;

    if(piLen) {
        *piLen = 0;
    }

    iRet = Init();
    if(iRet < 0) {
        return NULL;
    }

    if(NULL == f_pfnDbsGetField) {
       return NULL;
    }

    return f_pfnDbsGetField(iCol, piLen);
}

int dbsGetInd(int iCol)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsGetInd) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsGetInd(iCol);
}

int dbsClose(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsClose) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsClose();
}

/*--------------------------------------------------------*/
/*增强版API*/
/*--------------------------------------------------------*/
int dbsPrepareX(const char *sSql, T_TOP_DBS_STMT *phStmt)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsPrepareX) {
       return ERR_TOP_DBS_UNKNOW;
    }

	CheckDBConnected();

    iRet = f_pfnDbsPrepareX(sSql, phStmt);
    f_hStmt = *phStmt;
    _DBS_RETURN(iRet);

    return iRet;
}

int dbsBindCountX(T_TOP_DBS_STMT hStmt)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsBindCountX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsBindCountX(hStmt);
}

char *dbsBindNameX(T_TOP_DBS_STMT hStmt, int iCol, char *sName)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return NULL;
    }

    if(NULL == f_pfnDbsBindNameX) {
       return NULL;
    }

    return f_pfnDbsBindNameX(hStmt, iCol, sName);
}

char *dbsGetBindX(T_TOP_DBS_STMT hStmt, int iCol, char *sName, int *piLen)
{
    int iRet = 0;

    iRet = Init();
    if (iRet < 0) {
        return NULL;
    }

    if (NULL == f_pfnDbsGetBindX) {
        return NULL;
    }

    return f_pfnDbsGetBindX(hStmt, iCol, sName, piLen);
}

int dbsBindNullX(T_TOP_DBS_STMT hStmt, int iCol)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsBindNullX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsBindNullX(hStmt, iCol);
}

int dbsBindStringX(T_TOP_DBS_STMT hStmt, int iCol, const char *sStr, int iLen)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsBindStringX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsBindStringX(hStmt, iCol, sStr, iLen);
}

int dbsBindArrayX(T_TOP_DBS_STMT hStmt, char *asArray[], int iCnt)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsBindArrayX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsBindArrayX(hStmt, asArray, iCnt);
}

int dbsBindSepValuesX(T_TOP_DBS_STMT hStmt, char sSep, const char *sBuf, int iLen)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsBindSepValuesX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsBindSepValuesX(hStmt, sSep, sBuf, iLen);
}

int dbsOpenX(T_TOP_DBS_STMT hStmt)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsOpenX) {
       return ERR_TOP_DBS_UNKNOW;
    }

	CheckDBConnected();

    iRet = f_pfnDbsOpenX(hStmt);
    _DBS_RETURN(iRet);
}

int dbsColumnCountX(T_TOP_DBS_STMT hStmt)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnCountX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnCountX(hStmt);
}

char *dbsColumnNameX(T_TOP_DBS_STMT hStmt, int iCol, char *sName)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return NULL;
    }

    if(NULL == f_pfnDbsColumnNameX) {
       return NULL;
    }

    return f_pfnDbsColumnNameX(hStmt, iCol, sName);
}

int dbsColumnLengthX(T_TOP_DBS_STMT hStmt, int iCol)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnLengthX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnLengthX(hStmt, iCol);
}

int dbsColumnStringX(T_TOP_DBS_STMT hStmt, int iCol, char *sBuf, int iSize)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnStringX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnStringX(hStmt, iCol, sBuf, iSize);
}

int dbsColumnCharX(T_TOP_DBS_STMT hStmt, int iCol, char *sBuf, int iSize)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnCharX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnCharX(hStmt, iCol, sBuf, iSize);
}

int dbsColumnStringsX(T_TOP_DBS_STMT hStmt, char *sBuf, int iSize)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnStringsX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnStringsX(hStmt, sBuf, iSize);
}

int dbsColumnCharsX(T_TOP_DBS_STMT hStmt, char *sBuf, int iSize)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnCharsX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnCharsX(hStmt, sBuf, iSize);
}


int dbsColumnBufferX(T_TOP_DBS_STMT hStmt)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsColumnBufferX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsColumnBufferX(hStmt);
}

int  dbsFetchX(T_TOP_DBS_STMT hStmt)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsFetchX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    iRet = f_pfnDbsFetchX(hStmt);
    _DBS_RETURN(iRet);
}

int dbsOperSetNumberLenX(T_TOP_DBS_STMT hStmt, int nLen)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsOperSetNumberLenX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsOperSetNumberLenX(hStmt, nLen);
}

const char *dbsGetFieldX(T_TOP_DBS_STMT hStmt, int iCol, int *piLen)
{
    int iRet = 0;

    if(piLen) {
        *piLen = 0;
    }

    iRet = Init();
    if(iRet < 0) {
        return NULL;
    }

    if(NULL == f_pfnDbsGetFieldX) {
       return NULL;
    }

    // delete by GC 20130502 for it's cause poor performance
    //CheckDBConnected();

    return f_pfnDbsGetFieldX(hStmt, iCol, piLen);
}

int dbsGetIndX(T_TOP_DBS_STMT hStmt, int iCol)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsGetIndX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsGetIndX(hStmt, iCol);
}

int dbsCloseX(T_TOP_DBS_STMT hStmt)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsCloseX) {
       return ERR_TOP_DBS_UNKNOW;
    }

    return f_pfnDbsCloseX(hStmt);
}

int dbsExecStmtX(char *psSql, char * psBindVars[], int iBindCnt, char * psColVars[], int iColCnt)
{
    int iRet = 0;
    T_TOP_DBS_STMT hStmt = -1;
    int i = 0;
    const char *psValue = NULL;
    int iLen = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    /*prepare*/
    iRet =  dbsPrepareX(psSql, &hStmt);
    if(iRet < 0) {
        return iRet;
    }

    /*bind praram*/
    iRet = dbsBindCountX(hStmt);
    if(iRet != iBindCnt) {
        return ERR_TOP_DBS_PARAM;
    }

    iRet = dbsBindArrayX(hStmt, psBindVars, iBindCnt);
    if(iRet < 0) {
        return iRet;
    }

    iRet =  dbsOpenX(hStmt);
    if(iRet < 0) {
        dbsCloseX(hStmt);
        return iRet;
    }

    iRet = dbsColumnBufferX(hStmt);
    if(iRet < 0) {
        dbsCloseX(hStmt);
        return iRet;
    }

    iRet = dbsColumnCountX(hStmt);
    if(iColCnt != iRet) {
        dbsCloseX(hStmt);
        return ERR_TOP_DBS_PARAM;
    }

    if(iColCnt == 0) {
        dbsCloseX(hStmt);
        return 0;
    }

    iRet = dbsFetchX(hStmt);
    if(iRet < 0) {
        dbsCloseX(hStmt);
        return iRet;
    }

    for(i=0; i<iColCnt; i++) {
        iLen = 0;
        psValue = dbsGetFieldX(hStmt, i, &iLen);
        if((psValue != NULL) && (iLen > 0)) {
            memcpy(psColVars[i], psValue, iLen);
        }
    }

    dbsCloseX(hStmt);

    return 0;
}

int  dbsGetFetchArrayX(T_TOP_DBS_STMT hStmt, char * psColVars[], int iColCnt)
{
    int iRet = 0;
    int i = 0;
    const char *psValue = NULL;
    int iLen = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    iRet = dbsColumnCountX(hStmt);
    if(iColCnt != iRet) {
        return ERR_TOP_DBS_PARAM;
    }

    iRet = dbsFetchX(hStmt);
    if(iRet < 0) {
        return iRet;
    }

    for(i=0; i<iColCnt; i++) {
        iLen = 0;
        psValue = dbsGetFieldX(hStmt, i, &iLen);
        if((psValue != NULL) && (iLen > 0)) {
            memcpy(psColVars[i], psValue, iLen);
        }
    }

    return 0;
}

int dbsPreparePageX(const char *sSql, T_TOP_DBS_STMT *phStmt, T_DBS_PAGE *ptPage)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsPreparePageX) {
       return ERR_TOP_DBS_NOT_IMPLEMENTED;
    }

	CheckDBConnected();

    iRet = f_pfnDbsPreparePageX(sSql, phStmt, ptPage);
    f_hStmt = *phStmt;
    _DBS_RETURN(iRet);

    return iRet;
}

int dbsFetchPageX(T_TOP_DBS_STMT hStmt, T_DBS_PAGE *ptPage)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsFetchPageX) {
       return ERR_TOP_DBS_NOT_IMPLEMENTED;
    }

    iRet = f_pfnDbsFetchPageX(hStmt, ptPage);
    _DBS_RETURN(iRet);
}

char *dbsCountSql(const char *sSql, char *sCountSql, int iSize)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return NULL;
    }

    if(NULL == f_pfnDbsCountSql) {
       return NULL;
    }

	CheckDBConnected();

    return f_pfnDbsCountSql(sSql, sCountSql, iSize);
}

int dbsCloseAll(void)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsCloseAll) {
       return ERR_TOP_DBS_NOT_IMPLEMENTED;
    }

    return f_pfnDbsCloseAll();
}

int dbsSetOption(char *sOption, char *sValue)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }

    if(NULL == f_pfnDbsSetOption) {
       return ERR_TOP_DBS_NOT_IMPLEMENTED;
    }

    return f_pfnDbsSetOption(sOption, sValue);
}

int dbsSetCallBack(char *psName, PFN_TOP_DBS_CALLBACK pfnCallBack)
{
    int iRet = 0;

    iRet = Init();
    if(iRet < 0) {
        return iRet;
    }
    
    logMsg(LOG_TRC, "callback[%s] [%p]",  psName, pfnCallBack);
    
    if(strcasecmp(psName, VAR_TOP_DBS_CALLBACK_CONNMISS) == 0) {
        f_pfnConnMissCallBack = pfnCallBack;
    }
    
    return ERR_TOP_DBS_OK;
}

int dbsLog(char *psLogFile, int nLevel, char *psLevelStr, char *psFile, int nLine, const char *psFunction, char *psLogStr, ...)
{
#define LOG_ARG psLogFile,nLevel,psLevelStr,psFile,nLine,psFunction

    va_list tVaList;
    va_start(tVaList, psLogStr);
    logVInfo(LOG_ARG, psLogStr, tVaList);
    va_end(tVaList);

    logMsg(LOG_TRC, "SqlCode[%d] Err[%s]", dbsSqlCode(), dbsErrStr());

    char *psSql = dbsLastSql();
    if ( NULL != psSql && '\0' != psSql[0] ) {
    	logMsg(LOG_TRC, "%s", psSql);
    }

    int iMax = dbsBindCountX(f_hStmt);
    int i = 0;
    char sName[_DLEN_MINI_BUF];
    for (i=0; i<iMax; i++) {
        memset(sName, '\0', sizeof(sName));

        int iLen = 0;
        char *psValue = f_pfnDbsGetBindX(f_hStmt, i, sName, &iLen);
        if ('\0' == sName[0] ) {
            continue;
        }

        if (NULL == psValue) {
            logMsg(LOG_TRC, ":%s is null", sName);
        } else {
            logMsg(LOG_TRC, ":%s = '%.*s'", sName, iLen, psValue);
        }
    }

#undef LOG_ARG
    return 0;
}

/*-------------------------  Local functions ----------------------------*/
int Init(void)
{
    char sDbsLib[_DLEN_MINI_BUF];

    memset(sDbsLib, 0, sizeof(sDbsLib));

    if(f_iIsInit) {
        return 0;
    }

    f_iDbType =0;
    strcpy(sDbsLib, _DBS_LIB_ORA);
    f_hDbsLib = dllOpen(sDbsLib);
    if(NULL == f_hDbsLib) {
        logMsg(LOG_ERR, "dllOpen lib[%s] error[%s]", sDbsLib, dllErrStr());
	logMsg(LOG_TRC, RC_TOP_DBS_DBTYPE "%s^%s", sDbsLib, dllErrStr());
        return ERR_TOP_DBS_INIT;
    }


    /*----------------------------------------------------------*/
    /*log funs*/
    /*----------------------------------------------------------*/
    _DBS_FUN_LOAD(f_pfnDbsSetCfgFile        ,"dbsSetCfgFile");
    _DBS_FUN_LOAD(f_pfnDbsConnect           ,"dbsConnect");
    _DBS_FUN_LOAD(f_pfnDbsBegin             ,"dbsBegin");
    _DBS_FUN_LOAD(f_pfnDbsCommit            ,"dbsCommit");
    _DBS_FUN_LOAD(f_pfnDbsRollback          ,"dbsRollback");
    _DBS_FUN_LOAD(f_pfnDbsDisconnect        ,"dbsDisconnect");
    _DBS_FUN_LOAD(f_pfnDbsSqlCode           ,"dbsSqlCode");
    _DBS_FUN_LOAD(f_pfnDbsSqlcaSave         ,"dbsSqlcaSave");
    _DBS_FUN_LOAD(f_pfnDbsSqlcaRestore      ,"dbsSqlcaRestore");
    _DBS_FUN_LOAD(f_pfnDbsGetErrInfo        ,"dbsGetErrInfo");
    _DBS_FUN_LOAD(f_pfnDbsErrStr            ,"dbsErrStr");
    _DBS_FUN_LOAD(f_pfnDbsGetLastSql        ,"dbsGetLastSql");
    _DBS_FUN_LOAD(f_pfnDbsLastSql           ,"dbsLastSql");
    _DBS_FUN_LOAD(f_pfnDbsExecute           ,"dbsExecute");
    _DBS_FUN_LOAD(f_pfnDbsOperInit          ,"dbsOperInit");
    _DBS_FUN_LOAD(f_pfnDbsOperSetNumberLen  ,"dbsOperSetNumberLen");
    _DBS_FUN_LOAD(f_pfnDbsPrepare           ,"dbsPrepare");
    _DBS_FUN_LOAD(f_pfnDbsBindCount         ,"dbsBindCount");
    _DBS_FUN_LOAD(f_pfnDbsBindName          ,"dbsBindName");
    _DBS_FUN_LOAD(f_pfnDbsBindNull          ,"dbsBindNull");
    _DBS_FUN_LOAD(f_pfnDbsBindString        ,"dbsBindString");
    _DBS_FUN_LOAD(f_pfnDbsBindArray         ,"dbsBindArray");
    _DBS_FUN_LOAD(f_pfnDbsBindSepValues     ,"dbsBindSepValues");
    _DBS_FUN_LOAD(f_pfnDbsOpen              ,"dbsOpen");
    _DBS_FUN_LOAD(f_pfnDbsColumnCount       ,"dbsColumnCount");
    _DBS_FUN_LOAD(f_pfnDbsColumnName        ,"dbsColumnName");
    _DBS_FUN_LOAD(f_pfnDbsColumnLength      ,"dbsColumnLength");
    _DBS_FUN_LOAD(f_pfnDbsColumnString      ,"dbsColumnString");
    _DBS_FUN_LOAD(f_pfnDbsColumnChar        ,"dbsColumnChar");
    _DBS_FUN_LOAD(f_pfnDbsColumnStrings     ,"dbsColumnStrings");
    _DBS_FUN_LOAD(f_pfnDbsColumnChars       ,"dbsColumnChars");
    _DBS_FUN_LOAD(f_pfnDbsColumnBuffer      ,"dbsColumnBuffer");
    _DBS_FUN_LOAD(f_pfnDbsFetch             ,"dbsFetch");
    _DBS_FUN_LOAD(f_pfnDbsGetField          ,"dbsGetField");
    _DBS_FUN_LOAD(f_pfnDbsGetInd            ,"dbsGetInd");
    _DBS_FUN_LOAD(f_pfnDbsClose             ,"dbsClose");

    /*增强版API*/
    _DBS_FUN_LOAD(f_pfnDbsPrepareX           ,"dbsPrepareX");
    _DBS_FUN_LOAD(f_pfnDbsBindCountX         ,"dbsBindCountX");
    _DBS_FUN_LOAD(f_pfnDbsBindNameX          ,"dbsBindNameX");
    _DBS_FUN_LOAD(f_pfnDbsBindNullX          ,"dbsBindNullX");
    _DBS_FUN_LOAD(f_pfnDbsBindStringX        ,"dbsBindStringX");
    _DBS_FUN_LOAD(f_pfnDbsBindArrayX         ,"dbsBindArrayX");
    _DBS_FUN_LOAD(f_pfnDbsBindSepValuesX     ,"dbsBindSepValuesX");
    _DBS_FUN_LOAD(f_pfnDbsOpenX              ,"dbsOpenX");
    _DBS_FUN_LOAD(f_pfnDbsColumnCountX       ,"dbsColumnCountX");
    _DBS_FUN_LOAD(f_pfnDbsColumnNameX        ,"dbsColumnNameX");
    _DBS_FUN_LOAD(f_pfnDbsColumnLengthX      ,"dbsColumnLengthX");
    _DBS_FUN_LOAD(f_pfnDbsColumnStringX      ,"dbsColumnStringX");
    _DBS_FUN_LOAD(f_pfnDbsColumnCharX        ,"dbsColumnCharX");
    _DBS_FUN_LOAD(f_pfnDbsColumnStringsX     ,"dbsColumnStringsX");
    _DBS_FUN_LOAD(f_pfnDbsColumnCharsX       ,"dbsColumnCharsX");
    _DBS_FUN_LOAD(f_pfnDbsColumnBufferX      ,"dbsColumnBufferX");
    _DBS_FUN_LOAD(f_pfnDbsFetchX             ,"dbsFetchX");
    _DBS_FUN_LOAD(f_pfnDbsOperSetNumberLenX  ,"dbsOperSetNumberLenX");
    _DBS_FUN_LOAD(f_pfnDbsGetFieldX          ,"dbsGetFieldX");
    _DBS_FUN_LOAD(f_pfnDbsGetIndX            ,"dbsGetIndX");
    _DBS_FUN_LOAD(f_pfnDbsCloseX             ,"dbsCloseX");

    _DBS_FUN_LOAD_WRN(f_pfnDbsGetBindX       ,"dbsGetBindX");
    _DBS_FUN_LOAD_WRN(f_pfnDbsPreparePageX   ,"dbsPreparePageX");
    _DBS_FUN_LOAD_WRN(f_pfnDbsFetchPageX     ,"dbsFetchPageX");
    _DBS_FUN_LOAD_WRN(f_pfnDbsCountSql       ,"dbsCountSql");

    _DBS_FUN_LOAD_WRN(f_pfnDbsCloseAll       ,"dbsCloseAll");

    _DBS_FUN_LOAD_WRN(f_pfnDbsSetOption      ,"dbsSetOption");

    f_iIsInit = 1;

    return 0;
}
/*-----------------------------  End ------------------------------------*/
int CheckDBConnected()
{
    int iRet = 0;
    char * psArgv[1] = {NULL};
    char sSql[100];

    if( !f_iIsInit || !f_iChkFlag )
    {
        return 0;
    }

    sprintf(sSql, "update tbl_checkdb set checkdb=1 ");
    iRet = f_pfnDbsExecute( sSql );
    if( iRet == ERR_TOP_DBS_DISCONNECT ) {
    	logMsg(LOG_ERR, "CHECK DBConnect error[%d]", iRet );
        f_pfnConnMissCallBack(0, psArgv);
    }

    return 0;

}
