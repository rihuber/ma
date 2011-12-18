# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /home/rihuber/reconos/core/ecos/ecos-patched/ecos/packages
export PREFIX := /home/rihuber/reconos/projects/sort_demo/sw/ecos_build/install
export COMMAND_PREFIX := mb-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := infra/current
OBJECT_PREFIX := infra
CFLAGS := -msoft-float -mcpu=v4.00.a -Wall -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fdata-sections -fno-exceptions -I. -I$(HW_DESIGN)/microblaze_0/include -I$(RECONOS)/core/ecos/include
LDFLAGS := -msoft-float -mcpu=v4.00.a -g -nostdlib -Wl,-static -Wl,--fatal-warnings
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := tests/cxxsupp tests/diag_sprintf1 tests/diag_sprintf2

build: headers libtarget.a.stamp

LIBRARY := libtarget.a
COMPILE := src/startup.cxx src/prestart.cxx src/pkgstart.cxx src/userstart.cxx src/dummyxxmain.cxx src/null.cxx src/simple.cxx src/fancy.cxx src/buffer.cxx src/diag.cxx src/tcdiag.cxx src/memcpy.c src/memset.c src/delete.cxx src/eprintf.c src/pure.cxx
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

headers: mlt_headers $(PREFIX)/include/cyg/infra/diag.h $(PREFIX)/include/cyg/infra/clist.hxx $(PREFIX)/include/cyg/infra/cyg_ass.h $(PREFIX)/include/cyg/infra/cyg_trac.h $(PREFIX)/include/cyg/infra/testcase.h $(PREFIX)/include/cyg/infra/cyg_type.inc $(PREFIX)/include/cyg/infra/cyg_type.h

$(PREFIX)/include/cyg/infra/diag.h: $(REPOSITORY)/$(PACKAGE)/include/diag.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/infra/clist.hxx: $(REPOSITORY)/$(PACKAGE)/include/clist.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/infra/cyg_ass.h: $(REPOSITORY)/$(PACKAGE)/include/cyg_ass.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/infra/cyg_trac.h: $(REPOSITORY)/$(PACKAGE)/include/cyg_trac.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/infra/testcase.h: $(REPOSITORY)/$(PACKAGE)/include/testcase.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/infra/cyg_type.inc: $(REPOSITORY)/$(PACKAGE)/include/cyg_type.inc
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/infra/cyg_type.h: $(REPOSITORY)/$(PACKAGE)/include/cyg_type.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

