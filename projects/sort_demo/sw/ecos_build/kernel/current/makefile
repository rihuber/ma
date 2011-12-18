# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /home/rihuber/reconos/core/ecos/ecos-patched/ecos/packages
export PREFIX := /home/rihuber/reconos/projects/sort_demo/sw/ecos_build/install
export COMMAND_PREFIX := mb-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := kernel/current
OBJECT_PREFIX := kernel
CFLAGS := -msoft-float -mcpu=v4.00.a -Wall -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fdata-sections -fno-exceptions -I. -I$(HW_DESIGN)/microblaze_0/include -I$(RECONOS)/core/ecos/include
LDFLAGS := -msoft-float -mcpu=v4.00.a -g -nostdlib -Wl,--gc-sections -Wl,-static
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := tests/bin_sem0 tests/bin_sem1 tests/bin_sem2 tests/bin_sem3 tests/clock0 tests/clock1 tests/clockcnv tests/clocktruth tests/cnt_sem0 tests/cnt_sem1 tests/except1 tests/flag0 tests/flag1 tests/intr0 tests/kill tests/mbox1 tests/mqueue1 tests/mutex0 tests/mutex1 tests/mutex2 tests/mutex3 tests/release tests/sched1 tests/sync2 tests/sync3 tests/thread0 tests/thread1 tests/thread2 tests/kclock0 tests/kclock1 tests/kexcept1 tests/kflag0 tests/kflag1 tests/kintr0 tests/klock tests/kmbox1 tests/kmutex0 tests/kmutex1 tests/kmutex3 tests/kmutex4 tests/ksched1 tests/ksem0 tests/ksem1 tests/kthread0 tests/kthread1 tests/stress_threads tests/thread_gdb tests/timeslice tests/tm_basic tests/fptest tests/kalarm0 tests/kcache1 tests/kcache2

build: headers libtarget.a.stamp

LIBRARY := libtarget.a
COMPILE := src/common/clock.cxx src/common/timer.cxx src/common/kapi.cxx src/common/thread.cxx src/common/except.cxx src/intr/intr.cxx src/sched/bitmap.cxx src/sched/lottery.cxx src/sched/mlqueue.cxx src/sched/sched.cxx src/sync/bin_sem.cxx src/sync/cnt_sem.cxx src/sync/flag.cxx src/sync/cnt_sem2.cxx src/sync/mbox.cxx src/sync/mutex.cxx src/debug/dbg-thread-demux.c src/instrmnt/meminst.cxx
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

headers: mlt_headers $(PREFIX)/include/cyg/kernel/sched.inl $(PREFIX)/include/cyg/kernel/diag.h $(PREFIX)/include/cyg/kernel/intr.hxx $(PREFIX)/include/cyg/kernel/mlqueue.hxx $(PREFIX)/include/cyg/kernel/mqueue.inl $(PREFIX)/include/cyg/kernel/kapidata.h $(PREFIX)/include/cyg/kernel/sched.hxx $(PREFIX)/include/cyg/kernel/mboxt.inl $(PREFIX)/include/cyg/kernel/instrmnt.h $(PREFIX)/include/cyg/kernel/mboxt2.inl $(PREFIX)/include/cyg/kernel/mutex.hxx $(PREFIX)/include/cyg/kernel/mqueue.hxx $(PREFIX)/include/cyg/kernel/thread.hxx $(PREFIX)/include/cyg/kernel/kapi.h $(PREFIX)/include/cyg/kernel/errors.h $(PREFIX)/include/cyg/kernel/thread.inl $(PREFIX)/include/cyg/kernel/mboxt2.hxx $(PREFIX)/include/cyg/kernel/bitmap.hxx $(PREFIX)/include/cyg/kernel/mboxt.hxx $(PREFIX)/include/cyg/kernel/sema2.hxx $(PREFIX)/include/cyg/kernel/clock.inl $(PREFIX)/include/cyg/kernel/ktypes.h $(PREFIX)/include/cyg/kernel/mbox.hxx $(PREFIX)/include/cyg/kernel/clock.hxx $(PREFIX)/include/cyg/kernel/lottery.hxx $(PREFIX)/include/cyg/kernel/sema.hxx $(PREFIX)/include/cyg/kernel/timer.hxx $(PREFIX)/include/cyg/kernel/llistt.hxx $(PREFIX)/include/cyg/kernel/instrument_desc.h $(PREFIX)/include/cyg/kernel/flag.hxx $(PREFIX)/include/cyg/kernel/kernel.hxx $(PREFIX)/include/cyg/kernel/except.hxx $(PREFIX)/include/cyg/kernel/smp.hxx $(PREFIX)/include/cyg/kernel/test/stackmon.h

$(PREFIX)/include/cyg/kernel/sched.inl: $(REPOSITORY)/$(PACKAGE)/include/sched.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/diag.h: $(REPOSITORY)/$(PACKAGE)/include/diag.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/intr.hxx: $(REPOSITORY)/$(PACKAGE)/include/intr.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/mlqueue.hxx: $(REPOSITORY)/$(PACKAGE)/include/mlqueue.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/mqueue.inl: $(REPOSITORY)/$(PACKAGE)/include/mqueue.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/kapidata.h: $(REPOSITORY)/$(PACKAGE)/include/kapidata.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/sched.hxx: $(REPOSITORY)/$(PACKAGE)/include/sched.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/mboxt.inl: $(REPOSITORY)/$(PACKAGE)/include/mboxt.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/instrmnt.h: $(REPOSITORY)/$(PACKAGE)/include/instrmnt.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/mboxt2.inl: $(REPOSITORY)/$(PACKAGE)/include/mboxt2.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/mutex.hxx: $(REPOSITORY)/$(PACKAGE)/include/mutex.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/mqueue.hxx: $(REPOSITORY)/$(PACKAGE)/include/mqueue.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/thread.hxx: $(REPOSITORY)/$(PACKAGE)/include/thread.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/kapi.h: $(REPOSITORY)/$(PACKAGE)/include/kapi.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/errors.h: $(REPOSITORY)/$(PACKAGE)/include/errors.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/thread.inl: $(REPOSITORY)/$(PACKAGE)/include/thread.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/mboxt2.hxx: $(REPOSITORY)/$(PACKAGE)/include/mboxt2.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/bitmap.hxx: $(REPOSITORY)/$(PACKAGE)/include/bitmap.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/mboxt.hxx: $(REPOSITORY)/$(PACKAGE)/include/mboxt.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/sema2.hxx: $(REPOSITORY)/$(PACKAGE)/include/sema2.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/clock.inl: $(REPOSITORY)/$(PACKAGE)/include/clock.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/ktypes.h: $(REPOSITORY)/$(PACKAGE)/include/ktypes.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/mbox.hxx: $(REPOSITORY)/$(PACKAGE)/include/mbox.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/clock.hxx: $(REPOSITORY)/$(PACKAGE)/include/clock.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/lottery.hxx: $(REPOSITORY)/$(PACKAGE)/include/lottery.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/sema.hxx: $(REPOSITORY)/$(PACKAGE)/include/sema.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/timer.hxx: $(REPOSITORY)/$(PACKAGE)/include/timer.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/llistt.hxx: $(REPOSITORY)/$(PACKAGE)/include/llistt.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/instrument_desc.h: $(REPOSITORY)/$(PACKAGE)/include/instrument_desc.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/flag.hxx: $(REPOSITORY)/$(PACKAGE)/include/flag.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/kernel.hxx: $(REPOSITORY)/$(PACKAGE)/include/kernel.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/except.hxx: $(REPOSITORY)/$(PACKAGE)/include/except.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/smp.hxx: $(REPOSITORY)/$(PACKAGE)/include/smp.hxx
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/kernel/test/stackmon.h: $(REPOSITORY)/$(PACKAGE)/include/test/stackmon.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

