/*
 *
 * 
 * socket functions.
 * 
 * 
 * FileName: os_socket.h
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_SOCKET_H_20100118173010_
#define _OS_SOCKET_H_20100118173010_
/*--------------------------- Include files -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/
/*typedef enum {
    E_SockTcpIp = 0;
    E_SockUnix;
} E_SOCKET_TYPE;*/


/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: sockGetPeerAddr
 *
 * @desc: 获取对端IP和端口
 *
 * @param iSockNo: socket描述符
 * @param psAddr: 输出参数，对方Ip
 * @param piPort: 输出参数，对方端口
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int sockGetPeerAddr(int iSockNo, char *psAddr, int *piPort);

/**
 * @function: sockGetLocalAddr
 *
 * @desc: 获取本地地址
 *
 * @param iSockNo: socket描述符
 * @param psAddr: 输出参数，本地Ip
 * @param piPort: 输出参数，本地端口
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int sockGetLocalAddr(int iSockNo, char *psAddr, int *piPort);

/**
 * @function: sockCanRead
 *
 * @desc: 端口是否可读
 *
 * @param iSockNo:端口描述符
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int sockCanRead(int iSockNo);

/**
 * @function: sockIsPeerClosed
 *
 * @desc: 确定对端是否关闭,只有在linux上可用
 *
 * @param iSockNo: 端口描述符
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int sockIsPeerClosed(int iSockNo);

/**
 * @function: sockSetRTimeOut
 *
 * @desc: 设置接收超时时间
 *
 * @param iSockNo: 端口描述符
 * @param iSeconds: 超时时间
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int sockSetRTimeOut(int iSockNo, int iSeconds);

/**
 * @function: sockSetSTimeOut
 *
 * @desc: 设置发送超时时间
 *
 * @param iSockNo: 端口描述符
 * @param iSeconds: 超时时间
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int sockSetSTimeOut(int iSockNo, int iSeconds);

/**
 * @function: sockSetAddrReuse
 *
 * @desc: 设置地址重用
 *
 * @param iSockNo: socket文件描述符
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int sockSetAddrReuse(int iSockNo);

/**
 * @function: sockSetKeepAlive
 *
 * @desc: 设置KeepAlive
 *
 * @param iSockNo: 端口描述符
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int sockSetKeepAlive(int iSockNo);

/**
 * @function: sockGetHostIp
 *
 * @desc: 获取主机IP
 *
 * @param psHostName: 输入参数，主机名
 * @param psIpAddr: 输出参数，主机Ip
 *
 * @return int: 0:成功 <0:出错
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int sockGetHostIp(char *psHostName, char *psIpAddr);


#ifdef __cplusplus
}
#endif

#endif /*_OS_SOCKET_H_20100118173010_*/
/*-----------------------------  End ------------------------------------*/
