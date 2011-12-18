# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /home/rihuber/reconos/core/ecos/ecos-patched/ecos/packages
export PREFIX := /home/rihuber/reconos/projects/sort_demo/sw/ecos_build/install
export COMMAND_PREFIX := mb-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := hal/common/current
OBJECT_PREFIX := hal_common
CFLAGS := -msoft-float -mcpu=v4.00.a -Wall -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fdata-sections -fno-exceptions -I. -I$(HW_DESIGN)/microblaze_0/include -I$(RECONOS)/core/ecos/include
LDFLAGS := -msoft-float -mcpu=v4.00.a -g -nostdlib -Wl,--gc-sections -Wl,-static
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := tests/context tests/basic tests/cache

build: headers libtarget.a.stamp libextras.a.stamp

LIBRARY := libtarget.a
COMPILE := src/drv_api.c src/hal_if.c src/hal_misc.c
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

LIBRARY := libextras.a
COMPILE := src/dummy.c
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

$(PREFIX)/lib/extras.o: $(wildcard $(PREFIX)/lib/libextras.a)
	mb-ld -nostdlib -r --whole-archive -o $@ $<
	

headers: mlt_headers $(PREFIX)/include/cyg/hal/dbg-threads-api.h $(PREFIX)/include/cyg/hal/dbg-thread-syscall.h $(PREFIX)/include/cyg/hal/hal_endian.h $(PREFIX)/include/cyg/hal/generic-stub.h $(PREFIX)/include/cyg/hal/hal_misc.h $(PREFIX)/include/cyg/hal/hal_arbiter.h $(PREFIX)/include/cyg/hal/hal_tables.h $(PREFIX)/include/cyg/hal/hal_stub.h $(PREFIX)/include/cyg/hal/hal_spd.h $(PREFIX)/include/cyg/hal/drv_api.h $(PREFIX)/include/cyg/hal/hal_if.h

$(PREFIX)/include/cyg/hal/dbg-threads-api.h: $(REPOSITORY)/$(PACKAGE)/include/dbg-threads-api.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/dbg-thread-syscall.h: $(REPOSITORY)/$(PACKAGE)/include/dbg-thread-syscall.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/hal_endian.h: $(REPOSITORY)/$(PACKAGE)/include/hal_endian.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/generic-stub.h: $(REPOSITORY)/$(PACKAGE)/include/generic-stub.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/hal_misc.h: $(REPOSITORY)/$(PACKAGE)/include/hal_misc.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/hal_arbiter.h: $(REPOSITORY)/$(PACKAGE)/include/hal_arbiter.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/hal_tables.h: $(REPOSITORY)/$(PACKAGE)/include/hal_tables.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/hal_stub.h: $(REPOSITORY)/$(PACKAGE)/include/hal_stub.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/hal_spd.h: $(REPOSITORY)/$(PACKAGE)/include/hal_spd.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/drv_api.h: $(REPOSITORY)/$(PACKAGE)/include/drv_api.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/hal_if.h: $(REPOSITORY)/$(PACKAGE)/include/hal_if.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

