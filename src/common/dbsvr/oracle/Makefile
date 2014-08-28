LD=cc -fPIC -shared
LIB=-lclntsh -lsql10 -lm -lcurses
INCL1=/oracle/product/10g/precomp/public
INCL2=/home/tonglink/accs/include/incl_base
INCL3=/home/tonglink/accs/include
LIBP=/oracle/product/10g/lib

up: libdbs_oracle.so

clean:
	touch *.c

libdbs_oracle.so:dbs_oracle.c dbs_0.c dbs_1.c dbs_2.c dbs_3.c \
dbs_4.c dbs_5.c dbs_6.c dbs_7.c dbs_8.c dbs_9.c dbs_10.c \
dbs_11.c dbs_12.c dbs_13.c dbs_14.c dbs_15.c dbs_16.c \
dbs_17.c dbs_18.c dbs_19.c dbs_20.c dbs_oraoper.c dbs_page.c

	$(LD) -o $@ dbs_oracle.c dbs_0.c dbs_1.c dbs_2.c dbs_3.c \
dbs_4.c dbs_5.c dbs_6.c dbs_7.c dbs_8.c dbs_9.c dbs_10.c \
dbs_11.c dbs_12.c dbs_13.c dbs_14.c dbs_15.c dbs_16.c \
dbs_17.c dbs_18.c dbs_19.c dbs_20.c dbs_oraoper.c dbs_page.c $(LIB) -I$(INCL1) -I$(INCL3) -I$(INCL2) -I$(LIBP) -L$(LIBP)

	mv libdbs_oracle.so ../../../../lib

