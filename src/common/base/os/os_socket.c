/*
 *
 * 
 * socket functions.
 * 
 * 
 * FileName: os_socket.c
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include "os_socket.h"

#include <unistd.h>

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Local function declaration ---------------------*/

/*-------------------------  Global variable ----------------------------*/

/*-------------------------  Global functions ---------------------------*/
int sockGetPeerAddr(int iSockNo, char *psAddr, int *piPort)
{
    int iRet;
    struct sockaddr_in tRemoteAddr;
    int iLen = sizeof(tRemoteAddr);
    
    memset(&tRemoteAddr, 0, sizeof(tRemoteAddr));
    
    iRet =getpeername(iSockNo,(struct sockaddr*)&tRemoteAddr, (socklen_t *)&iLen);
    if(iRet != 0) {
        fprintf(stderr, "getpeername error[%d:%s]\n", errno, strerror(errno));
        return -1;
    }
    
    strcpy(psAddr, inet_ntoa(tRemoteAddr.sin_addr));
    *piPort = ntohs(tRemoteAddr.sin_port);
    return 0;
}

int sockGetLocalAddr(int iSockNo, char *psAddr, int *piPort)
{
    int iRet;
    struct sockaddr_in tLocalAddr;
    int iLen = sizeof(tLocalAddr);
    
    memset(&tLocalAddr, 0, sizeof(tLocalAddr));
    
    iRet =getsockname(iSockNo,(struct sockaddr*)&tLocalAddr, (socklen_t *)&iLen);
    if(iRet != 0) {
        fprintf(stderr, "getpeername error[%d:%s]\n", errno, strerror(errno));
        return -1;
    }
    
    strcpy(psAddr, inet_ntoa(tLocalAddr.sin_addr));
    *piPort = ntohs(tLocalAddr.sin_port);
    
    return 0;
}

int sockIsPeerClosed(int iSockNo)
{
#ifdef REDHAT_LINUX
    int iRet = 0;
    int iLen = 0;
    struct tcp_info  tOptval;
    
    memset(&tOptval, 0, sizeof(tOptval));
    iLen = sizeof(tOptval);
    
    iRet =  getsockopt(iSockNo, IPPROTO_TCP, TCP_INFO, &tOptval, (socklen_t *)&iLen);
    if(iRet < 0) {
        fprintf(stderr, "getsockopt error[%d:%s]\n", errno, strerror(errno));
        return -1;
    }
    
    if(TCP_ESTABLISHED == tOptval.tcpi_state) {
        return 1;
    }
#endif
    return 0;
}

int sockSetRTimeOut(int iSockNo, int iSeconds)
{
    int iRet = 0;
    struct timeval tv;
    
    memset(&tv, 0, sizeof(tv));
    tv.tv_sec = iSeconds;
    tv.tv_usec = 0;
    
    iRet = setsockopt(iSockNo,  SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
    if(iRet < 0) {
        fprintf(stderr, "sockSetRTimeOut setsockopt error[%d:%s]\n",
             errno, strerror(errno));
        return -1;
    }

	return 0;
}

int sockSetSTimeOut(int iSockNo, int iSeconds)
{
    int iRet = 0;
    struct timeval tv;
    
    memset(&tv, 0, sizeof(tv));
    tv.tv_sec = iSeconds;
    tv.tv_usec = 0;
    
    iRet = setsockopt(iSockNo, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv));
    if(iRet < 0) {
        fprintf(stderr, "sockSetSTimeOut setsockopt[%d][%d] error[%d:%s]\n", 
            iSockNo, iSeconds, errno, strerror(errno));
        return -1;
    }

	return 0;
}

int sockSetAddrReuse(int iSockNo)
{
    int iRet = 0;
    int iFlg = 1;
    
    iRet = setsockopt(iSockNo, SOL_SOCKET, SO_REUSEADDR, (char *)&iFlg, sizeof(iFlg));
    if (iRet) {
        fprintf(stderr, "sockSetSTimeOut setsockopt[%d] error[%d:%s]\n", 
            iSockNo, errno, strerror(errno));
        return -1;
    }
    
    return 0;
}

int sockSetKeepAlive(int iSockNo)
{
    int iLen = sizeof(int);
    int iRet = 0;
    int iOn = 1;
    
    iRet = setsockopt(iSockNo, SOL_SOCKET, SO_KEEPALIVE, (char*)&iOn, iLen);
    if(iRet < 0) {
        fprintf(stderr, "sockSetKeepAlive setsockopt error[%d:%s]\n", 
            errno, strerror(errno));
        return -1;
    }

	return 0;
}

int sockCanRead(int iSockNo)
{
    int iRet = 0;
    fd_set tFdR;
    struct timeval tTimeOut;
    
    tTimeOut.tv_sec = 0;
    tTimeOut.tv_usec = 0;
    
    FD_ZERO(&tFdR);
    FD_SET(iSockNo, &tFdR);
    
    iRet = select(iSockNo + 1, &tFdR, NULL, NULL, &tTimeOut);
    if(-1 == iRet) {
         fprintf(stderr, "sockCanRead select error[%d:%s]\n", 
            errno, strerror(errno));
         return -1;
    } else if (0 == iRet) {
        return 0;
    }
   
    return 1;
}

int sockGetHostIp(char *psHostName, char *psIpAddr)
{
    char **pptr;
    struct hostent * pHostEnty;
    
    pHostEnty = gethostbyname(psHostName); 
    if(NULL == pHostEnty) {
        fprintf(stderr, "gethostbyname %s errstr:%s\n",  psHostName, hstrerror(h_errno));
        return -1;
    }
    
    /*使用第一个ip*/
    pptr = pHostEnty->h_addr_list;
    if(NULL == *pptr) {
        fprintf(stderr, "pHostEnty->h_addr_list ==  NULL\n");
        return -1;
    }
    
    inet_ntop(pHostEnty->h_addrtype, *pptr, psIpAddr, INET6_ADDRSTRLEN);
    
    return 0;
}
/*-------------------------  Local functions ----------------------------*/

/*-----------------------------  End ------------------------------------*/

