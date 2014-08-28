/*
 *
 *
 * Pager for DB
 *
 *
 * FileName: dbs_page.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include        "common.h"
#include	"top_dbs.h"
#include	"dbs_x.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

/*--------------------------- Macro define ------------------------------*/
#define	_TINY_BUF_LEN	32
#define	_MINI_BUF_LEN	256
#define	_LARGE_BUF_LEN	1024
#define	_HUGE_BUF_LEN	10240

#define	setnull(x)		memset(&(x), 0, sizeof(x))
#define	setpnull(x)		memset(x, 0, sizeof(*(x)))
#define	max(x,y)		((x) > (y) ? (x) : (y))
#define	min(x,y)		((x) < (y) ? (x) : (y))

#define	_				logMsg

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
int
dbsPreparePageX(const char *sSql, T_TOP_DBS_STMT *phStmt, T_DBS_PAGE *ptPage)
{
	char	sPageSql[_HUGE_BUF_LEN];
/*
	snprintf(sPageSql, sizeof(sPageSql),
	         "SELECT * "
	         "  FROM (SELECT t.*, ROWNUM i__d "
	         "          FROM (%s) t "
	         "         WHERE ROWNUM <= %d) "
	         " WHERE i__d >= %d ",
	         sSql, ptPage->iStart + ptPage->iCnt, ptPage->iStart);
*/
	snprintf(sPageSql, sizeof(sPageSql),
	         "SELECT * "
	           "FROM (SELECT t.*, ROWNUM i__d FROM (%s) t) "
	          "WHERE i__d >= %d "
	            "AND i__d < %d ",
	         sSql, ptPage->iStart, ptPage->iStart + ptPage->iCnt);
	ptPage->iOut = 0;
	ptPage->iColumn = 0;

	_(LOG_TRC, "sPageSql[%s]", sPageSql);

	return dbsPrepareX(sPageSql, phStmt);
}

int
dbsFetchPageX(T_TOP_DBS_STMT hStmt, T_DBS_PAGE *ptPage)
{
	int	iRet;

	if (ptPage->iColumn == 0) {
		ptPage->iColumn = dbsColumnCountX(hStmt) - 1;
	}

	iRet = dbsFetchX(hStmt);
	if (iRet == ERR_TOP_DBS_OK) {
		ptPage->iOut++;
	}

	return iRet;
}

char *
dbsCountSql(const char *sSql, char *sCountSql, int iSize)
{
	snprintf(sCountSql, iSize, "SELECT COUNT(1) FROM (%s)", sSql);
	return sCountSql;
}

/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/
