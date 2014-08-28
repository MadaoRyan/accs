/*
 *
 *
 * container handle functions
 *
 *
 * FileName: util_ctn.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include	"util_ctn.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

/*--------------------------- Macro define ------------------------------*/
#define	_TINY_BUF_LEN	32
#define	_SMALL_BUF_LEN	256
#define	_LARGE_BUF_LEN	1024
#define	_HUGE_BUF_LEN	10240

#define	ctnIdxRaw(m, p)	(((p) - (m)->pData) / (m)->iSize)
#define	ctnGetRaw(m, i)	((m)->pData + (m)->iSize * (i))

/*---------------------------- Type define ------------------------------*/
struct T_CTN {
	int		iSize;
	int		iMax;
	int		iCnt;
	int		iSorted;
	int		(*fCompar)(const void *, const void *);
	void	*pData;
};

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
T_CTN *
ctnNew(int iSize, int (*fCompar)(const void *, const void *))
{
	T_CTN	*ptCtn;

	ptCtn = malloc(sizeof(T_CTN));
	if (ptCtn == NULL) {
		return NULL;
	}

	ptCtn->iSize = iSize;
	ptCtn->iMax = 0;
	ptCtn->iCnt = 0;
	ptCtn->pData = NULL;

	ctnSetCompar(ptCtn, fCompar);

	return ptCtn;
}

int
ctnReAlloc(T_CTN *ptCtn)
{
	int		iMax = ptCtn->iMax;
	void	*p;

	if (iMax == 0) {
		iMax = _TINY_BUF_LEN;
	} else {
		iMax *= 2;
	}

	p = realloc(ptCtn->pData, ptCtn->iSize * iMax);
	if (p == NULL) {
		return -1;
	}

	ptCtn->iMax = iMax;
	ptCtn->pData = p;

	return 0;
}

int
ctnFree(T_CTN *ptCtn)
{
	if (ptCtn != NULL) {
		free(ptCtn->pData);
		free(ptCtn);
	}

	return 0;
}

int
ctnSetSorted(T_CTN *ptCtn, int iSorted)
{
	ptCtn->iSorted = iSorted;

	return 0;
}

int
ctnSetCompar(T_CTN *ptCtn, int (*fCompar)(const void *, const void *))
{
	ptCtn->fCompar = fCompar;
	ptCtn->iSorted = 0;

	if (ptCtn->fCompar == NULL) {
		ptCtn->fCompar = (int(*)(const void *, const void *))strcmp;
	}

	return 0;
}

int
ctnAdd(T_CTN *ptCtn, void *pData)
{
	int	iRet;

	if (ptCtn->iCnt >= ptCtn->iMax) {
		iRet = ctnReAlloc(ptCtn);
		if (iRet < 0) {
			return -1;
		}
	}

	memcpy(ctnGetRaw(ptCtn, ptCtn->iCnt), pData, ptCtn->iSize);
	ptCtn->iCnt++;
	ptCtn->iSorted = 0;

	return 0;
}

void *
ctnGet(T_CTN *ptCtn, int iIdx)
{
	if (!ptCtn || iIdx < 0 || iIdx >= ptCtn->iCnt) {
		return NULL;
	}

	return (ctnGetRaw(ptCtn, iIdx));
}

int
ctnIdx(T_CTN *ptCtn, void *pData)
{
	int	iIdx = pData - ptCtn->pData;

	if (!ptCtn || iIdx < 0 || iIdx % ptCtn->iSize != 0) {
		return -1;
	}

	iIdx /= ptCtn->iSize;

	if (iIdx >= ptCtn->iCnt) {
		return -2;
	}

	return iIdx;
}

int
ctnCount(T_CTN *ptCtn)
{
	return ptCtn ? ptCtn->iCnt : 0;
}

int
ctnSort(T_CTN *ptCtn)
{
	qsort(ptCtn->pData, ptCtn->iCnt, ptCtn->iSize, ptCtn->fCompar);
	ptCtn->iSorted = 1;

	return 0;
}

void *
ctnSearch(T_CTN *ptCtn, const void *pKey)
{
	void *pCur;

	if (ptCtn->iSorted) {
		pCur = bsearch(pKey, ptCtn->pData, ptCtn->iCnt, ptCtn->iSize,
		               ptCtn->fCompar);
		if (pCur == NULL) {
			return NULL;
		}

		pCur -= ptCtn->iSize;
		while (pCur >= ptCtn->pData) {
			if (ptCtn->fCompar(pKey, pCur) != 0) {
				break;
			}
			pCur -= ptCtn->iSize;
		}

		return (pCur + ptCtn->iSize);

	} else {
		return (ctnSearchL(ptCtn, pKey, NULL));
	}
}

void *
ctnSearchL(T_CTN *ptCtn, const void *pKey, const void *pStart)
{
	void *pCur, *pEnd;

	pCur = pStart == NULL ? ptCtn->pData : (void *)pStart + ptCtn->iSize;
	pEnd = ctnGetRaw(ptCtn, ptCtn->iCnt);

	while (pCur < pEnd) {
		if (ptCtn->fCompar(pKey, pCur) == 0) {
			break;
		}
		pCur += ptCtn->iSize;
	}

	if (pCur < pEnd) {
		return pCur;
	}

	return NULL;
}

void *
ctnSearchM(T_CTN *ptCtn, const void *pKey, const void *pStart)
{
	void *pCur;

	if (ptCtn->iSorted) {
		if (pStart == NULL) {
			pCur = bsearch(pKey, ptCtn->pData, ptCtn->iCnt, ptCtn->iSize,
			               ptCtn->fCompar);
			if (pCur == NULL) {
				return NULL;
			}

			pCur -= ptCtn->iSize;
			while (pCur >= ptCtn->pData) {
				if (ptCtn->fCompar(pKey, pCur) != 0) {
					break;
				}
				pCur -= ptCtn->iSize;
			}

			return (pCur + ptCtn->iSize);

		} else {
			pCur = (void *)pStart + ptCtn->iSize;
			if (ptCtn->fCompar(pKey, pCur) == 0) {
				return pCur;
			}

			return NULL;
		}

	} else {
		return ctnSearchL(ptCtn, pKey, pStart);
	}
}

int
ctnSearchIdx(T_CTN *ptCtn, const void *pKey)
{
	void *p;

	p = ctnSearch(ptCtn, pKey);
	if (p == NULL) {
		return -1;
	}

	return (ctnIdxRaw(ptCtn, p));
}

int
ctnSearchIdxL(T_CTN *ptCtn, const void *pKey, const int iCur)
{
	void *p;

	p = iCur < 0 ? NULL : ctnGetRaw(ptCtn, iCur);

	p = ctnSearchL(ptCtn, pKey, p);
	if (p == NULL) {
		return -1;
	}

	return (ctnIdxRaw(ptCtn, p));
}

int
ctnSearchIdxM(T_CTN *ptCtn, const void *pKey, const int iCur)
{
	void *p;

	p = iCur < 0 ? NULL : ctnGetRaw(ptCtn, iCur);

	p = ctnSearchM(ptCtn, pKey, p);
	if (p == NULL) {
		return -1;
	}

	return (ctnIdxRaw(ptCtn, p));
}

int
ctnComparInt(const void *p1, const void *p2)
{
	return (*(int *)p1 - *(int *)p2);
}

/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

