/*
 * Safety_SCST.h
 *
 *  Created on: Jul 18, 2019
 *      Author: B33124
 */

#ifndef SAFETYLIB_SAFETY_SCST_H_
#define SAFETYLIB_SAFETY_SCST_H_

#include "cpu.h"

#include "m4_scst_typedefs.h"
#include"m4_scst_test_shell.h"

extern uint32_t bEnableScstRun;
extern uint32_t cntScstInterrupted;
extern uint32_t cntScstCompleted;

uint32_t SCST_RunTestSeries(void);
void SCST_RunPorTest(void);
uint32_t SCST_RunTestSingle(void);
void SCST_FaultCallback(void);
void SCST_SetStartNum(uint32_t nStart);
void SCST_SetEndNum(uint32_t nEnd);
uint32_t SCST_GetStartNum(void);
uint32_t SCST_GetEndNum(void);
uint32_t SCST_GetTestResult(void);
uint32_t SCST_GetSignature(void);
void SCST_SetCurrentTest(uint32_t numTest);
void SCST_Enable(void);
void SCST_Disable(void);

#endif /* SAFETYLIB_SAFETY_SCST_H_ */
