#ifndef __BSP_PWM_H
#define __BSP_PWM_H
#include "stm32f4xx_hal.h"



int BSP_Pwm1Init(void);
int BSP_SetPwm1Duty(uint8_t DutyCycles);
int BSP_Pwm2Init(void);
int BSP_SetPwm2Duty(uint8_t DutyCycles);
#endif
