/*
 *
 *
 * dbs oper x.
 *
 *
 * FileName: dbs_x.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _DBS_X_H_20110302155648_
#define _DBS_X_H_20110302155648_
/*--------------------------- Include files -----------------------------*/
#include	"log_info.h"

/*--------------------------- Macro define ------------------------------*/
#define _TOP_DBS_ITFS_DEFINE(_ITFNUM)         \
        int dbsOperInit##_ITFNUM(int , int ) ; \
        int dbsOperSetNumberLen##_ITFNUM(int) ; \
        int dbsPrepare##_ITFNUM(const char *) ; \
        int dbsBindCount##_ITFNUM(void) ; \
        char * dbsBindName##_ITFNUM(int , char *) ; \
        char * dbsGetBind##_ITFNUM(int , char *, int *piLen) ; \
        int dbsBindNull##_ITFNUM(int) ; \
        int dbsBindString##_ITFNUM(int , const char *, int ) ; \
        int dbsBindArray##_ITFNUM(char *asArray[], int iCnt) ; \
        int dbsBindSepValues##_ITFNUM(char , const char *, int ) ; \
        int dbsOpen##_ITFNUM(void) ; \
        int dbsColumnCount##_ITFNUM(void) ; \
        char * dbsColumnName##_ITFNUM(int , char *) ; \
        int dbsColumnLength##_ITFNUM(int) ; \
        int dbsColumnString##_ITFNUM(int , char *, int ) ; \
        int dbsColumnChar##_ITFNUM(int , char *, int ) ; \
        int dbsColumnStrings##_ITFNUM(char *, int ) ; \
        int dbsColumnChars##_ITFNUM(char *, int ) ; \
        int dbsColumnBuffer##_ITFNUM(void) ; \
        int dbsFetch##_ITFNUM(void) ; \
        const char * dbsGetField##_ITFNUM(int , int *) ; \
        int dbsGetInd##_ITFNUM(int) ; \
        int dbsClose##_ITFNUM(void) ;

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

_TOP_DBS_ITFS_DEFINE(0)
_TOP_DBS_ITFS_DEFINE(1)
_TOP_DBS_ITFS_DEFINE(2)
_TOP_DBS_ITFS_DEFINE(3)
_TOP_DBS_ITFS_DEFINE(4)
_TOP_DBS_ITFS_DEFINE(5)
_TOP_DBS_ITFS_DEFINE(6)
_TOP_DBS_ITFS_DEFINE(7)
_TOP_DBS_ITFS_DEFINE(8)
_TOP_DBS_ITFS_DEFINE(9)
_TOP_DBS_ITFS_DEFINE(10)
_TOP_DBS_ITFS_DEFINE(11)
_TOP_DBS_ITFS_DEFINE(12)
_TOP_DBS_ITFS_DEFINE(13)
_TOP_DBS_ITFS_DEFINE(14)
_TOP_DBS_ITFS_DEFINE(15)
_TOP_DBS_ITFS_DEFINE(16)
_TOP_DBS_ITFS_DEFINE(17)
_TOP_DBS_ITFS_DEFINE(18)
_TOP_DBS_ITFS_DEFINE(19)
_TOP_DBS_ITFS_DEFINE(20)

#ifdef __cplusplus
}
#endif

#endif /*_DBS_X_H_20110302155648_*/
/*-----------------------------  End ------------------------------------*/
