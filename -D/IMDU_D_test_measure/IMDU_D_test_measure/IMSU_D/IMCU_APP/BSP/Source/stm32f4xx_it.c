/**
  ******************************************************************************
  * @file    BSP/Src/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    26-June-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp_adc.h"  
#include "stm32f4xx_it.h"
#include "bsp_uart.h"
#include "bsp_capture.h"    
#include "bsp_can.h"   
#include "bsp_eeprom.h" 
#include "bsp_dido.h"
/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef    heval_I2c;
extern UART_HandleTypeDef   Uart1Handle;
extern UART_HandleTypeDef   Uart4Handle;
extern UART_HandleTypeDef   Uart5Handle;
extern UART_HandleTypeDef   Uart6Handle;
extern TIM_HandleTypeDef    Tim2Handle;
extern TIM_HandleTypeDef    Tim9Handle;
extern TIM_HandleTypeDef    Tim5Handle;
extern TIM_HandleTypeDef    Tim10Handle;
extern TIM_HandleTypeDef    Tim11Handle;
extern TIM_HandleTypeDef    Tim14Handle;
extern CAN_HandleTypeDef    Can_1_Handle;
//extern CAN_HandleTypeDef    Can_2_Handle;
extern I2C_HandleTypeDef    heval_I2c;
extern ADC_HandleTypeDef    AdcHandle;
extern ADC_HandleTypeDef    ADC_3_Handle;
//extern HCD_HandleTypeDef    hhcd;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
  //Toggle_Leds();
  //BSP_vFeedDogLed();
}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles UART interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream 
  *         used for USART data transmission     
  */
void USART6_IRQHandler(void)
{
}

void UART4_IRQHandler(void)
{
	uint32_t tmp1 = 0, tmp2 = 0;
        tmp1 = 0;
        tmp2 = 0;
	UNUSED(tmp1);
	UNUSED(tmp2);
	return;
}
void UART5_IRQHandler(void)
{
	uint16_t tmp;
        tmp = 0;
	UNUSED(tmp);
}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
}
void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}
/**
  * @brief  This function handles External line 2 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void ADCx_DMA_IRQHandler(void)
{
  HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
}
/**
  * @brief  This function handles External line 15_10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
}

/**
  * @brief  This function handles sEE DMA TX interrupt request.
  * @param  None
  * @retval None
  */
void EEPROM_I2C_DMA_TX_IRQHandler(void)
{ 
  HAL_DMA_IRQHandler(heval_I2c.hdmatx);
}

/**
  * @brief  This function handles sEE DMA RX interrupt request.
  * @param  None
  * @retval None
  */
void EEPROM_I2C_DMA_RX_IRQHandler(void)
{ 
  HAL_DMA_IRQHandler(heval_I2c.hdmarx);
}

void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&Tim2Handle);

}
void TIM1_BRK_TIM9_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&Tim9Handle);
}
void TIM1_UP_TIM10_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&Tim10Handle);
}
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&Tim11Handle);
}
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&Tim14Handle);
}
/**
* @brief  This function handles CAN1 RX0 interrupt request.
* @param  None
* @retval None
*/
void CAN1_RX0_IRQHandler(void)
{
}


/**
* @brief  This function handles CAN2 RX0 interrupt request.
* @param  None
* @retval None
*/
void CAN2_RX0_IRQHandler(void)
{
}

/**
* @brief  This function handles CAN1 RX1 interrupt request.
* @param  None
* @retval None
*/
void CAN1_RX1_IRQHandler(void)
{
}


/**
* @brief  This function handles CAN2 RX1 interrupt request.
* @param  None
* @retval None
*/
void CAN2_RX1_IRQHandler(void)
{
}

/**
* @brief  This function handles CAN1 TX interrupt request.
* @param  None
* @retval None
*/
void CAN1_TX_IRQHandler(void)
{
}

/**
* @brief  This function handles CAN2 TX interrupt request.
* @param  None
* @retval None
*/
void CAN2_TX_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM1 global interrupt request.
  * @param  None
  * @retval None
  */

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */ 

/**
  * @}
  */
void OTG_FS_IRQHandler(void)
{
  //HAL_HCD_IRQHandler(&hhcd);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
