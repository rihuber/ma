################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/mpmc_mem_test_example.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/mpmc_mem_test_example.o 

C_DEPS += \
./src/mpmc_mem_test_example.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../ml605_bist_bsp_0/microblaze_0/include -mxl-pattern-compare -mcpu=v8.20.a -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


