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
include $(GLBHOME)/mak/database.mak

all: debug

debug release clean:
	@$(MAKE) -f userlog.mak $@
	@$(MAKE) -f log_plug_userlog.mak $@
