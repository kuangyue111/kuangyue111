################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/SaftyCheck/McuSelfCheck.c" \

C_SRCS += \
../Sources/SaftyCheck/McuSelfCheck.c \

C_DEPS_QUOTED += \
"./Sources/SaftyCheck/McuSelfCheck.d" \

OBJS_QUOTED += \
"./Sources/SaftyCheck/McuSelfCheck.o" \

C_DEPS += \
./Sources/SaftyCheck/McuSelfCheck.d \

OBJS_OS_FORMAT += \
./Sources/SaftyCheck/McuSelfCheck.o \

OBJS += \
./Sources/SaftyCheck/McuSelfCheck.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/SaftyCheck/McuSelfCheck.o: ../Sources/SaftyCheck/McuSelfCheck.c
	@echo 'Building file: $<'
	@echo 'Executing target #233 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SaftyCheck/McuSelfCheck.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/SaftyCheck/McuSelfCheck.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


