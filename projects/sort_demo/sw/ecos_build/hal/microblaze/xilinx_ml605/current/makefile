# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /home/rihuber/reconos/core/ecos/ecos-patched/ecos/packages
export PREFIX := /home/rihuber/reconos/projects/sort_demo/sw/ecos_build/install
export COMMAND_PREFIX := mb-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := hal/microblaze/xilinx_ml605/current
OBJECT_PREFIX := hal_microblaze_xilinx_ml605
CFLAGS := -msoft-float -mcpu=v4.00.a -Wall -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fdata-sections -fno-exceptions -I. -I$(HW_DESIGN)/microblaze_0/include -I$(RECONOS)/core/ecos/include
LDFLAGS := -msoft-float -mcpu=v4.00.a -g -nostdlib -Wl,--gc-sections -Wl,-static
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := 

build: headers

headers: mlt_headers $(PREFIX)/include/cyg/hal/icap.h

$(PREFIX)/include/cyg/hal/icap.h: $(REPOSITORY)/$(PACKAGE)/include/icap.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

