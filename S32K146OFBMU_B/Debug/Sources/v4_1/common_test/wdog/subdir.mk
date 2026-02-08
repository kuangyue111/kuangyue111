################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/v4_1/common_test/wdog/iec60730b_wdog.c" \

C_SRCS += \
../Sources/v4_1/common_test/wdog/iec60730b_wdog.c \

C_DEPS_QUOTED += \
"./Sources/v4_1/common_test/wdog/iec60730b_wdog.d" \

OBJS_QUOTED += \
"./Sources/v4_1/common_test/wdog/iec60730b_wdog.o" \

C_DEPS += \
./Sources/v4_1/common_test/wdog/iec60730b_wdog.d \

OBJS_OS_FORMAT += \
./Sources/v4_1/common_test/wdog/iec60730b_wdog.o \

OBJS += \
./Sources/v4_1/common_test/wdog/iec60730b_wdog.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/v4_1/common_test/wdog/iec60730b_wdog.o: ../Sources/v4_1/common_test/wdog/iec60730b_wdog.c
	@echo 'Building file: $<'
	@echo 'Executing target #244 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/v4_1/common_test/wdog/iec60730b_wdog.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/v4_1/common_test/wdog/iec60730b_wdog.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


