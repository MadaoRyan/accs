#include "common.h"
#include "parse.h"
#include "tl_txn.h"
#include "tl_dbs.h"

/*全局变量声明*/
char strModuleName[] ="biz_trust";
char sHead [LEN_HEAD  + 1];
char sErrNo[LEN_ERRNO + 1];

int    nMoniQueueKey;      /* 监控消息队列键值  */
long   nMoniQueueId;       /* 监控消息队列      */
int    nOwnPort=0;         /* 本地端口号        */
int    nModuleId;          /* 模块编号          */
int    nListenSocket;      /* 侦听socket号     */
int    nSocketId;          /* 系统Socket号     */

/*函数声明*/
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
    /*变量初始化*/
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

/*参数检查*/
int BuildOptions(int argc, char **argv)
{
    int c;
    /* 跟踪命令行参数的个数 */
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
    fprintf(stderr, "处理模块 %s (V1.00) \n", strModuleName);
    fprintf(stderr, "命令行参数 : -p  <本地监听端口号>\n");
    fprintf(stderr, "             -o  <发送监控消息队列号>\n");
    return 0;
}

void ByeExit(n)
int n;
{
    if (nSocketId > 0)
        close(nSocketId);
    exit(n);
}

/*初始化消息队列*/
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
    logMsg(LOG_BUG, "接收到请求, 开始接收数据...");

    if((nRet = GetMessage(nSocketId, strMsgData, MSG_SIZE)) <= 0)
    {
        logMsg(LOG_ERR, "接收数据失败, 函数返回 [%d]...",nRet);
        return -1;
    }

    memcpy(sTxnno,strMsgData,5);

    logMsg(LOG_BUG,"接收报文成功，交易代码[%s]",sTxnno);
    logMsg(LOG_BUG,"接收报文之报文头：[%60.60s]",strMsgData);
    logMsg(LOG_BUG,"开始解析报文[%s]",strMsgData);

    /*开始解析报文*/

    /*解析头块*/
    t_Head=parse_head(strMsgData);
    sTxnData = parse_json(strMsgData+60,sTxnno);
    if(sTxnData[0] == '\0')
    {
        logMsg(LOG_ERR,"解析报文出错,返回值[%d]",nRet);
        strcat(sErrNo,"E0000004");
        RequestMsg(sTxnno,t_Head);
        return -1;
    }

    logMsg(LOG_BUG,"sTxnData[%s]",sTxnData);

    /*连接数据库*/
    /*获取数据库连接配置*/
    strcpy(sUserName, (char*)GetPara("/home/tonglink/accs/etc/system.ini", "DBBASE", "username") );
    strcpy(sPassWord, (char*)getDBPasswd("/home/tonglink/accs/etc/system.ini") );

    sprintf(sConnStr,"%s/%s",sUserName,sPassWord);

    dbsConnect(sConnStr);

    /*开始业务处理*/
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
        logMsg(LOG_ERR,"业务处理失败！错误码[%s]",sErrNo);
        RequestMsg(sTxnno,t_Head);
        return -1;
    }

    memcpy(sOutStr,sTxnno,5);
    memcpy(sOutStr+5,sOutData,sizeof(sOutData));
    strcat(sErrNo,"E0000000");
    logMsg(LOG_BUG,"%s",sOutStr);
    RequestMsg(sOutStr,t_Head);

    logMsg(LOG_BUG,"业务处理成功！");

    return 0;
}



/****************************************************
    RequestMsg():返回应答包文到应答消息队列
    参数：pMsg 交易结构体
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

    logMsg(LOG_TRC, "开始组报文...交易代码[%s]", sTxnno);

    strMsgData=generate_json(pMsg+5,sTxnno);
    if(strMsgData ==NULL && strlen(pMsg) >5)
    {
        logMsg(LOG_ERR, "组报文出错...");
        return -1;
    }

    logMsg(LOG_BUG, "组报文成功, 报文体长度...");

    /*补充报文头*/
    sprintf(sOutStr,"%s%s%s%s%s%s%s2%s",
        tHead.sTxnno,tHead.sTxnDate,tHead.sTxnTime,tHead.sTxnId,sErrNo,tHead.sChannelCode,tHead.sTermCode,strMsgData);

    logMsg(LOG_BUG,"finalMsg[%s]",sOutStr);

    nDataLen=strlen(sOutStr);

    nRet = PutMessage(nSocketId, sOutStr, nDataLen);
    if(nRet < 0)
    {
        logMsg(LOG_ERR, "发送数据失败, 函数返回[%d]...",nDataLen);
        return -1;
    }

    logMsg(LOG_TRC,"成功返回终端, 数据长度[%d]...", nDataLen);
    return 0;
}
