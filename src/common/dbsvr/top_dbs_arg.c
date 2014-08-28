/*
 *
 *
 * Power Dbsvr Function
 *
 *
 * FileName: top_dbs_arg.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include	"top_dbs.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	<stdarg.h>

#include	"util_str.h"

/*--------------------------- Macro define ------------------------------*/
#define	_TINY_BUF_LEN	32
#define	_SMALL_BUF_LEN	256
#define	_LARGE_BUF_LEN	1024
#define	_HUGE_BUF_LEN	10240

#define _TINY_LIST_NUM  64

#define _SQL_FLAG       ':'
#define _SQL_SPLIT      '^'

#define	setnull(x)		memset(&(x), 0, sizeof(x))
#define	max(x,y)		((x) > (y) ? (x) : (y))
#define	min(x,y)		((x) < (y) ? (x) : (y))
#define	and(x,y)		((x) ? (y) : (x))
#define	or(x,y)			((x) ? (x) : (y))

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/
static char * locSqlGetNext(char *psString, int *pnLen);
static int locSqlSetSplit(char *psSplit, int nLen, char *psValue, int *pnReal, int nMax, char **ppsList);
static int locSqlChangeGetValueVaList(char *psSql, int *pnNum, char **ppsValueList, va_list vaList);
static int locSqlChangeGetValueList(char * sSql, int iNum, char **psBind, int *piRealNum, char **ppsRealBind);

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
int
dbsOpenListV(T_TOP_DBS_STMT *phStmt, char *sSql, ...)
{
    int iRet;

    /* Init Value List */
    int iNum = _TINY_LIST_NUM;
    char *ppsValueList[_TINY_LIST_NUM];
    memset(ppsValueList, '\0', sizeof(ppsValueList));

    /* Change Sql, Get Value List */
    va_list vaList;
    va_start(vaList, sSql);
    iRet = locSqlChangeGetValueVaList(sSql, &(iNum), ppsValueList, vaList);
    if ( iRet != 0 ) {
        va_end(vaList);
        return ( iRet );
    }
    va_end(vaList);

    /* Inq Sql */
    iRet = dbsPrepareX(sSql, phStmt);
    if (iRet < 0) {
		return iRet;
	}

    iRet = dbsBindArrayX(*phStmt, ppsValueList, iNum);
    if (iRet < 0) {
		return iRet;
	}

    iRet = dbsOpenX(*phStmt);
    if (iRet < 0) {
		dbsSqlcaSave(); dbsCloseX(*phStmt); dbsSqlcaRestore();
		return iRet;
	}

	if (dbsColumnCountX(*phStmt) > 0) {
		iRet = dbsColumnBufferX(*phStmt);
		if (iRet < 0) {
			dbsSqlcaSave(); dbsCloseX(*phStmt); dbsSqlcaRestore();
			return iRet;
		}
	}

    return ( 0 );
}

int
dbsOpenV(T_TOP_DBS_STMT *phStmt, char *sSql, ...)
{
	va_list	vaList;
	int		iRet, i;

	iRet = dbsPrepareX(sSql, phStmt);
	if (iRet < 0) {
		return iRet;
	}

	va_start(vaList, sSql);
	for (i = 0; i < dbsBindCountX(*phStmt); i++) {
		iRet = dbsBindStringX(*phStmt, i, va_arg(vaList, char *), -1);
		if (iRet < 0) {
			va_end(vaList);
			return iRet;
		}
	}
	va_end(vaList);

	iRet = dbsOpenX(*phStmt);
	if (iRet < 0) {
		dbsSqlcaSave(); dbsCloseX(*phStmt); dbsSqlcaRestore();
		return iRet;
	}

	if (dbsColumnCountX(*phStmt) > 0) {
		iRet = dbsColumnBufferX(*phStmt);
		if (iRet < 0) {
			dbsSqlcaSave(); dbsCloseX(*phStmt); dbsSqlcaRestore();
			return iRet;
		}
	}

	return 0;
}

int
dbsFetchV(T_TOP_DBS_STMT hStmt, ...)
{
	va_list	vaList;
	int		iRet, i;

	iRet = dbsFetchX(hStmt);
	if (iRet < 0) {
		return iRet;
	}

	va_start(vaList, hStmt);
	for (i = 0; i < dbsColumnCountX(hStmt); i++) {
		strcpy(va_arg(vaList, char *),
		       or((char *)dbsGetFieldX(hStmt, i, NULL), ""));
	}
	va_end(vaList);

	return 0;
}

int
dbsCloseV(T_TOP_DBS_STMT hStmt)
{
	return dbsCloseX(hStmt);
}

int
dbsExecuteV(char *sSql, ...)
{
	T_TOP_DBS_STMT	hStmt;
	va_list			vaList;
	int				iRet, i;

	iRet = dbsPrepareX(sSql, &hStmt);
	if (iRet < 0) {
		return iRet;
	}

	va_start(vaList, sSql);
	for (i = 0; i < dbsBindCountX(hStmt); i++) {
		iRet = dbsBindStringX(hStmt, i, va_arg(vaList, char *), -1);
		if (iRet < 0) {
			va_end(vaList);
			return iRet;
		}
	}

	iRet = dbsOpenX(hStmt);
	if (iRet < 0) {
		dbsSqlcaSave(); dbsCloseX(hStmt); dbsSqlcaRestore();
		va_end(vaList);
		return iRet;
	}

	if (dbsColumnCountX(hStmt) > 0) {
		iRet = dbsColumnBufferX(hStmt);
		if (iRet < 0) {
			va_end(vaList);
			dbsSqlcaSave(); dbsCloseX(hStmt); dbsSqlcaRestore();
			return iRet;
		}

		iRet = dbsFetchX(hStmt);
		if (iRet < 0) {
			va_end(vaList);
			dbsSqlcaSave(); dbsCloseX(hStmt); dbsSqlcaRestore();
			return iRet;
		}

		for (i = 0; i < dbsColumnCountX(hStmt); i++) {
			strcpy(va_arg(vaList, char *),
			       or((char *)dbsGetFieldX(hStmt, i, NULL), ""));
		}
	}

	va_end(vaList);
	dbsCloseX(hStmt);
	return 0;
}

int
dbsSqlPrePorcListA(char *sSql, int iBindCnt, char *psBind[], int *piRealBindCnt, char *psRealBind[])
{
    return ( locSqlChangeGetValueList(sSql, iBindCnt, psBind, piRealBindCnt, psRealBind) );
}

int
dbsOpenListA(T_TOP_DBS_STMT *phStmt, char *sSql, int iBindCnt, char *psBind[])
{
    int iRet = 0;

    /* Init Value List */
    int iRealBindCnt = _TINY_LIST_NUM;
    char *psRealBind[_TINY_LIST_NUM];
    memset(psRealBind, '\0', sizeof(psRealBind));

    /* Change Sql, Get Value List */
    iRet = locSqlChangeGetValueList(sSql, iBindCnt, psBind, &(iRealBindCnt), psRealBind);
    if ( iRet != 0 ) {
        return ( iRet );
    }

    /* Inq Sql */
    iRet = dbsPrepareX(sSql, phStmt);
    if (iRet < 0) {
		return iRet;
	}

    iRet = dbsBindArrayX(*phStmt, psRealBind, iRealBindCnt);
    if (iRet < 0) {
		return iRet;
	}

    iRet = dbsOpenX(*phStmt);
    if (iRet < 0) {
		dbsSqlcaSave(); dbsCloseX(*phStmt); dbsSqlcaRestore();
		return iRet;
	}

	if (dbsColumnCountX(*phStmt) > 0) {
		iRet = dbsColumnBufferX(*phStmt);
		if (iRet < 0) {
			dbsSqlcaSave(); dbsCloseX(*phStmt); dbsSqlcaRestore();
			return iRet;
		}
	}

    return ( 0 );
}

int
dbsOpenA(T_TOP_DBS_STMT *phStmt, char *sSql, int iBindCnt, char *psBind[])
{
	int		iRet, i;

	iRet = dbsPrepareX(sSql, phStmt);
	if (iRet < 0) {
		return iRet;
	}

	for (i = 0; i < dbsBindCountX(*phStmt); i++) {
		iRet = dbsBindStringX(*phStmt, i, i < iBindCnt ? psBind[i] : "", -1);
		if (iRet < 0) {
			return iRet;
		}
	}

	iRet = dbsOpenX(*phStmt);
	if (iRet < 0) {
		dbsSqlcaSave(); dbsCloseX(*phStmt); dbsSqlcaRestore();
		return iRet;
	}

	if (dbsColumnCountX(*phStmt) > 0) {
		iRet = dbsColumnBufferX(*phStmt);
		if (iRet < 0) {
			dbsSqlcaSave(); dbsCloseX(*phStmt); dbsSqlcaRestore();
			return iRet;
		}
	}

	return 0;
}

int
dbsFetchA(T_TOP_DBS_STMT hStmt, int iSelectCnt, char *psSelect[])
{
	int	iRet, i;

	iRet = dbsFetchX(hStmt);
	if (iRet < 0) {
		return iRet;
	}

	for (i = 0; i < iSelectCnt; i++) {
		strcpy(psSelect[i], i < dbsColumnCountX(hStmt) ?
		       or((char *)dbsGetFieldX(hStmt, i, NULL), "") : "");
	}

	return 0;
}

int
dbsCloseA(T_TOP_DBS_STMT hStmt)
{
	return dbsCloseX(hStmt);
}

int
dbsExecuteA(char *sSql, int iBindCnt, char *psBind[],
            int iSelectCnt, char *psSelect[])
{
	T_TOP_DBS_STMT	hStmt;
	int				iRet, i;

	iRet = dbsPrepareX(sSql, &hStmt);
	if (iRet < 0) {
		return iRet;
	}

	for (i = 0; i < dbsBindCountX(hStmt); i++) {
		iRet = dbsBindStringX(hStmt, i, i < iBindCnt ? psBind[i] : "", -1);
		if (iRet < 0) {
			return iRet;
		}
	}

	iRet = dbsOpenX(hStmt);
	if (iRet < 0) {
		dbsSqlcaSave(); dbsCloseX(hStmt); dbsSqlcaRestore();
		return iRet;
	}

	if (dbsColumnCountX(hStmt) > 0) {
		iRet = dbsColumnBufferX(hStmt);
		if (iRet < 0) {
			dbsSqlcaSave(); dbsCloseX(hStmt); dbsSqlcaRestore();
			return iRet;
		}

		iRet = dbsFetchX(hStmt);
		if (iRet < 0) {
			dbsSqlcaSave(); dbsCloseX(hStmt); dbsSqlcaRestore();
			return iRet;
		}

		for (i = 0; i < iSelectCnt; i++) {
			strcpy(psSelect[i], i < dbsColumnCountX(hStmt) ?
			       or((char *)dbsGetFieldX(hStmt, i, NULL), "") : "");
		}
	}

	dbsCloseX(hStmt);
	return 0;
}

int
dbsCountA(const char *sSql, int iBindCnt, char *psBind[])
{
	char	sCountSql[_HUGE_BUF_LEN];
	char	sCount[_TINY_BUF_LEN], *pArray[] = {sCount};
	int		iRet;

	iRet = dbsExecuteA(dbsCountSql(sSql, sCountSql, sizeof(sCountSql)),
	                   iBindCnt, psBind, 1, pArray);
	if (iRet < 0) return iRet;

	return atoi(sCount);
}

T_DBS_TABLE *
dbsAnalyzeTable(char *sTable)
{
	char	sSql[_SMALL_BUF_LEN];

	snprintf(sSql, sizeof(sSql), "select * from %s where 0 = 1", sTable);

	T_TOP_DBS_STMT	hStmt;
	int				iRet;

	iRet = dbsPrepareX(sSql, &hStmt);
	if (iRet < 0) return NULL;

	iRet = dbsOpenX(hStmt);
	if (iRet < 0) return NULL;

	int	iCnt;

	iCnt = dbsColumnCountX(hStmt);

	T_DBS_TABLE	*ptTable;

	ptTable = malloc(sizeof(*ptTable) + sizeof(ptTable->psCol[0]) * (iCnt + 1));
	if (!ptTable) {
		dbsSqlcaSave(); dbsCloseX(hStmt); dbsSqlcaRestore();
		return NULL;
	}
	ptTable->iCnt = iCnt;

	int	i;

	for (i = 0; i < ptTable->iCnt; i++) {
		dbsColumnNameX(hStmt, i, ptTable->psCol[i]);
	}

	dbsCloseX(hStmt);
	return ptTable;
}

int
locSqlChangeGetValueList(char * sSql, int iNum, char **psBind, int *piRealNum, char **ppsRealBind)
{
    int iRet = 0;

    int i = 0;
    int j = 0;
    char *p = sSql;
    while ( 1 ) {
        /* Get Next Sql Pater */
        int nLen = 0;
        p = locSqlGetNext(p, &(nLen));
        if ( '\0' == *p ) {
            break;
        }

        /* Check Num */
        if ( j >= iNum ) {
            return ( ERR_TOP_DBS_PARAM );
        }

        /* Add value Len */
        iRet = locSqlSetSplit(p, nLen, psBind[j], &(i), *piRealNum, ppsRealBind);
        if ( iRet != 0 ) {
            return ( iRet );
        }

        /* Check Next */
        p += nLen;
        ++j;
    }

    /* Set Real Num */
    *piRealNum = i;

    return ( 0 );
}

int
locSqlChangeGetValueVaList(char *sSql, int *piRealNum, char **ppsRealBind, va_list vaList)
{
    int iRet = 0;

    int i = 0;
    char *p = sSql;
    while ( 1 ) {
        /* Get Next Sql Pater */
        int nLen = 0;
        p = locSqlGetNext(p, &(nLen));
        if ( '\0' == *p ) {
            break;
        }

        /* Add value Len */
        iRet = locSqlSetSplit(p, nLen, va_arg(vaList, char *), &(i), *piRealNum, ppsRealBind);
        if ( iRet != 0 ) {
            return ( iRet );
        }

        /* Check Next */
        p += nLen;
    }

    /* Set Real Num */
    *piRealNum = i;

    return ( 0 );
}

char *
locSqlGetNext(char *psString, int *pnLen)
{
    /* Check String */
    char *p = psString;
    int i = 0;
    int nFlag = 0;
    for ( ; p[i]!='\0'; ++i ) {
        /* Find value Char */
        if ( _SQL_FLAG == p[i] ) {
            nFlag = 1;
            continue;
        }

        /* Normal Char */
        if ( _SQL_SPLIT != p[i] ) {
            continue;
        }

        /* Find Split Char */
        p[i] = ' ';
        if ( 1 == nFlag ) {
            break;
        }

        /* Get Next Split */
        p += i + 1;
        i = 0;
    }

    /* Check Is End */
    if ( 1 != nFlag ) {
        p += i;
        i = 0;
    }

    /* Set Return */
    *pnLen = i;

    return ( p );
}

int
locSqlSetSplit(char *psSplit, int nLen, char *psValue, int *pnReal, int nMax, char **ppsList)
{
    /* No Value, Set Blank */
    if ( NULL == psValue || '\0' == psValue[0] ) {
        memset(psSplit, ' ', nLen);
        return (0);
    }

    /* Check Value Max Num */
    if ( *pnReal >= nMax ) {
        return ( ERR_TOP_DBS_PARAM );
    }

    /* Add Value To List */
    ppsList[*pnReal] = psValue;
    ++(*pnReal);

    return ( 0 );
}

int
dbsGetFieldXS(T_TOP_DBS_STMT hStmt, int iCol, char *sBuf, int iSize)
{
	const char	*pField;

	pField = dbsGetFieldX(hStmt, iCol, NULL);
	if (!pField) {
		return -1;
	}

	strMem2Str(sBuf, iSize, (char *)pField, -1);
	return 0;
}

/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/
