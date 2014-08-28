/*
 *
 *
 * OS Mmap Module
 *
 *
 * FileName: os_mmap.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include	"os_mmap.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	<fcntl.h>
#include	<sys/mman.h>
#include	<sys/stat.h>
#include	<sys/types.h>
#include	<unistd.h>

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

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/
static struct {
	char	*pMap;
	int		iFd;
	int		iSize;
} f_tMmap[OS_MMAP_MAX];

/*-------------------------  Global functions ---------------------------*/
char *
mapRead(char *sFile, int *piLen)
{
	struct stat	tStat;
	int			iRet, i;

	for (i = 0; i < arrayof(f_tMmap); i++) {
		if (!f_tMmap[i].pMap) break;
	}
	if (i >= arrayof(f_tMmap)) return NULL;

	f_tMmap[i].iFd = open(sFile, O_RDONLY);
	if (f_tMmap[i].iFd < 0) return NULL;

	iRet = fstat(f_tMmap[i].iFd, &tStat);
	if (iRet < 0) {
		close(f_tMmap[i].iFd);
		return NULL;
	}

	f_tMmap[i].iSize = tStat.st_size + 1;

	f_tMmap[i].pMap = mmap(NULL, f_tMmap[i].iSize, PROT_READ, MAP_PRIVATE,
	                       f_tMmap[i].iFd, 0);
	if (f_tMmap[i].pMap == MAP_FAILED) {
		f_tMmap[i].pMap = NULL;
		close(f_tMmap[i].iFd);
		return NULL;
	}

	if (piLen) *piLen = f_tMmap[i].iSize - 1;
	return f_tMmap[i].pMap;
}

int
mapFree(char *pMap)
{
	int	i;

	if (!pMap) return 0;

	for (i = 0; i < arrayof(f_tMmap); i++) {
		if (f_tMmap[i].pMap == pMap) break;
	}
	if (i >= arrayof(f_tMmap)) return 0;

	munmap(f_tMmap[i].pMap, f_tMmap[i].iSize);
	close(f_tMmap[i].iFd);
	f_tMmap[i].pMap = NULL;

	return 0;
}

int
mapFreeAll(void)
{
	int	i;

	for (i = 0; i < arrayof(f_tMmap); i++) {
		mapFree(f_tMmap[i].pMap);
	}

	return 0;
}

/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/
