LD=cc -fPIC -shared
INCL=/home/tonglink/accs/include
INCL1=/home/tonglink/accs/include/incl_base
LIBP=/home/tonglink/accs/lib

up:libtl_common.so

clean:
	touch *.c

libtl_common.so:log_msg.c
	$(LD) -o $@ log_msg.c comm.c des.c -I$(INCL) -I$(INCL1) -L$(LIBP) -ltl_base

	mv libtl_common.so ../../../lib
