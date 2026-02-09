################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/BoardDriver.c" \
"../Sources/main.c" \
"../Sources/rtos.c" \

C_SRCS += \
../Sources/BoardDriver.c \
../Sources/main.c \
../Sources/rtos.c \

OBJS_OS_FORMAT += \
./Sources/BoardDriver.o \
./Sources/main.o \
./Sources/rtos.o \

C_DEPS_QUOTED += \
"./Sources/BoardDriver.d" \
"./Sources/main.d" \
"./Sources/rtos.d" \

OBJS += \
./Sources/BoardDriver.o \
./Sources/main.o \
./Sources/rtos.o \

OBJS_QUOTED += \
"./Sources/BoardDriver.o" \
"./Sources/main.o" \
"./Sources/rtos.o" \

C_DEPS += \
./Sources/BoardDriver.d \
./Sources/main.d \
./Sources/rtos.d \


# Each subdirectory must supply rules for building sources it contributes
Sources/BoardDriver.o: ../Sources/BoardDriver.c
	@echo 'Building file: $<'
	@echo 'Executing target #91 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/BoardDriver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/BoardDriver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #92 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/main.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/rtos.o: ../Sources/rtos.c
	@echo 'Building file: $<'
	@echo 'Executing target #93 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/rtos.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/rtos.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


