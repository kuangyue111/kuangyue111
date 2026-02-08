################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/SaftyCheck/SafetyLib/Safety_ADC.c" \
"../Sources/SaftyCheck/SafetyLib/Safety_CMU.c" \
"../Sources/SaftyCheck/SafetyLib/Safety_CRC.c" \
"../Sources/SaftyCheck/SafetyLib/Safety_Flash.c" \
"../Sources/SaftyCheck/SafetyLib/Safety_NVIC.c" \
"../Sources/SaftyCheck/SafetyLib/Safety_RegCheck.c" \
"../Sources/SaftyCheck/SafetyLib/Safety_SCST.c" \
"../Sources/SaftyCheck/SafetyLib/Safety_SRAM.c" \
"../Sources/SaftyCheck/SafetyLib/Safety_WDOG.c" \

C_SRCS += \
../Sources/SaftyCheck/SafetyLib/Safety_ADC.c \
../Sources/SaftyCheck/SafetyLib/Safety_CMU.c \
../Sources/SaftyCheck/SafetyLib/Safety_CRC.c \
../Sources/SaftyCheck/SafetyLib/Safety_Flash.c \
../Sources/SaftyCheck/SafetyLib/Safety_NVIC.c \
../Sources/SaftyCheck/SafetyLib/Safety_RegCheck.c \
../Sources/SaftyCheck/SafetyLib/Safety_SCST.c \
../Sources/SaftyCheck/SafetyLib/Safety_SRAM.c \
../Sources/SaftyCheck/SafetyLib/Safety_WDOG.c \

C_DEPS_QUOTED += \
"./Sources/SaftyCheck/SafetyLib/Safety_ADC.d" \
"./Sources/SaftyCheck/SafetyLib/Safety_CMU.d" \
"./Sources/SaftyCheck/SafetyLib/Safety_CRC.d" \
"./Sources/SaftyCheck/SafetyLib/Safety_Flash.d" \
"./Sources/SaftyCheck/SafetyLib/Safety_NVIC.d" \
"./Sources/SaftyCheck/SafetyLib/Safety_RegCheck.d" \
"./Sources/SaftyCheck/SafetyLib/Safety_SCST.d" \
"./Sources/SaftyCheck/SafetyLib/Safety_SRAM.d" \
"./Sources/SaftyCheck/SafetyLib/Safety_WDOG.d" \

OBJS_QUOTED += \
"./Sources/SaftyCheck/SafetyLib/Safety_ADC.o" \
"./Sources/SaftyCheck/SafetyLib/Safety_CMU.o" \
"./Sources/SaftyCheck/SafetyLib/Safety_CRC.o" \
"./Sources/SaftyCheck/SafetyLib/Safety_Flash.o" \
"./Sources/SaftyCheck/SafetyLib/Safety_NVIC.o" \
"./Sources/SaftyCheck/SafetyLib/Safety_RegCheck.o" \
"./Sources/SaftyCheck/SafetyLib/Safety_SCST.o" \
"./Sources/SaftyCheck/SafetyLib/Safety_SRAM.o" \
"./Sources/SaftyCheck/SafetyLib/Safety_WDOG.o" \

C_DEPS += \
./Sources/SaftyCheck/SafetyLib/Safety_ADC.d \
./Sources/SaftyCheck/SafetyLib/Safety_CMU.d \
./Sources/SaftyCheck/SafetyLib/Safety_CRC.d \
./Sources/SaftyCheck/SafetyLib/Safety_Flash.d \
./Sources/SaftyCheck/SafetyLib/Safety_NVIC.d \
./Sources/SaftyCheck/SafetyLib/Safety_RegCheck.d \
./Sources/SaftyCheck/SafetyLib/Safety_SCST.d \
./Sources/SaftyCheck/SafetyLib/Safety_SRAM.d \
./Sources/SaftyCheck/SafetyLib/Safety_WDOG.d \

OBJS_OS_FORMAT += \
./Sources/SaftyCheck/SafetyLib/Safety_ADC.o \
./Sources/SaftyCheck/SafetyLib/Safety_CMU.o \
./Sources/SaftyCheck/SafetyLib/Safety_CRC.o \
./Sources/SaftyCheck/SafetyLib/Safety_Flash.o \
./Sources/SaftyCheck/SafetyLib/Safety_NVIC.o \
./Sources/SaftyCheck/SafetyLib/Safety_RegCheck.o \
./Sources/SaftyCheck/SafetyLib/Safety_SCST.o \
./Sources/SaftyCheck/SafetyLib/Safety_SRAM.o \
./Sources/SaftyCheck/SafetyLib/Safety_WDOG.o \

OBJS += \
./Sources/SaftyCheck/SafetyLib/Safety_ADC.o \
./Sources/SaftyCheck/SafetyLib/Safety_CMU.o \
./Sources/SaftyCheck/SafetyLib/Safety_CRC.o \
./Sources/SaftyCheck/SafetyLib/Safety_Flash.o \
./Sources/SaftyCheck/SafetyLib/Safety_NVIC.o \
./Sources/SaftyCheck/SafetyLib/Safety_RegCheck.o \
./Sources/SaftyCheck/SafetyLib/Safety_SCST.o \
./Sources/SaftyCheck/SafetyLib/Safety_SRAM.o \
./Sources/SaftyCheck/SafetyLib/Safety_WDOG.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/SaftyCheck/SafetyLib/Safety_ADC.o: ../Sources/SaftyCheck/SafetyLib/Safety_ADC.c
	@echo 'Building file: $<'
	@echo 'Executing target #234 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SaftyCheck/SafetyLib/Safety_ADC.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/SaftyCheck/SafetyLib/Safety_ADC.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SaftyCheck/SafetyLib/Safety_CMU.o: ../Sources/SaftyCheck/SafetyLib/Safety_CMU.c
	@echo 'Building file: $<'
	@echo 'Executing target #235 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SaftyCheck/SafetyLib/Safety_CMU.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/SaftyCheck/SafetyLib/Safety_CMU.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SaftyCheck/SafetyLib/Safety_CRC.o: ../Sources/SaftyCheck/SafetyLib/Safety_CRC.c
	@echo 'Building file: $<'
	@echo 'Executing target #236 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SaftyCheck/SafetyLib/Safety_CRC.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/SaftyCheck/SafetyLib/Safety_CRC.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SaftyCheck/SafetyLib/Safety_Flash.o: ../Sources/SaftyCheck/SafetyLib/Safety_Flash.c
	@echo 'Building file: $<'
	@echo 'Executing target #237 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SaftyCheck/SafetyLib/Safety_Flash.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/SaftyCheck/SafetyLib/Safety_Flash.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SaftyCheck/SafetyLib/Safety_NVIC.o: ../Sources/SaftyCheck/SafetyLib/Safety_NVIC.c
	@echo 'Building file: $<'
	@echo 'Executing target #238 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SaftyCheck/SafetyLib/Safety_NVIC.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/SaftyCheck/SafetyLib/Safety_NVIC.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SaftyCheck/SafetyLib/Safety_RegCheck.o: ../Sources/SaftyCheck/SafetyLib/Safety_RegCheck.c
	@echo 'Building file: $<'
	@echo 'Executing target #239 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SaftyCheck/SafetyLib/Safety_RegCheck.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/SaftyCheck/SafetyLib/Safety_RegCheck.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SaftyCheck/SafetyLib/Safety_SCST.o: ../Sources/SaftyCheck/SafetyLib/Safety_SCST.c
	@echo 'Building file: $<'
	@echo 'Executing target #240 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SaftyCheck/SafetyLib/Safety_SCST.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/SaftyCheck/SafetyLib/Safety_SCST.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SaftyCheck/SafetyLib/Safety_SRAM.o: ../Sources/SaftyCheck/SafetyLib/Safety_SRAM.c
	@echo 'Building file: $<'
	@echo 'Executing target #241 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SaftyCheck/SafetyLib/Safety_SRAM.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/SaftyCheck/SafetyLib/Safety_SRAM.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SaftyCheck/SafetyLib/Safety_WDOG.o: ../Sources/SaftyCheck/SafetyLib/Safety_WDOG.c
	@echo 'Building file: $<'
	@echo 'Executing target #242 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SaftyCheck/SafetyLib/Safety_WDOG.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/SaftyCheck/SafetyLib/Safety_WDOG.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


