################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f407vetx.s 

OBJS += \
./Startup/startup_stm32f407vetx.o 

S_DEPS += \
./Startup/startup_stm32f407vetx.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/startup_stm32f407vetx.o: ../Startup/startup_stm32f407vetx.s Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -c -x assembler-with-cpp -MMD -MP -MF"Startup/startup_stm32f407vetx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

