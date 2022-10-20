#---------------------------------------------------------------------------
# Complier environment variable
# NOTE:
#   - If the default toolchain path is not /opt/rwi/y31-ext, please modify
#   - the following environment variable.
#---------------------------------------------------------------------------
CC = /opt/rwi/y31-ext/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc
SYS_ROOT = /opt/rwi/y31-ext/sysroots/armv7at2hf-neon-poky-linux-gnueabi
ECHO = echo
GOAL1 := shmread
GOAL2 := shmwrite
O_PATH = ./tmp
#---------------------------------------------------------------------------
# Include section
#---------------------------------------------------------------------------
INCLUDE_DIR	+= 

#---------------------------------------------------------------------------
# Source section
#---------------------------------------------------------------------------
GOAL1_C_SRC       +=    shmread.c
GOAL2_C_SRC       +=    shmwrite.c

#---------------------------------------------------------------------------
# Library section
#---------------------------------------------------------------------------
LDLIBS     += -lpthread

#---------------------------------------------------------------------------
# Complier options
# NOTE:
#   - as fwupdate_client is not a Legato standard lib, you need to adb push
#   - the libfwupdate_client.so into /home in sa515m device, if you want to
#   - push this lib to another, please modify the following -rpath,replace
#   - the :/home to other file path, such as :/tmp
#---------------------------------------------------------------------------
CFLAGS     += -Wall -g -fPIC -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -O2 -fno-omit-frame-pointer
CFLAGS     += --sysroot=$(SYS_ROOT)
LDFLAGS    += -Wl,--export-dynamic

#---------------------------------------------------------------------------
# Implicit rules
#---------------------------------------------------------------------------
$(O_PATH)/%.o : %.c
	mkdir -p $(dir $(O_PATH)/$*)
	@$(CC) -c $(CFLAGS) -o $(O_PATH)/$*.o $< $(INCLUDE_DIR)

.cpp.o:
	@$(CC) -MM -MT $@ -o $*.d
	@$(CC) -c -o $*.o $<

#---------------------------------------------------------------------------
# Explicit ruls
#---------------------------------------------------------------------------
GOAL1_OBJ_FILES	= $(GOAL1_C_SRC:%.c=$(O_PATH)/%.o)
GOAL2_OBJ_FILES	= $(GOAL2_C_SRC:%.c=$(O_PATH)/%.o)

all: $(GOAL1)

$(GOAL1): $(GOAL1_OBJ_FILES)
	@$(ECHO) "------------------"
	@$(CC) -o $(GOAL1) $(GOAL1_OBJ_FILES) $(LDLIBS) $(LDFLAGS) $(CFLAGS)
	@$(ECHO) "*** $(GOAL1) is built success! ***"

all: $(GOAL2)

$(GOAL2): $(GOAL2_OBJ_FILES)
	@$(ECHO) "------------------"
	@$(CC) -o $(GOAL2) $(GOAL2_OBJ_FILES) $(LDLIBS) $(LDFLAGS) $(CFLAGS)
	@$(ECHO) "*** $(GOAL2) is built success! ***"

.PHONY: clean
clean:
	rm -f $(GOAL1) $(GOAL2)
	rm -f $(GOAL1_OBJ_FILES) $(GOAL2_OBJ_FILES)

clean_obj:
	rm -f $(GOAL1_OBJ_FILES) $(GOAL2_OBJ_FILES)


