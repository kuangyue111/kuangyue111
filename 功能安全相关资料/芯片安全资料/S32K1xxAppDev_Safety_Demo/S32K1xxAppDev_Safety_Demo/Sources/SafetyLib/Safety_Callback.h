/*
 * Safety_Callback.h
 *
 *  Created on: Jun 20, 2019
 *      Author: B33124
 */

#ifndef SAFETYLIB_SAFETY_CALLBACK_H_
#define SAFETYLIB_SAFETY_CALLBACK_H_

#if defined (__cplusplus)
extern "C" {
#endif

#include "cpu.h"
#include "Safety_PMC.h"

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

void Safety_PmcFault_Callback(PMC_Fault_Type pmcFault);

void Safety_Safeboot_Callback(void);

void Safety_RegCheck_Callback(void);

void Safety_MPUFault_Callback(void);

void Safety_FlashFault_Callback(void);

void Safety_FlashProtection_Callback(void);

#if defined (__cplusplus)
}
#endif


#endif /* SAFETYLIB_SAFETY_CALLBACK_H_ */
