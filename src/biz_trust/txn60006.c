#include "common_txn.h"

int txn60006Initial(char *pData,T_HEAD t_Head);
int txn60006Process();
int txn60006End();

/*交易信息定义*/
static char f_sPlatformId[LEN_PLATID    + 1];
static char f_sAmount    [LEN_AMOUNT    + 1];
static char f_sType      [LEN_TYPE      + 1];

/*头部分信息定义*/
static char f_sTxnDate    [LEN_DATE     + 1];
static char f_sTxnTime    [LEN_TIME     + 1];
static char f_sTxnId      [LEN_TXNID    + 1];
static char f_sRtCode     [LEN_RTCODE   + 1];
static char f_sChannelCode[LEN_CHLCODE  + 1];
static char f_sTermCode   [LEN_TERMCODE + 1];
static char f_sDrct       [LEN_DRCT     + 1];

static char f_sError[8 + 1];

static T_TXN60006R t_Rep;


int txn60006(char *pData,T_HEAD t_Head,char *pOutData,char *pErrNo)
{
	short   nRet;

	logMsg(LOG_TRC,"60006咨询费结算业务，处理开始");

    nRet = txn60006Initial(pData,t_Head);
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn60006End();
        return -1;
    }

    nRet = txn60006Process();
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn60006End();
        return -1;
    }
	memcpy(pOutData,t_Rep.sAmount,18);
	return 0;
}

int txn60006Initial(char *pData,T_HEAD t_Head)
{
	T_TXN60006 *t_txn = (T_TXN60006 *) pData;

	setnull(f_sPlatformId );
	setnull(f_sAmount     );
	setnull(f_sType       );
	setnull(f_sTxnDate    );
	setnull(f_sTxnTime    );
	setnull(f_sTxnId      );
	setnull(f_sRtCode     );
	setnull(f_sChannelCode);
	setnull(f_sTermCode   );
	setnull(f_sDrct       );
	setnull(f_sError      );

	memcpy(f_sPlatformId ,t_txn->sPlatformId ,sizeof(f_sPlatformId ));
	memcpy(f_sAmount     ,t_txn->sAmount     ,sizeof(f_sAmount     ));
	memcpy(f_sType       ,t_txn->sType       ,sizeof(f_sType       ));
	memcpy(f_sTxnDate    ,t_Head.sTxnDate    ,sizeof(f_sTxnDate    ));
	memcpy(f_sTxnTime    ,t_Head.sTxnTime    ,sizeof(f_sTxnTime    ));
	memcpy(f_sTxnId      ,t_Head.sTxnId      ,sizeof(f_sTxnId      ));
	memcpy(f_sRtCode     ,t_Head.sRtCode     ,sizeof(f_sRtCode     ));
	memcpy(f_sChannelCode,t_Head.sChannelCode,sizeof(f_sChannelCode));
	memcpy(f_sTermCode   ,t_Head.sTermCode   ,sizeof(f_sTermCode   ));
	memcpy(f_sDrct       ,t_Head.sDrct       ,sizeof(f_sDrct       ));

	logMsg(LOG_BUG,"TYPE[%s]",f_sType);
	logMsg(LOG_BUG,"CHANNELCODE[%s]",f_sChannelCode);

	return 0;
}

int txn60006Process()
{
	int iRet,nRet;
	char sId         [16 + 1];
	char sCount      [32 + 1];
	char sSqlCmd     [1024];

	T_ACCFLOW t_FLOW;

	setnull(sId);
	setnull(t_FLOW);
	setnull(sCount);
	setnull(t_Rep);
	setnull(sSqlCmd);

	if(memcmp(f_sType,"0",1) == 0)
	{
		setnull(f_sAmount);
	}
	else if (memcmp(f_sType,"1",1) == 0 && atoi(f_sAmount) == 0)
	{
		logMsg(LOG_ERR,"部分提取手续费的金额不能为0");
		strcat(f_sError,"E0000012");
		return -1;
	}

	/*记录平台账户*/
	nRet = UpdatePlatAcc(f_sPlatformId,"3",f_sAmount,"");
	if(nRet == 2)
	{
		logMsg(LOG_ERR,"账户余额校验失败[%d]",nRet);
		strcat(f_sError,"E0000010");
		return -1;
	}
	else if(nRet == 1)
	{
		logMsg(LOG_ERR,"账户余额不足[%d]",nRet);
		strcat(f_sError,"E0000002");
		return -1;
	}
	else if(nRet < 0)
	{
		logMsg(LOG_ERR,"记录平台账户失败[%d]",nRet);
		return -1;
	}

	/*取记账流水*/
	nRet = dbsExecuteV("SELECT fn_gettransdate()||lpad(to_char(SEQ_TG_LS.NEXTVAL), 8, '0') FROM DUAL",sId);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"取记账流水失败[%d]",nRet);
		return -1;
	}

	/*取记账信息*/
	memcpy(t_FLOW.sId        ,sId           ,sizeof(sId)            -1 );
	memcpy(t_FLOW.sPlatformId,f_sPlatformId ,sizeof(f_sPlatformId)  -1 );
	memcpy(t_FLOW.sTxnno     ,"60006"       ,5                         );
	memcpy(t_FLOW.sTxnCode   ,"0005"        ,4                         );
	memcpy(t_FLOW.sChlCode   ,f_sChannelCode,sizeof(f_sChannelCode) -1 );
	memcpy(t_FLOW.sTermCode  ,f_sTermCode   ,sizeof(f_sTermCode)    -1 );
	memcpy(t_FLOW.sAccountId ,f_sPlatformId ,sizeof(f_sPlatformId)  -1 );
	memcpy(t_FLOW.sAccCode   ,f_sPlatformId ,sizeof(f_sPlatformId)  -1 );
	memcpy(t_FLOW.sTransDate ,f_sTxnDate    ,sizeof(f_sTxnDate)     -1 );
	memcpy(t_FLOW.sTransTime ,f_sTxnTime    ,sizeof(f_sTxnTime)     -1 );
	memcpy(t_FLOW.sAmount    ,f_sAmount     ,sizeof(f_sAmount)      -1 );
	t_FLOW.sFee[0]='0';
	memcpy(t_FLOW.sTxnId     ,f_sTxnId      ,sizeof(f_sTxnId)       -1 );
	/*记录个人账务流水表*/
	nRet = InsertPerAccFlow(t_FLOW);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"记录个人账务流水失败[%d]",nRet);
		return -1;
	}

	setnull(sSqlCmd);
	sprintf(sSqlCmd,"select feeamount from TG_PLATACCINFO where platformid = '%s'",f_sPlatformId);
	nRet = dbsExecuteV(sSqlCmd,t_Rep.sAmount);
        if(nRet < 0)
        {
                logMsg(LOG_ERR,"查询数据库失败[%d][%s][%s]",dbsSqlCode(),dbsErrStr(),sSqlCmd);
                return -1;
        }

	return 0;
}

int txn60006End()
{
	logMsg(LOG_TRC,"60006咨询费结算业务，处理结束");
	return 0;
}
