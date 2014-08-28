#include "common_txn.h"

int txn60007Initial(char *pData,T_HEAD t_Head);
int txn60007Process();
int txn60007End();

/*交易信息定义*/
static char f_sPlatformId[LEN_PLATID    + 1];
static char f_sInstCode  [LEN_PLATID    + 1];
static char f_sChgType   [LEN_CHG_TYPE  + 1];
static char f_sInstName  [LEN_PLAT_NAME + 1];
static char f_sRemarks   [LEN_REMARKS   + 1];

/*头部分信息定义*/
static char f_sTxnDate    [LEN_DATE     + 1];
static char f_sTxnTime    [LEN_TIME     + 1];
static char f_sTxnId      [LEN_TXNID    + 1];
static char f_sRtCode     [LEN_RTCODE   + 1];
static char f_sChannelCode[LEN_CHLCODE  + 1];
static char f_sTermCode   [LEN_TERMCODE + 1];
static char f_sDrct       [LEN_DRCT     + 1];

static char f_sError[8 + 1];


int txn60007(char *pData,T_HEAD t_Head,char *pOutData,char *pErrNo)
{
	short   nRet;

	logMsg(LOG_TRC,"60007机构信息维护业务，处理开始");

    nRet = txn60007Initial(pData,t_Head);
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn60007End();
        return -1;
    }

    nRet = txn60007Process();
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn60007End();
        return -1;
    }
	return 0;
}

int txn60007Initial(char *pData,T_HEAD t_Head)
{
	T_TXN60007 *t_txn = (T_TXN60007 *) pData;

	setnull(f_sPlatformId );
	setnull(f_sInstCode   );
	setnull(f_sChgType    );
	setnull(f_sInstName   );
	setnull(f_sRemarks    );
	setnull(f_sTxnDate    );
	setnull(f_sTxnTime    );
	setnull(f_sTxnId      );
	setnull(f_sRtCode     );
	setnull(f_sChannelCode);
	setnull(f_sTermCode   );
	setnull(f_sDrct       );
	setnull(f_sError      );

	memcpy(f_sPlatformId ,t_txn->sPlatformId ,sizeof(f_sPlatformId ));
	memcpy(f_sInstCode   ,t_txn->sInstCode   ,sizeof(f_sInstCode   ));
	memcpy(f_sChgType    ,t_txn->sChgType    ,sizeof(f_sChgType    ));
	memcpy(f_sInstName   ,t_txn->sInstName   ,sizeof(f_sInstName   ));
	memcpy(f_sRemarks    ,t_txn->sRemarks    ,sizeof(f_sRemarks    ));
	memcpy(f_sTxnDate    ,t_Head.sTxnDate    ,sizeof(f_sTxnDate    ));
	memcpy(f_sTxnTime    ,t_Head.sTxnTime    ,sizeof(f_sTxnTime    ));
	memcpy(f_sTxnId      ,t_Head.sTxnId      ,sizeof(f_sTxnId      ));
	memcpy(f_sRtCode     ,t_Head.sRtCode     ,sizeof(f_sRtCode     ));
	memcpy(f_sChannelCode,t_Head.sChannelCode,sizeof(f_sChannelCode));
	memcpy(f_sTermCode   ,t_Head.sTermCode   ,sizeof(f_sTermCode   ));
	memcpy(f_sDrct       ,t_Head.sDrct       ,sizeof(f_sDrct       ));

	logMsg(LOG_BUG,"INSTNAME[%s]",f_sInstName);
	logMsg(LOG_BUG,"CHANNELCODE[%s]",f_sChannelCode);

	return 0;
}

int txn60007Process()
{
	int iRet,nRet;
	char sCount      [32 + 1];
	char sSqlCmd     [1024  ];

	setnull(sCount);
	setnull(sSqlCmd);

	nRet = dbsExecuteV("select count(1) from TG_ORGANINFO where platformid = :sPid and instcode = :sIid",
						f_sPlatformId,f_sInstCode,sCount);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"查询数据库失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}
	if(atoi(sCount) == 0 && (memcmp(f_sChgType,"01",2) == 0 || memcmp(f_sChgType,"02",2) == 0))
	{
		logMsg(LOG_ERR,"机构不存在");
		strcat(f_sError,"E0000006");
		return -1;
	}
	if(atoi(sCount) == 1 && memcmp(f_sChgType,"00",2) == 0)
	{
		logMsg(LOG_ERR,"机构已存在，不可新建");
		strcat(f_sError,"E0000014");
		return -1;
	}

	setnull(sSqlCmd);
	switch(atoi(f_sChgType))
	{
		case 0:
			sprintf(sSqlCmd,"insert into TG_ORGANINFO (instcode,platformid,instname,remark,status,createdate)"
							"VALUES('%s','%s','%s','%s','00',to_char(sysdate,'YYYYMMDD'))",
							f_sInstCode,f_sPlatformId,f_sInstName,f_sRemarks);
			break;
		case 1:
			sprintf(sSqlCmd,"update TG_ORGANINFO set instname = '%s',remark = '%s' where "
							"platformid = '%s' and instcode = '%s'",
							f_sInstName,f_sRemarks,f_sPlatformId,f_sInstCode);
			break;
		case 2:
			sprintf(sSqlCmd,"update TG_ORGANINFO set status = '02' where "
							"platformid = '%s' and instcode = '%s'",
							f_sPlatformId,f_sInstCode);
		default:
			logMsg(LOG_ERR,"错误的修改类型[%s]",f_sChgType);
			return -1;
	}

	nRet = dbsExecuteV(sSqlCmd);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"数据库操作失败[%s][%s][%d]",sSqlCmd,dbsErrStr(),dbsSqlCode());
		return -1;
	}

	if(atoi(f_sChgType) == 0)
	{
		setnull(sSqlCmd);
		sprintf(sSqlCmd,"insert into TG_INSTACCINFO "
						"VALUES ('%s','%s',0,0,0,0,0,0,0,0,0,rtrim(fn_getmd5(0)))",
						f_sInstCode,f_sPlatformId);
		nRet = dbsExecuteV(sSqlCmd);
		if(nRet < 0)
		{
			logMsg(LOG_ERR,"插入数据库失败[%s][%s][%d]",sSqlCmd,dbsErrStr(),dbsSqlCode());
			return -1;
		}
	}


	return 0;
}

int txn60007End()
{
	logMsg(LOG_TRC,"60007机构信息维护业务，处理结束");
	return 0;
}
