################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/uart/src/log.c 

OBJS += \
./Core/Src/uart/src/log.o 

C_DEPS += \
./Core/Src/uart/src/log.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/uart/src/%.o Core/Src/uart/src/%.su Core/Src/uart/src/%.cyclo: ../Core/Src/uart/src/%.c Core/Src/uart/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H753xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H7xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"/Users/sid/STM32_Stuff/apss_spi/Core/Src/uart" -I"/Users/sid/STM32_Stuff/apss_spi/Core/Src/spi" -I"/Users/sid/STM32_Stuff/apss_spi/Core/Src/spi/include" -I"/Users/sid/STM32_Stuff/apss_spi/Core/Src/uart/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-uart-2f-src

clean-Core-2f-Src-2f-uart-2f-src:
	-$(RM) ./Core/Src/uart/src/log.cyclo ./Core/Src/uart/src/log.d ./Core/Src/uart/src/log.o ./Core/Src/uart/src/log.su

.PHONY: clean-Core-2f-Src-2f-uart-2f-src

