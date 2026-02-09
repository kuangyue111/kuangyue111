/*
 * diagnostics.h
 *
 *  Created on: Oct 26, 2018
 *      Author: nxf44615
 */

#ifndef DIAGNOSTICS_H_
#define DIAGNOSTICS_H_

#include "Cpu.h"
#include "FS65_driver/sbc_fs65_common.h"

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/

/* Print SBC status register to console. */
void printSbcStatus(void);

/* Print MUX_OUT values to console. */
void printAmuxMeasurement(void);

/* Activates the LDT timer. */
status_t sbcTestLdt(void);

/* MCU specific function. Saves ADC resolution (e.g. 4095 for 12-bit ADC). */
extern status_t getAdcResolution(uint16_t* resolution);

/* MCU specific function. Executes ADC conversion and saves result to the adcRawValue. */
extern void runAmuxConversion(uint16_t* adcRawValue);

#endif /* DIAGNOSTICS_H_ */
