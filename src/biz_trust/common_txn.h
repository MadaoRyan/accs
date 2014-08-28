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
�����˺�ӳ�估���˻�
��Σ�
pPlatformId ƽ̨��
pAccId      �ⲿƽ̨ע���
pAccountCode ���ص��ڲ��˺�
pAccCode     ���ص��ڲ����˺�

return      0 �ɹ�
          <>0 ʧ��
*/
int CreateAcc(char *pPlatformId,char *pAccId,char *pIdType,char *pPerSonId,char *pAccName,char *pAccountCode,char *pAccCode);

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
int CheckAcc(char *pPlatformId,char *pAccId,char *pAccountCode,char *pAccCode);

/*
���¸����˻���
��Σ�
pAccountCode  �˻�����
pAccCode      ���˻�����
pType         �������ͣ�1-��ֵ��2-ת�룬3-ת����4-����
pAmount       ���
pFee          ������

return        0 �ɹ�
            <>0 ʧ��
*/
int UpdateAcc(char *pAccountCode,char *pAccCode,char *pType,char *pAmount,char *pFee);

/*
����ƽ̨�˻���
��Σ�
pPlatformId  ƽ̨��
pType        �������ͣ�1-��ֵ��2-���֣�3����������
pAmount      ���
pFee         ������
*/
int UpdatePlatAcc(char *pPlatformId,char *pType,char *pAmount,char *pFee);

/*
���»����˻���
��Σ�
pPlatformId ƽ̨��
pInstCode   ������
pType       �������ͣ�1-��ֵ��2-ת�룬3-ת����4-����
pAmount     ���
pFee        ������

return      0 �ɹ�
          <>0 ʧ��
*/
int UpdateInstAcc(char *pPlatformId,char *pInstCode,char *pType,char *pAmount,char *pFee);

/*
��¼����������ˮ��
��Σ�
pId         ������ˮ��
t_FLOW      ������Ϣ

return      0 �ɹ�
          <>0 ʧ��
*/
int InsertPerAccFlow(T_ACCFLOW t_FLOW);

/*
��¼����������ˮ��
��Σ�
pId         ������ˮ��
t_FLOW      ������Ϣ

return      0 �ɹ�
          <>0 ʧ��
*/
int InsertOrgAccFlow(T_ACCFLOW t_FLOW);
