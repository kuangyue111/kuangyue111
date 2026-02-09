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

/*
  * This simple demo project runs 'stand alone' (without the rest of the tower
 * system) on the Freedom Board or Validation Board, which is populated with a
 * S32K144 Cortex-M4 microcontroller.
 *
 * The idle hook function:
 * The idle hook function demonstrates how to query the amount of FreeRTOS heap
 * space that is remaining (see vApplicationIdleHook() defined in this file).
 *
 * The Queue Send Task:
 * The queue send task is implemented by the prvQueueSendTask() function in
 * this file.  prvQueueSendTask() sits in a loop that causes it to repeatedly
 * block for 200 milliseconds, before sending the value 100 to the queue that
 * was created within main().  Once the value is sent, the task loops back
 * around to block for another 200 milliseconds.
 *
 * The Queue Receive Task:
 * The queue receive task is implemented by the prvQueueReceiveTask() function
 * in this file.  prvQueueReceiveTask() sits in a loop that causes it to
 * repeatedly attempt to read data from the queue that was created within
 * main().  When data is received, the task checks the value of the data, and
 * if the value equals the expected 100, toggles the green LED.  The 'block
 * time' parameter passed to the queue receive function specifies that the task
 * should be held in the Blocked state indefinitely to wait for data to be
 * available on the queue.  The queue receive task will only leave the Blocked
 * state when the queue send task writes to the queue.  As the queue send task
 * writes to the queue every 200 milliseconds, the queue receive task leaves the
 * Blocked state every 200 milliseconds, and therefore toggles the blue LED
 * every 200 milliseconds.
 *
 */

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* SDK includes. */
#include "Cpu.h"
#include "adc_pal_cfg.h"
#include "can_pal_cfg.h"
#include "i2c_pal_cfg.h"
#include "ic_pal_cfg.h"
#include "pwm_pal_cfg.h"
#include "spi_pal_cfg.h"
#include "timing_pal_cfg.h"
#include "uart_pal_cfg.h"

/* User includes. */
#include "BoardDriver.h"

/*-----------------------------------------------------------
     Global variables
-----------------------------------------------------------*/

/* Global Buffer used to receive data from the console */
uint8_t PCS_rx_buffer[BUFFER_SIZE];
uint8_t PCS_rx_bufferIdx = 0;
uint8_t LCD_rx_buffer[BUFFER_SIZE];
uint8_t LCD_rx_bufferIdx = 0;
uint8_t EMS_rx_buffer[BUFFER_SIZE];
uint8_t EMS_rx_bufferIdx = 0;

uint8_t CANtestData = 0x55;
/* Define receive buffer */
can_message_t recvMsg1, recvMsg2, recvMsg3;
/* Set information about the data to be sent
 *  - Standard message ID
 *  - Bit rate switch disabled to use a different bitrate for the data segment
 *  - Flexible data rate disabled
 *  - Use zeros for FD padding
 */
can_buff_config_t buffCfg =  {
    .enableFD = false,
    .enableBRS = false,
    .fdPadding = 0U,
    .idType = CAN_MSG_ID_STD,
    .isRemote = false
};
/* Prepare message to be sent */
can_message_t message = {
    .cs = 0U,
    .id = TX_MSG_ID,
    .data[0] = 0x55,
    .length = 1U
};

/* Define ADC variables for test */
uint8_t numChans = 0;
uint8_t numBuffer = 0;
uint32_t sum[6], VREF;
float avgVolts[6];


/* Declaration of the I2C transfer buffer */
uint8_t masterTxBuffer[I2C_TRANSFER_SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7};
/* Initialization of i2c slave buffers */
uint8_t masterRxBuffer[I2C_TRANSFER_SIZE] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

uint8_t master_send[SPI_BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
uint8_t master_receive[SPI_BUFFER_SIZE];
uint8_t slave_send[SPI_BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
uint8_t slave_receive[SPI_BUFFER_SIZE];

/*-----------------------------------------------------------
    Local function declaration
-----------------------------------------------------------*/
static void boardSetup(void);

/*FUNCTION**********************************************************************
 *
 * Function Name : boardSetup
 * Description   : configure the I/O ports.
 *
 *END**************************************************************************/
static void boardSetup(void)
{
	/* Configure ports */
    PINS_DRV_SetMuxModeSel(PORTB, PCS_RS485_CTR,  PORT_MUX_AS_GPIO);
    PINS_DRV_SetMuxModeSel(PORTE, LCD_RS485_CTR,  PORT_MUX_AS_GPIO);
    PINS_DRV_SetMuxModeSel(PORTD, EMS_RS485_CTR,  PORT_MUX_AS_GPIO);
	/* Change PCS_RS485_CTR to outputs. */
	PINS_DRV_SetPinsDirection(PTB,  (1 << PCS_RS485_CTR));
	/* initial the PCS_RS485 as receive status. */
	PCS_RS485_CTR_RECEIVE;
	/* Change LCD_RS485_CTR to outputs. */
	PINS_DRV_SetPinsDirection(PTE,  (1 << LCD_RS485_CTR));
	/* initial the LCD_RS485 as receive status. */
	LCD_RS485_CTR_RECEIVE;
	/* Change EMS_RS485_CTR to outputs. */
	PINS_DRV_SetPinsDirection(PTD,  (1 << EMS_RS485_CTR));
	/* initial the EMS_RS485 as receive status. */
	EMS_RS485_CTR_RECEIVE;

#ifdef EVB
	/* Set LEDs pin direction as output */
    PINS_DRV_SetPinsDirection(LED_GPIO_PORT, (1 << LED_BLUE) | (1 << LED_RED) | (1 << LED_GREEN));
	/* Turn off LEDs */
	PINS_DRV_SetPins(LED_GPIO_PORT, (1 << LED_BLUE) | (1 << LED_RED) | (1 << LED_GREEN));
#endif
	/* Set Watchdog LED pin direction as output */
    PINS_DRV_SetPinsDirection(WDG_LED_GPIO_PORT, (1 << WDG_LED));
	/* Turn on Watchdog LEDs */
	PINS_DRV_ClearPins(WDG_LED_GPIO_PORT, (1 << WDG_LED));

}

/*FUNCTION**********************************************************************
 *
 * Function Name : prvSetupHardware
 * Description   : Setup the clocks, borad initialization, NVIC.
 *
 *END**************************************************************************/
void prvSetupHardware( void )
{
    /*Local variables*/
	extension_flexio_for_spi_t extension;
	status_t status;

	/* Initialize and configure clocks
     *  -   Setup system clocks, dividers
     *  -   see clock manager component for more details
     */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
     *    -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    boardSetup();

	/* Write configuration for spi4 master using flexio */
	extension.misoPin = 1;
	extension.mosiPin = 0;
	extension.sckPin = 2;
	extension.ssPin = 3;
	spi4_MasterConfig0.extension = &extension;

    /* Initialize SPI0(SBC), SPI1(HV), SPI2(Two Daisys), SPI4(Flash&KEA128) */
    SPI_MasterInit(SBC_SPI0_instance, SBC_SPI0_MasterConfig0);
    SPI_MasterInit(HV_SPI1_instance, HV_SPI1_MasterConfig0);
    SPI_MasterInit(DAISY_SPI2_instance, DAISY_SPI2_MasterConfig0);
    SPI_MasterInit(ExtFLASH_FLEXSPI1_instance, ExtFLASH_FLEXSPI1_MasterConfig0);

    /* Initialize for CAN */
    status = CAN_Init(&can_pal1_instance, &can_pal1_Config0);
    DEV_ASSERT(status == STATUS_SUCCESS);
    status = CAN_Init(&can_pal2_instance, &can_pal2_Config0);
    DEV_ASSERT(status == STATUS_SUCCESS);
    status = CAN_Init(&can_pal3_instance, &can_pal3_Config0);
    DEV_ASSERT(status == STATUS_SUCCESS);

    /* Initialize for I2C */
	I2C_MasterInit(&i2c1_instance, &i2c1_MasterConfig0);

    /* Initialize for input capture */
    /* Setup input capture for FTM 2 channel 0  - PTD10 */
    IC_Init(&ic_pal1_instance, &ic_pal1_InitConfig);

    /* Initialize for PWM output of FTM0 channel 6, start the PWM signal until to deinit */
    PWM_Init(&pwm_pal1Instance, &pwm_pal1Configs);

	/* Initialize TIMING over FTM1 */
    TIMING_Init(&timing_pal1_instance, &timing_pal1_InitConfig);
    /* Initialize TIMING over FTM3 */
    TIMING_Init(&timing_pal2_instance, &timing_pal2_InitConfig);
	/* Initialize TIMING over LPIT */
    TIMING_Init(&timing_pal3_instance, &timing_pal3_InitConfig);
    /* Initialize TIMING over LPTMR */
    TIMING_Init(&timing_pal4_instance, &timing_pal4_InitConfig);

	/* Initialize for ADC */
    DEV_ASSERT(adc_pal1_instance.instIdx == ADC0_INSTANCE);
    status = ADC_Init(&adc_pal1_instance, &adc_pal1_InitConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);
    DEV_ASSERT(adc_pal2_instance.instIdx == ADC1_INSTANCE);
    status = ADC_Init(&adc_pal2_instance, &adc_pal2_InitConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);

    /* Initialize for UART */
    UART_Init(&uart_pal1_instance, &uart_pal1_Config0);
    UART_Init(&uart_pal2_instance, &uart_pal2_Config0);
    UART_Init(&uart_pal3_instance, &uart_pal3_Config0);

    /* Initialize WDG PAL */
    //WDG_Init(&wdg_pal1_Instance, &wdg_pal1_Config0);
    /* Initialize EWM PAL*/
    //WDG_Init(&wdg_pal2_Instance, &wdg_pal2_Config0);
}

/*------------------------------------------------------------------------------
    Interrupt Call Back functions
------------------------------------------------------------------------------*/

/*FUNCTION**********************************************************************
 *
 * Function Name : PCS_rxCallback
 * Description   : UART rx callback for continuous reception, byte by byte.
 *
 *END**************************************************************************/
void PCS_rxCallback(void *driverState, uart_event_t event, void *userData)
{
    /* Unused parameters */
    (void)driverState;
    (void)userData;

    /* Check the event type */
    if (event == UART_EVENT_RX_FULL)
    {
        /* The reception stops when newline is received or the buffer is full */
        if ((PCS_rx_buffer[PCS_rx_bufferIdx] != '\n') && (PCS_rx_bufferIdx != (BUFFER_SIZE - 2U)))
        {
            /* Update the buffer index and the rx buffer */
        	PCS_rx_bufferIdx++;

        	if (PCS_rx_bufferIdx >= BUFFER_SIZE)
        	{
        		PCS_rx_bufferIdx = 0;
        	}
        	else
        	{
        		UART_SetRxBuffer(&uart_pal1_instance, &PCS_rx_buffer[PCS_rx_bufferIdx], 1U);
        	}
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LCD_rxCallback
 * Description   : UART rx callback for continuous reception, byte by byte.
 *
 *END**************************************************************************/
void LCD_rxCallback(void *driverState, uart_event_t event, void *userData)
{
    /* Unused parameters */
    (void)driverState;
    (void)userData;

    /* Check the event type */
    if (event == UART_EVENT_RX_FULL)
    {
        /* The reception stops when newline is received or the buffer is full */
        if ((LCD_rx_buffer[LCD_rx_bufferIdx] != '\n') && (LCD_rx_bufferIdx != (BUFFER_SIZE - 2U)))
        {
            /* Update the buffer index and the rx buffer */
        	LCD_rx_bufferIdx++;

        	if (LCD_rx_bufferIdx >= BUFFER_SIZE)
        	{
        		LCD_rx_bufferIdx = 0;
        	}
        	else
        	{
        		UART_SetRxBuffer(&uart_pal2_instance, &LCD_rx_buffer[LCD_rx_bufferIdx], 1U);
        	}
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EMS_rxCallback
 * Description   : UART rx callback for continuous reception, byte by byte.
 *
 *END**************************************************************************/
void EMS_rxCallback(void *driverState, uart_event_t event, void *userData)
{
    /* Unused parameters */
    (void)driverState;
    (void)userData;

    /* Check the event type */
    if (event == UART_EVENT_RX_FULL)
    {
        /* The reception stops when newline is received or the buffer is full */
        if ((EMS_rx_buffer[EMS_rx_bufferIdx] != '\n') && (EMS_rx_bufferIdx != (BUFFER_SIZE - 2U)))
        {
            /* Update the buffer index and the rx buffer */
        	EMS_rx_bufferIdx++;

        	if (EMS_rx_bufferIdx >= BUFFER_SIZE)
        	{
        		EMS_rx_bufferIdx = 0;
        	}
        	else
        	{
        		UART_SetRxBuffer(&uart_pal3_instance, &EMS_rx_buffer[EMS_rx_bufferIdx], 1U);
        	}
          }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PwmInputCapture_Callback
 * Description   : Input capture callback for pwm detection.
 *
 *END**************************************************************************/
void PwmInputCapture_Callback(ic_event_t event, void * userData)
{
    static uint32_t index = 0U;
	//uint8_t i;

    index++;

    userData = (void*) index;
    (void)userData;
    (void)event;

#ifdef EVB
	/* Test Code: Toggle LED_RED */
    if(index >= 500)
    {
    	PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_RED));
    	index = 0;
        VREF = adc_pal1_Results00[4];

    	/* Results are directly available in resultBuffer associated with the group at initialization */
    	for (i=0; i<numChans; i++)
    	{
    		sum[i] = adc_pal1_Results00[i];
    		avgVolts[i] = ((float)sum[i]/(float)VREF)*(EVB_ADC_VREFH - EVB_ADC_VREFL);
    	}
    }

#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : timing_pal1_FTM1_callBack0
 * Description   : The callback function of timing over FTM1 channel 0.
 *
 *END**************************************************************************/
void timing_pal1_FTM1_callBack0(void *userData)
{
    /* Unused parameters */
    (void)userData;

#ifdef EVB
    /* Toggle LED_BLUE */
	PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_BLUE));
#endif
	PINS_DRV_TogglePins(WDG_LED_GPIO_PORT, (1 << WDG_LED));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : timing_pal2_FTM3_callBack0
 * Description   : The callback function of timing over FTM3 channel 0.
 *
 *END**************************************************************************/

void timing_pal2_FTM3_callBack0(void *userData)
{
    /* Unused parameters */
    (void)userData;

#ifdef EVB
	/* Toggle LED_GREEN */
	PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_GREEN));
#endif
	PINS_DRV_TogglePins(WDG_LED_GPIO_PORT, (1 << WDG_LED));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : timing_pal1_LPTI0_callBack0
 * Description   : The callback function of timing over LPTI0 channel 0.
 *
 *END**************************************************************************/

void timing_pal1_LPTI0_callBack0(void *userData)
{
    /* Unused parameters */
    (void)userData;

#ifdef EVB
	/* Toggle LED_GREEN */
	PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_RED));
#endif
	PINS_DRV_TogglePins(WDG_LED_GPIO_PORT, (1 << WDG_LED));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : timing_pal1_LPTMR0_callBack0
 * Description   : The callback function of timing over LPTMR0 channel 0.
 *
 *END**************************************************************************/

void timing_pal1_LPTMR0_callBack0(void *userData)
{
    /* Unused parameters */
    (void)userData;

#ifdef EVB
	/* Toggle LED_GREEN */
	PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_BLUE));
#endif
	PINS_DRV_TogglePins(WDG_LED_GPIO_PORT, (1 << WDG_LED));
}
/*------------------------END of files-----------------------------------*/
