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
 * @desc: �������ļ��ж�ȡ�ַ���
 *
 * @param sSection: ����
 * @param sKey: ����
 * @param sDefault: Ĭ��ֵ
 * @param sBuf: ���������
 * @param iSize: �������������
 * @param sFile: �����ļ���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int cfgGetStr(char *sSection, char *sKey, char *sDefault,
              char *sBuf, int iSize, char *sFile);

/**
 * @function: cfgGetInt
 *
 * @desc: �������ļ��ж�ȡ��ֵ
 *
 * @param sSection: ����
 * @param sKey: ����
 * @param sDefault: Ĭ��ֵ
 * @param sFile: �����ļ���
 *
 * @return int: ��ֵ
 *
 */
int cfgGetInt(char *sSection, char *sKey, int iDefault, char *sFile);

/**
 * @function: cfgGetStrL
 *
 * @desc: �������ļ��ж�ȡ�ַ���(local)
 *
 * @param sSection: ����
 * @param sKey: ����
 * @param sDefault: Ĭ��ֵ
 * @param sBuf: ���������
 * @param iSize: �������������
 * @param sFile: �����ļ���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 * @comment:
 * �����ȶ�ȡ�ļ�sFile��CFG_LOCAL_SECTION��CFG_LOCAL_KEY��ֵ
 * Ȼ�����ζ�ȡ��ֵ�и��������ļ�(�Կո�ָ�)��sSection��sKey��ֵ
 * ��ȡ��ֵ, ��ʹ�ø�ֵ����
 * ��ȡ����, ��ʹ�ñ������ļ���sSection��sKey��ֵ����
 *
 */
int cfgGetStrL(char *sSection, char *sKey, char *sDefault,
               char *sBuf, int iSize, char *sFile);

/**
 * @function: cfgGetIntL
 *
 * @desc: �������ļ��ж�ȡ��ֵ(local)
 *
 * @param sSection: ����
 * @param sKey: ����
 * @param sDefault: Ĭ��ֵ
 * @param sFile: �����ļ���
 *
 * @return int: ��ֵ
 *
 */
int cfgGetIntL(char *sSection, char *sKey, int iDefault, char *sFile);

/**
 * @function: cfgFinal
 *
 * @desc: ������
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int cfgFinal(void);

/**
 * @function: cfgSetLogOff
 *
 * @desc: ʹ���ڲ�logģ��
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int cfgSetLogOff(void);

/**
 * @function: cfgSetLogOn
 *
 * @desc: ʹ��logInfoģ��(Ĭ��)
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int cfgSetLogOn(void);

#ifdef __cplusplus
}
#endif

#endif /*_CONFIGER_H_20111008144718_*/
/*-----------------------------  End ------------------------------------*/
