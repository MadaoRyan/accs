#ifndef __COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <curses.h>
#include <ctype.h>
#include <libgen.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>

#include <regex.h>
 
#define T_LOG_INFO(LEVEL) (LEVEL), __FILE__, __LINE__, __FUNCTION__, MOUDLE_NAME

#define _TL_LOG_ "TRUE"
#define LOG_ERR      T_LOG_INFO(1)
#define LOG_WRN      T_LOG_INFO(2)
#define LOG_BUG      T_LOG_INFO(3)
#define LOG_TRC      T_LOG_INFO(4)

#define MOUDLE_NAME  "biz_trust"

#define	 MMAX		2048
#ifndef   SLKEY
#define   SLKEY       "FDC32F7756DB1E76"
#endif

#define LEN_ERRNO   8
#define DLEN_TXNNO  5
#define MSG_SIZE    2048


/*function define begin*/
int ListenRemote(int nOwnPort);

int AcceptRemote(int nListenID);

int GetMessage(int sd, char *ptr, int max);

int PutMessage(int sd, char *ptr, int nbytes);

int readn(int sd, char *ptr, int nbyte);

int writen(int sd, char *ptr, int nbyte);

char* GetPara(char *strFileName, char *strSegName, char *strParaName);

char* getDBPasswd(char *strParaFile);

void DSPToHEX(char *strDsp, char *strHex, int nCount);

void HEXToDSP(char *strHex, char *strDsp, int nCount);

void Do_DES(unsigned char *ustrSource,unsigned char *ustrDest,unsigned char *ustrKey,unsigned char *uchFlag);

void logMsg(int nLevel,char *psFile,int nLine, const char *psFunction, const char *psModuleName,char *strMsg, ...);

#endif

