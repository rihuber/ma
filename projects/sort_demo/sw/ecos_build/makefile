# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /home/rihuber/reconos/core/ecos/ecos-patched/ecos/packages
export HOST := UNIX
export PREFIX := /home/rihuber/reconos/projects/sort_demo/sw/ecos_build/install
export COMMAND_PREFIX := mb-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export AR := $(COMMAND_PREFIX)ar

.PHONY: default build clean tests headers

build: headers $(PREFIX)/include/pkgconf/ecos.mak
	$(MAKE) -r -C hal/microblaze/arch/v2_0 $(PREFIX)/include/cyg/hal/mb_offsets.inc
	$(MAKE) -r -C hal/microblaze/arch/v2_0 ../../../common/current/makefile.tmp
	$(MAKE) -r -C reconos/current src/delegate.py
	$(MAKE) -r -C reconos/current src/delegate.c
	$(MAKE) -r -C services/memalloc/common/current heapgeninc.tcl
	$(MAKE) -r -C services/memalloc/common/current heaps.cxx
	$(MAKE) -r -C hal/common/current $@
	$(MAKE) -r -C io/common/current $@
	$(MAKE) -r -C io/serial/current $@
	$(MAKE) -r -C infra/current $@
	$(MAKE) -r -C kernel/current $@
	$(MAKE) -r -C services/memalloc/common/current $@
	$(MAKE) -r -C isoinfra/current $@
	$(MAKE) -r -C language/c/libc/common/current $@
	$(MAKE) -r -C language/c/libc/i18n/current $@
	$(MAKE) -r -C language/c/libc/setjmp/current $@
	$(MAKE) -r -C language/c/libc/signals/current $@
	$(MAKE) -r -C language/c/libc/startup/current $@
	$(MAKE) -r -C language/c/libc/stdio/current $@
	$(MAKE) -r -C language/c/libc/stdlib/current $@
	$(MAKE) -r -C language/c/libc/string/current $@
	$(MAKE) -r -C language/c/libc/time/current $@
	$(MAKE) -r -C language/c/libm/current $@
	$(MAKE) -r -C io/wallclock/current $@
	$(MAKE) -r -C error/current $@
	$(MAKE) -r -C hal/microblaze/arch/v2_0 $@
	$(MAKE) -r -C hal/microblaze/mb4a/v2_0 $@
	$(MAKE) -r -C hal/microblaze/generic/v2_0 $@
	$(MAKE) -r -C hal/microblaze/xilinx_ml605/current $@
	$(MAKE) -r -C reconos/current $@
	$(MAKE) -r -C compat/posix/current $@
	$(MAKE) -r -C hal/common/current $(PREFIX)/lib/extras.o
	$(MAKE) -r -C hal/microblaze/arch/v2_0 $(PREFIX)/lib/vectors.o
	$(MAKE) -r -C hal/microblaze/arch/v2_0 $(PREFIX)/lib/target.ld
	@echo $@ finished

clean:
	$(MAKE) -r -C hal/common/current $@
	$(MAKE) -r -C io/common/current $@
	$(MAKE) -r -C io/serial/current $@
	$(MAKE) -r -C infra/current $@
	$(MAKE) -r -C kernel/current $@
	$(MAKE) -r -C services/memalloc/common/current $@
	$(MAKE) -r -C isoinfra/current $@
	$(MAKE) -r -C language/c/libc/common/current $@
	$(MAKE) -r -C language/c/libc/i18n/current $@
	$(MAKE) -r -C language/c/libc/setjmp/current $@
	$(MAKE) -r -C language/c/libc/signals/current $@
	$(MAKE) -r -C language/c/libc/startup/current $@
	$(MAKE) -r -C language/c/libc/stdio/current $@
	$(MAKE) -r -C language/c/libc/stdlib/current $@
	$(MAKE) -r -C language/c/libc/string/current $@
	$(MAKE) -r -C language/c/libc/time/current $@
	$(MAKE) -r -C language/c/libm/current $@
	$(MAKE) -r -C io/wallclock/current $@
	$(MAKE) -r -C error/current $@
	$(MAKE) -r -C hal/microblaze/arch/v2_0 $@
	$(MAKE) -r -C hal/microblaze/mb4a/v2_0 $@
	$(MAKE) -r -C hal/microblaze/generic/v2_0 $@
	$(MAKE) -r -C hal/microblaze/xilinx_ml605/current $@
	$(MAKE) -r -C reconos/current $@
	$(MAKE) -r -C compat/posix/current $@
	@echo $@ finished

tests: build
	$(MAKE) -r -C hal/common/current $@
	$(MAKE) -r -C io/common/current $@
	$(MAKE) -r -C io/serial/current $@
	$(MAKE) -r -C infra/current $@
	$(MAKE) -r -C kernel/current $@
	$(MAKE) -r -C services/memalloc/common/current $@
	$(MAKE) -r -C isoinfra/current $@
	$(MAKE) -r -C language/c/libc/common/current $@
	$(MAKE) -r -C language/c/libc/i18n/current $@
	$(MAKE) -r -C language/c/libc/setjmp/current $@
	$(MAKE) -r -C language/c/libc/signals/current $@
	$(MAKE) -r -C language/c/libc/startup/current $@
	$(MAKE) -r -C language/c/libc/stdio/current $@
	$(MAKE) -r -C language/c/libc/stdlib/current $@
	$(MAKE) -r -C language/c/libc/string/current $@
	$(MAKE) -r -C language/c/libc/time/current $@
	$(MAKE) -r -C language/c/libm/current $@
	$(MAKE) -r -C io/wallclock/current $@
	$(MAKE) -r -C error/current $@
	$(MAKE) -r -C hal/microblaze/arch/v2_0 $@
	$(MAKE) -r -C hal/microblaze/mb4a/v2_0 $@
	$(MAKE) -r -C hal/microblaze/generic/v2_0 $@
	$(MAKE) -r -C hal/microblaze/xilinx_ml605/current $@
	$(MAKE) -r -C reconos/current $@
	$(MAKE) -r -C compat/posix/current $@
	@echo $@ finished

headers:
	$(MAKE) -r -C hal/common/current $@
	$(MAKE) -r -C io/common/current $@
	$(MAKE) -r -C io/serial/current $@
	$(MAKE) -r -C infra/current $@
	$(MAKE) -r -C kernel/current $@
	$(MAKE) -r -C services/memalloc/common/current $@
	$(MAKE) -r -C isoinfra/current $@
	$(MAKE) -r -C language/c/libc/common/current $@
	$(MAKE) -r -C language/c/libc/i18n/current $@
	$(MAKE) -r -C language/c/libc/setjmp/current $@
	$(MAKE) -r -C language/c/libc/signals/current $@
	$(MAKE) -r -C language/c/libc/startup/current $@
	$(MAKE) -r -C language/c/libc/stdio/current $@
	$(MAKE) -r -C language/c/libc/stdlib/current $@
	$(MAKE) -r -C language/c/libc/string/current $@
	$(MAKE) -r -C language/c/libc/time/current $@
	$(MAKE) -r -C language/c/libm/current $@
	$(MAKE) -r -C io/wallclock/current $@
	$(MAKE) -r -C error/current $@
	$(MAKE) -r -C hal/microblaze/arch/v2_0 $@
	$(MAKE) -r -C hal/microblaze/mb4a/v2_0 $@
	$(MAKE) -r -C hal/microblaze/generic/v2_0 $@
	$(MAKE) -r -C hal/microblaze/xilinx_ml605/current $@
	$(MAKE) -r -C reconos/current $@
	$(MAKE) -r -C compat/posix/current $@
	@echo $@ finished

$(PREFIX)/include/pkgconf/ecos.mak: makefile
	@echo 'ECOS_GLOBAL_CFLAGS = -msoft-float -mcpu=v4.00.a -Wall -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fdata-sections -fno-exceptions -I. -I$(HW_DESIGN)/microblaze_0/include -I$(RECONOS)/core/ecos/include' > $@
	@echo 'ECOS_GLOBAL_LDFLAGS = -msoft-float -mcpu=v4.00.a -g -nostdlib -Wl,--gc-sections -Wl,-static' >> $@
	@echo 'ECOS_COMMAND_PREFIX = $(COMMAND_PREFIX)' >> $@

