#include "common_txn.h"

/*
创建账号映射及子账户
入参：
pPlatformId 平台号
pAccId      外部平台注册号
pAccountCode 返回的内部账号
pAccCode     返回的内部子账号

return      0 成功
			1 无可用实体卡，绑定失败
          <>0 失败
*/
int CreateAcc(char *pPlatformId,char *pAccId,char *pIdType,char *pPersonId,char *pAccName,char *pAccountCode,char *pAccCode)
{
	int nRet;
	char sPlatformId [LEN_PLATID    + 1];
	char sAccId      [LEN_ACCID     + 1];
	char sIdType     [LEN_ID_TYPE   + 1];
	char sPersonId   [LEN_PERSON_ID + 1];
	char sAccName    [LEN_NAME      + 1];
	char sCardId     [19            + 1];
	char sAccountCode[19            + 1];
	char sAccCode    [19            + 1];
	char sCount      [32            + 1];
	char sCustId     [19            + 1];
	char sCardNo     [18            + 1];
	char sInstCode   [9             + 1];
	char sSqlCmd     [1024];

	setnull(sPlatformId);
	setnull(sAccId);
	setnull(sIdType);
	setnull(sPersonId);
	setnull(sAccName);
	setnull(sCardId);
	setnull(sAccountCode);
	setnull(sAccCode);
	setnull(sCount);
	setnull(sCustId);
	setnull(sCardNo);
	setnull(sInstCode);
	setnull(sSqlCmd);

	memcpy(sPlatformId,pPlatformId,sizeof(sPlatformId) -1);
	memcpy(sAccId     ,pAccId     ,sizeof(sAccId     ) -1);
	memcpy(sIdType    ,pIdType    ,sizeof(sIdType    ) -1);
	memcpy(sPersonId  ,pPersonId  ,sizeof(sPersonId  ) -1);
	memcpy(sAccName   ,sAccName   ,sizeof(sAccName   ) -1);

	sprintf(sSqlCmd,"SELECT K.CARDID,K.TRACKCARDNO,K.issinstcode FROM KP_CARDINFO K"
		            " WHERE K.CARDID NOT IN (SELECT T.CARDID FROM TG_ACCMAP T)"
		            " AND ROWNUM = '1' AND K.CARDSTAT = '0' FOR UPDATE");

	/*检查是否有可用实体卡*/
	nRet = dbsExecuteV(sSqlCmd,sCardId,sCardNo,sInstCode);
	if(nRet !=0 && dbsSqlCode()!=1403)
	{
		logMsg(LOG_ERR,"数据库查询失败[%d][%s][%s]",dbsSqlCode(),dbsErrStr(),sSqlCmd);
		return -1;
	}
	if(1403 == dbsSqlCode())
	{
		logMsg(LOG_ERR,"无可用子账户");
		return 1;
	}

	/*取账户代码*/
	nRet = dbsExecuteV("SELECT lpad(to_char(SEQ_ACCOUNTCODENO.NEXTVAL),19,'0') FROM DUAL",sAccountCode);
	if(nRet !=0)
	{
		logMsg(LOG_ERR,"取账户代码失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}
	/*取子账户代码*/
	nRet=CreateTgAcc(sCardId,sAccCode);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"取子账户代码失败");
		return -1;
	}
	/*取客户ID*/
	nRet = dbsExecuteV("SELECT lpad(to_char(SEQ_CUSTIDNO.NEXTVAL),19,'0') FROM DUAL",sCustId);
	if(nRet != 0)
	{
		logMsg(LOG_ERR,"取客户ID失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	/*INSERT客户信息表CRM_CUSTOMERINFO*/
	nRet = dbsExecuteV("INSERT INTO CRM_CUSTOMERINFO(CUSTID,CUSTNAME,PERSONID)VALUES(:sCustId,:sCustName,:sPersonId)",
						sCustId,sAccName,sPersonId);
	if(nRet != 0)
	{
		logMsg(LOG_ERR,"INSERT客户信息表CRM_CUSTOMERINFO失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	/*INSERT账户信息表KP_ACCOUNOTINFO*/
	setnull(sSqlCmd);
	sprintf(sSqlCmd,"INSERT INTO KP_ACCOUNTINFO(accountcode,cardid,salenum,saleamt,voidnum,voidamt,rechargenum,"
					"rechargeamt,refundnum,refundamt,pointused,pointtotal,checkused,checktotal)"
					"VALUES('%s','%s',0,0,0,0,0,0,0,0,0,0,0,0)",sAccountCode,sCustId);
	nRet = dbsExecuteV(sSqlCmd);
	if(nRet != 0)
	{
		logMsg(LOG_ERR,"INSERT账户信息表KP_ACCOUNOTINFO失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	/*INSERT子账户信息表KP_ACCINFO*/
	setnull(sSqlCmd);
	sprintf(sSqlCmd,"INSERT INTO KP_ACCINFO(accountcode,acccode,createdate,instcode,balance,endbalance,"
					"frozbalance,balancemd5,accstat)"
					"VALUES('%s','%s',fn_gettransdate(),'%s',0,0,0,rtrim(fn_getmd5(0)),'0')",sAccountCode,sAccCode,sInstCode);
	nRet = dbsExecuteV(sSqlCmd);
	if(nRet != 0)
	{
		logMsg(LOG_ERR,"INSERT子账户信息表KP_ACCINFO失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}
	/*INSERT托管业务账号映射表TG_ACCMAP*/
	setnull(sSqlCmd);
	sprintf(sSqlCmd,"INSERT INTO TG_ACCMAP(refid,platformid,accountcode,acccode,cardid,trackcardno,custid,status,createdate)"
					"VALUES('%s','%s','%s','%s','%s','%s','%s','0',fn_gettransdate())",
					sAccId,sPlatformId,sAccountCode,sAccCode,sCardId,sCardNo,sCustId);
	nRet = dbsExecuteV(sSqlCmd);
	if(nRet != 0)
	{
		logMsg(LOG_ERR,"INSERT托管业务账号映射表TG_ACCMAP失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	dbsCommit();
	logMsg(LOG_ERR,"sAccountCode[%s]sAccCode[%s]",sAccountCode,sAccCode);
	memcpy(pAccountCode,sAccountCode,sizeof(sAccountCode)-1);
	memcpy(pAccCode,sAccCode,sizeof(sAccCode)-1);
	
	return 0;
}

/*
检查外部账号是否已绑定
入参:
pPlatformId  平台号
pAccId       外部平台注册号
pAccountCode 返回的内部账号
pAccCode     返回的内部子账号

return      0 已注册
            1 未注册
          < 0 处理失败
*/
int CheckAcc(char *pPlatformId,char *pAccId,char *pAccountCode,char *pAccCode)
{
	int nRet;
	char sPlatformId [LEN_PLATID + 1];
	char sAccId      [LEN_ACCID  + 1];
	char sAccountCode[19         + 1];
	char sAccCode    [19         + 1];
	char sCount      [32         + 1];

	setnull(sPlatformId);
	setnull(sAccId);
	setnull(sCount);
	setnull(sAccountCode);
	setnull(sAccCode);

	memcpy(sPlatformId,pPlatformId,LEN_PLATID);
	memcpy(sAccId     ,pAccId     ,LEN_ACCID );

	nRet = dbsExecuteV("SELECT count(1) FROM TG_ACCMAP WHERE REFID = :sAccId AND PLATFORMID = :sPlatformId",
						sAccId,sPlatformId,sCount);
	if(nRet != 0)
	{
		logMsg(LOG_ERR,"查询数据库失败,SQLCODE[%d],ERRSTR[%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	if(atoi(sCount) == 1)
	{
		nRet = dbsExecuteV("SELECT accountcode,acccode FROM TG_ACCMAP WHERE REFID = :sAccId AND PLATFORMID = :sPlatformId",
			              sAccId,sPlatformId,sAccountCode,sAccCode);
		if(nRet != 0)
		{
			logMsg(LOG_ERR,"查询数据库失败,SQLCODE[%d],ERRSTR[%s]",dbsSqlCode(),dbsErrStr());
			return -1;
		}
		memcpy(pAccountCode,sAccountCode,sizeof(sAccountCode)-1);
		memcpy(pAccCode,sAccCode,sizeof(sAccCode)-1);

		return 0;
	}
	else if(atoi(sCount) ==0)
		/*没有相关绑定信息*/
		return 1;
	else
		return -1;

	return 0;
}

/*
更新个人账户表
入参：
pAccountCode  账户代码
pAccCode      子账户代码
pType         更新类型：1-充值，2-转入，3-转出，4-提现
pAmount       金额
pFee          手续费

return        0 成功
			  1 余额不足
			  2 余额MD5检查失败
            <>0 失败
*/
int UpdateAcc(char *pAccountCode,char *pAccCode,char *pType,char *pAmount,char *pFee)
{
	int nRet;
	char sAccountCode[19 + 1];
	char sAccCode    [19 + 1];
	char sType       [1  + 1];
	char sAmount     [18 + 1];
	char sFee        [18 + 1];
	char sCount      [32 + 1];
	char sSqlCmd     [1024  ];

	char sBalance    [18 + 1];

	setnull(sAccountCode);
	setnull(sAccCode);
	setnull(sType);
	setnull(sAmount);
	setnull(sFee);
	setnull(sCount);
	setnull(sSqlCmd);
	setnull(sBalance);

	memcpy(sAccountCode,pAccountCode,sizeof(sAccountCode) - 1);
	memcpy(sAccCode    ,pAccCode    ,sizeof(sAccCode)     - 1);
	memcpy(sType       ,pType       ,sizeof(sType)        - 1);
	memcpy(sAmount     ,pAmount     ,sizeof(sAmount)      - 1);
	memcpy(sFee        ,pFee        ,sizeof(sFee)         - 1);

	if(atoi(sAmount) == 0)
	{
		logMsg(LOG_ERR,"金额不可为0");
		return -1;
	}

	/*账户余额MD5校验*/
	setnull(sSqlCmd);
	sprintf(sSqlCmd,"select 1 from KP_ACCINFO where accountcode = '%s' and acccode ='%s'"
					" and rtrim(balancemd5) = rtrim(fn_getmd5(balance)) for update",
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
		return 2;
	}

	/*支出类交易账户余额检查*/
	if(memcmp(sType,"3",1) == 0 || memcmp(sType,"4",1) == 0)
	{
		setnull(sSqlCmd);
		sprintf(sSqlCmd,"select balance - frozbalance from KP_ACCINFO where accountcode = '%s' and acccode ='%s'"
						" and balance - ltrim (to_char('%s'/100,'9999999999999990.00')) - frozbalance >= 0",
						sAccountCode,sAccCode,sAmount);
		nRet = dbsExecuteV(sSqlCmd,sBalance);
		if(nRet != 0 && dbsSqlCode() != 1403)
		{
			logMsg(LOG_ERR,"数据库查询失败[%d][%s][%s]",dbsSqlCode(),dbsErrStr(),sSqlCmd);
			return -1;
		}
		if(dbsSqlCode() == 1403)
		{
			logMsg(LOG_ERR,"账户余额不足:可用余额[%s]，扣除金额[%s]",sBalance,sAmount);
			return 1;
		}
	}

	setnull(sSqlCmd);
	switch(atoi(sType))
	{
		case 1:
		case 2:
			sprintf(sSqlCmd,"update KP_ACCINFO set balance = balance + ltrim (to_char('%s'/100,'9999999999999990.00')), "
							"balancemd5 = rtrim(fn_getmd5(balance + ltrim (to_char('%s'/100,'9999999999999990.00')))),"
							"frozbalance=frozbalance + ltrim (to_char('%s'/100,'9999999999999990.00'))"
							"where accountcode = '%s' and acccode ='%s'",sAmount,sAmount,sFee,sAccountCode,sAccCode);
			break;
		case 3:
		case 4:
			sprintf(sSqlCmd,"update KP_ACCINFO set balance = balance - ltrim (to_char('%s'/100,'9999999999999990.00')), "
							"balancemd5 = rtrim(fn_getmd5(balance - ltrim (to_char('%s'/100,'9999999999999990.00')))),"
							"frozbalance=frozbalance + ltrim (to_char('%s'/100,'9999999999999990.00'))"
							"where accountcode = '%s' and acccode ='%s'",sAmount,sAmount,sFee,sAccountCode,sAccCode);
			break;
		default:
		logMsg(LOG_ERR,"更新类型错误[%s]",sType);
		return -1;
	}
	logMsg(LOG_BUG,"sSqlCmd[%s]",sSqlCmd);

	nRet = dbsExecuteV(sSqlCmd);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"更新子账户表信息失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	return 0;
}

/*
更新平台账户表
入参：
pPlatformId  平台号
pType        更新类型：1-充值，2-提现，3-提取手续费,4-记收益
pAmount      金额
pFee         手续费

return        0 成功
			  1 余额不足
			  2 余额MD5检查失败
            <>0 失败
*/
int UpdatePlatAcc(char *pPlatformId,char *pType,char *pAmount,char *pFee)
{
	int nRet;
	char sPlatformId[LEN_PLATID + 1];
	char sType      [1          + 1];
	char sAmount    [LEN_AMOUNT + 1];
	char sFee       [LEN_AMOUNT + 1];
	char sCount     [32         + 1];
	char sSqlCmd    [1024          ];
	char sBalance   [LEN_AMOUNT + 1];

	setnull(sPlatformId);
	setnull(sType);
	setnull(sAmount);
	setnull(sFee);
	setnull(sCount);
	setnull(sSqlCmd);
	setnull(sBalance);

	memcpy(sPlatformId,pPlatformId,LEN_PLATID);
	memcpy(sType      ,pType      ,1         );
	memcpy(sAmount    ,pAmount    ,LEN_AMOUNT);
	memcpy(sFee       ,pFee       ,LEN_AMOUNT);

	if(atoi(sAmount) == 0 && atoi(sType) != 3)
	{
		logMsg(LOG_ERR,"金额不可为0");
		return -1;
	}

	/*平台账户余额md5校验*/
	sprintf(sSqlCmd,"select 1 from TG_PLATACCINFO where platformid = '%s' "
					"and rtrim(feebalancemd5) = rtrim(fn_getmd5(feeamount)) "
					"and rtrim(balancemd5) = rtrim(fn_getmd5(amount)) for update",sPlatformId);
	nRet = dbsExecuteV(sSqlCmd,sCount);
	if(nRet < 0 && dbsSqlCode() != 1403)
	{
		logMsg(LOG_ERR,"查询数据库失败[%s][%d][%s]",sSqlCmd,dbsSqlCode(),dbsErrStr());
		return -1;
	}
	if(dbsSqlCode() == 1403)
	{
		logMsg(LOG_ERR,"余额MD5检查失败");
		return 2;
	}

	if(memcmp(sType,"3",1) == 0)
	{
		/*检查余额是否充足*/
		setnull(sCount);
		setnull(sSqlCmd);
		sprintf(sSqlCmd,"select feeamount from TG_PLATACCINFO where platformid ='%s' "
						"and feeamount - nvl('%s',0) > 0",sPlatformId,sAmount);
		nRet = dbsExecuteV(sSqlCmd,sBalance);
		if(nRet < 0 && dbsSqlCode() != 1403)
		{
			logMsg(LOG_ERR,"查询数据库失败[%d][%s]",dbsSqlCode(),dbsErrStr());
			return -1;
		}
		else if (dbsSqlCode() == 1403)
		{
			logMsg(LOG_ERR,"平台收余额不足");
			return 1;
		}

		/*提取咨询费金额为0则提取全部咨询费*/
		if(atoi(sAmount) == 0)
		{
			memcpy(sAmount,sBalance,sizeof(sAmount));
			memcpy(pAmount,sBalance,sizeof(sAmount));
		}
	}

	setnull(sSqlCmd);
	switch(atoi(sType))
	{
		case 1:
			sprintf(sSqlCmd,"UPDATE TG_PLATACCINFO SET rechargetotnum=rechargetotnum+1,"
							"rechargetotamt=rechargetotamt + %s,"
							"incometotnum = case when nvl('%s', 0) = 0 then incometotnum else incometotnum + 1 end,"
							"incometotamt = incometotamt + nvl('%s', 0),"
							"feeamount    = feeamount + nvl('%s', 0),"
							"feebalancemd5= rtrim(fn_getmd5(feeamount + nvl('%s', 0))),"
							"amount       = amount + %s,"
							"balancemd5   = rtrim(fn_getmd5(amount + %s)) "
							" where platformid = '%s'",sAmount,sFee,sFee,sFee,sFee,sAmount,sAmount,sPlatformId);
			break;
		case 2:
			sprintf(sSqlCmd,"UPDATE TG_PLATACCINFO SET cashtotnum = cashtotnum+1,"
							"cashtotamt   = cashtotamt + %s,"
							"incometotnum = case when nvl('%s', 0) = 0 then incometotnum else incometotnum + 1 end,"
							"incometotamt = incometotamt + nvl('%s', 0),"
							"feeamount    = feeamount + nvl('%s', 0),"
							"feebalancemd5= rtrim(fn_getmd5(feeamount + nvl('%s', 0))),"
							"amount       = amount - %s,"
							"balancemd5   = rtrim(fn_getmd5(amount - %s)) "
							" where platformid = '%s'",sAmount,sFee,sFee,sFee,sFee,sAmount,sAmount,sPlatformId);
			break;
		case 3:
			sprintf(sSqlCmd,"UPDATE TG_PLATACCINFO SET extractedtotnum = extractedtotnum + 1,"
							"extractedtotamt= extractedtotamt + %s,"
							"feeamount      = feeamount - %s,"
							"feebalancemd5  = rtrim(fn_getmd5(feeamount - %s)) "
							"where platformid = '%s'",sAmount,sAmount,sAmount,sPlatformId);
			break;
		case 4:
			sprintf(sSqlCmd,"UPDATE TG_PLATACCINFO SET feeamount = feeamount + %s,"
							"feebalancemd5 = rtrim(fn_getmd5(feeamount + %s)) "
							"where platformid = '%s'",sFee,sFee,sPlatformId);
			break;
		default:
			logMsg(LOG_ERR,"ERROR TYPE CODE [%s]",sType);
			return -1;
	}
	logMsg(LOG_BUG,"sSqlCmd[%s]",sSqlCmd);

	nRet = dbsExecuteV(sSqlCmd);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"更新平台账户表信息失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	return 0;
}

/*
更新机构账户表
入参：
pPlatformId 平台号
pInstCode   机构号
pType       更新类型：1-充值，2-转入，3-转出
pAmount     金额
pFee        手续费

return      0 成功
			1 余额不足
			2 金额校验失败
          <>0 失败
*/
int UpdateInstAcc(char *pPlatformId,char *pInstCode,char *pType,char *pAmount,char *pFee)
{
	int nRet;
	char sPlatformId[LEN_PLATID + 1];
	char sInstCode  [LEN_PLATID + 1];
	char sType      [1          + 1];
	char sAmount    [LEN_AMOUNT + 1];
	char sFee       [LEN_AMOUNT + 1];
	char sBalance   [LEN_AMOUNT + 1];
	char sCount     [32];
	char sSqlCmd    [1024];

	setnull(sPlatformId);
	setnull(sInstCode);
	setnull(sType);
	setnull(sAmount);
	setnull(sFee);
	setnull(sBalance);
	setnull(sCount);
	setnull(sSqlCmd);

	memcpy(sPlatformId,pPlatformId,LEN_PLATID);
	memcpy(sInstCode  ,pInstCode  ,LEN_PLATID);
	memcpy(sType      ,pType      ,1         );
	memcpy(sAmount    ,pAmount    ,LEN_AMOUNT);
	memcpy(sFee       ,pFee       ,LEN_AMOUNT);

	/*机构表金额MD5校验*/
	sprintf(sSqlCmd,"select 1 from TG_INSTACCINFO where platformid = '%s' and instcode = '%s'"
					"and rtrim(balancemd5) = rtrim(fn_getmd5(amount)) for update",sPlatformId,sInstCode);
	nRet = dbsExecuteV(sSqlCmd,sCount);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"查询数据库失败[%s][%d][%s]",sSqlCmd,dbsSqlCode(),dbsErrStr());
		return -1;
	}
	if(!atoi(sCount))
	{
		logMsg(LOG_ERR,"余额MD5检查失败");
		return 2;
	}
	/*支出类交易账户余额检查*/
	if(memcmp(sType,"3",1) == 0)
	{
		setnull(sSqlCmd);
		sprintf(sSqlCmd,"select amount from  TG_INSTACCINFO where instcode = '%s' and platformid ='%s'"
						" and amount - ltrim (to_char('%s'/100,'9999999999999990.00')) >= 0",
						sInstCode,sPlatformId,sAmount);
		nRet = dbsExecuteV(sSqlCmd,sBalance);
		if(nRet != 0 && dbsSqlCode() != 1403)
		{
			logMsg(LOG_ERR,"数据库查询失败[%d][%s][%s]",dbsSqlCode(),dbsErrStr(),sSqlCmd);
			return -1;
		}
		if(dbsSqlCode() == 1403)
		{
			logMsg(LOG_ERR,"账户余额不足:可用余额[%s]，扣除金额[%s]",sBalance,sAmount);
			return 1;
		}
	}
	setnull(sSqlCmd);
	switch(atoi(sType))
	{
		case 1:
			sprintf(sSqlCmd,"UPDATE TG_INSTACCINFO SET rechargetotnum=rechargetotnum+1,"
							"rechargetotamt=rechargetotamt + %s,"
							"amount       = amount + %s,"
							"balancemd5   = rtrim(fn_getmd5(amount + %s)) "
							"where platformid = '%s' and instcode = '%s'",
							sAmount,sAmount,sAmount,sPlatformId,sInstCode);
			break;
		case 2:
			sprintf(sSqlCmd,"UPDATE TG_INSTACCINFO SET transintotnum=transintotnum+1,"
							"transintotamt= transintotamt + %s,"
							"feetotnum    = case when nvl('%s', 0) = 0 then feetotnum else feetotnum + 1 end,"
							"feetotamt    = feetotamt + nvl('%s', 0),"
							"amount       = amount + %s,"
							"balancemd5   = rtrim(fn_getmd5(amount + %s)) "
							"where platformid = '%s' and instcode = '%s'",
							sAmount,sFee,sFee,sAmount,sAmount,sPlatformId,sInstCode);
			break;
		case 3:
			sprintf(sSqlCmd,"UPDATE TG_INSTACCINFO SET transouttotnum=transouttotnum+1,"
							"transouttotamt= transouttotamt + %s,"
							"feetotnum     = case when nvl('%s', 0) = 0 then feetotnum else feetotnum + 1 end,"
							"feetotamt     = feetotamt + nvl('%s', 0),"
							"amount        = amount - %s,"
							"balancemd5    = rtrim(fn_getmd5(amount - %s)) "
							"where platformid = '%s' and instcode = '%s'",
							sAmount,sFee,sFee,sAmount,sAmount,sPlatformId,sInstCode);
			break;
		default:
			logMsg(LOG_ERR,"ERROR TYPE CODE [%s]",sType);
			return -1;
	}
	logMsg(LOG_BUG,"sSqlCmd[%s]",sSqlCmd);

	nRet = dbsExecuteV(sSqlCmd);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"更新机构账户表信息失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	return 0;
}

/*
记录个人账务流水表
入参：
pId         账务流水号
t_FLOW      记账信息

return      0 成功
          <>0 失败
*/
int InsertPerAccFlow(T_ACCFLOW t_FLOW)
{
	int nRet;
	char sSqlCmd[1024];
	setnull(sSqlCmd);

	sprintf(sSqlCmd,"INSERT INTO TG_ACCOUNTFLOWP VALUES"
					"('%s','%s','%s','%s','%s',"
					" '%s','%s','','%s','','%s','%s',"
					"to_char(sysdate,'YYYYMMDD'),to_char(sysdate,'HH24MISS'),"
					" '%s','%s','%s','%s','00',"
					" '00','%s')",
					t_FLOW.sId,t_FLOW.sPlatformId,t_FLOW.sTxnCode,t_FLOW.sTxnno,t_FLOW.sChlCode,
					t_FLOW.sTermCode,t_FLOW.sAccountId,t_FLOW.sLinkAccId,t_FLOW.sAccCode,t_FLOW.sLinkAccCode,
					t_FLOW.sTransDate,t_FLOW.sTransTime,t_FLOW.sAmount,t_FLOW.sFee,
					t_FLOW.sTxnId);
	logMsg(LOG_ERR,"sSqlCmd[%s]",sSqlCmd);
	nRet = dbsExecuteV(sSqlCmd);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"插个人账务流水表失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	return 0;
}

/*
记录机构账务流水表
入参：
pId         账务流水号
t_FLOW      记账信息

return      0 成功
          <>0 失败
*/
int InsertOrgAccFlow(T_ACCFLOW t_FLOW)
{
	int nRet;
	char sSqlCmd[1024];
	setnull(sSqlCmd);

	sprintf(sSqlCmd,"INSERT INTO TG_ACCOUNTFLOWO VALUES"
					"('%s','%s','%s','%s','%s','%s',"
					" '%s','%s','','%s','','%s','%s',"
					"to_char(sysdate,'YYYYMMDD'),to_char(sysdate,'HH24MISS'),"
					" '%s','%s','%s','%s','00',"
					" '00','%s')",
					t_FLOW.sId,t_FLOW.sPlatformId,t_FLOW.sInstCode,t_FLOW.sTxnCode,t_FLOW.sTxnno,t_FLOW.sChlCode,
					t_FLOW.sTermCode,t_FLOW.sAccountId,t_FLOW.sLinkAccId,t_FLOW.sAccCode,t_FLOW.sLinkAccCode,
					t_FLOW.sTransDate,t_FLOW.sTransTime,t_FLOW.sAmount,t_FLOW.sFee,
					t_FLOW.sTxnId);
	logMsg(LOG_ERR,"sSqlCmd[%s]",sSqlCmd);
	nRet = dbsExecuteV(sSqlCmd);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"插个人账务流水表失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}
	
	return 0;
}

/*
取托管子账户代码函数
入参
*/
int CreateTgAcc(char *pCardId,char *pAccId)
{
	int nRet;
	char sCardId[19 + 1];
	char sSqlCmd[1024  ];
	char sAccId[19 + 1];
	setnull(sAccId);
	setnull(sCardId);
	setnull(sAccId);
	setnull(sSqlCmd);

	memcpy(sCardId,pCardId,19);
	sprintf(sSqlCmd,"select fn_genval(substr(cardnote, 1, 5) || substr(issinstcode, 8, 2) ||"
                    "'010' || lpad(to_char(SEQ_CUSTNO.nextval), 8, '0')) from kp_cardinfo "
  					"where cardid = '%s'",sCardId);
	nRet = dbsExecuteV(sSqlCmd,sAccId);
	if(nRet != 0)
	{
		logMsg(LOG_ERR,"取子账户代码失败[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}
	logMsg(LOG_BUG,"sAccId[%s]",sAccId);
	memcpy(pAccId,sAccId,19);
	/**pAccId=sAccId;*/
	logMsg(LOG_BUG,"sAccId[%s]",pAccId);

	return 0;
}
