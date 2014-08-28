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
	$(GLBHOME)/src/common/userlog/userlog.o \

PRGTARG = userlog
PRGLIBS = -ltoputil -ldl $(UXLIBS)

debug all: debugdynamic
release: releasedynamic

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@$(MAKE) -f $(GLBHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" MKHOME="$(GLBHOME)"

