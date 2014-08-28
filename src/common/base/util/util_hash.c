/*
 *
 *
 * Hash Tables
 *
 *
 * FileName: util_hash.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include	"util_hash.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"private/uthash.h"

#undef	uthash_fatal
#define	uthash_fatal(msg) (iRet = -1)

#undef	HASH_INITIAL_NUM_BUCKETS
#undef	HASH_INITIAL_NUM_BUCKETS_LOG2
#define	HASH_INITIAL_NUM_BUCKETS		iSize
#define	HASH_INITIAL_NUM_BUCKETS_LOG2	iLog2

#define	call(h,f,k,v)	(h)->tFuncs.f(&(h)->tFuncs, (k), (v))

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/
typedef struct {
	UT_hash_handle	hh;

	void	*pValue;
	char	sKey[0];
} T_HASH_ENTRY;

struct T_HASH_TABLE {
	T_HASH_ENTRY	*ptTable;
	int				iInitSize;

	T_HASH_ENTRY	*ptLast;

	T_HASH_FUNCS	tFuncs;
};

/*---------------------- Local function declaration ---------------------*/
static void *hashStringNew(T_HASH_FUNCS *ptFuncs,
                           const char *pKey, void *pValue);
static void *hashStructNew(T_HASH_FUNCS *ptFuncs,
                           const char *pKey, void *pValue);
static void  hashComFree  (T_HASH_FUNCS *ptFuncs,
                           const char *pKey, void *pValue);

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
T_HASH_TABLE *
hashNew(int iSize)
{
	T_HASH_TABLE	*ptHashTable;

	ptHashTable = malloc(sizeof(T_HASH_TABLE));
	if (ptHashTable == NULL) {
		return NULL;
	}
	memset(ptHashTable, 0, sizeof(*ptHashTable));

	ptHashTable->ptTable = NULL;
	ptHashTable->iInitSize = iSize < 0 ? 0 : iSize;

	return ptHashTable;
}

int
hashAdd(T_HASH_TABLE *ptHashTable, const char *sKey, void *pValue)
{
	T_HASH_ENTRY	*ptEntry;
	int				iSize = 32, iLog2 = 5;
	int				iRet = 0;

	if (!sKey) {
		return -4;
	}
	HASH_FIND_STR(ptHashTable->ptTable, sKey, ptEntry);
	if (ptEntry) {
		return -1;
	}

	ptEntry = malloc(sizeof(T_HASH_ENTRY) + strlen(sKey) + 1);
	if (ptEntry == NULL) {
		return -2;
	}

	strcpy(ptEntry->sKey, sKey);
	if (ptHashTable->tFuncs.fNew) {
		ptEntry->pValue = call(ptHashTable, fNew, sKey, pValue);
		if (ptEntry->pValue == NULL) {
			return -5;
		}
	} else {
		ptEntry->pValue = pValue;
	}

	if (!ptHashTable->ptTable) {
		if (ptHashTable->iInitSize > 0) {
			while (iSize < ptHashTable->iInitSize) {
				iSize *= 2;
				iLog2++;
			}
		}
	}

	HASH_ADD_STR(ptHashTable->ptTable, sKey, ptEntry);
	if (iRet < 0) {
		free(ptEntry);
		return -3;
	}

	return 0;
}

int
hashSet(T_HASH_TABLE *ptHashTable, const char *sKey, void *pValue)
{
	T_HASH_ENTRY	*ptEntry;
	int				iSize = 32, iLog2 = 5;
	int				iRet = 0;

	if (!sKey) {
		return -4;
	}
	HASH_FIND_STR(ptHashTable->ptTable, sKey, ptEntry);
	if (ptEntry) {
		if (ptHashTable->tFuncs.fNew) {
			call(ptHashTable, fDelete, ptEntry->sKey, ptEntry->pValue);
			ptEntry->pValue = call(ptHashTable, fNew, sKey, pValue);
			if (ptEntry->pValue == NULL) {
				return -6;
			}
		} else {
			ptEntry->pValue = pValue;
		}
		return 0;
	}

	ptEntry = malloc(sizeof(T_HASH_ENTRY) + strlen(sKey) + 1);
	if (ptEntry == NULL) {
		return -2;
	}

	strcpy(ptEntry->sKey, sKey);
	if (ptHashTable->tFuncs.fNew) {
		ptEntry->pValue = call(ptHashTable, fNew, sKey, pValue);
		if (ptEntry->pValue == NULL) {
			return -5;
		}
	} else {
		ptEntry->pValue = pValue;
	}

	if (!ptHashTable->ptTable) {
		if (ptHashTable->iInitSize > 0) {
			while (iSize < ptHashTable->iInitSize) {
				iSize *= 2;
				iLog2++;
			}
		}
	}

	HASH_ADD_STR(ptHashTable->ptTable, sKey, ptEntry);
	if (iRet < 0) {
		free(ptEntry);
		return -3;
	}

	return 0;
}

void *
hashGet(T_HASH_TABLE *ptHashTable, const char *sKey)
{
	T_HASH_ENTRY	*ptEntry;

	if (!sKey) {
		return NULL;
	}
	HASH_FIND_STR(ptHashTable->ptTable, sKey, ptEntry);
	if (!ptEntry) {
		return NULL;
	}

	return ptEntry->pValue;
}

void *
hashDel(T_HASH_TABLE *ptHashTable, const char *sKey)
{
	T_HASH_ENTRY	*ptEntry;
	void			*pValue;

	if (!sKey) {
		return NULL;
	}
	HASH_FIND_STR(ptHashTable->ptTable, sKey, ptEntry);
	if (!ptEntry) {
		return NULL;
	}

	pValue = ptEntry->pValue;

	if (ptHashTable->ptLast == ptEntry) {
		ptHashTable->ptLast = ptEntry->hh.prev;
	}

	HASH_DEL(ptHashTable->ptTable, ptEntry);
	if (ptHashTable->tFuncs.fDelete)
		call(ptHashTable, fDelete, ptEntry->sKey, ptEntry->pValue);
	free(ptEntry);

	return pValue;
}

int
hashFree(T_HASH_TABLE *ptHashTable)
{
	T_HASH_ENTRY	*ptCur, *ptTmp;

	if (ptHashTable) {
		HASH_ITER(hh, ptHashTable->ptTable, ptCur, ptTmp) {
			HASH_DEL(ptHashTable->ptTable, ptCur);
			if (ptHashTable->tFuncs.fDelete)
				call(ptHashTable, fDelete, ptCur->sKey, ptCur->pValue);
			free(ptCur);
		}
		free(ptHashTable->ptTable);
	}

	return 0;
}

int
hashCount(T_HASH_TABLE *ptHashTable)
{
	return HASH_COUNT(ptHashTable->ptTable);
}

const char *
hashIter(T_HASH_TABLE *ptHashTable)
{
	ptHashTable->ptLast = ptHashTable->ptLast ? ptHashTable->ptLast->hh.next
	                                          : ptHashTable->ptTable;
	if (!ptHashTable->ptLast) {
		return NULL;
	}

	return ptHashTable->ptLast->sKey;
}

int
hashRewind(T_HASH_TABLE *ptHashTable)
{
	ptHashTable->ptLast = NULL;

	return 0;
}

int
hashSort(T_HASH_TABLE *ptHashTable, int (*fCompare)(void *, void *))
{
	HASH_SORT(ptHashTable->ptTable, fCompare);

	return 0;
}

int
hashSetFuncs(T_HASH_TABLE *ptHashTable, T_HASH_FUNCS tFuncs)
{
	if (HASH_COUNT(ptHashTable->ptTable) > 0) {
		return -1;
	}

	ptHashTable->tFuncs = tFuncs;

	return 0;
}

T_HASH_FUNCS
hashFuncString(void)
{
	T_HASH_FUNCS	tFunc;

	memset(&tFunc, 0, sizeof(tFunc));

	tFunc.fNew = hashStringNew;
	tFunc.fDelete = hashComFree;

	return tFunc;
}

T_HASH_FUNCS
hashFuncStruct(int iSize)
{
	T_HASH_FUNCS	tFunc;

	memset(&tFunc, 0, sizeof(tFunc));

	tFunc.fNew = hashStructNew;
	tFunc.fDelete = hashComFree;
	tFunc.pData = (void *)iSize;

	return tFunc;
}

/*-------------------------  Local functions ----------------------------*/
static void *
hashStringNew(T_HASH_FUNCS *ptFuncs, const char *pKey, void *pValue)
{
	return strdup(pValue);
}

static void *
hashStructNew(T_HASH_FUNCS *ptFuncs, const char *pKey, void *pValue)
{
	void *pRet = malloc((int)ptFuncs->pData);
	if (pRet) memcpy(pRet, pValue, (int)ptFuncs->pData);
	return pRet;
}

static void
hashComFree(T_HASH_FUNCS *ptFuncs, const char *pKey, void *pValue)
{
	free(pValue);
	return;
}

/*-----------------------------  End ------------------------------------*/

