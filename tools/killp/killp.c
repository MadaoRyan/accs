#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define        BUF_SIZE        200
#define        ITEM_SIZE        40

main(int argc, char * argv[] )
{
    FILE *fp;
    char buffer[BUF_SIZE];
    char pid[ITEM_SIZE], temp[ITEM_SIZE], proc[ITEM_SIZE];
    int i;
    if (argc<2)
    {
	fprintf(stderr, "Usage %s  processname\n",argv[0]);
        exit(0);
    }
    if ((fp = popen("ps -e", "r")) != NULL)
    {
	fgets(buffer, BUF_SIZE, fp);
	while (fgets(buffer, BUF_SIZE, fp))
	{
	    sscanf(buffer, "%s%s%s%s", pid, temp,temp, proc);
	    for (i=0; i<argc-1; i++) 
		if (!memcmp(proc, argv[i+1], strlen(proc))) 
		{
		    kill(atol(pid),9);
		}
	}
	pclose(fp);
    }
}
