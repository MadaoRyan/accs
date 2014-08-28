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
 * @desc: 获得文件大小
 *
 * @param psFileName: 文件名
 *
 * @return long: ret>=0 文件大小; ret<0 失败
 *
 */
long fsGetFileSize(char *psFileName);


/**
 * @function: fsGetNewFile
 *
 * @desc: 获取一个可用的新文件路径
 *
 * @param sPrefix: 文件路径前缀
 * @param sFile: 文件名
 *
 * @return char *: 文件路径
 *
 */
char *fsGetNewFile(char *sPrefix, char *sFile);

/**
 * @function: fsReadData
 *
 * @desc: 从文件指定位置读入的数据
 *
 * @param psFile: 文件名
 * @param iPos: 读取文件位置，0:头部 >0:从文件头开始的某个位置
 * @param psBuf: 输入buf
 * @param iLen: 从文件读取长度,0:全部 >0:iLen长度
 *
 * @return int: ret>=0 实际读入字节数; ret<0 失败
 *
 */
int fsReadData(char *psFile, int iPos, char *psBuf, int iLen);

/**
 * @function: fsWriteData
 *
 * @desc: 向文件指定位置写入buf中的数据
 *
 * @param psFile: 目标文件名
 * @param iPos: 写入目标文件位置，0:新创建,-1:追加 >0:从文件头开始的某个位置
 * @param psBuf: 输入buf
 * @param iLen: 写入数据长度
 *
 * @return int: ret>=0 实际写入字节数; ret<0 失败
 *
 */
int fsWriteData(char *psFile, int iPos, char *psBuf, int iLen);

/**
 * @function: fsJoinFile
 *
 * @desc: 连接文件
 *
 * @param psDest: 目标文件名
 * @param iPos: 写入目标文件位置，0:新创建,-1:追加 >0:从文件头开始的某个位置
 * @param psSrc: 源文件名
 * @param iInPos: 读取源文件的开始位置， 0:开头,>0:读取位置
 * @param iLen: 从源文件读取长度,0:全部 >0:iLen长度
 *
 * @return int: ret>=0 写入字节数; ret<0 失败
 *
 */
int fsJoinFile(char *psDest, int iPos, char *psSrc, int iInPos,  int iLen);

#ifdef __cplusplus
}
#endif

#endif /*_OS_FS_H_20100513171042_*/
/*-----------------------------  End ------------------------------------*/
