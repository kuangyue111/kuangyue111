/*
 * CallBack.h
 *
 *  Created on: Apr 22, 2019
 *      Author: nxa07657
 */

#ifndef CALLBACK_H_
#define CALLBACK_H_

#if defined (__cplusplus)
extern "C" {
#endif

#include "cpu.h"

#include "App_Config.h"

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

extern void CAN_TX_RX_Callback(uint32_t instance, can_event_t eventType, uint32_t objIdx, void *driverState);

#if defined (__cplusplus)
}
#endif


#endif /* CALLBACK_H_ */
