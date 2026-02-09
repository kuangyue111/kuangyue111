/* 
 * Copyright (c) 2015 - 2016 , Freescale Semiconductor, Inc.                             
 * Copyright 2016-2017 NXP                                                                    
 * All rights reserved.                                                                  
 *                                                                                       
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.                          
 */

/* ###################################################################
**     Filename    : main.c
**     Project     : BMU01_NFS
**     Processor   : S32K144_100
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-10-31, 16:12, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"

#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */

#include "BoardDriver.h"

extern void rtos_start(void);
#define PEX_RTOS_START rtos_start

extern can_message_t recvMsg;
extern can_buff_config_t buffCfg;
extern uint8_t numChans;
extern uint8_t numBuffer;
extern uint8_t master_send[BUFFER_SIZE];

void TimeDelay(uint16_t xms)
{
	uint16_t x,y;
    for(x=xms;x>0;x--)
    {
    	for(y=255;y>0;y--);
    }
}

/*! 
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - __start (startup asm routine)
 * - __init_hardware()
 * - main()
 *   - PE_low_level_init()
 *     - Common_Init()
 *     - Peripherals_Init()
*/
int main(void)
{
	/* Write your local variable definition here */
	status_t status;
	uint64_t ftm1Resolution;
	uint64_t ftm3Resolution;
	uint64_t lpitResolution;
	uint64_t lptmrResolution;
	uint8_t channel = ic_pal1_ChnConfig->hwChannelId;
	uint8_t selectedGroupIndex = 0u; /* Select the index of a SW triggered group of conversions (see ADC PAL component) */
	uint8_t i;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                 /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

    prvSetupHardware();

  /*-----------------------------------------------------------
        Testing Code for BMU board
  -----------------------------------------------------------*/
  /* Send a welcome message */
#if 0
    PCS_RS485_CTR_SEND;
    UART_SendData(&uart_pal1_instance, (uint8_t *)welcomeMsg, strlen(welcomeMsg));
    TimeDelay(10000);
    PCS_RS485_CTR_RECEIVE;
#endif
#if 0
    LCD_RS485_CTR_SEND;
    UART_SendData(&uart_pal2_instance, (uint8_t *)welcomeMsg, strlen(welcomeMsg));
    TimeDelay(10000);
    LCD_RS485_CTR_RECEIVE;
#endif
#if 1
    EMS_RS485_CTR_SEND;
    UART_SendData(&uart_pal3_instance, (uint8_t *)welcomeMsg, strlen(welcomeMsg));
    TimeDelay(10000);
    EMS_RS485_CTR_RECEIVE;
#endif
    /* Get tick resolution in nanosecond unit for TIMING over FTM1 */
    TIMING_GetResolution(&timing_pal1_instance, TIMER_RESOLUTION_TYPE_NANOSECOND, &ftm1Resolution);
    /* Get tick resolution in nanosecond unit for TIMING over FTM3 */
    TIMING_GetResolution(&timing_pal2_instance, TIMER_RESOLUTION_TYPE_NANOSECOND, &ftm3Resolution);
	/* Get tick resolution in nanosecond unit for TIMING over LPIT */
	TIMING_GetResolution(&timing_pal3_instance, TIMER_RESOLUTION_TYPE_NANOSECOND, &lpitResolution);
	/* Get tick resolution in nanosecond unit for TIMING over LPTMR */
	TIMING_GetResolution(&timing_pal4_instance, TIMER_RESOLUTION_TYPE_NANOSECOND, &lptmrResolution);

	/* Start FTM1 channel 0 counting with the period is 1 microsecond,
     the period in tick = the period in nanosecond / FTM1 tick resolution in nanosecond */
	//TIMING_StartChannel(&timing_pal1_instance, FTM1_CHANNEL, PERIOD_BY_1MS/ftm1Resolution);
	/* Start FTM3 channel 0 counting with the period is 0.1 microsecond,
	   the period in tick = the period in nanosecond / FTM3 tick resolution nanosecond*/
	//TIMING_StartChannel(&timing_pal2_instance, FTM3_CHANNEL, PERIOD_BY_01MS/ftm3Resolution);
    /* Start LPIT channel 0 counting with the period is 1 second,
       the period in tick = the period in nanosecond / LPIT tick resolution in nanosecond */
	TIMING_StartChannel(&timing_pal3_instance, LPIT_CHANNEL, PERIOD_BY_NS/lpitResolution);
	/* Start LPTMR channel 0 counting with the period is 1 second,
	   the period in tick = the period in nanosecond / LPTMR tick resolution nanosecond*/
	TIMING_StartChannel(&timing_pal4_instance, LPTMR_CHANNEL, PERIOD_BY_NS/lptmrResolution);

	/* Configure RX buffer with index RX_MAILBOX */
	//CAN_ConfigRxBuff(&can_pal1_instance, RX_MAILBOX, &buffCfg, RX_MSG_ID);
	//CAN_ConfigRxBuff(&can_pal2_instance, RX_MAILBOX, &buffCfg, RX_MSG_ID);
	CAN_ConfigRxBuff(&can_pal3_instance, RX_MAILBOX, &buffCfg, RX_MSG_ID);

	/* Start the selected SW triggered group of conversions */
	status = ADC_StartGroupConversion(&adc_pal1_instance, selectedGroupIndex);
	DEV_ASSERT(status == STATUS_SUCCESS);
	numChans = adc_pal1_InitConfig0.groupConfigArray[0].numChannels;
	numBuffer = adc_pal1_InitConfig0.groupConfigArray[0].numSetsResultBuffer;

	/* Start the FTM2 channel0 input capture */
	IC_StartChannel(&ic_pal1_instance, channel);

	/* Initialize the data buffer of I2C mater send buffer */
	for (i = 0u; i < I2C_TRANSFER_SIZE; i++)
	{
		master_send[i] = i;
	}

  /*-----------------------------------------------------------
        END Testing Code for BMU board
  -----------------------------------------------------------*/
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
