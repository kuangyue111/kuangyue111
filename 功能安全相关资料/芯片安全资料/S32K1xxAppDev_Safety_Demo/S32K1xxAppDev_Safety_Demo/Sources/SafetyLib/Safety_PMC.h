/*
 * Safety_PMC.h
 *
 *  Created on: 2019Äê5ÔÂ20ÈÕ
 *      Author: nxf47391
 */

#ifndef SAFETYLIB_SAFETY_PMC_H_
#define SAFETYLIB_SAFETY_PMC_H_

#include "Cpu.h"
#include "SafetyLib.h"

/*! @brief Defines the callback used to be called by ADC PAL after the last conversion result in a group
 * has been copied to the result buffer.
 */
typedef void (* const pmc_callback_t)(PMC_Fault_Type pmcFault);

/*!
 * @brief Defines the configuration structure for an PWCCtr conversion group
 *
 * Implements : adc_group_config_t_Class
 */
typedef struct
{
	pmc_callback_t callback;                      /*!< Callback function associated with group conversion complete */
} pmc_group_config_t;


void PMC_LVDIE_Config(STD_Type On_Off);

void PMC_LVDRE_Config(STD_Type On_Off);

void PMC_LVDF_Clear(void);

void PMC_LVWIE_Config(STD_Type On_Off);

void PMC_LVWF_Clear(void);

void PMC_Init(void);

void PMC_Monitor_Task(void);

#endif /* SAFETYLIB_SAFETY_PMCMONITOR_H_ */
