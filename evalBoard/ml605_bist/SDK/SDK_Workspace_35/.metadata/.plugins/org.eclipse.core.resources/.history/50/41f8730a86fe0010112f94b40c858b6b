################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
bootloader_p30t.elf.size \

ELFCHECK += \
bootloader_p30t.elf.elfcheck \


# All Target
all: bootloader_p30t.elf secondary-outputs

# Tool invocations
bootloader_p30t.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo Building target: $@
	@echo Invoking: MicroBlaze gcc linker
	mb-gcc -Wl,-T -Wl,../src/lscript.ld -L../../ml605_bist_bsp_0/microblaze_0/lib -mxl-pattern-compare -mcpu=v8.20.a -mno-xl-soft-mul -o"bootloader_p30t.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@
	@echo ' '

bootloader_p30t.elf.size: bootloader_p30t.elf
	@echo Invoking: MicroBlaze Print Size
	mb-size bootloader_p30t.elf  |tee "bootloader_p30t.elf.size"
	@echo Finished building: $@
	@echo ' '

bootloader_p30t.elf.elfcheck: bootloader_p30t.elf
	@echo Invoking: Xilinx ELF Check
	elfcheck bootloader_p30t.elf -hw ../../ml605_bist_hw_platform/system.xml -pe microblaze_0  |tee "bootloader_p30t.elf.elfcheck"
	@echo Finished building: $@
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(ELFCHECK)$(S_UPPER_DEPS) bootloader_p30t.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
