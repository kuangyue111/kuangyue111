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

/* User includes. */
#include "BoardDriver.h"

/*-----------------------------------------------------------
     Global variables
-----------------------------------------------------------*/
/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

extern uint8_t PCS_rx_buffer[BUFFER_SIZE];
extern uint8_t PCS_rx_bufferIdx;
extern uint8_t LCD_rx_buffer[BUFFER_SIZE];
extern uint8_t LCD_rx_bufferIdx;
extern uint8_t EMS_rx_buffer[BUFFER_SIZE];
extern uint8_t EMS_rx_bufferIdx;

extern uint8_t CANtestData;
extern can_buff_config_t buffCfg;
extern can_message_t recvMsg1, recvMsg2, recvMsg3;
extern can_message_t message;

extern uint8_t masterTxBuffer[I2C_TRANSFER_SIZE];
extern uint8_t masterRxBuffer[I2C_TRANSFER_SIZE];

extern uint8_t master_send[SPI_BUFFER_SIZE];
extern uint8_t master_receive[SPI_BUFFER_SIZE];
extern uint8_t slave_send[SPI_BUFFER_SIZE];
extern uint8_t slave_receive[SPI_BUFFER_SIZE];

/*-----------------------------------------------------------
    Local function declaration
-----------------------------------------------------------*/
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );
//static void uartEchoTask( void *pvParameters );
static void CANTxTask( void *pvParameters );
static void CANRxTask( void *pvParameters );
static void I2CMasterTask( void *pvParameters );

//extern void prvSetupHardware( void );

/*FUNCTION**********************************************************************
 *
 * Function Name : prvQueueSendTask
 * Description   : first test task - send the message to receive task.
 *
 *END**************************************************************************/
static void prvQueueSendTask( void *pvParameters )
{
TickType_t xNextWakeTime;
const unsigned long ulValueToSend = 100UL;

	/* Casting pvParameters to void because it is unused */
	(void)pvParameters;

	/* Initialise xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();

	for( ;; )
	{
		/* Place this task in the blocked state until it is time to run again.
		The block time is specified in ticks, the constant used converts ticks
		to ms.  While in the Blocked state this task will not consume any CPU
		time. */
		vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS );

		/* Send to the queue - causing the queue receive task to unblock and
		toggle an LED.  0 is used as the block time so the sending operation
		will not block - it shouldn't need to block as the queue should always
		be empty at this point in the code. */
		xQueueSend( xQueue, &ulValueToSend, mainDONT_BLOCK );
	}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : prvQueueReceiveTask
 * Description   : Second test task - receive the message to send task.
 *
 *END**************************************************************************/
static void prvQueueReceiveTask( void *pvParameters )
{
unsigned long ulReceivedValue;

	/* Casting pvParameters to void because it is unused */
	(void)pvParameters;

	for( ;; )
	{
		/* Wait until something arrives in the queue - this task will block
		indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
		FreeRTOSConfig.h. */
		xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

		/*  To get here something must have been received from the queue, but
		is it the expected value?  If it is, toggle the LED. */
		if( ulReceivedValue == 100UL )
		{
	        /* Reset WDG PAL counter */
	        //WDG_Refresh(&wdg_pal1_Instance);
	        /* Reset EWM PAL counter */
	        //WDG_Refresh(&wdg_pal2_Instance);
#ifdef EVB
	        /* Test Code: Toggle LED_RED */
	        PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_RED));
#endif
			PINS_DRV_TogglePins(WDG_LED_GPIO_PORT, (1 << WDG_LED));
		}
	}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uartEchoTask
 * Description   : test the uart port driver.
 *
 *END**************************************************************************/

static void uartEchoTask( void *pvParameters )
{
	/* Casting pvParameters to void because it is unused */
	(void)pvParameters;
	uint32_t bytesRemaining = 0;
	status_t status;

	for( ;; )
	{
#if 0
		UART_ReceiveData(&uart_pal1_instance, PCS_rx_buffer, 1U);
	    /* Wait for transfer to be completed */
	    status = UART_GetReceiveStatus(&uart_pal1_instance, &bytesRemaining);

		if(PCS_rx_bufferIdx > 1)
		{
			if (PCS_rx_buffer[PCS_rx_bufferIdx] == '\n')
			{
				/* Replace end of line character with null */
				PCS_rx_buffer[PCS_rx_bufferIdx] = 0U;

				//PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_GREEN));
				PINS_DRV_TogglePins(WDG_LED_GPIO_PORT, (1 << WDG_LED));

				//strcpy((char *)PCS_rx_buffer, "Hello World!\n");
				/* Send the received data back */
				//UART_SendData(PCS_uart_instance, PCS_rx_buffer, strlen((char *) PCS_rx_buffer));
				/* Reset the buffer index to start a new reception */
				PCS_rx_bufferIdx = 0U;
			}
		}
#endif
#if 0
		UART_ReceiveData(&uart_pal2_instance, LCD_rx_buffer, 1U);
	    /* Wait for transfer to be completed */
	    status = UART_GetReceiveStatus(&uart_pal2_instance, &bytesRemaining);

		if(LCD_rx_bufferIdx > 1)
		{
			if (LCD_rx_buffer[LCD_rx_bufferIdx] == '\n')
			{
				/* Replace end of line character with null */
				LCD_rx_buffer[LCD_rx_bufferIdx] = 0U;

				//PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_GREEN));
				PINS_DRV_TogglePins(WDG_LED_GPIO_PORT, (1 << WDG_LED));

				//strcpy((char *)PCS_rx_buffer, "Hello World!\n");
				/* Send the received data back */
				//UART_SendData(PCS_uart_instance, PCS_rx_buffer, strlen((char *) PCS_rx_buffer));
				/* Reset the buffer index to start a new reception */
				LCD_rx_bufferIdx = 0U;
			}
		}
	    #endif
#if 1
		UART_ReceiveData(&uart_pal3_instance, EMS_rx_buffer, 1U);
	    /* Wait for transfer to be completed */
	    status = UART_GetReceiveStatus(&uart_pal3_instance, &bytesRemaining);

		if(EMS_rx_bufferIdx > 1)
		{
			if (EMS_rx_buffer[EMS_rx_bufferIdx] == '\n')
			{
				/* Replace end of line character with null */
				EMS_rx_buffer[EMS_rx_bufferIdx] = 0U;

				//PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_GREEN));
				PINS_DRV_TogglePins(WDG_LED_GPIO_PORT, (1 << WDG_LED));

				//strcpy((char *)PCS_rx_buffer, "Hello World!\n");
				/* Send the received data back */
				//UART_SendData(PCS_uart_instance, PCS_rx_buffer, strlen((char *) PCS_rx_buffer));
				/* Reset the buffer index to start a new reception */
				EMS_rx_bufferIdx = 0U;
			}
		}
#endif

		OSIF_TimeDelay(10);
	}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CANETxTask
 * Description   : test the CAN send messages.
 *
 *END**************************************************************************/

static void CANTxTask( void *pvParameters )
{
	/* Casting pvParameters to void because it is unused */
	(void)pvParameters;

	uint32_t count = 0;

	for( ;; )
	{
        count++;
		if (count >= 100)
        {
            count = 0;
#if 0
			/* Configure TX buffer with index TX_MAILBOX*/
            CAN_ConfigTxBuff(&can_pal1_instance, TX_MAILBOX, &buffCfg);
            /* Send the information via CAN */
            CAN_Send(&can_pal1_instance, TX_MAILBOX, &message);
#endif
#if 0
			/* Configure TX buffer with index TX_MAILBOX*/
            CAN_ConfigTxBuff(&can_pal2_instance, TX_MAILBOX, &buffCfg);
            /* Send the information via CAN */
            CAN_Send(&can_pal2_instance, TX_MAILBOX, &message);
#endif
			/* Configure TX buffer with index TX_MAILBOX*/
            CAN_ConfigTxBuff(&can_pal3_instance, TX_MAILBOX, &buffCfg);
            /* Send the information via CAN */
            CAN_Send(&can_pal3_instance, TX_MAILBOX, &message);
        }

		OSIF_TimeDelay(10);
	}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CANERxTask
 * Description   : test the CAN receive messages.
 *
 *END**************************************************************************/

static void CANRxTask( void *pvParameters )
{
	/* Casting pvParameters to void because it is unused */
	(void)pvParameters;

	uint32_t count = 0;
	status_t status;

	for( ;; )
	{
        count++;
#if 0
		/* Start receiving data in RX_MAILBOX. */
        CAN_Receive(&can_pal1_instance, RX_MAILBOX, &recvMsg1);
        status = CAN_GetTransferStatus(&can_pal1_instance, RX_MAILBOX);

		if ((recvMsg1.data[0] == 0xAA)&&(recvMsg1.id == RX_MSG_ID)&&(status == STATUS_SUCCESS))
		{
			PINS_DRV_TogglePins(WDG_LED_GPIO_PORT, (1 << WDG_LED));
			recvMsg1.data[0] = 0;
			count = 0;
		}
#endif
#if 0
        /* Start receiving data in RX_MAILBOX. */
        CAN_Receive(&can_pal2_instance, RX_MAILBOX, &recvMsg2);
        status = CAN_GetTransferStatus(&can_pal2_instance, RX_MAILBOX);

		if ((recvMsg2.data[0] == 0xAA)&&(recvMsg2.id == RX_MSG_ID)&&(status == STATUS_SUCCESS))
		{
			PINS_DRV_TogglePins(WDG_LED_GPIO_PORT, (1 << WDG_LED));
			recvMsg2.data[0] = 0;
			count = 0;
		}
#endif
#if 1
        /* Start receiving data in RX_MAILBOX. */
		CAN_Receive(&can_pal3_instance, RX_MAILBOX, &recvMsg3);
		status = CAN_GetTransferStatus(&can_pal3_instance, RX_MAILBOX);

		if ((recvMsg3.data[0] == 0xAA)&&(recvMsg3.id == RX_MSG_ID)&&(status == STATUS_SUCCESS))
		{
			recvMsg3.data[0] = 0;
			count = 0;
		}
#endif
		OSIF_TimeDelay(10);
	}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2CMasterTask
 * Description   : test the I2C master communication with i2c slave in the other
 *                 board.
 *
 *END**************************************************************************/

static void I2CMasterTask( void *pvParameters )
{
	/* Casting pvParameters to void because it is unused */
	(void)pvParameters;

	uint32_t count = 0;
	//status_t status;

	for( ;; )
	{
        count++;
        /*status = I2C_MasterReceiveData(&i2c1_instance, masterRxBuffer, I2C_TRANSFER_SIZE, true);
		if (status == STATUS_SUCCESS)
		{
			PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_BLUE));
		}
        */
#if 0
		if (count >= 100)
        {
			 /* i2c master sends masterTxBuffer to i2c slave */
			status = I2C_MasterSendData(&i2c1_instance, masterTxBuffer, I2C_TRANSFER_SIZE, true);
			if (status == STATUS_SUCCESS)
			{
				//PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_GREEN));
				PINS_DRV_TogglePins(WDG_LED_GPIO_PORT, (1 << WDG_LED));
			}
			count = 0;
        }
#endif

		OSIF_TimeDelay(10);
	}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPIMasterTask
 * Description   : test the I2C master communication with SPI slave in the other
 *                 board.
 *
 *END**************************************************************************/

static void SPIMasterTask( void *pvParameters )
{
	/* Casting pvParameters to void because it is unused */
	(void)pvParameters;

	uint32_t count = 0;
	status_t status;

	for( ;; )
	{
        count++;
		/* SPI master sends masterTxBuffer to spi slave */
#if 1
        status = SPI_MasterTransfer(&spi1Instance, master_send, master_receive, SPI_NUMBER_OF_FRAMES);
		if (status == STATUS_SUCCESS)
		{
			//PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_GREEN));
			PINS_DRV_TogglePins(WDG_LED_GPIO_PORT, (1 << WDG_LED));
		}
#endif
		OSIF_TimeDelay(10);
	}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : rtos_start
 * Description   : create the task and start the RTOS.
 *
 *END**************************************************************************/
void rtos_start( void )
{
	/* Configure the NVIC, LED outputs and button inputs. */
	//prvSetupHardware();

	/* Create the queue. */
	xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( unsigned long ) );

	if( xQueue != NULL )
	{
		/* Start the two tasks as described in the comments at the top of this
		file. */

		xTaskCreate( prvQueueReceiveTask, "RX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_RECEIVE_TASK_PRIORITY, NULL );
		xTaskCreate( prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );
		//xTaskCreate( uartEchoTask, "uart_echo", configMINIMAL_STACK_SIZE, NULL, UART_PAL_ECHO_TASK_PRIORITY, NULL );
		//xTaskCreate( CANTxTask, "can_tx", configMINIMAL_STACK_SIZE, NULL, CAN_PAL_TX_TASK_PRIORITY, NULL );
		//xTaskCreate( CANRxTask, "can_rx", configMINIMAL_STACK_SIZE, NULL, CAN_PAL_RX_TASK_PRIORITY, NULL );
		//xTaskCreate( I2CMasterTask, "i2c_tx", configMINIMAL_STACK_SIZE, NULL, I2C_PAL_MASTER_TASK_PRIORITY, NULL );
		xTaskCreate( SPIMasterTask, "spi_tx", configMINIMAL_STACK_SIZE, NULL, SPI_PAL_MASTER_TASK_PRIORITY, NULL );

		/* Start the tasks and timer running. */
		vTaskStartScheduler();
	}

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}

/*------------------------------------------------------------------------------
    Hood functions
------------------------------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    volatile size_t xFreeHeapSpace;

	/* This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amount of FreeRTOS heap that
	remains unallocated. */
	xFreeHeapSpace = xPortGetFreeHeapSize();

	if( xFreeHeapSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}

}
/*-----------------------------------------------------------*/

/* A tick hook is used by the "Full" build configuration.  The Full and blinky
build configurations share a FreeRTOSConfig.h header file, so this simple build
configuration also has to define a tick hook - even though it does not actually
use it for anything. */
void vApplicationTickHook( void ) {}

/* The Blinky build configuration does not include run time stats gathering,
however, the Full and Blinky build configurations share a FreeRTOSConfig.h
file.  Therefore, dummy run time stats functions need to be defined to keep the
linker happy. */
void vMainConfigureTimerForRunTimeStats( void ) {}
unsigned long ulMainGetRunTimeCounterValue( void ) { return 0UL; }

/*------------------------END of files-----------------------------------*/
