#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	<unistd.h>

#define	setnull(x)	memset(&(x), 0, sizeof(x))

#define	CLOG_RED	"40;31;1"
#define	CLOG_YELLOW	"40;33;1"
#define	CLOG_BLUE	"40;34;1"
#define	CLOG_WHITE	"40;37;1"

static char	*f_pLast = NULL;

int color(char *sOut, char *sIn, char *sStr, char *sColor);

int
main(int argc, char *argv[])
{
	char	sLine[4096] = {0, };
	char	sLine2[sizeof(sLine) + 14] = {0, };
	int		iTty = isatty(fileno(stdout));

	while (fgets(sLine, sizeof(sLine), stdin)) {
		if (iTty) {
			     if (color(sLine2, sLine, "[ERROR]"  , CLOG_RED   ));
			else if (color(sLine2, sLine, "[WARN]"  , CLOG_YELLOW));
			else if (color(sLine2, sLine, "[DEBUG]"  , CLOG_WHITE ));
			else if (color(sLine2, sLine, "[TRACE]"  , NULL       ));
			else     color(sLine2, sLine, NULL   , NULL       ) ;

			fputs(sLine2, stdout);
		} else {
			fputs(sLine, stdout);
		}

		fflush(stdout);
	}

	exit(EXIT_SUCCESS);
}

int
color(char *sOut, char *sIn, char *sStr, char *sColor)
{
	char	*pCur;

	if (sStr) {
		pCur = strstr(sIn, sStr);
		if (!pCur) {
			return 0;
		}

	} else {
		sColor = f_pLast;
	}

	if (sColor) {
		sprintf(sOut, "\033[%sm%s\033[0m", sColor, sIn);
	} else {
		strcpy(sOut, sIn);
	}

	f_pLast = sColor;
	return 1;
}
