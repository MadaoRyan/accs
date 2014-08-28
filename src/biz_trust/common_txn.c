#include "common_txn.h"

/*
�����˺�ӳ�估���˻�
��Σ�
pPlatformId ƽ̨��
pAccId      �ⲿƽ̨ע���
pAccountCode ���ص��ڲ��˺�
pAccCode     ���ص��ڲ����˺�

return      0 �ɹ�
			1 �޿���ʵ�忨����ʧ��
          <>0 ʧ��
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

	/*����Ƿ��п���ʵ�忨*/
	nRet = dbsExecuteV(sSqlCmd,sCardId,sCardNo,sInstCode);
	if(nRet !=0 && dbsSqlCode()!=1403)
	{
		logMsg(LOG_ERR,"���ݿ��ѯʧ��[%d][%s][%s]",dbsSqlCode(),dbsErrStr(),sSqlCmd);
		return -1;
	}
	if(1403 == dbsSqlCode())
	{
		logMsg(LOG_ERR,"�޿������˻�");
		return 1;
	}

	/*ȡ�˻�����*/
	nRet = dbsExecuteV("SELECT lpad(to_char(SEQ_ACCOUNTCODENO.NEXTVAL),19,'0') FROM DUAL",sAccountCode);
	if(nRet !=0)
	{
		logMsg(LOG_ERR,"ȡ�˻�����ʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}
	/*ȡ���˻�����*/
	nRet=CreateTgAcc(sCardId,sAccCode);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"ȡ���˻�����ʧ��");
		return -1;
	}
	/*ȡ�ͻ�ID*/
	nRet = dbsExecuteV("SELECT lpad(to_char(SEQ_CUSTIDNO.NEXTVAL),19,'0') FROM DUAL",sCustId);
	if(nRet != 0)
	{
		logMsg(LOG_ERR,"ȡ�ͻ�IDʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	/*INSERT�ͻ���Ϣ��CRM_CUSTOMERINFO*/
	nRet = dbsExecuteV("INSERT INTO CRM_CUSTOMERINFO(CUSTID,CUSTNAME,PERSONID)VALUES(:sCustId,:sCustName,:sPersonId)",
						sCustId,sAccName,sPersonId);
	if(nRet != 0)
	{
		logMsg(LOG_ERR,"INSERT�ͻ���Ϣ��CRM_CUSTOMERINFOʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	/*INSERT�˻���Ϣ��KP_ACCOUNOTINFO*/
	setnull(sSqlCmd);
	sprintf(sSqlCmd,"INSERT INTO KP_ACCOUNTINFO(accountcode,cardid,salenum,saleamt,voidnum,voidamt,rechargenum,"
					"rechargeamt,refundnum,refundamt,pointused,pointtotal,checkused,checktotal)"
					"VALUES('%s','%s',0,0,0,0,0,0,0,0,0,0,0,0)",sAccountCode,sCustId);
	nRet = dbsExecuteV(sSqlCmd);
	if(nRet != 0)
	{
		logMsg(LOG_ERR,"INSERT�˻���Ϣ��KP_ACCOUNOTINFOʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	/*INSERT���˻���Ϣ��KP_ACCINFO*/
	setnull(sSqlCmd);
	sprintf(sSqlCmd,"INSERT INTO KP_ACCINFO(accountcode,acccode,createdate,instcode,balance,endbalance,"
					"frozbalance,balancemd5,accstat)"
					"VALUES('%s','%s',fn_gettransdate(),'%s',0,0,0,rtrim(fn_getmd5(0)),'0')",sAccountCode,sAccCode,sInstCode);
	nRet = dbsExecuteV(sSqlCmd);
	if(nRet != 0)
	{
		logMsg(LOG_ERR,"INSERT���˻���Ϣ��KP_ACCINFOʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}
	/*INSERT�й�ҵ���˺�ӳ���TG_ACCMAP*/
	setnull(sSqlCmd);
	sprintf(sSqlCmd,"INSERT INTO TG_ACCMAP(refid,platformid,accountcode,acccode,cardid,trackcardno,custid,status,createdate)"
					"VALUES('%s','%s','%s','%s','%s','%s','%s','0',fn_gettransdate())",
					sAccId,sPlatformId,sAccountCode,sAccCode,sCardId,sCardNo,sCustId);
	nRet = dbsExecuteV(sSqlCmd);
	if(nRet != 0)
	{
		logMsg(LOG_ERR,"INSERT�й�ҵ���˺�ӳ���TG_ACCMAPʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	dbsCommit();
	logMsg(LOG_ERR,"sAccountCode[%s]sAccCode[%s]",sAccountCode,sAccCode);
	memcpy(pAccountCode,sAccountCode,sizeof(sAccountCode)-1);
	memcpy(pAccCode,sAccCode,sizeof(sAccCode)-1);
	
	return 0;
}

/*
����ⲿ�˺��Ƿ��Ѱ�
���:
pPlatformId  ƽ̨��
pAccId       �ⲿƽ̨ע���
pAccountCode ���ص��ڲ��˺�
pAccCode     ���ص��ڲ����˺�

return      0 ��ע��
            1 δע��
          < 0 ����ʧ��
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
		logMsg(LOG_ERR,"��ѯ���ݿ�ʧ��,SQLCODE[%d],ERRSTR[%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	if(atoi(sCount) == 1)
	{
		nRet = dbsExecuteV("SELECT accountcode,acccode FROM TG_ACCMAP WHERE REFID = :sAccId AND PLATFORMID = :sPlatformId",
			              sAccId,sPlatformId,sAccountCode,sAccCode);
		if(nRet != 0)
		{
			logMsg(LOG_ERR,"��ѯ���ݿ�ʧ��,SQLCODE[%d],ERRSTR[%s]",dbsSqlCode(),dbsErrStr());
			return -1;
		}
		memcpy(pAccountCode,sAccountCode,sizeof(sAccountCode)-1);
		memcpy(pAccCode,sAccCode,sizeof(sAccCode)-1);

		return 0;
	}
	else if(atoi(sCount) ==0)
		/*û����ذ���Ϣ*/
		return 1;
	else
		return -1;

	return 0;
}

/*
���¸����˻���
��Σ�
pAccountCode  �˻�����
pAccCode      ���˻�����
pType         �������ͣ�1-��ֵ��2-ת�룬3-ת����4-����
pAmount       ���
pFee          ������

return        0 �ɹ�
			  1 ����
			  2 ���MD5���ʧ��
            <>0 ʧ��
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
		logMsg(LOG_ERR,"����Ϊ0");
		return -1;
	}

	/*�˻����MD5У��*/
	setnull(sSqlCmd);
	sprintf(sSqlCmd,"select 1 from KP_ACCINFO where accountcode = '%s' and acccode ='%s'"
					" and rtrim(balancemd5) = rtrim(fn_getmd5(balance)) for update",
					sAccountCode,sAccCode);
	nRet = dbsExecuteV(sSqlCmd,sCount);
	if(nRet < 0 && dbsSqlCode() != 1403)
	{
		logMsg(LOG_ERR,"���ݿ��ѯʧ��[%d][%s][%s]",dbsSqlCode(),dbsErrStr(),sSqlCmd);
		return -1;
	}
	if(dbsSqlCode() == 1403)
	{
		logMsg(LOG_ERR,"�˻����MD5���ʧ��");
		return 2;
	}

	/*֧���ཻ���˻������*/
	if(memcmp(sType,"3",1) == 0 || memcmp(sType,"4",1) == 0)
	{
		setnull(sSqlCmd);
		sprintf(sSqlCmd,"select balance - frozbalance from KP_ACCINFO where accountcode = '%s' and acccode ='%s'"
						" and balance - ltrim (to_char('%s'/100,'9999999999999990.00')) - frozbalance >= 0",
						sAccountCode,sAccCode,sAmount);
		nRet = dbsExecuteV(sSqlCmd,sBalance);
		if(nRet != 0 && dbsSqlCode() != 1403)
		{
			logMsg(LOG_ERR,"���ݿ��ѯʧ��[%d][%s][%s]",dbsSqlCode(),dbsErrStr(),sSqlCmd);
			return -1;
		}
		if(dbsSqlCode() == 1403)
		{
			logMsg(LOG_ERR,"�˻�����:�������[%s]���۳����[%s]",sBalance,sAmount);
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
		logMsg(LOG_ERR,"�������ʹ���[%s]",sType);
		return -1;
	}
	logMsg(LOG_BUG,"sSqlCmd[%s]",sSqlCmd);

	nRet = dbsExecuteV(sSqlCmd);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"�������˻�����Ϣʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	return 0;
}

/*
����ƽ̨�˻���
��Σ�
pPlatformId  ƽ̨��
pType        �������ͣ�1-��ֵ��2-���֣�3-��ȡ������,4-������
pAmount      ���
pFee         ������

return        0 �ɹ�
			  1 ����
			  2 ���MD5���ʧ��
            <>0 ʧ��
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
		logMsg(LOG_ERR,"����Ϊ0");
		return -1;
	}

	/*ƽ̨�˻����md5У��*/
	sprintf(sSqlCmd,"select 1 from TG_PLATACCINFO where platformid = '%s' "
					"and rtrim(feebalancemd5) = rtrim(fn_getmd5(feeamount)) "
					"and rtrim(balancemd5) = rtrim(fn_getmd5(amount)) for update",sPlatformId);
	nRet = dbsExecuteV(sSqlCmd,sCount);
	if(nRet < 0 && dbsSqlCode() != 1403)
	{
		logMsg(LOG_ERR,"��ѯ���ݿ�ʧ��[%s][%d][%s]",sSqlCmd,dbsSqlCode(),dbsErrStr());
		return -1;
	}
	if(dbsSqlCode() == 1403)
	{
		logMsg(LOG_ERR,"���MD5���ʧ��");
		return 2;
	}

	if(memcmp(sType,"3",1) == 0)
	{
		/*�������Ƿ����*/
		setnull(sCount);
		setnull(sSqlCmd);
		sprintf(sSqlCmd,"select feeamount from TG_PLATACCINFO where platformid ='%s' "
						"and feeamount - nvl('%s',0) > 0",sPlatformId,sAmount);
		nRet = dbsExecuteV(sSqlCmd,sBalance);
		if(nRet < 0 && dbsSqlCode() != 1403)
		{
			logMsg(LOG_ERR,"��ѯ���ݿ�ʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
			return -1;
		}
		else if (dbsSqlCode() == 1403)
		{
			logMsg(LOG_ERR,"ƽ̨������");
			return 1;
		}

		/*��ȡ��ѯ�ѽ��Ϊ0����ȡȫ����ѯ��*/
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
		logMsg(LOG_ERR,"����ƽ̨�˻�����Ϣʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	return 0;
}

/*
���»����˻���
��Σ�
pPlatformId ƽ̨��
pInstCode   ������
pType       �������ͣ�1-��ֵ��2-ת�룬3-ת��
pAmount     ���
pFee        ������

return      0 �ɹ�
			1 ����
			2 ���У��ʧ��
          <>0 ʧ��
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

	/*��������MD5У��*/
	sprintf(sSqlCmd,"select 1 from TG_INSTACCINFO where platformid = '%s' and instcode = '%s'"
					"and rtrim(balancemd5) = rtrim(fn_getmd5(amount)) for update",sPlatformId,sInstCode);
	nRet = dbsExecuteV(sSqlCmd,sCount);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"��ѯ���ݿ�ʧ��[%s][%d][%s]",sSqlCmd,dbsSqlCode(),dbsErrStr());
		return -1;
	}
	if(!atoi(sCount))
	{
		logMsg(LOG_ERR,"���MD5���ʧ��");
		return 2;
	}
	/*֧���ཻ���˻������*/
	if(memcmp(sType,"3",1) == 0)
	{
		setnull(sSqlCmd);
		sprintf(sSqlCmd,"select amount from  TG_INSTACCINFO where instcode = '%s' and platformid ='%s'"
						" and amount - ltrim (to_char('%s'/100,'9999999999999990.00')) >= 0",
						sInstCode,sPlatformId,sAmount);
		nRet = dbsExecuteV(sSqlCmd,sBalance);
		if(nRet != 0 && dbsSqlCode() != 1403)
		{
			logMsg(LOG_ERR,"���ݿ��ѯʧ��[%d][%s][%s]",dbsSqlCode(),dbsErrStr(),sSqlCmd);
			return -1;
		}
		if(dbsSqlCode() == 1403)
		{
			logMsg(LOG_ERR,"�˻�����:�������[%s]���۳����[%s]",sBalance,sAmount);
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
		logMsg(LOG_ERR,"���»����˻�����Ϣʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	return 0;
}

/*
��¼����������ˮ��
��Σ�
pId         ������ˮ��
t_FLOW      ������Ϣ

return      0 �ɹ�
          <>0 ʧ��
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
		logMsg(LOG_ERR,"�����������ˮ��ʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	return 0;
}

/*
��¼����������ˮ��
��Σ�
pId         ������ˮ��
t_FLOW      ������Ϣ

return      0 �ɹ�
          <>0 ʧ��
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
		logMsg(LOG_ERR,"�����������ˮ��ʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}
	
	return 0;
}

/*
ȡ�й����˻����뺯��
���
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
		logMsg(LOG_ERR,"ȡ���˻�����ʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}
	logMsg(LOG_BUG,"sAccId[%s]",sAccId);
	memcpy(pAccId,sAccId,19);
	/**pAccId=sAccId;*/
	logMsg(LOG_BUG,"sAccId[%s]",pAccId);

	return 0;
}
