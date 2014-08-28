#include "common.h"
static unsigned char dirty_buf[MMAX];

/* ---------------------------------------------------------------- 
 *  函数原型 :  int  ListenRemote
 *  说    明 :  等待一个网络连接
 *  输入参数 :   int  nOwnPort :端口号
 *  输出参数 ： 无
 *  返 回 值 :  sd :侦听 socket 标识号  -1 :失败
 * ----------------------------------------------------------------
 */     
int ListenRemote(nOwnPort)
int nOwnPort;
{
    static  struct  sockaddr_in  SerAddr; 
    static  int     FirstCall = TRUE;
    int  sd;
    int  opt =1;
    struct linger stLinger;

    if (FirstCall) 
    {
        memset(&SerAddr, 0, sizeof(SerAddr));
        SerAddr.sin_family = AF_INET;
        SerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        FirstCall = FALSE;
    }

     /* 设置端口 */
    SerAddr.sin_port = htons(nOwnPort);

    /* 取得 socket */ 
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) 
    {
        logMsg(LOG_TRC,"TCPT : [ %05d ] sd = %d\n", getpid(), sd);
        return -1;
    }

    /* 置 socket 重新使用  */
    opt = 1;
    if (setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,(char *) &opt,sizeof(opt))<0) 
    {
        logMsg(LOG_TRC,"TCPT : [ %05d ] : setsockopt error\n", getpid());
        close(sd);
        return -1;
    }

    if (bind(sd, (struct sockaddr *) &SerAddr, sizeof(SerAddr)) < 0) 
    {
        logMsg(LOG_TRC,"TCPT : [ %05d ] Cannot bind server at %hd\n", getpid(), nOwnPort);
        close(sd);
        return -1;
    }

    listen(sd, 5);
    return(sd);
}


/* ---------------------------------------------------------------- 
 *  函数原型 :  int  AcceptRemote
 *  说    明 :  接受一个网络连接
 *  输入参数 :   int  nListenID :侦听socket 标识号
 *  输出参数 ： 无
 *  返 回 值 :  sd :系统socket 标识号  -1 :失败
 * ----------------------------------------------------------------
 */         
int AcceptRemote(nListenID)
int nListenID;
{
    struct  sockaddr_in CliAddr;
    int  sd;
    unsigned long  clilen;
    struct linger stLinger;

    clilen = sizeof(CliAddr);
    sd = accept(nListenID, (struct sockaddr *) &CliAddr, &clilen);
    if (sd < 0) 
    {
        perror("accept:");
        return -1;
    }
    stLinger.l_onoff = 1;
    stLinger.l_linger = 0;
    if (setsockopt(sd,SOL_SOCKET,SO_LINGER,(char *) &stLinger,sizeof(stLinger))<0) 
    {
        logMsg(LOG_TRC,"TCPT : [ %05d ] : setsockopt error\n", getpid());
        close(sd);
        return -1;
    }
    return sd;
}


int GetMessage(int sd, char *ptr, int max)
{
    int   nread;
    unsigned char  cs_nob[6+1]; /* No. of bytes in character */
    int s_nob;          /* No. of bytes in short */
    int   nbyte;            /* No. of bytes to read from stream */
    int   ret = -1;         /* Final return value */
    int i;
    char temp[50];
    
    if ((nread=readn(sd, (char *)cs_nob, 6)) != 6)
        return -1;
    cs_nob[6] = 0;
    s_nob = atoi((char *)cs_nob);
    if (s_nob > max)
        nbyte = max;
    else
        nbyte = s_nob;

    /*errlog(__FILE__,__LINE__,"GetMBOC: nbyte=%d", nbyte);*/
    /* Read remaining bytes */
    nread = readn(sd, ptr, nbyte);
    ret = nread;

    
    /*errlog(__FILE__,__LINE__,"GetMBOC: ret=%d", ret);*/
    
    if (nread != nbyte)
        return -1;
    
    /* Check if there is remaining bytes to clear */
    /* Return value independent on result of this section */
    nread = s_nob - max;     /*Could be negative */
    while (nread > 0) 
    {
        if (nread >= MMAX) 
            nbyte = MMAX;
        else
            nbyte = nread; 
        
        nbyte = readn(sd, dirty_buf, nbyte);
        if (nbyte > 0)
            nread -= nbyte;
        else
            break;
    }
    
    return (ret);
}


int PutMessage(int sd, char *ptr, int nbytes)
{
    int nwritten;
    unsigned char  cs_nob[6+1];
    char temp[20];
    
    sprintf((char *)cs_nob, "%06d", nbytes);
    if ((nwritten=writen(sd, (char *)cs_nob, 6)) != 6)
        return -1;
    /* Write out application data */
    nwritten = writen(sd, ptr, nbytes);
    if (nwritten != nbytes)
        return -1;
    
    return nwritten;
}


/* Internal buffer for whatever purpose */

/*
* readn() -- Function to read n bytes from network connection
*   para   --
*   return --
*/
int readn(int sd, char *ptr, int nbyte)
{
    int nleft, nread,nCount;
    nleft = nbyte;
    
    nCount=0;
    while (nleft > 0) 
    {
        /*i++;*/
        nread = read(sd, ptr, nleft);
        if (nread < 0) 
        {
            /*errlog(__FILE__,__LINE__,"readn()=%d, i=%d", nread, i);*/
            if (errno == EINTR)
                continue;
            return (nread);
        }
        else if (nread == 0) /* EOF */
        {
            /*errlog(__FILE__,__LINE__,"readn()=%d, i=%d", nread, i);*/
            break;
        }
        nleft -= nread;
        ptr += nread;
        nCount+=nread;
        /*fprintf(stdout,"readn==>已接收的数据长度:[%d],还剩:[%d]\n",nCount,nleft);*/ 
    }
    /* Return number of bytes read == nbyte - nleft */
    /*errlog(__FILE__,__LINE__, "readn(), nbyte=%d, nread=%d, nleft=%d, i=%d", nbyte, nread, nleft, i);*/
    return (nbyte - nleft);
}

/*
 *   writen() -- Function to write n bytes from network connection
 *   para   --
 *   return --
 */
int writen(int sd, char *ptr, int nbyte)
{
    int nleft, nwritten;
    
    nleft = nbyte;
    while (nleft > 0) 
    {
        nwritten = write(sd, ptr, nleft);
        if (nwritten <= 0) 
        {
            perror("write");
            return (nwritten);
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    
    /* Return number of bytes written == nbyte - nleft */
    return (nbyte - nleft);
}



/* ------------------------------------------------------------------------- 
 *  函数原型 :  char *GetPara( )
 *  说    明 :  读取windows标准配置文件格式的参数
 *  输入参数 :  char *strFileName   --- 配置文件名称
 *              char *strSegName    --- 段落名
 *              char *strParaName   --- 参数名
 *  输出参数 :  无 
 *  返回  值 :  参数值(字符串)
 * -------------------------------------------------------------------------
 */ 
char* GetPara(strFileName, strSegName, strParaName)
char *strFileName;
char *strSegName;
char *strParaName;
{
    FILE *fp;
    int find=0, flag=0;
    char *p, *p1, buf[260], pp[200], vv[200];
    static char val[200];
    
    memset(val, 0, sizeof(val));
    if ((fp = fopen(strFileName, "rt")) == NULL) 
    {
        fprintf(stderr, "Can not open file: %s\n", strFileName);
        return val;
    }
    
    while (!feof(fp)) 
    {
        memset(buf, 0, sizeof(buf));
        memset(pp, 0, sizeof(pp));
        memset(vv, 0, sizeof(vv));
        if (fgets(buf, sizeof(buf), fp) == NULL)
            break;
    
        p = buf;
        while (*p!=0) 
        {
            if (*p == ' ') 
            { 
                p++;
                continue;
            } 
            else 
            {
                if (*p == '#' || *p == ';') flag = 1;
                break;
            }
        }
        
        if (flag) 
        { 
            flag = 0; 
            continue;    
        }
        
        if ((p1=strchr(p, '=')) != NULL) *p1 = ' ';
        else if (find) 
        {
            if (*p == '[') 
                break; 
            else 
                continue;
        }
        
        sscanf(p, "%s %s", pp, vv);
        if (!find) 
        {
            strcpy(vv, "[");
            strcat(vv, strSegName);
            strcat(vv, "]");
            if (!StrCmp(vv, pp)) 
                find = 1;
        } 
        else 
        {
            if (!StrCmp(pp, strParaName)) 
            {
                if ((p1=strchr(vv, ';')) != NULL)   *p1 = 0;
                if ((p1=strchr(vv, '#')) != NULL)   *p1 = 0;
                strcpy(val, vv);
                break;
            }
        }
    } /*end while (!feof(fp)) */
    fclose(fp);
    return val;
}



char* getDBPasswd(strParaFile)
char* strParaFile;
{
    char strPwd[17], strEncPwd[9], strEncKey[9], temp[17];
    char *ptr, dst[9];
    int i;
    
/*    strcpy( (char *)username.arr, GetPara(strParaFile, "DBBASE", "username") );*/
    memset( strPwd, 0, sizeof(strPwd) );
    strcpy( strPwd, GetPara(strParaFile, "DBBASE", "passwd") );

    memset(strEncPwd, 0, sizeof(strEncPwd));
    DSPToHEX(strPwd, strEncPwd, 8);
    memset(strEncKey, 0, sizeof(strEncKey));
    DSPToHEX(SLKEY, strEncKey, 8);
    Do_DES(strEncPwd, dst, strEncKey, 'D');
    HEXToDSP(dst, temp, 8);
    if ((ptr = strchr(temp, 'F')) != NULL)
        *ptr = '\0';
    else
        temp[16] = 0;

    return temp;
}


/* -------------------------------------------------------------------- 
 *  函数原型 :  int StrCmp()
 *  说    明 :  比较两个字符串的大小（不区分大小写）
 *  输入参数 :  char *strFirst  --- 第一个字符串
 *              char *strSecond --- 第二个字符串
 *  输出参数 :  无
 *  返 回 值 :  0   --- 相同
 *              1   --- strFirst>strSecond
 *              -1  --- strFirst<strSecond
 * --------------------------------------------------------------------
 */ 
int StrCmp(strFirst,strSecond)
char *strFirst;
char *strSecond;
{
    char *p1, *p2;
    unsigned char c1, c2;
    
    p1 = strFirst; p2 = strSecond;
    while (*p1 && *p2) {
        c1 = toupper(*p1);
        c2 = toupper(*p2);
        if (c1 > c2) return 1; 
        else if (c1 < c2) return -1;
        p1++; p2++;
    }
    if (*p1 && !*p2) return 1;
    if (!*p1 && *p2) return -1;
    
    return 0;
}
