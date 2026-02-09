################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 93
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Generated_Code/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include SDK/platform/devices/S32K146/startup/subdir.mk
-include SDK/platform/devices/subdir.mk
-include SDK/platform/drivers/src/adc/subdir.mk
-include SDK/platform/drivers/src/clock/S32K1xx/subdir.mk
-include SDK/platform/drivers/src/edma/subdir.mk
-include SDK/platform/drivers/src/ewm/subdir.mk
-include SDK/platform/drivers/src/flexcan/subdir.mk
-include SDK/platform/drivers/src/flexio/subdir.mk
-include SDK/platform/drivers/src/ftm/subdir.mk
-include SDK/platform/drivers/src/interrupt/subdir.mk
-include SDK/platform/drivers/src/lpi2c/subdir.mk
-include SDK/platform/drivers/src/lpit/subdir.mk
-include SDK/platform/drivers/src/lpspi/subdir.mk
-include SDK/platform/drivers/src/lptmr/subdir.mk
-include SDK/platform/drivers/src/lpuart/subdir.mk
-include SDK/platform/drivers/src/pdb/subdir.mk
-include SDK/platform/drivers/src/pins/subdir.mk
-include SDK/platform/drivers/src/trgmux/subdir.mk
-include SDK/platform/drivers/src/wdog/subdir.mk
-include SDK/platform/pal/src/adc/subdir.mk
-include SDK/platform/pal/src/can/subdir.mk
-include SDK/platform/pal/src/i2c/subdir.mk
-include SDK/platform/pal/src/ic/subdir.mk
-include SDK/platform/pal/src/pwm/subdir.mk
-include SDK/platform/pal/src/spi/subdir.mk
-include SDK/platform/pal/src/timing/subdir.mk
-include SDK/platform/pal/src/uart/subdir.mk
-include SDK/platform/pal/src/wdg/subdir.mk
-include SDK/rtos/FreeRTOS_S32K/Source/subdir.mk
-include SDK/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/subdir.mk
-include SDK/rtos/FreeRTOS_S32K/Source/portable/MemMang/subdir.mk
-include SDK/rtos/osif/subdir.mk
-include Sources/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
BMU_DEBUG_146.elf \

EXECUTABLES_QUOTED += \
"BMU_DEBUG_146.elf" \

EXECUTABLES_OS_FORMAT += \
BMU_DEBUG_146.elf \

SECONDARY_SIZE += \
BMU_DEBUG_146.siz \

SECONDARY_SIZE_QUOTED += \
"BMU_DEBUG_146.siz" \

SECONDARY_SIZE_OS_FORMAT += \
BMU_DEBUG_146.siz \


# All Target
all: warning-messages explicit-dependencies BMU_DEBUG_146.elf secondary-outputs

# Tool invocations
BMU_DEBUG_146.elf: $(OBJS) D:/NXP/BMU_DEBUG_146/BMU_DEBUG_146/Project_Settings/Linker_Files/S32K146_128_flash.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #94 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "BMU_DEBUG_146.elf" "@BMU_DEBUG_146.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

BMU_DEBUG_146.siz: $(EXECUTABLES)
	@echo 'Executing target #95 $@'
	@echo 'Invoking: Standard S32DS Print Size'
	arm-none-eabi-size --format=berkeley $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*/*.d ./*/*/*/*.d ./*/*/*/*/*/*/*.d ./*/*/*/*/*/*/*/*.d ./*/*/*/*/*.d ./*/*/*/*/*/*.d  ./*/*.o ./*/*/*/*.o ./*/*/*/*/*/*/*.o ./*/*/*/*/*/*/*/*.o ./*/*/*/*/*.o ./*/*/*/*/*/*.o ./*/*/*.o  $(SECONDARY_SIZE_QUOTED) $(EXECUTABLES_QUOTED)
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 

