# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /home/rihuber/reconos/core/ecos/ecos-patched/ecos/packages
export PREFIX := /home/rihuber/reconos/projects/sort_demo/sw/ecos_build/install
export COMMAND_PREFIX := mb-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := hal/microblaze/arch/v2_0
OBJECT_PREFIX := hal_microblaze_arch
CFLAGS := -msoft-float -mcpu=v4.00.a -Wall -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fdata-sections -fno-exceptions -I. -I$(HW_DESIGN)/microblaze_0/include -I$(RECONOS)/core/ecos/include
LDFLAGS := -msoft-float -mcpu=v4.00.a -g -nostdlib -Wl,--gc-sections -Wl,-static
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := 

build: headers libtarget.a.stamp

LIBRARY := libtarget.a
COMPILE := src/hal_misc.c src/context.S src/mb_stub.c src/hal_intr.c
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

$(PREFIX)/include/cyg/hal/mb_offsets.inc: $(wildcard $(REPOSITORY)/$(PACKAGE)/src/hal_mk_defs.c)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -Wp,-MD,mb_offsets.tmp -o hal_mk_defs.tmp -S $<
	fgrep .equ hal_mk_defs.tmp | sed s/#// > $@
	@echo $@ ": \\" > $(notdir $@).deps
	@tail -n 2 mb_offsets.tmp >> $(notdir $@).deps
	@echo >> $(notdir $@).deps
	@rm mb_offsets.tmp hal_mk_defs.tmp
	

$(PREFIX)/lib/vectors.o: $(wildcard $(REPOSITORY)/$(PACKAGE)/src/vectors.S)
	$(CC) -Wp,-MD,vectors.tmp $(INCLUDE_PATH) $(CFLAGS) -c -o $@ $<
	@echo $@ ": \\" > $(notdir $@).deps
	@tail -n 2 vectors.tmp >> $(notdir $@).deps
	@echo >> $(notdir $@).deps
	@rm vectors.tmp
	

$(PREFIX)/lib/target.ld: $(wildcard $(REPOSITORY)/$(PACKAGE)/src/microblaze.ld)
	$(CC) -E -P -Wp,-MD,target.tmp -DEXTRAS=1 -xc $(INCLUDE_PATH) $(CFLAGS) -o $@ $<
	@echo $@ ": \\" > $(notdir $@).deps
	@tail -n 2 target.tmp >> $(notdir $@).deps
	@echo >> $(notdir $@).deps
	@rm target.tmp
	

../../../common/current/makefile.tmp: $(wildcard ../../../common/current/makefile)
	mv ../../../common/current/makefile ../../../common/current/makefile.tmp
	cat ../../../common/current/makefile.tmp | sed -e "s/\$$(CC) \$$(CFLAGS) -nostdlib -Wl,-r -Wl,--whole-archive -o \$$@ \$$</$(COMMAND_PREFIX)ld -nostdlib -r --whole-archive -o \$$@ \$$</" > ../../../common/current/makefile
	

headers: mlt_headers $(PREFIX)/include/cyg/hal/basetype.h $(PREFIX)/include/cyg/hal/mb_regs.h $(PREFIX)/include/cyg/hal/hal_mem.h $(PREFIX)/include/cyg/hal/platform.h $(PREFIX)/include/cyg/hal/arch.inc $(PREFIX)/include/cyg/hal/hal_io.h $(PREFIX)/include/cyg/hal/hal_cache.h $(PREFIX)/include/cyg/hal/hal_arch.h $(PREFIX)/include/cyg/hal/hal_intr.h $(PREFIX)/include/cyg/hal/mb_stub.h

$(PREFIX)/include/cyg/hal/basetype.h: $(REPOSITORY)/$(PACKAGE)/include/basetype.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/mb_regs.h: $(REPOSITORY)/$(PACKAGE)/include/mb_regs.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/hal_mem.h: $(REPOSITORY)/$(PACKAGE)/include/hal_mem.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/platform.h: $(REPOSITORY)/$(PACKAGE)/include/platform.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/arch.inc: $(REPOSITORY)/$(PACKAGE)/include/arch.inc
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/hal_io.h: $(REPOSITORY)/$(PACKAGE)/include/hal_io.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/hal_cache.h: $(REPOSITORY)/$(PACKAGE)/include/hal_cache.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/hal_arch.h: $(REPOSITORY)/$(PACKAGE)/include/hal_arch.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/hal_intr.h: $(REPOSITORY)/$(PACKAGE)/include/hal_intr.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/hal/mb_stub.h: $(REPOSITORY)/$(PACKAGE)/include/mb_stub.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

