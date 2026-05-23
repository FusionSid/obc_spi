################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/spi/src/packet.c \
../Core/Src/spi/src/spi.c \
../Core/Src/spi/src/test_payload.c 

OBJS += \
./Core/Src/spi/src/packet.o \
./Core/Src/spi/src/spi.o \
./Core/Src/spi/src/test_payload.o 

C_DEPS += \
./Core/Src/spi/src/packet.d \
./Core/Src/spi/src/spi.d \
./Core/Src/spi/src/test_payload.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/spi/src/%.o Core/Src/spi/src/%.su Core/Src/spi/src/%.cyclo: ../Core/Src/spi/src/%.c Core/Src/spi/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H753xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H7xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"/Users/sid/STM32_Stuff/apss_spi/Core/Src/uart" -I"/Users/sid/STM32_Stuff/apss_spi/Core/Src/spi" -I"/Users/sid/STM32_Stuff/apss_spi/Core/Src/spi/include" -I"/Users/sid/STM32_Stuff/apss_spi/Core/Src/uart/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-spi-2f-src

clean-Core-2f-Src-2f-spi-2f-src:
	-$(RM) ./Core/Src/spi/src/packet.cyclo ./Core/Src/spi/src/packet.d ./Core/Src/spi/src/packet.o ./Core/Src/spi/src/packet.su ./Core/Src/spi/src/spi.cyclo ./Core/Src/spi/src/spi.d ./Core/Src/spi/src/spi.o ./Core/Src/spi/src/spi.su ./Core/Src/spi/src/test_payload.cyclo ./Core/Src/spi/src/test_payload.d ./Core/Src/spi/src/test_payload.o ./Core/Src/spi/src/test_payload.su

.PHONY: clean-Core-2f-Src-2f-spi-2f-src

