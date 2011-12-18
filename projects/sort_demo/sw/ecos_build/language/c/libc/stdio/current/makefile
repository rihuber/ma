# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /home/rihuber/reconos/core/ecos/ecos-patched/ecos/packages
export PREFIX := /home/rihuber/reconos/projects/sort_demo/sw/ecos_build/install
export COMMAND_PREFIX := mb-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := language/c/libc/stdio/current
OBJECT_PREFIX := language_c_libc_stdio
CFLAGS := -msoft-float -mcpu=v4.00.a -Wall -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fdata-sections -fno-exceptions -I. -I$(HW_DESIGN)/microblaze_0/include -I$(RECONOS)/core/ecos/include
LDFLAGS := -msoft-float -mcpu=v4.00.a -g -nostdlib -Wl,--gc-sections -Wl,-static
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := tests/sprintf1 tests/sprintf2 tests/sscanf tests/stdiooutput 

build: headers libtarget.a.stamp

LIBRARY := libtarget.a
COMPILE := src/common/fflush.cxx src/common/feof.cxx src/common/freopen.cxx src/common/setvbuf.cxx src/common/snprintf.cxx src/common/sprintf.cxx src/common/sscanf.cxx src/common/stderr.cxx src/common/stdin.cxx src/common/stdiofiles.cxx src/common/fseek.cxx src/common/stdioinlines.cxx src/common/stdiosupp.cxx src/common/stdout.cxx src/common/stream.cxx src/common/streambuf.cxx src/common/ungetc.cxx src/common/vsnprintf.cxx src/common/vsscanf.cxx src/common/fileops.cxx src/input/fgetc.cxx src/input/fgets.cxx src/input/fread.cxx src/input/fscanf.cxx src/input/gets.cxx src/input/scanf.cxx src/input/vfscanf.cxx src/output/fnprintf.cxx src/output/fprintf.cxx src/output/fputc.cxx src/output/fputs.cxx src/output/fwrite.cxx src/output/printf.cxx src/output/vfnprintf.cxx src/common/fopen.cxx src/common/fclose.cxx
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

headers: mlt_headers $(PREFIX)/include/cyg/libc/stdio/streambuf.inl $(PREFIX)/include/cyg/libc/stdio/stdio.h $(PREFIX)/include/cyg/libc/stdio/io.inl $(PREFIX)/include/cyg/libc/stdio/stream.inl $(PREFIX)/include/cyg/libc/stdio/io.hxx $(PREFIX)/include/cyg/libc/stdio/streambuf.hxx $(PREFIX)/include/cyg/libc/stdio/stdiofiles.hxx $(PREFIX)/include/cyg/libc/stdio/stream.hxx $(PREFIX)/include/cyg/libc/stdio/stdiosupp.hxx $(PREFIX)/include/cyg/libc/stdio/stdio.inl $(PREFIX)/include/cyg/libc/stdio/stdiofiles.inl

$(PREFIX)/include/cyg/libc/stdio/streambuf.inl: $(REPOSITORY)/$(PACKAGE)/include/streambuf.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/stdio/stdio.h: $(REPOSITORY)/$(PACKAGE)/include/stdio.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/stdio/io.inl: $(REPOSITORY)/$(PACKAGE)/include/io.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/stdio/stream.inl: $(REPOSITORY)/$(PACKAGE)/include/stream.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/stdio/io.hxx: $(REPOSITORY)/$(PACKAGE)/include/io.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/stdio/streambuf.hxx: $(REPOSITORY)/$(PACKAGE)/include/streambuf.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/stdio/stdiofiles.hxx: $(REPOSITORY)/$(PACKAGE)/include/stdiofiles.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/stdio/stream.hxx: $(REPOSITORY)/$(PACKAGE)/include/stream.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/stdio/stdiosupp.hxx: $(REPOSITORY)/$(PACKAGE)/include/stdiosupp.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/stdio/stdio.inl: $(REPOSITORY)/$(PACKAGE)/include/stdio.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/stdio/stdiofiles.inl: $(REPOSITORY)/$(PACKAGE)/include/stdiofiles.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

