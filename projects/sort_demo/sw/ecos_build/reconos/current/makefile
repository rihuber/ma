# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /home/rihuber/reconos/core/ecos/ecos-patched/ecos/packages
export PREFIX := /home/rihuber/reconos/projects/sort_demo/sw/ecos_build/install
export COMMAND_PREFIX := mb-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := reconos/current
OBJECT_PREFIX := reconos
CFLAGS := -msoft-float -mcpu=v4.00.a -Wall -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fdata-sections -fno-exceptions -I. -I$(HW_DESIGN)/microblaze_0/include -I$(RECONOS)/core/ecos/include -D__RECONOS__
LDFLAGS := -msoft-float -mcpu=v4.00.a -g -nostdlib -Wl,--gc-sections -Wl,-static
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := 

build: headers libtarget.a.stamp

LIBRARY := libtarget.a
COMPILE := src/reconos.c src/delegate.c src/osif_comm.c
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

src/delegate.py: $(wildcard $(REPOSITORY)/$(PACKAGE)/src/delegate.tmpl)
	cheetah compile --flat --odir src $<
	

src/delegate.c: $(wildcard src/delegate.py $(RECONOS)/core/defs/*.cmddef)
	PYTHONPATH=$(PYTHONPATH):$(RECONOS)/tools/python/generator python src/delegate.py $(RECONOS)/core/defs/*.cmddef | indent > $@
	

headers: mlt_headers $(PREFIX)/include/reconos/reconos.h $(PREFIX)/include/reconos/hw_sched.h $(PREFIX)/include/reconos/thermal.h $(PREFIX)/include/reconos/ecap.h $(PREFIX)/include/reconos/osif_comm.h $(PREFIX)/include/reconos/statetrace.h $(PREFIX)/include/reconos/hw_thread.hxx $(PREFIX)/include/reconos/resources.h

$(PREFIX)/include/reconos/reconos.h: $(REPOSITORY)/$(PACKAGE)/include/reconos.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/reconos/hw_sched.h: $(REPOSITORY)/$(PACKAGE)/include/hw_sched.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/reconos/thermal.h: $(REPOSITORY)/$(PACKAGE)/include/thermal.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/reconos/ecap.h: $(REPOSITORY)/$(PACKAGE)/include/ecap.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/reconos/osif_comm.h: $(REPOSITORY)/$(PACKAGE)/include/osif_comm.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/reconos/statetrace.h: $(REPOSITORY)/$(PACKAGE)/include/statetrace.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/reconos/hw_thread.hxx: $(REPOSITORY)/$(PACKAGE)/include/hw_thread.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/reconos/resources.h: $(REPOSITORY)/$(PACKAGE)/include/resources.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

