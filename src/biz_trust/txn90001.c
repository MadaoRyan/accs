#include "common_txn.h"

int txn90001Initial(char *pData,T_HEAD t_Head);
int txn90001Process();
int txn90001End();

/*交易信息定义*/
static char f_sAccId     [LEN_ACCID     + 1];
static char f_sPlatformId[LEN_PLATID    + 1];

/*头部分信息定义*/
static char f_sTxnDate    [LEN_DATE     + 1];
static char f_sTxnTime    [LEN_TIME     + 1];
static char f_sTxnId      [LEN_TXNID    + 1];
static char f_sRtCode     [LEN_RTCODE   + 1];
static char f_sChannelCode[LEN_CHLCODE  + 1];
static char f_sTermCode   [LEN_TERMCODE + 1];
static char f_sDrct       [LEN_DRCT     + 1];
static T_TXN90001R t_Rep;

static char f_sError[8 + 1];


int txn90001(char *pData,T_HEAD t_Head,char *pOutData,char *pErrNo)
{
	short   nRet;

	logMsg(LOG_TRC,"90001账户查询业务，处理开始");

    nRet = txn90001Initial(pData,t_Head);
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn90001End();
        return -1;
    }

    nRet = txn90001Process();
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn90001End();
        return -1;
    }

    memcpy(pOutData                                       ,t_Rep.sPlatformId,sizeof(f_sPlatformId)-1);
	memcpy(pOutData+sizeof(f_sPlatformId)                 ,t_Rep.sAccId     ,sizeof(f_sAccId)     -1);
	memcpy(pOutData+sizeof(f_sPlatformId)+sizeof(f_sAccId),t_Rep.sBalance   ,18                     );
	return 0;
}

int txn90001Initial(char *pData,T_HEAD t_Head)
{
	T_TXN90001 *t_txn = (T_TXN90001 *) pData;

	setnull(f_sAccId      );
	setnull(f_sPlatformId );
	setnull(f_sTxnDate    );
	setnull(f_sTxnTime    );
	setnull(f_sTxnId      );
	setnull(f_sRtCode     );
	setnull(f_sChannelCode);
	setnull(f_sTermCode   );
	setnull(f_sDrct       );
	setnull(f_sError      );

	memcpy(f_sAccId      ,t_txn->sAccId      ,sizeof(f_sAccId      ));
	memcpy(f_sPlatformId ,t_txn->sPlatformId ,sizeof(f_sPlatformId ));
	memcpy(f_sTxnDate    ,t_Head.sTxnDate    ,sizeof(f_sTxnDate    ));
	memcpy(f_sTxnTime    ,t_Head.sTxnTime    ,sizeof(f_sTxnTime    ));
	memcpy(f_sTxnId      ,t_Head.sTxnId      ,sizeof(f_sTxnId      ));
	memcpy(f_sRtCode     ,t_Head.sRtCode     ,sizeof(f_sRtCode     ));
	memcpy(f_sChannelCode,t_Head.sChannelCode,sizeof(f_sChannelCode));
	memcpy(f_sTermCode   ,t_Head.sTermCode   ,sizeof(f_sTermCode   ));
	memcpy(f_sDrct       ,t_Head.sDrct       ,sizeof(f_sDrct       ));

	logMsg(LOG_BUG,"ACCID[%s]",f_sAccId);
	logMsg(LOG_BUG,"CHANNELCODE[%s]",f_sChannelCode);

	return 0;
}

int txn90001Process()
{
	int iRet,nRet;
	char sSqlCmd[1024];
	char sAccountCode [19 + 1];
	char sAccCode     [19 + 1];
	char sBalance     [18 + 1];
	char sCount       [8  + 1];

	setnull(t_Rep);
	setnull(sSqlCmd);
	setnull(sAccountCode);
	setnull(sAccCode);
	setnull(sBalance);
	setnull(sCount);

	nRet = dbsExecuteV("select accountcode,acccode from TG_ACCMAP WHERE refid =:accid and platformid = :pid",
						f_sAccId,f_sPlatformId,sAccountCode,sAccCode);
	if(nRet < 0 && dbsSqlCode() != 1403)
	{
		logMsg(LOG_ERR,"查询数据库失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}
	else if (dbsSqlCode() == 1403)
	{
		logMsg(LOG_ERR,"查询账号未绑定");
		strcat(f_sError,"E0000013");
		return -1;
	}

	/*账户余额MD5校验*/
	setnull(sSqlCmd);
	sprintf(sSqlCmd,"select 1 from KP_ACCINFO where accountcode = '%s' and acccode ='%s'"
					" and rtrim(balancemd5) = rtrim(fn_getmd5(balance))",
					sAccountCode,sAccCode);
	nRet = dbsExecuteV(sSqlCmd,sCount);
	if(nRet < 0 && dbsSqlCode() != 1403)
	{
		logMsg(LOG_ERR,"数据库查询失败[%d][%s][%s]",dbsSqlCode(),dbsErrStr(),sSqlCmd);
		return -1;
	}
	if(dbsSqlCode() == 1403)
	{
		logMsg(LOG_ERR,"账户余额MD5检查失败");
		strcat(f_sError,"E0000010");
		return -1;
	}

	nRet = dbsExecuteV("select (balance - frozbalance)*100 from KP_ACCINFO where accountcode = :id1 and acccode = :id2",
						sAccountCode,sAccCode,sBalance);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"查询数据库失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	memcpy(t_Rep.sPlatformId,f_sPlatformId,sizeof(f_sPlatformId)-1);
	memcpy(t_Rep.sAccId     ,f_sAccId     ,sizeof(f_sAccId)     -1);
	memcpy(t_Rep.sBalance   ,sBalance     ,sizeof(sBalance)     -1);

	return 0;
}

int txn90001End()
{
	logMsg(LOG_TRC,"90001账户查询业务，处理结束");
	return 0;
}
