/*
 *
 * 
 * Algorithm Functions
 * 
 * 
 * FileName: util_alg.c
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include	"util_alg.h"

#include	<stdlib.h>
#include	<string.h>

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/
static int	algHIsPrime(unsigned int iNum);

/*-------------------------  Global variable ----------------------------*/
static T_HTABLE	f_tHtab;

/*-------------------------  Global functions ---------------------------*/
/**
 * qsort
 *
 * @param pBase: 起始地址
 * @param iNum: 个数
 * @param iSize: 每个大小
 * @param fCompar: 比较函数
 *
 * @return 无
 *
 */
void
algSort(void *pBase, size_t iNum, size_t iSize, F_COMPAR fCompar)
{
	qsort(pBase, iNum, iSize, fCompar);
}

/**
 * algBSearch
 *
 * @param pKey: 查找目标
 * @param pBase: 起始地址
 * @param iNum: 个数
 * @param iSize: 每个大小
 * @param fCompar: 比较函数
 *
 * @return >0:成功
 *       NULL:失败
 *
 */
void *
algBSearch(const void *pKey, const void *pBase, size_t iNum, size_t iSize,
		   F_COMPAR fCompar)
{
	return (bsearch(pKey, pBase, iNum, iSize, fCompar));
}

/**
 * algLSearch
 *
 * @param pKey: 查找目标，
 * @param pBase: 起始地址
 * @param iNum: 个数
 * @param iSize: 每个大小
 * @param fCompar: 比较函数
 *
 * @return >0:成功
 *       NULL:失败
 *
 */
void *
algLSearch(const void *pKey, const void *pBase, size_t iNum, size_t iSize,
		   F_COMPAR fCompar)
{
	const void *pResult = pBase;
	size_t iCnt = 0;

	while (iCnt < iNum && (*fCompar)(pKey, pResult) != 0) {
		pResult += iSize;
		++iCnt;
	}

	return iCnt < iNum ? (void *) pResult : NULL;
}

/**
 * algHCreateR
 *
 * @param iNum: 哈希表能容纳的元素个数
 * @param ptHtab: 哈希表句柄
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
algHCreateR(size_t iNum, T_HTABLE *ptHtab)
{
	/* Test for correct arguments. */
	if (ptHtab == NULL) {
		return -1;
	}

	/* There is still another ptTable active. Return with error. */
	if (ptHtab->ptTable != NULL)
		return -2;

	/* We need a iSize of at least 3.
	   Otherwise the hash functions we use will not work. */
	if (iNum < 3)
		iNum = 3;

	/* Change iNum to the first prime number not smaller as iNum. */
	iNum |= 1;	/* make odd */
	while (!algHIsPrime(iNum))
		iNum += 2;

	ptHtab->iSize = iNum;
	ptHtab->iFilled = 0;

	/* allocate memory and zero out */
	ptHtab->ptTable = (T_TABLE *)calloc(ptHtab->iSize + 1, sizeof(T_TABLE));
	if (ptHtab->ptTable == NULL)
		return -3;

	/* everything went alright */
	return 0;
}

/**
 * algHSearchR
 *
 * @param tItem: 查找目标
 * @param ptHtab: 哈希表句柄
 *
 * @return >0:成功
 *       NULL:失败
 *
 */
T_ITEM *
algHSearchR(T_ITEM tItem, T_HTABLE *ptHtab)
{
	unsigned int iHval;
	unsigned int iCount;
	unsigned int iLen = strlen(tItem.pKey);
	unsigned int iIdx;

	/* Compute an value for the given string. Perhaps use a better method. */
	iHval = iLen;
	iCount = iLen;
	while (iCount-- > 0) {
		iHval <<= 4;
		iHval += tItem.pKey[iCount];
	}
	if (iHval == 0)
		++iHval;

	/* First hash function: simply take the modul but prevent zero. */
	iIdx = iHval % ptHtab->iSize + 1;

	if (ptHtab->ptTable[iIdx].iUsed) {
		/* Further action might be required according to the action value. */
		if (ptHtab->ptTable[iIdx].iUsed == iHval
			&& strcmp(tItem.pKey, ptHtab->ptTable[iIdx].tItem.pKey) == 0) {
			return &ptHtab->ptTable[iIdx].tItem;
		}

		/* Second hash function, as suggested in [Knuth] */
		unsigned int hval2 = 1 + iHval % (ptHtab->iSize - 2);
		unsigned int first_idx = iIdx;

		do {
			/* Because SIZE is prime this guarantees to step through all
			   available indeces. */
			if (iIdx <= hval2)
				iIdx = ptHtab->iSize + iIdx - hval2;
			else
				iIdx -= hval2;

			/* If we visited all entries leave the loop unsuccessfully. */
			if (iIdx == first_idx)
				break;

			/* If tItem is found use it. */
			if (ptHtab->ptTable[iIdx].iUsed == iHval
				&& strcmp(tItem.pKey, ptHtab->ptTable[iIdx].tItem.pKey) == 0) {
				return &ptHtab->ptTable[iIdx].tItem;
			}
		} while (ptHtab->ptTable[iIdx].iUsed);
	}

	return NULL;
}

/**
 * algHEnterR
 *
 * @param tItem: 欲插入哈希表的元素
 * @param ptHtab: 哈希表句柄
 *
 * @return 1:重键
 *         0:成功
 *        <0:失败
 *
 */
int
algHEnterR(T_ITEM tItem, T_HTABLE *ptHtab)
{
	unsigned int iHval;
	unsigned int iCount;
	unsigned int iLen = strlen(tItem.pKey);
	unsigned int iIdx;

	/* Compute an value for the given string. Perhaps use a better method. */
	iHval = iLen;
	iCount = iLen;
	while (iCount-- > 0) {
		iHval <<= 4;
		iHval += tItem.pKey[iCount];
	}
	if (iHval == 0)
		++iHval;

	/* First hash function: simply take the modul but prevent zero. */
	iIdx = iHval % ptHtab->iSize + 1;

	if (ptHtab->ptTable[iIdx].iUsed) {
		/* Further action might be required according to the action value. */
		if (ptHtab->ptTable[iIdx].iUsed == iHval
			&& strcmp(tItem.pKey, ptHtab->ptTable[iIdx].tItem.pKey) == 0) {
			return 1;
		}

		/* Second hash function, as suggested in [Knuth] */
		unsigned int hval2 = 1 + iHval % (ptHtab->iSize - 2);
		unsigned int first_idx = iIdx;

		do {
			/* Because SIZE is prime this guarantees to step through all
			   available indeces. */
			if (iIdx <= hval2)
				iIdx = ptHtab->iSize + iIdx - hval2;
			else
				iIdx -= hval2;

			/* If we visited all entries leave the loop unsuccessfully. */
			if (iIdx == first_idx)
				break;

			/* If tItem is found use it. */
			if (ptHtab->ptTable[iIdx].iUsed == iHval
				&& strcmp(tItem.pKey, ptHtab->ptTable[iIdx].tItem.pKey) == 0) {
				return 1;
			}
		} while (ptHtab->ptTable[iIdx].iUsed);
	}

	/* An empty bucket has been found. */
	/* If ptTable is full and another tItem should be entered return
	   with error. */
	if (ptHtab->iFilled == ptHtab->iSize) {
		return -1;
	}

	ptHtab->ptTable[iIdx].iUsed = iHval;
	ptHtab->ptTable[iIdx].tItem = tItem;

	++ptHtab->iFilled;

	return 0;
}

/**
 * algHDestroyR
 *
 * @return 无
 *
 */
void
algHDestroyR(T_HTABLE *ptHtab)
{
	/* Test for correct arguments. */
	if (ptHtab == NULL) {
		return;
	}

	/* Free iUsed memory. */
	free (ptHtab->ptTable);

	/* the sign for an existing ptTable is an value != NULL in htable */
	ptHtab->ptTable = NULL;
}

/**
 * algHCreate
 *
 * @param iNum: 哈希表能容纳的元素个数
 *
 * @return 0:成功
 *        <0:失败
 *
 */
int
algHCreate(size_t iNum)
{
	return algHCreateR(iNum, &f_tHtab);
}

/**
 * algHSearch
 *
 * @param tItem: 查找目标
 *
 * @return >0:成功
 *       NULL:失败
 *
 */
T_ITEM *
algHSearch(T_ITEM tItem)
{
	return algHSearchR(tItem, &f_tHtab);
}

/**
 * algHSearch
 *
 * @param tItem: 欲插入哈希表的元素
 *
 * @return >0:成功
 *       NULL:失败
 *
 */
int
algHEnter(T_ITEM tItem)
{
	return algHEnterR(tItem, &f_tHtab);
}

/**
 * algHDestroy
 *
 * @return 无
 *
 */
void	algHDestroy(void)
{
	algHDestroyR(&f_tHtab);
}

/* for test */
/*
#include	<stdio.h>

#define	strSetZero(x)	memset(&(x), 0, sizeof(x))

int
main(int argc, char *argv[])
{
	char a[] = "1234567890";
	int ca1(const void *x, const void *y) { return (*(char *)x - *(char *)y); }
	int ca2(const void *x, const void *y) { return (*(char *)y - *(char *)x); }
	char b = '7';

	algSort(a, strlen(a), sizeof(char), ca1);
	printf("a[%s]", a);
	printf("[%d]", (char *)algBSearch(&b, a, strlen(a), sizeof(char), ca1)-a);
	printf("[%d]\n", (char *)algLSearch(&b, a, strlen(a), sizeof(char), ca1)-a);
	algSort(a, strlen(a), sizeof(char), ca2);
	printf("a[%s]", a);
	printf("[%d]", (char *)algBSearch(&b, a, strlen(a), sizeof(char), ca2)-a);
	printf("[%d]\n", (char *)algLSearch(&b, a, strlen(a), sizeof(char), ca1)-a);

	printf("----\n");

	char		*d[] = {
		"alpha", "bravo", "charlie", "delta", "echo", "foxtrot", "golf",
		"hotel", "india", "juliet", "kilo", "lima", "mike", "november", "oscar",
		"papa", "quebec", "romeo", "sierra", "tango", "uniform", "victor",
		"whisky", "x-ray", "yankee", "zulu"
	};
	int			r, i;
	T_HTABLE	tHtab;
	T_ITEM		tItem, *ptItem;

	strSetZero(tHtab);
	r = algHCreateR(30, &tHtab);
	if (r < 0) {
		printf("algHCreateR() error on [%d], [%d:%s]",
			   r, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < 24; i++) {
		tItem.pKey = d[i];
		tItem.pData = (void *) i;
		ptItem = algHSearchR(tItem, eEnter, &tHtab);
		if (ptItem == NULL) {
			printf("algHSearchR() error, [%d:%s]", errno, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	for (i = 22; i < 26; i++) {
		tItem.pKey = d[i];
		ptItem = algHSearchR(tItem, eFind, &tHtab);
		printf("%9.9s -> %9.9s:%d\n", tItem.pKey,
			   ptItem ? ptItem->pKey : "NULL",
			   ptItem ? (int)(ptItem->pData) : 0);
	}

	algHDestroyR(&tHtab);

	printf("----\n");

	r = algHCreate(30);
	if (r < 0) {
		printf("algHCreateR() error on [%d], [%d:%s]",
			   r, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < 24; i++) {
		tItem.pKey = d[i];
		tItem.pData = (void *) i;
		ptItem = algHSearch(tItem, eEnter);
		if (ptItem == NULL) {
			printf("algHSearchR() error, [%d:%s]", errno, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	for (i = 22; i < 26; i++) {
		tItem.pKey = d[i];
		ptItem = algHSearch(tItem, eFind);
		printf("%9.9s -> %9.9s:%d\n", tItem.pKey,
			   ptItem ? ptItem->pKey : "NULL",
			   ptItem ? (int)(ptItem->pData) : 0);
	}

	algHDestroy();

	exit(EXIT_SUCCESS);
}
*/

/*-------------------------  Local functions ----------------------------*/
static int
algHIsPrime(unsigned int iNum)
{
	/* no even number will be passed */
	unsigned int iDiv = 3;

	while (iDiv * iDiv < iNum && iNum % iDiv != 0)
		iDiv += 2;

	return iNum % iDiv != 0;
}

/*-----------------------------  End ------------------------------------*/

