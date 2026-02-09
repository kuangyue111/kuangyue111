
#include "bsp_capture.h"
#include "main.h"


/* Timer handler declaration */
TIM_HandleTypeDef      Tim2Handle;
TIM_HandleTypeDef      Tim9Handle;
TIM_HandleTypeDef      Tim10Handle;
TIM_HandleTypeDef      Tim11Handle;
TIM_HandleTypeDef      Tim14Handle;
TIM_HandleTypeDef      Tim5Handle;
TIM_HandleTypeDef	   Tim7Handle;
TIM_HandleTypeDef	   Tim8Handle;

/* Timer Input Capture Configuration Structure declaration */
TIM_IC_InitTypeDef     sICConfig;
TIM_ClockConfigTypeDef	sTIMClockConfig;


/* Private function prototypes -----------------------------------------------*/
int bsp_Capture1Init(void);
int bsp_Capture23Init(void);
int bsp_Capture4Init(void);
int bsp_Capture5Init(void);
int bsp_Capture6Init(void);

/* Private functions ---------------------------------------------------------*/
static int bsp_VF_Init(void)
{
	if(!u8Is1000VHardwareA00())
	{
		return HAL_ERROR;
	}

	Tim2Handle.Instance = TIM2;
	//  1/(Tclk) * XY = T
	Tim2Handle.Init.Period        = 0xffff;
	Tim2Handle.Init.Prescaler     = 0;
	Tim2Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim2Handle.Init.CounterMode   = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&Tim2Handle);

	sTIMClockConfig.ClockFilter = 0;
	sTIMClockConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
	sTIMClockConfig.ClockPolarity = TIM_CLOCKPOLARITY_RISING;
	sTIMClockConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
	HAL_TIM_ConfigClockSource(&Tim2Handle,&sTIMClockConfig);
	HAL_TIM_Base_Start(&Tim2Handle);


	Tim8Handle.Instance = TIM8;
	//  1/(Tclk) * XY = T
	Tim8Handle.Init.Period        = 0xffff;
	Tim8Handle.Init.Prescaler     = 0;
	Tim8Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim8Handle.Init.CounterMode   = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&Tim8Handle);
	sTIMClockConfig.ClockFilter = 0;
	sTIMClockConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
	sTIMClockConfig.ClockPolarity = TIM_CLOCKPOLARITY_RISING;
	sTIMClockConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
	HAL_TIM_ConfigClockSource(&Tim8Handle,&sTIMClockConfig);
	HAL_TIM_Base_Start(&Tim8Handle);
	return HAL_OK;
}

/*bsp_CaptureInit*/
int BSP_CaptureInit(void)
{
  uint8_t i=0;
  
  if(bsp_VF_Init() !=  HAL_OK)
  {
	  if(bsp_Capture1Init() !=  HAL_OK)
	  {
		  return HAL_ERROR;
	  }
	  else
	  {
		return HAL_ERROR;
	  }
  }
 
  
  //if(bsp_Capture23Init() !=  HAL_OK)
  //{
  //  return HAL_ERROR;
  //}
  
  //if(bsp_Capture4Init() !=  HAL_OK)
  //{
  //  return HAL_ERROR;
  //}
  //
  //if(bsp_Capture5Init() !=  HAL_OK)
  //{
  //  return HAL_ERROR;
  //}
  
  //if(bsp_Capture6Init() !=  HAL_OK)
  //{
  //  return HAL_ERROR;
  //}
  
  return HAL_OK;  
}

/*bsp_Capture1Init*/
int bsp_Capture1Init(void)
{
  Tim2Handle.Instance = TIM2;
  Tim2Handle.Init.Period        = 0xFFFF;
  Tim2Handle.Init.Prescaler     = 0;
  Tim2Handle.Init.ClockDivision = 0;
  Tim2Handle.Init.CounterMode   = 0;  
  if(HAL_TIM_IC_Init(&Tim2Handle) != HAL_OK)
  {
    return HAL_ERROR;
  }
  sICConfig.ICPolarity  = 0;
  sICConfig.ICSelection = 1;
  sICConfig.ICPrescaler = 0;
  sICConfig.ICFilter    = 0;   
  if(HAL_TIM_IC_ConfigChannel(&Tim2Handle, &sICConfig, TIM_CHANNEL_4) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if(HAL_TIM_IC_Start_IT(&Tim2Handle, TIM_CHANNEL_4) != HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}

/*bsp_Capture2Init*/
int bsp_Capture23Init(void)
{
	Tim9Handle.Instance = TIM9;
	Tim9Handle.Init.Period        = 0xFFFF;
	Tim9Handle.Init.Prescaler     = 0;
	Tim9Handle.Init.ClockDivision = 0;
	Tim9Handle.Init.CounterMode   = 0;  
	if(HAL_TIM_IC_Init(&Tim9Handle) != HAL_OK)
	{
		return HAL_ERROR;
	}
	sICConfig.ICPolarity  = 0;
	sICConfig.ICSelection = 1;
	sICConfig.ICPrescaler = 0;
	sICConfig.ICFilter    = 0;   
	if(HAL_TIM_IC_ConfigChannel(&Tim9Handle, &sICConfig, TIM_CHANNEL_1) != HAL_OK)
	{
		return HAL_ERROR;
	}
	if(HAL_TIM_IC_ConfigChannel(&Tim9Handle, &sICConfig, TIM_CHANNEL_2) != HAL_OK)
	{
		return HAL_ERROR;
	}
	
	if(HAL_TIM_IC_Start_IT(&Tim9Handle, TIM_CHANNEL_1) != HAL_OK)
	{
		return HAL_ERROR;
	}
	if(HAL_TIM_IC_Start_IT(&Tim9Handle, TIM_CHANNEL_2) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}


void BSP_TIM_BaseInit(void)
{
	Tim5Handle.Instance = TIM5;
	//  1/(Tclk) * XY = T
	Tim5Handle.Init.Period        = (PERIOD_TIME_5 - 1);
	Tim5Handle.Init.Prescaler     = (PRESCALER_TIME_5 - 1);
	Tim5Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim5Handle.Init.CounterMode   = TIM_COUNTERMODE_UP;

	HAL_TIM_Base_Init(&Tim5Handle);
	HAL_TIM_Base_Start_IT(&Tim5Handle);

	if(u8Is1000VHardwareA00()== FALSE)
	{
		return;
	}
	Tim7Handle.Instance = TIM7;
	//  1/(Tclk) * XY = T
	Tim7Handle.Init.Period        = (PERIOD_TIME_7 - 1);
	Tim7Handle.Init.Prescaler     = (PRESCALER_TIME_7 - 1);
	Tim7Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim7Handle.Init.CounterMode   = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&Tim7Handle);
	HAL_TIM_Base_Start_IT(&Tim7Handle);
}

/*bsp_Capture4Init*/
int bsp_Capture4Init(void)
{
	Tim10Handle.Instance = TIM10;
	Tim10Handle.Init.Period        = 0xFFFF;
	Tim10Handle.Init.Prescaler     = 0;
	Tim10Handle.Init.ClockDivision = 0;
	Tim10Handle.Init.CounterMode   = 0;  
	if(HAL_TIM_IC_Init(&Tim10Handle) != HAL_OK)
	{
		return HAL_ERROR;
	}
	sICConfig.ICPolarity  = 0;
	sICConfig.ICSelection = 1;
	sICConfig.ICPrescaler = 0;
	sICConfig.ICFilter    = 0;   
	if(HAL_TIM_IC_ConfigChannel(&Tim10Handle, &sICConfig, TIM_CHANNEL_1) != HAL_OK)
	{
		return HAL_ERROR;
	}
	if(HAL_TIM_IC_Start_IT(&Tim10Handle, TIM_CHANNEL_1) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

/*bsp_Capture5Init*/
int bsp_Capture5Init(void)
{
  Tim11Handle.Instance = TIM11;
  Tim11Handle.Init.Period        = 0xFFFF;
  Tim11Handle.Init.Prescaler     = 0;
  Tim11Handle.Init.ClockDivision = 0;
  Tim11Handle.Init.CounterMode   = 0;  
  if(HAL_TIM_IC_Init(&Tim11Handle) != HAL_OK)
  {
    return HAL_ERROR;
  }
  sICConfig.ICPolarity  = 0;
  sICConfig.ICSelection = 1;
  sICConfig.ICPrescaler = 0;
  sICConfig.ICFilter    = 0;   
  if(HAL_TIM_IC_ConfigChannel(&Tim11Handle, &sICConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  if(HAL_TIM_IC_Start_IT(&Tim11Handle, TIM_CHANNEL_1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}

/*bsp_Capture6Init*/
int bsp_Capture6Init(void)
{
  Tim14Handle.Instance = TIM14;
  Tim14Handle.Init.Period        = 0xFFFF;
  Tim14Handle.Init.Prescaler     = 0;
  Tim14Handle.Init.ClockDivision = 0;
  Tim14Handle.Init.CounterMode   = 0;  
  if(HAL_TIM_IC_Init(&Tim14Handle) != HAL_OK)
  {
    return HAL_ERROR;
  }
  sICConfig.ICPolarity  = 0;
  sICConfig.ICSelection = 1;
  sICConfig.ICPrescaler = 0;
  sICConfig.ICFilter    = 0;   
  if(HAL_TIM_IC_ConfigChannel(&Tim14Handle, &sICConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  if(HAL_TIM_IC_Start_IT(&Tim14Handle, TIM_CHANNEL_1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if ((htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) &&(htim->Instance == TIM9))
	{
		 dv_Capture_PWM.u16PulseCnt[AC_PWM_FREQUENCY]++;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static UINT16 count =0;
        count ++;
	if(htim->Instance == TIM5)
	{
		BSP_Base_TIME_Deal();
	}
	else if(htim->Instance == TIM7)
	{
		dv_Capture_PWM.u16PulseCnt[BUS_VOLT_1_SQC_NO] = (UINT16)(Tim2Handle.Instance->CNT);
		dv_Capture_PWM.u16PulseCnt[BUS_VOLT_2_SQC_NO] = (UINT16)(Tim8Handle.Instance->CNT);
		dv_Capture_PWM.fGetInputPulse();
	}
	
}