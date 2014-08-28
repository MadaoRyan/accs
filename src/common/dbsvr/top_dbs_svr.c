/*
 *
 *
 * interface for dbsvr
 *
 *
 * FileName: top_dbs_svr.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include	"top_dbs.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"util_str.h"

/*--------------------------- Macro define ------------------------------*/
#define	_TINY_BUF_LEN	32
#define	_MINI_BUF_LEN	256
#define	_LARGE_BUF_LEN	1024
#define	_HUGE_BUF_LEN	10240

#define	setnull(t)		memset(&(t), 0, sizeof(t))
#define	setpnull(p)		memset(p, 0, sizeof(*(p)))
#define	max(x,y)		((x) > (y) ? (x) : (y))
#define	min(x,y)		((x) < (y) ? (x) : (y))
#define	arrayof(a)		(sizeof(a) / sizeof(*(a)))

#define dbsCheck(r)         \
do {                        \
	if ((r) < 0) {          \
		dbsSqlcaSave();     \
		dbsCloseX(hStmt);   \
		dbsSqlcaRestore();  \
		return (r);         \
	}                       \
} while (0)

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
int
dbsSvrFind(char *sSql, int iCol, T_DBS_SVR_COL *ptCol)
{
	T_TOP_DBS_STMT	hStmt;
	int				iRet, i, j;

	iRet = dbsPrepareX(sSql, &hStmt);
	dbsCheck(iRet);

	j = 0;
	for (i = 0; i < iCol; i++) {
		if (ptCol[i].iKey) {
			iRet = dbsBindStringX(hStmt, j, ptCol[i].sBuf,
			                      strLength(ptCol[i].sBuf, ptCol[i].iSize - 1));
			dbsCheck(iRet);
			++j;
		}
	}

	iRet = dbsOpenX(hStmt);
	dbsCheck(iRet);

	iRet = dbsColumnBufferX(hStmt);
	dbsCheck(iRet);

	iRet = dbsFetchX(hStmt);
	dbsCheck(iRet);

	for (i = 0; i < iCol; i++) {
		iRet = dbsGetFieldXS(hStmt, i, ptCol[i].sBuf, ptCol[i].iSize);
		dbsCheck(iRet);
	}

	dbsCloseX(hStmt);

	return 0;
}

int
dbsSvrLock(T_TOP_DBS_STMT *phStmt, char *sSql, int iCol, T_DBS_SVR_COL *ptCol)
{
	T_TOP_DBS_STMT	hStmt;
	int				iRet, i, j;

	*phStmt = -1;

	iRet = dbsPrepareX(sSql, &hStmt);
	dbsCheck(iRet);

	j = 0;
	for (i = 0; i < iCol; i++) {
		if (ptCol[i].iKey) {
			iRet = dbsBindStringX(hStmt, j, ptCol[i].sBuf,
			                      strLength(ptCol[i].sBuf, ptCol[i].iSize - 1));
			dbsCheck(iRet);
			++j;
		}
	}

	iRet = dbsOpenX(hStmt);
	dbsCheck(iRet);

	iRet = dbsColumnBufferX(hStmt);
	dbsCheck(iRet);

	iRet = dbsFetchX(hStmt);
	dbsCheck(iRet);

	for (i = 0; i < iCol; i++) {
		iRet = dbsGetFieldXS(hStmt, i, ptCol[i].sBuf, ptCol[i].iSize);
		dbsCheck(iRet);
	}

	dbsCloseX(hStmt);
	return 0;
}

int
dbsSvrClose(T_TOP_DBS_STMT hStmt)
{
	return 0;
}

int
dbsSvrUpdate(char *sSql, int iCol, T_DBS_SVR_COL *ptCol)
{
	T_TOP_DBS_STMT	hStmt;
	int				iRet, i, j;

	iRet = dbsPrepareX(sSql, &hStmt);
	dbsCheck(iRet);

	for (i = 0; i < iCol; i++) {
		iRet = dbsBindStringX(hStmt, i, ptCol[i].sBuf,
		                      strLength(ptCol[i].sBuf, ptCol[i].iSize - 1));
		dbsCheck(iRet);
	}

	j = iCol;
	for (i = 0; i < iCol; i++) {
		if (ptCol[i].iKey) {
			iRet = dbsBindStringX(hStmt, j++, ptCol[i].sBuf,
			                      strLength(ptCol[i].sBuf, ptCol[i].iSize - 1));
			dbsCheck(iRet);
		}
	}

	iRet = dbsOpenX(hStmt);
	dbsCheck(iRet);

	dbsCloseX(hStmt);

	return 0;
}

int
dbsSvrDelete(char *sSql, int iCol, T_DBS_SVR_COL *ptCol)
{
	T_TOP_DBS_STMT	hStmt;
	int				iRet, i, j;

	iRet = dbsPrepareX(sSql, &hStmt);
	dbsCheck(iRet);

	j = 0;
	for (i = 0; i < iCol; i++) {
		if (ptCol[i].iKey) {
			iRet = dbsBindStringX(hStmt, j, ptCol[i].sBuf,
			                      strLength(ptCol[i].sBuf, ptCol[i].iSize - 1));
			dbsCheck(iRet);
			++j;
		}
	}

	iRet = dbsOpenX(hStmt);
	dbsCheck(iRet);

	dbsCloseX(hStmt);

	return 0;
}

int
dbsSvrInsert(char *sSql, int iCol, T_DBS_SVR_COL *ptCol)
{
	T_TOP_DBS_STMT	hStmt;
	int				iRet, i;

	iRet = dbsPrepareX(sSql, &hStmt);
	dbsCheck(iRet);

	for (i = 0; i < iCol; i++) {
		iRet = dbsBindStringX(hStmt, i, ptCol[i].sBuf,
		                      strLength(ptCol[i].sBuf, ptCol[i].iSize - 1));
		dbsCheck(iRet);
	}

	iRet = dbsOpenX(hStmt);
	dbsCheck(iRet);

	dbsCloseX(hStmt);

	return 0;
}

/*-------------------------  Local functions ----------------------------*/


/*-----------------------------  End ------------------------------------*/
