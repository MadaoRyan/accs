#include "common_txn.h"

int txn90004Initial(char *pData,T_HEAD t_Head);
int txn90004Process();
int txn90004End();

/*交易信息定义*/
static char f_sPlatformId[LEN_PLATID    + 1];

/*头部分信息定义*/
static char f_sTxnDate    [LEN_DATE     + 1];
static char f_sTxnTime    [LEN_TIME     + 1];
static char f_sTxnId      [LEN_TXNID    + 1];
static char f_sRtCode     [LEN_RTCODE   + 1];
static char f_sChannelCode[LEN_CHLCODE  + 1];
static char f_sTermCode   [LEN_TERMCODE + 1];
static char f_sDrct       [LEN_DRCT     + 1];
static T_TXN90004R t_Rep;

static char f_sError[8 + 1];


int txn90004(char *pData,T_HEAD t_Head,char *pOutData,char *pErrNo)
{
	short   nRet;

	logMsg(LOG_TRC,"90004账户查询业务，处理开始");

    nRet = txn90004Initial(pData,t_Head);
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn90004End();
        return -1;
    }

    nRet = txn90004Process();
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn90004End();
        return -1;
    }

    memcpy(pOutData                      ,t_Rep.sPlatformId,sizeof(f_sPlatformId) -1);
    memcpy(pOutData+sizeof(f_sPlatformId),t_Rep.sAmount    ,18                      );
	return 0;
}

int txn90004Initial(char *pData,T_HEAD t_Head)
{
	T_TXN90004 *t_txn = (T_TXN90004 *) pData;

	setnull(f_sPlatformId );
	setnull(f_sTxnDate    );
	setnull(f_sTxnTime    );
	setnull(f_sTxnId      );
	setnull(f_sRtCode     );
	setnull(f_sChannelCode);
	setnull(f_sTermCode   );
	setnull(f_sDrct       );
	setnull(f_sError      );

	memcpy(f_sPlatformId ,t_txn->sPlatformId ,sizeof(f_sPlatformId ));
	memcpy(f_sTxnDate    ,t_Head.sTxnDate    ,sizeof(f_sTxnDate    ));
	memcpy(f_sTxnTime    ,t_Head.sTxnTime    ,sizeof(f_sTxnTime    ));
	memcpy(f_sTxnId      ,t_Head.sTxnId      ,sizeof(f_sTxnId      ));
	memcpy(f_sRtCode     ,t_Head.sRtCode     ,sizeof(f_sRtCode     ));
	memcpy(f_sChannelCode,t_Head.sChannelCode,sizeof(f_sChannelCode));
	memcpy(f_sTermCode   ,t_Head.sTermCode   ,sizeof(f_sTermCode   ));
	memcpy(f_sDrct       ,t_Head.sDrct       ,sizeof(f_sDrct       ));

	logMsg(LOG_BUG,"PLATID[%s]",f_sPlatformId);
	logMsg(LOG_BUG,"CHANNELCODE[%s]",f_sChannelCode);

	return 0;
}

int txn90004Process()
{
	int iRet,nRet;
	char sSqlCmd[1024];
	char sBalance     [18 + 1];
	char sCount       [8  + 1];

	setnull(t_Rep);
	setnull(sSqlCmd);
	setnull(sBalance);
	setnull(sCount);

	nRet = dbsExecuteV("select count(1) from TG_PLATFORMINFO WHERE platformid = :pid",
						f_sPlatformId,sCount);
	if(nRet < 0 && dbsSqlCode() != 1403)
	{
		logMsg(LOG_ERR,"查询数据库失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}
	else if (atoi(sCount) == 0)
	{
		logMsg(LOG_ERR,"平台不存在");
		strcat(f_sError,"E0000005");
		return -1;
	}

	/*平台账户余额md5校验*/
	sprintf(sSqlCmd,"select feeamount from TG_PLATACCINFO where platformid = '%s' "
					"and rtrim(feebalancemd5) = rtrim(fn_getmd5(feeamount)) "
					"and rtrim(balancemd5) = rtrim(fn_getmd5(amount))",f_sPlatformId);
	nRet = dbsExecuteV(sSqlCmd,sBalance);
	if(nRet < 0 && dbsSqlCode() != 1403)
	{
		logMsg(LOG_ERR,"查询数据库失败[%s][%d][%s]",sSqlCmd,dbsSqlCode(),dbsErrStr());
		return -1;
	}
	if(dbsSqlCode() == 1403)
	{
		logMsg(LOG_ERR,"余额MD5检查失败");
		strcat(f_sError,"E0000010");
		return -1;
	}

	memcpy(t_Rep.sPlatformId,f_sPlatformId,sizeof(f_sPlatformId)-1);
	memcpy(t_Rep.sAmount    ,sBalance     ,sizeof(sBalance)     -1);

	return 0;
}

int txn90004End()
{
	logMsg(LOG_TRC,"90004平台收益查询业务，处理结束");
	return 0;
}
