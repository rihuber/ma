# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /home/rihuber/reconos/core/ecos/ecos-patched/ecos/packages
export PREFIX := /home/rihuber/reconos/projects/sort_demo/sw/ecos_build/install
export COMMAND_PREFIX := mb-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := services/memalloc/common/current
OBJECT_PREFIX := services_memalloc_common
CFLAGS := -msoft-float -mcpu=v4.00.a -Wall -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fdata-sections -fno-exceptions -I. -I$(HW_DESIGN)/microblaze_0/include -I$(RECONOS)/core/ecos/include
LDFLAGS := -msoft-float -mcpu=v4.00.a -g -nostdlib -Wl,--gc-sections -Wl,-static
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := tests/dlmalloc1 tests/dlmalloc2 tests/heaptest tests/kmemfix1 tests/kmemvar1 tests/malloc1 tests/malloc2 tests/malloc3 tests/malloc4 tests/memfix1 tests/memfix2 tests/memvar1 tests/memvar2 tests/realloc tests/sepmeta1 tests/sepmeta2

build: headers libtarget.a.stamp

LIBRARY := libtarget.a
COMPILE := src/dlmalloc.cxx src/memfixed.cxx src/memvar.cxx src/sepmeta.cxx src/debug.c src/kapi.cxx src/malloc.cxx heaps.o.d
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

heaps.o.d: heaps.cxx
	$(CC) $(ACTUAL_CXXFLAGS) $(INCLUDE_PATH) -Wp,-MD,heaps.tmp -c -o $(OBJECT_PREFIX)_$(notdir $(@:.o.d=.o)) $<
	@sed -e '/^ *\\/d' -e "s#.*: #$@: #" heaps.tmp > $@
	@rm heaps.tmp
	

heapgeninc.tcl: $(wildcard $(REPOSITORY)/$(PACKAGE)/src/heapgen.cpp)
	$(CC) $(ACTUAL_CXXFLAGS) $(INCLUDE_PATH) -Wp,-MD,heapgen.tmp -E $< -o $@
	@sed -e '/^ *\\/d' -e "s#.*: #$@: #" heapgen.tmp > $(notdir $@).deps
	@rm heapgen.tmp
	

heaps.cxx: $(wildcard heapgeninc.tcl $(REPOSITORY)/$(PACKAGE)/src/heapgen.tcl)
	XPWD=`pwd` ; cd $(REPOSITORY)/$(PACKAGE)/src ; sh heapgen.tcl "$(PREFIX)" "$$XPWD"
	@cp heaps.hxx "$(PREFIX)"/include/pkgconf/heaps.hxx
	@chmod u+w "$(PREFIX)"/include/pkgconf/heaps.hxx
	

headers: mlt_headers $(PREFIX)/include/cyg/memalloc/mfiximpl.inl $(PREFIX)/include/cyg/memalloc/kapidata.h $(PREFIX)/include/cyg/memalloc/memjoin.hxx $(PREFIX)/include/cyg/memalloc/kapi.h $(PREFIX)/include/cyg/memalloc/memvar.hxx $(PREFIX)/include/cyg/memalloc/mvarimpl.inl $(PREFIX)/include/cyg/memalloc/mempolt2.hxx $(PREFIX)/include/cyg/memalloc/common.hxx $(PREFIX)/include/cyg/memalloc/mempoolt.hxx $(PREFIX)/include/cyg/memalloc/mempolt2.inl $(PREFIX)/include/cyg/memalloc/mfiximpl.hxx $(PREFIX)/include/cyg/memalloc/memfixed.hxx $(PREFIX)/include/cyg/memalloc/dlmalloc.hxx $(PREFIX)/include/cyg/memalloc/memjoin.inl $(PREFIX)/include/cyg/memalloc/sepmeta.hxx $(PREFIX)/include/cyg/memalloc/mvarimpl.hxx $(PREFIX)/include/cyg/memalloc/sepmetaimpl.hxx $(PREFIX)/include/cyg/memalloc/mempoolt.inl $(PREFIX)/include/cyg/memalloc/dlmallocimpl.hxx $(PREFIX)/include/cyg/memalloc/sepmetaimpl.inl

$(PREFIX)/include/cyg/memalloc/mfiximpl.inl: $(REPOSITORY)/$(PACKAGE)/include/mfiximpl.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/kapidata.h: $(REPOSITORY)/$(PACKAGE)/include/kapidata.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/memjoin.hxx: $(REPOSITORY)/$(PACKAGE)/include/memjoin.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/kapi.h: $(REPOSITORY)/$(PACKAGE)/include/kapi.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/memvar.hxx: $(REPOSITORY)/$(PACKAGE)/include/memvar.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/mvarimpl.inl: $(REPOSITORY)/$(PACKAGE)/include/mvarimpl.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/mempolt2.hxx: $(REPOSITORY)/$(PACKAGE)/include/mempolt2.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/common.hxx: $(REPOSITORY)/$(PACKAGE)/include/common.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/mempoolt.hxx: $(REPOSITORY)/$(PACKAGE)/include/mempoolt.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/mempolt2.inl: $(REPOSITORY)/$(PACKAGE)/include/mempolt2.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/mfiximpl.hxx: $(REPOSITORY)/$(PACKAGE)/include/mfiximpl.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/memfixed.hxx: $(REPOSITORY)/$(PACKAGE)/include/memfixed.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/dlmalloc.hxx: $(REPOSITORY)/$(PACKAGE)/include/dlmalloc.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/memjoin.inl: $(REPOSITORY)/$(PACKAGE)/include/memjoin.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/sepmeta.hxx: $(REPOSITORY)/$(PACKAGE)/include/sepmeta.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/mvarimpl.hxx: $(REPOSITORY)/$(PACKAGE)/include/mvarimpl.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/sepmetaimpl.hxx: $(REPOSITORY)/$(PACKAGE)/include/sepmetaimpl.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/mempoolt.inl: $(REPOSITORY)/$(PACKAGE)/include/mempoolt.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/dlmallocimpl.hxx: $(REPOSITORY)/$(PACKAGE)/include/dlmallocimpl.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/memalloc/sepmetaimpl.inl: $(REPOSITORY)/$(PACKAGE)/include/sepmetaimpl.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

