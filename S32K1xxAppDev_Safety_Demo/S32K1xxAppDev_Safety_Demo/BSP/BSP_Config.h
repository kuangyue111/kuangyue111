/*******************************************************************************
*
* Copyright 2006-2015 Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
****************************************************************************//*!
*
* @file     BSP_Config.h
*
* @date     March-28-2017
*
* @brief    MCU Peripherals Configuration
*
*******************************************************************************/
#ifndef BSP_CONFIG_H_
#define BSP_CONFIG_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include <string.h>

#include "Cpu.h"

/* FreeRTOS Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/*******************************************************************************
* Constants and macros
*******************************************************************************/

/**********************************************************************
 * enable this macro to enable debug print
 * comment it to disable the debug print
 **********************************************************************/
#define DEBUG_PRINT_ENABLE 1

/********************************************************************************
 * the mutex for UART print peripheral lock
 * user use define it before use it
 * recommend to create the mutex in AppTasks_Create() after LPUART initialization
 ********************************************************************************/
 extern mutex_t UART_Print_Mutex;

/*********************************************************************
 * the RGB LED PORT, GPIO and PIN definition according S32K144-EVB
 *********************************************************************/
#define LED_PORT    PORTD
#define LED_GPIO    PTD
#define BLUE_PIN    (1<<0)
#define RED_PIN     (1<<15)
#define GREEN_PIN   (1<<16)

typedef enum{
	BLUE = 0,
	RED = 15,
	GREEN = 16
} RGB_LED_Type;

#define RGB_BLUE_LED_Toggle()   PINS_DRV_TogglePins(LED_GPIO,BLUE_PIN)
#define RGB_RED_LED_Toggle()    PINS_DRV_TogglePins(LED_GPIO,RED_PIN)
#define RGB_GREEN_LED_Toggle()  PINS_DRV_TogglePins(LED_GPIO,GREEN_PIN)

#define RGB_BLUE_LED_On()   PINS_DRV_ClearPins(LED_GPIO,BLUE_PIN)
#define RGB_RED_LED_On()    PINS_DRV_ClearPins(LED_GPIO,RED_PIN)
#define RGB_GREEN_LED_On()  PINS_DRV_ClearPins(LED_GPIO,GREEN_PIN)

#define RGB_BLUE_LED_Off()   PINS_DRV_SetPins(LED_GPIO,BLUE_PIN)
#define RGB_RED_LED_Off()    PINS_DRV_SetPins(LED_GPIO,RED_PIN)
#define RGB_GREEN_LED_Off()  PINS_DRV_SetPins(LED_GPIO,GREEN_PIN)

#define RGB_LED_Ctrl(led, ctrl)  PINS_DRV_WritePin(LED_GPIO,led,ctrl)

/*********************************************************************
 * the user buttons(SW) PORT, GPIO and PIN definition according S32K144-EVB
 *********************************************************************/
#define USER_SW_PORT    PORTC
#define USER_SW_GPIO    PTC
#define SW2_PIN    		(1<<12)
#define SW3_PIN    		(1<<13)

#define USER_SW_GPIO_IRQn   PORTC_IRQn

/*********************************************************************
 * Definition of power modes indexes, as configured in Power Manager Component
 *  Refer to the Reference Manual for details about the power modes.
 *********************************************************************/
#define HSRUN (0u) /* High speed run      */
#define RUN   (1u) /* Run                 */
#define VLPR  (2u) /* Very low power run  */
#define STOP1 (3u) /* Stop option 1       */
#define STOP2 (4u) /* Stop option 2       */
#define VLPS  (5u) /* Very low power stop */


/*******************************************************************************
* Global function prototypes
*******************************************************************************/

void MCU_SystemInit(void);
void McuClockConfig(void);
void McuPowerConfig(void);
void McuIntConfig(void);
void McuPinsConfig(void);
void McuCacheConfig(void);
void McuDmaConfig(void);
void McuMpuInit(void);
void McuFlashPrefetchConfig(void);

/*the route used for debug print via OpenSDA virtual UART*/
status_t UART_Print(char *string);

#endif /* BSP_CONFIG_H_ */
