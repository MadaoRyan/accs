#ifndef _PARSE_H
#define _PARSE_H

#define LEN_HEAD                  60
#define LEN_ACCID                 32
#define LEN_PLATID                8
#define LEN_AMOUNT                18
#define LEN_ID_TYPE               2
#define LEN_PERSON_ID             18
#define LEN_NAME                  20
#define LEN_TYPE                  1
#define LEN_CHG_TYPE              2
#define LEN_PLAT_NAME             80
#define LEN_REMARKS               40
#define LEN_DATE                  8
#define LEN_TIME                  6
#define LEN_NUM                   8
#define LEN_TXNID                 16
#define LEN_TXNNO                 5
#define LEN_RTCODE                8
#define LEN_CHLCODE               8
#define LEN_TERMCODE              8
#define LEN_DRCT                  1

typedef struct T_HEAD
{
        char sTxnno      [LEN_TXNNO       + 1];
        char sTxnDate    [LEN_DATE        + 1];
        char sTxnTime    [LEN_TIME        + 1];
        char sTxnId      [LEN_TXNID       + 1];
        char sRtCode     [LEN_RTCODE      + 1];
        char sChannelCode[LEN_CHLCODE     + 1];
        char sTermCode   [LEN_TERMCODE    + 1];
        char sDrct       [LEN_DRCT        + 1];
}T_HEAD;

typedef struct T_TXN60001
{
        char sAccId     [LEN_ACCID     + 1];/*外部系统客户注册号*/
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sAmount    [LEN_AMOUNT    + 1];/*充值金额*/
        char sFee       [LEN_AMOUNT    + 1];/*咨询费金额*/
        char sIdType    [LEN_ID_TYPE   + 1];/*证件类型*/
        char sPersonId  [LEN_PERSON_ID + 1];/*证件号*/
        char sAccName   [LEN_NAME      + 1];/*客户姓名*/
}T_TXN60001;

typedef struct T_TXN60001R
{
        char sAmount    [LEN_AMOUNT    + 1];/*充值后金额*/
}T_TXN60001R;

typedef struct T_TXN60002
{
        char sAccId     [LEN_ACCID     + 1];/*外部系统客户注册号*/
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sInstCode  [LEN_PLATID    + 1];/*机构号*/
        char sAmount    [LEN_AMOUNT    + 1];/*充值金额*/
}T_TXN60002;

typedef struct T_TXN60002R
{
        char sAmount    [LEN_AMOUNT    + 1];/*充值后金额*/
}T_TXN60002R;

typedef struct T_TXN60003
{
        char sAccId     [LEN_ACCID     + 1];/*外部系统客户注册号*/
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sAmount    [LEN_AMOUNT    + 1];/*提现金额*/
        char sFee       [LEN_AMOUNT    + 1];/*咨询费金额*/
}T_TXN60003;

typedef struct T_TXN60003R
{
        char sAmount    [LEN_AMOUNT    + 1];/*提现后金额*/
}T_TXN60003R;

typedef struct T_TXN60004
{
        char sPayerAccId[LEN_ACCID     + 1];/*出借人外部系统注册号*/
        char sPayeeAccId[LEN_ACCID     + 1];/*借款人外部系统注册号*/
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sAmount    [LEN_AMOUNT    + 1];/*转账金额*/
        char sFee       [LEN_AMOUNT    + 1];/*咨询费金额*/
        char sAccName   [LEN_NAME      + 1];/*借款人姓名*/
}T_TXN60004;

typedef struct T_TXN60005
{
        char sPayeeAccId[LEN_ACCID     + 1];/*借款人外部系统注册号*/
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sInstCode  [LEN_PLATID    + 1];/*机构号*/
        char sTransType [LEN_CHG_TYPE  + 1];
        char sAmount    [LEN_AMOUNT    + 1];/*转账金额*/
        char sFee       [LEN_AMOUNT    + 1];/*咨询费金额*/
        char sAccName   [LEN_NAME      + 1];/*借款人姓名*/
}T_TXN60005;

typedef struct T_TXN60006
{
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sType      [LEN_TYPE      + 1];/*结算类型*/
        char sAmount    [LEN_AMOUNT    + 1];/*结算金额*/
}T_TXN60006;

typedef struct T_TXN60006R
{
        char sAmount    [LEN_AMOUNT    + 1];/*结算后金额*/
}T_TXN60006R;

typedef struct T_TXN60007
{
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sInstCode  [LEN_PLATID    + 1];/*机构号*/
        char sChgType   [LEN_CHG_TYPE  + 1];/*维护类型*/
        char sInstName  [LEN_PLAT_NAME + 1];/*机构名*/
        char sRemarks   [LEN_REMARKS   + 1];/*备注*/
}T_TXN60007;

typedef struct T_TXN60008
{
        char sPlatformId  [LEN_PLATID    + 1];/*平台号*/
        char sChgType     [LEN_CHG_TYPE  + 1];/*维护类型*/
        char sPlatformName[LEN_PLAT_NAME + 1];/*平台名*/
        char sRemarks     [LEN_REMARKS   + 1];/*备注*/
}T_TXN60008;

typedef struct T_TXN90001
{
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sAccId     [LEN_ACCID     + 1];/*个人账号*/
}T_TXN90001;

typedef struct T_TXN90001R
{
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sAccId     [LEN_ACCID     + 1];/*个人账号*/
        char sBalance   [LEN_AMOUNT    + 1];/*账户余额*/
}T_TXN90001R;

typedef struct T_TXN90002
{
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
}T_TXN90002;

typedef struct T_TXN90002R
{
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sBalance   [LEN_AMOUNT    + 1];/*账户余额*/
}T_TXN90002R;

typedef struct T_TXN90003
{
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sStartDate [LEN_DATE      + 1];/*开始日期*/
        char sEndDate   [LEN_DATE      + 1];/*结束日期*/
        char sPageSize  [LEN_NUM       + 1];/*页大小*/
        char sPageNum   [LEN_NUM       + 1];/*页数*/
}T_TXN90003;

typedef struct T_90003_LIST
{
        char sTransDate [LEN_DATE      + 1];/*委托日期*/
        char sAmount    [LEN_AMOUNT    + 1];/*借款金额*/
        char sAccId     [LEN_ACCID     + 1];/*借款人平台账号*/
        char sFee       [LEN_AMOUNT    + 1];/*咨询费*/
        char sTxnNo     [LEN_TXNID     + 1];/*交易流水号*/
}T_90003_LIST;

typedef struct T_TXN90003R
{
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sTotal     [LEN_NUM       + 1];/*总记录数*/
        char sNumber    [8             + 1];/*循环记录数*/
        T_90003_LIST    L0[99];/*明细列表*/
}T_TXN90003R;

typedef struct T_TXN90004
{
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
}T_TXN90004;

typedef struct T_TXN90004R
{
        char sPlatformId[LEN_PLATID    + 1];/*平台号*/
        char sAmount    [LEN_AMOUNT    + 1];/*平台收益*/
}T_TXN90004R;

char* parse_json(char *pInStr,char *pTxnno);

char* generate_json(char *pInStr,char *pTxnno);

T_HEAD parse_head(char *pStr);

#endif
