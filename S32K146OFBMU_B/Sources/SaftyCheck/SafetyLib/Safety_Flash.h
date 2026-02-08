/*
 * Safety_Flash.h
 *
 *  Created on: Jun 27, 2019
 *      Author: B33124
 */

#ifndef SAFETYLIB_SAFETY_FLASH_H_
#define SAFETYLIB_SAFETY_FLASH_H_

#include "cpu.h"
void FTFC_Fault_IRQHandler(void);
void FTFC_SetErrorAddr(uint32_t errAddr);
uint32_t FTFC_GetErrorAddr(void);
void FTFC_EccSelfTest(void);
uint8_t GetFlashSelfTestStatus(void);

#endif /* SAFETYLIB_SAFETY_FLASH_H_ */
