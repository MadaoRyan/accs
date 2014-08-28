#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2010-2020 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME    :                                                             |
#| DESCRIPTIONS :                                                             |
#|----------------------------------------------------------------------------|
                                                                               

include $(GLBHOME)/mak/platform.mak
include $(GLBHOME)/mak/tuxedo.mak

PRGOBJS = \
	$(GLBHOME)/src/common/os/os_lock.o \
	$(GLBHOME)/src/common/os/os_socket.o \
	$(GLBHOME)/src/common/os/os_stream.o \
	$(GLBHOME)/src/common/os/os_shm.o \
	$(GLBHOME)/src/common/os/os_sem.o \
	$(GLBHOME)/src/common/os/os_dll.o \
	$(GLBHOME)/src/common/os/os_process.o \
	$(GLBHOME)/src/common/os/os_fs.o \
	$(GLBHOME)/src/common/os/os_memory.o \
	$(GLBHOME)/src/common/os/os_cpu.o \
	$(GLBHOME)/src/common/os/os_disk.o \
	$(GLBHOME)/src/common/os/os_proc.o \
	$(GLBHOME)/src/common/os/os_net.o \
	$(GLBHOME)/src/common/os/os_env.o \
	$(GLBHOME)/src/common/os/os_cli.o \
	$(GLBHOME)/src/common/os/os_mmap.o \
	$(GLBHOME)/src/common/os/os_ppool.o \
	$(GLBHOME)/src/common/util/desalgo/deskey.o \
	$(GLBHOME)/src/common/util/desalgo/desport.o \
	$(GLBHOME)/src/common/util/desalgo/dessp.o \
	$(GLBHOME)/src/common/util/util_alg.o \
	$(GLBHOME)/src/common/util/util_csv.o \
	$(GLBHOME)/src/common/util/util_conv.o \
	$(GLBHOME)/src/common/util/util_ctn.o \
	$(GLBHOME)/src/common/util/util_des.o \
	$(GLBHOME)/src/common/util/util_dtm.o \
	$(GLBHOME)/src/common/util/util_enc.o \
	$(GLBHOME)/src/common/util/util_expr.o \
	$(GLBHOME)/src/common/util/util_fil.o \
	$(GLBHOME)/src/common/util/util_hash.o \
	$(GLBHOME)/src/common/util/util_ini.o \
	$(GLBHOME)/src/common/util/util_str.o \
	$(GLBHOME)/src/common/log4c/log_info.o \
	$(GLBHOME)/src/common/configer/configer.o \
	$(GLBHOME)/src/common/configer/configer_local.o \

PRGTARG = toputil
PRGLIBS = -ldl $(ICONV) $(PERFSTAT) $(UXLIBS)
PRGDEFS = -D_DB_ONDB_

debug all: debugdynamic
release: releasedynamic

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@$(MAKE) -f $(GLBHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" MKHOME="$(GLBHOME)"

