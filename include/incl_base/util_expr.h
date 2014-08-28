/*
 *
 *
 * Calculation Expression
 *
 *
 * FileName: util_expr.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_EXPR_H_20110603111053_
#define _UTIL_EXPR_H_20110603111053_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/
#define	EXPR_MAX_ARGS		64

#define	ERR_EXPR_BASE		0
#define	ERR_EXPR_MEMORY		(ERR_EXPR_BASE - 1)
#define	ERR_EXPR_BUFFER		(ERR_EXPR_BASE - 2)
#define	ERR_EXPR_QUOTE		(ERR_EXPR_BASE - 3)
#define	ERR_EXPR_BRACKET	(ERR_EXPR_BASE - 4)
#define	ERR_EXPR_TYPE		(ERR_EXPR_BASE - 5)
#define	ERR_EXPR_OPERAND	(ERR_EXPR_BASE - 6)
#define	ERR_EXPR_CALC		(ERR_EXPR_BASE - 7)
#define	ERR_EXPR_SYNTAX		(ERR_EXPR_BASE - 8)
#define	ERR_EXPR_ARG		(ERR_EXPR_BASE - 9)

/*---------------------------- Type define ------------------------------*/

/*---------------------- Global function declaration --------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function: exprLogic
 *
 * @desc: 表达式计算
 *
 * @param sExpr: 逻辑表达式
 *
 * @return int: ret=1 真; ret=0 假; ret<0 失败
 *
 * @comment:
 *   support operator:
 *      ()                 括号
 *      * / % + -          整数运算符
 *      == != >  <  >= <=  数字比较运算
 *      eq ne gt lt ge le  字符串比较运算
 *      !                  逻辑非
 *      &&                 逻辑与
 *      ||                 逻辑或
 *   Wrong: "5">=4&&!("123"==123)
 *   Right: "5" >= 4 && ! ( "123" == 123 )
 *
 */
int exprLogic(const char *sExpr);

/**
 * @function: exprStr
 *
 * @desc: 表达式计算
 *
 * @param sExpr: 表达式
 * @param sBuf: 运算结果
 * @param iSize: 缓存大小
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int exprStr(const char *sExpr, char *sBuf, int iSize);

/**
 * @function: exprInt
 *
 * @desc: 表达式计算
 *
 * @param sExpr: 表达式
 * @param piRet: 运算结果
 *
 * @return int: ret=0 成功; ret<0 失败
 *
 */
int exprInt(const char *sExpr, int *piRet);

/**
 * @function: exprErrStr
 *
 * @desc: 获得表达式计算错误信息
 *
 * @return const char *: 错误信息
 *
 */
const char *exprErrStr(void);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_EXPR_H_20110603111053_*/
/*-----------------------------  End ------------------------------------*/
