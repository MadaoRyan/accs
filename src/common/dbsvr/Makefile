LD=cc -fPIC -shared
LIB=-ltl_base -ldl -lm -lc -liconv
INCL=/home/tonglink/accs/include/incl_base
INCL1=/home/tonglink/accs/include
LIBP=/home/tonglink/accs/lib

up: libtl_dbs.so

clean:
	touch *.c

libtl_dbs.so:top_dbs.c top_dbs_arg.c top_dbs_svr.c
	$(LD) -o $@ top_dbs.c top_dbs_arg.c top_dbs_svr.c -L$(LIBP) $(LIB) -I$(INCL) -I$(INCL1)

	mv libtl_dbs.so ../../../lib

