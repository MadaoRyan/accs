#include "common_txn.h"

int txn60001Initial(char *pData,T_HEAD t_Head);
int txn60001Process();
int txn60001End();

/*������Ϣ����*/
static char f_sAccId     [LEN_ACCID     + 1];
static char f_sPlatformId[LEN_PLATID    + 1];
static char f_sAmount    [LEN_AMOUNT    + 1];
static char f_sFee       [LEN_AMOUNT    + 1];
static char f_sIdType    [LEN_ID_TYPE   + 1];
static char f_sPersonId  [LEN_PERSON_ID + 1];
static char f_sAccName   [LEN_NAME      + 1];

/*ͷ������Ϣ����*/
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

	logMsg(LOG_TRC,"60001��ֵҵ�񣬴���ʼ");

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

	/*����ⲿ�˺��Ƿ��Ѱ�*/
	iRet = CheckAcc(f_sPlatformId,f_sAccId,sAccountCode,sAccCode);
	if(iRet < 0)
	{
		logMsg(LOG_ERR,"����ʧ��[%d]",iRet);
		strcat(f_sError,"E0000008");
		return -1;
	}
	else if(iRet == 1)
	{
		/*�˺Ż�δ��*/
		logMsg(LOG_BUG,"�ⲿ�˺Ż�δ���а󶨣��󶨿�ʼ...");
		nRet = CreateAcc(f_sPlatformId,f_sAccId,f_sIdType,f_sPersonId,f_sAccName,sAccountCode,sAccCode);
		if(nRet == 1)
		{
			logMsg(LOG_ERR,"���˺�ʧ�ܣ��޿���ʵ�忨");
			strcat(f_sError,"E0000001");
			return -1;
		}
		else if(nRet != 0)
		{
			logMsg(LOG_ERR,"���˺�ʧ��");
			strcat(f_sError,"E0000009");
			return -1;
		}
	}

	/*����˻�״̬*/
	setnull(sSqlCmd);
	sprintf(sSqlCmd,"SELECT count(1) FROM KP_ACCINFO WHERE accountcode=%s and acccode=%s and accstat ='0'",sAccountCode,sAccCode);
	nRet = dbsExecuteV(sSqlCmd,sCount);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"��ѯ���ݿ�ʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
		return -1;
	}

	if(!atoi(sCount))
	{
		logMsg(LOG_ERR,"�˻�״̬���ʧ��");
		strcat(f_sError,"E0000011");
		return -1;
	}

	/*�޸ĸ����˻�*/
	nRet = UpdateAcc(sAccountCode,sAccCode,"1",f_sAmount,f_sFee);
	if(nRet == 2)
	{
		logMsg(LOG_ERR,"�˻����У��ʧ��[%d]",nRet);
		strcat(f_sError,"E0000010");
		return -1;
	}
	else if(nRet < 0)
	{
		logMsg(LOG_ERR,"��ֵʧ��[%d]",nRet);
		return -1;
	}

	/*��¼ƽ̨�˻�*/
	nRet = UpdatePlatAcc(f_sPlatformId,"1",f_sAmount,f_sFee);
	if(nRet == 2)
	{
		logMsg(LOG_ERR,"�˻����У��ʧ��[%d]",nRet);
		strcat(f_sError,"E0000010");
		return -1;
	}
	else if(nRet < 0)
	{
		logMsg(LOG_ERR,"��¼ƽ̨�˻�ʧ��[%d]",nRet);
		return -1;
	}

	/*ȡ������ˮ*/
	nRet = dbsExecuteV("SELECT fn_gettransdate()||lpad(to_char(SEQ_TG_LS.NEXTVAL), 8, '0') FROM DUAL",sId);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"ȡ������ˮʧ��[%d]",nRet);
		return -1;
	}

	/*ȡ������Ϣ*/
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
	/*��¼����������ˮ��*/
	nRet = InsertPerAccFlow(t_FLOW);
	if(nRet < 0)
	{
		logMsg(LOG_ERR,"��¼����������ˮʧ��[%d]",nRet);
		return -1;
	}

	setnull(sSqlCmd);
        sprintf(sSqlCmd,"SELECT (balance-frozbalance) * 100  FROM KP_ACCINFO WHERE accountcode=%s and acccode=%s and accstat ='0'",sAccountCode,sAccCode);
        nRet = dbsExecuteV(sSqlCmd,t_Rep.sAmount);
        if(nRet < 0)
        {
                logMsg(LOG_ERR,"��ѯ���ݿ�ʧ��[%d][%s]",dbsSqlCode(),dbsErrStr());
                return -1;
        }

	return 0;
}

int txn60001End()
{
	logMsg(LOG_TRC,"60001��ֵҵ�񣬴������");
	return 0;
}
