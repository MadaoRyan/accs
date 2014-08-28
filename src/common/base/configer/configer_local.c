/*
 *
 *
 * An extended configuration parser (poc)
 *
 *
 * FileName: configer_file.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include	"topcfg.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"core_ctx.h"
#include	"log_info.h"
#include	"os_process.h"
#include	"util_ini.h"
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
#define	_				cfgLog

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/
static int cfgIsKey(char *sSection, char *sKey);

/*-------------------------  Global variable ----------------------------*/
extern PFN_RTCORE_LOGPRINT	cfgLog;

/*-------------------------  Global functions ---------------------------*/
int
cfgGetStrL(char *sSection, char *sKey, char *sDefault,
           char *sBuf, int iSize, char *sFile)
{
	char	sIni[INI_MAX_LINE], sRef[INI_MAX_LINE];
	char	*psArgv[_TINY_BUF_LEN], *pRef = sRef;
	int		iArgc, iKey, iRet, i;

	#define	cfgReturn(r)                            \
	do {                                            \
		if (sDefault) {                             \
			strMem2Str(sBuf, iSize, sDefault, -1);  \
			return -(r);                            \
		} else {                                    \
			return  (r);                            \
		}                                           \
	} while(0)

	iKey = cfgIsKey(sSection, sKey);

	if (!iKey) {
		iRet = cfgGetStr(CFG_SECTION_GENERAL, CFG_KEY_LOCAL, NULL,
		                 sIni, sizeof(sIni), sFile);
		if (iRet < 0) {
			iArgc = 0;
		} else {
			iArgc = procParseAgrs(sIni, psArgv);
		}

		for (i = 0; i < iArgc; i++) {
			iRet = cfgGetStr(sSection, sKey, NULL, sBuf, iSize, psArgv[i]);
			if (iRet < 0) {
				continue;

			} else {
				_(LOG_TRC, "VALID: sFile[%s] sSection[%s] sKey[%s]",
				  psArgv[i], sSection, sKey);
				return 0;
			}
		}
	}

	iRet = cfgGetStr(sSection, sKey, NULL, sBuf, iSize, sFile);
	if (iRet < 0) {
		if (iKey) {
			cfgReturn(-1);

		} else {
			iRet = cfgGetStr(CFG_SECTION_GENERAL, CFG_KEY_DEFAULT, NULL,
			                 sIni, sizeof(sIni), sFile);
			if (iRet < 0) cfgReturn(-2);

			iArgc = procParseAgrs(sIni, psArgv);

			iRet = cfgGetStr(sSection, CFG_KEY_REFERENCE, NULL,
			                 sRef, sizeof(sRef), sFile);
			if (iRet < 0) pRef = sSection;

			for (i = 0; i < iArgc; i++) {
				iRet = cfgGetStr(pRef, sKey, NULL, sBuf, iSize, psArgv[i]);
				if (iRet < 0) {
					continue;

				} else {
					_(LOG_TRC, "VALID: sFile[%s] sSection[%s] sKey[%s]",
					  psArgv[i], pRef, sKey);
					return 0;
				}
			}

			cfgReturn(-3);
		}
	}

	#undef	cfgReturn
	return 0;
}

int
cfgGetIntL(char *sSection, char *sKey, int iDefault, char *sFile)
{
	char	sBuf[_TINY_BUF_LEN];

	return (cfgGetStrL(sSection, sKey, "", sBuf, sizeof(sBuf), sFile) == 0 ?
			atoi(sBuf) : iDefault);
}

/*-------------------------  Local functions ----------------------------*/
static int
cfgIsKey(char *sSection, char *sKey)
{
	return (
		(
			strcmp(sSection, CFG_SECTION_GENERAL) == 0 && (
				strcmp(sKey, CFG_KEY_LOCAL) == 0 ||
				strcmp(sKey, CFG_KEY_DEFAULT) == 0
			)
		) || (
			strncmp(sKey, CFG_KEY_CONFIG, strlen(CFG_KEY_CONFIG)) == 0
		)
	);
}

/*-----------------------------  End ------------------------------------*/
