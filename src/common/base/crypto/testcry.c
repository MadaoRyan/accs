#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"topcry.h"

int
main(int argc, char *argv[])
{
	int	iRet;

	iRet = cryHexSelfTest(1);
	if (iRet < 0) {
		printf("cryHexSelfTest error on [%d]\n", iRet);
		exit(EXIT_FAILURE);
	}

	iRet = cryBase64SelfTest(1);
	if (iRet < 0) {
		printf("cryBase64SelfTest error on [%d]\n", iRet);
		exit(EXIT_FAILURE);
	}

	iRet = cryDesSelfTest(1);
	if (iRet < 0) {
		printf("cryDesSelfTest error on [%d]\n", iRet);
		exit(EXIT_FAILURE);
	}

	iRet = cryAesSelfTest(1);
	if (iRet < 0) {
		printf("cryAesSelfTest error on [%d]\n", iRet);
		exit(EXIT_FAILURE);
	}

	iRet = cryMdxSelfTest(1);
	if (iRet < 0) {
		printf("cryMdxSelfTest error on [%d]\n", iRet);
		exit(EXIT_FAILURE);
	}

	iRet = cryShaxSelfTest(1);
	if (iRet < 0) {
		printf("cryShaxSelfTest error on [%d]\n", iRet);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
