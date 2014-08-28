/*
 *
 *
 *  For net.
 *
 *
 * FileName: os_net.h 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

#ifndef _OS_NET_H_20110506161402_
#define _OS_NET_H_20110506161402_

/*------------------------ Include files ------------------------*/

#if 0
#pragma mark -
#pragma mark < Macro define >
#endif
/*--------------------- Macro define ----------------------------*/

#if 0
#pragma mark -
#pragma mark < Type define >
#endif
/*--------------------- Type define -----------------------------*/
typedef struct {
    int nAll;
    int nEstablished;
    int nSynSend;
    int nSynRecv;
    int nFinWait1;
    int nFinWait2;
    int nTimeWait;
    int nClose;
    int nCloseWait;
    int nLastAck;
    int nListen;
} T_NET_TCP_STAT;

#if 0
#pragma mark -
#pragma mark < Global functions declaration >
#endif
/*--------------------- Global function declaration -------------*/
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @function: netStat
 *
 * @desc: Get Net Stat for givinng ip & port
 *
 * @param psIp:Check Ip,if psIp is Null Check Local Ip ( Ip / 0.0.0.0 / 127.0.0.1),
 * @param nPort:return All net connect info,
 * @param ptTcpStat:Check Port
 *
 * @return int: 0 - ok ; <0 - err
 *
 */
int netStat(char *psIp, int nPort, T_NET_TCP_STAT *ptTcpStat);
    
#ifdef __cplusplus
}
#endif
/*--------------------- Global variable -------------------------*/

#endif /* _OS_NET_H_20110506161402_ */
/*--------------------- End -------------------------------------*/
