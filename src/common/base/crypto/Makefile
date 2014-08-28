#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2010-2020 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME    :                                                             |
#| DESCRIPTIONS :                                                             |
#|----------------------------------------------------------------------------|
include $(GLBHOME)/mak/platform.mak

all: debug

debug release clean: 
	@$(MAKE) -f topcry.mak $@

test:
	@$(MAKE) -f testcry.mak
	@$(GLBHOME)/bin/testcry
	@$(MAKE) -f testcry.mak clean
