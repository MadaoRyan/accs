/*
 *
 *
 * dbs common define.
 *
 *
 * FileName: glb_dbs.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _GLB_DBS_H_20100115105856_
#define _GLB_DBS_H_20100115105856_
/*--------------------------- Include files -----------------------------*/
#include "top_dbs.h"

/*--------------------------- Macro define ------------------------------*/

#define	DBS_FIND		1
#define	DBS_LOCK		2
#define	DBS_UPDATE		3
#define	DBS_INSERT		4
#define	DBS_DELETE		5
#define	DBS_CLOSE		6
#define	CUR_OPEN		7
#define	CUR_FETCH		8
#define	CUR_DELETE		9
#define	CUR_CLOSE		10
#define	CUR_UPDATE		11
#define	CUR_ALLOPEN		12
#define	CUR_ALLFETCH	13
#define	CUR_ALLCLOSE	14

/* oracle database sqlcode */

#define	DB_OK			0
#define	DB_NOTFOUND		1403
#define	DB_NULL			-1405
#define	DB_NOTINTRANS	-255
#define	DB_DUPLICATE	-1
#define	DB_INDEXDUP		-239
#define	DB_CURSOR		-1001

typedef int	sqlint32;

/* db2 database sqlcode */
#ifdef _DB_DB2

#define	DB_OK			0
#define	DB_NOTFOUND		100
#define	DB_NULL			-305
#define	DB_NOTINTRANS	-255
#define	DB_DUPLICATE	-1
#define	DB_INDEXDUP		-239
#define	DB_CURSOR		-1001

#endif
/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /*_GLB_DBS_H_20100115105856_*/
/*-----------------------------  End ------------------------------------*/
