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
        char sAccId     [LEN_ACCID     + 1];/*�ⲿϵͳ�ͻ�ע���*/
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sAmount    [LEN_AMOUNT    + 1];/*��ֵ���*/
        char sFee       [LEN_AMOUNT    + 1];/*��ѯ�ѽ��*/
        char sIdType    [LEN_ID_TYPE   + 1];/*֤������*/
        char sPersonId  [LEN_PERSON_ID + 1];/*֤����*/
        char sAccName   [LEN_NAME      + 1];/*�ͻ�����*/
}T_TXN60001;

typedef struct T_TXN60001R
{
        char sAmount    [LEN_AMOUNT    + 1];/*��ֵ����*/
}T_TXN60001R;

typedef struct T_TXN60002
{
        char sAccId     [LEN_ACCID     + 1];/*�ⲿϵͳ�ͻ�ע���*/
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sInstCode  [LEN_PLATID    + 1];/*������*/
        char sAmount    [LEN_AMOUNT    + 1];/*��ֵ���*/
}T_TXN60002;

typedef struct T_TXN60002R
{
        char sAmount    [LEN_AMOUNT    + 1];/*��ֵ����*/
}T_TXN60002R;

typedef struct T_TXN60003
{
        char sAccId     [LEN_ACCID     + 1];/*�ⲿϵͳ�ͻ�ע���*/
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sAmount    [LEN_AMOUNT    + 1];/*���ֽ��*/
        char sFee       [LEN_AMOUNT    + 1];/*��ѯ�ѽ��*/
}T_TXN60003;

typedef struct T_TXN60003R
{
        char sAmount    [LEN_AMOUNT    + 1];/*���ֺ���*/
}T_TXN60003R;

typedef struct T_TXN60004
{
        char sPayerAccId[LEN_ACCID     + 1];/*�������ⲿϵͳע���*/
        char sPayeeAccId[LEN_ACCID     + 1];/*������ⲿϵͳע���*/
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sAmount    [LEN_AMOUNT    + 1];/*ת�˽��*/
        char sFee       [LEN_AMOUNT    + 1];/*��ѯ�ѽ��*/
        char sAccName   [LEN_NAME      + 1];/*���������*/
}T_TXN60004;

typedef struct T_TXN60005
{
        char sPayeeAccId[LEN_ACCID     + 1];/*������ⲿϵͳע���*/
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sInstCode  [LEN_PLATID    + 1];/*������*/
        char sTransType [LEN_CHG_TYPE  + 1];
        char sAmount    [LEN_AMOUNT    + 1];/*ת�˽��*/
        char sFee       [LEN_AMOUNT    + 1];/*��ѯ�ѽ��*/
        char sAccName   [LEN_NAME      + 1];/*���������*/
}T_TXN60005;

typedef struct T_TXN60006
{
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sType      [LEN_TYPE      + 1];/*��������*/
        char sAmount    [LEN_AMOUNT    + 1];/*������*/
}T_TXN60006;

typedef struct T_TXN60006R
{
        char sAmount    [LEN_AMOUNT    + 1];/*�������*/
}T_TXN60006R;

typedef struct T_TXN60007
{
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sInstCode  [LEN_PLATID    + 1];/*������*/
        char sChgType   [LEN_CHG_TYPE  + 1];/*ά������*/
        char sInstName  [LEN_PLAT_NAME + 1];/*������*/
        char sRemarks   [LEN_REMARKS   + 1];/*��ע*/
}T_TXN60007;

typedef struct T_TXN60008
{
        char sPlatformId  [LEN_PLATID    + 1];/*ƽ̨��*/
        char sChgType     [LEN_CHG_TYPE  + 1];/*ά������*/
        char sPlatformName[LEN_PLAT_NAME + 1];/*ƽ̨��*/
        char sRemarks     [LEN_REMARKS   + 1];/*��ע*/
}T_TXN60008;

typedef struct T_TXN90001
{
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sAccId     [LEN_ACCID     + 1];/*�����˺�*/
}T_TXN90001;

typedef struct T_TXN90001R
{
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sAccId     [LEN_ACCID     + 1];/*�����˺�*/
        char sBalance   [LEN_AMOUNT    + 1];/*�˻����*/
}T_TXN90001R;

typedef struct T_TXN90002
{
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
}T_TXN90002;

typedef struct T_TXN90002R
{
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sBalance   [LEN_AMOUNT    + 1];/*�˻����*/
}T_TXN90002R;

typedef struct T_TXN90003
{
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sStartDate [LEN_DATE      + 1];/*��ʼ����*/
        char sEndDate   [LEN_DATE      + 1];/*��������*/
        char sPageSize  [LEN_NUM       + 1];/*ҳ��С*/
        char sPageNum   [LEN_NUM       + 1];/*ҳ��*/
}T_TXN90003;

typedef struct T_90003_LIST
{
        char sTransDate [LEN_DATE      + 1];/*ί������*/
        char sAmount    [LEN_AMOUNT    + 1];/*�����*/
        char sAccId     [LEN_ACCID     + 1];/*�����ƽ̨�˺�*/
        char sFee       [LEN_AMOUNT    + 1];/*��ѯ��*/
        char sTxnNo     [LEN_TXNID     + 1];/*������ˮ��*/
}T_90003_LIST;

typedef struct T_TXN90003R
{
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sTotal     [LEN_NUM       + 1];/*�ܼ�¼��*/
        char sNumber    [8             + 1];/*ѭ����¼��*/
        T_90003_LIST    L0[99];/*��ϸ�б�*/
}T_TXN90003R;

typedef struct T_TXN90004
{
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
}T_TXN90004;

typedef struct T_TXN90004R
{
        char sPlatformId[LEN_PLATID    + 1];/*ƽ̨��*/
        char sAmount    [LEN_AMOUNT    + 1];/*ƽ̨����*/
}T_TXN90004R;

char* parse_json(char *pInStr,char *pTxnno);

char* generate_json(char *pInStr,char *pTxnno);

T_HEAD parse_head(char *pStr);

#endif
