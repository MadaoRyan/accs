#include "common_txn.h"

int txn60002Initial(char *pData,T_HEAD t_Head);
int txn60002Process();
int txn60002End();

/*交易信息定义*/
static char f_sAccId     [LEN_ACCID     + 1];
static char f_sPlatformId[LEN_PLATID    + 1];
static char f_sAmount    [LEN_AMOUNT    + 1];
static char f_sInstCode  [LEN_PLATID    + 1];

/*头部分信息定义*/
static char f_sTxnDate    [LEN_DATE     + 1];
static char f_sTxnTime    [LEN_TIME     + 1];
static char f_sTxnId      [LEN_TXNID    + 1];
static char f_sRtCode     [LEN_RTCODE   + 1];
static char f_sChannelCode[LEN_CHLCODE  + 1];
static char f_sTermCode   [LEN_TERMCODE + 1];
static char f_sDrct       [LEN_DRCT     + 1];

static char f_sError[8 + 1];
static T_TXN60002R t_Rep;


int txn60002(char *pData,T_HEAD t_Head,char *pOutData,char *pErrNo)
{
	short   nRet;

	logMsg(LOG_TRC,"60002机构充值业务，处理开始");

    nRet = txn60002Initial(pData,t_Head);
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn60002End();
        return -1;
    }

    nRet = txn60002Process();
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn60002End();
        return -1;
    }
	memcpy(pOutData,t_Rep.sAmount,18);
	return 0;
}

int txn60002Initial(char *pData,T_HEAD t_Head)
{
	T_TXN60002 *t_txn = (T_TXN60002 *) pData;

	setnull(f_sAccId      );
	setnull(f_sPlatformId );
	setnull(f_sAmount     );
	setnull(f_sInstCode   );
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
	memcpy(f_sInstCode   ,t_txn->sInstCode   ,sizeof(f_sInstCode   ));
	memcpy(f_sTxnDate    ,t_Head.sTxnDate    ,sizeof(f_sTxnDate    ));
	memcpy(f_sTxnTime    ,t_Head.sTxnTime    ,sizeof(f_sTxnTime    ));
	memcpy(f_sTxnId      ,t_Head.sTxnId      ,sizeof(f_sTxnId      ));
	memcpy(f_sRtCode     ,t_Head.sRtCode     ,sizeof(f_sRtCode     ));
	memcpy(f_sChannelCode,t_Head.sChannelCode,sizeof(f_sChannelCode));
	memcpy(f_sTermCode   ,t_Head.sTermCode   ,sizeof(f_sTermCode   ));
	memcpy(f_sDrct       ,t_Head.sDrct       ,sizeof(f_sDrct       ));

	logMsg(LOG_BUG,"INSTCODE[%s]",f_sInstCode);
	logMsg(LOG_BUG,"CHANNELCODE[%s]",f_sChannelCode);

	return 0;
}

int txn60002Process()
{
	int nRet;
	char sId         [16 + 1];
	char sCount      [32 + 1];
	char sSqlCmd     [1024];

	T_ACCFLOW t_FLOW;

	setnull(sId);
	setnull(t_FLOW);
	setnull(sCount);
	setnull(t_Rep);

	/*检查机构是否存在切状态正常*/
	nRet = dbsExecuteV("select COUNT(1) from TG_ORGANINFO where platformid = :sPlatid and instcode = :sInst and status = '00'",
						f_sPlatformId,f_sInstCode,sCount);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"查询数据库失败[%s][%d]",dbsErrStr(),dbsSqlCode());
		return -1;
	}
	if(!atoi(sCount))
	{
		logMsg(LOG_ERR,"机构不存在或者机构状态异常");
		strcat(f_sError,"E0000006");
		return -1;
	}

	/*充值更新机构账户*/
	nRet=UpdateInstAcc(f_sPlatformId,f_sInstCode,"1",f_sAmount,"0");
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"机构充值失败");
		return -1;
	}
	else if(nRet == 2)
	{
		logMsg(LOG_ERR,"余额MD5校验失败");
		strcat(f_sError,"E0000010");
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
	memcpy(t_FLOW.sInstCode  ,f_sInstCode   ,sizeof(f_sInstCode)    -1 );
	memcpy(t_FLOW.sTxnno     ,"60002"       ,5                         );
	memcpy(t_FLOW.sTxnCode   ,"0001"        ,4                         );
	memcpy(t_FLOW.sChlCode   ,f_sChannelCode,sizeof(f_sChannelCode) -1 );
	memcpy(t_FLOW.sTermCode  ,f_sTermCode   ,sizeof(f_sTermCode)    -1 );
	memcpy(t_FLOW.sAccountId ,f_sInstCode   ,sizeof(f_sInstCode)    -1 );
	memcpy(t_FLOW.sAccCode   ,f_sInstCode   ,sizeof(f_sInstCode)    -1 );
	memcpy(t_FLOW.sTransDate ,f_sTxnDate    ,sizeof(f_sTxnDate)     -1 );
	memcpy(t_FLOW.sTransTime ,f_sTxnTime    ,sizeof(f_sTxnTime)     -1 );
	memcpy(t_FLOW.sAmount    ,f_sAmount     ,sizeof(f_sAmount)      -1 );
	t_FLOW.sFee[0]='0';
	memcpy(t_FLOW.sTxnId     ,f_sTxnId      ,sizeof(f_sTxnId)       -1 );
	/*记录个人账务流水表*/
	nRet = InsertOrgAccFlow(t_FLOW);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"记录机构账务流水失败[%d]",nRet);
		return -1;
	}

	setnull(sSqlCmd);
	sprintf(sSqlCmd,"select amount  from TG_INSTACCINFO where platformid = '%s' and instcode = '%s'",f_sPlatformId,f_sInstCode);
        nRet = dbsExecuteV(sSqlCmd,t_Rep.sAmount);
        if(nRet < 0)
        {
                logMsg(LOG_ERR,"查询数据库失败[%s][%d][%s]",sSqlCmd,dbsSqlCode(),dbsErrStr());
                return -1;
        }

	return 0;
}

int txn60002End()
{
	logMsg(LOG_TRC,"60002机构充值业务，处理结束");
	return 0;
}
