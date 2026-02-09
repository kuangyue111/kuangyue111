/*
 * ADC_App.h
 *
 *  Created on: Apr 29, 2019
 *      Author: nxa07657
 */

#ifndef ADC_APP_H_
#define ADC_APP_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "Cpu.h"

#include "BSP_Config.h"
#include "App_Config.h"

/*******************************************************************************
* Constants and macros
*******************************************************************************/


/*******************************************************************************
* Global function prototypes
*******************************************************************************/

void Task_ADC_Mesurement(void *pvParameters);


/*******************************************************************************
* Global variables declare
*******************************************************************************/

/*semaphore used for ADC0 group convert finished interrupt notification*/
extern semaphore_t ADC0_ConvertComplete_Sema;
/*semaphore used for ADC0 group convert finished interrupt notification*/
extern semaphore_t ADC1_ConvertComplete_Sema;

#endif /* ADC_APP_H_ */

/********************************************************************************
 * END OF THE FILE
 * *****************************************************************************/

