LD=cc -fPIC -shared
INCL=/home/tonglink/accs/include/incl_base
LIB=-ldl -lm -lc -liconv

up: libtl_base.so

clean:
	touch *.c

libtl_base.so: ./os/os_lock.c ./os/os_socket.c ./os/os_stream.c ./os/os_shm.c \
./os/os_sem.c ./os/os_dll.c ./os/os_process.c ./os/os_fs.c ./os/os_memory.c \
./os/os_cpu.c ./os/os_disk.c ./os/os_proc.c ./os/os_net.c ./os/os_env.c ./os/os_cli.c \
./os/os_mmap.c ./os/os_ppool.c ./util/desalgo/deskey.c ./util/desalgo/desport.c \
./util/desalgo/dessp.c ./util/util_alg.c ./util/util_csv.c ./util/util_conv.c \
./util/util_ctn.c ./util/util_des.c ./util/util_dtm.c ./util/util_enc.c ./util/util_expr.c \
./util/util_fil.c ./util/util_hash.c ./util/util_ini.c ./util/util_str.c \
./log4c/log_info.c ./configer/configer.c ./configer/configer_local.c

	$(LD) -o $@  ./os/os_lock.c ./os/os_socket.c ./os/os_stream.c ./os/os_shm.c \
./os/os_sem.c ./os/os_dll.c ./os/os_process.c ./os/os_fs.c ./os/os_memory.c \
./os/os_cpu.c ./os/os_disk.c ./os/os_proc.c ./os/os_net.c ./os/os_env.c ./os/os_cli.c \
./os/os_mmap.c ./os/os_ppool.c ./util/desalgo/deskey.c ./util/desalgo/desport.c \
./util/desalgo/dessp.c ./util/util_alg.c ./util/util_csv.c ./util/util_conv.c \
./util/util_ctn.c ./util/util_des.c ./util/util_dtm.c ./util/util_enc.c ./util/util_expr.c \
./util/util_fil.c ./util/util_hash.c ./util/util_ini.c ./util/util_str.c \
./log4c/log_info.c ./configer/configer.c ./configer/configer_local.c $(LIB) -I$(INCL)

	mv libtl_base.so ../../../lib
