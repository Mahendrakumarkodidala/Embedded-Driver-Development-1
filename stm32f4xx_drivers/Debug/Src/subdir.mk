################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Project_RTC_LCD.c 

OBJS += \
./Src/Project_RTC_LCD.o 

C_DEPS += \
./Src/Project_RTC_LCD.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Academics/Learnings/Learning/Embedded Systems/Fastbit Embedded Academy/MCU 1/Projects/MCU 1/stm32f4xx_drivers/drivers/Inc" -I../Inc -I../Project_BSP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/Project_RTC_LCD.cyclo ./Src/Project_RTC_LCD.d ./Src/Project_RTC_LCD.o ./Src/Project_RTC_LCD.su

.PHONY: clean-Src

