# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /home/rihuber/reconos/core/ecos/ecos-patched/ecos/packages
export PREFIX := /home/rihuber/reconos/projects/sort_demo/sw/ecos_build/install
export COMMAND_PREFIX := mb-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := io/serial/current
OBJECT_PREFIX := io_serial
CFLAGS := -msoft-float -mcpu=v4.00.a -Wall -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fdata-sections -fno-exceptions -I. -I$(HW_DESIGN)/microblaze_0/include -I$(RECONOS)/core/ecos/include
LDFLAGS := -msoft-float -mcpu=v4.00.a -g -nostdlib -Wl,--gc-sections -Wl,-static
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := 

build: headers libextras.a.stamp

LIBRARY := libextras.a
COMPILE := src/common/serial.c src/common/tty.c src/common/haldiag.c
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

headers: mlt_headers $(PREFIX)/include/cyg/io/ttyio.h $(PREFIX)/include/cyg/io/serial.h $(PREFIX)/include/cyg/io/serialio.h

$(PREFIX)/include/cyg/io/ttyio.h: $(REPOSITORY)/$(PACKAGE)/include/ttyio.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/io/serial.h: $(REPOSITORY)/$(PACKAGE)/include/serial.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/io/serialio.h: $(REPOSITORY)/$(PACKAGE)/include/serialio.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

