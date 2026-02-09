/*
 * Safety_SRAM.h
 *
 *  Created on: Jun 27, 2019
 *      Author: B33124
 */

#ifndef SAFETYLIB_SAFETY_SRAM_H_
#define SAFETYLIB_SAFETY_SRAM_H_

#include "cpu.h"

#define SRAM_L_ECC_ADDR		0x1FFFFFF0
#define SRAM_U_ECC_ADDR		0x20000000

void SRAM_ERM_Init(void);

void SRAM_EccEimTest(void);

void SRAM_EccInject(uint32_t nChan);

void ERM_double_fault_IRQHandler(void);

void LMEM_CacheEnable(void);

void LMEM_CacheDisable(void);

#endif /* SAFETYLIB_SAFETY_SRAM_H_ */
