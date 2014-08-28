#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2010-2020 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME    :                                                             |
#| DESCRIPTIONS :                                                             |
#|----------------------------------------------------------------------------|
include $(GLBHOME)/mak/platform.mak

SRCPATH = $(GLBHOME)/src/common/regex

PRGOBJS = \
	$(SRCPATH)/tre/regcomp.o \
	$(SRCPATH)/tre/regerror.o \
	$(SRCPATH)/tre/regexec.o \
	$(SRCPATH)/tre/tre-ast.o \
	$(SRCPATH)/tre/tre-compile.o \
	$(SRCPATH)/tre/tre-match-backtrack.o \
	$(SRCPATH)/tre/tre-match-parallel.o \
	$(SRCPATH)/tre/tre-mem.o \
	$(SRCPATH)/tre/tre-parse.o \
	$(SRCPATH)/tre/tre-stack.o \
	$(SRCPATH)/topregex.o \

PRGTARG = topregex
PRGLIBS = $(UXLIBS)

debug all: debugdynamic
release: releasedynamic

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@$(MAKE) -f $(GLBHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" MKHOME="$(GLBHOME)"
