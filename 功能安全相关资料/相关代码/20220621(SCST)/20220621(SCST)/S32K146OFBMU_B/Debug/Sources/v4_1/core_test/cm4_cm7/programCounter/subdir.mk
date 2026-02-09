################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS_QUOTED += \
"../Sources/v4_1/core_test/cm4_cm7/programCounter/iec60730b_cm4_cm7_pc_object.S" \

S_UPPER_SRCS += \
../Sources/v4_1/core_test/cm4_cm7/programCounter/iec60730b_cm4_cm7_pc_object.S \

OBJS_QUOTED += \
"./Sources/v4_1/core_test/cm4_cm7/programCounter/iec60730b_cm4_cm7_pc_object.o" \

OBJS_OS_FORMAT += \
./Sources/v4_1/core_test/cm4_cm7/programCounter/iec60730b_cm4_cm7_pc_object.o \

OBJS += \
./Sources/v4_1/core_test/cm4_cm7/programCounter/iec60730b_cm4_cm7_pc_object.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/v4_1/core_test/cm4_cm7/programCounter/iec60730b_cm4_cm7_pc_object.o: ../Sources/v4_1/core_test/cm4_cm7/programCounter/iec60730b_cm4_cm7_pc_object.S
	@echo 'Building file: $<'
	@echo 'Executing target #245 $<'
	@echo 'Invoking: Standard S32DS Assembler'
	arm-none-eabi-gcc "@Sources/v4_1/core_test/cm4_cm7/programCounter/iec60730b_cm4_cm7_pc_object.args" -o "Sources/v4_1/core_test/cm4_cm7/programCounter/iec60730b_cm4_cm7_pc_object.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


