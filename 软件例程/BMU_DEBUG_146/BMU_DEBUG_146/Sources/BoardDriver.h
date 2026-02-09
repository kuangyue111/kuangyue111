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

#ifndef _BOARD_DEFINES_H_
#define _BOARD_DEFINES_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#include "pins_driver.h"

/*-----------------------------------------------------------
    Macro definition declaration for prototype test
-----------------------------------------------------------*/
//********************************************************************************
/* Priorities at which the tasks are created. */
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define CAN_PAL_TX_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )
#define CAN_PAL_RX_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )
#define UART_PAL_ECHO_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )
#define SPI_PAL_MASTER_TASK_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define I2C_PAL_MASTER_TASK_PRIORITY		( tskIDLE_PRIORITY + 3 )


/* The rate at which data is sent to the queue, specified in milliseconds, and
converted to ticks using the portTICK_PERIOD_MS constant. */
#define mainQUEUE_SEND_FREQUENCY_MS			( 200 / portTICK_PERIOD_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH					( 1 )

/* A block time of zero simply means "don't block". */
#define mainDONT_BLOCK						( 0UL )

#define welcomeMsg "'Hello!'\r\n"

/* Error message displayed at the console, in case data is received erroneously */
#define errorMsg "An error occurred! The application will stop!\r\n"

/* Timeout in ms for blocking operations */
#define TIMEOUT         5000UL
#define BUFFER_SIZE     10UL

/* Use this define to specify if the application runs as master or slave */
#define MASTER
/* #define SLAVE */

/* Definition of the TX and RX message buffers depending on the bus role */
#if defined(MASTER)
    #define TX_MAILBOX    (1UL)
    #define TX_MSG_ID     (1UL)
    #define RX_MAILBOX    (0UL)
    //#define RX_MAILBOX_1  (1UL)
    //#define RX_MAILBOX_2  (2UL)
    //#define RX_MAILBOX_3  (3UL)
    #define RX_MSG_ID     (2UL)
#elif defined(SLAVE)
    #define TX_MAILBOX  (0UL)
    #define TX_MSG_ID   (2UL)
    #define RX_MAILBOX  (1UL)
    #define RX_MSG_ID   (1UL)
#endif

/* This define configures led pin and port for use with EVB, to make it run on
 * FVB you need to comment the following line.
 */
//#define EVB

#ifdef EVB
	#define LED_BLUE  		0U		/* pin PTD0 - RGB LED on DEV-KIT */
	#define LED_RED  		15U		/* pin PTD15 - RGB LED on DEV-KIT */
	#define LED_GREEN  		16U		/* pin PTE16 - RGB LED on DEV-KIT */
	#define LED_GPIO_PORT	PTD		/* GPIO PORT D */
#endif

#define WDG_LED 			11U // watchdog display LED
#define WDG_LED_GPIO_PORT	PTA	// GPIO PORT A

//********************************************************************************
/*-----------------------------------------------------------
          END Definition for prototype test
-----------------------------------------------------------*/

/* Defines the specific ports and pins for Board and Validation Board. */
#define PCS_RS485_CTR   4U  //PTB4
#define LCD_RS485_CTR   9U  //PTE9
#define EMS_RS485_CTR   5U  //PTD5
#define EXT_FLASH_WP    11U  //PTE11

/* Define the specific MACROs of RS485 send and receive control signals. */
#define PCS_RS485_CTR_SEND  	PINS_DRV_SetPins(PTB,  (1 << PCS_RS485_CTR))
#define PCS_RS485_CTR_RECEIVE   PINS_DRV_ClearPins(PTB,  (1 << PCS_RS485_CTR))
#define LCD_RS485_CTR_SEND  	PINS_DRV_SetPins(PTE,  (1 << LCD_RS485_CTR))
#define LCD_RS485_CTR_RECEIVE   PINS_DRV_ClearPins(PTE,  (1 << LCD_RS485_CTR))
#define EMS_RS485_CTR_SEND  	PINS_DRV_SetPins(PTD,  (1 << EMS_RS485_CTR))
#define EMS_RS485_CTR_RECEIVE   PINS_DRV_ClearPins(PTD,  (1 << EMS_RS485_CTR))

/* Define the specific MACROs of external flash control signals. */
#define EXT_FLASH_WP_HIGH  	PINS_DRV_SetPins(PTE,  (1 << EXT_FLASH_WP))
#define EXT_FLASH_WP_LOW    PINS_DRV_ClearPins(PTE,  (1 << EXT_FLASH_WP))

/* Use this define to specify if the application uses LPUART */
#define PCS_uart_instance     &uart_pal1_instance
#define PCS_UART_PAL_CONFIG   &uart_pal1_Config0
#define LCD_uart_instance     &uart_pal2_instance
#define LCD_UART_PAL_CONFIG   &uart_pal2_Config0
#define EMS_uart_instance     &uart_pal3_instance
#define EMS_UART_PAL_CONFIG   &uart_pal3_Config0

/* Use this define to specify if the application uses SPI */
#define SBC_SPI0_instance                 &spi1Instance
#define SBC_SPI0_MasterConfig0            &spi1_MasterConfig0
#define HV_SPI1_instance                  &spi2Instance
#define HV_SPI1_MasterConfig0             &spi2_MasterConfig0
#define DAISY_SPI2_instance               &spi3Instance
#define DAISY_SPI2_MasterConfig0          &spi3_MasterConfig0
#define ExtFLASH_FLEXSPI1_instance        &spi4Instance
#define ExtFLASH_FLEXSPI1_MasterConfig0   &spi4_MasterConfig0

/* Use this define to specify if the application uses CAN */
#define PCS_CAN1_instance                 &can_pal1_instance
#define PCS_CAN1_Config0                  &can_pal1_Config0
#define BSU_CAN2_instance                 &can_pal2_instance
#define BSU_CAN2_Config0                  &can_pal2_Config0
#define CHARGE_CAN3_instance              &can_pal3_instance
#define CHARGE_CAN3_Config0               &can_pal3_Config0

/* Use this define to specify if the application uses I2C */
#define ExtEEPROM_I2C1_instance           &i2c1_instance
#define ExtEEPROM_I2C1_Config0            &i2c1_MasterConfig0

/* Uses ADC PAL config 0 - configured to measure ADC0 InputChannel 0,1,6,7,VREFH. */
#define ADC0_INSTANCE   0UL
#define ADC1_INSTANCE   1UL
#define ADC_VREFH       3.3f
#define ADC_VREFL       0.0f

/* LPIT channel used */
#define LPIT_CHANNEL	    0UL
/* LPTMR channel used */
#define LPTMR_CHANNEL	    0UL
/* FTM1 channel used */
#define FTM1_CHANNEL	    0UL
/* FTM3 channel used */
#define FTM3_CHANNEL	    0UL
/* Period in microsecond unit */
#define PERIOD_BY_1MS	1000000UL /* The period is 1ms for 100ns clock*/
#define PERIOD_BY_01MS	100000UL /* The period is 0.1ms for 100ns clock*/
/* Period in nanosecond unit */
#define PERIOD_BY_NS	1000000000UL /* The period is 1 second */

#ifdef EVB
/* Uses ADC PAL config 0 - configured to measure ADC0 InputChannel 12, connected to the potentiometer. */
#define EVB_NUM_CONV_GROUP_ITERATIONS       10UL
#define EVB_DELAY_BETWEEN_SW_TRIG_GROUPS    1500UL /* [milliseconds] */
#define EVB_ADC_INSTANCE    0UL
#define EVB_ADC_CHN         12U
#define EVB_ADC_VREFH       5.0f
#define EVB_ADC_VREFL       0.0f
#endif

/* i2c Definition of the data transfer size */
#define I2C_TRANSFER_SIZE 8

#define SPI_NUMBER_OF_FRAMES    10U
#define SPI_BUFFER_SIZE         10

/*-----------------------------------------------------------
    Variables declaration
-----------------------------------------------------------*/


/*-----------------------------------------------------------
    Functions declaration
-----------------------------------------------------------*/
void prvSetupHardware( void );

#endif
