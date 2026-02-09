/*
 * ADC_App.c
 *
 *  Created on: Apr 29, 2019
 *      Author: nxa07657
 */

#include "ADC_App.h"

/*semaphore used for ADC0 group convert finished interrupt notification*/
semaphore_t ADC0_ConvertComplete_Sema;
/*semaphore used for ADC0 group convert finished interrupt notification*/
semaphore_t ADC1_ConvertComplete_Sema;

/*******************************************************************************************
 * the ADC test application task
 * trigger the ADC sample and convert, then read the result and send to PC
 *******************************************************************************************/
void Task_ADC_Mesurement(void *pvParameters)
{
	uint8_t i = 0, len = 0;
	uint8_t Output_Str[50];/*use for information print*/
	/*
	* initialize the ADC module  for the application
	*/
	ADC_Init(&adc_pal1_instance, &adc_pal1_InitConfig0);

	INT_SYS_SetPriority(ADC0_IRQn, 4);/*set the ADC0 interrupt priority*/

	/**
	 * if use HW(hardware) trigger mode, ADC0 will use PDB0 interrupt
	 * and ADC1 will use PDB1 corresponding, so it's a must to set their interrupt priority as well
	 */
	INT_SYS_SetPriority(PDB0_IRQn, 4);/*set the PDB0 interrupt priority*/

	/*****************************************************************************************************
	 * the semaphore, mutex and queue used by ISR or callback must be created before enable the interrupt
	 * to avoid RTOS memory management error/conflict
	 ****************************************************************************************************/
	OSIF_SemaCreate(&ADC0_ConvertComplete_Sema, 0);/*create a semaphore for ADC0 ISR callback*/

	while(1)
	{
		//no need to start ADC0 sampling here, because it's already started every 100ms in PMC monitor task
		/*start the ADC0 sample and convert if PTC12 is pressed*/
		if(PINS_DRV_ReadPins(PTC) & (1 << 12))	//push SW2 on EVB to enable ADC ch12 result print
		{
			// ADC sampling is done in PMC_Monitor_Task() function. So just using the ADC result here.
			len = sprintf((char *)Output_Str, "ADC0 ch%d result is: %d.", adc_pal1_ChansArray00[0], adc_pal1_Results00[0]);
			UART_Print((char *)Output_Str);
			for(i=0; i<len; i++)
				Output_Str[i] = 8;			//8=backspace
			UART_Print((char *)Output_Str);
		}
		else
		{
			if(len)
			{
				i =  len;
				Output_Str[i++] = '\r';
				Output_Str[i++] = '\n';
				Output_Str[i++] = '\0';
				UART_Print((char *)Output_Str);
				len = 0;
			}
		}
		OSIF_TimeDelay(200);/*delay 200ms*/

	}

}

/********************************************************************************
 * END OF THE FILE
 * *****************************************************************************/

