#ifndef __BSP_ADC_H
#define __BSP_ADC_H
#include "stm32f4xx_hal.h"

#define ADCx_DMA_IRQHandler	DMA2_Stream0_IRQHandler
#define ADC_FAST_CHANNEL_NUM 2
#define ADC_SLOW_CHANAL_NUM 5
typedef enum 
{
	ADC_CHANNEL_1_FAST	= 0,//快速通道放前面，且数量与ADC_FAST_CHANNEL_NUM关联
	ADC_CHANNEL_2_FAST	= 1,///快速通道放前面，且数量与ADC_FAST_CHANNEL_NUM关联
	ADC_CHANNEL_3_SLOW	= 2,//慢速通道放后面，且数量与ADC_SLOW_CHANAL_NUM 关联
	ADC_CHANNEL_4_SLOW	= 3,
	ADC_CHANNEL_5_SLOW	= 4,
	ADC_CHANNEL_6_SLOW	= 5, 
	ADC_CHANNEL_7_SLOW	= 6, 
	ADC_ALL_CHANAL_NUM   = 7,
}AdcChannel_TypeDef;

int BSP_AdcInit(void);
int BSP_ADC3_Init(void);
int BSP_AdcChannelSelect(ADC_HandleTypeDef *ADC_hande,uint8_t AdcMuxOffset);
int BSP_AdcStart(ADC_HandleTypeDef *ADC_hande,uint32_t* pDataBuff, uint32_t Length);
int BSP_GetAdcVal(uint16_t* u16AdcVal);
void BSP_vAdcMuxSelect(uint8_t u8ChannelVal);
void BSP_AdcStop(ADC_HandleTypeDef *ADC_hande);
#endif