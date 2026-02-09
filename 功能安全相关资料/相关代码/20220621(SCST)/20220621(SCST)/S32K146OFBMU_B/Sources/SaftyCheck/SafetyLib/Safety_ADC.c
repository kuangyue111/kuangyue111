/*
 * Safety_ADC.c
 *
 *  Created on: 2021年9月7日
 *      Author: zsy342
 */

#include "adc1_pal.h"
#include "adc2_pal.h"
#include "McuSelfCheck.h"
#include "BSPADC.h"
#include "Safety_ADC.h"

uint32_t adc0SelfTestFlag = 0;
uint32_t adc1SelfTestFlag = 0;

uint8_t AdcSelfTestStatus = 0;	/*0:正常   1：异常*/

extern const adc_instance_t adc1_pal_instance;
extern const adc_instance_t adc2_pal_instance;

/*ADC0初始化结构体*/
extern adc_converter_config_t Adc0InitConfig;

uint16_t adc_converter_tempData[3] =
{
		0,/*BANDGAP*/
		0,/*VREFSH*/
		0 /*VREFSL*/
};

uint32_t  ADCtimerCounter = 0;
/**************************************************************************************************
 * Sample ADC internal reference channels and external reference channel to validate ADC operation.
 * Conduct the following 5 samples and compare the results with expected value:
 * VREFL
 * external channel
 * VREFH
 * external channel
 * VREFL
 **************************************************************************************************/
void ADC_PorSelfTest(void)
{
#if 0
	uint32_t ret = 0;
	uint32_t i;

	// ADC0 self-test: sample the 6 channels: Bandgap, VREFL, ch12, VREFH, ch12, VREFL;
	adc0SelfTestFlag = 0x55AA55AA;
	ADC_Init(&adc1_pal_instance, &adc_pal1_InitConfig0);
	ADC_StartGroupConversion(&adc1_pal_instance, 0/* groupIdx */);
	//wait for ADC result ready
	for(i=0; i<1000; i++)
	{
		if(adc0SelfTestFlag == 0)
			break;
	}
	//if( (bandgap < 0.9V) || (bandgap > 1.1V) )
	if( (adc_pal1_Results00[1] < 737) || (adc_pal1_Results00[1] > 901) )
			ret ++;
	if(adc_pal1_Results00[3] > 10)
		ret ++;
	if( (adc_pal1_Results00[4] < (2048-100)) || (adc_pal1_Results00[4] > (2048+100)) )
		ret ++;
	if( adc_pal1_Results00[5] < (4096-10) )
		ret ++;
	if( (adc_pal1_Results00[6] < (2048-100)) || (adc_pal1_Results00[4] > (2048+100)) )
		ret ++;
	if(adc_pal1_Results00[7] > 10)
		ret ++;
	if( ret == 0 )
//		selfTestStatus |= SELFTEST_PASS_ADC0;		//bit1 of selfTestStatus indicate ADC selftest passed
		AdcselfTestStatus = 1;
	else
//		selfTestStatus &= ~SELFTEST_PASS_ADC0;
		AdcselfTestStatus = 0;

	ADC_Deinit(&adc1_pal_instance);

	// ADC1 self-test: sample the 6 channels: Bandgap, VREFL, ch12, VREFH, ch12, VREFL;
	adc1SelfTestFlag = 0x55AA55AA;
	ADC_Init(&adc2_pal_instance, &adc_pal2_InitConfig0);
	ADC_StartGroupConversion(&adc2_pal_instance, 0/* groupIdx */);
	//wait for ADC result ready
	for(i=0; i<1000; i++)
	{
		if(adc1SelfTestFlag == 0)
			break;
	}
	//if( (bandgap < 0.9V) || (bandgap > 1.1V) )
	if( (adc_pal2_Results00[1] < 737) || (adc_pal2_Results00[1] > 901) )
			ret ++;
	if(adc_pal2_Results00[3] > 10)
		ret ++;
	if( (adc_pal2_Results00[4] < (2048-2046)) || (adc_pal2_Results00[4] > (2048+2046)) )
		ret ++;
	if( adc_pal2_Results00[5] < (4096-10) )
		ret ++;
	if( (adc_pal2_Results00[6] < (2048-2046)) || (adc_pal2_Results00[4] > (2048+2046)) )
		ret ++;
	if(adc_pal2_Results00[7] > 10)
		ret ++;
	if( ret == 0 )
//		selfTestStatus |= SELFTEST_PASS_ADC0;		//bit1 of selfTestStatus indicate ADC selftest passed
		AdcselfTestStatus = 1;
	else
//		selfTestStatus &= ~SELFTEST_PASS_ADC0;
		AdcselfTestStatus = 0;

	ADC_Deinit(&adc2_pal_instance);

#endif
	uint32_t i = 0;
	uint32_t ret = 0;

	adc_chan_config_t AdcChanConfig;		/*ADC通道初始化,ADC0与ADC1通道数一样*/


    ADC_DRV_ConfigConverter(eADC_MOD0, &Adc0InitConfig);/*ADC0初始化*/
    //ADC_DRV_GetConverterConfig(module, &watchtemp0);
    ADC_DRV_AutoCalibration(eADC_MOD0);

    //通道转换
    //ADCchan = ADCChanChangeToChannelx(channel);

    AdcChanConfig.interruptEnable = false;  /*该ADC通道中断使能*/
    AdcChanConfig.channel = (adc_inputchannel_t)ADC_INPUTCHAN_BANDGAP;/*选择ADC通道*/

    ADC_DRV_ConfigChan(eADC_MOD0, 0, &AdcChanConfig);

	/*等待AD转换完成*/
	ADC_DRV_WaitConvDone(eADC_MOD0);
	/*获取AD值*/
	//ADC_DRV_GetChanResult(module, channel, &tempData);
	ADC_DRV_GetChanResult((uint32_t)eADC_MOD0, 0, (uint16_t *)&adc_converter_tempData[0]);


    AdcChanConfig.interruptEnable = false;  /*该ADC通道中断使能*/
    AdcChanConfig.channel = (adc_inputchannel_t)ADC_INPUTCHAN_VREFSH;/*选择ADC通道*/

    ADC_DRV_ConfigChan(eADC_MOD0, 0, &AdcChanConfig);

	/*等待AD转换完成*/
	ADC_DRV_WaitConvDone(eADC_MOD0);
	/*获取AD值*/
	//ADC_DRV_GetChanResult(module, channel, &tempData);
	ADC_DRV_GetChanResult((uint32_t)eADC_MOD0, 0, (uint16_t *)&adc_converter_tempData[1]);


    AdcChanConfig.interruptEnable = false;  /*该ADC通道中断使能*/
    AdcChanConfig.channel = (adc_inputchannel_t)ADC_INPUTCHAN_VREFSL;/*选择ADC通道*/

    ADC_DRV_ConfigChan(eADC_MOD0, 0, &AdcChanConfig);

	/*等待AD转换完成*/
	ADC_DRV_WaitConvDone(eADC_MOD0);
	/*获取AD值*/
	//ADC_DRV_GetChanResult(module, channel, &tempData);
	ADC_DRV_GetChanResult((uint32_t)eADC_MOD0, 0, (uint16_t *)&adc_converter_tempData[2]);

	for(i = 0; i < 3; i++)
	{
		adc_converter_tempData[i] = adc_converter_tempData[i] * 3300 / 4096;
	}

	//if( (bandgap < 0.9V) || (bandgap > 1.1V) )
	if( (adc_converter_tempData[0] < 900) || (adc_converter_tempData[0] > 1100) )
	{
		ret ++;
	}

	//if( (VREFSH < 3.2V) )
	if (adc_converter_tempData[1] < (3300-100))
	{
		ret ++;
	}

	//if( (VREFSL > 0.01V) )
	if (adc_converter_tempData[2] > 10)
	{
		ret ++;
	}

	if(ret == 0)
	{
		AdcSelfTestStatus = 0;

		ADCtimerCounter++;
		if(ADCtimerCounter > 20)
		{
			ADCtimerCounter = 30;
			AdcSelfTestStatus = 0;
		}
	}
	else
	{
		AdcSelfTestStatus = 1;
	}

//	ADC_Deinit(&adc1_pal_instance);
	/*S32K146的两个ADC模块共用BANDGAP、VREFSH、VREFSL，因此只需要测试第一个ADC*/
}

uint8_t GetAdcSelfTestStatus(void)
{
	return(AdcSelfTestStatus);
}


