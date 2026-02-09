/*
 * App_Config.h
 *
 *  Created on: Apr 22, 2019
 *      Author: nxa07657
 */

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

/*the system and BSP header here*/
#include <BSP_Config.h>
#include <stdio.h>
#include "FreeRTOS_CLI.h"
#include "User_Commands.h"

/*the application header file here*/
#include "GPIO_IRQ_App.h"
#include "CAN_Communication.h"
#include "ADC_App.h"
#include "Safety_App.h"

/***************************************************
 * define the application task priority
 * the task priority ranges from 0 to  configMAX_PRIORITIES ¨C 1
 * 0 is the lowest priority
 ***************************************************/
#define Task_ADC_Mesurement_TaskPriority  2
#define Task_RGB_LED_Ctrl_TaskPriority    2
#define Task_CAN_Com_TaskPriority         1
#define Task_CommandConsole_TaskPriority  3
#define Task_SafetyDiagnostic_TaskPriority  4

/***************************************************
 * define the application task stack size
 * the actual task size i s the size*4 Bytes
 ***************************************************/
#define Task_ADC_Mesurement__STACK_SIZE   512
#define Task_RGB_LED_Ctrl_STACK_SIZE      256
#define Task_CAN_COM_STACK_SIZE           256
#define Task_CommandConsole_STACK_SIZE    512
#define Task_SafetyDiagnostic_STACK_SIZE    512

#endif /* APP_CONFIG_H_ */

/********************************************************************************
 * END OF THE FILE
 * *****************************************************************************/
