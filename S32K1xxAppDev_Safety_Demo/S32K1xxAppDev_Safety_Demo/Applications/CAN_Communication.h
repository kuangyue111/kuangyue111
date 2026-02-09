/*
 * CAN_Communication.h
 *
 *  Created on: Apr 29, 2019
 *      Author: nxa07657
 */

#ifndef CAN_COMMUNICATION_H_
#define CAN_COMMUNICATION_H_


/*******************************************************************************
* Includes
*******************************************************************************/
#include "Cpu.h"

#include "CallBack.h"

/*******************************************************************************
* Constants and macros
*******************************************************************************/

/*********************************************************************************************
 *  the configuration for CAN communication
 ********************************************************************************************/
#define TX_MB 		0    /*use MB0 for transmit*/
#define RX_MB 		1	 /*use MB1 for receive*/
#define RX_ACK_ID 0x123  /*the receive message ID is 0x123, due to the mask register is default set as 0xFFFFFFF, which means check every ID bit*/

/*configure the buffer number for CAN RX*/
#define CAN_RX_Buffer_MAX 10

/*configure the buffer size for CAN RX*/
#define CAN_RX_Buffer_SIZE sizeof(can_message_t)

/*the TX buffer size is set as the same of RX buffer for test*/
#define CAN_TX_Buffer_SIZE CAN_RX_Buffer_SIZE


/*******************************************************************************
* Global function prototypes
*******************************************************************************/

void Task_CAN_Communication(void *pvParameters);


/*******************************************************************************
* Global variables declare
*******************************************************************************/

extern uint32_t CAN_RX_BufferIndex;/*buffer index for CAN RX*/
extern QueueHandle_t CAN_RX_Queue;/*queue used for message exchange between CAN RX ISR and task*/
extern can_message_t CAN_RX_Message_Buffer[CAN_RX_Buffer_MAX];/*circle buffer for CAN RX*/

#endif /* CAN_COMMUNICATION_H_ */

/********************************************************************************
 * END OF THE FILE
 * *****************************************************************************/
