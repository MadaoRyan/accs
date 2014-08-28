/*
 *
 *
 *  For Os Porc.
 *
 *
 * FileName: os_proc.h 
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 *
 */

#ifndef _OS_PROC_H_20110509150802_
#define _OS_PROC_H_20110509150802_

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

#if 0
#pragma mark -
#pragma mark < Global functions declaration >
#endif
/*--------------------- Global function declaration -------------*/
#ifdef __cplusplus
extern "C" {
#endif
    

/**
 * @function: prcGetProcNum
 *
 * @desc: Get Proc Num of Process Name
 *
 * @param psProcName:Porcess Name
 *
 * @return int: 
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
    int prcGetProcNum(char *psProcName);

/**
 * @function: prcGetProcName
 *
 * @desc: Get Process Name By Process Id
 *
 * @param nPid:Porcess Id,
 * @param psProcName:Return Process Name Buffer,
 * @param nNameLen:Return Buffer Max Len
 *
 * @return int: 
 *
 * @comment:
 *
 *
 *
 * @sample:
 *
 *
 */
    int prcGetProcName(int nPid, char *psProcName, int nNameLen);

#ifdef __cplusplus
}
#endif
/*--------------------- Global variable -------------------------*/

#endif /* _OS_PROC_H_20110509150802_ */
/*--------------------- End -------------------------------------*/
