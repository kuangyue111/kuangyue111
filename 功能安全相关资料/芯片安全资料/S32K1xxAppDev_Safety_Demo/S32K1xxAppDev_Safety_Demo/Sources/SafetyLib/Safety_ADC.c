/*
 * Safety_ADC.c
 *
 *  Created on: Sep 2, 2019
 *      Author: B33124
 */

#include "SafetyLib.h"
uint32_t adc0SelfTestFlag = 0;
uint32_t adc1SelfTestFlag = 0;


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
	uint32_t ret = 0;
	uint32_t i;

	// ADC0 self-test: sample the 6 channels: Bandgap, VREFL, ch12, VREFH, ch12, VREFL;
	adc0SelfTestFlag = 0x55AA55AA;
	ADC_Init(&adc_pal1_instance, &adc_pal1_InitConfig0);
	ADC_StartGroupConversion(&adc_pal1_instance, 0/* groupIdx */);
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
		selfTestStatus |= SELFTEST_PASS_ADC0;		//bit1 of selfTestStatus indicate ADC selftest passed
	else
		selfTestStatus &= ~SELFTEST_PASS_ADC0;

	ADC_Deinit(&adc_pal1_instance);

	// ADC1 self-test: sample the 6 channels: Bandgap, VREFL, ch12, VREFH, ch12, VREFL;
	adc1SelfTestFlag = 0x55AA55AA;
	ADC_Init(&adc_pal2_instance, &adc_pal2_InitConfig0);
	ADC_StartGroupConversion(&adc_pal2_instance, 0/* groupIdx */);
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
		selfTestStatus |= SELFTEST_PASS_ADC1;		//bit1 of selfTestStatus indicate ADC selftest passed
	else
		selfTestStatus &= ~SELFTEST_PASS_ADC1;

	ADC_Deinit(&adc_pal2_instance);
}


/*******************************************************************************
 * the call back of ADC_PAL1 (ADC0 conversion completion interrupt) component ISR for application notification
 ******************************************************************************/
extern semaphore_t ADC0_ConvertComplete_Sema;
void adc_pal1_callback00(const adc_callback_info_t * const callbackInfo, void * userData)
{
	if(adc0SelfTestFlag == 0x55AA55AA)	//if it's ADC self test interrupt
		adc0SelfTestFlag = 0;
	else
		OSIF_SemaPost(&ADC0_ConvertComplete_Sema);/*post the semaphore for notification*/
}

/*******************************************************************************
 * the call back of ADC_PAL2 ((ADC1 conversion completion interrupt)) component ISR for application notification
 ******************************************************************************/
void adc_pal2_callback00(const adc_callback_info_t * const callbackInfo, void * userData)
{
	if(adc1SelfTestFlag == 0x55AA55AA)	//if it's ADC self test interrupt
		adc1SelfTestFlag = 0;
}
