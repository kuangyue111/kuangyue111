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

void printSbcStatus(void);

void printAmuxMeasurement(void);

extern status_t getAdcResolution(uint16_t* resolution);

extern void runAmuxConversion(uint16_t* adcRawValue);

status_t sbcTestLdt(void);

#endif /* DIAGNOSTICS_H_ */
