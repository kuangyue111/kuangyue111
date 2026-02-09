/* ###################################################################
**     Filename    : main.c
**     Processor   : S32K1xx
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


/* Including necessary module. Cpu.h contains other modules needed for compiling.*/
#include "Cpu.h"

  volatile int exit_code = 0;

/* User includes (#include below this line is not maintained by Processor Expert) */

#include "App_Config.h"
#include "CallBack.h"


TaskHandle_t xTaskAdc;
TaskHandle_t xTaskLed;
TaskHandle_t xTaskCan;
TaskHandle_t xTaskCmd;
TaskHandle_t xTaskSafety;

/*******************************************************************************************
 * create the tasks for Application
 * set the task name, stack size and priority
 * it's user's responsibility to add the application task here to run later
 *******************************************************************************************/
void AppTasks_Create(void)
{
	/*
	 * initialize the LPUART at first for command line support
	 */

	INT_SYS_SetPriority(LPUART1_RxTx_IRQn, 4);/*set the LPUART1 interrupt priority*/

	LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);

	/*
	 * create a mutex for UART print
	 */
	if(STATUS_SUCCESS==OSIF_MutexCreate(&UART_Print_Mutex))
	{
		OSIF_MutexUnlock(&UART_Print_Mutex);  /*if success, unlock the mutex*/
	}

	/***************************************************************************
	 * create application tasks here
	 ***************************************************************************/
	xTaskCreate(Task_ADC_Mesurement, (const char * const)"ADC_Sample", \
			Task_ADC_Mesurement__STACK_SIZE, NULL, \
			Task_ADC_Mesurement_TaskPriority, &xTaskAdc );
	xTaskCreate(Task_RGB_LED_Ctrl, (const char *)"RGB_LED", \
			Task_RGB_LED_Ctrl_STACK_SIZE, NULL, \
			Task_RGB_LED_Ctrl_TaskPriority, &xTaskLed );
	xTaskCreate(Task_CAN_Communication, (const char *)"CAN_Com",\
			Task_CAN_COM_STACK_SIZE, NULL, \
			Task_CAN_Com_TaskPriority, &xTaskCan );
	xTaskCreate(vCommandConsoleTask, (const char *)"CmdConsole", \
			Task_CommandConsole_STACK_SIZE, NULL, \
			Task_CommandConsole_TaskPriority, &xTaskCmd );
	xTaskCreate(Task_SafetyDiagnostic, (const char *)"SafetyDiagnostic", \
			Task_SafetyDiagnostic_STACK_SIZE, NULL, \
			Task_SafetyDiagnostic_TaskPriority, &xTaskSafety );

}

/*! 
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */

    MCU_SystemInit();     /* basic clock and pin function initialization */
    AppTasks_Create();    /* create Application tasks */
    vTaskStartScheduler();/* Starts the RTOS scheduler, and run the applications.*/

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
**     for the NXP S32K series of microcontrollers.
**
** ###################################################################
*/
