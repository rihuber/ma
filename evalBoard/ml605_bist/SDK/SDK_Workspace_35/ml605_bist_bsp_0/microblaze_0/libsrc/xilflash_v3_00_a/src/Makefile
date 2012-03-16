####################################/-*-Makefile-*- 
#
# Copyright (c) 2007-2008 Xilinx, Inc.  All rights reserved.
#
# Xilinx, Inc.
#
# XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
# COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
# ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
# STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
# IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
# FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.  
# XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
# THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
# ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
# FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
# AND FITNESS FOR A PARTICULAR PURPOSE.
# 
######################################################

COMPILER=
ARCHIVER=
CP=cp
COMPILER_FLAGS=
EXTRA_COMPILER_FLAGS=

RELEASEDIR=../../../lib
INCLUDEDIR=../../../include
INCLUDES=-I${INCLUDEDIR} 
ISF_DIR = .

EXPORT_INCLUDE_FILES = $(ISF_DIR)/include/xilflash.h \
			$(ISF_DIR)/include/xilflash_cfi.h \
			$(ISF_DIR)/include/xilflash_intel.h \
			$(ISF_DIR)/include/xilflash_properties.h \
			$(ISF_DIR)/include/xilflash_amd.h 

LIB_SRCS = $(ISF_DIR)/xilflash.c \
          $(ISF_DIR)/xilflash_cfi.c \
          $(ISF_DIR)/xilflash_intel.c \
          $(ISF_DIR)/xilflash_amd.c 


# create ISF_SRCS based on configured options

ISF_SRCS = $(LIB_SRCS)

ISF_OBJS = $(ISF_SRCS:%.c=%.o)

libs: libxilflash.a
	cp libxilflash.a $(RELEASEDIR)
	make clean
	
include: 
	@for i in $(EXPORT_INCLUDE_FILES); do \
	echo ${CP} -r $$i ${INCLUDEDIR}; \
	${CP} -r $$i ${INCLUDEDIR}; \
	done

clean:
	rm -rf obj/*.o	
	rmdir obj
	rm libxilflash.a

libxilflash.a: obj_dir print_msg_flash_base $(ISF_OBJS) 
	@echo "Creating archive $@"
	$(ARCHIVER) rc $@ obj/*.o

obj_dir:
	mkdir -p obj

print_msg_flash_base:
	@echo "Compiling XilFlash Library:"

.c.o:
	$(COMPILER) $(COMPILER_FLAGS) $(EXTRA_COMPILER_FLAGS) $(INCLUDES) -c $< -o obj/$(@F)
