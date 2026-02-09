#include "bsp_pwm.h"
#include  "main.h"
TIM_HandleTypeDef    Tim3Handle;
TIM_HandleTypeDef    Tim13Handle;
TIM_OC_InitTypeDef sConfig;

int BSP_Pwm1Start(void);
int BSP_Pwm2Start(void);

/*PWM1*/
int BSP_Pwm1Init(void)
{
	Tim3Handle.Instance = TIM3;
	Tim3Handle.Init.Prescaler = 11;
	Tim3Handle.Init.Period = 1399;
	Tim3Handle.Init.ClockDivision = 0;
	Tim3Handle.Init.CounterMode = 0x00;
	if(HAL_TIM_PWM_Init(&Tim3Handle) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

int BSP_SetPwm1Duty(uint8_t DutyCycles)
{
	if(DutyCycles>100)
	{
	    return HAL_ERROR;
	}
	sConfig.OCMode = 0x60;
	sConfig.OCFastMode = 0x00;

	if (gEvChargeInfo.u8PWMPolarityCover)
	{
		sConfig.OCPolarity = 0x02;
	}
	else
	{
		sConfig.OCPolarity = 0x00;//0x00;	0x02脉冲反向
	}
	
	sConfig.Pulse = ((uint32_t)DutyCycles*14);
	if(HAL_TIM_PWM_ConfigChannel(&Tim3Handle, &sConfig, 0x08) != HAL_OK)
	{
	  return HAL_ERROR;
	}
	BSP_Pwm1Start();
	return HAL_OK;
}

int BSP_Pwm1Start(void)
{
	if(HAL_TIM_PWM_Start(&Tim3Handle, 0x08) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}
/*PWM2*/
int BSP_Pwm2Init(void)
{
	Tim13Handle.Instance = TIM13;
	Tim13Handle.Init.Prescaler = 1;	//2分频
	Tim13Handle.Init.Period = 419;	//调节频率

	Tim13Handle.Init.ClockDivision = 0;
	Tim13Handle.Init.CounterMode = 0x00;
	if(HAL_TIM_PWM_Init(&Tim13Handle) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

int BSP_SetPwm2Duty(uint8_t DutyCycles)
{
	if(DutyCycles>100)
	{
		return HAL_ERROR;
	}
	sConfig.OCMode = 0x60;

	if (gEvChargeInfo.u8PWMPolarityCover)
	{
		sConfig.OCPolarity = 0x02;
	}
	else
	{
		sConfig.OCPolarity = 0x00;//0x00;	0x02脉冲反向
	}

	sConfig.OCFastMode = 0x00;
	
	//sConfig.Pulse = ((uint32_t)DutyCycles*1);
	//sConfig.Pulse = ((uint32_t)699);
	sConfig.Pulse = (uint32_t)((uint32_t)DutyCycles*4.2);//调节占空比
	if(HAL_TIM_PWM_ConfigChannel(&Tim13Handle, &sConfig, 0x00) != HAL_OK)
	{
		return HAL_ERROR;
	}
	BSP_Pwm2Start();
	return HAL_OK;
}

int BSP_Pwm2Start(void)
{
	if(HAL_TIM_PWM_Start(&Tim13Handle, 0x00) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}
