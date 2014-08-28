/*
 *
 *
 *  Warp logInfo New Api To Old Api.
 *
 *
 * FileName: log_info_warp.h 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

#ifndef _LOG_INFO_WARP_H_20101124141036_
#define _LOG_INFO_WARP_H_20101124141036_

/*------------------------ Include files ------------------------*/
#include "log_info.h"

#if 0
#pragma mark -
#pragma mark < Macro define >
#endif
/*--------------------- Macro define ----------------------------*/
/* Redefine LogInit */
#define logInit(f, n) \
( ((MODULE_NAME) = logCfgInit((f), (n))) ? 0 : -1)

/* Redfine LogEnd */
#define logEnd() logCfgEnd((MODULE_NAME))

/* Log Level Args */
#undef F_LOG_INFO
#define F_LOG_INFO(LEVEL) (MODULE_NAME), (LEVEL), __FILE__, __LINE__, __FUNCTION__

/* Log Level Args */
#undef LOG_ERR
#define LOG_ERR  F_LOG_INFO( 1 )
#undef LOG_WRN
#define LOG_WRN  F_LOG_INFO( 2 )
#undef LOG_NOR
#define LOG_NOR  F_LOG_INFO( 3 )
#undef LOG_BUG
#define LOG_BUG  F_LOG_INFO( 4 )
#undef LOG_TRC
#define LOG_TRC  F_LOG_INFO( 5 )

/* Redefine logInfo */
#define logInfo logCfgInfo

/* Redfine Session */
#define logSession(k) logCfgSession((MODULE_NAME), (k))
#define logSessionStart(k) logCfgSessionStart((MODULE_NAME), (k))
#define logSessionEnd(k) logCfgSessionEnd((MODULE_NAME), (k))

/* Redefine logDebug */
#undef logDebug
#undef logCfgDebug
#define logDebug(s, n) logCfgDebugString((MODULE_NAME), __FILE__, __LINE__, __FUNCTION__, (s), (#s), (n))

/* Clarm */
#define LOG_CFG_DEFINE \
struct LogAllCfg * MODULE_NAME = NULL;

#if 0
#pragma mark -
#pragma mark < Type define >
#endif
/*--------------------- Type define -----------------------------*/
extern struct LogAllCfg * MODULE_NAME;

#if 0
#pragma mark -
#pragma mark < Global functions declaration >
#endif
/*--------------------- Global function declaration -------------*/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
/*--------------------- Global variable -------------------------*/

#endif /* _LOG_INFO_WARP_H_20101124141036_ */
/*--------------------- End -------------------------------------*/
