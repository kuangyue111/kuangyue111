/*
 * CAN_Communication.c
 *
 *  Created on: Apr 29, 2019
 *      Author: nxa07657
 */

#include "CAN_Communication.h"

uint32_t CAN_RX_BufferIndex = 0;/*buffer index for CAN RX*/
QueueHandle_t CAN_RX_Queue;/*queue used for message exchange between CAN RX ISR and task*/
can_message_t CAN_RX_Message_Buffer[CAN_RX_Buffer_MAX];/*circle buffer for CAN RX*/

void Task_CAN_Communication(void *pvParameters)
{
	/* CAN 2.0 A/B Buffer configuration */
	 can_buff_config_t CAN_buffConfig = {
	  .enableFD = false,
	  .enableBRS = false,
	  .fdPadding = 0xCC,
	  .idType = CAN_MSG_ID_STD,
	  .isRemote = false
	 };

	can_message_t CAN_TX_Buffer ={
			.id = 0x111,
			.data = "12345678",
			.length = 8
	};/*used for CAN message transmit test*/

	CAN_Init(&can_pal1_instance, &can_pal1_Config0);/*initialize the CAN module*/

	/*
	 * add your CAN transceiver control configuration here if needed
	 * for example, toggle the EN/STANDBY pin of TJA1043/44 to enable the transceiver or
	 * let it enter normal work mode
	 */

	/*set the CAN0 interrupt priority lower than FreeRTOS kernel interrupt priority(1)*/
	INT_SYS_SetPriority(CAN0_ORed_IRQn, 3);   		/*set the interrupt priority of CAN0 OR'ed [Bus Off OR Transmit Warning OR Receive Warning]*/
	INT_SYS_SetPriority(CAN0_Error_IRQn, 3);		/*set the interrupt priority of CAN0 Interrupt indicating that errors were detected on the CAN bus*/
	INT_SYS_SetPriority(CAN0_Wake_Up_IRQn, 3);		/*set the interrupt priority of CAN0 Interrupt asserted when Pretended Networking operation is enabled, \
	and a valid message matches the selected filter criteria during Low Power mode*/
	INT_SYS_SetPriority(CAN0_ORed_0_15_MB_IRQn, 3); /*set the interrupt priority of CAN0 OR'ed Message buffer (0-15)*/
	INT_SYS_SetPriority(CAN0_ORed_16_31_MB_IRQn, 3);/*set the interrupt priority of CAN0 OR'ed Message buffer (16-31)*/

	CAN_ConfigTxBuff(&can_pal1_instance,TX_MB, &CAN_buffConfig);/*configure the transmit MB*/

	/*create a queue for UART1 RX message transmit between ISR and task*/
	CAN_RX_Queue = xQueueCreate(CAN_RX_Buffer_MAX,CAN_RX_Buffer_SIZE);

	CAN_SendBlocking(&can_pal1_instance, TX_MB, &CAN_TX_Buffer, 1000);/*send a message for test at first*/

	CAN_ConfigRxBuff(&can_pal1_instance,RX_MB, &CAN_buffConfig, RX_ACK_ID);			 /*configure the receive MB*/
	CAN_Receive(&can_pal1_instance,RX_MB,&CAN_RX_Message_Buffer[CAN_RX_BufferIndex]);/*start the CAN message receive*/

	CAN_InstallEventCallback(&can_pal1_instance,CAN_TX_RX_Callback, NULL);			/*install the TX/RX call back*/

	CAN_SendBlocking(&can_pal1_instance, TX_MB, &CAN_TX_Buffer, 1000);				/*send it back via CAN*/

	while(1)
	{
		/*check whether the queue data of CAN RX is available, and receive it into CAN TX buffer*/
		if(pdTRUE==xQueueReceive(CAN_RX_Queue,&CAN_TX_Buffer,10))
		{
			CAN_SendBlocking(&can_pal1_instance, TX_MB, &CAN_TX_Buffer, 1000);/*send it back via CAN*/
		}
	}
}

/********************************************************************************
 * END OF THE FILE
 * *****************************************************************************/

