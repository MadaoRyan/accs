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

#include	"poc_object.h"

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
#define	_				logInfo

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
int
cfgGet(char *sFile, char *sSection, char *sKey,
       char *sIn, char *sOut, int iSize)
{
	sOut[POC_GetValueS(sIn, sOut, iSize - 1)] = '\0';
	return 0;
}

/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/
