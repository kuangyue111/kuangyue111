/*
 * GPIO_IRQ_App.c
 *
 *  Created on: Apr 29, 2019
 *      Author: nxa07657
 */

#include "GPIO_IRQ_App.h"

semaphore_t SW2_Int_Sema;/*semaphore used for SW2 IRQ interrupt*/
semaphore_t SW3_Int_Sema;/*semaphore used for SW2 IRQ interrupt*/
uint32_t ledRGBValue[3] = {255 * 2, 255 * 2, 255 * 2};

/****************************************************************************************
 * the GPIO IRQ interrupt ISR for user button input
 ****************************************************************************************/
void GPIO_IRQ_Interrupt_ISR(void)
{
	uint32_t Interrupt_Flag = 0;

	/* read the USER_SW_PORT interrupt Flags */
	Interrupt_Flag = PINS_DRV_GetPortIntFlag(USER_SW_PORT);

	if(Interrupt_Flag&SW2_PIN)/*if the interrupt happened*/
	{
		OSIF_SemaPost(&SW2_Int_Sema);/*post the semaphore */
	}

	if(Interrupt_Flag&SW3_PIN)/*if the interrupt happened*/
	{
		OSIF_SemaPost(&SW3_Int_Sema);/*post the semaphore */
	}

	/*
	 * add other Port E pin's interrupt handle here if it's enabled and used
	 */

	PINS_DRV_ClearPortIntFlagCmd(USER_SW_PORT);/*clear all the interrupt flags of USER_SW_PORT*/

}


/*******************************************************************************************
* S32K144 EVB GPIO application task
* Get the user buttons(SW2/3) input via IRQ interrupt and change the RGB LED ON/OFF/TOGGLE
*******************************************************************************************/
#if 0
void Task_RGB_LED_Ctrl(void *pvParameters)
{
	uint8_t LED_Ctrl_Cnt = 0;
	uint8_t SW3_Act_Cnt = 0;

	/*****************************************************************************************************
	 * the semaphore, mutex and queue used by ISR or callback must be created before enable the interrupt
	 * to avoid RTOS memory management error/conflict
	 ****************************************************************************************************/
	OSIF_SemaCreate(&SW2_Int_Sema, 0);/*create a semaphore for SW2 GPIO IRQ ISR callback*/
	OSIF_SemaCreate(&SW3_Int_Sema, 0);/*create a semaphore for SW3 GPIO IRQ ISR callback*/

	/*
	 * install the ISR for user buttons input IRQ interrupt
	 */
	INT_SYS_InstallHandler(USER_SW_GPIO_IRQn,GPIO_IRQ_Interrupt_ISR,NULL);
	/*
	 * set the interrupt priority
	 */
	INT_SYS_SetPriority(USER_SW_GPIO_IRQn, 3);

	/*
	 * enable the IRQ interrupt
	 */
	INT_SYS_EnableIRQ(USER_SW_GPIO_IRQn);


	while(1)
	{
		LED_Ctrl_Cnt++;/*increase the counter*/

		if(STATUS_SUCCESS==OSIF_SemaWait(SW2_Int_Sema,1))/*check if SW2 pressed*/
		{
			if(LED_Ctrl_Cnt%2==0)
				RGB_BLUE_LED_Toggle();
			if(LED_Ctrl_Cnt%4==0)
				RGB_RED_LED_Toggle();
			if(LED_Ctrl_Cnt%8==0)
				RGB_GREEN_LED_Toggle();

			UART_Print("user button SW2 has been pressed!\n");

		}
		else
		{
			RGB_BLUE_LED_Off();
			RGB_GREEN_LED_Off();

			RGB_RED_LED_Toggle();
		}

		if(STATUS_SUCCESS==OSIF_SemaWait(SW3_Int_Sema,1))/*check if SW2 pressed*/
		{
			SW3_Act_Cnt++;

			UART_Print("user button SW3 has been pressed!\n");

		}

		if(SW3_Act_Cnt%2)
			OSIF_TimeDelay(200);/*delay 200ms*/
		else
			OSIF_TimeDelay(400);/*delay 400ms*/
	}

}
#endif
void Task_RGB_LED_Ctrl(void *pvParameters)
{
	uint8_t LED_Ctrl_Cnt = 0;

	FTM0->CNT = 0;
	FTM0->MOD = 10000;
	FTM0->CONTROLS[0].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK;		//ch0 = RED
	FTM0->CONTROLS[0].CnV = 200;
	FTM0->CONTROLS[1].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK;		//ch1 = GREEN
	FTM0->CONTROLS[1].CnV = 200;
	FTM0->CONTROLS[2].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK;		//ch2 = BLUE
	FTM0->CONTROLS[2].CnV = 200;
	FTM0->CNTIN = 0;
	FTM0->SC = FTM_SC_PWMEN0_MASK | FTM_SC_PWMEN1_MASK | FTM_SC_PWMEN2_MASK | FTM_SC_CLKS(1);
	PORTD->PCR[0] = PORT_PCR_MUX(2);
	PORTD->PCR[15] = PORT_PCR_MUX(2);
	PORTD->PCR[16] = PORT_PCR_MUX(2);

	while(1)
	{
		LED_Ctrl_Cnt++;/*increase the counter*/
		FTM0->CONTROLS[0].CnV = ledRGBValue[0];
		FTM0->CONTROLS[1].CnV = ledRGBValue[1];
		FTM0->CONTROLS[2].CnV = ledRGBValue[2];
		OSIF_TimeDelay(200);/*delay 200ms*/
	}

}
/********************************************************************************
 * END OF THE FILE
 * *****************************************************************************/

