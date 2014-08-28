#include "common.h"
#include "tl_dbs.h"
#include "parse.h"

#define setnull(x) memset(&(x),0x00,sizeof(x));


typedef struct T_ACCFLOW
{
	char sId         [LEN_TXNID   + 1];
	char sPlatformId [LEN_PLATID  + 1];
	char sInstCode   [LEN_PLATID  + 1];
	char sTxnno      [LEN_TXNNO   + 1];
	char sTxnCode    [4           + 1];
	char sChlCode    [LEN_CHLCODE + 1];
	char sTermCode   [LEN_TERMCODE+ 1];
	char sAccountId  [19          + 1];
	char sLinkAccId  [19          + 1];
	char sAccCode    [19          + 1];
	char sLinkAccCode[19          + 1];
	char sTransDate  [LEN_DATE    + 1];
	char sTransTime  [LEN_TIME    + 1];
	char sAmount     [LEN_AMOUNT  + 1];
	char sFee        [LEN_AMOUNT  + 1];
	char sTxnId      [LEN_TXNID   + 1];
}T_ACCFLOW;
/*
创建账号映射及子账户
入参：
pPlatformId 平台号
pAccId      外部平台注册号
pAccountCode 返回的内部账号
pAccCode     返回的内部子账号

return      0 成功
          <>0 失败
*/
int CreateAcc(char *pPlatformId,char *pAccId,char *pIdType,char *pPerSonId,char *pAccName,char *pAccountCode,char *pAccCode);

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
int CheckAcc(char *pPlatformId,char *pAccId,char *pAccountCode,char *pAccCode);

/*
更新个人账户表
入参：
pAccountCode  账户代码
pAccCode      子账户代码
pType         更新类型：1-充值，2-转入，3-转出，4-提现
pAmount       金额
pFee          手续费

return        0 成功
            <>0 失败
*/
int UpdateAcc(char *pAccountCode,char *pAccCode,char *pType,char *pAmount,char *pFee);

/*
更新平台账户表
入参：
pPlatformId  平台号
pType        更新类型：1-充值，2-提现，3，收手续费
pAmount      金额
pFee         手续费
*/
int UpdatePlatAcc(char *pPlatformId,char *pType,char *pAmount,char *pFee);

/*
更新机构账户表
入参：
pPlatformId 平台号
pInstCode   机构号
pType       更新类型：1-充值，2-转入，3-转出，4-提现
pAmount     金额
pFee        手续费

return      0 成功
          <>0 失败
*/
int UpdateInstAcc(char *pPlatformId,char *pInstCode,char *pType,char *pAmount,char *pFee);

/*
记录个人账务流水表
入参：
pId         账务流水号
t_FLOW      记账信息

return      0 成功
          <>0 失败
*/
int InsertPerAccFlow(T_ACCFLOW t_FLOW);

/*
记录机构账务流水表
入参：
pId         账务流水号
t_FLOW      记账信息

return      0 成功
          <>0 失败
*/
int InsertOrgAccFlow(T_ACCFLOW t_FLOW);
