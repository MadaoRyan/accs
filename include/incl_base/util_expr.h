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
 * @desc: ���ʽ����
 *
 * @param sExpr: �߼����ʽ
 *
 * @return int: ret=1 ��; ret=0 ��; ret<0 ʧ��
 *
 * @comment:
 *   support operator:
 *      ()                 ����
 *      * / % + -          ���������
 *      == != >  <  >= <=  ���ֱȽ�����
 *      eq ne gt lt ge le  �ַ����Ƚ�����
 *      !                  �߼���
 *      &&                 �߼���
 *      ||                 �߼���
 *   Wrong: "5">=4&&!("123"==123)
 *   Right: "5" >= 4 && ! ( "123" == 123 )
 *
 */
int exprLogic(const char *sExpr);

/**
 * @function: exprStr
 *
 * @desc: ���ʽ����
 *
 * @param sExpr: ���ʽ
 * @param sBuf: ������
 * @param iSize: �����С
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int exprStr(const char *sExpr, char *sBuf, int iSize);

/**
 * @function: exprInt
 *
 * @desc: ���ʽ����
 *
 * @param sExpr: ���ʽ
 * @param piRet: ������
 *
 * @return int: ret=0 �ɹ�; ret<0 ʧ��
 *
 */
int exprInt(const char *sExpr, int *piRet);

/**
 * @function: exprErrStr
 *
 * @desc: ��ñ��ʽ���������Ϣ
 *
 * @return const char *: ������Ϣ
 *
 */
const char *exprErrStr(void);

#ifdef __cplusplus
}
#endif

#endif /*_UTIL_EXPR_H_20110603111053_*/
/*-----------------------------  End ------------------------------------*/
