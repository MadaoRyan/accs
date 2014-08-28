/*
 *
 *
 * An extended configuration parser
 *
 *
 * FileName: topcfg.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _CONFIGER_H_20111008144718_
#define _CONFIGER_H_20111008144718_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/
#define	CFG_FUN_GET			"cfgGet"

#define	CFG_SECTION_GENERAL	"general"
#define	CFG_KEY_LOCAL		"local"
#define	CFG_KEY_DEFAULT		"default"
#define	CFG_KEY_CONFIG		"cfg."
#define	CFG_KEY_REFERENCE	CFG_KEY_CONFIG "ref"

/*---------------------------- Type define ------------------------------*/
typedef int (*F_CFG_GET)(char *sFile, char *sSection, char *sKey,
                         char *sIn, char *sOut, int iSize);

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: cfgGetStr
 *
 * @desc: 从配置文件中读取字符串
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sDefault: 默认值
 * @param sBuf: 输出缓冲区
 * @param iSize: 输出缓冲区长度
 * @param sFile: 配置文件名
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int cfgGetStr(char *sSection, char *sKey, char *sDefault,
              char *sBuf, int iSize, char *sFile);

/**
 * @function: cfgGetInt
 *
 * @desc: 从配置文件中读取数值
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sDefault: 默认值
 * @param sFile: 配置文件名
 *
 * @return int: 键值
 *
 */
int cfgGetInt(char *sSection, char *sKey, int iDefault, char *sFile);

/**
 * @function: cfgGetStrL
 *
 * @desc: 从配置文件中读取字符串(local)
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sDefault: 默认值
 * @param sBuf: 输出缓冲区
 * @param iSize: 输出缓冲区长度
 * @param sFile: 配置文件名
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 * @comment:
 * 会首先读取文件sFile中CFG_LOCAL_SECTION段CFG_LOCAL_KEY的值
 * 然后依次读取该值中各个配置文件(以空格分隔)中sSection段sKey的值
 * 若取到值, 则使用该值返回
 * 若取不到, 则使用本配置文件中sSection段sKey的值返回
 *
 */
int cfgGetStrL(char *sSection, char *sKey, char *sDefault,
               char *sBuf, int iSize, char *sFile);

/**
 * @function: cfgGetIntL
 *
 * @desc: 从配置文件中读取数值(local)
 *
 * @param sSection: 段名
 * @param sKey: 键名
 * @param sDefault: 默认值
 * @param sFile: 配置文件名
 *
 * @return int: 键值
 *
 */
int cfgGetIntL(char *sSection, char *sKey, int iDefault, char *sFile);

/**
 * @function: cfgFinal
 *
 * @desc: 清理环境
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int cfgFinal(void);

/**
 * @function: cfgSetLogOff
 *
 * @desc: 使用内部log模块
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int cfgSetLogOff(void);

/**
 * @function: cfgSetLogOn
 *
 * @desc: 使用logInfo模块(默认)
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int cfgSetLogOn(void);

#ifdef __cplusplus
}
#endif

#endif /*_CONFIGER_H_20111008144718_*/
/*-----------------------------  End ------------------------------------*/
