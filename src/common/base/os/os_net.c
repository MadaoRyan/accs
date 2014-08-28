/*
 *
 *
 *  For Net status.
 *
 *
 * FileName: os_net.c 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

/*------------------------ Include files ------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>

#include "os_net.h"

#if defined (__linux)

#include <sys/types.h>

#endif

#if 0
#pragma mark -
#pragma mark < Macro define >
#endif
/*--------------------- Macro define ----------------------------*/
#define NET_LOCAL  1
#define NET_REM    2

#if 0
#pragma mark -
#pragma mark < Type define >
#endif
/*--------------------- Type define -----------------------------*/

/*--------------------- Local function declaration --------------*/
#if defined (_AIX50)

static int CheckIp(char *psBuf, char *psIp, char *psPort);
static int GetStat(char *psStat, T_NET_TCP_STAT *ptTcpStat);
static int GetLocalIp(char *psIp);

#elif defined (__linux)

static int GetStat(char *psStat, T_NET_TCP_STAT *ptTcpStat);
static int GetHexPort(char *psHexPort, int nPort);
static int GetHexIp(char *psHexIp, char *psIp);
static int CheckIp(char *psBuf, char *psIp, char *psPort);
static int GetLocalIp(char *psIp);

#endif

static char * SkipOne(char *psBuf);

/*--------------------- Global variable -------------------------*/

#if 0
#pragma mark -
#pragma mark < Global functions >
#endif
/*--------------------- Global functions ------------------------*/
#if defined (_AIX50)

/**
 * netStat
 * Get Net From System commd "netstat -an"
 * Get All Info by buffer
 *
 * @param psIp: Check Ip
 * @param nPort: Check Port
 * @param ptTcpStat: retrun All net status Info
 *
 * @return >0  : ok
 *         <0  : err
 */
int netStat(char *psIp, int nPort, T_NET_TCP_STAT *ptTcpStat)
{
    FILE *ptFp = popen("netstat -an", "r");
    if ( NULL == ptFp ) {
        return ( -1 );
    }
    
    char sTmpIp[16];
    char sTmpPort[5];
    memset(ptTcpStat, '\0', sizeof(T_NET_TCP_STAT));
    memset(sTmpIp   , '\0', sizeof(sTmpIp        ));
    memset(sTmpPort , '\0', sizeof(sTmpPort      ));
    
    snprintf(sTmpPort, sizeof(sTmpPort), "%d", nPort);
    if ( NULL == psIp ) {
         GetLocalIp(sTmpIp);
    } else {
        strncpy(sTmpIp, psIp, sizeof(sTmpIp)-1);
    }
    
    char sBuf[1024];
    while ( fgets(sBuf, sizeof(sBuf), ptFp) != NULL ) {
        char *p = sBuf;
        p = SkipOne(p);
        p = SkipOne(p);
        p = SkipOne(p); /* Get Local Ip */
        if ( NULL == psIp ) {
            if ( CheckIp(p, sTmpIp, sTmpPort) != 0 && 
                CheckIp(p, "*", sTmpPort) != 0 ) {
                continue;
            }
        }
        
        p = SkipOne(p); /* Get remort ip */
        if ( NULL != psIp ) {
            if ( CheckIp(p, sTmpIp, sTmpPort) != 0 ) {
                continue;
            }
        }
        
        p = SkipOne(p); /* Get Stat */
        GetStat(p, ptTcpStat);
    }
    
    pclose(ptFp);
    
    return (0);
}

#elif defined (__linux)

/**
 * netStat
 * Get Stat Info From /proc/net/tcp
 * if psIp is Null Check Local Ip ( Ip / 0.0.0.0 / 127.0.0.1)
 * else Check remort Ip
 * if match return status
 *
 * @param psIp: Check Ip
 * @param nPort: Check Port
 *
 * @return >0  : ok
 *         <0  : err
 */
int netStat(char *psIp, int nPort, T_NET_TCP_STAT *ptTcpStat)
{
    memset(ptTcpStat, '\0', sizeof(T_NET_TCP_STAT));
    int nFlag = NET_REM;
    if ( NULL == psIp ) {
        nFlag = NET_LOCAL;
    }

    FILE *ptFp = fopen("/proc/net/tcp", "r");
    if ( NULL == ptFp ) {
        return ( -1 );
    }
    
    char sHexIp[9];
    char sHexPort[5];
    memset(sHexIp, '\0', sizeof(sHexIp));
    memset(sHexPort, '\0', sizeof(sHexPort));
    
    GetHexIp(sHexIp, psIp);
    GetHexPort(sHexPort, nPort);
    
    char sBuf[1024];
    while ( fgets(sBuf, sizeof(sBuf), ptFp) != NULL ) {
        char *p = sBuf;
        p = SkipOne(p); /* Get Local Ip */
        if ( NET_LOCAL == nFlag ) {
            if ( CheckIp(p, sHexIp, sHexPort) != 0 && 
                CheckIp(p, "00000000", sHexPort) != 0 &&
                CheckIp(p, "0100007F", sHexPort) != 0) {
                continue;
            }
        }
        
        p = SkipOne(p); /* Get remort ip */
        if ( NET_REM == nFlag ) {
            if ( CheckIp(p, sHexIp, sHexPort) != 0 ) {
                continue;
            }
        }
        
        p = SkipOne(p); /* Get Stat */
        GetStat(p, ptTcpStat);
    }
    
    fclose(ptFp);
    
    return ( 0 );
}

#else

/**
 * netStat
 * Get Net Stat for not seport system
 *
 * @param psIp: NULL
 * @param psPort: NULL
 *
 * @return >0  : ok
 *         <0  : err
 */
int netStat(char *psIp, int nPort, T_NET_TCP_STAT *ptTcpStat)
{
    return ( 0 );
}

#endif

#if 0
#pragma mark -
#pragma mark < Local functions >
#endif
/*--------------------- Local functions -------------------------*/
#if defined (_AIX50)
/**
 * CheckIp
 * Check Ip Is curect
 * Buf is "IP:PORT"
 *
 * @param psBuf: Buf Info
 * @param psIp: Ip Info
 * @param psPort: Port Info
 *
 * @return >0  : ok
 *         <0  : err
 */
static int CheckIp(char *psBuf, char *psIp, char *psPort)
{
    int nIpLen = strlen(psIp);
    int nPortLen = strlen(psPort);
    
    if ( memcmp(psBuf, psIp, nIpLen) == 0 &&
        psBuf[nIpLen] == '.' &&
        memcmp(psBuf+nIpLen+1, psPort, nPortLen) == 0 &&
        psBuf[nIpLen + 1 + nPortLen] == ' ') {
        return (0);
    }
    
    return (-1);
}

/**
 * GetStat
 * change Stat from string(hex) to int
 *
 * @param psStat: string stat
 *
 * @return >0  : ok
 *         <0  : err
 */
static int GetStat(char *psStat, T_NET_TCP_STAT *ptTcpStat)
{
    ptTcpStat->nAll += 1;
    
    if ( memcmp("ESTABLISHED", psStat, sizeof("ESTABLISHED")-1) == 0 ) {
        ptTcpStat->nEstablished += 1;
    } else if ( memcmp("SYN_SEND", psStat, sizeof("SYN_SEND")-1) == 0 ) {
        ptTcpStat->nSynSend += 1;
    } else if ( memcmp("SYN_RECV", psStat, sizeof("SYN_RECV")-1) == 0 ) {
        ptTcpStat->nSynRecv += 1;
    } else if ( memcmp("FIN_WAIT1", psStat, sizeof("FIN_WAIT1")-1) == 0 ) {
        ptTcpStat->nFinWait1 += 1;
    } else if ( memcmp("FIN_WAIT2", psStat, sizeof("FIN_WAIT2")-1) == 0 ) {
        ptTcpStat->nFinWait2 += 1;
    } else if ( memcmp("TIME_WAIT", psStat, sizeof("TIME_WAIT")-1) == 0 ) {
        ptTcpStat->nTimeWait += 1;
    } else if ( memcmp("CLOSE_WAIT", psStat, sizeof("CLOSE_WAIT")-1) == 0 ) {
        ptTcpStat->nCloseWait += 1;
    } else if ( memcmp("CLOSE", psStat, sizeof("CLOSE")-1) == 0 ) {
        ptTcpStat->nClose += 1;
    } else if ( memcmp("LAST_ACK", psStat, sizeof("LAST_ACK")-1) == 0 ) {
        ptTcpStat->nLastAck += 1;
    } else if ( memcmp("LISTEN", psStat, sizeof("LISTEN")-1) == 0 ) {
        ptTcpStat->nListen += 1;
    }
    
    return ( 0 );
}

/**
 * GetLocalIp
 * Get Local Ip
 *
 * @param psIp: Return Ip
 *
 * @return >0  : ok
 *         <0  : err
 */
static int GetLocalIp(char *psIp)
{
    int nSocket;   
    
    if ( (nSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {  
        strcpy(psIp, "0.0.0.0");
        return (0);
    }  
    
    struct ifreq tReq;
    memset(&(tReq), '\0', sizeof(struct ifreq));
    strcpy(tReq.ifr_name, "en0");
    ioctl(nSocket, SIOCGIFADDR, &tReq);
    
    struct sockaddr_in * ptHost = (struct sockaddr_in*)&tReq.ifr_addr;
    strcpy(psIp, inet_ntoa(ptHost->sin_addr));
    close(nSocket);
    
    return (0);
}

#elif defined (__linux)

/**
 * GetHexIp
 * Change Ip To Hex Mode
 * If Ip is Null Get Local Ip
 *
 * @param psHexIp: Return Hex Ip
 * @param psIp: Org Ip
 *
 * @return >0  : ok
 *         <0  : err
 */
static int GetHexIp(char *psHexIp, char *psIp)
{
    char sTmpIp[16];
    memset(sTmpIp, '\0', sizeof(sTmpIp));
    if ( NULL == psIp ) {
        GetLocalIp(sTmpIp);
    } else {
        memcpy(sTmpIp, psIp, sizeof(sTmpIp)-1);
    }
    
    in_addr_t nIp = inet_addr(sTmpIp);
    sprintf(psHexIp, "%08X", nIp);
    
    return (0);
}

/**
 * GetHexPort
 * Get Hex Port
 * Make Int Port To Internet Foramt Port
 * Print to Hex
 *
 * @param psHexPort: Reutrn Hex Port
 * @param nPort: org Port
 *
 * @return >0  : ok
 *         <0  : err
 */
static int GetHexPort(char *psHexPort, int nPort)
{
    sprintf(psHexPort, "%04X", nPort);
    
    return (0);
}

/**
 * GetStat
 * change Stat from string(hex) to int
 *
 * @param psStat: string stat
 *
 * @return >0  : ok
 *         <0  : err
 */
static int GetStat(char *psStat, T_NET_TCP_STAT *ptTcpStat)
{
    ptTcpStat->nAll += 1;
    
    /* Check Status */
    switch ( psStat[1] ) {
        case '1':
            ptTcpStat->nEstablished += 1;
            break;
            
        case '2':
            ptTcpStat->nSynSend += 1;
            break;
            
        case '3':
            ptTcpStat->nSynRecv += 1;
            break;
            
        case '4':
            ptTcpStat->nFinWait1 += 1;
            break;
            
        case '5':
            ptTcpStat->nFinWait2 += 1;
            break;
            
        case '6':
            ptTcpStat->nTimeWait += 1;
            break;
            
        case '7':
            ptTcpStat->nClose += 1;
            break;
            
        case '8':
            ptTcpStat->nCloseWait += 1;
            break;
            
        case '9':
            ptTcpStat->nLastAck += 1;
            break;
            
        case 'a':
        case 'A':
            ptTcpStat->nListen += 1;
            break;
    }
    
    return ( 0 );
}

/**
 * CheckIp
 * Check Ip Is curect
 * Buf is "IP:PORT"
 *
 * @param psBuf: Buf Info
 * @param psIp: Ip Info
 * @param psPort: Port Info
 *
 * @return >0  : ok
 *         <0  : err
 */
static int CheckIp(char *psBuf, char *psIp, char *psPort)
{
    if ( memcmp(psBuf, psIp, 8) == 0 &&
        psBuf[8] == ':' &&
        memcmp(psBuf+9, psPort, 4) == 0 ) {
        return (0);
    }
    
    return (-1);
}

/**
 * GetLocalIp
 * Get Local Ip
 *
 * @param psIp: Return Ip
 *
 * @return >0  : ok
 *         <0  : err
 */
static int GetLocalIp(char *psIp)
{
    int nSocket;   
    
    if ( (nSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0 ) {  
        strcpy(psIp, "0.0.0.0");
        return (0);
    }  
    
    struct ifreq tReq;
    memset(&(tReq), '\0', sizeof(struct ifreq));
    strcpy(tReq.ifr_name, "eth0");
    ioctl(nSocket, SIOCGIFADDR, &tReq);
    
    struct sockaddr_in * ptHost = (struct sockaddr_in*)&tReq.ifr_addr;
    strcpy(psIp, inet_ntoa(ptHost->sin_addr));
    close(nSocket);
    
    return (0);
}

#endif

/**
 * SkipOne
 * skip One Word
 * 1: skip blank
 * 2: skip word
 * 3: skip blank
 *
 * @param psBuf: Buf
 *
 * @return >0  : ok
 *         NULL: err
 */
static char * SkipOne(char *psBuf)
{
    if ( NULL == psBuf || *psBuf == '\0' ) {
        return (psBuf);
    }
    
    char *p = psBuf;
    for (; *p == ' '; ++p);
    for (; *p != ' '; ++p);
    for (; *p == ' '; ++p);
    
    return (p);
}

/*--------------------- End -------------------------------------*/
