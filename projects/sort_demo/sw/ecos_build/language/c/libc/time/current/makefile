# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /home/rihuber/reconos/core/ecos/ecos-patched/ecos/packages
export PREFIX := /home/rihuber/reconos/projects/sort_demo/sw/ecos_build/install
export COMMAND_PREFIX := mb-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := language/c/libc/time/current
OBJECT_PREFIX := language_c_libc_time
CFLAGS := -msoft-float -mcpu=v4.00.a -Wall -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fdata-sections -fno-exceptions -I. -I$(HW_DESIGN)/microblaze_0/include -I$(RECONOS)/core/ecos/include -Wno-format
LDFLAGS := -msoft-float -mcpu=v4.00.a -g -nostdlib -Wl,--gc-sections -Wl,-static
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := tests/asctime tests/clock tests/ctime tests/gmtime tests/localtime tests/mktime tests/strftime tests/time tests/strptime

build: headers libtarget.a.stamp

LIBRARY := libtarget.a
COMPILE := src/asctime.cxx src/clock.cxx src/ctime.cxx src/difftime.cxx src/gmtime.cxx src/localtime.cxx src/mktime.cxx src/settime.cxx src/strftime.cxx src/time.cxx src/timeutil.cxx src/asctime_r.cxx src/ctime_r.cxx src/gmtime_r.cxx src/localtime_r.cxx src/strptime.cxx
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

headers: mlt_headers $(PREFIX)/include/cyg/libc/time/time.h $(PREFIX)/include/cyg/libc/time/time.inl $(PREFIX)/include/cyg/libc/time/timeutil.h

$(PREFIX)/include/cyg/libc/time/time.h: $(REPOSITORY)/$(PACKAGE)/include/time.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/time/time.inl: $(REPOSITORY)/$(PACKAGE)/include/time.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/time/timeutil.h: $(REPOSITORY)/$(PACKAGE)/include/timeutil.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

