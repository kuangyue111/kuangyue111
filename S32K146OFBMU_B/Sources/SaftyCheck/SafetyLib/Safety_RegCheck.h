/*
 * Safety_RegCheck.h
 *
 *  Created on: Jun 26, 2019
 *      Author: B33124
 */

#ifndef SAFETYLIB_SAFETY_REGCHECK_H_
#define SAFETYLIB_SAFETY_REGCHECK_H_

#include "cpu.h"

extern uint16_t regPortPcrCrcBaseValue;
extern uint16_t regPortPcrCrcRuntimeValue;

uint16_t RegCheck_CrcCalc(void);
void RegCheck_Init(void);
void RegCheck_Compare(uint16_t u16CrcValue);
void RegCheck_EnableHardLock(void);
void RegCheck_DisableDebugMode(void);

uint8_t GetPortSelfTestStatus(void);

#endif /* SAFETYLIB_SAFETY_REGCHECK_H_ */
