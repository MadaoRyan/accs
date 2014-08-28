/*
 *
 * 
 * command line interface.
 * 
 * 
 * FileName: os_cli.h
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_CLI_H_20111130145006_
#define _OS_CLI_H_20111130145006_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: cliExecCmd
 *
 * @desc: ִ������
 *
 * @param psCmd:����
 * @param psOutPut:��׼���buf
 * @param piOutLen:�������������
 * @param iOption:ѡ��
 *
 * @return int:  >=0 sucess, <0 error
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
int cliExecCmd(char *psCmd, char *psOutPut, int *piOutLen, int iOption);

#ifdef __cplusplus
}
#endif

#endif /*_OS_CLI_H_20111130145006_*/
/*-----------------------------  End ------------------------------------*/
