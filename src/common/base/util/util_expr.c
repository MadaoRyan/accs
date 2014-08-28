/*
 *
 *
 * Calculation Expression
 *
 *
 * FileName: util_expr.c
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
/*--------------------------- Include files -----------------------------*/
#include	"util_expr.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"util_str.h"

/*--------------------------- Macro define ------------------------------*/
#define	_TINY_BUF_LEN	32
#define	_SMALL_BUF_LEN	256
#define	_LARGE_BUF_LEN	1024
#define	_HUGE_BUF_LEN	10240

#define	setnull(x)		memset(&(x), 0, sizeof(x))
#define	setpnull(x)		memset(x, 0, sizeof(*(x)))
#define	max(x,y)		((x) > (y) ? (x) : (y))
#define	min(x,y)		((x) < (y) ? (x) : (y))
#define	arrayof(a)		(sizeof(a) / sizeof(*a))

#define	_				snprintf

#define	arg(pt,i)		((pt)->tArgs + (i))
#define	top(pt)			((pt)->tArgs + (pt)->iCnt-1)

#define	push(s,e)		do {                                \
	if ((s)->iCnt >= arrayof((s)->tArgs)) {                 \
		_(f_sErrStr, sizeof(f_sErrStr), "no more buffer");  \
		return ERR_EXPR_BUFFER;                             \
	}                                                       \
	(s)->tArgs[(s)->iCnt++] = *(e);                         \
} while (0)

#define	pop(s)			((s)->iCnt > 0 ? (s)->tArgs + --((s)->iCnt) : NULL)

/*---------------------------- Type define ------------------------------*/
typedef enum {
	eBegin, eWord, eQuote, eEnd,
} E_EXPR_STATUS;

typedef enum {
	eNone,

	eLeft, eRight,

	eMul, eDiv, eMod, eAdd, sSub,

	eEqual, eNotEqual, eGreat, eLess, eGreatEqual, eLessEqual,
	eEqualS, eNotEqualS, eGreatS, eLessS, eGreatEqualS, eLessEqualS,

	eNot, eAnd, eOr,

	eString = 100,
	eBool,
	eInteger,
} E_EXPR_TYPE;

typedef struct {
	E_EXPR_TYPE	eType;
	char		*pValue;
} T_EXPR_WORD;

typedef struct {
	char		*sExpr;
	int			iCnt;
	T_EXPR_WORD	tArgs[EXPR_MAX_ARGS];
	int			iFree;
} T_EXPR;

typedef int (*F_EXPR_UNARY )(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptRet);
typedef int (*F_EXPR_BINARY)(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                             T_EXPR_WORD *ptRet);

/*---------------------- Local function declaration ---------------------*/
int exprWrapper(T_EXPR *ptExpr, const char *sExpr, T_EXPR_WORD *ptRet);

T_EXPR *exprNew (T_EXPR *ptExpr, const char *sExpr);
int     exprFree(T_EXPR *ptExpr);

int exprSplit(T_EXPR *ptExpr);
int exprCalc (T_EXPR *ptExpr, T_EXPR_WORD *ptResult);

static E_EXPR_TYPE exprType(char *sName);

static int exprPush(T_EXPR_WORD *ptWord, T_EXPR *ptExpr, T_EXPR *ptTmp);
static int exprOperand(T_EXPR_WORD *ptWord);

static int   exprToBool(T_EXPR_WORD *ptWord);
static int   exprToInt (T_EXPR_WORD *ptWord);
static char *exprToStr (T_EXPR_WORD *ptWord, char *sBuf, int iSize);

static int exprNot        (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptRet);
static int exprAnd        (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprOr         (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprEqual      (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprNotEqual   (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprGreat      (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprLess       (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprGreatEqual (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprLessEqual  (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprEqualS     (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprNotEqualS  (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprGreatS     (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprLessS      (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprGreatEqualS(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprLessEqualS (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprAdd        (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprSub        (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprMul        (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprDiv        (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);
static int exprMod        (T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB,
                           T_EXPR_WORD *ptRet);

/*-------------------------  Global variable ----------------------------*/
static struct {
	const char	*sName;
	int			iPriority;
	int			iOperand;
	void		*pFunc;
} f_tOpers[] = {
	{NULL, 0, 0, NULL},

	{"(", 0, 0, NULL}, {")", 0, 0, NULL},

	{"*", 60, 2, exprMul}, {"/", 60, 2, exprDiv}, {"%", 60, 2, exprMod},
	{"+", 60, 2, exprAdd}, {"-", 60, 2, exprSub},

	{"==", 40, 2, exprEqual      }, {"!=", 40, 2, exprNotEqual  },
	{">" , 40, 2, exprGreat      }, {"<" , 40, 2, exprLess      },
	{">=", 40, 2, exprGreatEqual }, {"<=", 40, 2, exprLessEqual },
	{"eq", 40, 2, exprEqualS     }, {"ne", 40, 2, exprNotEqualS },
	{"gt", 40, 2, exprGreatS     }, {"lt", 40, 2, exprLessS     },
	{"ge", 40, 2, exprGreatEqualS}, {"le", 40, 2, exprLessEqualS},

	{"!", 30, 1, exprNot}, {"&&", 20, 2, exprAnd}, {"||", 10, 2, exprOr},
};

static const T_EXPR_WORD f_tTrue  = {eBool, (char *)1};
static const T_EXPR_WORD f_tFalse = {eBool, (char *)0};

static char f_sErrStr[_LARGE_BUF_LEN];

/*-------------------------  Global functions ---------------------------*/
int
exprLogic(const char *sExpr)
{
	T_EXPR		tExpr;
	T_EXPR_WORD	tRet;
	int			iRet;

	iRet = exprWrapper(&tExpr, sExpr, &tRet);
	if (iRet < 0) {
		return iRet;
	}

	iRet = exprToBool(&tRet);

	exprFree(&tExpr);
	return iRet;
}

int
exprStr(const char *sExpr, char *sBuf, int iSize)
{
	T_EXPR		tExpr;
	T_EXPR_WORD	tRet;
	char		*pRet;
	int			iRet;

	iRet = exprWrapper(&tExpr, sExpr, &tRet);
	if (iRet < 0) {
		return iRet;
	}

	pRet = exprToStr(&tRet, sBuf, iSize);
	if (pRet != sBuf) strMem2Str(sBuf, iSize, pRet, -1);

	exprFree(&tExpr);
	return 0;
}

int
exprInt(const char *sExpr, int *piRet)
{
	T_EXPR		tExpr;
	T_EXPR_WORD	tRet;
	int			iRet;

	iRet = exprWrapper(&tExpr, sExpr, &tRet);
	if (iRet < 0) {
		return iRet;
	}

	*piRet = exprToInt(&tRet);

	exprFree(&tExpr);
	return 0;
}

int
exprWrapper(T_EXPR *ptExpr, const char *sExpr, T_EXPR_WORD *ptRet)
{
	int	iRet;

	if (!sExpr || *sExpr == '\0') {
		return ERR_EXPR_ARG;
	}

	if (!exprNew(ptExpr, sExpr)) {
		return ERR_EXPR_MEMORY;
	}

	iRet = exprSplit(ptExpr);
	if (iRet < 0) {
		exprFree(ptExpr);
		return iRet;
	}

	iRet = exprCalc(ptExpr, ptRet);
	if (iRet < 0) {
		exprFree(ptExpr);
		return iRet;
	}

	return iRet;
}

const char *
exprErrStr(void)
{
	return f_sErrStr;
}

T_EXPR *
exprNew(T_EXPR *ptExpr, const char *sExpr)
{
	if (!ptExpr) {
		ptExpr = malloc(sizeof(*ptExpr));
		if (!ptExpr) {
			_(f_sErrStr, sizeof(f_sErrStr), "cannot allocate memory");
			return NULL;
		}
		setpnull(ptExpr);
		ptExpr->iFree = 1;

	} else {
		setpnull(ptExpr);
		ptExpr->iFree = 0;
	}

	ptExpr->sExpr = strdup(sExpr);
	if (!ptExpr->sExpr) {
		_(f_sErrStr, sizeof(f_sErrStr), "cannot allocate memory");
		return NULL;
	}

	return ptExpr;
}

int
exprFree(T_EXPR *ptExpr)
{
	if (ptExpr) {
		free(ptExpr->sExpr);
		ptExpr->sExpr = NULL;
		if (ptExpr->iFree) free(ptExpr);
	}
	return 0;
}

int
exprSplit(T_EXPR *ptExpr)
{
	char			*pCur = ptExpr->sExpr;
	E_EXPR_STATUS	eStatus = eBegin;
	T_EXPR_WORD		tWord;
	T_EXPR			tTmp;
	int				iRet;

	setnull(tTmp);
	setnull(tWord);
	push(&tTmp, &tWord);

	while (*pCur) {
		switch (eStatus) {
			case eBegin: {
				if (*pCur == ' '  || *pCur == '\t' ||
					*pCur == '\r' || *pCur == '\n') {
					/* noop */

				} else if (*pCur == '"') {
					tWord.eType = eString;
					tWord.pValue = pCur + 1;
					eStatus = eQuote;

				} else {
					tWord.pValue = pCur;
					eStatus = eWord;
				}
				break;
			}
			case eWord: {
				if (*pCur == ' '  || *pCur == '\t' ||
					*pCur == '\r' || *pCur == '\n') {
					*pCur = '\0';
					tWord.eType = exprType(tWord.pValue);
					iRet = exprPush(&tWord, ptExpr, &tTmp);
					if (iRet < 0) return iRet;
					eStatus = eBegin;

				} else if (*pCur == '\"') {
					_(f_sErrStr, sizeof(f_sErrStr),
					  "unexpect quote '\"' at column %td",
					  pCur - ptExpr->sExpr);
					return ERR_EXPR_QUOTE;
				}
				break;
			}
			case eQuote: {
				if (*pCur == '"') {
					*pCur = '\0';
					eStatus = eEnd;
				}
				break;
			}
			case eEnd: {
				if (*pCur == ' '  || *pCur == '\t' ||
					*pCur == '\r' || *pCur == '\n') {
					iRet = exprPush(&tWord, ptExpr, &tTmp);
					if (iRet < 0) return iRet;
					eStatus = eBegin;

				} else if (*pCur == '"') {
					memmove(tWord.pValue + 1, tWord.pValue,
					        pCur - tWord.pValue - 1);
					tWord.pValue++;
					eStatus = eQuote;

				} else {
					_(f_sErrStr, sizeof(f_sErrStr),
					  "expect space or quote '\"' at column %td",
					  pCur - ptExpr->sExpr);
					return ERR_EXPR_QUOTE;
				}
				break;
			}
		}
		pCur++;
	}

	switch (eStatus) {
		case eWord: {
			tWord.eType = exprType(tWord.pValue);
			/* fall through */
		}
		case eEnd: {
			iRet = exprPush(&tWord, ptExpr, &tTmp);
			if (iRet < 0) return iRet;
			/* fall through */
		}
		case eBegin: {
			break;
		}
		case eQuote: {
			_(f_sErrStr, sizeof(f_sErrStr),
			  "expect quote '\"' at column %td", pCur - ptExpr->sExpr);
			return ERR_EXPR_QUOTE;
		}
	}

	setnull(tWord);
	iRet = exprPush(&tWord, ptExpr, &tTmp);
	if (iRet < 0) return iRet;

	return 0;
}

int
exprCalc(T_EXPR *ptExpr, T_EXPR_WORD *ptRet)
{
	F_EXPR_UNARY	fUnary;
	F_EXPR_BINARY	fBinary;
	T_EXPR_WORD		tRet, *ptWordA, *ptWordB;
	T_EXPR			tTmp;
	int				iRet, i;

	setnull(tTmp);
	for (i = 0; i < ptExpr->iCnt; i++) {
		switch (exprOperand(arg(ptExpr, i))) {
			case 1: {
				ptWordA = pop(&tTmp);
				if (!ptWordA) {
					_(f_sErrStr, sizeof(f_sErrStr),
					  "operator '%s' nead a operand",
					  arg(ptExpr, i)->pValue);
					return ERR_EXPR_OPERAND;
				}

				fUnary = f_tOpers[arg(ptExpr, i)->eType].pFunc;

				iRet = fUnary(ptWordA, &tRet);
				if (iRet < 0) {
					_(f_sErrStr, sizeof(f_sErrStr),
					  "operator '%s' calculate error",
					  arg(ptExpr, i)->pValue);
					return ERR_EXPR_CALC;
				}

				push(&tTmp, &tRet);

				break;
			}
			case 2: {
				ptWordB = pop(&tTmp);
				if (!ptWordB) {
					_(f_sErrStr, sizeof(f_sErrStr),
					  "operator '%s' nead two operands",
					  arg(ptExpr, i)->pValue);
					return ERR_EXPR_OPERAND;
				}

				ptWordA = pop(&tTmp);
				if (!ptWordA) {
					_(f_sErrStr, sizeof(f_sErrStr),
					  "operator '%s' nead two operands",
					  arg(ptExpr, i)->pValue);
					return ERR_EXPR_OPERAND;
				}

				fBinary = f_tOpers[arg(ptExpr, i)->eType].pFunc;

				iRet = fBinary(ptWordA, ptWordB, &tRet);
				if (iRet < 0) {
					_(f_sErrStr, sizeof(f_sErrStr),
					  "operator '%s' calculate error",
					  arg(ptExpr, i)->pValue);
					return ERR_EXPR_CALC;
				}

				push(&tTmp, &tRet);

				break;
			}
			default: {
				push(&tTmp, arg(ptExpr, i));
				break;
			}
		}
	}

	if (tTmp.iCnt != 1) {
		_(f_sErrStr, sizeof(f_sErrStr), "syntax error");
		return ERR_EXPR_SYNTAX;
	}

	*ptRet = *top(&tTmp);

	return 0;
}

/*-------------------------  Local functions ----------------------------*/
static E_EXPR_TYPE
exprType(char *sName)
{
	int	i;

	for (i = 1; i < arrayof(f_tOpers); i++) {
		if (strcmp(sName, f_tOpers[i].sName) == 0) {
			return i;
		}
	}

	return eString;
}

static int
exprOperand(T_EXPR_WORD *ptWord)
{
	if (ptWord->eType < arrayof(f_tOpers))
		return f_tOpers[ptWord->eType].iOperand;

	return 0;
}

static int
exprPush(T_EXPR_WORD *ptWord, T_EXPR *ptExpr, T_EXPR *ptTmp)
{
	switch (ptWord->eType) {
		case eString: {
			push(ptExpr, ptWord);
			break;
		}
		case eLeft: {
			push(ptTmp, ptWord);
			break;
		}
		case eRight: {
			while (top(ptTmp)->eType != eLeft &&
			       top(ptTmp)->eType != eNone) {
				push(ptExpr, pop(ptTmp));
			}
			if (pop(ptTmp)->eType != eLeft) {
				_(f_sErrStr, sizeof(f_sErrStr), "expect left bracket '('");
				return ERR_EXPR_BRACKET;
			}
			break;
		}
		case eNone: {
			while (top(ptTmp)->eType != eNone) {
				ptWord = pop(ptTmp);
				if (ptWord->eType == eLeft) {
					_(f_sErrStr, sizeof(f_sErrStr), "expect left bracket '('");
					return ERR_EXPR_BRACKET;
				}
				push(ptExpr, ptWord);
			}
			break;
		}
		default: { /* Operator */
			while (f_tOpers[ptWord->eType].iPriority <=
			       f_tOpers[top(ptTmp)->eType].iPriority) {
				push(ptExpr, pop(ptTmp));
			}
			push(ptTmp, ptWord);
			break;
		}
	}

	return 0;
}

static int
exprToBool(T_EXPR_WORD *ptWord)
{
	if (ptWord->eType == eString) {
		return ptWord->pValue && ptWord->pValue[0] != '\0' ?
			(atoi(ptWord->pValue) ? 1 : 0) : 0;
	} else if (ptWord->eType == eInteger) {
		return ptWord->pValue ? 1 : 0;
	} else {
		return (int)ptWord->pValue;
	}
}

static int
exprToInt(T_EXPR_WORD *ptWord)
{
	if (ptWord->eType == eString) {
		return ptWord->pValue && ptWord->pValue[0] != '\0' ?
		       atoi(ptWord->pValue) : 0;
	} else if (ptWord->eType == eBool) {
		return ptWord->pValue ? 1 : 0;
	} else {
		return (int)ptWord->pValue;
	}
}

static char *
exprToStr(T_EXPR_WORD *ptWord, char *sBuf, int iSize)
{
	if (ptWord->eType == eInteger) {
		snprintf(sBuf, iSize, "%d", (int)ptWord->pValue);
		return sBuf;

	} else if (ptWord->eType == eBool) {
		snprintf(sBuf, iSize, "%s", ptWord->pValue ? "true" : "false");
		return sBuf;

	} else {
		return ptWord->pValue;
	}
}

/*-------------------------  Operator functions ----------------------------*/
static int
exprNot(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptRet)
{
	if (!exprToBool(ptWordA)) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprAnd(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	if (exprToBool(ptWordA) && exprToBool(ptWordB)) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprOr(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	if (exprToBool(ptWordA) || exprToBool(ptWordB)) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprEqual(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	if (exprToInt(ptWordA) == exprToInt(ptWordB)) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprNotEqual(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	if (exprToInt(ptWordA) != exprToInt(ptWordB)) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprGreat(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	if (exprToInt(ptWordA) > exprToInt(ptWordB)) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprLess(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	if (exprToInt(ptWordA) < exprToInt(ptWordB)) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprGreatEqual(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	if (exprToInt(ptWordA) >= exprToInt(ptWordB)) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprLessEqual(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	if (exprToInt(ptWordA) <= exprToInt(ptWordB)) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprEqualS(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	char	sBufA[_TINY_BUF_LEN], sBufB[_TINY_BUF_LEN];

	if (strcmp(exprToStr(ptWordA, sBufA, sizeof(sBufA)),
		       exprToStr(ptWordB, sBufB, sizeof(sBufB))) == 0) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprNotEqualS(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	char	sBufA[_TINY_BUF_LEN], sBufB[_TINY_BUF_LEN];

	if (strcmp(exprToStr(ptWordA, sBufA, sizeof(sBufA)),
		       exprToStr(ptWordB, sBufB, sizeof(sBufB))) != 0) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprGreatS(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	char	sBufA[_TINY_BUF_LEN], sBufB[_TINY_BUF_LEN];

	if (strcmp(exprToStr(ptWordA, sBufA, sizeof(sBufA)),
		       exprToStr(ptWordB, sBufB, sizeof(sBufB))) > 0) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprLessS(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	char	sBufA[_TINY_BUF_LEN], sBufB[_TINY_BUF_LEN];

	if (strcmp(exprToStr(ptWordA, sBufA, sizeof(sBufA)),
		       exprToStr(ptWordB, sBufB, sizeof(sBufB))) < 0) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprGreatEqualS(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	char	sBufA[_TINY_BUF_LEN], sBufB[_TINY_BUF_LEN];

	if (strcmp(exprToStr(ptWordA, sBufA, sizeof(sBufA)),
		       exprToStr(ptWordB, sBufB, sizeof(sBufB))) >= 0) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprLessEqualS(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	char	sBufA[_TINY_BUF_LEN], sBufB[_TINY_BUF_LEN];

	if (strcmp(exprToStr(ptWordA, sBufA, sizeof(sBufA)),
		       exprToStr(ptWordB, sBufB, sizeof(sBufB))) <= 0) {
		*ptRet = f_tTrue;
	} else {
		*ptRet = f_tFalse;
	}

	return 0;
}

static int
exprAdd(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	ptRet->eType = eInteger;
	ptRet->pValue = (char *)(exprToInt(ptWordA) + exprToInt(ptWordB));

	return 0;
}

static int
exprSub(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	ptRet->eType = eInteger;
	ptRet->pValue = (char *)(exprToInt(ptWordA) - exprToInt(ptWordB));

	return 0;
}

static int
exprMul(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
	ptRet->eType = eInteger;
	ptRet->pValue = (char *)(exprToInt(ptWordA) * exprToInt(ptWordB));

	return 0;
}

static int
exprDiv(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
    int iIntB = 0;

	ptRet->eType = eInteger;
    iIntB = exprToInt(ptWordB);
    if (0 != iIntB) {
        ptRet->pValue = (char *)(exprToInt(ptWordA) / iIntB);
    } else {
        ptRet->pValue = (char *)0;
    }

	return 0;
}

static int
exprMod(T_EXPR_WORD *ptWordA, T_EXPR_WORD *ptWordB, T_EXPR_WORD *ptRet)
{
    int iIntB = 0;

	ptRet->eType = eInteger;
    iIntB = exprToInt(ptWordB);
    if (0 != iIntB) {
        ptRet->pValue = (char *)(exprToInt(ptWordA) % iIntB);
    } else {
        ptRet->pValue = (char *)0;
    }

	return 0;
}

/*-----------------------------  End ------------------------------------*/
