#include "common.h"
#include "util_dtm.h"

#define LOGFILE_SIZE 1024*1024*10

/* -----------------------------------------------------------------
 *  函数原型 :  char *GetstrCurDate(cFmtCh)
 *  说    明 :  取当前系统日期
 *  输入参数 :  char cFmtCh (分隔年月日的分隔符,默认为'')
 *  输出参数 :  无
 *  返 回 值 :  当前日期(字符型)
 * ----------------------------------------------------------------
 */  
char* GetstrCurDate(cFmtch)
char cFmtch;
{
    struct tm *current_time;
    time_t t1;
    static    char  str[11];
	char      *strFMT = "%04d%c%02d%c%02d";
	
	
    memset( str, 0, sizeof( str));

    time( &t1) ;
    current_time = localtime ( &t1) ;

    current_time -> tm_year = current_time -> tm_year + 1900 ;
    
    if (cFmtch == 0x00){
    	strFMT = "%04d%02d%02d";
    	sprintf( str , strFMT,
        current_time -> tm_year,
        current_time -> tm_mon + 1,
        current_time -> tm_mday);
	}        
    else
    sprintf( str , strFMT,
        current_time -> tm_year, cFmtch,
        current_time -> tm_mon + 1, cFmtch,
        current_time -> tm_mday);

    return str ;
}


/*写日志函数*/
void logMsg(int nLevel,char *psFile,int nLine, const char *psFunction, const char *psModuleName,char *strMsg, ...)
{
    va_list aplist;
    FILE *fp;
    char *ptr;
    int fd;
    char strRunMsgFile[100] = "";
    char strTime[21+ 1];
    char *sLevel;
    char sCmd[256];
    struct stat buf;

    time_t tSystime = time(NULL);
    struct tm *ptTm  = localtime(&tSystime);
    strftime(strTime, 19, "%Y-%m-%d %H:%M:%S", ptTm);

    /*日志级别参数检查，若日志级别设置错误，则设为4-TRACE*/
    if(nLevel>4 || nLevel < 1)
    	nLevel = 4;
    
    /*sprintf(strRunMsgFile, "%s/trace/runmsg.%s", getenv("HOME"), GetstrCurDate(0) + 4);*/

    sprintf(strRunMsgFile,"/home/tonglink/accs/log/%s.log",psModuleName);
    stat(strRunMsgFile, &buf);
    if(buf.st_size > LOGFILE_SIZE)
    {
        memset(sCmd,0x00,sizeof(sCmd));
        sprintf(sCmd,"mv %s %s.%s",strRunMsgFile,strRunMsgFile,GetstrCurDate());
        system(sCmd);
    }

    va_start(aplist, strMsg);
    ptr = getenv("LOG_LEVEL");
    if(ptr == NULL)
    {
    	/*若未设置日志级别LOG_LEVEL的环境变量，则设置默认级别为:4-TRACE*/
    	ptr = "4";
    }
    if (atoi(ptr) >= nLevel)
    {
    	switch(nLevel){
    		case 1:sLevel="ERROR";break;
    		case 2:sLevel="WARN";break;
    		case 3:sLevel="DEBUG";break;
    		case 4:sLevel="TRACE";break;
    		default:sLevel="TRACE";
    	}

    	strftime(strTime, 21, "%Y-%m-%d %H:%M:%S", ptTm);
        fp = fopen(strRunMsgFile, "a");
        fd = fileno(fp);
        lockf(fd, F_LOCK, 0L);
        fprintf(fp, "[%s][%s][%d][%s:%d][%s]", strTime,sLevel,getpid(),psFile,nLine,psFunction);
        va_start(aplist, strMsg);
        vfprintf(fp, strMsg, aplist);
	fprintf(fp,"\n");
        lockf(fd, F_ULOCK, 0L);
        fclose(fp);
    }
    va_end(aplist);
}
