################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Project_BSP/DS1307.c \
../Project_BSP/LCD.c 

OBJS += \
./Project_BSP/DS1307.o \
./Project_BSP/LCD.o 

C_DEPS += \
./Project_BSP/DS1307.d \
./Project_BSP/LCD.d 


# Each subdirectory must supply rules for building sources it contributes
Project_BSP/%.o Project_BSP/%.su Project_BSP/%.cyclo: ../Project_BSP/%.c Project_BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Academics/Learnings/Learning/Embedded Systems/Fastbit Embedded Academy/MCU 1/Projects/MCU 1/stm32f4xx_drivers/drivers/Inc" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Project_BSP

clean-Project_BSP:
	-$(RM) ./Project_BSP/DS1307.cyclo ./Project_BSP/DS1307.d ./Project_BSP/DS1307.o ./Project_BSP/DS1307.su ./Project_BSP/LCD.cyclo ./Project_BSP/LCD.d ./Project_BSP/LCD.o ./Project_BSP/LCD.su

.PHONY: clean-Project_BSP

