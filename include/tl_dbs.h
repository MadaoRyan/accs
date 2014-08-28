/*
 *
 *
 * topframe dbs interface.
 *
 *
 * FileName: top_dbs.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _TOP_DBS_H_20110224163903_
#define _TOP_DBS_H_20110224163903_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/
/*database type*/
#define VAR_TOP_DBS_DBTYPE_ORA 0                /*Oracle数据库*/  
#define VAR_TOP_DBS_DBTYPE_DB2 1                /*DB2数据库*/
#define VAR_TOP_DBS_DBTYPE_MYSQL 2              /*MYSQL数据库*/
#define VAR_TOP_DBS_DBTYPE_SYBASE 3             /*Sybase数据库*/
#define VAR_TOP_DBS_DBTYPE_INFORMIX 4           /*Informix数据库*/
#define VAR_TOP_DBS_DBTYPE_SQLITE 5             /*Sqlite数据库*/

/* option type */
#define VAR_TOP_DBS_OPT_LOCK_WAIT	"lock_wait"

/* data type */
#define VAR_TOP_DBS_TYPE_STR 0
#define VAR_TOP_DBS_TYPE_NUM 1
#define VAR_TOP_DBS_TYPE_CHAR 2

/* ind value */
#define VAR_TOP_DBS_IND_NOTNULL 0
#define VAR_TOP_DBS_IND_NULL -1

/*callback name define*/
#define VAR_TOP_DBS_CALLBACK_CONNMISS "connmiss"  /*连接断开*/

/**/
#define RC_TOP_DBS_DBTYPE "000050000^"
#define RC_TOP_DBS_CONN "000050001^"
#define RC_TOP_DBS_DISCONN "000050002^"
#define RC_TOP_DBS_CONNOK "000050003^"

/**/
#define ERR_TOP_DBS_OK 0

#define ERR_TOP_DBS_BASE (-3000)
#define ERR_TOP_DBS_NOTFOUND (ERR_TOP_DBS_BASE-1)               /*没有数据，游标查询结束返回*/
#define ERR_TOP_DBS_DBNULL (ERR_TOP_DBS_BASE-2)                 /*有null值*/
#define ERR_TOP_DBS_PREPARE (ERR_TOP_DBS_BASE-3)                /*SQL准备错误，一般是SQL语句语法错误*/
#define ERR_TOP_DBS_OPEN (ERR_TOP_DBS_BASE-4)                   /*打开游标错误，SQL语句执行错误*/
#define ERR_TOP_DBS_FETCH (ERR_TOP_DBS_BASE-5)                  /*游标数据获取错误*/
#define ERR_TOP_DBS_CLOSE (ERR_TOP_DBS_BASE-6)                  /*关闭游标错误*/
#define ERR_TOP_DBS_CONN (ERR_TOP_DBS_BASE-7)                   /*连接数据库错误，一般是连接数据库的串错误，如：地址、用户名、密码等*/
#define ERR_TOP_DBS_DISCONN (ERR_TOP_DBS_BASE-8)                /*断开连接错误*/
#define ERR_TOP_DBS_COMMIT (ERR_TOP_DBS_BASE-9)                 /*提交事务错误*/
#define ERR_TOP_DBS_ROLLBACK (ERR_TOP_DBS_BASE-10)              /*回滚事务错误*/
#define ERR_TOP_DBS_DECLARE (ERR_TOP_DBS_BASE-11)               /*声明游标错误*/
#define ERR_TOP_DBS_ANALYSQL (ERR_TOP_DBS_BASE-12)              /*SQL分析错误， SQL语句不正确*/
                                                                /**/
#define ERR_TOP_DBS_UNIQUE (ERR_TOP_DBS_BASE-13)                /*主键重复*/
#define ERR_TOP_DBS_DISCONNECT (ERR_TOP_DBS_BASE-14)            /*数据库连接断开*/
                                                                /**/
#define ERR_TOP_DBS_NOT_IMPLEMENTED (ERR_TOP_DBS_BASE-15)       /*对应数据库实现不支持该函数调用*/
                                                                /**/
#define ERR_TOP_DBS_NOT_NULL (ERR_TOP_DBS_BASE-16)              /*不为NULL*/
                                                                /**/
#define ERR_TOP_DBS_INIT (ERR_TOP_DBS_BASE-20)                  /*初始化错误*/
#define ERR_TOP_DBS_PARAM (ERR_TOP_DBS_BASE-21)                 /*参数错误*/
#define ERR_TOP_DBS_HANDLE (ERR_TOP_DBS_BASE-22)                /*句柄错误*/
#define ERR_TOP_DBS_SPACE (ERR_TOP_DBS_BASE-23)                 /*没有可用空间，一般是游标没有关闭，致使游标耗尽*/
#define ERR_TOP_DBS_STAT (ERR_TOP_DBS_BASE-24)                  /*状态错误，一般是执行dbs函数顺序错误*/
#define ERR_TOP_DBS_ARGS (ERR_TOP_DBS_BASE-25)                  /*参数错误*/
                                                                /**/
#define ERR_TOP_DBS_UNKNOW (ERR_TOP_DBS_BASE-500)               /*未知错误*/

#define	DLEN_TABLE_COLUMN	64

/*---------------------------- Type define ------------------------------*/
typedef int T_TOP_DBS_STMT;

struct T_TOP_DBS_VAR {
    int iType;
    int iLen;
    char *psVar;
} ;

typedef struct T_TOP_DBS_VAR T_DBS_VAR;

typedef struct {
	int		iCnt;
	char	psCol[1][DLEN_TABLE_COLUMN];
} T_DBS_TABLE;

typedef struct {
	int	iStart;
	int	iCnt;
	int	iOut;
	int	iColumn;
} T_DBS_PAGE;

typedef enum {
	E_DBS_FIND,
	E_DBS_LOCK,
	E_DBS_UPDATE,
	E_DBS_DELETE,
	E_DBS_CLOSE,
	E_DBS_INSERT
} T_DBS_SVR_FUNC;

typedef struct {
	char	*sBuf;
	int		iSize;
	int		iKey;
} T_DBS_SVR_COL;

typedef int (*PFN_TOP_DBS_CALLBACK)(int iArgc, char *psArgv[]);
typedef void (*PFN_TOP_DBS_ERR_CALL)(char *);

/*---------------------- Global function declaration --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: dbsSetDbType
 *
 * @desc: 设置数据库类型, 支持: oracle db2 sybase mqsql informix
 *
 * @param iDbType: 数据库类型
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int dbsSetDbType(int iDbType);


/**
 * @function: dbsGetDbType
 * @desc: 获取当前系统使用数据库类型 
 * @return int: 0-ORACLE 1-DB2
 */

int dbsGetDbType( );

/**
 * @function: dbsSetCfgFile
 *
 * @desc: 设置配置文件
 *
 * @param psCfgFile: 配置文件
 * @param psCfgSection: 配置段
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int dbsSetCfgFile(char *psCfgFile, char *psCfgSection);

/**
 * @function: dbsConnect
 *
 * @desc: 连接数据库
 *
 * @param sConnStr:连接字符串
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsConnect(char *sConnStr);

/**
 * @function: dbsBegin
 *
 * @desc: 开启事务
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsBegin(void);

/**
 * @function: dbsCommit
 *
 * @desc: 提交事务
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsCommit(void);

/**
 * @function: dbsRollback
 *
 * @desc: 回滚事务
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsRollback(void);

/**
 * @function: dbsDisconnect
 *
 * @desc: 断开数据库
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsDisconnect(void);

/**
 * @function: dbsSqlCode
 *
 * @desc: 返回sqlcode
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSqlCode(void);

/**
 * @function: dbsSqlcaSave
 *
 * @desc: 保存当前sqlca通信区
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSqlcaSave(void);

/**
 * @function: dbsSqlcaRestore
 *
 * @desc: 恢复sqlca通信区
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSqlcaRestore(void);

/**
 * @function: dbsGetErrInfo
 *
 * @desc: 获得错误信息
 *
 * @param sBuf: 输出缓存区
 * @param iBufLen: 输出缓存区长度
 *
 * @return char *: 输出缓存区
 *
 */
char *dbsGetErrInfo(char *sBuf, int iBufLen);

/**
 * @function: dbsErrStr
 *
 * @desc: 获得错误信息
 *
 * @return char *: 错误信息
 *
 */
char *dbsErrStr(void);

/**
 * @function: dbsGetLastSql
 *
 * @desc: 获得最后一次执行的SQL语句
 *
 * @param sBuf: 输出缓存区
 * @param iBufLen: 输出缓存区长度
 *
 * @return char *: 输出缓存区
 *
 */
char *dbsGetLastSql(char *sBuf, int iBufLen);

/**
 * @function: dbsLastSql
 *
 * @desc: 获得最后一次执行的SQL语句
 *
 * @return char *: 最后一次执行的SQL语句
 *
 */
char *dbsLastSql(void);

/**
 * @function: dbsExecute
 *
 * @desc: 执行SQL语句
 *
 * @param sSql: SQL语句
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsExecute(const char *sSql);

/*------------------------ General SQL Interface ------------------------*/

/**
 * @function: dbsOperInit
 *
 * @desc: 初始化内部缓存
 *
 * @param iMaxVarNum: 最大变量个数
 * @param iMaxNameLen: 最大变量名长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsOperInit(int iMaxVarNum, int iMaxNameLen);


/**
 * @function: dbsPrepare
 *
 * @desc: 准备SQL语句
 *
 * @param sSql: SQL语句
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsPrepare(const char *sSql);

/**
 * @function: dbsBindCount
 *
 * @desc: 获得变量个数
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsBindCount(void);

/**
 * @function: dbsBindName
 *
 * @desc: 获得变量名称
 *
 * @param iCol: 变量序号
 * @param sName: 变量名称
 *
 * @return char *: 变量名称
 *
 */
char *dbsBindName(int iCol, char *sName);

/**
 * @function: dbsBindNull
 *
 * @desc: 绑定NULL至变量
 *
 * @param iCol: 变量序号
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsBindNull(int iCol);

/**
 * @function: dbsBindString
 *
 * @desc: 绑定字符串至变量
 *
 * @param iCol: 变量序号
 * @param sStr: 变量值
 * @param iLen: 变量长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsBindString(int iCol, const char *sStr, int iLen);

/**
 * @function: dbsBindArray
 *
 * @desc: 绑定数组至变量
 *
 * @param asArray[]: 数组
 * @param iCnt: 数组长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsBindArray(char *asArray[], int iCnt);

/**
 * @function: dbsBindSepValues
 *
 * @desc: 绑定特定字符分隔的字符串至变量
 *
 * @param sSep: 分隔符
 * @param sBuf: 字符串
 * @param iLen: 字符串长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsBindSepValues(char sSep, const char *sBuf, int iLen);

/**
 * @function: dbsOpen
 *
 * @desc: 打开SQL游标
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsOpen(void);

/**
 * @function: dbsColumnCount
 *
 * @desc: 获得列数
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnCount(void);

/**
 * @function: dbsColumnName
 *
 * @desc: 获得列名
 *
 * @param iCol: 列序号
 * @param sName: 列名
 *
 * @return char *: 列名
 *
 */
char *dbsColumnName(int iCol, char *sName);

/**
 * @function: dbsColumnLength
 *
 * @desc: 获得列长度
 *
 * @param iCol: 列序号
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnLength(int iCol);

/**
 * @function: dbsColumnString
 *
 * @desc: 设置string类型列输出缓存
 *
 * @param iCol: 列序号
 * @param sBuf: 缓存区
 * @param iSize: 缓存区长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnString(int iCol, char *sBuf, int iSize);

/**
 * @function: dbsColumnChar
 *
 * @desc: 设置char类型列输出缓存
 *
 * @param iCol: 列序号
 * @param sBuf: 缓存区
 * @param iSize: 缓存区长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnChar(int iCol, char *sBuf, int iSize);

/**
 * @function: dbsColumnStrings
 *
 * @desc: 设置所有string类型列至单一缓存
 *
 * @param sBuf: 缓存区
 * @param iSize: 缓存区长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnStrings(char *sBuf, int iSize);

/**
 * @function: dbsColumnChars
 *
 * @desc: 设置所有char类型列至单一缓存
 *
 * @param sBuf: 缓存区
 * @param iSize: 缓存区长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnChars(char *sBuf, int iSize);

/**
 * @function: dbsColumnBuffer
 *
 * @desc: 设置所有string类型列至内部缓存
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnBuffer(void);

/**
 * @function: dbsFetch
 *
 * @desc: 获取数据
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsFetch(void);

/**
 * @function: dbsOperSetNumberLen
 *
 * @desc: 设置默认数值类型长度
 *
 * @param nLen: 数值类型长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsOperSetNumberLen(int nLen);

/**
 * @function: dbsGetField
 *
 * @desc: 获取列信息
 *
 * @param iCol: 列序号
 * @param piLen: 列长度
 *
 * @return const char *: 列值
 *
 */
const char *dbsGetField(int iCol, int *piLen);

/**
 * @function: dbsGetInd
 *
 * @desc: 获取列指示器值
 *
 * @param iCol: 列序号
 *
 * @return int: 列指示器值
 *
 */
int dbsGetInd(int iCol);

/**
 * @function: dbsClose
 *
 * @desc: 关闭SQL游标
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsClose(void);

/* Example:
 *	**** Prepare the SQL ****
 *	iRet = dbsPrepare("select a from ab where b = :b");
 *	if (iRet < 0) {
 *		return -1;
 *	}
 *
 *	**** Bind the Input Variables ****
 *	**** Also Use dbsBindArray() and dbsBindSepValues() ****
 *	iRet = dbsBindString(1, "string", -1);
 *	if (iRet < 0) {
 *		return -1;
 *	}
 *
 *	**** Open the Cursor ****
 *	iRet = dbsOpen();
 *	if (iRet < 0) {
 *		return -1;
 *	}
 *
 *	**** Set the Output Variables ****
 *	**** Also Use dbsColumnStrings() and dbsColumnChars() ****
 *	iRet = dbsColumnString(1, sStr, sizeof(sStr));
 *	if (iRet < 0) {
 *		return -1;
 *	}
 *
 *	**** Fetch the Data ****
 *	iRet = dbsFetch();
 *	if (iRet < 0) {
 *		return -1;
 *	}
 *
 *	**** Use the Data ****
 *	printf("sStr[%s]", sStr);
 *
 *	**** Close the Cursor ****
 *	dbsClose();
 */

/*----------------------------------------------------------*/
/*增强版API*/
/*----------------------------------------------------------*/
/**
 * @function: dbsPrepareX
 *
 * @desc: 准备SQL语句
 *
 * @param phStmt: SQL句柄
 * @param sSql: SQL语句
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsPrepareX(const char *sSql, T_TOP_DBS_STMT *phStmt);

/**
 * @function: dbsBindCountX
 *
 * @desc: 获得变量个数
 *
 * @param hStmt: SQL句柄
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsBindCountX(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsBindNameX
 *
 * @desc: 获得变量名称
 *
 * @param hStmt: SQL句柄
 * @param iCol: 变量序号
 * @param sName: 变量名称
 *
 * @return char *: 变量名称
 *
 */
char *dbsBindNameX(T_TOP_DBS_STMT hStmt, int iCol, char *sName);

/**
 * @function: dbsBindNullX
 *
 * @desc: 绑定NULL至变量
 *
 * @param hStmt: SQL句柄
 * @param iCol: 变量序号
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsBindNullX(T_TOP_DBS_STMT hStmt, int iCol);

/**
 * @function: dbsBindStringX
 *
 * @desc: 绑定字符串至变量
 *
 * @param hStmt: SQL句柄
 * @param iCol: 变量序号
 * @param sStr: 变量值
 * @param iLen: 变量长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsBindStringX(T_TOP_DBS_STMT hStmt, int iCol, const char *sStr, int iLen);

/**
 * @function: dbsBindArrayX
 *
 * @desc: 绑定数组至变量
 *
 * @param hStmt: SQL句柄
 * @param asArray[]: 数组
 * @param iCnt: 数组长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsBindArrayX(T_TOP_DBS_STMT hStmt, char *asArray[], int iCnt);

/**
 * @function: dbsBindSepValuesX
 *
 * @desc: 绑定特定字符分隔的字符串至变量
 *
 * @param hStmt: SQL句柄
 * @param sSep: 分隔符
 * @param sBuf: 字符串
 * @param iLen: 字符串长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsBindSepValuesX(T_TOP_DBS_STMT hStmt, char sSep, const char *sBuf, int iLen);

/**
 * @function: dbsOpenX
 *
 * @desc: 打开SQL游标
 *
 * @param hStmt: SQL句柄
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsOpenX(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsColumnCountX
 *
 * @desc: 获得列数
 *
 * @param hStmt: SQL句柄
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnCountX(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsColumnNameX
 *
 * @desc: 获得列名
 *
 * @param hStmt: SQL句柄
 * @param iCol: 列序号
 * @param sName: 列名
 *
 * @return char *: 列名
 *
 */
char *dbsColumnNameX(T_TOP_DBS_STMT hStmt, int iCol, char *sName);

/**
 * @function: dbsColumnLengthX
 *
 * @desc: 获得列长度
 *
 * @param hStmt: SQL句柄
 * @param iCol: 列序号
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnLengthX(T_TOP_DBS_STMT hStmt, int iCol);

/**
 * @function: dbsColumnStringX
 *
 * @desc: 设置string类型列输出缓存
 *
 * @param hStmt: SQL句柄
 * @param iCol: 列序号
 * @param sBuf: 缓存区
 * @param iSize: 缓存区长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnStringX(T_TOP_DBS_STMT hStmt, int iCol, char *sBuf, int iSize);

/**
 * @function: dbsColumnCharX
 *
 * @desc: 设置char类型列输出缓存
 *
 * @param hStmt: SQL句柄
 * @param iCol: 列序号
 * @param sBuf: 缓存区
 * @param iSize: 缓存区长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnCharX(T_TOP_DBS_STMT hStmt, int iCol, char *sBuf, int iSize);

/**
 * @function: dbsColumnStringsX
 *
 * @desc: 设置所有string类型列至单一缓存
 *
 * @param hStmt: SQL句柄
 * @param sBuf: 缓存区
 * @param iSize: 缓存区长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnStringsX(T_TOP_DBS_STMT hStmt, char *sBuf, int iSize);

/**
 * @function: dbsColumnCharsX
 *
 * @param hStmt: SQL句柄
 * @desc: 设置所有char类型列至单一缓存
 *
 * @param sBuf: 缓存区
 * @param iSize: 缓存区长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnCharsX(T_TOP_DBS_STMT hStmt, char *sBuf, int iSize);

/**
 * @function: dbsColumnBufferX
 *
 * @desc: 设置所有string类型列至内部缓存
 *
 * @param hStmt: SQL句柄
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsColumnBufferX(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsFetchX
 *
 * @desc: 获取数据
 *
 * @param hStmt: SQL句柄
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsFetchX(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsOperSetNumberLenX
 *
 * @desc: 设置默认数值类型长度
 *
 * @param hStmt: SQL句柄
 * @param nLen: 数值类型长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsOperSetNumberLenX(T_TOP_DBS_STMT hStmt, int nLen);

/**
 * @function: dbsGetFieldX
 *
 * @desc: 获取列信息
 *
 * @param hStmt: SQL句柄
 * @param iCol: 列序号
 * @param piLen: 列长度
 *
 * @return const char *: 列值
 *
 */
const char *dbsGetFieldX(T_TOP_DBS_STMT hStmt, int iCol, int *piLen);

/**
 * @function: dbsGetIndX
 *
 * @desc: 获取列指示器值
 *
 * @param hStmt: SQL句柄
 * @param iCol: 列序号
 *
 * @return int: 列指示器值
 *
 */
int dbsGetIndX(T_TOP_DBS_STMT hStmt, int iCol);

/**
 * @function: dbsGetFieldXS
 *
 * @desc: 获取列信息
 *
 * @param hStmt: SQL句柄
 * @param iCol: 列序号
 * @param hStmt: 缓冲区
 * @param iCol: 缓冲区长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsGetFieldXS(T_TOP_DBS_STMT hStmt, int iCol, char *sBuf, int iSize);

/**
 * @function: dbsCloseX
 *
 * @desc: 关闭SQL游标
 *
 * @param hStmt: SQL句柄
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsCloseX(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsGetFetchArrayX
 *
 * @desc: 取一条记录，列为指针数组
 *
 * @param hStmt: SQL句柄
 * @param psColVars: 指针数组
 * @param iColCnt: 指针数组长度
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsGetFetchArrayX(T_TOP_DBS_STMT hStmt, char *psColVars[], int iColCnt);

/**
 * @function: dbsExecStmtX
 *
 * @desc: 执行SQL语句
 *
 * @param psSql: SQL语句
 * @param psBindVars: 输入变量
 * @param iBindCnt: 输入变量个数
 * @param psColVars: 输出变量
 * @param iColCnt: 输出变量个数
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsExecStmtX(char *psSql, char *psBindVars[], int iBindCnt,
                 char *psColVars[], int iColCnt);

/**
 * @function: dbsPreparePageX
 *
 * @desc: 准备分页SQL语句
 *
 * @param sSql: SQL语句
 * @param phStmt: SQL句柄
 * @param ptPage: 分页控制结构
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsPreparePageX(const char *sSql, T_TOP_DBS_STMT *phStmt,
                    T_DBS_PAGE *ptPage);

/**
 * @function: dbsFetchPageX
 *
 * @desc: 取得分页SQL语句结果
 *
 * @param phStmt: SQL句柄
 * @param ptPage: 分页控制结构
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsFetchPageX(T_TOP_DBS_STMT hStmt, T_DBS_PAGE *ptPage);

/**
 * @function: dbsCountSql
 *
 * @desc: 生成计数SQL语句
 *
 * @param sSql: 原始SQL语句
 * @param sCountSql: 计数SQL语句
 * @param iSize: 计数SQL语句缓冲区大小
 *
 * @return char *: 计数SQL语句
 *
 */
char *dbsCountSql(const char *sSql, char *sCountSql, int iSize);

/**
 * @function: dbsOpenListV
 *
 * @desc: 打开SQL游标
 *
 * @param phStmt: SQL句柄
 * @param psSql: SQL语句
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsOpenListV(T_TOP_DBS_STMT *phStmt, char *psSql, ...);

/**
 * @function: dbsOpenV
 *
 * @desc: 打开SQL游标
 *
 * @param phStmt: SQL句柄
 * @param psSql: SQL语句
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsOpenV(T_TOP_DBS_STMT *phStmt, char *sSql, ...);

/**
 * @function: dbsFetchV
 *
 * @desc: 获取数据
 *
 * @param hStmt: SQL句柄
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsFetchV(T_TOP_DBS_STMT hStmt, ...);

/**
 * @function: dbsCloseV
 *
 * @desc: 关闭SQL游标
 *
 * @param hStmt: SQL句柄
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsCloseV(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsExecuteV
 *
 * @desc: 执行SQL语句
 *
 * @param sSql: SQL语句
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsExecuteV(char *sSql, ...);

/**
 * @function: dbsSqlPrePorcListA
 *
 * @desc: 预先处理SQL语句
 *
 * @param sSql: SQL语句
 * @param iBindCnt: 变量个数
 * @param psBind: 变量
 * @param piRealBindCnt: 实际变量个数
 * @param psRealBind: 实际变量
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSqlPrePorcListA(char *sSql, int iBindCnt, char *psBind[], int *piRealBindCnt, char *psRealBind[]);

/**
 * @function: dbsOpenListA
 *
 * @desc: 打开SQL游标
 *
 * @param phStmt: SQL句柄
 * @param sSql: SQL语句
 * @param iBindCnt: 变量个数
 * @param psBind: 变量
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsOpenListA(T_TOP_DBS_STMT *phStmt, char *sSql, int iBindCnt, char *psBind[]);

/**
 * @function: dbsOpenA
 *
 * @desc: 打开SQL游标
 *
 * @param phStmt: SQL句柄
 * @param psSql: SQL语句
 * @param iBindCnt: 变量个数
 * @param psBind: 变量
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsOpenA(T_TOP_DBS_STMT *phStmt, char *sSql, int iBindCnt, char *psBind[]);

/**
 * @function: dbsFetchA
 *
 * @desc: 获取数据
 *
 * @param hStmt: SQL句柄
 * @param iSelectCnt: 列数
 * @param psSelect: 列缓冲区
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsFetchA(T_TOP_DBS_STMT hStmt, int iSelectCnt, char *psSelect[]);

/**
 * @function: dbsCloseA
 *
 * @desc: 关闭SQL游标
 *
 * @param hStmt: SQL句柄
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsCloseA(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsExecuteA
 *
 * @desc: 执行SQL语句
 *
 * @param sSql: SQL语句
 * @param iBindCnt: 变量个数
 * @param psBind: 变量
 * @param iSelectCnt: 列数
 * @param psSelect: 列缓冲区
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsExecuteA(char *sSql, int iBindCnt, char *psBind[],
                int iSelectCnt, char *psSelect[]);

/**
 * @function: dbsCountA
 *
 * @desc: 获得结果总条数
 *
 * @param sSql: SQL语句
 * @param iBindCnt: 变量个数
 * @param psBind: 变量
*
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsCountA(const char *sSql, int iBindCnt, char *psBind[]);

/**
 * @function: dbsAnalyzeTable
 *
 * @desc: 分析表结构
 *
 * @param sTable: 表名
 *
 * @return T_DBS_TABLE *: ret!=NULL 表结构; ret=NULL 失败
 *
 */
T_DBS_TABLE *dbsAnalyzeTable(char *sTable);

/**
 * @function: dbsSvrFind
 *
 * @desc: Dbsvr支持函数
 *
 * @param sSql: SQL语句
 * @param iCol: 字段个数
 * @param ptCol: 具体字段描述
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSvrFind(char *sSql, int iCol, T_DBS_SVR_COL *ptCol);

/**
 * @function: dbsSvrLock
 *
 * @desc: Dbsvr支持函数
 *
 * @param phStmt: 游标句柄
 * @param sSql: SQL语句
 * @param iCol: 字段个数
 * @param ptCol: 具体字段描述
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSvrLock(T_TOP_DBS_STMT *phStmt,
               char *sSql, int iCol, T_DBS_SVR_COL *ptCol);

/**
 * @function: dbsSvrClose
 *
 * @desc: Dbsvr支持函数
 *
 * @param hStmt: 游标句柄
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSvrClose(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsSvrUpdate
 *
 * @desc: Dbsvr支持函数
 *
 * @param sSql: SQL语句
 * @param iCol: 字段个数
 * @param ptCol: 具体字段描述
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSvrUpdate(char *sSql, int iCol, T_DBS_SVR_COL *ptCol);

/**
 * @function: dbsSvrDelete
 *
 * @desc: Dbsvr支持函数
 *
 * @param sSql: SQL语句
 * @param iCol: 字段个数
 * @param ptCol: 具体字段描述
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSvrDelete(char *sSql, int iCol, T_DBS_SVR_COL *ptCol);

/**
 * @function: dbsSvrInsert
 *
 * @desc: Dbsvr支持函数
 *
 * @param sSql: SQL语句
 * @param iCol: 字段个数
 * @param ptCol: 具体字段描述
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSvrInsert(char *sSql, int iCol, T_DBS_SVR_COL *ptCol);

/**
 * @function: dbsCloseAll
 *
 * @desc: 关闭所有已打开的SQL游标
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsCloseAll(void);

/**
 * @function: dbsSetOption
 *
 * @desc: 设置数据库选项
 *
 * @param sOption: 选项名称
 * @param sValue:  选项内容
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSetOption(char *sOption, char *sValue);

/**
 * @function: dbsSetCallBack
 *
 * @desc: 设置回调
 *
 * @param psName: 回调函数名称
 * @param pfnCallBack: 回调函数地址
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSetCallBack(char *psName, PFN_TOP_DBS_CALLBACK pfnCallBack);

/** Add by GC **/
/**
 * @function: dbsSetCheckBefore
 *
 * @desc: 设置交易前检查标志
 *
 * @param iChkFlag: 检查标志  0 不检查; 1 检查
 *
 * @return int: ret=0 成功; ret!=0 失败
 *
 */
int dbsSetCheckBefore(int iChkFlag);
int dbsSetReconnErrcode(char *psReconnErrCode);
/** End Add by GC **/

PFN_TOP_DBS_ERR_CALL dbsSetErrCall(PFN_TOP_DBS_ERR_CALL fFnc);

#ifdef __cplusplus
}
#endif

#endif /*_TOP_DBS_H_20110224163903_*/
/*-----------------------------  End ------------------------------------*/
