/*
 *
 *
 * file system functions.
 *
 *
 * FileName: os_fs.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _OS_FS_H_20100513171042_
#define _OS_FS_H_20100513171042_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: fsGetFileSize
 *
 * @desc: ����ļ���С
 *
 * @param psFileName: �ļ���
 *
 * @return long: ret>=0 �ļ���С; ret<0 ʧ��
 *
 */
long fsGetFileSize(char *psFileName);


/**
 * @function: fsGetNewFile
 *
 * @desc: ��ȡһ�����õ����ļ�·��
 *
 * @param sPrefix: �ļ�·��ǰ׺
 * @param sFile: �ļ���
 *
 * @return char *: �ļ�·��
 *
 */
char *fsGetNewFile(char *sPrefix, char *sFile);

/**
 * @function: fsReadData
 *
 * @desc: ���ļ�ָ��λ�ö��������
 *
 * @param psFile: �ļ���
 * @param iPos: ��ȡ�ļ�λ�ã�0:ͷ�� >0:���ļ�ͷ��ʼ��ĳ��λ��
 * @param psBuf: ����buf
 * @param iLen: ���ļ���ȡ����,0:ȫ�� >0:iLen����
 *
 * @return int: ret>=0 ʵ�ʶ����ֽ���; ret<0 ʧ��
 *
 */
int fsReadData(char *psFile, int iPos, char *psBuf, int iLen);

/**
 * @function: fsWriteData
 *
 * @desc: ���ļ�ָ��λ��д��buf�е�����
 *
 * @param psFile: Ŀ���ļ���
 * @param iPos: д��Ŀ���ļ�λ�ã�0:�´���,-1:׷�� >0:���ļ�ͷ��ʼ��ĳ��λ��
 * @param psBuf: ����buf
 * @param iLen: д�����ݳ���
 *
 * @return int: ret>=0 ʵ��д���ֽ���; ret<0 ʧ��
 *
 */
int fsWriteData(char *psFile, int iPos, char *psBuf, int iLen);

/**
 * @function: fsJoinFile
 *
 * @desc: �����ļ�
 *
 * @param psDest: Ŀ���ļ���
 * @param iPos: д��Ŀ���ļ�λ�ã�0:�´���,-1:׷�� >0:���ļ�ͷ��ʼ��ĳ��λ��
 * @param psSrc: Դ�ļ���
 * @param iInPos: ��ȡԴ�ļ��Ŀ�ʼλ�ã� 0:��ͷ,>0:��ȡλ��
 * @param iLen: ��Դ�ļ���ȡ����,0:ȫ�� >0:iLen����
 *
 * @return int: ret>=0 д���ֽ���; ret<0 ʧ��
 *
 */
int fsJoinFile(char *psDest, int iPos, char *psSrc, int iInPos,  int iLen);

#ifdef __cplusplus
}
#endif

#endif /*_OS_FS_H_20100513171042_*/
/*-----------------------------  End ------------------------------------*/
