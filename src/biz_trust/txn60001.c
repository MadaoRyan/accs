#include "common_txn.h"

int txn60001Initial(char *pData,T_HEAD t_Head);
int txn60001Process();
int txn60001End();

/*交易信息定义*/
static char f_sAccId     [LEN_ACCID     + 1];
static char f_sPlatformId[LEN_PLATID    + 1];
static char f_sAmount    [LEN_AMOUNT    + 1];
static char f_sFee       [LEN_AMOUNT    + 1];
static char f_sIdType    [LEN_ID_TYPE   + 1];
static char f_sPersonId  [LEN_PERSON_ID + 1];
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

static T_TXN60001R t_Rep;


int txn60001(char *pData,T_HEAD t_Head,char *pOutData,char *pErrNo)
{
	short   nRet;

	logMsg(LOG_TRC,"60001充值业务，处理开始");

    nRet = txn60001Initial(pData,t_Head);
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn60001End();
        return -1;
    }

    nRet = txn60001Process();
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn60001End();
        return -1;
    }
	memcpy(pOutData,t_Rep.sAmount,18);
	return 0;
}

int txn60001Initial(char *pData,T_HEAD t_Head)
{
	T_TXN60001 *t_txn = (T_TXN60001 *) pData;

	setnull(f_sAccId      );
	setnull(f_sPlatformId );
	setnull(f_sAmount     );
	setnull(f_sFee        );
	setnull(f_sIdType     );
	setnull(f_sPersonId   );
	setnull(f_sAccName    );
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
	memcpy(f_sAmount     ,t_txn->sAmount     ,sizeof(f_sAmount     ));
	memcpy(f_sFee        ,t_txn->sFee        ,sizeof(f_sFee        ));
	memcpy(f_sIdType     ,t_txn->sIdType     ,sizeof(f_sIdType     ));
	memcpy(f_sPersonId   ,t_txn->sPersonId   ,sizeof(f_sPersonId   ));
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

int txn60001Process()
{
	int iRet,nRet;
	char sAccCode    [19 + 1];
	char sAccountCode[19 + 1];
	char sId         [16 + 1];
	char sCount      [32 + 1];
	char sSqlCmd     [1024];

	T_ACCFLOW t_FLOW;

	setnull(sAccCode);
	setnull(sAccountCode);
	setnull(sId);
	setnull(t_FLOW);
	setnull(sCount);
	setnull(t_Rep);

	/*检查外部账号是否已绑定*/
	iRet = CheckAcc(f_sPlatformId,f_sAccId,sAccountCode,sAccCode);
	if(iRet < 0)
	{
		logMsg(LOG_ERR,"检查绑定失败[%d]",iRet);
		strcat(f_sError,"E0000008");
		return -1;
	}
	else if(iRet == 1)
	{
		/*账号还未绑定*/
		logMsg(LOG_BUG,"外部账号还未进行绑定，绑定开始...");
		nRet = CreateAcc(f_sPlatformId,f_sAccId,f_sIdType,f_sPersonId,f_sAccName,sAccountCode,sAccCode);
		if(nRet == 1)
		{
			logMsg(LOG_ERR,"绑定账号失败，无可用实体卡");
			strcat(f_sError,"E0000001");
			return -1;
		}
		else if(nRet != 0)
		{
			logMsg(LOG_ERR,"绑定账号失败");
			strcat(f_sError,"E0000009");
			return -1;
		}
	}

	/*检查账户状态*/
	setnull(sSqlCmd);
	sprintf(sSqlCmd,"SELECT count(1) FROM KP_ACCINFO WHERE accountcode=%s and acccode=%s and accstat ='0'",sAccountCode,sAccCode);
	nRet = dbsExecuteV(sSqlCmd,sCount);
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
	nRet = UpdateAcc(sAccountCode,sAccCode,"1",f_sAmount,f_sFee);
	if(nRet == 2)
	{
		logMsg(LOG_ERR,"账户余额校验失败[%d]",nRet);
		strcat(f_sError,"E0000010");
		return -1;
	}
	else if(nRet < 0)
	{
		logMsg(LOG_ERR,"充值失败[%d]",nRet);
		return -1;
	}

	/*记录平台账户*/
	nRet = UpdatePlatAcc(f_sPlatformId,"1",f_sAmount,f_sFee);
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
	memcpy(t_FLOW.sTxnno     ,"60001"       ,5                         );
	memcpy(t_FLOW.sTxnCode   ,"0001"        ,4                         );
	memcpy(t_FLOW.sChlCode   ,f_sChannelCode,sizeof(f_sChannelCode) -1 );
	memcpy(t_FLOW.sTermCode  ,f_sTermCode   ,sizeof(f_sTermCode)    -1 );
	memcpy(t_FLOW.sAccountId ,sAccountCode  ,sizeof(sAccountCode)   -1 );
	memcpy(t_FLOW.sAccCode   ,sAccCode      ,sizeof(sAccCode)       -1 );
	memcpy(t_FLOW.sTransDate ,f_sTxnDate    ,sizeof(f_sTxnDate)     -1 );
	memcpy(t_FLOW.sTransTime ,f_sTxnTime    ,sizeof(f_sTxnTime)     -1 );
	memcpy(t_FLOW.sAmount    ,f_sAmount     ,sizeof(f_sAmount)      -1 );
	memcpy(t_FLOW.sFee       ,f_sFee        ,sizeof(f_sFee)         -1 );
	memcpy(t_FLOW.sTxnId     ,f_sTxnId      ,sizeof(f_sTxnId)       -1 );
	/*记录个人账务流水表*/
	nRet = InsertPerAccFlow(t_FLOW);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"记录个人账务流水失败[%d]",nRet);
		return -1;
	}

	setnull(sSqlCmd);
        sprintf(sSqlCmd,"SELECT (balance-frozbalance) * 100  FROM KP_ACCINFO WHERE accountcode=%s and acccode=%s and accstat ='0'",sAccountCode,sAccCode);
        nRet = dbsExecuteV(sSqlCmd,t_Rep.sAmount);
        if(nRet < 0)
        {
                logMsg(LOG_ERR,"查询数据库失败[%d][%s]",dbsSqlCode(),dbsErrStr());
                return -1;
        }

	return 0;
}

int txn60001End()
{
	logMsg(LOG_TRC,"60001充值业务，处理结束");
	return 0;
}
