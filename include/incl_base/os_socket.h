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
 * @desc: ��ȡ�Զ�IP�Ͷ˿�
 *
 * @param iSockNo: socket������
 * @param psAddr: ����������Է�Ip
 * @param piPort: ����������Է��˿�
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ��ȡ���ص�ַ
 *
 * @param iSockNo: socket������
 * @param psAddr: �������������Ip
 * @param piPort: ������������ض˿�
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: �˿��Ƿ�ɶ�
 *
 * @param iSockNo:�˿�������
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ȷ���Զ��Ƿ�ر�,ֻ����linux�Ͽ���
 *
 * @param iSockNo: �˿�������
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ���ý��ճ�ʱʱ��
 *
 * @param iSockNo: �˿�������
 * @param iSeconds: ��ʱʱ��
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ���÷��ͳ�ʱʱ��
 *
 * @param iSockNo: �˿�������
 * @param iSeconds: ��ʱʱ��
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ���õ�ַ����
 *
 * @param iSockNo: socket�ļ�������
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ����KeepAlive
 *
 * @param iSockNo: �˿�������
 *
 * @return int: 0:�ɹ� <0:����
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
 * @desc: ��ȡ����IP
 *
 * @param psHostName: ���������������
 * @param psIpAddr: �������������Ip
 *
 * @return int: 0:�ɹ� <0:����
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
