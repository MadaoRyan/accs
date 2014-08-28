#include "common_txn.h"

int txn60004Initial(char *pData,T_HEAD t_Head);
int txn60004Process();
int txn60004End();

/*交易信息定义*/
static char f_sPayerAccId[LEN_ACCID     + 1];
static char f_sPayeeAccId[LEN_ACCID     + 1];
static char f_sPlatformId[LEN_PLATID    + 1];
static char f_sAmount    [LEN_AMOUNT    + 1];
static char f_sFee       [LEN_AMOUNT    + 1];
static char f_sAccName   [LEN_NAME      + 1];

/*头部分信息定义*/
static char f_sTxnDate    [LEN_DATE     + 1];
static char f_sTxnTime    [LEN_TIME     + 1];
static char f_sTxnId      [LEN_TXNID    + 1];
static char f_sRtCode     [LEN_RTCODE   + 1];
static char f_sChannelCode[LEN_CHLCODE  + 1];
static char f_sTermCode   [LEN_TERMCODE + 1];
static char f_sDrct       [LEN_DRCT     + 1];

static char f_sError[8 + 1];


int txn60004(char *pData,T_HEAD t_Head,char *pOutData,char *pErrNo)
{
	short   nRet;

	logMsg(LOG_TRC,"60004转账业务，处理开始");

    nRet = txn60004Initial(pData,t_Head);
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn60004End();
        return -1;
    }

    nRet = txn60004Process();
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn60004End();
        return -1;
    }
	return 0;
}

int txn60004Initial(char *pData,T_HEAD t_Head)
{
	T_TXN60004 *t_txn = (T_TXN60004 *) pData;

	setnull(f_sPayerAccId );
	setnull(f_sPayeeAccId );
	setnull(f_sPlatformId );
	setnull(f_sAmount     );
	setnull(f_sFee        );
	setnull(f_sAccName    );
	setnull(f_sTxnDate    );
	setnull(f_sTxnTime    );
	setnull(f_sTxnId      );
	setnull(f_sRtCode     );
	setnull(f_sChannelCode);
	setnull(f_sTermCode   );
	setnull(f_sDrct       );
	setnull(f_sError      );

	memcpy(f_sPayerAccId ,t_txn->sPayerAccId ,sizeof(f_sPayerAccId ));
	memcpy(f_sPayeeAccId ,t_txn->sPayeeAccId ,sizeof(f_sPayeeAccId ));
	memcpy(f_sPlatformId ,t_txn->sPlatformId ,sizeof(f_sPlatformId ));
	memcpy(f_sAmount     ,t_txn->sAmount     ,sizeof(f_sAmount     ));
	memcpy(f_sFee        ,t_txn->sFee        ,sizeof(f_sFee        ));
	memcpy(f_sAccName    ,t_txn->sAccName    ,sizeof(f_sAccName    ));
	memcpy(f_sTxnDate    ,t_Head.sTxnDate    ,sizeof(f_sTxnDate    ));
	memcpy(f_sTxnTime    ,t_Head.sTxnTime    ,sizeof(f_sTxnTime    ));
	memcpy(f_sTxnId      ,t_Head.sTxnId      ,sizeof(f_sTxnId      ));
	memcpy(f_sRtCode     ,t_Head.sRtCode     ,sizeof(f_sRtCode     ));
	memcpy(f_sChannelCode,t_Head.sChannelCode,sizeof(f_sChannelCode));
	memcpy(f_sTermCode   ,t_Head.sTermCode   ,sizeof(f_sTermCode   ));
	memcpy(f_sDrct       ,t_Head.sDrct       ,sizeof(f_sDrct       ));

	logMsg(LOG_BUG,"ACCNAME[%s]",f_sAccName);
	logMsg(LOG_BUG,"CHANNELCODE[%s]",f_sChannelCode);

	return 0;
}

int txn60004Process()
{
	int iRet,nRet;
	char sPayerAccCode    [19 + 1];
	char sPayerAccountCode[19 + 1];
	char sPayeeAccCode    [19 + 1];
	char sPayeeAccountCode[19 + 1];
	char sId         [16 + 1];
	char sCount      [32 + 1];

	T_ACCFLOW t_FLOW;

	setnull(sPayerAccCode);
	setnull(sPayerAccountCode);
	setnull(sPayeeAccCode);
	setnull(sPayeeAccountCode);
	setnull(sId);
	setnull(t_FLOW);
	setnull(sCount);

	/*出借人外部信息检查*/
	/*检查外部账号是否已绑定*/
	iRet = CheckAcc(f_sPlatformId,f_sPayerAccId,sPayerAccountCode,sPayerAccCode);
	if(iRet < 0)
	{
		logMsg(LOG_ERR,"检查出借人绑定失败[%d]",iRet);
		strcat(f_sError,"E0000008");
		return -1;
	}
	else if(iRet == 1)
	{
		/*账号还未绑定*/
		logMsg(LOG_ERR,"出借人外部账号还未进行绑定，不能转账");
		strcat(f_sError,"E0000003");
		return -1;
	}

	/*检查账户状态*/
	nRet = dbsExecuteV("SELECT count(1) FROM KP_ACCINFO WHERE accountcode=:sCode and acccode=:sId and accstat ='0'",
						sPayerAccountCode,sPayerAccCode,sCount);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"查询数据库失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	if(!atoi(sCount))
	{
		logMsg(LOG_ERR,"出借人账户状态检查失败");
		strcat(f_sError,"E0000011");
		return -1;
	}

	/*借款人信息检查*/
	setnull(sCount);
	iRet = CheckAcc(f_sPlatformId,f_sPayeeAccId,sPayeeAccountCode,sPayeeAccCode);
	if(iRet < 0)
	{
		logMsg(LOG_ERR,"检查借款人绑定失败[%d]",iRet);
		strcat(f_sError,"E0000008");
		return -1;
	}
	else if(iRet == 1)
	{
		/*账号还未绑定*/
		logMsg(LOG_BUG,"借款人外部账号还未进行绑定，绑定开始...");
		nRet = CreateAcc(f_sPlatformId,f_sPayeeAccId,"","",f_sPayeeAccId,sPayeeAccountCode,sPayeeAccCode);
		if(nRet == 1)
		{
			logMsg(LOG_ERR,"借款人绑定账号失败，无可用实体卡");
			strcat(f_sError,"E0000001");
			return -1;
		}
		else if(nRet != 0)
		{
			logMsg(LOG_ERR,"借款人绑定账号失败");
			strcat(f_sError,"E0000009");
			return -1;
		}
	}

	/*检查账户状态*/
	nRet = dbsExecuteV("SELECT count(1) FROM KP_ACCINFO WHERE accountcode=:sCode and acccode=:sId and accstat ='0'",
						sPayeeAccountCode,sPayeeAccCode,sCount);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"查询数据库失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	if(!atoi(sCount))
	{
		logMsg(LOG_ERR,"账户状态检查失败");
		strcat(f_sError,"E0000011");
		return -1;
	}

	/*修改个人账户*/
	nRet = UpdateAcc(sPayerAccountCode,sPayerAccCode,"3",f_sAmount,f_sFee);
	if(nRet == 2)
	{
		logMsg(LOG_ERR,"出借人账户余额校验失败[%d]",nRet);
		strcat(f_sError,"E0000010");
		return -1;
	}
	else if(nRet < 0)
	{
		logMsg(LOG_ERR,"转出失败[%d]",nRet);
		return -1;
	}

	nRet = UpdateAcc(sPayeeAccountCode,sPayeeAccCode,"2",f_sAmount,f_sFee);
	if(nRet == 2)
	{
		logMsg(LOG_ERR,"借款人账户余额校验失败[%d]",nRet);
		strcat(f_sError,"E0000010");
		return -1;
	}
	else if(nRet < 0)
	{
		logMsg(LOG_ERR,"转入失败[%d]",nRet);
		return -1;
	}

	/*记录平台账户*/
	nRet = UpdatePlatAcc(f_sPlatformId,"4",f_sAmount,f_sFee);
	if(nRet == 2)
	{
		logMsg(LOG_ERR,"账户余额校验失败[%d]",nRet);
		strcat(f_sError,"E0000010");
		return -1;
	}
	else if(nRet < 0)
	{
		logMsg(LOG_ERR,"记录平台账户失败[%d]",nRet);
		return -1;
	}

	/*记录转入账务流水*/
	/*取记账流水*/
	nRet = dbsExecuteV("SELECT fn_gettransdate()||lpad(to_char(SEQ_TG_LS.NEXTVAL), 8, '0') FROM DUAL",sId);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"取记账流水失败[%d]",nRet);
		return -1;
	}

	/*取记账信息*/
	memcpy(t_FLOW.sId         ,sId              ,sizeof(sId)               -1 );
	memcpy(t_FLOW.sPlatformId ,f_sPlatformId    ,sizeof(f_sPlatformId)     -1 );
	memcpy(t_FLOW.sTxnno      ,"60004"          ,5                            );
	memcpy(t_FLOW.sTxnCode    ,"0003"           ,4                            );
	memcpy(t_FLOW.sChlCode    ,f_sChannelCode   ,sizeof(f_sChannelCode)    -1 );
	memcpy(t_FLOW.sTermCode   ,f_sTermCode      ,sizeof(f_sTermCode)       -1 );
	memcpy(t_FLOW.sAccountId  ,sPayeeAccountCode,sizeof(sPayeeAccountCode) -1 );
	memcpy(t_FLOW.sLinkAccId  ,sPayerAccountCode,sizeof(sPayerAccountCode) -1 );
	memcpy(t_FLOW.sAccCode    ,sPayeeAccCode    ,sizeof(sPayeeAccCode)     -1 );
	memcpy(t_FLOW.sLinkAccCode,sPayerAccCode    ,sizeof(sPayerAccCode)     -1 );
	memcpy(t_FLOW.sTransDate  ,f_sTxnDate       ,sizeof(f_sTxnDate)        -1 );
	memcpy(t_FLOW.sTransTime  ,f_sTxnTime       ,sizeof(f_sTxnTime)        -1 );
	memcpy(t_FLOW.sAmount     ,f_sAmount        ,sizeof(f_sAmount)         -1 );
	memcpy(t_FLOW.sFee        ,f_sFee           ,sizeof(f_sFee)            -1 );
	memcpy(t_FLOW.sTxnId      ,f_sTxnId         ,sizeof(f_sTxnId)          -1 );
	/*记录个人账务流水表*/
	nRet = InsertPerAccFlow(t_FLOW);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"记录个人账务流水失败[%d]",nRet);
		return -1;
	}

	/*记录转出账务流水*/
	/*取记账流水*/
	setnull(sId);
	setnull(t_FLOW);
	nRet = dbsExecuteV("SELECT fn_gettransdate()||lpad(to_char(SEQ_TG_LS.NEXTVAL), 8, '0') FROM DUAL",sId);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"取记账流水失败[%d]",nRet);
		return -1;
	}

	/*取记账信息*/
	memcpy(t_FLOW.sId         ,sId              ,sizeof(sId)               -1 );
	memcpy(t_FLOW.sPlatformId ,f_sPlatformId    ,sizeof(f_sPlatformId)     -1 );
	memcpy(t_FLOW.sTxnno      ,"60004"          ,5                            );
	memcpy(t_FLOW.sTxnCode    ,"0004"           ,4                            );
	memcpy(t_FLOW.sChlCode    ,f_sChannelCode   ,sizeof(f_sChannelCode)    -1 );
	memcpy(t_FLOW.sTermCode   ,f_sTermCode      ,sizeof(f_sTermCode)       -1 );
	memcpy(t_FLOW.sAccountId  ,sPayerAccountCode,sizeof(sPayerAccountCode) -1 );
	memcpy(t_FLOW.sLinkAccId  ,sPayeeAccountCode,sizeof(sPayeeAccountCode) -1 );
	memcpy(t_FLOW.sAccCode    ,sPayerAccCode    ,sizeof(sPayerAccCode)     -1 );
	memcpy(t_FLOW.sLinkAccCode,sPayeeAccCode    ,sizeof(sPayeeAccCode)     -1 );
	memcpy(t_FLOW.sTransDate  ,f_sTxnDate       ,sizeof(f_sTxnDate)        -1 );
	memcpy(t_FLOW.sTransTime  ,f_sTxnTime       ,sizeof(f_sTxnTime)        -1 );
	memcpy(t_FLOW.sAmount     ,f_sAmount        ,sizeof(f_sAmount)         -1 );
	memcpy(t_FLOW.sFee        ,f_sFee           ,sizeof(f_sFee)            -1 );
	memcpy(t_FLOW.sTxnId      ,f_sTxnId         ,sizeof(f_sTxnId)          -1 );
	/*记录个人账务流水表*/
	nRet = InsertPerAccFlow(t_FLOW);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"记录个人账务流水失败[%d]",nRet);
		return -1;
	}

	return 0;
}

int txn60004End()
{
	logMsg(LOG_TRC,"60004充值业务，处理结束");
	return 0;
}
