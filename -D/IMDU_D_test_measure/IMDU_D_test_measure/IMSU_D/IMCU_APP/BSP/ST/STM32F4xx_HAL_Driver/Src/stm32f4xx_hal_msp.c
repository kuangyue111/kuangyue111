#include "bsp_uart.h"
#include "bsp_can.h"
#include "bsp_adc.h"
#include "bsp_pwm.h"
#include "bsp_capture.h"
#include "bsp_eeprom.h"    
#include "bsp_fsmc.h"   

GPIO_InitTypeDef  GPIO_InitStruct;

#define GPIO_Pin_0                 ((uint16_t)0x0001)  /* Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /* Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /* Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /* Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /* Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /* Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /* Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /* Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /* Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /* Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /* Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /* Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /* Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /* Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /* Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /* Pin 15 selected */
void HAL_TIM_Base_GPIO_MspInit(TIM_HandleTypeDef *htim)
{	
	if (htim->Instance == TIM2)
	{
		//GPIO_InitStruct.Pin = GPIO_Pin_8 | GPIO_Pin_5;  //PA5  ETR
		//GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		//HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  
		//A04			PA5			TIME2_CH1   
		GPIO_InitStruct.Pin = GPIO_Pin_5;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}


	if (htim->Instance == TIM8)
	{
		GPIO_InitStruct.Pin = GPIO_Pin_0;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitStruct.Pin = GPIO_Pin_11;//0x0800;
	GPIO_InitStruct.Mode = 0x02;
	GPIO_InitStruct.Pull = 0x01;
	GPIO_InitStruct.Speed = 0x03;
	GPIO_InitStruct.Alternate = 0x01;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(TIM2_IRQn, 3, 1);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	//GPIO_InitStruct.Pin = GPIO_Pin_5;//0x20;
	//GPIO_InitStruct.Alternate = 0x03;
	//HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	//GPIO_InitStruct.Pin = GPIO_Pin_6;//0x40;
	//HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	//HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 3, 1);
	//HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);  
	//GPIO_InitStruct.Pin = GPIO_Pin_6;//0x40;
	//HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	//HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 3, 1);
	//HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); 
	//GPIO_InitStruct.Pin = GPIO_Pin_7;//0x80;
	//HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	//HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 3, 1);
	//HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn); 
	//GPIO_InitStruct.Pin = GPIO_Pin_9;//0x0200;
	//GPIO_InitStruct.Alternate = 0x09;
	//HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	//HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 3, 1);
	//HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn); 
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
	if (CAN1 == hcan->Instance)
	{
		GPIO_InitStruct.Pin = GPIO_Pin_9;//0x0200;		//GPIO_Pin_9
		GPIO_InitStruct.Mode = 0x02;
		GPIO_InitStruct.Speed = 0x02;
		GPIO_InitStruct.Pull = 0x01;	
		GPIO_InitStruct.Alternate = 0x09;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = GPIO_Pin_8;//0x0100;		//GPIO_Pin_8
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(CANx_RX_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(CANx_RX_IRQn);
	}
	else
	{
		GPIO_InitStruct.Pin = GPIO_Pin_13;	//GPIO_Pin_3
		GPIO_InitStruct.Mode = 0x02;
		GPIO_InitStruct.Speed = 0x02;
		GPIO_InitStruct.Pull = 0x01;
		GPIO_InitStruct.Alternate = 0x09;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = GPIO_Pin_12;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(CAN2_RX_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(CAN2_RX_IRQn);
	}
}
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;      //此行原有

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   //GPIO配置为输出

	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;         //强上拉

	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOB, 6, GPIO_PIN_SET);       //拉高SCL
	HAL_GPIO_WritePin(GPIOB, 7, GPIO_PIN_SET);       //拉高SDA

	hi2c->Instance->CR1 = I2C_CR1_SWRST; //复位 I2C 控制器
	hi2c->Instance->CR1 = 0 ; //解除复位（不会自动清除）


	RCC->AHB1ENR |= 0x00000002;
	GPIO_InitStruct.Pin = GPIO_Pin_6 | GPIO_Pin_7;//0xc0;
	GPIO_InitStruct.Mode = 0x12;
	GPIO_InitStruct.Pull = 0x00;
	GPIO_InitStruct.Speed = 0x02;
	GPIO_InitStruct.Alternate = 0x04;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	RCC->APB1ENR |= 0x00200000;
	RCC->APB1RSTR |= (0x200000);
	RCC->APB1RSTR &= ~(0x200000);

}
void I2C3_MspInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;  
	GPIO_InitStruct.Pin = GPIO_Pin_8;//0x0100;//
	GPIO_InitStruct.Mode = 0x12;
	GPIO_InitStruct.Pull = 0;
	GPIO_InitStruct.Speed = 2;
	GPIO_InitStruct.Alternate = 4;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_Pin_9;//0x0200;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	RCC->APB1RSTR |= 0x800000;
	RCC->APB1RSTR &= ~(0x800000);
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
	if(hi2c->Instance == I2C1)
	{
		GPIO_InitStruct.Pin = GPIO_Pin_6 | GPIO_Pin_7;//0xc0;
		GPIO_InitStruct.Mode = 1;
		GPIO_InitStruct.Pull = 1;
		GPIO_InitStruct.Speed = 2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOB, 0xc0, GPIO_PIN_SET); 
	}
	else if(hi2c->Instance == I2C3)
	{
		GPIO_InitStruct.Pin = GPIO_Pin_8;//0x0100;
		GPIO_InitStruct.Mode = 1;
		GPIO_InitStruct.Pull = 1;
		GPIO_InitStruct.Speed = 2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = GPIO_Pin_9;//0x0200;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOA, 0x0100, GPIO_PIN_SET); 
		HAL_GPIO_WritePin(GPIOC, 0x0200, GPIO_PIN_SET); 
	}
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
	GPIO_InitStruct.Mode      = 0x02;
	GPIO_InitStruct.Pull      = 0x01;
	GPIO_InitStruct.Speed     = 0x03;
	GPIO_InitStruct.Alternate = 0xC;
	GPIO_InitStruct.Pin   = 0xDFB3;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
	GPIO_InitStruct.Pin   = 0xFF83;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); 
	GPIO_InitStruct.Pin   = 0xF03F;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct); 
	GPIO_InitStruct.Pin   = 0x023F;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct); 
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{	
	//UART4:	PC10--TX	PC11--RX
	GPIO_InitStruct.Pin       = GPIO_Pin_10;//0x0400;
	GPIO_InitStruct.Mode      = 0x02;
	GPIO_InitStruct.Pull      = 0x01;
	GPIO_InitStruct.Speed     = 0x02;
	GPIO_InitStruct.Alternate = 0x08;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin       = GPIO_Pin_11;//0x0800;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(UART4_IRQn, 6, 0);
	HAL_NVIC_EnableIRQ(UART4_IRQn);  
	
	//UART5:	PC12-TX	PD2-RX
	GPIO_InitStruct.Pin       = GPIO_Pin_12;//0x1000;
	GPIO_InitStruct.Mode      = 0x02;
	GPIO_InitStruct.Pull      = 0x01;
	GPIO_InitStruct.Speed     = 0x02;
	GPIO_InitStruct.Alternate = 0x08;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_Pin_2;//0x04;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(UART5_IRQn, 6, 1);
	HAL_NVIC_EnableIRQ(UART5_IRQn);

	//UART6:	PC6--TX  PC7--RX
	GPIO_InitStruct.Pin       = GPIO_Pin_6;//0x40;
	GPIO_InitStruct.Mode      = 0x02;
	GPIO_InitStruct.Pull      = 0x01;
	GPIO_InitStruct.Speed     = 0x02;
	GPIO_InitStruct.Alternate = 0x08;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_Pin_7;//0x80;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(USART6_IRQn, 6, 2);
	HAL_NVIC_EnableIRQ(USART6_IRQn);

	//UART1	PA10--RX PA9--TX
	GPIO_InitStruct.Pin = GPIO_Pin_10;
	GPIO_InitStruct.Mode      = 0x02;
	GPIO_InitStruct.Pull      = 0x01;
	GPIO_InitStruct.Speed     = 0x02;
	GPIO_InitStruct.Alternate = 0x07;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_Pin_9;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(USART1_IRQn, 6, 3);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	//UART2 PD6--RX
	GPIO_InitStruct.Pin = GPIO_Pin_6;
	GPIO_InitStruct.Mode      = 0x02;
	GPIO_InitStruct.Pull      = 0x01;
	GPIO_InitStruct.Speed     = 0x02;
	GPIO_InitStruct.Alternate = 0x07;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(USART2_IRQn, 6, 4);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	static DMA_HandleTypeDef  hdma_adc;
	static DMA_HandleTypeDef  hdma_adc_3;

	if (ADC1 == hadc->Instance)
	{
		extern unsigned char  u8Is1000VHardwareA00(void);
		if(u8Is1000VHardwareA00())
		{
			GPIO_InitStruct.Pin =  GPIO_Pin_4 | GPIO_Pin_6;												
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP | GPIO_MODE_AF_PP;								
			GPIO_InitStruct.Pull = 0x00;
		}
		else
		{
			GPIO_InitStruct.Pin =  GPIO_Pin_4 | GPIO_Pin_5  | GPIO_Pin_6 | GPIO_Pin_0;												
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP | GPIO_MODE_AF_PP;								
			GPIO_InitStruct.Pull = 0x00;

		}
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


		GPIO_InitStruct.Pin = GPIO_Pin_0;															//PB0		ad_curr
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		if(u8Is1000VHardwareA00())
		{
			GPIO_InitStruct.Pin = GPIO_Pin_0 | GPIO_Pin_2;								
		}
		else
		{
			GPIO_InitStruct.Pin = GPIO_Pin_0;									
		}

		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  
	
		hdma_adc.Instance = DMA2_Stream0;
		hdma_adc.Init.Channel  = DMA_CHANNEL_0;
		hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_adc.Init.Mode = DMA_CIRCULAR;					//0x0100;
		hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;			//0x020000;
		hdma_adc.Init.FIFOMode = DMA_FIFOMODE_DISABLE;         
		hdma_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
		hdma_adc.Init.MemBurst = DMA_MBURST_SINGLE;
		hdma_adc.Init.PeriphBurst = DMA_PBURST_SINGLE; 
		HAL_DMA_Init(&hdma_adc);
		__HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);
		HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 6, 1);   
		HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	}
	else if (ADC3 == hadc->Instance)
	{
		//A04
		GPIO_InitStruct.Pin = GPIO_Pin_9;									//A04	PA5  PA6 用于输入捕捉功能！
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP | GPIO_MODE_AF_PP;								//0x03;
		GPIO_InitStruct.Pull = 0x00;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

		GPIO_InitStruct.Pin =  GPIO_Pin_8 ;												
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG ;								
		GPIO_InitStruct.Pull = 0x00;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

		hdma_adc_3.Instance = DMA2_Stream1;
		hdma_adc_3.Init.Channel  = DMA_CHANNEL_2;
		hdma_adc_3.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc_3.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc_3.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc_3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_adc_3.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_adc_3.Init.Mode = DMA_CIRCULAR;								//0x0100;
		hdma_adc_3.Init.Priority = DMA_PRIORITY_HIGH;						//0x020000;
		hdma_adc_3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;					//0;         
		hdma_adc_3.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;		//1;
		hdma_adc_3.Init.MemBurst = DMA_MBURST_SINGLE;						//0;
		hdma_adc_3.Init.PeriphBurst = DMA_PBURST_SINGLE;					//0; 
		HAL_DMA_Init(&hdma_adc_3);
		__HAL_LINKDMA(hadc, DMA_Handle, hdma_adc_3);
		HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 6, 1);   
		HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
	}
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitStruct.Mode = 0x02;
  GPIO_InitStruct.Pull = 0x01;
  GPIO_InitStruct.Speed = 0x03;
  GPIO_InitStruct.Alternate = 0x02;
  GPIO_InitStruct.Pin = GPIO_Pin_8; //0x0100;	PC8 PWMG2		PF8 PWMG2  
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
 // GPIO_InitStruct.Alternate = 0x09;
 // HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

