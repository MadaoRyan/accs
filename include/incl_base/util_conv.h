/*
 *
 *
 * iconv wrapper functions
 *
 *
 * FileName: util_conv.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_CONV_H_20111209102216_
#define _UTIL_CONV_H_20111209102216_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/
#define	CONV_OPEN_MAX	8

#define	CONV_CODE_GB	"GB18030"
#define	CONV_CODE_UTF8	"UTF-8"

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: convOpen
 *
 * @desc: �򿪱���ת���ļ�
 *
 * @param sFrom: Դ����
 * @param sTo: Ŀ�����
 *
 * @return int: ret>=0 ����ת�����; ret<0 ʧ��
 *
 */
int convOpen(char *sFrom, char *sTo);

/**
 * @function: convClose
 *
 * @desc: �رձ���ת���ļ�
 *
 * @param iConv: ����ת�����
 *
 * @return int: ret>=0 �ɹ�; ret<0 ʧ��
 *
 */
int convClose(int iConv);

/**
 * @function: convCloseAll
 *
 * @desc: �ر������Ѵ򿪵ı���ת���ļ�
 *
 * @return int: ret>=0 �ɹ�; ret<0 ʧ��
 *
 */
int convCloseAll(void);

/**
 * @function: convTransform
 *
 * @desc: ����ת��
 *
 * @param iConv: ����ת�����
 * @param sIn: ���뻺��
 * @param iInLen: ���뻺�泤��
 * @param sOut: �������
 * @param iOutLen: ������泤��
 *
 * @return int: ret>=0 �������; ret<0 ʧ��
 *
 */
int convTrans(int iConv, char *sIn, int iInLen, char *sOut, int iOutLen);

/**
 * @function: convTransform
 *
 * @desc: ����ת��
 *
 * @param sFrom: Դ����
 * @param sTo: Ŀ�����
 * @param sIn: ���뻺��
 * @param iInLen: ���뻺�泤��
 * @param sOut: �������
 * @param iOutLen: ������泤��
 *
 * @return int: ret>=0 �������; ret<0 ʧ��
 *
 */
int convTransform(char *sFrom, char *sTo,
                  char *sIn, int iInLen, char *sOut, int iOutLen);

/**
 * @function: convFileTransform
 *
 * @desc: �ļ�����ת��
 *
 * @param sFrom: Դ����
 * @param sTo: Ŀ�����
 * @param sFileIn: �����ļ���
 * @param sFileOut: ����ļ���
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int convFileTransform(char *sFrom, char *sTo,
                      char *sFileIn, char *sFileOut);

/**
 * @function: convGBtoUTF8
 *
 * @desc: ��GB18030����ת����UTF-8����
 *
 * @param sIn: ���뻺��
 * @param iInLen: ���뻺�泤��
 * @param sOut: �������
 * @param iOutLen: ������泤��
 *
 * @return int: ret>=0 �������; ret<0 ʧ��
 *
 */
int convGBtoUTF8(char *sIn, int iInLen, char *sOut, int iOutLen);

/**
 * @function: convUTF8toGB
 *
 * @desc: ��UTF-8����ת����GB18030����
 *
 * @param sIn: ���뻺��
 * @param iInLen: ���뻺�泤��
 * @param sOut: �������
 * @param iOutLen: ������泤��
 *
 * @return int: ret>=0 �������; ret<0 ʧ��
 *
 */
int convUTF8toGB(char *sIn, int iInLen, char *sOut, int iOutLen);

/**
 * @function: convTransformS
 *
 * @desc: ����ת��(�ð汾�����'\0')
 *
 * @param sFrom: Դ����
 * @param sTo: Ŀ�����
 * @param sIn: ���뻺��
 * @param iInLen: ���뻺�泤��
 * @param sOut: �������
 * @param iOutLen: ������泤��
 *
 * @return int: ret>=0 �������; ret<0 ʧ��
 *
 */
int convTransformS(char *sFrom, char *sTo,
                   char *sIn, int iInLen, char *sOut, int iOutLen);

/**
 * @function: convGBtoUTF8s
 *
 * @desc: ��GB18030����ת����UTF-8����(�ð汾�����'\0')
 *
 * @param sIn: ���뻺��
 * @param iInLen: ���뻺�泤��
 * @param sOut: �������
 * @param iOutLen: ������泤��
 *
 * @return int: ret>=0 �������; ret<0 ʧ��
 *
 */
int convGBtoUTF8s(char *sIn, int iInLen, char *sOut, int iOutLen);

/**
 * @function: convUTF8toGBs
 *
 * @desc: ��UTF-8����ת����GB18030����(�ð汾�����'\0')
 *
 * @param sIn: ���뻺��
 * @param iInLen: ���뻺�泤��
 * @param sOut: �������
 * @param iOutLen: ������泤��
 *
 * @return int: ret>=0 �������; ret<0 ʧ��
 *
 */
int convUTF8toGBs(char *sIn, int iInLen, char *sOut, int iOutLen);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_CONV_H_20111209102216_*/
/*-----------------------------  End ------------------------------------*/
