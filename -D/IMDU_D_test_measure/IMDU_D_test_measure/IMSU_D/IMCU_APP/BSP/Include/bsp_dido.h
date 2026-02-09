
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_DIDO_H
#define __BSP_DIDO_H
#include "main.h"
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
//TXY
#include "stm32f4xx_hal_rcc.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

#define DISLCD_LRST    GPIOE->BSRRH = GPIO_PIN_3 
#define ENLCD_LRST     GPIOE->BSRRL = GPIO_PIN_3
#define ENLCD_BL       GPIOE->BSRRH = GPIO_PIN_4
#define DISLCD_BL      GPIOE->BSRRL = GPIO_PIN_4

typedef enum 
{
	NEWDO1 = 0,//对应丝印BUSA   端口J6DN1
	NEWDO2 = 1,//对应丝印BUSB	端口J6DN3
	NEWDO3 = 2,//对应丝印LOCKA  端口J12UP12
	NEWDO4 = 3,
	NEWDO5 = 4,
	NEWDO6 = 5,	 //锁开关
	NEWDO7 = 6,	 //方向
	NEWDO8 = 7,
	NEWDO9 = 8, 
	NEWDO10 = 9,
	NEWDO11 = 10,//A BMS  24 12伏切换
	NEWDO12 = 11,//B BMS  24 12伏切换
	NEWDO13 = 12,
	NEWDO14 = 13,
	NEWDO15 = 14,
	NEWDO16 = 15, 

}NEWDoChannel_TypeDef;


typedef enum 
{
  DO1 = 0,
  DO2 = 1,
  DO3 = 2,
  DO4 = 3,
  DO5 = 4,
  DO6 = 5, 
  DO7 = 6, 
  DO8 = 7,   
}DoChannel_TypeDef;
typedef enum 
{
  DO_OFF = 0,
  DO_ON = 1,
}DoStatus_TypeDef;

typedef enum 
{
  LVD1 = 6, 
  LVD2 = 7,   
}LvdChannel_TypeDef;

typedef enum 
{
  LVD_OFF = 0,
  LVD_ON = 1,
}LvdStatus_TypeDef;

void BSP_LvdCtrl(LvdChannel_TypeDef u8LvdChannel,LvdStatus_TypeDef u8LvdStatus);
void BSP_InitGpioF(void);
//TXY
void HAL_GPIO_EXTI_5_Callback(uint16_t GPIO_Pin);
void BSP_vR_LED_ON(void);
void BSP_vR_LED_OFF(void);
void BSP_vGpioEXIT1Init(void);
void BSP_DO_Ctrl(NEWDoChannel_TypeDef iDONo, DoStatus_TypeDef u8DoStatus);
void BSP_vGpioInit(void);
void BSP_vGpioReInit(void);
void BSP_vCLOCKInit(void);
void BSP_1000VClockInit(void);
GPIO_PinState u8GetPFOStatus(void);
GPIO_PinState BSP_u8GetPFOStatus(void);
void BSP_vFeedDogLed(void);
void BSP_vBuzzer_On(void);
void BSP_vBuzzer_Off(void);
GPIO_PinState BSP_u8GetLcdStatus(void);
GPIO_PinState BSP_u8GetDI1Status(void);	//DI 1应急开关
GPIO_PinState BSP_u8GetDI2Status(void);//电子锁A 反馈
GPIO_PinState BSP_u8GetDI3Status(void);//防雷
GPIO_PinState BSP_u8GetDI4Status(void);//A枪  BMS 电源反馈
GPIO_PinState BSP_u8GetDI5Status(void);//充电枪 温度告警
GPIO_PinState BSP_u8GetDI6Status(void);	//DI6 电子锁 B 反馈
GPIO_PinState BSP_u8GetDI7Status(void);	//DI7 门禁
GPIO_PinState BSP_u8GetDI8Status(void);	//DI8 B 枪 BMS 电源反馈

GPIO_PinState BSP_u8GetFAN_ALM1Status(void);
GPIO_PinState BSP_u8GetFAN_ALM2Status(void);
GPIO_PinState BSP_u8GetFAN_ALM3Status(void);
GPIO_PinState BSP_u8GetFAN_ALM4Status(void);
GPIO_PinState BSP_u8GetFAN_ALM5Status(void);
GPIO_PinState BSP_u8GetFAN_ALM6Status(void);
GPIO_PinState BSP_u8GetSSD5Status(void);
GPIO_PinState BSP_u8GetSD55Status(void);

void TESTXXXXXXX(int iPort, int pinNo, int val);
void BSP_WDGInit(void);
void BSP_WDGRefresh(void);
#endif