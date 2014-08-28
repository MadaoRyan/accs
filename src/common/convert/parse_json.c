#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "common.h"
#include "parse.h"


#define _PARSE(x,y,z) memcpy(x,cJSON_GetObjectItem(y,z)->valuestring,strlen(cJSON_GetObjectItem(y,z)->valuestring));

char* parse_60001(cJSON *json);
char* parse_60002(cJSON *json);
char* parse_60003(cJSON *json);
char* parse_60004(cJSON *json);
char* parse_60005(cJSON *json);
char* parse_60006(cJSON *json);
char* parse_60007(cJSON *json);
char* parse_60008(cJSON *json);
char* parse_90001(cJSON *json);
char* parse_90002(cJSON *json);
char* parse_90003(cJSON *json);
char* parse_90004(cJSON *json);


static T_TXN60001 t_txn60001;
static T_TXN60002 t_txn60002;
static T_TXN60003 t_txn60003;
static T_TXN60004 t_txn60004;
static T_TXN60005 t_txn60005;
static T_TXN60006 t_txn60006;
static T_TXN60007 t_txn60007;
static T_TXN60008 t_txn60008;
static T_TXN90001 t_txn90001;
static T_TXN90002 t_txn90002;
static T_TXN90003 t_txn90003;
static T_TXN90004 t_txn90004;

/*

解析Json报文函数
pInStr  Json 报文buffer
pTxnno  交易编号
pOutStr 解析后存放buffer,会根据不同交易存放在相应strcut中

iRet    返回处理结果 =0 正常

					 <>0 失败

备注：本函数不包括解析报头部分
*/
char * parse_json(char *pInStr,char *pTxnno)
{
	int i,j,iRet;
	char sTxnno  [5    + 1];
	char sInstr  [1024 * 2];
	char *sOutStr= NULL;

	cJSON *json;

	char * (*parseFunc) (cJSON *pIn);

	memset(sTxnno  ,0x00,sizeof(sTxnno  ));
	memset(sInstr  ,0x00,sizeof(sInstr  ));

	memcpy(sTxnno,pTxnno,sizeof(sTxnno) - 1);
	strcpy(sInstr,pInStr);

	json=cJSON_Parse(sInstr);
	if (!json)
	{
		logMsg(LOG_ERR,"Error before: [%s]\n",cJSON_GetErrorPtr());
		return NULL;
	}
	else
	{
		switch(atoi(sTxnno))
		{
			case 60001:parseFunc = parse_60001;break;
			case 60002:parseFunc = parse_60002;break;
			case 60003:parseFunc = parse_60003;break;
			case 60004:parseFunc = parse_60004;break;
			case 60005:parseFunc = parse_60005;break;
			case 60006:parseFunc = parse_60006;break;
			case 60007:parseFunc = parse_60007;break;
			case 60008:parseFunc = parse_60008;break;
			case 90001:parseFunc = parse_90001;break;
			case 90002:parseFunc = parse_90002;break;
			case 90003:parseFunc = parse_90003;break;
			case 90004:parseFunc = parse_90004;break;
			default:
				logMsg(LOG_ERR,"txnno[%s] is error!\n",sTxnno);
				cJSON_Delete(json);
				return NULL;
		}

		sOutStr = parseFunc(json);
		if(iRet < 0)
		{
			logMsg(LOG_ERR,"解析报文失败！");
			cJSON_Delete(json);
			return NULL;
		}
	}

	cJSON_Delete(json);

	return sOutStr;
}

T_HEAD parse_head(char *pStr)
{
	T_HEAD t_head;

	memset(&t_head,0x00,sizeof(T_HEAD));

	logMsg(LOG_TRC,"PARSE MSG HEAD BEGIN...");
	memcpy(t_head.sTxnno      ,pStr               ,5 );
	logMsg(LOG_TRC,"sTxnno      [%s]",t_head.sTxnno);
	memcpy(t_head.sTxnDate    ,pStr+5             ,8 );
	logMsg(LOG_TRC,"sTxnDate    [%s]",t_head.sTxnDate);
	memcpy(t_head.sTxnTime    ,pStr+5+8           ,6 );
	logMsg(LOG_TRC,"sTxnTime    [%s]",t_head.sTxnTime);
	memcpy(t_head.sTxnId      ,pStr+5+8+6         ,16);
	logMsg(LOG_TRC,"sTxnId      [%s]",t_head.sTxnId);
	memcpy(t_head.sRtCode     ,pStr+5+8+6+16      ,8 );
	logMsg(LOG_TRC,"sRtCode     [%s]",t_head.sRtCode);
	memcpy(t_head.sChannelCode,pStr+5+8+6+16+8    ,8 );
	logMsg(LOG_TRC,"sChannelCode[%s]",t_head.sChannelCode);
	memcpy(t_head.sTermCode   ,pStr+5+8+6+16+8+8  ,8 );
	logMsg(LOG_TRC,"sTermCode   [%s]",t_head.sTermCode);
	memcpy(t_head.sDrct       ,pStr+5+8+6+16+8+8+8,1 );
	logMsg(LOG_TRC,"sDrct       [%s]",t_head.sDrct);
	logMsg(LOG_TRC,"PARSE MSG HEAD END");

	return t_head;
}

char* parse_60001(cJSON *json)
{
	int i;
	char *sStr;

	memset(&t_txn60001,0x00,sizeof(T_TXN60001));

	logMsg(LOG_TRC,"PARSE MSG 60001 BEGIN...");

	_PARSE(t_txn60001.sAccId     ,json,"accid"         );
	logMsg(LOG_TRC,"accid     [%32.32s]",t_txn60001.sAccId     );
	_PARSE(t_txn60001.sPlatformId,json,"platformid"    );
	logMsg(LOG_TRC,"platformid[%8.8s]"  ,t_txn60001.sPlatformId);
	_PARSE(t_txn60001.sAmount    ,json,"amount"        );
	logMsg(LOG_TRC,"amount    [%18.18s]",t_txn60001.sAmount    );
	_PARSE(t_txn60001.sFee       ,json,"fee"           );
	logMsg(LOG_TRC,"fee       [%18.18s]",t_txn60001.sFee       );
	_PARSE(t_txn60001.sIdType    ,json,"idtype"        );
	logMsg(LOG_TRC,"idtype    [%2.2s]"  ,t_txn60001.sIdType     );
	_PARSE(t_txn60001.sPersonId  ,json,"personid"      );
	logMsg(LOG_TRC,"personid  [%18.18s]",t_txn60001.sPersonId   );
	_PARSE(t_txn60001.sAccName   ,json,"accname"       );
	logMsg(LOG_TRC,"accname   [%20.20s]",t_txn60001.sAccName    );

	logMsg(LOG_TRC,"PARSE MSG 60001 END");

	sStr = (char *)(&t_txn60001);

	return sStr;
}
char* parse_60002(cJSON *json)
{
	char *sStr;
	memset(&t_txn60002,0x00,sizeof(T_TXN60002));

	logMsg(LOG_TRC,"PARSE MSG 60002 BEGIN...");

	_PARSE(t_txn60002.sAccId     ,json,"accid"         );
	logMsg(LOG_TRC,"accid     [%32.32s]",t_txn60002.sAccId     );
	_PARSE(t_txn60002.sPlatformId,json,"platformid"    );
	logMsg(LOG_TRC,"platformid[%8.8s]"  ,t_txn60002.sPlatformId);
	_PARSE(t_txn60002.sInstCode  ,json,"instcode"      );
	logMsg(LOG_TRC,"instcode  [%8.8s]"  ,t_txn60002.sInstCode  );
	_PARSE(t_txn60002.sAmount    ,json,"amount"        );
	logMsg(LOG_TRC,"amount    [%18.18s]",t_txn60002.sAmount    );

	logMsg(LOG_TRC,"PARSE MSG 60002 END");

	sStr = (char *)(&t_txn60002);

	return sStr;
}
char* parse_60003(cJSON *json)
{
	char *sStr;

	memset(&t_txn60003,0x00,sizeof(T_TXN60003));

	logMsg(LOG_TRC,"PARSE MSG 60003 BEGIN...");

	_PARSE(t_txn60003.sAccId     ,json,"accid"         );
	logMsg(LOG_TRC,"accid     [%32.32s]",t_txn60003.sAccId     );
	_PARSE(t_txn60003.sPlatformId,json,"platformid"    );
	logMsg(LOG_TRC,"platformid[%8.8s]"  ,t_txn60003.sPlatformId);
	_PARSE(t_txn60003.sAmount    ,json,"amount"        );
	logMsg(LOG_TRC,"amount    [%18.18s]",t_txn60003.sAmount    );
	_PARSE(t_txn60003.sFee       ,json,"fee"           );
	logMsg(LOG_TRC,"fee       [%18.18s]",t_txn60003.sFee       );

	logMsg(LOG_TRC,"PARSE MSG 60003 END");

	sStr = (char *)(&t_txn60003);
	return  sStr;
}
char* parse_60004(cJSON *json)
{
	char *sStr;

	memset(&t_txn60004,0x00,sizeof(T_TXN60004));

	logMsg(LOG_TRC,"PARSE MSG 60004 BEGIN...");

	_PARSE(t_txn60004.sPayerAccId     ,json,"payeraccid");
	logMsg(LOG_TRC,"payeraccid[%32.32s]",t_txn60004.sPayerAccId);
	_PARSE(t_txn60004.sPayeeAccId     ,json,"payeeaccid");
	logMsg(LOG_TRC,"payeeaccid[%32.32s]",t_txn60004.sPayeeAccId);
	_PARSE(t_txn60004.sPlatformId,json,"platformid"    );
	logMsg(LOG_TRC,"platformid[%8.8s]"  ,t_txn60004.sPlatformId);
	_PARSE(t_txn60004.sAmount    ,json,"amount"        );
	logMsg(LOG_TRC,"amount    [%18.18s]",t_txn60004.sAmount    );
	_PARSE(t_txn60004.sFee       ,json,"fee"           );
	logMsg(LOG_TRC,"fee       [%18.18s]",t_txn60004.sFee       );
	_PARSE(t_txn60004.sAccName   ,json,"accname"       );
	logMsg(LOG_TRC,"accname   [%20.20s]",t_txn60004.sAccName   );

	logMsg(LOG_TRC,"PARSE MSG 60004 END");

	sStr = (char *)(&t_txn60004);
	return  sStr;
}
char* parse_60005(cJSON *json)
{
	char *sStr;

	memset(&t_txn60005,0x00,sizeof(T_TXN60005));

	logMsg(LOG_TRC,"PARSE MSG 60005 BEGIN...");

	_PARSE(t_txn60005.sPayeeAccId,json,"payeeaccid");
	logMsg(LOG_TRC,"payeeaccid[%32.32s]",t_txn60005.sPayeeAccId);
	_PARSE(t_txn60005.sPlatformId,json,"platformid");
	logMsg(LOG_TRC,"platformid[%8.8s]"  ,t_txn60005.sPlatformId);
	_PARSE(t_txn60005.sInstCode  ,json,"instcode"  );
	logMsg(LOG_TRC,"instcode  [%8.8s]"  ,t_txn60005.sInstCode  );
	_PARSE(t_txn60005.sTransType ,json,"transtype" );
	logMsg(LOG_TRC,"transtype [%2.2s]"  ,t_txn60005.sTransType );
	_PARSE(t_txn60005.sAmount    ,json,"amount"    );
	logMsg(LOG_TRC,"amount    [%18.18s]",t_txn60005.sAmount    );
	_PARSE(t_txn60005.sFee       ,json,"fee"       );
	logMsg(LOG_TRC,"fee       [%18.18s]",t_txn60005.sFee       );
	_PARSE(t_txn60005.sAccName   ,json,"accname"   );
	logMsg(LOG_TRC,"accname   [%20.20s]",t_txn60005.sAccName   );

	logMsg(LOG_TRC,"PARSE MSG 60005 END");

	sStr = (char *)(&t_txn60005);

	return  sStr;
}
char* parse_60006(cJSON *json)
{
	char *sStr;

	memset(&t_txn60006,0x00,sizeof(T_TXN60006));

	logMsg(LOG_TRC,"PARSE MSG 60006 BEGIN...");

	_PARSE(t_txn60006.sPlatformId,json  ,"platformid");
	logMsg(LOG_TRC,"platformid[%8.8s]"  ,t_txn60006.sPlatformId);
	_PARSE(t_txn60006.sType      ,json  ,"type"      );
	logMsg(LOG_TRC,"type      [%1.1s]"  ,t_txn60006.sType      );
	_PARSE(t_txn60006.sAmount    ,json,"amount"      );
	logMsg(LOG_TRC,"amount    [%18.18s]",t_txn60006.sAmount    );

	logMsg(LOG_TRC,"PARSE MSG 60006 END");

	sStr = (char *)(&t_txn60006);
	return  sStr;
}
char* parse_60007(cJSON *json)
{
	char *sStr;

	memset(&t_txn60007,0x00,sizeof(T_TXN60007));

	logMsg(LOG_TRC,"PARSE MSG 60007 BEGIN...");

	_PARSE(t_txn60007.sPlatformId,json,"platformid");
	logMsg(LOG_TRC,"platformid[%8.8s]"  ,t_txn60007.sPlatformId);
	_PARSE(t_txn60007.sInstCode  ,json,"instcode"  );
	logMsg(LOG_TRC,"instcode  [%8.8s]"  ,t_txn60007.sInstCode  );
	_PARSE(t_txn60007.sChgType  ,json,"chgtype"    );
	logMsg(LOG_TRC,"chgtype   [%2.2s]"  ,t_txn60007.sChgType   );
	_PARSE(t_txn60007.sInstName  ,json,"instname"  );
	logMsg(LOG_TRC,"instname  [%80.80s]",t_txn60007.sInstName  );
	_PARSE(t_txn60007.sRemarks  ,json,"remarks"    );
	logMsg(LOG_TRC,"remarks   [%40.40s]",t_txn60007.sRemarks   );

	logMsg(LOG_TRC,"PARSE MSG 60007 END");

	sStr = (char *)(&t_txn60007);

	return  sStr;
}
char* parse_60008(cJSON *json)
{
	char *sStr;

	memset(&t_txn60008,0x00,sizeof(T_TXN60008));

	logMsg(LOG_TRC,"PARSE MSG 60008 BEGIN...");

	_PARSE(t_txn60008.sPlatformId  ,json,"platformid"  );
	logMsg(LOG_TRC,"platformid  [%8.8s]"  ,t_txn60008.sPlatformId  );
	_PARSE(t_txn60008.sChgType     ,json,"chgtype"     );
	logMsg(LOG_TRC,"chgtype     [%2.2s]"  ,t_txn60008.sChgType     );
	_PARSE(t_txn60008.sPlatformName,json,"platformname");
	logMsg(LOG_TRC,"platformname[%80.80s]",t_txn60008.sPlatformName);
	_PARSE(t_txn60008.sRemarks     ,json,"remarks"     );
	logMsg(LOG_TRC,"remarks     [%40.40s]",t_txn60008.sRemarks     );

	logMsg(LOG_TRC,"PARSE MSG 60008 END");
	sStr = (char *)(&t_txn60008);
	return  sStr;
}
char* parse_90001(cJSON *json)
{
	char *sStr;

	memset(&t_txn90001,0x00,sizeof(T_TXN90001));

	logMsg(LOG_TRC,"PARSE MSG 90001 BEGIN...");
	_PARSE(t_txn90001.sAccId     ,json,"accid"         );
	logMsg(LOG_TRC,"accid     [%32.32s]",t_txn90001.sAccId     );
	_PARSE(t_txn90001.sPlatformId,json,"platformid"    );
	logMsg(LOG_TRC,"platformid[%8.8s]"  ,t_txn90001.sPlatformId);

	logMsg(LOG_TRC,"PARSE MSG 90001 END");

	sStr = (char *)(&t_txn90001);
	return  sStr;
}
char* parse_90002(cJSON *json)
{
	char *sStr;

	memset(&t_txn90002,0x00,sizeof(T_TXN90002));

	logMsg(LOG_TRC,"PARSE MSG 90002 BEGIN...");

	_PARSE(t_txn90002.sPlatformId,json,"platformid"    );
	logMsg(LOG_TRC,"platformid[%8.8s]"  ,t_txn90002.sPlatformId);

	logMsg(LOG_TRC,"PARSE MSG 90002 END");

	sStr = (char *)(&t_txn90002);
	return  sStr;
}
char* parse_90003(cJSON *json)
{
	char *sStr;

	memset(&t_txn90003,0x00,sizeof(T_TXN90003));

	logMsg(LOG_TRC,"PARSE MSG 90003 BEGIN...");

	_PARSE(t_txn90003.sPlatformId,json,"platformid");
	logMsg(LOG_TRC,"platformid[%8.8s]",t_txn90003.sPlatformId);
	_PARSE(t_txn90003.sStartDate ,json,"startdate" );
	logMsg(LOG_TRC,"startdate [%8.8s]",t_txn90003.sStartDate );
	_PARSE(t_txn90003.sEndDate   ,json,"enddate"   );
	logMsg(LOG_TRC,"enddate   [%8.8s]",t_txn90003.sEndDate   );
	_PARSE(t_txn90003.sPageSize  ,json,"pagesize"  );
	logMsg(LOG_TRC,"pagesize  [%8.8s]",t_txn90003.sPageSize  );
	_PARSE(t_txn90003.sPageNum   ,json,"page"      );
	logMsg(LOG_TRC,"page      [%8.8s]",t_txn90003.sPageNum   );

	logMsg(LOG_TRC,"PARSE MSG 90003 END");

	sStr = (char *)(&t_txn90003);
	return  sStr;
}
char* parse_90004(cJSON *json)
{
	char *sStr;

	memset(&t_txn90004,0x00,sizeof(T_TXN90004));

	logMsg(LOG_TRC,"PARSE MSG 90004 BEGIN...");

	_PARSE(t_txn90004.sPlatformId,json,"platformid"    );
	logMsg(LOG_TRC,"platformid[%8.8s]"  ,t_txn90004.sPlatformId);

	logMsg(LOG_TRC,"PARSE MSG 90004 END");

	sStr = (char *)(&t_txn90004);
	
	return  sStr;
}

