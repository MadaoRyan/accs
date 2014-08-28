#include "common_txn.h"

int txn90003Initial(char *pData,T_HEAD t_Head);
int txn90003Process();
int txn90003End();

/*交易信息定义*/
static char f_sPlatformId[LEN_PLATID    + 1];
static char f_sStartDate [LEN_DATE      + 1];
static char f_sEndDate   [LEN_DATE      + 1];
static char f_sPageSize  [LEN_NUM       + 1];
static char f_sPageNum   [LEN_NUM       + 1];

/*头部分信息定义*/
static char f_sTxnDate    [LEN_DATE     + 1];
static char f_sTxnTime    [LEN_TIME     + 1];
static char f_sTxnId      [LEN_TXNID    + 1];
static char f_sRtCode     [LEN_RTCODE   + 1];
static char f_sChannelCode[LEN_CHLCODE  + 1];
static char f_sTermCode   [LEN_TERMCODE + 1];
static char f_sDrct       [LEN_DRCT     + 1];
static T_TXN90003R t_Rep;

static char f_sError[8 + 1];

static T_TOP_DBS_STMT tStmt = 0;

int txn90003(char *pData,T_HEAD t_Head,char *pOutData,char *pErrNo)
{
	short   nRet;
	int     i,nLen=0;

	logMsg(LOG_TRC,"90003收益明细查询业务，处理开始");

    nRet = txn90003Initial(pData,t_Head);
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn90003End();
        return -1;
    }

    nRet = txn90003Process();
    if (nRet != 0)
    {
    	memcpy(pErrNo,f_sError,8);
    	txn90003End();
        return -1;
    }
	memcpy(pOutData,    t_Rep.sPlatformId,sizeof(t_Rep.sPlatformId) -1);
	nLen =        sizeof(t_Rep.sPlatformId);
	memcpy(pOutData+nLen,t_Rep.sTotal    ,sizeof(t_Rep.sTotal)      -1);
	nLen = nLen + sizeof(t_Rep.sTotal);
	memcpy(pOutData+nLen,t_Rep.sNumber   ,sizeof(t_Rep.sNumber)     -1);
	nLen = nLen + sizeof(t_Rep.sNumber);
	for (i = 0;i< atoi(t_Rep.sNumber);i++)
	{
		memcpy(pOutData+nLen,t_Rep.L0[i].sTransDate,8);
		nLen = nLen + 9;
		memcpy(pOutData+nLen,t_Rep.L0[i].sAmount,18);
		nLen = nLen +19;
		memcpy(pOutData+nLen,t_Rep.L0[i].sAccId,32);
		nLen = nLen +33;
		memcpy(pOutData+nLen,t_Rep.L0[i].sFee,18);
		nLen = nLen +19;
		memcpy(pOutData+nLen,t_Rep.L0[i].sTxnNo,16);
		nLen = nLen +17;
	}
	return 0;
}

int txn90003Initial(char *pData,T_HEAD t_Head)
{
	T_TXN90003 *t_txn = (T_TXN90003 *) pData;

	setnull(f_sPlatformId );
	setnull(f_sStartDate  );
	setnull(f_sEndDate    );
	setnull(f_sPageSize   );
	setnull(f_sPageNum    );
	setnull(f_sTxnDate    );
	setnull(f_sTxnTime    );
	setnull(f_sTxnId      );
	setnull(f_sRtCode     );
	setnull(f_sChannelCode);
	setnull(f_sTermCode   );
	setnull(f_sDrct       );
	setnull(f_sError      );

	memcpy(f_sPlatformId ,t_txn->sPlatformId ,sizeof(f_sPlatformId ));
	memcpy(f_sStartDate  ,t_txn->sStartDate  ,sizeof(f_sStartDate  ));
	memcpy(f_sEndDate    ,t_txn->sEndDate    ,sizeof(f_sEndDate    ));
	memcpy(f_sPageSize   ,t_txn->sPageSize   ,sizeof(f_sPageSize   ));
	memcpy(f_sPageNum    ,t_txn->sPageNum    ,sizeof(f_sPageNum    ));
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

int txn90003Process()
{
	int iRet,nRet,i=0;
	char sSqlCmd[1024*2];
	char sCount    [32 + 1];
	char sNum      [32 + 1];
	char sAccountId[19 + 1];
	char sTransDate[8  + 1];
	char sAmount   [18 + 1];
	char sFee      [18 + 1];
	char sTxnId    [16 + 1];
	char sRefId    [32 + 1];

	setnull(sSqlCmd);
	setnull(sCount);
	setnull(sAccountId);
	setnull(sTransDate);
	setnull(sAmount);
	setnull(sFee);
	setnull(sTxnId);
	setnull(sRefId);

	sprintf(sSqlCmd,"select count(1) "
				"from (select 1 "
					"from TG_ACCOUNTFLOWP "
					"where platformid = '%s' "
					"and transdate <= '%s' "
					"and transdate >= '%s' "
					"and ((transcode in ('0001','0002') "
					"and nvl(fee,0) > 0 ) OR transcode = '0003' ) "
					"and status = '00' "
					"union all "
					"select 1 "
					"from TG_ACCOUNTFLOWO "
					"where platformid = '%s' "
					"and transdate <= '%s' "
					"and transdate >= '%s' "
					"and ((transcode in ('0001','0002') "
					"and nvl(fee,0) > 0 ) OR transcode = '0003')"
					"and status = '00')",
					f_sPlatformId,f_sEndDate,f_sStartDate,
					f_sPlatformId,f_sEndDate,f_sStartDate);
	logMsg(LOG_TRC,"sSqlCmd[%s]",sSqlCmd);
	nRet = dbsExecuteV(sSqlCmd,sCount);
	if(nRet < 0 )
	{
		logMsg(LOG_ERR,"查询数据库失败");
	}

	memcpy(t_Rep.sPlatformId,f_sPlatformId,sizeof(f_sPlatformId));
	sprintf(t_Rep.sTotal,"%d",atoi(sCount));
	if(atoi(sCount) == 0)
	{
		t_Rep.sNumber[0] = '0';
		return 0;
	}
	else if(atoi(f_sPageNum) == 0)
	{
		t_Rep.sNumber[0] = '0';
		return 0;
	}
	else if(atoi(sCount) <= atoi(f_sPageSize))
	{
		sprintf(t_Rep.sNumber,"%d",atoi(sCount));
	}
	else if(atoi(sCount) - atoi(f_sPageSize) * (atoi(f_sPageNum) -1) < atoi(f_sPageSize) )
	{
		sprintf(t_Rep.sNumber,"%d",atoi(sCount) - atoi(f_sPageSize) * (atoi(f_sPageNum) -1));
	}
	else
	{
		memcpy(t_Rep.sNumber,f_sPageSize,sizeof(f_sPageSize)-1);
	}

	if(atoi(sCount) > 0)
	{
		setnull(sSqlCmd);
		sprintf(sSqlCmd,"select * from (select transdate, amount, accountid, fee, txnid,rownum num "
					"from (select transdate, amount, accountid, fee, txnid "
						"from TG_ACCOUNTFLOWP "
						"where platformid = '%s' "
						"and transdate <= '%s' "
						"and transdate >= '%s' "
						"and ((transcode in ('0001','0002') "
						"and nvl(fee,0) > 0) OR transcode = '0003') "
						"and status = '00' "
						"union all "
						"select transdate, amount, accountid, fee, txnid "
						"from TG_ACCOUNTFLOWO "
						"where platformid = '%s' "
						"and transdate <= '%s' "
						"and transdate >= '%s' "
						"and ((transcode in ('0001','0002') "
						"and nvl(fee,0) > 0) OR transcode = '0003' ) "
						"and status = '00')) "
						"where num > ('%s' - 1) * '%s' "
						"and num <= '%s' * '%s' "
						"order by txnid",
						f_sPlatformId,f_sEndDate,f_sStartDate,
						f_sPlatformId,f_sEndDate,f_sStartDate,
						f_sPageNum,f_sPageSize,f_sPageNum,f_sPageSize);
		logMsg(LOG_TRC,"sSqlCmd[%s]",sSqlCmd);
		nRet = dbsOpenV(&tStmt, sSqlCmd);
    		if(iRet < 0) 
    		{
        		logMsg(LOG_ERR, "打开游标失败[%d][%s]", dbsSqlCode(),dbsErrStr());
        		dbsCloseV(tStmt);
        		return -1;
    		}

		i=0;

    		while(1)
    		{
			setnull(sNum);
    			setnull(sTransDate);
    			setnull(sAmount);
    			setnull(sAccountId);
    			setnull(sFee);
    			setnull(sTxnId);
    			setnull(sRefId);

    			nRet = dbsFetchV(tStmt, sTransDate, sAmount, sAccountId,sFee,sTxnId,sNum);
    			if(dbsSqlCode() == 1403)
    			{
    				logMsg(LOG_BUG,"轮询数据结束");
    				break;
    			}
    			if(nRet < 0 && dbsSqlCode() != -1405)
    			{
    				logMsg(LOG_ERR,"FETCH游标失败[%s][%d]",dbsErrStr(),dbsSqlCode());
    				dbsCloseV(tStmt);
    				return -1;
    			}
			logMsg(LOG_TRC,"循环域-第[%d]层处理开始...",i+1);

    			nRet = dbsExecuteV("select rtrim(refid) from TG_ACCMAP where platformid = :Pid and accountcode = :Aid",
    							f_sPlatformId,sAccountId,sRefId);
    			if(nRet < 0 )
    			{
    				logMsg(LOG_ERR,"查询数据库失败[%s][%d]",dbsErrStr(),dbsSqlCode());
    				return -1;
    			}

    			/*为结构体赋值*/
    			logMsg(LOG_BUG,"循环域-第[%d]层赋值",i+1);
    			memcpy(t_Rep.L0[i].sTransDate,sTransDate,8 );
    			memcpy(t_Rep.L0[i].sAmount   ,sAmount   ,18);
    			memcpy(t_Rep.L0[i].sAccId    ,sRefId    ,32);
    			memcpy(t_Rep.L0[i].sFee      ,sFee      ,18);
    			memcpy(t_Rep.L0[i].sTxnNo    ,sTxnId    ,16);

    			i++;
    		}
    		dbsCloseV(tStmt);
	}
	return 0;
}

int txn90003End()
{
	logMsg(LOG_TRC,"90003收益明细查询业务，处理结束");
	return 0;
}
