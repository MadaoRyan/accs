#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME    : libibpsdbs.a                                                |
#| DESCRIPTIONS : ibpsis DbSvr Lib                                            |
#|----------------------------------------------------------------------------|
include $(GLBHOME)/mak/platform.mak

SRCPATH=$(GLBHOME)/src/dbsvr

PRGOBJS = \
	$(SRCPATH)/top_dbs.o \
	$(SRCPATH)/top_dbs_arg.o \
	$(SRCPATH)/top_dbs_svr.o \

PRGTARG = top_dbs
PRGLIBS = -ltoputil $(UXLIBS)
PRGDEFS =

debug all: debugdynamic
release: releasedynamic

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@$(MAKE) -f $(GLBHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" MKHOME="$(GLBHOME)"
