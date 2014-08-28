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
#define VAR_TOP_DBS_DBTYPE_ORA 0                /*Oracle���ݿ�*/  
#define VAR_TOP_DBS_DBTYPE_DB2 1                /*DB2���ݿ�*/
#define VAR_TOP_DBS_DBTYPE_MYSQL 2              /*MYSQL���ݿ�*/
#define VAR_TOP_DBS_DBTYPE_SYBASE 3             /*Sybase���ݿ�*/
#define VAR_TOP_DBS_DBTYPE_INFORMIX 4           /*Informix���ݿ�*/
#define VAR_TOP_DBS_DBTYPE_SQLITE 5             /*Sqlite���ݿ�*/

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
#define VAR_TOP_DBS_CALLBACK_CONNMISS "connmiss"  /*���ӶϿ�*/

/**/
#define RC_TOP_DBS_DBTYPE "000050000^"
#define RC_TOP_DBS_CONN "000050001^"
#define RC_TOP_DBS_DISCONN "000050002^"
#define RC_TOP_DBS_CONNOK "000050003^"

/**/
#define ERR_TOP_DBS_OK 0

#define ERR_TOP_DBS_BASE (-3000)
#define ERR_TOP_DBS_NOTFOUND (ERR_TOP_DBS_BASE-1)               /*û�����ݣ��α��ѯ��������*/
#define ERR_TOP_DBS_DBNULL (ERR_TOP_DBS_BASE-2)                 /*��nullֵ*/
#define ERR_TOP_DBS_PREPARE (ERR_TOP_DBS_BASE-3)                /*SQL׼������һ����SQL����﷨����*/
#define ERR_TOP_DBS_OPEN (ERR_TOP_DBS_BASE-4)                   /*���α����SQL���ִ�д���*/
#define ERR_TOP_DBS_FETCH (ERR_TOP_DBS_BASE-5)                  /*�α����ݻ�ȡ����*/
#define ERR_TOP_DBS_CLOSE (ERR_TOP_DBS_BASE-6)                  /*�ر��α����*/
#define ERR_TOP_DBS_CONN (ERR_TOP_DBS_BASE-7)                   /*�������ݿ����һ�����������ݿ�Ĵ������磺��ַ���û����������*/
#define ERR_TOP_DBS_DISCONN (ERR_TOP_DBS_BASE-8)                /*�Ͽ����Ӵ���*/
#define ERR_TOP_DBS_COMMIT (ERR_TOP_DBS_BASE-9)                 /*�ύ�������*/
#define ERR_TOP_DBS_ROLLBACK (ERR_TOP_DBS_BASE-10)              /*�ع��������*/
#define ERR_TOP_DBS_DECLARE (ERR_TOP_DBS_BASE-11)               /*�����α����*/
#define ERR_TOP_DBS_ANALYSQL (ERR_TOP_DBS_BASE-12)              /*SQL�������� SQL��䲻��ȷ*/
                                                                /**/
#define ERR_TOP_DBS_UNIQUE (ERR_TOP_DBS_BASE-13)                /*�����ظ�*/
#define ERR_TOP_DBS_DISCONNECT (ERR_TOP_DBS_BASE-14)            /*���ݿ����ӶϿ�*/
                                                                /**/
#define ERR_TOP_DBS_NOT_IMPLEMENTED (ERR_TOP_DBS_BASE-15)       /*��Ӧ���ݿ�ʵ�ֲ�֧�ָú�������*/
                                                                /**/
#define ERR_TOP_DBS_NOT_NULL (ERR_TOP_DBS_BASE-16)              /*��ΪNULL*/
                                                                /**/
#define ERR_TOP_DBS_INIT (ERR_TOP_DBS_BASE-20)                  /*��ʼ������*/
#define ERR_TOP_DBS_PARAM (ERR_TOP_DBS_BASE-21)                 /*��������*/
#define ERR_TOP_DBS_HANDLE (ERR_TOP_DBS_BASE-22)                /*�������*/
#define ERR_TOP_DBS_SPACE (ERR_TOP_DBS_BASE-23)                 /*û�п��ÿռ䣬һ�����α�û�йرգ���ʹ�α�ľ�*/
#define ERR_TOP_DBS_STAT (ERR_TOP_DBS_BASE-24)                  /*״̬����һ����ִ��dbs����˳�����*/
#define ERR_TOP_DBS_ARGS (ERR_TOP_DBS_BASE-25)                  /*��������*/
                                                                /**/
#define ERR_TOP_DBS_UNKNOW (ERR_TOP_DBS_BASE-500)               /*δ֪����*/

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
 * @desc: �������ݿ�����, ֧��: oracle db2 sybase mqsql informix
 *
 * @param iDbType: ���ݿ�����
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int dbsSetDbType(int iDbType);


/**
 * @function: dbsGetDbType
 * @desc: ��ȡ��ǰϵͳʹ�����ݿ����� 
 * @return int: 0-ORACLE 1-DB2
 */

int dbsGetDbType( );

/**
 * @function: dbsSetCfgFile
 *
 * @desc: ���������ļ�
 *
 * @param psCfgFile: �����ļ�
 * @param psCfgSection: ���ö�
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int dbsSetCfgFile(char *psCfgFile, char *psCfgSection);

/**
 * @function: dbsConnect
 *
 * @desc: �������ݿ�
 *
 * @param sConnStr:�����ַ���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsConnect(char *sConnStr);

/**
 * @function: dbsBegin
 *
 * @desc: ��������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsBegin(void);

/**
 * @function: dbsCommit
 *
 * @desc: �ύ����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsCommit(void);

/**
 * @function: dbsRollback
 *
 * @desc: �ع�����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsRollback(void);

/**
 * @function: dbsDisconnect
 *
 * @desc: �Ͽ����ݿ�
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsDisconnect(void);

/**
 * @function: dbsSqlCode
 *
 * @desc: ����sqlcode
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsSqlCode(void);

/**
 * @function: dbsSqlcaSave
 *
 * @desc: ���浱ǰsqlcaͨ����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsSqlcaSave(void);

/**
 * @function: dbsSqlcaRestore
 *
 * @desc: �ָ�sqlcaͨ����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsSqlcaRestore(void);

/**
 * @function: dbsGetErrInfo
 *
 * @desc: ��ô�����Ϣ
 *
 * @param sBuf: ���������
 * @param iBufLen: �������������
 *
 * @return char *: ���������
 *
 */
char *dbsGetErrInfo(char *sBuf, int iBufLen);

/**
 * @function: dbsErrStr
 *
 * @desc: ��ô�����Ϣ
 *
 * @return char *: ������Ϣ
 *
 */
char *dbsErrStr(void);

/**
 * @function: dbsGetLastSql
 *
 * @desc: ������һ��ִ�е�SQL���
 *
 * @param sBuf: ���������
 * @param iBufLen: �������������
 *
 * @return char *: ���������
 *
 */
char *dbsGetLastSql(char *sBuf, int iBufLen);

/**
 * @function: dbsLastSql
 *
 * @desc: ������һ��ִ�е�SQL���
 *
 * @return char *: ���һ��ִ�е�SQL���
 *
 */
char *dbsLastSql(void);

/**
 * @function: dbsExecute
 *
 * @desc: ִ��SQL���
 *
 * @param sSql: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsExecute(const char *sSql);

/*------------------------ General SQL Interface ------------------------*/

/**
 * @function: dbsOperInit
 *
 * @desc: ��ʼ���ڲ�����
 *
 * @param iMaxVarNum: ����������
 * @param iMaxNameLen: ������������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsOperInit(int iMaxVarNum, int iMaxNameLen);


/**
 * @function: dbsPrepare
 *
 * @desc: ׼��SQL���
 *
 * @param sSql: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsPrepare(const char *sSql);

/**
 * @function: dbsBindCount
 *
 * @desc: ��ñ�������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsBindCount(void);

/**
 * @function: dbsBindName
 *
 * @desc: ��ñ�������
 *
 * @param iCol: �������
 * @param sName: ��������
 *
 * @return char *: ��������
 *
 */
char *dbsBindName(int iCol, char *sName);

/**
 * @function: dbsBindNull
 *
 * @desc: ��NULL������
 *
 * @param iCol: �������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsBindNull(int iCol);

/**
 * @function: dbsBindString
 *
 * @desc: ���ַ���������
 *
 * @param iCol: �������
 * @param sStr: ����ֵ
 * @param iLen: ��������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsBindString(int iCol, const char *sStr, int iLen);

/**
 * @function: dbsBindArray
 *
 * @desc: ������������
 *
 * @param asArray[]: ����
 * @param iCnt: ���鳤��
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsBindArray(char *asArray[], int iCnt);

/**
 * @function: dbsBindSepValues
 *
 * @desc: ���ض��ַ��ָ����ַ���������
 *
 * @param sSep: �ָ���
 * @param sBuf: �ַ���
 * @param iLen: �ַ�������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsBindSepValues(char sSep, const char *sBuf, int iLen);

/**
 * @function: dbsOpen
 *
 * @desc: ��SQL�α�
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsOpen(void);

/**
 * @function: dbsColumnCount
 *
 * @desc: �������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnCount(void);

/**
 * @function: dbsColumnName
 *
 * @desc: �������
 *
 * @param iCol: �����
 * @param sName: ����
 *
 * @return char *: ����
 *
 */
char *dbsColumnName(int iCol, char *sName);

/**
 * @function: dbsColumnLength
 *
 * @desc: ����г���
 *
 * @param iCol: �����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnLength(int iCol);

/**
 * @function: dbsColumnString
 *
 * @desc: ����string�������������
 *
 * @param iCol: �����
 * @param sBuf: ������
 * @param iSize: ����������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnString(int iCol, char *sBuf, int iSize);

/**
 * @function: dbsColumnChar
 *
 * @desc: ����char�������������
 *
 * @param iCol: �����
 * @param sBuf: ������
 * @param iSize: ����������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnChar(int iCol, char *sBuf, int iSize);

/**
 * @function: dbsColumnStrings
 *
 * @desc: ��������string����������һ����
 *
 * @param sBuf: ������
 * @param iSize: ����������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnStrings(char *sBuf, int iSize);

/**
 * @function: dbsColumnChars
 *
 * @desc: ��������char����������һ����
 *
 * @param sBuf: ������
 * @param iSize: ����������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnChars(char *sBuf, int iSize);

/**
 * @function: dbsColumnBuffer
 *
 * @desc: ��������string���������ڲ�����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnBuffer(void);

/**
 * @function: dbsFetch
 *
 * @desc: ��ȡ����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsFetch(void);

/**
 * @function: dbsOperSetNumberLen
 *
 * @desc: ����Ĭ����ֵ���ͳ���
 *
 * @param nLen: ��ֵ���ͳ���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsOperSetNumberLen(int nLen);

/**
 * @function: dbsGetField
 *
 * @desc: ��ȡ����Ϣ
 *
 * @param iCol: �����
 * @param piLen: �г���
 *
 * @return const char *: ��ֵ
 *
 */
const char *dbsGetField(int iCol, int *piLen);

/**
 * @function: dbsGetInd
 *
 * @desc: ��ȡ��ָʾ��ֵ
 *
 * @param iCol: �����
 *
 * @return int: ��ָʾ��ֵ
 *
 */
int dbsGetInd(int iCol);

/**
 * @function: dbsClose
 *
 * @desc: �ر�SQL�α�
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
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
/*��ǿ��API*/
/*----------------------------------------------------------*/
/**
 * @function: dbsPrepareX
 *
 * @desc: ׼��SQL���
 *
 * @param phStmt: SQL���
 * @param sSql: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsPrepareX(const char *sSql, T_TOP_DBS_STMT *phStmt);

/**
 * @function: dbsBindCountX
 *
 * @desc: ��ñ�������
 *
 * @param hStmt: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsBindCountX(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsBindNameX
 *
 * @desc: ��ñ�������
 *
 * @param hStmt: SQL���
 * @param iCol: �������
 * @param sName: ��������
 *
 * @return char *: ��������
 *
 */
char *dbsBindNameX(T_TOP_DBS_STMT hStmt, int iCol, char *sName);

/**
 * @function: dbsBindNullX
 *
 * @desc: ��NULL������
 *
 * @param hStmt: SQL���
 * @param iCol: �������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsBindNullX(T_TOP_DBS_STMT hStmt, int iCol);

/**
 * @function: dbsBindStringX
 *
 * @desc: ���ַ���������
 *
 * @param hStmt: SQL���
 * @param iCol: �������
 * @param sStr: ����ֵ
 * @param iLen: ��������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsBindStringX(T_TOP_DBS_STMT hStmt, int iCol, const char *sStr, int iLen);

/**
 * @function: dbsBindArrayX
 *
 * @desc: ������������
 *
 * @param hStmt: SQL���
 * @param asArray[]: ����
 * @param iCnt: ���鳤��
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsBindArrayX(T_TOP_DBS_STMT hStmt, char *asArray[], int iCnt);

/**
 * @function: dbsBindSepValuesX
 *
 * @desc: ���ض��ַ��ָ����ַ���������
 *
 * @param hStmt: SQL���
 * @param sSep: �ָ���
 * @param sBuf: �ַ���
 * @param iLen: �ַ�������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsBindSepValuesX(T_TOP_DBS_STMT hStmt, char sSep, const char *sBuf, int iLen);

/**
 * @function: dbsOpenX
 *
 * @desc: ��SQL�α�
 *
 * @param hStmt: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsOpenX(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsColumnCountX
 *
 * @desc: �������
 *
 * @param hStmt: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnCountX(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsColumnNameX
 *
 * @desc: �������
 *
 * @param hStmt: SQL���
 * @param iCol: �����
 * @param sName: ����
 *
 * @return char *: ����
 *
 */
char *dbsColumnNameX(T_TOP_DBS_STMT hStmt, int iCol, char *sName);

/**
 * @function: dbsColumnLengthX
 *
 * @desc: ����г���
 *
 * @param hStmt: SQL���
 * @param iCol: �����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnLengthX(T_TOP_DBS_STMT hStmt, int iCol);

/**
 * @function: dbsColumnStringX
 *
 * @desc: ����string�������������
 *
 * @param hStmt: SQL���
 * @param iCol: �����
 * @param sBuf: ������
 * @param iSize: ����������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnStringX(T_TOP_DBS_STMT hStmt, int iCol, char *sBuf, int iSize);

/**
 * @function: dbsColumnCharX
 *
 * @desc: ����char�������������
 *
 * @param hStmt: SQL���
 * @param iCol: �����
 * @param sBuf: ������
 * @param iSize: ����������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnCharX(T_TOP_DBS_STMT hStmt, int iCol, char *sBuf, int iSize);

/**
 * @function: dbsColumnStringsX
 *
 * @desc: ��������string����������һ����
 *
 * @param hStmt: SQL���
 * @param sBuf: ������
 * @param iSize: ����������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnStringsX(T_TOP_DBS_STMT hStmt, char *sBuf, int iSize);

/**
 * @function: dbsColumnCharsX
 *
 * @param hStmt: SQL���
 * @desc: ��������char����������һ����
 *
 * @param sBuf: ������
 * @param iSize: ����������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnCharsX(T_TOP_DBS_STMT hStmt, char *sBuf, int iSize);

/**
 * @function: dbsColumnBufferX
 *
 * @desc: ��������string���������ڲ�����
 *
 * @param hStmt: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsColumnBufferX(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsFetchX
 *
 * @desc: ��ȡ����
 *
 * @param hStmt: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsFetchX(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsOperSetNumberLenX
 *
 * @desc: ����Ĭ����ֵ���ͳ���
 *
 * @param hStmt: SQL���
 * @param nLen: ��ֵ���ͳ���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsOperSetNumberLenX(T_TOP_DBS_STMT hStmt, int nLen);

/**
 * @function: dbsGetFieldX
 *
 * @desc: ��ȡ����Ϣ
 *
 * @param hStmt: SQL���
 * @param iCol: �����
 * @param piLen: �г���
 *
 * @return const char *: ��ֵ
 *
 */
const char *dbsGetFieldX(T_TOP_DBS_STMT hStmt, int iCol, int *piLen);

/**
 * @function: dbsGetIndX
 *
 * @desc: ��ȡ��ָʾ��ֵ
 *
 * @param hStmt: SQL���
 * @param iCol: �����
 *
 * @return int: ��ָʾ��ֵ
 *
 */
int dbsGetIndX(T_TOP_DBS_STMT hStmt, int iCol);

/**
 * @function: dbsGetFieldXS
 *
 * @desc: ��ȡ����Ϣ
 *
 * @param hStmt: SQL���
 * @param iCol: �����
 * @param hStmt: ������
 * @param iCol: ����������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsGetFieldXS(T_TOP_DBS_STMT hStmt, int iCol, char *sBuf, int iSize);

/**
 * @function: dbsCloseX
 *
 * @desc: �ر�SQL�α�
 *
 * @param hStmt: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsCloseX(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsGetFetchArrayX
 *
 * @desc: ȡһ����¼����Ϊָ������
 *
 * @param hStmt: SQL���
 * @param psColVars: ָ������
 * @param iColCnt: ָ�����鳤��
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsGetFetchArrayX(T_TOP_DBS_STMT hStmt, char *psColVars[], int iColCnt);

/**
 * @function: dbsExecStmtX
 *
 * @desc: ִ��SQL���
 *
 * @param psSql: SQL���
 * @param psBindVars: �������
 * @param iBindCnt: �����������
 * @param psColVars: �������
 * @param iColCnt: �����������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsExecStmtX(char *psSql, char *psBindVars[], int iBindCnt,
                 char *psColVars[], int iColCnt);

/**
 * @function: dbsPreparePageX
 *
 * @desc: ׼����ҳSQL���
 *
 * @param sSql: SQL���
 * @param phStmt: SQL���
 * @param ptPage: ��ҳ���ƽṹ
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsPreparePageX(const char *sSql, T_TOP_DBS_STMT *phStmt,
                    T_DBS_PAGE *ptPage);

/**
 * @function: dbsFetchPageX
 *
 * @desc: ȡ�÷�ҳSQL�����
 *
 * @param phStmt: SQL���
 * @param ptPage: ��ҳ���ƽṹ
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsFetchPageX(T_TOP_DBS_STMT hStmt, T_DBS_PAGE *ptPage);

/**
 * @function: dbsCountSql
 *
 * @desc: ���ɼ���SQL���
 *
 * @param sSql: ԭʼSQL���
 * @param sCountSql: ����SQL���
 * @param iSize: ����SQL��仺������С
 *
 * @return char *: ����SQL���
 *
 */
char *dbsCountSql(const char *sSql, char *sCountSql, int iSize);

/**
 * @function: dbsOpenListV
 *
 * @desc: ��SQL�α�
 *
 * @param phStmt: SQL���
 * @param psSql: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsOpenListV(T_TOP_DBS_STMT *phStmt, char *psSql, ...);

/**
 * @function: dbsOpenV
 *
 * @desc: ��SQL�α�
 *
 * @param phStmt: SQL���
 * @param psSql: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsOpenV(T_TOP_DBS_STMT *phStmt, char *sSql, ...);

/**
 * @function: dbsFetchV
 *
 * @desc: ��ȡ����
 *
 * @param hStmt: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsFetchV(T_TOP_DBS_STMT hStmt, ...);

/**
 * @function: dbsCloseV
 *
 * @desc: �ر�SQL�α�
 *
 * @param hStmt: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsCloseV(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsExecuteV
 *
 * @desc: ִ��SQL���
 *
 * @param sSql: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsExecuteV(char *sSql, ...);

/**
 * @function: dbsSqlPrePorcListA
 *
 * @desc: Ԥ�ȴ���SQL���
 *
 * @param sSql: SQL���
 * @param iBindCnt: ��������
 * @param psBind: ����
 * @param piRealBindCnt: ʵ�ʱ�������
 * @param psRealBind: ʵ�ʱ���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsSqlPrePorcListA(char *sSql, int iBindCnt, char *psBind[], int *piRealBindCnt, char *psRealBind[]);

/**
 * @function: dbsOpenListA
 *
 * @desc: ��SQL�α�
 *
 * @param phStmt: SQL���
 * @param sSql: SQL���
 * @param iBindCnt: ��������
 * @param psBind: ����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsOpenListA(T_TOP_DBS_STMT *phStmt, char *sSql, int iBindCnt, char *psBind[]);

/**
 * @function: dbsOpenA
 *
 * @desc: ��SQL�α�
 *
 * @param phStmt: SQL���
 * @param psSql: SQL���
 * @param iBindCnt: ��������
 * @param psBind: ����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsOpenA(T_TOP_DBS_STMT *phStmt, char *sSql, int iBindCnt, char *psBind[]);

/**
 * @function: dbsFetchA
 *
 * @desc: ��ȡ����
 *
 * @param hStmt: SQL���
 * @param iSelectCnt: ����
 * @param psSelect: �л�����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsFetchA(T_TOP_DBS_STMT hStmt, int iSelectCnt, char *psSelect[]);

/**
 * @function: dbsCloseA
 *
 * @desc: �ر�SQL�α�
 *
 * @param hStmt: SQL���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsCloseA(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsExecuteA
 *
 * @desc: ִ��SQL���
 *
 * @param sSql: SQL���
 * @param iBindCnt: ��������
 * @param psBind: ����
 * @param iSelectCnt: ����
 * @param psSelect: �л�����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsExecuteA(char *sSql, int iBindCnt, char *psBind[],
                int iSelectCnt, char *psSelect[]);

/**
 * @function: dbsCountA
 *
 * @desc: ��ý��������
 *
 * @param sSql: SQL���
 * @param iBindCnt: ��������
 * @param psBind: ����
*
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsCountA(const char *sSql, int iBindCnt, char *psBind[]);

/**
 * @function: dbsAnalyzeTable
 *
 * @desc: ������ṹ
 *
 * @param sTable: ����
 *
 * @return T_DBS_TABLE *: ret!=NULL ��ṹ; ret=NULL ʧ��
 *
 */
T_DBS_TABLE *dbsAnalyzeTable(char *sTable);

/**
 * @function: dbsSvrFind
 *
 * @desc: Dbsvr֧�ֺ���
 *
 * @param sSql: SQL���
 * @param iCol: �ֶθ���
 * @param ptCol: �����ֶ�����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsSvrFind(char *sSql, int iCol, T_DBS_SVR_COL *ptCol);

/**
 * @function: dbsSvrLock
 *
 * @desc: Dbsvr֧�ֺ���
 *
 * @param phStmt: �α���
 * @param sSql: SQL���
 * @param iCol: �ֶθ���
 * @param ptCol: �����ֶ�����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsSvrLock(T_TOP_DBS_STMT *phStmt,
               char *sSql, int iCol, T_DBS_SVR_COL *ptCol);

/**
 * @function: dbsSvrClose
 *
 * @desc: Dbsvr֧�ֺ���
 *
 * @param hStmt: �α���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsSvrClose(T_TOP_DBS_STMT hStmt);

/**
 * @function: dbsSvrUpdate
 *
 * @desc: Dbsvr֧�ֺ���
 *
 * @param sSql: SQL���
 * @param iCol: �ֶθ���
 * @param ptCol: �����ֶ�����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsSvrUpdate(char *sSql, int iCol, T_DBS_SVR_COL *ptCol);

/**
 * @function: dbsSvrDelete
 *
 * @desc: Dbsvr֧�ֺ���
 *
 * @param sSql: SQL���
 * @param iCol: �ֶθ���
 * @param ptCol: �����ֶ�����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsSvrDelete(char *sSql, int iCol, T_DBS_SVR_COL *ptCol);

/**
 * @function: dbsSvrInsert
 *
 * @desc: Dbsvr֧�ֺ���
 *
 * @param sSql: SQL���
 * @param iCol: �ֶθ���
 * @param ptCol: �����ֶ�����
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsSvrInsert(char *sSql, int iCol, T_DBS_SVR_COL *ptCol);

/**
 * @function: dbsCloseAll
 *
 * @desc: �ر������Ѵ򿪵�SQL�α�
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsCloseAll(void);

/**
 * @function: dbsSetOption
 *
 * @desc: �������ݿ�ѡ��
 *
 * @param sOption: ѡ������
 * @param sValue:  ѡ������
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsSetOption(char *sOption, char *sValue);

/**
 * @function: dbsSetCallBack
 *
 * @desc: ���ûص�
 *
 * @param psName: �ص���������
 * @param pfnCallBack: �ص�������ַ
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
 *
 */
int dbsSetCallBack(char *psName, PFN_TOP_DBS_CALLBACK pfnCallBack);

/** Add by GC **/
/**
 * @function: dbsSetCheckBefore
 *
 * @desc: ���ý���ǰ����־
 *
 * @param iChkFlag: ����־  0 �����; 1 ���
 *
 * @return int: ret=0 �ɹ�; ret!=0 ʧ��
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
