/*
 * Safety_CMU.h
 *
 *  Created on: 2019Äê5ÔÂ20ÈÕ
 *      Author: nxf47391
 */

#ifndef SAFETYLIB_SAFETY_CMU_H_
#define SAFETYLIB_SAFETY_CMU_H_

#include "Cpu.h"
//#include "SafetyLib.h"
#include "McuSelfCheck.h"

typedef enum
{
	CMU_Interrupt_On_Error = 0,
	CMU_Reset_On_Error,
}CMU_INT_RST_Type;

void CMU_OSC_Config(STD_Type On_Off);

void CMU_OSC_IERE_Config(CMU_INT_RST_Type IE_RE);

void CMU_SOSC_Error_Clear(void);

void CMU_PLL_Config(STD_Type On_Off);

void CMU_PLL_IERE_Config(CMU_INT_RST_Type IE_RE);

void CMU_SPLL_Error_Clear(void);

void CMU_Init(void);

void CMU_Enable(void);

void CMU_Disable(void);

void CMU_ClockFreqCheck(void);

uint8_t GetCmuSelfTestStatus(void);

#endif /* SAFETYLIB_SAFETY_CLKMONITOR_H_ */
