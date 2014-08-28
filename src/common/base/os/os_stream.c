/*
 *
 *
 * stream operate functions.
 *
 *
 * FileName: os_stream.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include "os_stream.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <signal.h>
#include <sys/stat.h>
#include<sys/ioctl.h>

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/
typedef struct List{
    char *p;
    struct List *ptPre;
    struct List *ptNext;
} T_List;

/*---------------------- Local function declaration ---------------------*/
static void _reset(int i);
static char * _fgets(char *sBuf, int iLen, int iWide);
char * streamGets(char *psBuf, int iLen);
static void _printf(char *psBuf, int iOffset, int iWide);

/*-------------------------  Global variable ----------------------------*/
static T_List *g_ptList = NULL;
static struct termios g_OldTerm;
static struct termios g_NewTerm;

/*-------------------------  Global functions ---------------------------*/
int streamWriteNByte(void *pBuf, int iLen, int iFileNo)
{
    int iWriteLen = 0;
    int iRet = 0;

    while(iWriteLen < iLen) {
        iRet = write(iFileNo, pBuf+iWriteLen, iLen - iWriteLen);
        if(iRet <= 0) {
            fprintf(stderr, "write error[%d:%s] len[%d] writelen[%d]\n",
                    errno, strerror(errno), iLen, iWriteLen);
            return iWriteLen;
        }

        iWriteLen += iRet;
    }

    return iWriteLen;
}

int streamReadNByte(void *pBuf, int iLen, int *piReadLen, int iFileNo)
{
    int iReadLen = 0;
    int iRet = 0;

    while(iReadLen < iLen) {
        iRet = read(iFileNo, pBuf+iReadLen, iLen - iReadLen);
        if(-1 == iRet) {
            fprintf(stderr, "read error[%d:%s] len[%d] readlen[%d]\n",
                    errno, strerror(errno), iLen, iReadLen);
            return -1;
        } else if(0 == iRet) {
            return 0;
        }

        iReadLen += iRet;

        *piReadLen = iReadLen;
    }

    return iReadLen;
}

int streamWriteLine(void *pBuf, int iLen, int iFileNo)
{
    return -1;
}

int streamReadLine(void *pBuf, int iLen, int *piReadLen, int iFileNo)
{
    return -1;
}

int streamWriteNbit(void *pBuf, int iBitCnt, int iFileNo)
{
    return -1;
}

int streamReadNbit(void *pBuf, int iBitCnt, int *piReadBitCnt, int iFileNo)
{
    return -1;
}

char * streamGets(char *psBuf, int iLen)
{
    struct stat tStatBuf;
    memset(&(tStatBuf), '\0', sizeof(tStatBuf));
    int iRet = fstat(STDIN_FILENO, &tStatBuf);
    if ( iRet != 0 ) {
        return NULL;
    }

    if (!S_ISCHR(tStatBuf.st_mode)) {
        return fgets(psBuf, iLen, stdin);
    }

    struct winsize tWinSize;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &tWinSize);

    sigset(SIGINT, _reset);

    tcgetattr (0, &g_OldTerm);
    memcpy(&g_NewTerm, &g_OldTerm, sizeof(g_OldTerm));
    g_NewTerm.c_lflag &= ~(ICANON | ECHO);
    g_NewTerm.c_cc[VTIME] = 0;
    g_NewTerm.c_cc[VMIN] = 1;
    tcsetattr (0, TCSANOW, &g_NewTerm);

    char *pRet = _fgets(psBuf, iLen, tWinSize.ws_col);

    tcsetattr (0, TCSANOW, &g_OldTerm);
    sigset(SIGINT, NULL);

    return pRet;
}

/*-------------------------  Local functions ----------------------------*/
static char * _fgets(char *sBuf, int iBufLen, int iWide)
{
	memset(sBuf, '\0', iBufLen);
	T_List *p = NULL;

    _printf(NULL, 0, 0);

	int i = 0;
    while(1) {
        _printf(sBuf, i, iWide);
        unsigned char a = getchar();

        /* end of file */
		if ( 255 == a ) {
			return NULL;
		}

        /* enter */
        if ( 10 == a ) {
            printf("\n");
            break;
		}

        /* specal key */
        if (27 == a) { /* ESC */
            a = getchar();
            if ( 91 == a ) { /* [ */
                a = getchar();

                /* up : get last cmd */
                if ( 65 == a ) {
                    if (NULL == p) {
                        if (NULL == g_ptList) {
                            continue;
                        }
                        p = g_ptList;
                    } else {
                        if (NULL == p->ptNext) {
                            continue;
                        }
                        p = p->ptNext;
                    }

                    i = strlen(p->p);
                    memset(sBuf, '\0', iBufLen);
                    strcpy(sBuf, p->p);
                    continue;
                }

                /* down : get next cmd */
                if ( 66 == a ) {
                    if (NULL == p ) {
                        continue;
                    } else if (NULL == p->ptPre) {
                        p = NULL;
                        i = 0;
                        memset(sBuf, '\0', iBufLen);
                        continue;
                    }

                    p = p->ptPre;
                    i = strlen(p->p);
                    memset(sBuf, '\0', iBufLen);
                    strcpy(sBuf, p->p);
                    continue;
                }

                /* right : move cursor right */
                if (67 == a) {
                    if (i < strlen(sBuf)) {
                        i += 1;
                    }
                    continue;
                }

                /* lift : move cursor lift */
                if (68 == a) {
                    if (i > 0) {
                        i -= 1;
                    }
                    continue;
                }
            }

            /* ignore other */
            continue;
        }

        /* back space */
		if ( 8 == a ) {
			if ( i > 0 ) {
                --i;
                memmove(sBuf + i, sBuf + i + 1, strlen(sBuf) - i);
			}
            continue;
		}

        /* normal key */
        memmove(sBuf + i + 1, sBuf + i, strlen(sBuf) - i);
		sBuf[i++] = a;
        if (i >= iBufLen - 1) {
            break;
        }
    }

    /* store cmd */
    T_List *q = (T_List *)malloc(sizeof(T_List));
    q->p = strdup(sBuf);

    q->ptPre = NULL;
    q->ptNext = g_ptList;
    if (NULL != g_ptList) {
        g_ptList->ptPre = q;
    }
    g_ptList = q;

    return sBuf;
}

static void _reset(int i)
{
	tcsetattr(0, TCSANOW, &g_OldTerm);
	exit(0);
}

static void _printf(char *psBuf, int iOffset, int iWide)
{
    static int s_iOldOffset = 0;

    /* init */
    if (NULL == psBuf) {
        s_iOldOffset = 0;
        return;
    }

    /* clean */
    int iHighOffset = s_iOldOffset / iWide;
    if ( iHighOffset > 0 ) {
        printf("\033[%dA", iHighOffset);
    }

    printf("\033[%dD\033[J", iWide);

    /* printf */
    printf(psBuf);

    /* if buffer is full of screen
     printf one blank let cursor to next line */
    int iBufLen = strlen(psBuf);
    if (0 == iBufLen % iWide) {
        printf(" \033[D");
    }

    /* now cursor is end of buf, move cursor to right line */
    iHighOffset = (iBufLen/iWide) - (iOffset/iWide);
    if (iHighOffset > 0) {
        printf("\033[%dA", iHighOffset);
    }

    /* move cursor to right postion */
    int iWideOffset = (iBufLen%iWide) - (iOffset%iWide);
    if (iWideOffset > 0) {
        printf("\033[%dD", iWideOffset);
    }
    
    if (iWideOffset < 0) {
        printf("\033[%dC", -iWideOffset);
    }
    
    s_iOldOffset = iOffset;
    return ;
}

/*-----------------------------  End ------------------------------------*/

