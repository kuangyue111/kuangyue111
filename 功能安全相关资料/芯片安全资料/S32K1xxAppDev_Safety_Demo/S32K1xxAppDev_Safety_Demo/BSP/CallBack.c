/*
 * CallBack.c
 *
 *  Created on: Apr 22, 2019
 *      Author: nxa07657
 */


/*******************************************************************************
 * User Include
 ******************************************************************************/
#include "CallBack.h"

/* FreeRTOS Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/


void CAN_TX_RX_Callback(uint32_t instance, can_event_t eventType, uint32_t objIdx, void *driverState)
{
	BaseType_t xHigherPriorityTaskWokenByPost;/*used for ISR wake-up task sync mechanism */

	/* We have not woken a task at the start of the ISR. */
	xHigherPriorityTaskWokenByPost = pdFALSE;

	if(CAN_EVENT_RX_COMPLETE==eventType)
	{
		/*post the RX completed message/buffer through the queue at first*/
		xQueueSendFromISR(CAN_RX_Queue,&CAN_RX_Message_Buffer[CAN_RX_BufferIndex++%CAN_RX_Buffer_MAX], &xHigherPriorityTaskWokenByPost);

		/*use next circle buffer for the new CAN message receive*/
		CAN_Receive(&can_pal1_instance,RX_MB,&CAN_RX_Message_Buffer[CAN_RX_BufferIndex%CAN_RX_Buffer_MAX]);
	}

	if(xHigherPriorityTaskWokenByPost)
	{
		taskYIELD();/*force a context switch*/
	}
}

/******************************************************************************
 * EOF
 *****************************************************************************/
