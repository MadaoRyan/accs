/*
 *
 * 
 * define portable os type.
 * 
 * 
 * FileName: glb_os.h
 * 
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _GLB_OS_H_20100118185412_
#define _GLB_OS_H_20100118185412_
/*--------------------------- Include files -----------------------------*/
#include "os_cli.h"
#include "os_cpu.h"
#include "os_disk.h"
#include "os_dll.h"
#include "os_env.h"
#include "os_fs.h"
#include "os_lock.h"
#include "os_memory.h"
#include "os_mmap.h"
#include "os_net.h"
#include "os_ppool.h"
#include "os_process.h"
#include "os_proc.h"
#include "os_sem.h"
#include "os_shm.h"
#include "os_socket.h"
#include "os_stream.h"
#include "os_thread.h"

/*--------------------------- Macro define ------------------------------*/
#if defined(__powerpc64__) || defined(__x86_64__) || defined(__ia64__) || defined(__s390x__)
#define GLB_OS_64_BIT
#endif
 
 
#define ERR_GLB_OS_OK 0               /*成功*/

#define ERR_GLB_OS_FAIL_BASE - 300    /*失败*/
#define ERR_GLB_OS_PARAM     (ERR_GLB_OS_FAIL_BASE - 1)  /*参数错误*/
#define ERR_GLB_OS_NOSPACE   (ERR_GLB_OS_FAIL_BASE - 2)  /*空间不足*/
#define ERR_GLB_OS_EXPIRE    (ERR_GLB_OS_FAIL_BASE - 3)  /*超时*/
#define ERR_GLB_OS_INIT      (ERR_GLB_OS_FAIL_BASE - 4)  /*初始化错误*/
#define ERR_GLB_OS_NONE      (ERR_GLB_OS_FAIL_BASE - 5)  /*空*/
#define ERR_GLB_OS_INT       (ERR_GLB_OS_FAIL_BASE - 6)  /*被中断*/
#define ERR_GLB_OS_OTHER     (ERR_GLB_OS_FAIL_BASE - 20) /*其他错误*/
/*---------------------------- Type define ------------------------------*/
#ifdef GLB_OS_64_BIT
typedef long LONG;
#else
typedef long long LONG;
#endif
/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /*_GLB_OS_H_20100118185412_*/
/*-----------------------------  End ------------------------------------*/
