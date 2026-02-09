/*
 * User_Commands.h
 *
 *  Created on: Apr 27, 2019
 *      Author: nxa07657
 */

#ifndef USER_COMMANDS_H_
#define USER_COMMANDS_H_

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Utils includes. */
#include "FreeRTOS_CLI.h"


void vCommandConsoleTask( void *pvParameters );

#endif /* USER_COMMANDS_H_ */
