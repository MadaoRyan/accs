/*
 *
 *
 * String Functions
 *
 *
 * FileName: util_str.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_STR_H_20100105103547_
#define _UTIL_STR_H_20100105103547_

/*--------------------------- Include files -----------------------------*/
#include	<stddef.h>

/*--------------------------- Macro define ------------------------------*/
#ifdef  __GNUC__
#define __PRINTF__(f,s) __attribute__((format(printf, (f), (s))))
#else
#define __PRINTF__(f,s)
#endif

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: strRightTrim
 *
 * @desc: ȥ���ַ����ұߵĿո�
 *
 * @param sStr: Ҫ�������ַ���
 *
 * @return char *: ������ɵ��ַ���
 *
 */
char *strRightTrim(char *sStr);

/**
 * @function: strLeftTrim
 *
 * @desc: ȥ���ַ�����ߵĿո�
 *
 * @param sStr: Ҫ�������ַ���
 *
 * @return char *: ������ɵ��ַ���
 *
 */
char *strLeftTrim(char *sStr);

/**
 * @function: strTrim
 *
 * @desc: ȥ���ַ����������ߵĿո�
 *
 * @param sStr: Ҫ�������ַ���
 *
 * @return char *: ������ɵ��ַ���
 *
 */
char *strTrim(char *sStr);

/**
 * @function: strInt2Str
 *
 * @desc: ������ֵת�����ַ���
 *
 * @param nNum: ��ת��������ֵ
 * @param sBuf: ���������
 * @param nBufLen: �����������
 *
 * @return char *: ���������
 *
 */
char *strInt2Str(int nNum, char *sBuf, size_t nBufLen);

/**
 * @function: strStr2Int
 *
 * @desc: ���ַ���ת��������ֵ
 *
 * @param sBuf: �����ַ���
 * @param nBufLen: �����ַ�����
 *
 * @return int: ת���ɵ�����ֵ
 *
 */
int strStr2Int(char *sBuf, size_t nBufLen);

/**
 * @function: strLong2Str
 *
 * @desc: ������ֵת�����ַ���
 *
 * @param nNum: ��ת��������ֵ
 * @param sBuf: ���������
 * @param nBufLen: �����������
 *
 * @return char *: ���������
 *
 */
char *strLong2Str(long nNum, char *sBuf, size_t nBufLen);

/**
 * @function: strStr2Long
 *
 * @desc: ���ַ���ת��������ֵ
 *
 * @param sBuf: �����ַ���
 * @param nBufLen: �����ַ�����
 *
 * @return long: ת���ɵ�����ֵ
 *
 */
long strStr2Long(char *sBuf, size_t nBufLen);

/**
 * @function: strLL2Str
 *
 * @desc: ������ֵת�����ַ���
 *
 * @param nNum: ��ת��������ֵ
 * @param sBuf: ���������
 * @param nBufLen: �����������
 *
 * @return char *: ���������
 *
 */
char *strLL2Str(long long nNum, char *sBuf, size_t nBufLen);

/**
 * @function: strStr2LL
 *
 * @desc: ���ַ���ת��������ֵ
 *
 * @param sBuf: �����ַ���
 * @param nBufLen: �����ַ�����
 *
 * @return long long: ת���ɵ�����ֵ
 *
 */
long long strStr2LL(char *sBuf, size_t nBufLen);

/**
 * @function: strDbl2Str
 *
 * @desc: ������ֵת�����ַ���
 *
 * @param dNum: ��ת���ĸ���ֵ
 * @param sBuf: ���������
 * @param nBufLen: �����������
 * @param nDot: С��λ��
 *
 * @return char *: ���������
 *
 * @comment:
 * (16.0 , 5, 0) -> 00016
 * (16.12, 5, 2) -> 01612
 *
 */
char *strDbl2Str(double dNum, char *sBuf, size_t nBufLen, int nDot);

/**
 * @function: strDbl2StrRaw
 *
 * @desc: ������ֵת�����ַ���
 *
 * @param dNum: ��ת���ĸ���ֵ
 * @param sBuf: ���������
 * @param nBufLen: �����������
 * @param nDot: С��λ��
 *
 * @return char *: ���������
 *
 * @comment:
 * (16.0, 0) -> "16"
 * (16.12, 2) -> "0.16"
 * (1600, 2) -> "16.00"
 *
 */
char *strDbl2StrRaw(double dNum, char *sBuf, size_t nBufLen, int nDot);

/**
 * @function: strStr2Dbl
 *
 * @desc: ���ַ���ת���ɸ���ֵ
 *
 * @param sBuf: ���������
 * @param nBufLen: �����������
 * @param nDot: С��λ��
 *
 * @return double: ת���ɵĸ���ֵ
 *
 */
double strStr2Dbl(char *sBuf, size_t nBufLen, int nDot);

/**
 * @function: strStr2DblRaw
 *
 * @desc: ���ַ���ת���ɸ���ֵ
 *
 * @param sBuf: ���������
 * @param nBufLen: �����������
 * @param nDot: С��λ��
 *
 * @return double: ת���ɵĸ���ֵ
 *
 * @comment:
 * ("16.0" , 0) -> 16.0
 * ("16.12", 2) -> 1612.0
 * ("1600", -2) -> 16.0
 *
 */
double strStr2DblRaw(char *sBuf, size_t nBufLen, int nDot);

/**
 * @function: strMem2Str
 *
 * @desc: �����ַ���
 *
 * @param sDest: Ŀ�껺����
 * @param iDestSize: Ŀ�껺������С
 * @param sSrc: Դ����
 * @param iSrcLen: Դ���泤��
 *
 * @return char *: Ŀ�껺����
 *
 */
char *strMem2Str(char *sDest, int iDestSize, char *sSrc, int iSrcLen);

/**
 * @function: strToLower
 *
 * @desc: ���ַ����еĴ�д��ĸת��ΪСд
 *
 * @param sStr: Ҫ�������ַ���
 *
 * @return char *: ������ɵ��ַ���
 *
 */
char *strToLower(char *sStr);

/**
 * @function: strToUpper
 *
 * @desc: ���ַ����е�Сд��ĸת��Ϊ��д
 *
 * @param sStr:Ҫ�������ַ���
 *
 * @return char *: ������ɵ��ַ���
 *
 */
char *strToUpper(char *sStr);

/**
 * @function: strFirstUpper
 *
 * @desc: ���ַ�������λת��Ϊ��д, ����ת��ΪСд
 *
 * @param sStr:Ҫ�������ַ���
 *
 * @return char *: ������ɵ��ַ���
 *
 */
char *strFirstUpper(char *sStr);

/**
 * @function: strStrChr
 *
 * @desc: ��sBuf��ʼ��[��]����sBuf
 *
 * @param sBuf: ����������
 * @param pIsMatch: ƥ�亯��
 * @param pIsEnd: ��βƥ�亯��
 *
 * @return char *: ret!=NULL �ɹ�; ret=NULL ʧ��
 *
 * @comment:
 *     ƥ�亯��: ƥ��ɹ�ʱ���ط�0, ƥ��ʧ��ʱ����0
 *               ��ΪNULL, �򷵻�sBuf
 * ��βƥ�亯��: ����βʱ���ط�0, δ����βʱ����0
 *               ��ΪNULL, ��Ĭ��Ϊ����'\0'ʱ����
 *
 */
char *strStrChr(char *sBuf, int (*pIsMatch)(char), int (*pIsEnd)(char));

/**
 * @function: strStrNStr
 *
 * @desc: ���ַ����в���Ŀ���ַ���
 *
 * @param sBuf: ԭ�ַ���
 * @param iLen: ԭ�ַ�����
 * @param sTarget: Ŀ���ַ���
 *
 * @return char *: ret!=NULL �ɹ�; ret=NULL ʧ��
 *
 */
char *strStrNStr(char *sBuf, int iLen, char *sTarget);

/**
 * @function: strDirName
 *
 * @desc: ��ȡ·���е�Ŀ¼��
 *
 * @param sPath: Ҫ������·��
 *
 * @return char *: ������ɵ�·��
 *
 */
char *strDirName(char *sPath);

/**
 * @function: strBaseName
 *
 * @desc: ��ȡ·���е�Ŀ¼��
 *
 * @param sPath:Ҫ������·��
 *
 * @return char *: ������ɵ�·��
 *
 */
char *strBaseName(char *sPath);

/**
 * @function: strPathJoin
 *
 * @desc: ����·������ǰ׺
 *
 * @param sPrefix: ·��ǰ׺
 * @param sPath: Ҫ������·��
 *
 * @return char *: ������ɵ�·��
 *
 */
char *strPathJoin(char *sPrefix, char *sPath);

/**
 * @function: strPathJoin2
 *
 * @desc: ����·�������׺
 *
 * @param sPath: Ҫ������·��
 * @param sAppend: ·����׺
 *
 * @return char *: ������ɵ�·��
 *
 */
char *strPathJoin2(char *sPath, char *sAppend);

/**
 * @function: strReplaceDir
 *
 * @desc: �滻Ŀ¼��
 *
 * @param sPath: Ҫ������·��
 * @param sDir: �µ�Ŀ¼·��
 *
 * @return char *: ������ɵ�·��
 *
 */
char *strReplaceDir(char *sPath, char *sDir);

/**
 * @function: strLength
 *
 * @desc: ��ȡ�ַ�������
 *
 * @param sStr: �ַ���
 * @param iMax: ��󳤶�
 *
 * @return int: ʵ�ʳ���
 *
 */
int strLength(const char *sStr, int iMax);

/**
 * @function: strErrno
 *
 * @desc: ��ʾ��ǰerrno��Ӧ�Ĵ�����Ϣ
 *
 * @return char *: ������Ϣ
 *
 */
char *strErrno(void);

/**
 * @function: strAprintf
 *
 * @desc: ��ӡ���·�����ڴ�����
 *
 * @param sFmt: ��ʽ��Ϣ
 *
 * @return char *: ret!=NULL �ɹ�; ret=NULL ʧ��
 *
 */
char *strAprintf(const char *sFmt, ...) __PRINTF__(1,2);

/**
 * @function: strEnv
 *
 * @desc: ��û������������û��������������򷵻ؿ��ַ���
 *
 * @param sEnv: ������������
 *
 * @return const char *: ��������
 *
 */
const char *strEnv(const char *sEnv);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_STR_H_20100105103547_*/
/*-----------------------------  End ------------------------------------*/
