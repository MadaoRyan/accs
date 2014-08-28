#include "common.h"
#include "parse.h"
#include "tl_txn.h"
#include "tl_dbs.h"

/*ȫ�ֱ�������*/
char strModuleName[] ="biz_trust";
char sHead [LEN_HEAD  + 1];
char sErrNo[LEN_ERRNO + 1];

int    nMoniQueueKey;      /* �����Ϣ���м�ֵ  */
long   nMoniQueueId;       /* �����Ϣ����      */
int    nOwnPort=0;         /* ���ض˿ں�        */
int    nModuleId;          /* ģ����          */
int    nListenSocket;      /* ����socket��     */
int    nSocketId;          /* ϵͳSocket��     */

/*��������*/
int    BuildOptions(int argc, char **argv);
void   ByeExit(int n);
int    DoMainTrans();


int main(int argc,char **argv)
{
    int    nRet;

    if (BuildOptions(argc, argv) != 0)
    {
        DispHelp();
        exit(1);
    }

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGILL, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGSEGV, SIG_IGN);
    signal(SIGUSR1,ByeExit);

    InitMsgQueue();
    /*������ʼ��*/
    nSocketId = 0;
    nListenSocket = ListenRemote(nOwnPort);

    if(nListenSocket < 0)
    {
        logMsg(LOG_ERR, "Listen at [%ld] fail!\n",nOwnPort);
        exit(-1);
    }
    logMsg(LOG_TRC, "Listen at [%ld] success!\n", nOwnPort);

    while(1)
    {
        fflush(stdout);
        fflush(stderr);
        do
        {
            nSocketId = AcceptRemote(nListenSocket);
            if(nSocketId < 0) sleep(1);
        }while(nSocketId < 0);

        signal(SIGCLD,SIG_IGN);
        if( (nRet = fork()) == 0)
        {
            nRet = DoMainTrans();
            if(nRet == 0 )
            {
                logMsg(LOG_BUG,"=============================================================================");
                dbsCommit();
            }
            else
            {
                logMsg(LOG_BUG,"=============================================================================");
                dbsRollback();
            }
            dbsDisconnect();
            shutdown(nSocketId, 2);
            sleep(1);
            close(nSocketId);
            exit(0);
        }
        else if(nRet < 0)
        {
            logMsg(LOG_ERR, "fork error\n");
        }
        else
        {
            close(nSocketId);
        }
    }
    return 0;
}

/*�������*/
int BuildOptions(int argc, char **argv)
{
    int c;
    /* ���������в����ĸ��� */
    int	nArgCount = 0;
    int nRet;
    char strTmp[100];

    memset(strTmp,0,sizeof(strTmp));
    if ( (nRet = ReadCommand(argc,argv,strTmp,"p")) != 1)
	    return -1;
    else
    {
	    nArgCount++;
	    nOwnPort = atoi(strTmp);
    }

    memset(strTmp,0,sizeof(strTmp));
    if ( (nRet = ReadCommand(argc, argv, strTmp, "o")) != 1)
        return -1;
    else
    {
        nArgCount++;
        nMoniQueueKey = atoi(strTmp);
    }

    if (nArgCount != 2)
    {
        DispHelp();
        return -1;
    }

    return 0;
}


int ReadCommand(argc,argv,val,fmt)
int argc;
char **argv;
char *val;
char *fmt;
{
    char strPra[20];
    int  nPraNum;
    int  nFind=0;

    memset(strPra,0,sizeof(strPra));
    strPra[0] = '-';
    strcat(strPra,fmt);


    for(nPraNum=1;nPraNum<argc;nPraNum++)
    {
        if (strcmp(*(argv+nPraNum),strPra) ==0 )
        {
                strcpy(val,*(argv+nPraNum+1));
                nFind = 1;
                break;
        }
    }

    if (nFind ==0)
        val = NULL;
    return(nFind);
}

DispHelp()
{
    fprintf(stderr, "����ģ�� %s (V1.00) \n", strModuleName);
    fprintf(stderr, "�����в��� : -p  <���ؼ����˿ں�>\n");
    fprintf(stderr, "             -o  <���ͼ����Ϣ���к�>\n");
    return 0;
}

void ByeExit(n)
int n;
{
    if (nSocketId > 0)
        close(nSocketId);
    exit(n);
}

/*��ʼ����Ϣ����*/
int InitMsgQueue()
{
    extern int errno;
    if((nMoniQueueId = msgget((key_t)nMoniQueueKey, 0)) < 0 )
    {
        if((nMoniQueueId = msgget((key_t)nMoniQueueKey, IPC_CREAT | 0777)) < 0)
        {
            printf("errno[%d]str[%s]\n",errno,strerror(errno));
            ByeExit(0);
        }
    }
    return(0);
}

int DoMainTrans()
{

    int nRet=0;
    char sTxnno  [DLEN_TXNNO + 1];
    char *sTxnData;
    char sOutData[MSG_SIZE -5];
    char sUserName[20 + 1];
    char sPassWord[20 + 1];
    char sConnStr[40 + 1];
    char sOutStr            [MSG_SIZE];
    unsigned char strMsgData[MSG_SIZE];

    int (*txnFunc) (char *pInData,T_HEAD t_HEAD,char *pOutData,char *pErrno);
    T_HEAD t_Head;

    memset(sErrNo    ,0x00,sizeof(sErrNo    ));
    memset(strMsgData,0x00,sizeof(strMsgData));
    memset(sOutStr   ,0x00,sizeof(sOutStr   ));
    memset(sOutData  ,0x00,sizeof(sOutData  ));
    memset(sUserName ,0x00,sizeof(sUserName ));
    memset(sPassWord ,0x00,sizeof(sPassWord ));
    memset(sConnStr  ,0x00,sizeof(sConnStr  ));
    memset(&t_Head   ,0x00,sizeof(T_HEAD    ));
    
    logMsg(LOG_BUG,"=====================================================================");
    logMsg(LOG_BUG, "���յ�����, ��ʼ��������...");

    if((nRet = GetMessage(nSocketId, strMsgData, MSG_SIZE)) <= 0)
    {
        logMsg(LOG_ERR, "��������ʧ��, �������� [%d]...",nRet);
        return -1;
    }

    memcpy(sTxnno,strMsgData,5);

    logMsg(LOG_BUG,"���ձ��ĳɹ������״���[%s]",sTxnno);
    logMsg(LOG_BUG,"���ձ���֮����ͷ��[%60.60s]",strMsgData);
    logMsg(LOG_BUG,"��ʼ��������[%s]",strMsgData);

    /*��ʼ��������*/

    /*����ͷ��*/
    t_Head=parse_head(strMsgData);
    sTxnData = parse_json(strMsgData+60,sTxnno);
    if(sTxnData[0] == '\0')
    {
        logMsg(LOG_ERR,"�������ĳ���,����ֵ[%d]",nRet);
        strcat(sErrNo,"E0000004");
        RequestMsg(sTxnno,t_Head);
        return -1;
    }

    logMsg(LOG_BUG,"sTxnData[%s]",sTxnData);

    /*�������ݿ�*/
    /*��ȡ���ݿ���������*/
    strcpy(sUserName, (char*)GetPara("/home/tonglink/accs/etc/system.ini", "DBBASE", "username") );
    strcpy(sPassWord, (char*)getDBPasswd("/home/tonglink/accs/etc/system.ini") );

    sprintf(sConnStr,"%s/%s",sUserName,sPassWord);

    dbsConnect(sConnStr);

    /*��ʼҵ����*/
    switch(atoi(sTxnno))
    {
        case 60001:txnFunc=txn60001;break;
        case 60002:txnFunc=txn60002;break;
        case 60003:txnFunc=txn60003;break;
        case 60004:txnFunc=txn60004;break;
        case 60005:txnFunc=txn60005;break;
        case 60006:txnFunc=txn60006;break;
        case 60007:txnFunc=txn60007;break;
        case 60008:txnFunc=txn60008;break;
        case 90001:txnFunc=txn90001;break;
        case 90002:txnFunc=txn90002;break;
        case 90003:txnFunc=txn90003;break;
        case 90004:txnFunc=txn90004;break;
        default:
            logMsg(LOG_ERR,"error txnno ![%s]",sTxnno);
            strcat(sErrNo,"E9999999");
            RequestMsg(sTxnno,t_Head);
            dbsClose();
            return -1;
    }

    nRet = txnFunc(sTxnData,t_Head,sOutData,sErrNo);
    if(nRet <0)
    {
        if(strlen(sErrNo) == 0)
            strcat(sErrNo,"E9999999");
        logMsg(LOG_ERR,"ҵ����ʧ�ܣ�������[%s]",sErrNo);
        RequestMsg(sTxnno,t_Head);
        return -1;
    }

    memcpy(sOutStr,sTxnno,5);
    memcpy(sOutStr+5,sOutData,sizeof(sOutData));
    strcat(sErrNo,"E0000000");
    logMsg(LOG_BUG,"%s",sOutStr);
    RequestMsg(sOutStr,t_Head);

    logMsg(LOG_BUG,"ҵ����ɹ���");

    return 0;
}



/****************************************************
    RequestMsg():����Ӧ����ĵ�Ӧ����Ϣ����
    ������pMsg ���׽ṹ��
****************************************************/
int RequestMsg(char *pMsg,T_HEAD tHead)
{
    int nRet, nLen, nDataLen,iLen;
    char sTxnno  [DLEN_TXNNO + 1];
    char sLen[6 + 1];
    char sHeadBuf[LEN_HEAD +1];
    unsigned char sOutStr[MSG_SIZE];
    unsigned char *strMsgData;

    memset(sTxnno    ,0x00,sizeof(sTxnno    ));
    memset(sOutStr   ,0x00,sizeof(sOutStr   ));
    memset(sHeadBuf  ,0x00,sizeof(sHeadBuf  ));
    memset(sLen      ,0x00,sizeof(sLen      ));
    memcpy(sTxnno,pMsg,5);

    logMsg(LOG_TRC, "��ʼ�鱨��...���״���[%s]", sTxnno);

    strMsgData=generate_json(pMsg+5,sTxnno);
    if(strMsgData ==NULL && strlen(pMsg) >5)
    {
        logMsg(LOG_ERR, "�鱨�ĳ���...");
        return -1;
    }

    logMsg(LOG_BUG, "�鱨�ĳɹ�, �����峤��...");

    /*���䱨��ͷ*/
    sprintf(sOutStr,"%s%s%s%s%s%s%s2%s",
        tHead.sTxnno,tHead.sTxnDate,tHead.sTxnTime,tHead.sTxnId,sErrNo,tHead.sChannelCode,tHead.sTermCode,strMsgData);

    logMsg(LOG_BUG,"finalMsg[%s]",sOutStr);

    nDataLen=strlen(sOutStr);

    nRet = PutMessage(nSocketId, sOutStr, nDataLen);
    if(nRet < 0)
    {
        logMsg(LOG_ERR, "��������ʧ��, ��������[%d]...",nDataLen);
        return -1;
    }

    logMsg(LOG_TRC,"�ɹ������ն�, ���ݳ���[%d]...", nDataLen);
    return 0;
}
