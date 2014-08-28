LD=cc
INCL=/home/tonglink/accs/include
INCL1=/home/tonglink/accs/include/incl_base
LIBP=/home/tonglink/accs/lib

up:biz_trust

clean:
	touch *.c

biz_trust:biz_trust.c txn60001.c txn60002.c txn60003.c txn60004.c txn60005.c txn60006.c txn60007.c txn60008.c txn90001.c txn90002.c txn90003.c txn90004.c
	$(LD) -o $@ biz_trust.c txn60001.c txn60002.c txn60003.c txn60004.c txn60005.c txn60006.c txn60007.c txn60008.c txn90001.c txn90002.c txn90003.c txn90004.c common_txn.c -I$(INCL) -I$(INCL1) -L$(LIBP) -ltl_base -ltl_common -ltl_cvt -ltl_dbs

	mv  biz_trust ../../bin
