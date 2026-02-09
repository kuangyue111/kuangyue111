/*
 * Safety_MPU.h
 *
 *  Created on: Apr 18, 2019
 *      Author: B33124
 */

#ifndef SAFETYLIB_SAFETY_MPU_H_
#define SAFETYLIB_SAFETY_MPU_H_

#include "cpu.h"

extern mpu_access_err_info_t mpuErrorInfo;
extern uint32_t mpuErrorCheckFlag;

void MPU_Config(void);
void MPU_GlobalEnable(void);
void MPU_GlobalDisable(void);
void MPU_ErrorInjectionTest(void);

#endif /* SAFETYLIB_SAFETY_MPU_H_ */
