################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/spi_stuff/src/test_spi.c 

OBJS += \
./Core/Src/spi_stuff/src/test_spi.o 

C_DEPS += \
./Core/Src/spi_stuff/src/test_spi.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/spi_stuff/src/%.o Core/Src/spi_stuff/src/%.su Core/Src/spi_stuff/src/%.cyclo: ../Core/Src/spi_stuff/src/%.c Core/Src/spi_stuff/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H753xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H7xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"/Users/sid/STM32_Stuff/apss_spi/Core/Src/spi_stuff/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-spi_stuff-2f-src

clean-Core-2f-Src-2f-spi_stuff-2f-src:
	-$(RM) ./Core/Src/spi_stuff/src/test_spi.cyclo ./Core/Src/spi_stuff/src/test_spi.d ./Core/Src/spi_stuff/src/test_spi.o ./Core/Src/spi_stuff/src/test_spi.su

.PHONY: clean-Core-2f-Src-2f-spi_stuff-2f-src

