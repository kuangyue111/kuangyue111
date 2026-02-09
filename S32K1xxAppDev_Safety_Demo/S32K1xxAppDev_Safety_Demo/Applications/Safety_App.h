/*
 * Safety_App.h
 *
 *  Created on: May 8, 2019
 *      Author: B33124
 */

#ifndef SAFETY_APP_H_
#define SAFETY_APP_H_

#include "SafetyLib.h"

void Task_SafetyDiagnostic(void *pvParameters);

extern uint32_t wdogRefreshDisable;
extern char *faultSourceString[];
extern char *resetSourceString[];


#endif /* SAFETY_APP_H_ */
