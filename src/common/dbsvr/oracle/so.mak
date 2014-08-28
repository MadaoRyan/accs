#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME    : libibpsdbs.a                                                |
#| DESCRIPTIONS : ibpsis DbSvr Lib                                            |
#|----------------------------------------------------------------------------|
include $(GLBHOME)/mak/platform.mak
include $(GLBHOME)/mak/database_ora.mak

SRCPATH=$(GLBHOME)/src/dbsvr/oracle

PRGOBJS = \
	$(SRCPATH)/dbs_oracle.o \
	$(SRCPATH)/dbs_0.o \
	$(SRCPATH)/dbs_1.o \
	$(SRCPATH)/dbs_2.o \
	$(SRCPATH)/dbs_3.o \
	$(SRCPATH)/dbs_4.o \
	$(SRCPATH)/dbs_5.o \
	$(SRCPATH)/dbs_6.o \
	$(SRCPATH)/dbs_7.o \
	$(SRCPATH)/dbs_8.o \
	$(SRCPATH)/dbs_9.o \
	$(SRCPATH)/dbs_10.o \
	$(SRCPATH)/dbs_11.o \
	$(SRCPATH)/dbs_12.o \
	$(SRCPATH)/dbs_13.o \
	$(SRCPATH)/dbs_14.o \
	$(SRCPATH)/dbs_15.o \
	$(SRCPATH)/dbs_16.o \
	$(SRCPATH)/dbs_17.o \
	$(SRCPATH)/dbs_18.o \
	$(SRCPATH)/dbs_19.o \
	$(SRCPATH)/dbs_20.o \
	$(SRCPATH)/dbs_oraoper.o \
	$(SRCPATH)/dbs_page.o \

PRGTARG = dbs_oracle
PRGLIBS = -ltoputil $(DBLIBS) $(UXLIBS)
PRGDEFS =

debug all: debugdynamic
release: releasedynamic

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@$(MAKE) -f $(GLBHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" MKHOME="$(GLBHOME)" DBT="$(DBT)"
