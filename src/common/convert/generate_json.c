#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "parse.h"
#include "common.h"

#define MAX_MSG_SIZE 1024 * 10


char* generate_60001(char *pInStr);
char* generate_60002(char *pInStr);
char* generate_60003(char *pInStr);
char* generate_60004(char *pInStr);
char* generate_60005(char *pInStr);
char* generate_60006(char *pInStr);
char* generate_60007(char *pInStr);
char* generate_60008(char *pInStr);
char* generate_90001(char *pInStr);
char* generate_90002(char *pInStr);
char* generate_90003(char *pInStr);
char* generate_90004(char *pInStr);

char* generate_json(char *pInStr,char *pTxnno)
{
	logMsg(LOG_TRC,"解析报文开始");
	int iRet;
	char  sTxnno[5 + 1];
	char *sOutStr;

	char * (*genFunc)(char *pIn);
	if(pInStr[0] =='\0')
	{
		logMsg(LOG_BUG,"报文为空");
		return NULL;
	}

	memset(sTxnno,0x00,sizeof(sTxnno));
	memcpy(sTxnno,pTxnno,5);

	switch(atoi(sTxnno)){
		case 60001: genFunc=generate_60001;break;
		case 60002: genFunc=generate_60002;break;
		case 60003: genFunc=generate_60003;break;
		case 60004: genFunc=generate_60004;break;
		case 60005: genFunc=generate_60005;break;
		case 60006: genFunc=generate_60006;break;
		case 60007: genFunc=generate_60007;break;
		case 60008: genFunc=generate_60008;break;
		case 90001: genFunc=generate_90001;break;
		case 90002: genFunc=generate_90002;break;
		case 90003: genFunc=generate_90003;break;
		case 90004: genFunc=generate_90004;break;
		default:
			logMsg(LOG_ERR,"err txnno [%s]\n",sTxnno);
			return NULL;
	}

	sOutStr=genFunc(pInStr);
	if(iRet < 0)
	{
		logMsg(LOG_ERR,"generate msg error [%d]\n",iRet);
		return NULL;
	}

	return sOutStr;
}

char* generate_60001(char *pInStr)
{
	int iRet,i,iLen=0;
	char *sOutStr=NULL;
	T_TXN60001R *x = (T_TXN60001R *) pInStr;

	cJSON *root;

	root=cJSON_CreateObject();
	cJSON_AddStringToObject(root,"balance"   ,x->sAmount   );

	sOutStr=cJSON_Print(root);
	cJSON_Delete(root);

	return sOutStr;
}
char* generate_60002(char *pInStr)
{
	int iRet,i,iLen=0;
	char *sOutStr=NULL;
	T_TXN60002R *x = (T_TXN60002R *) pInStr;

	cJSON *root;

	root=cJSON_CreateObject();
	cJSON_AddStringToObject(root,"balance"   ,x->sAmount   );

	sOutStr=cJSON_Print(root);
	cJSON_Delete(root);

	return sOutStr;
}
char* generate_60003(char *pInStr)
{
	int iRet,i,iLen=0;
	char *sOutStr=NULL;
	T_TXN60003R *x = (T_TXN60003R *) pInStr;

	cJSON *root;

	root=cJSON_CreateObject();
	cJSON_AddStringToObject(root,"balance"   ,x->sAmount   );

	sOutStr=cJSON_Print(root);
	cJSON_Delete(root);

	return sOutStr;
}
char* generate_60004(char *pInStr)
{return NULL;}
char* generate_60005(char *pInStr)
{return NULL;}
char* generate_60006(char *pInStr)
{
	int iRet,i,iLen=0;
	char *sOutStr=NULL;
	T_TXN60006R *x = (T_TXN60006R *) pInStr;

	cJSON *root;

	root=cJSON_CreateObject();
	cJSON_AddStringToObject(root,"balance"   ,x->sAmount   );

	sOutStr=cJSON_Print(root);
	cJSON_Delete(root);

	return sOutStr;
}
char* generate_60007(char *pInStr)
{return NULL;}
char* generate_60008(char *pInStr)
{return NULL;}
char* generate_90001(char *pInStr)
{
	int iRet,i,iLen=0;
	char *sOutStr=NULL;
	T_TXN90001R *x = (T_TXN90001R *) pInStr;

	cJSON *root;

	root=cJSON_CreateObject();
	cJSON_AddStringToObject(root,"platformid",x->sPlatformId);
	cJSON_AddStringToObject(root,"accid"     ,x->sAccId     );
	cJSON_AddStringToObject(root,"balance"   ,x->sBalance   );

	sOutStr=cJSON_Print(root);
	cJSON_Delete(root);

	return sOutStr;
}
char* generate_90002(char *pInStr)
{
	int iRet,i,iLen=0;
	char *sOutStr=NULL;
	T_TXN90002R *x = (T_TXN90002R *) pInStr;

	cJSON *root;

	root=cJSON_CreateObject();
	cJSON_AddStringToObject(root,"platformid",x->sPlatformId);
	cJSON_AddStringToObject(root,"balance"   ,x->sBalance   );

	sOutStr=cJSON_Print(root);
	cJSON_Delete(root);

	return sOutStr;
}
char* generate_90003(char *pInStr)
{
	int iRet,i,iLen=0;
	char *sOutStr=NULL;
	T_TXN90003R *x = (T_TXN90003R *) pInStr;

	cJSON *root,*L0,*arry;

	root=cJSON_CreateObject();
	cJSON_AddStringToObject(root,"platformid",x->sPlatformId);
	cJSON_AddStringToObject(root,"total"     ,x->sTotal     );
	cJSON_AddItemToObject(root,"detaillist",L0=cJSON_CreateArray());

	for(i = 0;i< atoi(x->sNumber);i++)
	{
		cJSON_AddItemToArray(L0,arry=cJSON_CreateObject());
		cJSON_AddStringToObject(arry, "transdate", x->L0[i].sTransDate);
		cJSON_AddStringToObject(arry, "amount"   , x->L0[i].sAmount   );
		cJSON_AddStringToObject(arry, "accid"    , x->L0[i].sAccId    );
		cJSON_AddStringToObject(arry, "fee"      , x->L0[i].sFee      );
		cJSON_AddStringToObject(arry, "txnno"    , x->L0[i].sTxnNo    );
	}

	sOutStr=cJSON_Print(root);
	cJSON_Delete(root);
	return sOutStr;
}
char* generate_90004(char *pInStr)
{
	int iRet,i,iLen=0;
	char *sOutStr=NULL;
	T_TXN90004R *x = (T_TXN90004R *) pInStr;

	cJSON *root;

	root=cJSON_CreateObject();
	cJSON_AddStringToObject(root,"platformid",x->sPlatformId);
	cJSON_AddStringToObject(root,"amount"    ,x->sAmount    );

	sOutStr=cJSON_Print(root);
	cJSON_Delete(root);
	return sOutStr;
}
