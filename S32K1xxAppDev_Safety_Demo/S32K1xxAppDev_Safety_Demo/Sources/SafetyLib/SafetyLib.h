/*
 * SafetyLib.h
 *
 *  Created on: Mar 13, 2019
 *      Author: B33124
 */

#ifndef SAFETYLIB_SAFETYLIB_H_
#define SAFETYLIB_SAFETYLIB_H_

typedef enum
{
	STD_OFF = 0,
	STD_ON,
}STD_Type;

typedef enum
{
	PMC_NONE = 0,
	PMC_LVD,
	PMC_LVW,
	PMC_HVD
}PMC_Fault_Type;

#include "Safety_WDOG.h"
#include "Safety_CRC.h"
#include "Safety_MPU.h"
#include "Safety_RCM.h"
#include "Safety_FaultSave.h"
#include "Safety_HardFaultHandler.h"
#include "Safety_PMC.h"
#include "Safety_CMU.h"
#include "Safety_RegCheck.h"
#include "Safety_Safeboot.h"
#include "Safety_SRAM.h"
#include "Safety_Flash.h"
#include "Safety_Callback.h"
#include "Safety_ADC.h"

#include "Safety_SCST.h"

//#include "BSP_Config.h"

void SAFETY_InitialCheck(void);

void SAFETY_SelfTestResultRead(void);

#endif /* SAFETYLIB_SAFETYLIB_H_ */
