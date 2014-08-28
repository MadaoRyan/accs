/*
 *
 *
 * An extended configuration parser
 *
 *
 * FileName: configer.c
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
#include	"os_dll.h"
#include	"util_csv.h"
#include	"util_dtm.h"
#include	"util_hash.h"
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

#define	CFG_MAX_LEVEL	8

#define	CFG_FILE		"file"
#define	CFG_FILE_SEP	':'

#define	cfgIsValid(c)                       \
(                                           \
	((c) >= '0' && (c) <= '9') ||           \
	((c) >= 'a' && (c) <= 'z') ||           \
	((c) >= 'A' && (c) <= 'Z') ||           \
	(c) == '_' || (c) == '.' || (c) == ':'  \
)

/*---------------------------- Type define ------------------------------*/
typedef struct {
	H_DllFile	hLib;
	F_CFG_GET	hFun;
	int			iFree;
} T_CFG_INTERFACE;

/*---------------------- Local function declaration ---------------------*/
static char *cfgParse(char *sBuf, char **ppValue);
static int   cfgEnvReplace(char *sFile, char *sIn, char *sOut, int iSize);
static char *cfgGetEnv(char *sFile, char *sEnv, char *sBuf, int iSize);

static int cfgFuncInit(void);
static int cfgFuncFinal(void);

static F_CFG_GET cfgGetFunc(char *sType);

static void *cfgHashNew (T_HASH_FUNCS *ptFuncs, const char *pKey, void *pValue);
static void  cfgHashFree(T_HASH_FUNCS *ptFuncs, const char *pKey, void *pValue);

static int cfgGetFile(char *sFile, char *sSection, char *sKey,
                      char *sIn, char *sOut, int iSize);

static int cfgLogNull(char *sLog, int iLevel, char *sLevel,
                      char *sFile, int iLine, const char *sFunction,
                      char *sLogStr, ...);

/*-------------------------  Global variable ----------------------------*/
PFN_RTCORE_LOGPRINT	cfgLog = cfgLogNull;

static T_HASH_FUNCS	f_tHashFuncs = {cfgHashNew, cfgHashFree, NULL};
static T_HASH_TABLE	*f_hHash;
static int			f_iLevel;

/*-------------------------  Global functions ---------------------------*/
int
cfgGetStr(char *sSection, char *sKey, char *sDefault,
          char *sBuf, int iSize, char *sFile)
{
	char		sIni[INI_MAX_LINE], sIni2[INI_MAX_LINE];
	F_CFG_GET	fGet = NULL;
	char		*pType, *pValue = sIni;
	int			iRet;

	#define	cfgReturn(r)                            \
	do {                                            \
		f_iLevel--;                                 \
		if (sDefault) {                             \
			strMem2Str(sBuf, iSize, sDefault, -1);  \
			return -(r);                            \
		} else {                                    \
			return  (r);                            \
		}                                           \
	} while(0)

	if (++f_iLevel > CFG_MAX_LEVEL) {
		_(LOG_ERR, "Too Many Levels of Nesting");
		cfgReturn(-1);
	}

	iRet = iniReadString(sSection, sKey, NULL, sIni, sizeof(sIni), sFile);
	if (iRet < 0) {
		cfgReturn(-2);
	}

	pType = cfgParse(sIni, &pValue);
	if (pType) {
		fGet = cfgGetFunc(pType);
		if (!fGet) {
			_(LOG_ERR, "cfgGetFunc() error, sType[%s]", pType);
			cfgReturn(-3);
		}

		iRet = cfgEnvReplace(sFile, pValue, sIni2, sizeof(sIni2));
		if (iRet < 0) {
			_(LOG_ERR, "cfgEnvReplace() error, sFile[%s] sIn[%s] iOut[%zu]",
			  sFile, pValue, sizeof(sIni2));
			cfgReturn(-4);
		}

		iRet = fGet(sFile, sSection, sKey, sIni2, sBuf, iSize);
		if (iRet < 0) {
			_(LOG_ERR, "fGet[%s]() error, sIn[%s] iOut[%d]",
			  pType, sIni2, iSize);
			cfgReturn(-5);
		}

	} else {
		iRet = cfgEnvReplace(sFile, sIni, sBuf, iSize);
		if (iRet < 0) {
			_(LOG_ERR, "cfgEnvReplace() error, sFile[%s] sIn[%s] iOut[%d]",
			  sFile, sIni, iSize);
			cfgReturn(-6);
		}
	}

	f_iLevel--;

	#undef	cfgReturn
	return 0;
}

int
cfgGetInt(char *sSection, char *sKey, int iDefault, char *sFile)
{
	char	sBuf[_TINY_BUF_LEN];

	return (cfgGetStr(sSection, sKey, "", sBuf, sizeof(sBuf), sFile) == 0 ?
			atoi(sBuf) : iDefault);
}

int
cfgFinal(void)
{
	return cfgFuncFinal();
}

int
cfgSetLogOff(void)
{
	_ = cfgLogNull;
	return 0;
}

int
cfgSetLogOn(void)
{
	_ = logInfo;
	return 0;
}

/*-------------------------  Local functions ----------------------------*/
static char *
cfgParse(char *sBuf, char **ppValue)
{
	char	*pCur;
	int		iLen = strlen(sBuf);

	if (sBuf[0] == '%' && sBuf[1] == '{' && sBuf[iLen-1] == '}') {
		sBuf[iLen-1] = '\0';
		*ppValue = sBuf + 2;
		return "poc";

	} else if (sBuf[0] == '&' && sBuf[1] == '{' && sBuf[iLen-1] == '}') {
		sBuf[iLen-1] = '\0';
		*ppValue = sBuf + 2;
		return "file";

	} else if (sBuf[0] == '<' && (pCur = strstr(sBuf, ">{")) &&
	           sBuf[iLen-1] == '}') {
		*pCur = '\0';
		sBuf[iLen-1] = '\0';
		*ppValue = pCur + 2;
		strToLower(sBuf + 1);
		return sBuf + 1;
	}

	return NULL;
}

static int
cfgEnvReplace(char *sFile, char *sIn, char *sOut, int iSize)
{
	char	*pCur = sIn, *pLast, *pOut = sOut;
	char	sBuf[INI_MAX_LINE], sEnv[_MINI_BUF_LEN], *pEnv;

	#define	cfgCopy(p,l)                                \
	do {                                                \
		if (iSize < (l)) {                              \
			_(LOG_ERR, "Output Buffer is Not Enough");  \
			return -3;                                  \
		}                                               \
		memcpy(pOut, (p), (l));                         \
		pOut += (l);                                    \
		iSize -= (l);                                   \
	} while(0)

	iSize--;
	while (pCur) {
		pLast = pCur;

		pCur = strchr(pCur, '$');
		if (pCur) {
			cfgCopy(pLast, pCur - pLast);

			pCur++;
			if (*pCur == '(') {
				pCur++;
				pLast = pCur;
				pCur = strchr(pCur, ')');
				if (!pCur) {
					_(LOG_ERR, "Missing Right Bracket");
					return -1;
				}
				strMem2Str(sEnv, sizeof(sEnv), pLast, pCur - pLast);
				pCur++;

			} else if (!cfgIsValid(*pCur)) {
				pCur++;
				cfgCopy(pCur - 2, 2);
				continue;

			} else {
				pLast = pCur;
				while (cfgIsValid(*pCur)) pCur++;
				strMem2Str(sEnv, sizeof(sEnv), pLast, pCur - pLast);
			}

			pEnv = cfgGetEnv(sFile, sEnv, sBuf, sizeof(sBuf));
			if (pEnv) {
				cfgCopy(pEnv, strlen(pEnv));
			} else {
				_(LOG_ERR, "sEnv[%s] Not Exist", sEnv);
				return -2;
			}

		} else {
			cfgCopy(pLast, strlen(pLast));
		}
	}
	*pOut = '\0';

	#undef	cfgCopy
	return pOut - sOut;
}

static char *
cfgGetEnv(char *sFile, char *sEnv, char *sBuf, int iSize)
{
	int	iRet;

	iRet = cfgGetStr(CFG_SECTION_GENERAL, sEnv, NULL, sBuf, iSize, sFile);
	if (iRet != 0) {
		return getenv(sEnv);
	}

	return sBuf;
}

static int
cfgFuncInit(void)
{
	f_hHash = hashNew(0);
	if (!f_hHash) {
		_(LOG_ERR, "hashNew() error");
		return -1;
	}

	int iRet = hashSetFuncs(f_hHash, f_tHashFuncs);
	if (iRet < 0) {
		_(LOG_ERR, "hashSetFuncs() error on [%d]", iRet);
		cfgFuncFinal();
		return -2;
	}

	T_CFG_INTERFACE	tInterface;

	setnull(tInterface);
	tInterface.hFun = cfgGetFile;

	iRet = hashAdd(f_hHash, CFG_FILE, &tInterface);
	if (iRet < 0) {
		_(LOG_ERR, "hashAdd() error, sKey[%s]", CFG_FILE);
		return -3;
	}

	return 0;
}

static int
cfgFuncFinal(void)
{
	if (f_hHash) {
		hashFree(f_hHash);
		f_hHash = NULL;
	}

	return 0;
}

static F_CFG_GET
cfgGetFunc(char *sType)
{
	T_CFG_INTERFACE	*ptInterface;
	int				iRet;

	if (!f_hHash) {
		iRet = cfgFuncInit();
		if (iRet < 0) {
			_(LOG_ERR, "cfgFuncInit() error on [%d]", iRet);
			return NULL;
		}
	}

	ptInterface = hashGet(f_hHash, sType);
	if (!ptInterface) {
		_(LOG_WRN, "hashGet() error, sKey[%s]", sType);

		iRet = hashAdd(f_hHash, sType, NULL);
		if (iRet < 0) {
			_(LOG_ERR, "hashAdd() error, sKey[%s]", sType);
			return NULL;
		}

		ptInterface = hashGet(f_hHash, sType);
		if (!ptInterface) {
			_(LOG_ERR, "hashGet() error, sKey[%s]", sType);
			return NULL;
		}
	}

	return ptInterface->hFun;
}

static void *
cfgHashNew(T_HASH_FUNCS *ptFuncs, const char *pKey, void *pValue)
{
	T_CFG_INTERFACE	*ptInterface;

	ptInterface = malloc(sizeof(*ptInterface));
	if (!ptInterface) {
		_(LOG_ERR, "malloc() error, iSize[%zu]", sizeof(*ptInterface));
		return NULL;
	}

	if (pValue) {
		*ptInterface = *((T_CFG_INTERFACE *)pValue);

	} else {
		char	sBuf[_MINI_BUF_LEN];

		snprintf(sBuf, sizeof(sBuf), "libtopcfg_%s.so", pKey);

		ptInterface->hLib = dllOpen(sBuf);
		if (!ptInterface->hLib) {
			_(LOG_ERR, "dllOpen() error, sLib[%s] sErrStr[%s]", sBuf, dllErrStr());
			free(ptInterface);
			return NULL;
		}

		ptInterface->hFun = dllGetFun(ptInterface->hLib, CFG_FUN_GET);
		if (!ptInterface->hFun) {
			_(LOG_ERR, "dllGetFun() error, sFun[%s] sErrStr[%s]",
			  CFG_FUN_GET, dllErrStr());
			dllClose(ptInterface->hLib);
			free(ptInterface);
			return NULL;
		}

		ptInterface->iFree = 1;
	}

	return ptInterface;
}

static void
cfgHashFree(T_HASH_FUNCS *ptFuncs, const char *pKey, void *pValue)
{
	if (((T_CFG_INTERFACE *)pValue)->iFree) {
		dllClose(((T_CFG_INTERFACE *)pValue)->hLib);
	}
	free(pValue);
	return;
}

static int
cfgGetFile(char *sFile, char *sSection, char *sKey,
           char *sIn, char *sOut, int iSize)
{
	T_SPLIT_STR	tArgs;
	enum		{eFile, eSection, eKey};
	char		*pArgs[] = {sFile, sSection, sKey};
	int			iRet, i;

	iRet = csvSplitStrN(sIn, CFG_FILE_SEP, arrayof(pArgs), &tArgs);
	if (iRet < 0) {
		_(LOG_ERR, "csvSplitStrN() error on [%d], cSep[%c] iCnt[%zu]",
		  iRet, CFG_FILE_SEP, arrayof(pArgs));
		return -1;
	}

	for (i = 0; i < tArgs.iCnt; i++) {
		if (tArgs.psCol[i][0] != '\0') pArgs[i] = tArgs.psCol[i];
	}

	iRet = cfgGetStr(pArgs[eSection], pArgs[eKey], NULL, sOut, iSize,
	                 pArgs[eFile]);
	if (iRet < 0) {
		_(LOG_ERR, "cfgGetStr() error on [%d], "
		  "sSection[%s] sKey[%s] iOut[%d] sFile[%s]",
		  iRet, pArgs[eSection], pArgs[eKey], iSize, pArgs[eFile]);
		return -2;
	}

	_(LOG_BUG, "VALID: sFile[%s] sSection[%s] sKey[%s]",
	  pArgs[eFile], pArgs[eSection], pArgs[eKey]);

	return 0;
}

static int
cfgLogNull(char *sLog, int iLevel, char *sLevel,
           char *sFile, int iLine, const char *sFunction, char *sLogStr, ...)
{
	return 0;
}

/*-----------------------------  End ------------------------------------*/
