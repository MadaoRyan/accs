
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[9];
};
static struct sqlcxp sqlfpn =
{
    8,
    "dbs_2.pc"
};


static unsigned int sqlctx = 17915;


static struct sqlexd {
   unsigned long  sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
            short *cud;
   unsigned char  *sqlest;
            char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
   unsigned char  **sqphsv;
   unsigned long  *sqphsl;
            int   *sqphss;
            short **sqpind;
            int   *sqpins;
   unsigned long  *sqparm;
   unsigned long  **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
            int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
   unsigned char  *sqhstv[1];
   unsigned long  sqhstl[1];
            int   sqhsts[1];
            short *sqindv[1];
            int   sqinds[1];
   unsigned long  sqharm[1];
   unsigned long  *sqharc[1];
   unsigned short  sqadto[1];
   unsigned short  sqtdso[1];
} sqlstm = {12,1};

/* SQLLIB Prototypes */
extern sqlcxt (/*_ void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * _*/);
extern sqlcx2t(/*_ void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * _*/);
extern sqlbuft(/*_ void **, char * _*/);
extern sqlgs2t(/*_ void **, char * _*/);
extern sqlorat(/*_ void **, unsigned int *, void * _*/);

/* Forms Interface */
static int IAPSUCC = 0;
static int IAPFAIL = 1403;
static int IAPFTL  = 535;
extern void sqliem(/*_ char *, int * _*/);

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* CUD (Compilation Unit Data) Array */
static short sqlcud0[] =
{12,4130,852,0,0,
5,0,0,1,0,0,17,136,0,0,1,1,0,1,0,1,97,0,0,
24,0,0,1,0,0,19,150,0,0,1,1,0,1,0,3,32,0,0,
43,0,0,1,0,0,11,303,0,0,1,1,0,1,0,1,32,0,0,
62,0,0,1,0,0,20,310,0,0,1,1,0,1,0,3,32,0,0,
81,0,0,1,0,0,15,314,0,0,0,0,0,1,0,
96,0,0,1,0,0,15,322,0,0,0,0,0,1,0,
111,0,0,1,0,0,14,497,0,0,1,0,0,1,0,2,32,0,0,
130,0,0,1,0,0,15,537,0,0,0,0,0,1,0,
};


/*
 *
 * 
 * dbs 2
 * 
 * 
 * FileName: dbs_2.c
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* EXEC SQL INCLUDE sqlca;
 */ 
/*
 * $Header: sqlca.h 24-apr-2003.12:50:58 mkandarp Exp $ sqlca.h 
 */

/* Copyright (c) 1985, 2003, Oracle Corporation.  All rights reserved.  */
 
/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:
 
    #define SQLCA_STORAGE_CLASS extern
 
  will define the SQLCA as an extern.
 
  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.
 
MODIFIED
    lvbcheng   07/31/98 -  long to int
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   08/11/92 -  No sqlca var if SQLCA_NONE macro set 
  Clare      12/06/84 - Ch SQLCA to not be an extern.
  Clare      10/21/85 - Add initialization.
  Bradbury   01/05/86 - Only initialize when SQLCA_INIT set
  Clare      06/12/86 - Add SQLCA_STORAGE_CLASS option.
*/
 
#ifndef SQLCA
#define SQLCA 1
 
struct   sqlca
         {
         /* ub1 */ char    sqlcaid[8];
         /* b4  */ int     sqlabc;
         /* b4  */ int     sqlcode;
         struct
           {
           /* ub2 */ unsigned short sqlerrml;
           /* ub1 */ char           sqlerrmc[70];
           } sqlerrm;
         /* ub1 */ char    sqlerrp[8];
         /* b4  */ int     sqlerrd[6];
         /* ub1 */ char    sqlwarn[8];
         /* ub1 */ char    sqlext[8];
         };

#ifndef SQLCA_NONE 
#ifdef   SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
         struct sqlca sqlca
#endif
 
#ifdef  SQLCA_INIT
         = {
         {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
         sizeof(struct sqlca),
         0,
         { 0, {0}},
         {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
         }
#endif
         ;
#endif
 
#endif
 
/* end SQLCA */

#define TOP_DBS_NUM_SUBFIX(_STR) _STR##2

/* EXEC SQL INCLUDE  "dbs_x.pc";
 */ 
/*
 *
 *
 * Database Oracle Common Function
 *
 *
 * FileName: dbs_oraoper.pc
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */

/*--------------------------- Include files -----------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlca.h"
#include "sqlda.h"
#include "sqlcpr.h"
#include "common.h"

#include "glb_dbs.h"
#include "log_info.h"

/*--------------------------- Macro define ------------------------------*/
#define	_TINY_BUF_LEN	32
#define	_MINI_BUF_LEN	256
#define	_LARGE_BUF_LEN	1024
#define	_HUGE_BUF_LEN	10240

#define	DBS_ORA_TYPE_VARCHAR2	1
#define	DBS_ORA_TYPE_NUMBER		2
#define	DBS_ORA_TYPE_STRING		5
#define	DBS_ORA_TYPE_DATE		12
#define	DBS_ORA_TYPE_CHAR		96
#define	DBS_ORA_TYPE_ROWID		104

#define	DBS_ORA_IS_NOT_NULL		0
#define	DBS_ORA_IS_NULL			-1

#define	DBS_ORA_STATE_CLOSED	0
#define	DBS_ORA_STATE_PREPARED	1
#define	DBS_ORA_STATE_OPENED	2
#define	DBS_ORA_STATE_FETCHED	3

#define	min(x,y)				((x) < (y) ? (x) : (y))
#define	_						logMsg

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/
static SQLDA	*dbsAlloc(int iMaxVarNum, int iMaxNameLen);
static int		dbsFree(SQLDA *des);
static int		dbsGetNext(char sSep, const char *pBegin, const char *pEnd);
static int		dbsGetLen(int iType, int iLen);
extern int		dbsSqlCode2DbsCode(int iSqlCode);

/*-------------------------  Global variable ----------------------------*/
static SQLDA	*f_desBind = NULL;
static SQLDA	*f_desSelect = NULL;
static int		f_iMaxVarNum;

static int		f_iState;

static char		*f_pBuffer;
static int		f_iSize;
static int     f_iNumberLen = 30;

/*-------------------------  Global functions ---------------------------*/
int TOP_DBS_NUM_SUBFIX(dbsOperInit)(int iMaxVarNum, int iMaxNameLen)
{
	dbsFree(f_desBind);
	f_desBind = dbsAlloc(iMaxVarNum, iMaxNameLen);
	if (!f_desBind) {
		return ERR_TOP_DBS_SPACE;
	}

	dbsFree(f_desSelect);
	f_desSelect = dbsAlloc(iMaxVarNum, iMaxNameLen);
	if (!f_desSelect) {
		dbsFree(f_desBind);
		return ERR_TOP_DBS_SPACE;
	}

	f_iMaxVarNum = iMaxVarNum;

	f_iState = DBS_ORA_STATE_CLOSED;

	free(f_pBuffer);
	f_pBuffer = NULL;
	f_iSize = 0;

	return 0;
}


int TOP_DBS_NUM_SUBFIX(dbsOperFree)(void)
{
	int	iRet;

	iRet = dbsFree(f_desBind);
	if (iRet < 0) {
		return ERR_TOP_DBS_UNKNOW;
	}

	iRet = dbsFree(f_desSelect);
	if (iRet < 0) {
		return ERR_TOP_DBS_UNKNOW;
	}

	free(f_pBuffer);

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsPrepare)(const char *sSql)
{
	int	iRet;

	if (f_iState != DBS_ORA_STATE_PREPARED &&
		f_iState != DBS_ORA_STATE_CLOSED) {
		return ERR_TOP_DBS_STAT;
	}

	if (!f_desBind || !f_desSelect) {
		iRet = dbsOperInit(_MINI_BUF_LEN, _MINI_BUF_LEN);
		if (iRet < 0) {
			return ERR_TOP_DBS_INIT;
		}
	}

	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		char sTmp[1025*6];
	/* EXEC SQL END DECLARE SECTION; */ 

	strcpy(sTmp,sSql);
	/* EXEC SQL PREPARE f_stmt FROM :sTmp; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)sTmp;
 sqlstm.sqhstl[0] = (unsigned long )6150;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	if ((iRet = sqlca.sqlcode) != DB_OK) {
		_(LOG_BUG, "sql[%s] sqlcode[%d]", sSql, sqlca.sqlcode);
		return ERR_TOP_DBS_PREPARE;
	}

	/* EXEC SQL DECLARE f_cur CURSOR FOR f_stmt; */ 

	if ((iRet = sqlca.sqlcode) != DB_OK) {
		_(LOG_BUG, "sqlcode[%d]", sqlca.sqlcode);
		return ERR_TOP_DBS_DECLARE;
	}

	f_desBind->N = f_iMaxVarNum;

	/* EXEC SQL DESCRIBE BIND VARIABLES FOR f_stmt INTO f_desBind; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )24;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)f_desBind;
 sqlstm.sqhstl[0] = (unsigned long )0;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	if ((iRet = sqlca.sqlcode) != DB_OK) {
		_(LOG_BUG, "sqlcode[%d]", sqlca.sqlcode);
		return ERR_TOP_DBS_ANALYSQL;
	}

	if (f_desBind->F < 0 || f_desBind->N < f_desBind->F) {
		_(LOG_BUG, "sqlcode[%d]", sqlca.sqlcode);
		return ERR_TOP_DBS_ANALYSQL;
	}

	f_desBind->N = f_desBind->F;

	f_iState = DBS_ORA_STATE_PREPARED;

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsBindCount)(void)
{
/*
	if (f_iState != DBS_ORA_STATE_PREPARED) {
		return ERR_TOP_DBS_STAT;
	}
*/

	return f_desBind->F;
}

char * TOP_DBS_NUM_SUBFIX(dbsBindName)(int iCol, char *sName)
{
	if (f_iState != DBS_ORA_STATE_PREPARED ||
		iCol < 0 || iCol >= f_desBind->F) {
		return NULL;
	}

	memcpy(sName, f_desBind->S[iCol], f_desBind->M[iCol]);
	sName[f_desBind->C[iCol]] = '\0';

	return sName;
}

const char * TOP_DBS_NUM_SUBFIX(dbsGetBind)(int iCol, char *sName, int *piLen)
{
	if (iCol < 0 || iCol >= f_desSelect->F) {
		return NULL;
	}

	if (piLen != NULL) {
        if ( DBS_ORA_IS_NULL == *(f_desBind->I[iCol]) ) {
            *piLen = 0;
        } else {
            *piLen = f_desBind->L[iCol];
        }
	}
    
    if ( sName != NULL ) {
        memcpy(sName, f_desBind->S[iCol], f_desBind->M[iCol]);
        sName[f_desBind->C[iCol]] = '\0';
    }
    
    if ( DBS_ORA_IS_NULL == *(f_desBind->I[iCol]) ) {
        return "";
    }

	return f_desBind->V[iCol];
}

int TOP_DBS_NUM_SUBFIX(dbsBindNull)(int iCol)
{
	if (f_iState != DBS_ORA_STATE_PREPARED ||
		iCol < 0 || iCol >= f_desBind->F) {
		return ERR_TOP_DBS_STAT;
	}

	*(f_desBind->I[iCol]) = DBS_ORA_IS_NULL;

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsBindString)(int iCol, const char *sStr, int iLen)
{
	if (f_iState != DBS_ORA_STATE_PREPARED ||
		iCol < 0 || iCol >= f_desBind->F) {
		return ERR_TOP_DBS_STAT;
	}

	f_desBind->V[iCol] = (char *)sStr;
	f_desBind->L[iCol] = iLen < 0 ? strlen(sStr) : iLen;
	*(f_desBind->I[iCol]) = DBS_ORA_IS_NOT_NULL;
	f_desBind->T[iCol] = DBS_ORA_TYPE_CHAR;

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsBindArray)(char *asArray[], int iCnt)
{
	int	i;

	if (f_iState != DBS_ORA_STATE_PREPARED) {
		return ERR_TOP_DBS_STAT;
	}

	iCnt = min(iCnt, f_desBind->F);
	for (i = 0; i < iCnt; i++) {
		f_desBind->V[i] = asArray[i];
		f_desBind->L[i] = strlen(asArray[i]);
		*(f_desBind->I[i]) = DBS_ORA_IS_NOT_NULL;
		f_desBind->T[i] = DBS_ORA_TYPE_CHAR;
	}

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsBindSepValues)(char sSep, const char *sBuf, int iLen)
{
	const char	*pCur, *pEnd;
	int			i = 0;

	if (f_iState != DBS_ORA_STATE_PREPARED) {
		return ERR_TOP_DBS_STAT;
	}

	iLen = iLen < 0 ? strlen(sBuf) : iLen;
	pCur = sBuf;
	pEnd = sBuf + iLen;

	while ((iLen = dbsGetNext(sSep, pCur, pEnd)) >= 0) {
		if (i > f_desBind->F) {
			return ERR_TOP_DBS_PARAM;
		}

		f_desBind->V[i] = (char *)pCur;
		f_desBind->L[i] = iLen;
		*(f_desBind->I[i]) = DBS_ORA_IS_NOT_NULL;
		f_desBind->T[i] = DBS_ORA_TYPE_CHAR;

		pCur += iLen + 1;
		i++;
	}

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsOpen)(void)
{
	struct sqlca	sqlcaSave;
	int				iRet, iNullOk, i;

	if (f_iState != DBS_ORA_STATE_PREPARED) {
		return ERR_TOP_DBS_STAT;
	}

	/* EXEC SQL OPEN f_cur USING DESCRIPTOR f_desBind; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )43;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (unsigned char  *)f_desBind;
 sqlstm.sqhstl[0] = (unsigned long )0;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	if ((iRet = sqlca.sqlcode) != DB_OK) {
	    return dbsSqlCode2DbsCode(sqlca.sqlcode);
	}

	f_desSelect->N = f_iMaxVarNum;

	/* EXEC SQL DESCRIBE SELECT LIST FOR f_stmt INTO f_desSelect; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )62;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)f_desSelect;
 sqlstm.sqhstl[0] = (unsigned long )0;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	if ((iRet = sqlca.sqlcode) != DB_OK) {
		_(LOG_BUG, "sqlcode[%d]", sqlca.sqlcode);
		sqlcaSave = sqlca;
		/* EXEC SQL CLOSE f_cur; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 1;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )81;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		sqlca = sqlcaSave;
		return ERR_TOP_DBS_ANALYSQL;
	}

	if (f_desSelect->F < 0 || f_desSelect->N < f_desSelect->F) {
		_(LOG_BUG, "f_desSelect->N[%d] f_desSelect->F[%d]", f_desSelect->N, f_desSelect->F);
		sqlcaSave = sqlca;
		/* EXEC SQL CLOSE f_cur; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 1;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )96;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		sqlca = sqlcaSave;
		return ERR_TOP_DBS_ANALYSQL;
	}

	f_desSelect->N = f_desSelect->F;

	for (i = 0; i < f_desSelect->F; i++) {
		sqlnul((unsigned short *)(f_desSelect->T + i),
		       (unsigned short *)(f_desSelect->T + i), &iNullOk);

		f_desSelect->L[i] = dbsGetLen(f_desSelect->T[i], f_desSelect->L[i]);
	}

	f_iState = DBS_ORA_STATE_OPENED;

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsColumnCount)(void)
{
	if (f_iState != DBS_ORA_STATE_OPENED &&
		f_iState != DBS_ORA_STATE_FETCHED) {
		return ERR_TOP_DBS_STAT;
	}

	return f_desSelect->F;
}

char * TOP_DBS_NUM_SUBFIX(dbsColumnName)(int iCol, char *sName)
{
	if ((f_iState != DBS_ORA_STATE_OPENED &&
		f_iState != DBS_ORA_STATE_FETCHED) ||
		iCol < 0 || iCol >= f_desSelect->F) {
		return NULL;
	}

	memcpy(sName, f_desSelect->S[iCol], f_desSelect->C[iCol]);
	sName[f_desSelect->C[iCol]] = '\0';

	return sName;
}

int TOP_DBS_NUM_SUBFIX(dbsColumnLength)(int iCol)
{
	if ((f_iState != DBS_ORA_STATE_OPENED &&
		f_iState != DBS_ORA_STATE_FETCHED) ||
		iCol < 0 || iCol >= f_desSelect->F) {
		return ERR_TOP_DBS_STAT;
	}

	return f_desSelect->L[iCol];
}

int TOP_DBS_NUM_SUBFIX(dbsColumnString)(int iCol, char *sBuf, int iSize)
{
	if (f_iState != DBS_ORA_STATE_OPENED ||
		iCol < 0 || iCol >= f_desSelect->F) {
		return ERR_TOP_DBS_STAT;
	}

	f_desSelect->V[iCol] = sBuf;
	f_desSelect->L[iCol] = iSize;
	f_desSelect->T[iCol] = DBS_ORA_TYPE_STRING;

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsColumnChar)(int iCol, char *sBuf, int iSize)
{
	if (f_iState != DBS_ORA_STATE_OPENED ||
		iCol < 0 || iCol >= f_desSelect->F) {
		return ERR_TOP_DBS_STAT;
	}

	f_desSelect->V[iCol] = sBuf;
	f_desSelect->L[iCol] = iSize;
	f_desSelect->T[iCol] = DBS_ORA_TYPE_CHAR;

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsColumnStrings)(char *sBuf, int iSize)
{
	int		i;
	char	*pCur, *pEnd;

	if (f_iState != DBS_ORA_STATE_OPENED) {
		return ERR_TOP_DBS_STAT;
	}

	pCur = sBuf;
	pEnd = sBuf + iSize;

	for (i = 0; i < f_desSelect->F; i++) {
		f_desSelect->V[i] = pCur;
		f_desSelect->L[i]++;
		f_desSelect->T[i] = DBS_ORA_TYPE_STRING;

		pCur += f_desSelect->L[i];
		if (pCur > pEnd) {
			for (/* noop */; i >= 0; i--) {
				f_desSelect->L[i]--;
			}
			return ERR_TOP_DBS_SPACE;
		}
	}

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsColumnChars)(char *sBuf, int iSize)
{
	int		i;
	char	*pCur, *pEnd;

	if (f_iState != DBS_ORA_STATE_OPENED) {
		return ERR_TOP_DBS_STAT;
	}

	pCur = sBuf;
	pEnd = sBuf + iSize;

	for (i = 0; i < f_desSelect->F; i++) {
		f_desSelect->V[i] = pCur;
		f_desSelect->T[i] = DBS_ORA_TYPE_CHAR;

		pCur += f_desSelect->L[i];
		if (pCur > pEnd) {
			return ERR_TOP_DBS_SPACE;
		}
	}

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsColumnBuffer)(void)
{
	char	*pBuffer;
	int		iRet, i, iSize;

	if (f_iState != DBS_ORA_STATE_OPENED) {
		return ERR_TOP_DBS_STAT;
	}

	for (i = iSize = 0; i < f_desSelect->F; i++) {
		iSize += f_desSelect->L[i] + 1;
	}

	if (f_iSize < iSize) {
		pBuffer = realloc(f_pBuffer, iSize);
		if (pBuffer == NULL) {
			return ERR_TOP_DBS_SPACE;
		}
		f_pBuffer = pBuffer;
		f_iSize = iSize;
	}

	iRet = TOP_DBS_NUM_SUBFIX(dbsColumnStrings)(f_pBuffer, f_iSize);
	if (iRet < 0) {
		return iRet;
	}

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsFetch)(void)
{
	int	iRet;

	if (f_iState != DBS_ORA_STATE_OPENED &&
		f_iState != DBS_ORA_STATE_FETCHED) {
		return ERR_TOP_DBS_STAT;
	}

	/* EXEC SQL FETCH f_cur USING DESCRIPTOR f_desSelect; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )111;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqfoff = (         int )0;
 sqlstm.sqfmod = (unsigned int )2;
 sqlstm.sqhstv[0] = (unsigned char  *)f_desSelect;
 sqlstm.sqhstl[0] = (unsigned long )0;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	if ((iRet = sqlca.sqlcode) != DB_OK) {
	    return dbsSqlCode2DbsCode(sqlca.sqlcode);
	}

	f_iState = DBS_ORA_STATE_FETCHED;

	return 0;
}

const char * TOP_DBS_NUM_SUBFIX(dbsGetField)(int iCol, int *piLen)
{
	if (f_iState != DBS_ORA_STATE_FETCHED ||
		iCol < 0 || iCol >= f_desSelect->F) {
		return NULL;
	}

	if (piLen != NULL) {
		*piLen = f_desSelect->L[iCol];
	}

	return f_desSelect->V[iCol];
}

int TOP_DBS_NUM_SUBFIX(dbsGetInd)(int iCol)
{
	if (f_iState != DBS_ORA_STATE_FETCHED ||
		iCol < 0 || iCol >= f_desSelect->F) {
		return ERR_TOP_DBS_STAT;
	}

	return *(f_desSelect->I[iCol]);
}

int TOP_DBS_NUM_SUBFIX(dbsClose)(void)
{
	int	iRet;

	if (f_iState == DBS_ORA_STATE_OPENED ||
		f_iState == DBS_ORA_STATE_FETCHED) {
		/* EXEC SQL CLOSE f_cur; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 1;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )130;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		if ((iRet = sqlca.sqlcode) != DB_OK) {
			/*return ERR_TOP_DBS_STAT;*/
		}
	}

	f_iState = DBS_ORA_STATE_CLOSED;

	return 0;
}

int TOP_DBS_NUM_SUBFIX(dbsOperSetNumberLen)(int nLen)
{
	if ( nLen <= 0 ) {
		return ERR_TOP_DBS_PARAM;
	}

	f_iNumberLen = nLen;

	return (0);
}

/*-------------------------  Local functions ----------------------------*/
static SQLDA *
dbsAlloc(int iMaxVarNum, int iMaxNameLen)
{
	SQLDA	*des;
	short	*pShort;
	int		i;

	des = sqlald(iMaxVarNum, iMaxNameLen, 0);
	if (des == NULL) {
		return NULL;
	}

	pShort = malloc(sizeof(short) * iMaxVarNum);
	if (pShort == NULL) {
		sqlclu(des);
		return NULL;
	}

	for (i = 0; i < iMaxVarNum; i++) {
		des->I[i] = pShort + i;
	}

	return des;
}

static int
dbsFree(SQLDA *des)
{
	if (des) {
		if (des->I[0]) {
			free(des->I[0]);
		}

		sqlclu(des);
	}

	return 0;
}

static int
dbsGetNext(char sSep, const char *pBegin, const char *pEnd)
{
	const char	*p;

	if (pBegin >= pEnd) {
		return -1;
	}

	if (*pBegin == '+') {
		pBegin++;
	}

	p = pBegin;
	while (p < pEnd && *p != sSep) p++;

	return (p - pBegin);
}

static int
dbsGetLen(int iType, int iLen)
{
	int	iRet, iPrec, iScale;

	switch (iType) {
		case DBS_ORA_TYPE_VARCHAR2:
		case DBS_ORA_TYPE_CHAR: {
			iRet = iLen;
			break;
		}

		case DBS_ORA_TYPE_NUMBER: {
			sqlprc((unsigned int *)&iLen, &iPrec, &iScale);
			if (iPrec == 0) iPrec = f_iNumberLen;
			if (iScale < 0) iPrec += -iScale;
			iPrec += 2;

			iRet = iPrec;
			break;
		}

		case DBS_ORA_TYPE_DATE: {
			iRet = 9;	/* $NLS_LANG = NULL */
			break;
		}

		case DBS_ORA_TYPE_ROWID: {
			iRet = 18;
			break;
		}

		default: {
			iRet = -1;
			break;
		}
	}

	return iRet;
}

/*-----------------------------  End ------------------------------------*/


/*--------------------------- Macro define ------------------------------*/
#define _DLEN_TINY_BUF 64
#define _DLEN_MINI_BUF 256
#define _DLEN_LARGE_BUF 1024
#define _DLEN_HUGE_BUF 10240
/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/

/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

