#ifndef _DESALGO_H_20100105103527_
#define _DESALGO_H_20100105103527_

typedef unsigned long DES_KS[16][2];	/* Single-key DES key schedule */
typedef unsigned long DES3_KS[48][2];	/* Triple-DES key schedule */

#define	DES_ENCRYPT	0
#define	DES_DECRYPT	1

#ifdef __cplusplus
extern "C" {
#endif

/* In deskey.c */
void __deskey(DES_KS,unsigned char *,int);
void __des3key(DES3_KS,unsigned char *,int);

/* In desport.c */
void __des(DES_KS,unsigned char *);
void __des3(DES3_KS,unsigned char *);

#ifdef __cplusplus
}
#endif

#endif /*_DESALGO_H_20100105103527_*/
