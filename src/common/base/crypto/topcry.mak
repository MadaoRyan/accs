#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2010-2020 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME    :                                                             |
#| DESCRIPTIONS :                                                             |
#|----------------------------------------------------------------------------|
include $(GLBHOME)/mak/platform.mak

SRCPATH = $(GLBHOME)/src/common/crypto

PRGOBJS = \
	$(SRCPATH)/polarssl/library/aes.o \
	$(SRCPATH)/polarssl/library/base64.o \
	$(SRCPATH)/polarssl/library/des.o \
	$(SRCPATH)/polarssl/library/md5.o \
	$(SRCPATH)/polarssl/library/sha1.o \
	$(SRCPATH)/polarssl/library/sha2.o \
	$(SRCPATH)/polarssl/library/sha4.o \
	$(SRCPATH)/topcry.o \
	$(SRCPATH)/topcry_wrap.o \

PRGTARG = topcry
PRGLIBS = $(UXLIBS)
PRGDEFS = -I$(SRCPATH)/polarssl/include

debug all: debugdynamic
release: releasedynamic

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@$(MAKE) -f $(GLBHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" MKHOME="$(GLBHOME)"
