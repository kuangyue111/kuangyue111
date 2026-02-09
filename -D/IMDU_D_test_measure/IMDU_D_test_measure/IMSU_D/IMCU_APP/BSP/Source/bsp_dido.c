#include "bsp_dido.h"
#include "bsp_fsmc.h"
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
uint8_t g_u8LvdStatusBuk=0;
static GPIO_InitTypeDef  GPIO_InitStruct;
IWDG_HandleTypeDef IwdgHandle;

void BSP_LvdCtrl(LvdChannel_TypeDef u8LvdChannel,LvdStatus_TypeDef u8LvdStatus)
{
	if((u8LvdChannel > LVD2 )||(u8LvdChannel < LVD1 )||(u8LvdStatus > LVD_ON))
	{
		return;
	}
	g_u8LvdStatusBuk &=(~((uint8_t)0x01<<u8LvdChannel));
	g_u8LvdStatusBuk |= (u8LvdStatus <<u8LvdChannel);
	g_u8LvdStatusBuk &=0xc0;
	if(u8LvdChannel == LVD1)
	{
		HAL_GPIO_WritePin(GPIOG,GPIO_Pin_12,(GPIO_PinState)u8LvdStatus);//0x1000 GPIO_Pin_12
	}
	else if(u8LvdChannel == LVD2)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_Pin_13,(GPIO_PinState)u8LvdStatus);//0x2000 GPIO_Pin_13
	}
}

/****************************************************************************************************
	协议中控制和上报DO序号      电路注释		 对应凤凰端子输出
	DO1				--	CRELAY1     		-- 母排A---  j6-1     
	DO2				--	CRELAY2     		-- 母排B---  j6-5 
	DO3				--	CRELAY3     		-- A电子锁方向 未引出。
	DO4				--	CRELAY4     		-- 并机 ---  j6-2 
	DO5				--	CRELAY5     		-- A电子锁控制---J12-24
	DO6				--	CRELAY6     		-- B电子锁控制---J12-68
	DO9	    		--	CRELAY9     		-- B枪 BMS供电---J3-68脚
	DO10			--	CRELAY10    		-- 电源、预约指示灯---J12-57脚
	DO11    		--	CRELAY11    		-- A枪BMS电源选择---J3-13脚
	DO12			--	CRELAY12    		-- B枪BMS电源选择---J3-57脚
	DO13			--	CRELAY13    		-- A充电灯---J13-24脚
	DO14			--	CRELAY14    		-- A告警灯---J13-68脚
	DO7	    		--	CRELAY7     		-- B枪电子锁方向---未引出
	DO8				--	CRELAY8     		-- A枪 BMS供电---J3-24脚
	DO15			--	CRELAY15    		-- B充电灯---J13-13脚
	DO16			--	CRELAY16			-- B告警灯---J13-57脚
******************************************************************************************************/
void BSP_DO_Ctrl(NEWDoChannel_TypeDef iDONo, DoStatus_TypeDef u8DoStatus)
{
	static uint8_t g_u8DoStatusBuk=0;
	static uint8_t g_u8DoStatusBuk_2=0;
	if((iDONo > NEWDO16 )||(u8DoStatus > DO_ON))
	{
		return;
	}
	else
	{
		if (NEWDO1 == iDONo || NEWDO2 == iDONo || NEWDO3 == iDONo || NEWDO4 == iDONo || NEWDO5 == iDONo || NEWDO6 == iDONo || NEWDO7 == iDONo || NEWDO8 == iDONo)
		{
			g_u8DoStatusBuk &=(~((uint8_t)0x01<<iDONo));
			g_u8DoStatusBuk |= (u8DoStatus <<iDONo);
			CSDO1 = g_u8DoStatusBuk;
		}
		if (NEWDO9 == iDONo || NEWDO10 == iDONo || NEWDO11 == iDONo || NEWDO12 == iDONo || NEWDO13 == iDONo || NEWDO14 == iDONo || NEWDO15 == iDONo || NEWDO16 == iDONo)
		{	
			g_u8DoStatusBuk_2 &=(~((uint8_t)0x01<<(iDONo-NEWDO9)));
			g_u8DoStatusBuk_2 |= (u8DoStatus <<(iDONo-NEWDO9));
			CSDO2 = g_u8DoStatusBuk_2;
		}
	}
}

void BSP_InitGpioF(void)
{
	GPIO_InitStruct.Pin = 0;			//0x2048
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;	//0x01;	输出推挽
	GPIO_InitStruct.Pull = 0x01;			//上拉！
	GPIO_InitStruct.Speed = 0x02;
	/***********************************************************
	PF	0x0400
		GPIO_Pin_10                ((uint16_t)0x0400) UART6-CTL 实际是控制的UART5
	***********************************************************/
	GPIO_InitStruct.Pin = GPIO_Pin_10;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}
void BSP_1000VClockInit(void)
{
	__ADC3_CLK_ENABLE();
	__TIM8_CLK_ENABLE();
	__TIM7_CLK_ENABLE();
	__DMA1_CLK_ENABLE();
}

void BSP_vCLOCKInit(void)
{
	__USART1_CLK_ENABLE();

	__USART2_CLK_ENABLE();
	//TXY
	//RCC->APB2ENR |= 0x00000020;	
	__USART6_CLK_ENABLE();

	//RCC->APB2ENR |= 0x00000100;	
	__ADC1_CLK_ENABLE();

	//RCC->APB1ENR |= 0x00000002;	
	__TIM3_CLK_ENABLE();

	__TIM5_CLK_ENABLE();

	//RCC->APB1ENR |= 0x00000080;	
	__TIM13_CLK_ENABLE();

	//RCC->APB1ENR |= 0x00000001;
	__TIM2_CLK_ENABLE();

	//RCC->APB2ENR |= 0x00010000;
	__TIM9_CLK_ENABLE();

	//RCC->APB2ENR |= 0x00020000;
	__TIM10_CLK_ENABLE();

	//RCC->APB2ENR |= 0x00040000;
	__TIM11_CLK_ENABLE();

	//RCC->APB1ENR |= 0x00000100;
	__TIM14_CLK_ENABLE();

	//RCC->APB1ENR |= 0x02000000;
	__CAN1_CLK_ENABLE();

	//TXY 
	__CAN2_CLK_ENABLE();		//(RCC->APB1ENR |= (RCC_APB1ENR_CAN2EN))

	//RCC->APB1ENR |= 0x00200000;
	__I2C1_CLK_ENABLE();

	//RCC->AHB1ENR |= 0x00000001;
	__GPIOA_CLK_ENABLE();

	//RCC->AHB1ENR |= 0x00000002;
	__GPIOB_CLK_ENABLE();

	//RCC->AHB1ENR |= 0x00000004;
	__GPIOC_CLK_ENABLE();

	//RCC->AHB1ENR |= 0x00000008;
	__GPIOD_CLK_ENABLE();

	//RCC->AHB1ENR |= 0x00000010;
	__GPIOE_CLK_ENABLE();

	//RCC->AHB1ENR |= 0x00000020;
	__GPIOF_CLK_ENABLE();

	//RCC->AHB1ENR |= 0x00000040;
	__GPIOG_CLK_ENABLE();

	//RCC->AHB1ENR |= 0x00400000;
	__DMA2_CLK_ENABLE();

	//RCC->AHB1ENR |= 0x02000000;
	__ETHMACEN_CLK_ENABLE();

	//RCC->AHB1ENR |= 0x04000000;
	__ETHMACTXEN_CLK_ENABLE();

	//RCC->AHB1ENR |= 0x08000000;
	__ETHMACRXEN_CLK_ENABLE();

	//RCC->AHB3ENR |= 0x01;
	__FSMC_CLK_ENABLE();//Note #define STM32F407xx	1

	//RCC->APB1ENR |= 0x00080000;
	__UART4E_CLK_ENABLE();

	//RCC->APB1ENR |= 0x00100000;
	__UART5E_CLK_ENABLE();

	//RCC->APB1ENR |= 0x00800000;
	__I2C3_CLK_ENABLE();

	__SPI3_CLK_ENABLE();
}

void BSP_vGpioEXIT1Init(void)
{
	unsigned long   PinVal;
	PinVal = GPIO_Pin_1;	
	__GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin = PinVal;
	//PB1
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;				//下降沿触发    因为有数据了  MCP2515是给了一个低电平
	GPIO_InitStruct.Pull = GPIO_NOPULL;							//上拉！
	GPIO_InitStruct.Speed = 0x02;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI1_IRQn, 3, 1);			//ISR_CIM_EXTI1
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}
void BSP_vGpioEXIT5_Init(void)
{
	unsigned long	PinVal;
	PinVal = GPIO_Pin_5;	
	__GPIOE_CLK_ENABLE();
	GPIO_InitStruct.Pin = PinVal;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;				//上升沿触发
	GPIO_InitStruct.Pull = GPIO_NOPULL;	
	GPIO_InitStruct.Speed = 0x02;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 3, 2);					//ISR_CIM_EXTI9_5
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void HAL_GPIO_EXTI_5_Callback(uint16_t GPIO_Pin)
{
	return;
}
void BSP_vGpioReInit(void)
{
	/***************************************************************************************************
				OUTPUT
	***************************************************************************************************/
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;								//0x01;	输出推挽
	GPIO_InitStruct.Pull = 0x01;											//上拉！
	GPIO_InitStruct.Speed = 0x02;
	//GPIO_InitStruct.Pin = GPIO_Pin_8 | GPIO_Pin_2;	
	GPIO_InitStruct.Pin = GPIO_Pin_8;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);									//PC2 在1000V系统上做AD使用


	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;									//0x00;
	GPIO_InitStruct.Pull = 0x00;
	GPIO_InitStruct.Speed = 0x02;											//50M速率
	GPIO_InitStruct.Pin = GPIO_Pin_11 | GPIO_Pin_9;			
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_Pin_13;  
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_Pin_8;  
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);   
}

void BSP_vGpioInit(void)
{
	unsigned long   PinVal;
	/***********************************************************
	PD	0x2048
		GPIO_Pin_3	((uint16_t)0x0008)	  UART4 收发控制
	***********************************************************/
	PinVal = 0;
	PinVal = GPIO_Pin_3;
	GPIO_InitStruct.Pin = PinVal;			//0x2048
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;	//0x01;	输出推挽
	GPIO_InitStruct.Pull = 0x01;			//上拉！
	GPIO_InitStruct.Speed = 0x02;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


	/***********************************************************
	PG	0x11c0
		GPIO_Pin_7                 ((uint16_t)0x0080)  BUZZER
		GPIO_Pin_6                 ((uint16_t)0x0040)  LED_CTL
	    GPIO_Pin_10					UART5 Ctrl
	***********************************************************/
	PinVal = 0;
	PinVal = GPIO_Pin_7 | GPIO_Pin_6| GPIO_Pin_10 ;
	GPIO_InitStruct.Pin =PinVal;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/***********************************************************
	PE	0x18
		GPIO_Pin_4                 ((uint16_t)0x0010)	LEN
		GPIO_Pin_3                 ((uint16_t)0x0008)   LRST
		GPIO_Pin_2					SPI2_NSS
	
	***********************************************************/
	PinVal = 0;
	PinVal = GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2;
	GPIO_InitStruct.Pin = PinVal; 
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); 

	/***********************************************************
	PF	0x0400
	GPIO_Pin_10;	 UART5 收发转换
	GPIO_Pin_8		 CPWM2
	GPIO_Pin_7		 LED_alm
	***********************************************************/
	GPIO_InitStruct.Pin = GPIO_Pin_10;
	GPIO_InitStruct.Pin |= GPIO_Pin_7;		//LED_alm
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/***********************************************************
	PC	0x04
		GPIO_Pin_2                 ((uint16_t)0x0004)  I2C_WP
		GPIO_Pin_8					CPWM1
		低电平有效
	***********************************************************/
	GPIO_InitStruct.Pin = GPIO_Pin_2;
	GPIO_InitStruct.Pin |= GPIO_Pin_3;			////Relay1
	//TXY TEST
	GPIO_InitStruct.Pin |= GPIO_Pin_8;			//CPWM1  扩展板DO
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/***********************************************************
	PB	0x04
		GPIO_Pin_2                 BOOT1				
		PWM1 PWM2 的片选端 低电平有效
		GPIO_Pin_12					SPI2_NSS
	***********************************************************/
	GPIO_InitStruct.Pin = GPIO_Pin_2 | GPIO_Pin_12;
	//SPI3333333333333
	//GPIO_InitStruct.Pin = GPIO_Pin_2 | GPIO_Pin_12 | GPIO_PIN_5;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//PA15   SPI3_NSS!!
	GPIO_InitStruct.Pin = GPIO_Pin_15; 
	GPIO_InitStruct.Pin |= GPIO_Pin_3;			//Relay2
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);   
/***********************************END		OUTPUT	***************************************************/


/***********************************START	INPUT	**************************************************
************************************START	INPUT	**************************************************
**********************************************************************************************************/

	/***********************************************************
	PE	0x04
		GPIO_Pin_6                 ((uint16_t)0x0040)  SDI4
		GPIO_Pin_5					悬空
	***********************************************************/
	GPIO_InitStruct.Pin = GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;	//0x00;
	GPIO_InitStruct.Pull = 0x00;
	GPIO_InitStruct.Speed = 0x02;//50M速率
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/***********************************************************
	PB
		//TXY
		GPIO_Pin_11				FANLAM1 
		GPIO_Pin_1				SPI3_INT
	***********************************************************/
	PinVal = 0;
	PinVal = GPIO_Pin_11 |GPIO_Pin_1;
	GPIO_InitStruct.Pin = PinVal;  
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/***********************************************************
	PG	0x8400
		GPIO_Pin_8		SW1
		GPIO_Pin_12		DI8
		GPIO_Pin_15		SW2
	***********************************************************/
	PinVal = 0;
	PinVal = GPIO_Pin_15 | GPIO_Pin_12 | GPIO_Pin_8;
	GPIO_InitStruct.Pin = PinVal;  
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/***********************************************************
	PF	0x800
		GPIO_Pin_11                ((uint16_t)0x0800)  DI1
		GPIO_Pin_9                 ((uint16_t)0x0200)  DI3
		GPIO_Pin_6			悬空
	***********************************************************/
	GPIO_InitStruct.Pin = GPIO_Pin_11 | GPIO_Pin_9 | GPIO_Pin_6;  
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/***********************************************************
	PC	0x2000
		GPIO_Pin_13                ((uint16_t)0x2000) SSD2	DI2
		GPIO_Pin_9					新增为					DI6
		GPIO_Pin_14					暂时类似悬空
		GPIO_Pin_15					暂时类似悬空
		GPIO_Pin_0					悬空
	***********************************************************/
	GPIO_InitStruct.Pin = GPIO_Pin_13 | GPIO_Pin_9 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0;  
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);   

	/***********************************************************
	PD	
		GPIO_Pin_13                新增为DI7
	***********************************************************/
	GPIO_InitStruct.Pin = GPIO_Pin_13;  
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/***********************************************************
	PA	
		GPIO_Pin_8                新增为DI5
	***********************************************************/
	GPIO_InitStruct.Pin = GPIO_Pin_8;  
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);   
}

GPIO_PinState BSP_u8GetPFOStatus(void)
{
	return HAL_GPIO_ReadPin(GPIOG,0x0400);
}

void BSP_vFeedDogLed(void)
{
	HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_6);//0x40
}

void BSP_vBuzzer_On(void)
{
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_SET);//0x80
}
void BSP_vBuzzer_Off(void)
{
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_RESET);//0x80
}

void BSP_vR_LED_OFF(void)
{
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7,GPIO_PIN_RESET);
}
void BSP_vR_LED_ON(void)
{
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7,GPIO_PIN_SET);
}
void TESTXXXXXXX(int iPort, int pinNo, int val)
{
	if (0 == iPort)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,(GPIO_PinState)val);
	}

	
	if (2 == iPort)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,(GPIO_PinState)val);
	}
}

GPIO_PinState BSP_u8GetLcdStatus(void)
{
	return HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2);
}

GPIO_PinState BSP_u8GetDI1Status(void)
{
	return HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_11);//0x0800
}

GPIO_PinState BSP_u8GetDI2Status(void)
{
	return HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13);//0x2000
}

GPIO_PinState BSP_u8GetDI3Status(void)
{
	if(u8Is1000VHardwareA00())
	{
		return HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6);//A04  从PF9 变为 PD6  
	}
	else
	{
		return HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_9);//0x2000
	}
}

GPIO_PinState BSP_u8GetDI4Status(void)
{
	return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6);//0x2000
}

GPIO_PinState BSP_u8GetDI5Status(void)
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);//0x2000
}

GPIO_PinState BSP_u8GetDI6Status(void)
{
	return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9);//0x2000
}

GPIO_PinState BSP_u8GetDI7Status(void)
{
	return HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13);//0x2000
}

GPIO_PinState BSP_u8GetDI8Status(void)
{
	return HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_12);//0x2000
}



GPIO_PinState BSP_u8GetFAN_ALM1Status(void)
{
	return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
}

GPIO_PinState BSP_u8GetFAN_ALM2Status(void)
{
	return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5);
}

GPIO_PinState BSP_u8GetFAN_ALM3Status(void)
{
	return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);
}

GPIO_PinState BSP_u8GetFAN_ALM4Status(void)
{
	return HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6);
}

GPIO_PinState BSP_u8GetFAN_ALM5Status(void)
{
	return HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_7);
}

GPIO_PinState BSP_u8GetFAN_ALM6Status(void)
{
	return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
}

GPIO_PinState BSP_u8GetSSD5Status(void)
{
	return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
}

GPIO_PinState BSP_u8GetSD55Status(void)
{
	return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);
}

void BSP_WDGInit(void)
{
	IwdgHandle.Instance = IWDG;
	//IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
	IwdgHandle.Init.Prescaler = IWDG_PRESCALER_128;
	IwdgHandle.Init.Reload    = 0xFFF;//1600;
	HAL_IWDG_Init(&IwdgHandle);
	HAL_IWDG_Start(&IwdgHandle);
}
void BSP_WDGRefresh(void)
{
	HAL_IWDG_Refresh(&IwdgHandle);
}